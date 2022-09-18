#include "Inventory.h"

void Inventory::AddItem(AFortPlayerController* PlayerController, UFortItemDefinition* ItemDef, int Count, bool bAddToQuickBar, EFortQuickBars QuickBars, int Slot)
{
	if (!ItemDef || !PlayerController)
		return;

	for (int i = 0; i < PlayerController->WorldInventory->Inventory.ItemInstances.Num(); i++)
	{
		auto ItemInstance = PlayerController->WorldInventory->Inventory.ItemInstances[i];

		if (ItemInstance->GetItemDefinitionBP() == ItemDef)
		{
			if (ItemInstance->GetItemDefinitionBP()->MaxStackSize > (ItemInstance->ItemEntry.Count + Count))
			{
				ItemInstance->ItemEntry.Count += Count;
				PlayerController->WorldInventory->Inventory.MarkItemDirty(ItemInstance->ItemEntry);

				for (int j = 0; j < PlayerController->WorldInventory->Inventory.ReplicatedEntries.Num(); j++)
				{
					auto Entry = &PlayerController->WorldInventory->Inventory.ReplicatedEntries[j];

					if (Entry->ItemDefinition == ItemDef)
					{
						Entry->Count += Count;
						PlayerController->WorldInventory->Inventory.MarkItemDirty(*Entry);

						return;
					}
				}
			}
			else {
				return;
			}
		}
	}

	auto ItemInstance = (UFortWorldItem*)ItemDef->CreateTemporaryItemInstanceBP(1, 0);
	if (ItemInstance)
	{
		ItemInstance->ItemEntry.Count = Count;
		ItemInstance->SetOwningControllerForTemporaryItem(PlayerController);

		PlayerController->WorldInventory->Inventory.ItemInstances.Add(ItemInstance);
		PlayerController->WorldInventory->Inventory.ReplicatedEntries.Add(ItemInstance->ItemEntry);

		if (bAddToQuickBar)
			PlayerController->QuickBars->ServerAddItemInternal(ItemInstance->GetItemGuid(), QuickBars, Slot);
	}
}

void Inventory::Setup(AFortPlayerController* PlayerController)
{
	static auto PickaxeDef = FindObject<UFortItemDefinition>("/Game/Athena/Items/Weapons/WID_Harvest_HalloweenScythe_Athena_C_T01.WID_Harvest_HalloweenScythe_Athena_C_T01");
	static auto WallDef = FindObject<UFortItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall");
	static auto FloorDef = FindObject<UFortItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor");
	static auto StairDef = FindObject<UFortItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_Stair_W.BuildingItemData_Stair_W");
	static auto RoofDef = FindObject<UFortItemDefinition>("/Game/Items/Weapons/BuildingTools/BuildingItemData_RoofS.BuildingItemData_RoofS");
	static auto EditToolDef = FindObject<UFortItemDefinition>("/Game/Items/Weapons/BuildingTools/EditTool.EditTool");

	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Secondary, 0);
	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Secondary, 1);
	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Secondary, 2);
	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Secondary, 3);
	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Secondary, 4);

	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Primary, 0);
	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Primary, 1);
	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Primary, 2);
	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Primary, 3);
	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Primary, 4);
	PlayerController->QuickBars->EnableSlot(EFortQuickBars::Primary, 5);

	AddItem(PlayerController, PickaxeDef, 1, true, EFortQuickBars::Primary, 0);
	AddItem(PlayerController, WallDef, 1, true, EFortQuickBars::Secondary, 0);
	AddItem(PlayerController, FloorDef, 1, true, EFortQuickBars::Secondary, 1);
	AddItem(PlayerController, StairDef, 1, true, EFortQuickBars::Secondary, 2);
	AddItem(PlayerController, RoofDef, 1, true, EFortQuickBars::Secondary, 3);
	AddItem(PlayerController, EditToolDef, 1);

	PlayerController->bHasInitializedWorldInventory = true;
	PlayerController->OverriddenBackpackSize = 5;
}

void Inventory::Update(AFortPlayerController* PlayerController)
{
	PlayerController->HandleWorldInventoryLocalUpdate();

	PlayerController->WorldInventory->HandleInventoryLocalUpdate();
	PlayerController->WorldInventory->bRequiresLocalUpdate = true;
	PlayerController->QuickBars->OnRep_PrimaryQuickBar();
	PlayerController->QuickBars->OnRep_SecondaryQuickBar();
	PlayerController->QuickBars->ForceNetUpdate();
	PlayerController->WorldInventory->ForceNetUpdate();

	PlayerController->WorldInventory->Inventory.MarkArrayDirty();
}

void Inventory::UpdateCount(AFortPlayerController* PlayerController, UFortItemDefinition* ItemDef, int Count)
{
	auto Inventory = PlayerController->WorldInventory->Inventory;

	for (int i = 0; i < Inventory.ItemInstances.Num(); i++)
	{
		auto ItemInstance = Inventory.ItemInstances[i];

		if (ItemInstance->GetItemDefinitionBP() == ItemDef)
		{
			ItemInstance->ItemEntry.Count = Count;
			Inventory.MarkItemDirty(ItemInstance->ItemEntry);
			break;
		}
	}

	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		auto Entry = &Inventory.ReplicatedEntries[i];

		if (Entry->ItemDefinition == ItemDef)
		{
			Entry->Count = Count;
			Inventory.MarkItemDirty(*Entry);
			break;
		}
	}
}
