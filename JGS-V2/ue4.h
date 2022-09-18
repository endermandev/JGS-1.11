#pragma once

#include "SDK.hpp"
#include "classes.h"
#include "Util.h"
#include "Containers.h"

FORCEINLINE int32& GetReplicationFrame(UNetDriver* Driver)
{
    return *(int32*)(int64(Driver) + 0x002C8);
}

FORCEINLINE FNetworkObjectList& GetNetworkObjectList(UNetDriver* Driver)
{
    return *(*(Containers::TSharedPtr<FNetworkObjectList>*)(int64(Driver) + 0x00490));
}

FORCEINLINE bool& GetIsTimeSensitity(UNetConnection* Connection)
{
    return *(bool*)(int64(Connection) + 0x00241);
}

FORCEINLINE Containers::TSet<int32>& GetDestroyedStartupOrDormantActorGUIDs(UNetConnection* Connection)
{
    return *(Containers::TSet<int32>*)(int64(Connection) + 0x33678);
}

FORCEINLINE Containers::TMap<TWeakObjectPtr<AActor>, UActorChannel*>& GetActorChannels(UNetConnection* Connection)
{
    return *(Containers::TMap<TWeakObjectPtr<AActor>, UActorChannel*>*)(int64(Connection) + 0x33588);
}

FORCEINLINE UE::TMap<int32, FActorDestructionInfo*>& GetDestroyedStartupOrDormantActors(UNetDriver* NetDriver)
{
    return *(UE::TMap<int32, FActorDestructionInfo*>*)(int64(NetDriver) + 0x00228);
}