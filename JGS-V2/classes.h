#pragma once

#include "SDK.hpp"
#include "Containers.h"

#include <intrin.h>

struct FActorDestructionInfo
{
    TWeakObjectPtr<ULevel>		Level;
    TWeakObjectPtr<UObject>		ObjOuter;
    FVector			            DestroyedPosition;
    int32       	            NetGUID;
    FString			            PathName;

    FName			            StreamingLevelName;
};

struct FNetworkObjectInfo
{
    AActor* Actor;
    TWeakObjectPtr<AActor> WeakActor;
    double NextUpdateTime;
    double LastNetReplicateTime;
    float OptimalNetUpdateDelta;
    float LastNetUpdateTime;
    uint32 bPendingNetUpdate : 1;
    uint32 bForceRelevantNextUpdate : 1;
    Containers::TSet<TWeakObjectPtr<UNetConnection>> DormantConnections;
    Containers::TSet<TWeakObjectPtr<UNetConnection>> RecentlyDormantConnections;
};

class FNetworkObjectList
{
public:
    typedef Containers::TSet<Containers::TSharedPtr<FNetworkObjectInfo>> FNetworkObjectSet;

    FNetworkObjectSet AllNetworkObjects;
    FNetworkObjectSet ActiveNetworkObjects;
    FNetworkObjectSet ObjectsDormantOnAllConnections;

    TMap<TWeakObjectPtr<UNetConnection>, int32> NumDormantObjectsPerConnection;
};

// ScriptStruct Engine.NetViewer
// 0x0030
struct FNetViewer
{
    class UNetConnection* Connection;                                               // 0x0000(0x0008) (ZeroConstructor, IsPlainOldData)
    class AActor* InViewer;                                                 // 0x0008(0x0008) (ZeroConstructor, IsPlainOldData)
    class AActor* ViewTarget;                                               // 0x0010(0x0008) (ZeroConstructor, IsPlainOldData)
    struct FVector                                     ViewLocation;                                             // 0x0018(0x000C) (IsPlainOldData)
    struct FVector                                     ViewDir;                                                  // 0x0024(0x000C) (IsPlainOldData)

    FNetViewer()
        : Connection(0)
        , InViewer(0)
        , ViewTarget(0)
        , ViewLocation(FVector({ 0, 0, 0 }))
        , ViewDir(FVector({ 0, 0, 0 }))
    {
    }

    FNetViewer(UNetConnection* InConnection, float DeltaSeconds)
        : Connection(InConnection)
        , InViewer((InConnection->PlayerController ? InConnection->PlayerController : InConnection->OwningActor))
        , ViewTarget(InConnection->ViewTarget)
        , ViewLocation(FVector({ 0, 0, 0 }))
        , ViewDir(FVector({ 0, 0, 0 }))
    {
        APlayerController* ViewingController = InConnection->PlayerController;

        FVector ViewTargetLoc = ViewTarget->K2_GetActorLocation();

        if (ViewTargetLoc)
        {
            ViewLocation = ViewTargetLoc;

            if (ViewingController)
            {
                FRotator ViewRotation = ViewingController->ControlRotation;
                ViewingController->GetActorEyesViewPoint(&ViewLocation, &ViewRotation);
                ViewDir = ViewRotation.Vector();
            }
        }
    }

    inline const bool operator==(const ::FNetViewer& Other) const
    {
        return Connection == Other.Connection && InViewer == Other.InViewer && ViewTarget == Other.ViewTarget;
    }
    inline const bool operator!=(const ::FNetViewer& Other) const
    {
        return Connection != Other.Connection || InViewer != Other.InViewer || ViewTarget != Other.ViewTarget;
    }
};

struct FActorPriority
{
    int32 Priority;

    FNetworkObjectInfo* ActorInfo;
    UActorChannel* Channel;

    struct FActorDestructionInfo* DestructionInfo;

    FActorPriority()
        : Priority(0)
        , ActorInfo(NULL)
        , Channel(NULL)
        , DestructionInfo(NULL)
    {
    }

    FActorPriority(UNetConnection* InConnection, UActorChannel* InChannel, FNetworkObjectInfo* InActorInfo, std::vector<::FNetViewer>& Viewers, bool bLowBandwidth)
        : ActorInfo(InActorInfo)
        , Channel(InChannel)
        , DestructionInfo(NULL)
    {
        static auto RoundToInt = [](float F)
        {
            return _mm_cvt_ss2si(_mm_set_ss(F + F + 0.5f)) >> 1;
        };

        //float Time = Channel ? (InConnection->Driver->Time - Channel->LastUpdateTime) : InConnection->Driver->SpawnPrioritySeconds;

        Priority = 0;
        for (int32 i = 0; i < Viewers.size(); i++)
        {
            // Priority = Util::Max(Priority, RoundToInt(65536.0f * GetNetPriority(ActorInfo->Actor, Viewer.ViewLocation, Viewer.ViewDir, Viewer.InViewer, Viewer.ViewTarget, InChannel, Time, bLowBandwidth)));
        }
    }
    FActorPriority(UNetConnection* InConnection, FActorDestructionInfo* Info, std::vector<::FNetViewer>& Viewers)
        : ActorInfo(NULL)
        , Channel(NULL)
        , DestructionInfo(Info)
    {
        Priority = 0;

        for (int32 i = 0; i < Viewers.size(); i++)
        {
            float Time = InConnection->Driver->SpawnPrioritySeconds;

            FVector Dir = DestructionInfo->DestroyedPosition - Viewers[i].ViewLocation;
            float DistSq = Dir.SizeSquared();

            // adjust priority based on distance and whether actor is in front of viewer
            if ((Viewers[i].ViewDir | Dir) < 0.f)
            {
                if (DistSq > (2000.f * 2000.f))
                {
                    Time *= 0.2f;
                }
                else if (DistSq > (500.f * 500.f))
                {
                    Time *= 0.4f;
                }
            }
            else if (DistSq > (3162.f * 3162.f))
            {
                Time *= 0.4f;
            }

            Priority = max(Priority, int(65536.0f * Time));
        }
    }
};