#pragma once

#include "framework.h"

inline auto InitHost = reinterpret_cast<bool(*)(AOnlineBeaconHost*)>(GetBaseAddress() + 0x2A72770);
inline auto SetWorld = reinterpret_cast<void(*)(UNetDriver*, UWorld*)>(GetBaseAddress() + 0x1AEFA50);
inline auto InitListen = reinterpret_cast<bool(*)(UIpNetDriver*, void*, FURL, bool, FString&)>(GetBaseAddress() + 0x2A72B50);

class Net
{
public:
	static void Listen();
};