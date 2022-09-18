#include "Loot.h"

int Loot::GetWeightForLootSpawn()
{
	bool bEpicOrLeg = UKismetMathLibrary::RandomBoolWithWeight(0.07);

	int Index = 0;

	if (bEpicOrLeg)
	{
		Index = UKismetMathLibrary::RandomIntegerInRange(3, 4);
	}
	else {
		Index = UKismetMathLibrary::RandomIntegerInRange(0, 2);
	}

	return Index;
}

UFortItemDefinition* Loot::GetRandomItemFromRarity(int Rarity)
{
	UFortItemDefinition* RetValue = nullptr;

	switch (Rarity)
	{
	case 0:
		RetValue = CommonWeapons[UKismetMathLibrary::RandomInteger(CommonWeapons.size())];
		break;
	case 1:
		RetValue = UnCommonWeapons[UKismetMathLibrary::RandomInteger(UnCommonWeapons.size())];
		break;
	case 2:
		RetValue = RareWeapons[UKismetMathLibrary::RandomInteger(RareWeapons.size())];
		break;
	case 3:
		RetValue = EpicWeapons[UKismetMathLibrary::RandomInteger(EpicWeapons.size())];
		break;
	case 4:
		RetValue = LegendaryWeapons[UKismetMathLibrary::RandomInteger(LegendaryWeapons.size())];
		break;
	}

	return RetValue;
}

UFortItemDefinition* Loot::GetRandomConsumable()
{
	return Consumables[UKismetMathLibrary::RandomInteger(Consumables.size())];
}

std::vector<UFortItemDefinition*> Loot::GetRandomAmmos()
{
	std::vector<UFortItemDefinition*> RetValue;

	for (int i = 0; i < 2; i++)
	{
		RetValue.push_back(Ammo[UKismetMathLibrary::RandomInteger(Ammo.size())]);
	}

	return RetValue;
}

UFortItemDefinition* Loot::GetRandomResource()
{
	return Resources[UKismetMathLibrary::RandomInteger(Resources.size())];
}

void Loot::SpawnPickup(UFortItemDefinition* ItemDef, int Count, FVector Location, FRotator Rotation)
{
	AFortPickupAthena* Pickup = SpawnActor<AFortPickupAthena>(Location, Rotation);
	if (Pickup)
	{
		Pickup->PrimaryPickupItemEntry.Count = Count;
		Pickup->PrimaryPickupItemEntry.ItemDefinition = ItemDef;
		Pickup->OnRep_PrimaryPickupItemEntry();

		Pickup->TossPickup(Location, nullptr, ItemDef->MaxStackSize, true);
	}
}

