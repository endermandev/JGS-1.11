#pragma once

#include "framework.h"

class Inventory
{
public:
	static void AddItem(AFortPlayerController* PlayerController, UFortItemDefinition* ItemDef, int Count, bool bAddToQuickBar = false, EFortQuickBars QuickBars = EFortQuickBars::Primary, int Slot = 0);
	static void Setup(AFortPlayerController* PlayerController);
	static void Update(AFortPlayerController* PlayerController);
	static void UpdateCount(AFortPlayerController* PlayerController, UFortItemDefinition* ItemDef, int Count);
};