#pragma once

#include "framework.h"
#include "Util.h"
#include "Net.h"
#include "Replication.h"
#include "Inventory.h"
#include "Abilities.h"
#include "Helpers.h"
#include "Loot.h"

static void* (*ProcessEvent)(UObject*, UFunction*, void*);
static void (*TickFlush)(UNetDriver*);

class Hooks
{
public:
	static void* ProcessEventHook(UObject* pObject, UFunction* pFunction, void* pParams);
	static int64_t KickPatchHook(int64_t, int64_t);
	static void TickFlushHook(UNetDriver* NetDriver);
	static void Init();
};