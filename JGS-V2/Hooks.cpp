#include "Hooks.h"

bool bListening = false;
bool bSetup = false;
bool bSetupFloorLoot = false;

void* Hooks::ProcessEventHook(UObject* pObject, UFunction* pFunction, void* pParams)
{
	auto FuncName = pFunction->GetName();

	if (pFunction->FunctionFlags & 0x01000000 || pFunction->FunctionFlags & 0x00200000 &&
		!FuncName.contains("UpdateCamera") &&
		!FuncName.contains("NoBase"))
	{
		if (!FuncName.contains("ClientAckGoodMove") &&
			!FuncName.contains("ServerMove") &&
			!FuncName.contains("ClientAdjustPosition"))
		{
			if (!FuncName.contains("ServerFireAIDirectorEvent") &&
				!FuncName.contains("ServerTriggerCombatEvent") &&
				!FuncName.contains("ServerFireAIDirectorEventBatch") &&
				!FuncName.contains("ServerTriggerCombatEventBatch"))
			{
				LOG("RPC: {}", FuncName);
			}
		}
	}

	if (FuncName == "ReadyToStartMatch")
	{
		if (!bSetup)
		{
			GetPlayerController(0)->CheatManager->DestroyAll(APlayerController::StaticClass());

			Net::Listen();

			auto GameMode = Cast<AFortGameModeZone>(GetWorld()->AuthorityGameMode);
			auto GameState = Cast<AFortGameStateZone>(GetWorld()->GameState);

			GameMode->GameSession = SpawnActor<AFortGameSession>();
			GameMode->GameSession->MaxPlayers = 100;

			GameState->GameSessionID = L"JGS-GameSession-ID";
			GameState->OnRep_GameSessionID();

			GameMode->DefaultPawnClass = FindObject<UClass>("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");

			GameMode->StartMatch();
			GameMode->StartPlay();

			bSetup = true;
			bListening = true;
		}
	}

	if (FuncName == "HandleStartingNewPlayer" && bListening)
	{
		auto PlayerController = *(AFortPlayerControllerAthena**)(pParams);

		if (PlayerController)
		{
			if (!bSetupFloorLoot)
			{
				Loot::SpawnFloorLoot();
				bSetupFloorLoot = true;
			}

			auto Pawn = SpawnActor<AFortPlayerPawn>(GetPlayerStartLocation(), {}, GetWorld()->AuthorityGameMode->DefaultPawnClass);
			PlayerController->Possess(Pawn);

			Pawn->SetMaxHealth(100);
			Pawn->SetHealth(100);

			auto HealthSet = Pawn->HealthSet;
			HealthSet->CurrentShield.Minimum = 0;
			HealthSet->CurrentShield.Maximum = 100;
			HealthSet->CurrentShield.BaseValue = 100;
			HealthSet->CurrentShield.CurrentValue = 0;

			HealthSet->Shield.Minimum = 0;
			HealthSet->Shield.Maximum = 100;
			HealthSet->Shield.BaseValue = 100;
			HealthSet->Shield.CurrentValue = 0;

			HealthSet->OnRep_Shield();
			HealthSet->OnRep_CurrentShield();

			static auto NoBackpackQuestionMark = FindObject<UCustomCharacterPart>("/Game/Characters/CharacterParts/Backpacks/NoBackpack.NoBackpack");
			static auto HeadPart = FindObject<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
			static auto BodyPart = FindObject<UCustomCharacterPart>("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");
			Pawn->ServerChoosePart(EFortCustomPartType::Head, HeadPart);
			Pawn->ServerChoosePart(EFortCustomPartType::Body, BodyPart);
			Pawn->ServerChoosePart(EFortCustomPartType::Backpack, NoBackpackQuestionMark);

			PlayerController->bHasServerFinishedLoading = true;
			PlayerController->OnRep_bHasServerFinishedLoading();

			auto PlayerState = PlayerController->PlayerState->Cast<AFortPlayerStateAthena>();
			PlayerState->bHasFinishedLoading = true;
			PlayerState->bHasStartedPlaying = true;
			PlayerState->bIsWorldDataOwner = true;
			PlayerState->bIsGameSessionAdmin = true;
			PlayerState->bIsGameSessionOwner = true;

			PlayerState->OnRep_PlayerTeam();

			PlayerState->OnRep_bHasStartedPlaying();

			PlayerState->OnRep_CharacterParts();
			PlayerState->OnRep_PlayerTeam();

			PlayerController->QuickBars = SpawnActor<AFortQuickBars>();
			PlayerController->QuickBars->SetOwner(PlayerController);
			PlayerController->OnRep_QuickBar();

			Inventory::Setup(PlayerController);
			Inventory::Update(PlayerController);
			Abilities::GrantAbilties(Pawn->AbilitySystemComponent);
		}
	}

	if (FuncName == "ServerTryActivateAbility")
	{
		auto Comp = Cast<UAbilitySystemComponent>(pObject);
		auto Params = Cast<UAbilitySystemComponent_ServerTryActivateAbility_Params>(pParams);

		Abilities::InternalServerTryActiveAbility(Comp, Params->AbilityToActivate, Params->InputPressed, Params->PredictionKey, nullptr);

		return NULL;
	}

	if (FuncName == "ServerTryActivateAbilityWithEventData")
	{
		auto Comp = Cast<UAbilitySystemComponent>(pObject);
		auto Params = Cast<UAbilitySystemComponent_ServerTryActivateAbilityWithEventData_Params>(pParams);

		Abilities::InternalServerTryActiveAbility(Comp, Params->AbilityToActivate, Params->InputPressed, Params->PredictionKey, &Params->TriggerEventData);

		return NULL;
	}

	if (FuncName == "ServerAbilityRPCBatch")
	{
		auto Comp = Cast<UAbilitySystemComponent>(pObject);
		auto Params = Cast<UAbilitySystemComponent_ServerAbilityRPCBatch_Params>(pParams);

		Abilities::ServerAbilityRPCBatch_Internal(Comp, Params->BatchInfo);

		return NULL;
	}

	if (FuncName == "ServerReturnToMainMenu")
	{
		auto PlayerController = (AFortPlayerController*)(pObject);
		auto Params = (AFortPlayerController_ServerReturnToMainMenu_Params*)(pParams);

		PlayerController->ClientTravel(L"/Game/Maps/Frontend", ETravelType::TRAVEL_Absolute, false, FGuid());
	}

	if (FuncName == "ServerExecuteInventoryItem")
	{
		auto PlayerController = Cast<AFortPlayerController>(pObject);
		auto Params = Cast<AFortPlayerController_ServerExecuteInventoryItem_Params>(pParams);

		if (!PlayerController)
			goto out;

		auto Items = PlayerController->WorldInventory->Inventory.ItemInstances;
		for (int i = 0; i < Items.Num(); i++)
		{
			auto Item = Items[i];

			if (UKismetGuidLibrary::EqualEqual_GuidGuid(Item->GetItemGuid(), Params->ItemGuid))
			{
				if (!PlayerController->MyFortPawn)
					goto out;

				PlayerController->MyFortPawn->EquipWeaponDefinition(Cast<UFortWeaponItemDefinition>(Item->GetItemDefinitionBP()), Params->ItemGuid);

				if (Item->GetItemDefinitionBP()->IsA(UFortTrapItemDefinition::StaticClass()))
				{
					PlayerController->MyFortPawn->PickUpActor(nullptr, (UFortDecoItemDefinition*)Item->GetItemDefinitionBP());
				}
			}
		}
	}

	if (FuncName == "OnDamageServer")
	{
		if (!pObject->IsA(ABuildingSMActor::StaticClass()))
			goto out;

		auto BuildingActor = Cast<ABuildingSMActor>(pObject);
		auto Params = Cast<ABuildingActor_OnDamageServer_Params>(pParams);

		bool bHitWeakPoint = Params->Damage == 100;

		if (!Params->InstigatedBy->IsA(AFortPlayerController::StaticClass()))
			goto out;

		auto PlayerController = Cast<AFortPlayerController>(Params->InstigatedBy);

		if (PlayerController)
		{
			if (bHitWeakPoint)
			{
				PlayerController->ClientReportDamagedResourceBuilding(BuildingActor, BuildingActor->ResourceType, UKismetMathLibrary::RandomIntegerInRange(4, 9), BuildingActor->bDestroyed, bHitWeakPoint);
			}
			else {
				PlayerController->ClientReportDamagedResourceBuilding(BuildingActor, BuildingActor->ResourceType, UKismetMathLibrary::RandomIntegerInRange(2, 5), BuildingActor->bDestroyed, bHitWeakPoint);
			}
		}
	}

	if (FuncName == "ClientReportDamagedResourceBuilding")
	{
		auto PlayerController = Cast<AFortPlayerController>(pObject);
		auto Params = Cast<AFortPlayerController_ClientReportDamagedResourceBuilding_Params>(pParams);

		if (!PlayerController)
			goto out;

		Inventory::AddItem(PlayerController, Helpers::GetItemDefinitionFromResourceType(Params->PotentialResourceType), Params->PotentialResourceCount);
		Inventory::Update(PlayerController);
	}

	if (FuncName == "ServerAttemptInteract")
	{
		auto PlayerController = Cast<AFortPlayerController>(pObject);
		auto Params = Cast<AFortPlayerController_ServerAttemptInteract_Params>(pParams);

		if (!PlayerController)
			goto out;

		if (Params->ReceivingActor)
		{
			if (Params->ReceivingActor->IsA(ABuildingContainer::StaticClass()))
			{
				Cast<ABuildingContainer>(Params->ReceivingActor)->bAlreadySearched = true;
				Cast<ABuildingContainer>(Params->ReceivingActor)->OnRep_bAlreadySearched();
			}
		}
	}

	if (FuncName == "ServerAttemptAircraftJump")
	{
		auto PlayerController = Cast<AFortPlayerControllerAthena>(pObject);
		auto Params = Cast<AFortPlayerControllerAthena_ServerAttemptAircraftJump_Params>(pParams);

		if (!PlayerController)
			goto out;

		if (PlayerController->Pawn)
			goto out;

		auto Pawn = SpawnActor<AFortPlayerPawn>(Cast<AFortGameStateAthena>(GetWorld()->GameState)->GetAircraft()->K2_GetActorLocation(), {}, GetWorld()->AuthorityGameMode->DefaultPawnClass);
		PlayerController->Possess(Pawn);

		Pawn->SetMaxHealth(100);
		Pawn->SetHealth(100);

		auto HealthSet = Pawn->HealthSet;
		HealthSet->CurrentShield.Minimum = 0;
		HealthSet->CurrentShield.Maximum = 100;
		HealthSet->CurrentShield.BaseValue = 100;
		HealthSet->CurrentShield.CurrentValue = 0;

		HealthSet->Shield.Minimum = 0;
		HealthSet->Shield.Maximum = 100;
		HealthSet->Shield.BaseValue = 100;
		HealthSet->Shield.CurrentValue = 0;

		HealthSet->OnRep_Shield();
		HealthSet->OnRep_CurrentShield();
	}

	if (FuncName == "ServerHandlePickup")
	{
		auto Pawn = Cast<AFortPlayerPawn>(pObject);
		auto Params = Cast<AFortPlayerPawn_ServerHandlePickup_Params>(pParams);

		if (!Pawn)
			goto out;

		if (!Pawn->Controller)
			goto out;

		auto PlayerController = Cast<AFortPlayerController>(Pawn->Controller);
		auto Inventory = PlayerController->WorldInventory->Inventory;
		auto QuickBar = PlayerController->QuickBars->PrimaryQuickBar;

		auto Pickup = Params->Pickup;

		auto ItemGuid = Pickup->PrimaryPickupItemEntry.ItemGuid;
		auto ItemDef = Pickup->PrimaryPickupItemEntry.ItemDefinition;

		if (ItemDef->IsA(UFortResourceItemDefinition::StaticClass()) || ItemDef->IsA(UFortAmmoItemDefinition::StaticClass()))
		{
			Inventory::AddItem(PlayerController, ItemDef, Pickup->PrimaryPickupItemEntry.Count);
			Inventory::Update(PlayerController);

			Pickup->PickupLocationData.PickupTarget = Pawn;
			Pickup->PickupLocationData.FlyTime = 0.75f;
			Pickup->PickupLocationData.ItemOwner = Pawn;
			Pickup->OnRep_PickupLocationData();
			Pickup->OnRep_bPickedUp();

			goto out;
		}

		int SlotToAddTo = 0;

		for (int i = 0; i < QuickBar.Slots.Num(); i++)
		{
			auto Slot = QuickBar.Slots[i];

			if (Slot.Items.IsValidIndex(0))
			{
				if (4 >= i && QuickBar.CurrentFocusedSlot == 0)
				{
					auto Guid = Slot.Items[0];

					for (int j = 0; j < Inventory.ItemInstances.Num(); j++)
					{
						auto ItemInstance = Inventory.ItemInstances[j];

						if (UKismetGuidLibrary::EqualEqual_GuidGuid(ItemInstance->GetItemGuid(), Guid))
						{
							Inventory.ItemInstances.Remove(j);

							for (int k = 0; k < Inventory.ReplicatedEntries.Num(); k++)
							{
								auto Entry = Inventory.ReplicatedEntries[k];

								if (UKismetGuidLibrary::EqualEqual_GuidGuid(Entry.ItemGuid, Guid))
								{
									Inventory.ReplicatedEntries[k];
									Loot::SpawnPickup(Entry.ItemDefinition, Entry.Count, Pawn->K2_GetActorLocation(), {});
								}
							}
						}
					}

					SlotToAddTo = i;
					PlayerController->QuickBars->EmptySlot(EFortQuickBars::Primary, i);
					break;
				}
				else {
					auto Guid = QuickBar.Slots[QuickBar.CurrentFocusedSlot].Items[0];

					for (int j = 0; j < Inventory.ItemInstances.Num(); j++)
					{
						auto ItemInstance = Inventory.ItemInstances[j];

						if (UKismetGuidLibrary::EqualEqual_GuidGuid(ItemInstance->GetItemGuid(), Guid))
						{
							Inventory.ItemInstances.Remove(j);

							for (int k = 0; k < Inventory.ReplicatedEntries.Num(); k++)
							{
								auto Entry = Inventory.ReplicatedEntries[k];

								if (UKismetGuidLibrary::EqualEqual_GuidGuid(Entry.ItemGuid, Guid))
								{
									Inventory.ReplicatedEntries[k];
									Loot::SpawnPickup(Entry.ItemDefinition, Entry.Count, Pawn->K2_GetActorLocation(), {});
								}
							}
						}
					}

					SlotToAddTo = QuickBar.CurrentFocusedSlot;
					PlayerController->QuickBars->EmptySlot(EFortQuickBars::Primary, QuickBar.CurrentFocusedSlot);
					break;
				}
			}
			else {
				SlotToAddTo = i;
				break;
			}
		}

		auto NewItem = Cast<UFortWorldItem>(ItemDef->CreateTemporaryItemInstanceBP(Pickup->PrimaryPickupItemEntry.Count, 1));
		NewItem->ItemEntry = Pickup->PrimaryPickupItemEntry;

		Inventory.ItemInstances.Add(NewItem);
		Inventory.ReplicatedEntries.Add(NewItem->ItemEntry);

		PlayerController->QuickBars->ServerAddItemInternal(NewItem->GetItemGuid(), EFortQuickBars::Primary, SlotToAddTo);

		Pickup->PickupLocationData.PickupTarget = Pawn;
		Pickup->PickupLocationData.FlyTime = 0.75f;
		Pickup->PickupLocationData.ItemOwner = Pawn;
		Pickup->OnRep_PickupLocationData();
		Pickup->OnRep_bPickedUp();

		Inventory::Update(PlayerController);
	}

	if (FuncName == "ServerPlayEmoteItem")
	{
		auto PlayerController = Cast<AFortPlayerController>(pObject);
		auto Params = Cast<AFortPlayerController_ServerPlayEmoteItem_Params>(pParams);

		if (!PlayerController)
			goto out;

		auto FortPawn = PlayerController->MyFortPawn;

		if (!FortPawn)
			goto out;

		auto EmoteAsset = Params->EmoteAsset;
	}

	out:
    return ProcessEvent(pObject, pFunction, pParams);
}

int64_t Hooks::KickPatchHook(int64_t, int64_t)
{
	return 0;
}

void Hooks::TickFlushHook(UNetDriver* NetDriver)
{
	if (NetDriver)
	{
		Replication::ReplicateActors(NetDriver);
	}

	return TickFlush(NetDriver);
}

void Hooks::Init()
{
    CREATEHOOK(GetEngine()->VFT[64], ProcessEventHook, &ProcessEvent);
	CREATEHOOK(GetBaseAddress() + 0x73DAB0, KickPatchHook, nullptr);
	CREATEHOOK(GetBaseAddress() + 0x1AF10F0, TickFlushHook, &TickFlush);
}