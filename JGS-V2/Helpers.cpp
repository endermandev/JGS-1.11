#include "Helpers.h"

UFortItemDefinition* Helpers::GetItemDefinitionFromResourceType(EFortResourceType ResourceType)
{
	UFortItemDefinition* RetValue = nullptr;

	static auto WoodDef = FindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
	static auto StoneDef = FindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
	static auto MetalDef = FindObject<UFortItemDefinition>("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");

	switch (ResourceType)
	{
	case EFortResourceType::Wood:
		RetValue = WoodDef;
		break;
	case EFortResourceType::Stone:
		RetValue = StoneDef;
		break;
	case EFortResourceType::Metal:
		RetValue = MetalDef;
		break;
	default:
		RetValue = WoodDef;
		break;
	}

	return RetValue;
}