void Loot::InitLootTables()
{
	//Gray Weapons
	CommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_C_Ore_T02.WID_Assault_Auto_Athena_C_Ore_T02"));
	CommonWeapons.push_back(FindObject<UFortItemDefinition>("FortniteGame/Content/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_C_Ore_T02.WID_Assault_SemiAuto_Athena_C_Ore_T02"));
	CommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03"));
	CommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_C_Ore_T02.WID_Pistol_SixShooter_Athena_C_Ore_T02"));
	CommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_C_Ore_T02.WID_Pistol_SemiAuto_Athena_C_Ore_T02"));
	CommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_C_Ore_T02.WID_Pistol_AutoHeavy_Athena_C_Ore_T02"));
	CommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_C_Ore_T02.WID_Pistol_AutoHeavySuppressed_Athena_C_Ore_T02"));

	//Green Weapons
	UnCommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03"));
	UnCommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_UC_Ore_T03.WID_Assault_SemiAuto_Athena_UC_Ore_T03"));
	UnCommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_UC_Ore_T03.WID_Pistol_AutoHeavy_Athena_UC_Ore_T03"));
	UnCommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_UC_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_UC_Ore_T03"));
	UnCommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_UC_Ore_T03.WID_Pistol_Scavenger_Athena_UC_Ore_T03"));
	UnCommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_UC_Ore_T03.WID_Pistol_SemiAuto_Athena_UC_Ore_T03"));
	UnCommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_UC_Ore_T03.WID_Pistol_SixShooter_Athena_UC_Ore_T03"));
	UnCommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_UC_Ore_T03.WID_Shotgun_SemiAuto_Athena_UC_Ore_T03"));
	UnCommonWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03"));

	//Blue Weapons
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_R_Ore_T03.WID_Assault_SemiAuto_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Surgical_Athena_R_Ore_T03.WID_Assault_Surgical_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_R_Ore_T03.WID_Launcher_Grenade_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_R_Ore_T03.WID_Launcher_Rocket_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_R_Ore_T03.WID_Pistol_AutoHeavy_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_R_Ore_T03.WID_Pistol_Scavenger_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_R_Ore_T03.WID_Pistol_SemiAuto_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_R_Ore_T03.WID_Pistol_SixShooter_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_R_Ore_T03.WID_Shotgun_SemiAuto_Athena_R_Ore_T03"));
	RareWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03"));

	//Purple Weapons
	EpicWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
	EpicWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_Surgical_Athena_VR_Ore_T03.WID_Assault_Surgical_Athena_VR_Ore_T03"));
	EpicWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_VR_Ore_T03.WID_Launcher_Grenade_Athena_VR_Ore_T03"));
	EpicWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_VR_Ore_T03.WID_Launcher_Rocket_Athena_VR_Ore_T03"));
	EpicWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_VR_Ore_T03.WID_Pistol_Scavenger_Athena_VR_Ore_T03"));
	EpicWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_VR_Ore_T03.WID_Pistol_SemiAuto_Athena_VR_Ore_T03"));
	EpicWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03"));
	EpicWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03"));
	EpicWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_VR_Ore_T03.WID_Sniper_Standard_Scope_Athena_VR_Ore_T03"));

	//Gold Weapons
	LegendaryWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"));
	LegendaryWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_SR_Ore_T03.WID_Launcher_Rocket_Athena_SR_Ore_T03"));
	LegendaryWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_SR_Ore_T03.WID_Pistol_SemiAuto_Athena_SR_Ore_T03"));
	LegendaryWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03"));
	LegendaryWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03"));
	LegendaryWeapons.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_SR_Ore_T03.WID_Sniper_Standard_Scope_Athena_SR_Ore_T03"));

	//Consumables
	Consumables.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Bush/Athena_Bush.Athena_Bush"));
	Consumables.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/DanceGrenade/Athena_DanceGrenade.Athena_DanceGrenade"));
	Consumables.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Grenade/Athena_Grenade.Athena_Grenade"));
	Consumables.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit"));
	Consumables.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/PurpleStuff/Athena_PurpleStuff.Athena_PurpleStuff"));
	Consumables.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields"));
	Consumables.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall"));
	Consumables.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/SuperMedkit/Athena_SuperMedkit.Athena_SuperMedkit"));
	Consumables.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Consumables/SmokeGrenade/Athena_SmokeGrenade.Athena_SmokeGrenade"));

	//Traps
	Traps.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Ceiling_Electric_Single_Athena_R_T03.TID_Ceiling_Electric_Single_Athena_R_T03"));
	Traps.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Ceiling_Falling_Athena.TID_Ceiling_Falling_Athena"));
	Traps.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_ContextTrap_Athena.TID_ContextTrap_Athena"));
	Traps.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Player_Campfire_Athena.TID_Floor_Player_Campfire_Athena"));
	Traps.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Player_Jump_Pad_Athena.TID_Floor_Player_Jump_Pad_Athena"));
	Traps.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Player_Jump_Pad_Free_Direction_Athena.TID_Floor_Player_Jump_Pad_Free_Direction_Athena"));
	Traps.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Player_Launch_Pad_Athena.TID_Floor_Player_Launch_Pad_Athena"));
	Traps.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Floor_Spikes_Athena_R_T03.TID_Floor_Spikes_Athena_R_T03"));
	Traps.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Traps/TID_Wall_Electric_Athena_R_T03.TID_Wall_Electric_Athena_R_T03"));

	//Ammo
	Ammo.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AmmoDataRockets.AmmoDataRockets"));
	Ammo.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy"));
	Ammo.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight"));
	Ammo.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium"));
	Ammo.push_back(FindObject<UFortItemDefinition>("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells"));

	//Resources
	Resources.push_back(FindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData"));
	Resources.push_back(FindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData"));
	Resources.push_back(FindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData"));
}

void Loot::SpawnFloorLoot()
{
	TArray<AActor*> FloorLootActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), UObject::FindClass("BlueprintGeneratedClass Tiered_Athena_FloorLoot_01.Tiered_Athena_FloorLoot_01_C"), &FloorLootActors);

	for (int i = 0; i < FloorLootActors.Num(); i++)
	{
		auto FloorLootActor = FloorLootActors[i];

		bool bIsConsumable = UKismetMathLibrary::RandomBoolWithWeight(0.4);

		if (FloorLootActor)
		{
			auto Location = FloorLootActor->K2_GetActorLocation();
			auto Rotation = FloorLootActor->K2_GetActorRotation();

			if (!bIsConsumable)
			{
				auto WeaponDefinition = Cast<UFortWeaponItemDefinition>(GetRandomItemFromRarity(GetWeightForLootSpawn()));

				if (WeaponDefinition)
				{
					auto AmmoDefinition = WeaponDefinition->GetAmmoWorldItemDefinition_BP();

					SpawnPickup(WeaponDefinition, 1, FloorLootActor->K2_GetActorLocation(), FloorLootActor->K2_GetActorRotation());

					if (AmmoDefinition) {
						SpawnPickup(AmmoDefinition, AmmoDefinition->DropCount, Location, Rotation);
					}
				}
			}
			else {
				auto ConsumableDefinition = Cast<UFortConsumableItemDefinition>(GetRandomConsumable());
				
				if (ConsumableDefinition)
				{
					SpawnPickup(ConsumableDefinition, ConsumableDefinition->DropCount, Location, Rotation);
				}
			}
		}

		if (FloorLootActor)
		{
			FloorLootActor->K2_DestroyActor();
		}
	}
}

void Loot::SpawnLootForChest(ABuildingContainer* Container)
{
	auto Location = Container->K2_GetActorLocation();

	auto WeaponDef = Cast<UFortWeaponItemDefinition>(GetRandomItemFromRarity(GetWeightForLootSpawn()));

	if (WeaponDef)
	{
		auto AmmoDef = WeaponDef->GetAmmoWorldItemDefinition_BP();

		SpawnPickup(WeaponDef, 1, Location, {});

		if (AmmoDef)
		{
			SpawnPickup(AmmoDef, AmmoDef->DropCount, Location, {});
		}
	}

	auto ConsumableDef = Cast<UFortConsumableItemDefinition>(GetRandomConsumable());
	if (ConsumableDef)
	{
		SpawnPickup(ConsumableDef, ConsumableDef->DropCount, Location, {});
	}

	auto ResourceDef = GetRandomResource();
	if (ResourceDef)
	{
		SpawnPickup(ResourceDef, 30, Location, {});
	}
}

void Loot::SpawnLootForAmmoBox(ABuildingContainer* Container)
{
	auto Location = Container->K2_GetActorLocation();

	auto Ammos = GetRandomAmmos();

	for (auto Ammo : Ammos)
	{
		auto AmmoDef = Cast<UFortAmmoItemDefinition>(Ammo);

		if (AmmoDef)
		{
			SpawnPickup(AmmoDef, AmmoDef->DropCount * 3, Location, {});
		}
	}
}
