#pragma once

#include "framework.h"

class Loot
{
public:
	static inline std::vector<UFortItemDefinition*> CommonWeapons;
	static inline std::vector<UFortItemDefinition*> UnCommonWeapons;
	static inline std::vector<UFortItemDefinition*> RareWeapons;
	static inline std::vector<UFortItemDefinition*> EpicWeapons;
	static inline std::vector<UFortItemDefinition*> LegendaryWeapons;

	static inline std::vector<UFortItemDefinition*> Consumables;

	static inline std::vector<UFortItemDefinition*> Traps;

	static inline std::vector<UFortItemDefinition*> Ammo;

	static inline std::vector<UFortItemDefinition*> Resources;

	static int GetWeightForLootSpawn();
	static UFortItemDefinition* GetRandomItemFromRarity(int Rarity);
	static UFortItemDefinition* GetRandomConsumable();
	static std::vector<UFortItemDefinition*> GetRandomAmmos();
	static UFortItemDefinition* GetRandomResource();

	static void SpawnPickup(UFortItemDefinition* ItemDef, int Count, FVector Location, FRotator Rotation);
		
	static void InitLootTables();
	static void SpawnFloorLoot();
	static void SpawnLootForChest(ABuildingContainer* Container);
	static void SpawnLootForAmmoBox(ABuildingContainer* Container);
};