#pragma once

#include "framework.h"
#include "Util.h"
#include "classes.h"
#include "ue4.h"

inline auto CallPreReplication = reinterpret_cast<void(*)(AActor*, UNetDriver*)>(GetBaseAddress() + 0x1761700);
inline auto IsNetRelevantFor = reinterpret_cast<bool(*)(AActor*, AActor*, AActor*, FVector&)>(GetBaseAddress() + 0x1775200);
inline auto ActorChannelClose = reinterpret_cast<void(*)(UActorChannel*)>(GetBaseAddress() + 0x1953080);
inline auto CreateChannel = reinterpret_cast<UActorChannel*(*)(UNetConnection*, int, bool, int32_t)>(GetBaseAddress() + 0x1AD6A60);
inline auto SetChannelActor = reinterpret_cast<void(*)(UActorChannel*, AActor*)>(GetBaseAddress() + 0x1970310);
inline auto ReplicateActor = reinterpret_cast<bool(*)(UActorChannel*)>(GetBaseAddress() + 0x196B970);
inline auto SendClientAdjustment = reinterpret_cast<void(*)(APlayerController*)>(GetBaseAddress() + 0x1BE16B0);
inline auto GetMaxTickRate = reinterpret_cast<float(*)(UEngine*, float, bool)>(GetBaseAddress() + 0x1D13F30);

static UActorChannel* FindChannel(AActor* Actor, UNetConnection* Connection)
{
	for (int i = 0; i < Connection->OpenChannels.Num(); i++)
	{
		auto Channel = Connection->OpenChannels[i];

		if (Channel && Channel->Class)
		{
			if (Channel->Class == UActorChannel::StaticClass())
			{
				if (((UActorChannel*)Channel)->Actor == Actor)
					return ((UActorChannel*)Channel);
			}
		}
	}

	return NULL;
}

static bool IsActorRelevantToConnection(AActor* Actor, std::vector<::FNetViewer> ConnectionViewers)
{
	for (int32 viewerIdx = 0; viewerIdx < ConnectionViewers.size(); viewerIdx++)
	{
		if (IsNetRelevantFor(Actor, ConnectionViewers[viewerIdx].InViewer, ConnectionViewers[viewerIdx].ViewTarget, ConnectionViewers[viewerIdx].ViewLocation))
		{
			return true;
		}
	}

	return false;
}

static auto GetNetOwner = [](const AActor* Actor) -> const AActor*
{
	for (auto Owner = Actor; Actor; Actor = Actor->Owner)
	{
		if (Actor->IsA(APlayerController::StaticClass()))
		{
			return ((APlayerController*)Actor);
		}
	}

	return nullptr;
};

static UNetConnection* IsActorOwnedByAndRelevantToConnection(const AActor* Actor, std::vector<::FNetViewer>& ConnectionViewers, bool& bOutHasNullViewTarget)
{
	const AActor* ActorOwner = GetNetOwner(Actor);

	bOutHasNullViewTarget = false;

	for (int i = 0; i < ConnectionViewers.size(); i++)
	{
		UNetConnection* ViewerConnection = ConnectionViewers[i].Connection;

		if (ViewerConnection->ViewTarget == nullptr)
		{
			bOutHasNullViewTarget = true;
		}

		if (ActorOwner == ViewerConnection->PlayerController ||
			(ViewerConnection->PlayerController && ActorOwner == ViewerConnection->PlayerController->Pawn) ||
			(ViewerConnection->ViewTarget && ViewerConnection->ViewTarget->IsRelevancyOwnerFor(Actor, ActorOwner, ViewerConnection->OwningActor)))
		{
			return ViewerConnection;
		}
	}

	return nullptr;
}

namespace Replication
{
	static UActorChannel* ReplicateToClient(AActor* InActor, UNetConnection* InConnection)
	{
		if (InActor->IsA(APlayerController::StaticClass()) && InActor != InConnection->PlayerController)
			return nullptr;

		auto Channel = (UActorChannel*)(CreateChannel(InConnection, 2, true, -1));

		if (Channel) {
			SetChannelActor(Channel, InActor);
			Channel->Connection = InConnection;

			return Channel;
		}

		return NULL;
	}

	static int PrepConnections(UNetDriver* NetDriver)
	{
		int ReadyConnections = 0;

		for (int ConnIdx = 0; ConnIdx < NetDriver->ClientConnections.Num(); ConnIdx++)
		{
			UNetConnection* Connection = NetDriver->ClientConnections[ConnIdx];
			if (!Connection) continue;

			AActor* OwningActor = Connection->OwningActor;

			if (OwningActor)
			{
				ReadyConnections++;
				AActor* DesiredViewTarget = OwningActor;

				if (Connection->PlayerController)
				{
					if (AActor* ViewTarget = Connection->PlayerController->GetViewTarget())
					{
						DesiredViewTarget = ViewTarget;
					}
				}

				Connection->ViewTarget = DesiredViewTarget;

				for (int ChildIdx = 0; ChildIdx < Connection->Children.Num(); ++ChildIdx)
				{
					UNetConnection* ChildConnection = Connection->Children[ChildIdx];
					if (ChildConnection && ChildConnection->PlayerController && ChildConnection->ViewTarget)
					{
						ChildConnection->ViewTarget = DesiredViewTarget;
					}
				}
			}
			else
			{
				Connection->ViewTarget = nullptr;

				for (int ChildIdx = 0; ChildIdx < Connection->Children.Num(); ++ChildIdx)
				{
					UNetConnection* ChildConnection = Connection->Children[ChildIdx];
					if (ChildConnection && ChildConnection->PlayerController && ChildConnection->ViewTarget)
					{
						ChildConnection->ViewTarget = nullptr;
					}
				}
			}
		}

		return ReadyConnections;
	}

	static UActorChannel* FindChannel(AActor* Actor, UNetConnection* Connection)
	{
		for (int i = 0; i < Connection->OpenChannels.Num(); i++)
		{
			auto Channel = Connection->OpenChannels[i];

			if (Channel && Channel->Class)
			{
				if (Channel->Class == UActorChannel::StaticClass())
				{
					if (((UActorChannel*)Channel)->Actor == Actor)
						return ((UActorChannel*)Channel);
				}
			}
		}

		return NULL;
	}

	static UNetConnection* GetOwningConnection(AActor* Actor)
	{
		for (auto Owner = Actor; Actor; Actor = Actor->GetOwner())
		{
			if (Actor->IsA(APlayerController::StaticClass()))
			{
				return ((APlayerController*)Actor)->NetConnection;
			}
		}
	}

	static void BuildConsiderList(UNetDriver* NetDriver, std::vector<FNetworkObjectInfo*>& OutConsiderList)
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), &Actors);

		for (int i = 0; i < Actors.Num(); i++)
		{
			auto Actor = Actors[i];

			if (!Actor)
				continue;

			if (!Actor)
			{
				continue;
			}

			if (Actor->bActorIsBeingDestroyed)
			{
				continue;
			}

			if (Actor->RemoteRole == ENetRole::ROLE_None)
			{
				continue;
			}

			if (Actor->NetDormancy == ENetDormancy::DORM_Initial && Actor->bNetStartup)
			{
				continue;
			}

			if (Actor->Name.ComparisonIndex != 0)
			{
				if (CallPreReplication)
					CallPreReplication(Actor, NetDriver);

				//Actor->NetCullDistanceSquared = Actor->NetCullDistanceSquared * 2.5;

				FNetworkObjectInfo* Info = new FNetworkObjectInfo();
				Info->Actor = Actor;

				OutConsiderList.push_back(Info);
			}
		}

		FreeMemory((void*)Actors.Data);
		Actors.Data = 0;
		Actors.Count = 0;
		Actors.Max = 0;
	}

	static bool IsActorRelevantToConnection(AActor* Actor, UNetConnection* NetConnection)
	{
		if (NetConnection)
		{
			if (NetConnection->ViewTarget)
			{
				auto ActorLocation = NetConnection->ViewTarget->K2_GetActorLocation();
				return IsNetRelevantFor(Actor, NetConnection->ViewTarget, NetConnection->ViewTarget, ActorLocation);
			}
		}

		return false; //Hope this fixs the damn issue
	}

	static void ReplicateActors(UNetDriver* NetDriver)
	{
		++* (DWORD*)(__int64(NetDriver) + 712);

		auto NumClientsToTick = PrepConnections(NetDriver);

		if (NumClientsToTick == 0)
			return;

		std::vector<FNetworkObjectInfo*> ConsiderList;
		BuildConsiderList(NetDriver, ConsiderList);

		for (int i = 0; i < NetDriver->ClientConnections.Num(); i++)
		{
			auto Connection = NetDriver->ClientConnections[i];

			if (!Connection)
				continue;

			if (i >= NumClientsToTick)
				continue;

			if (!Connection->ViewTarget)
				continue;

			if (Connection->PlayerController)
			{
				SendClientAdjustment(Connection->PlayerController);
			}

			for (int32_t ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
			{
				if (Connection->Children[ChildIdx]->PlayerController != NULL)
				{
					SendClientAdjustment(Connection->Children[ChildIdx]->PlayerController);
				}
			}

			for (auto ActorInfo : ConsiderList)
			{
				if (!ActorInfo)
					continue;

				if (!ActorInfo->Actor)
					continue;

				auto Actor = ActorInfo->Actor;

				auto Channel = FindChannel(Actor, Connection);

				const bool bIsRelevant = IsActorRelevantToConnection(ActorInfo->Actor, Connection);

				if (!Actor->bAlwaysRelevant && !Actor->bNetUseOwnerRelevancy && !Actor->bOnlyRelevantToOwner)
				{
					if (Connection && Connection->ViewTarget)
					{
						auto Viewer = Connection->ViewTarget;
						auto Loc = Viewer->K2_GetActorLocation();
						if (!IsNetRelevantFor(Actor, Viewer, Connection->ViewTarget, Loc))
						{
							if (Channel)
								ActorChannelClose(Channel);
							continue;
						}
					}
				}

				if (!Channel)
					Channel = ReplicateToClient(Actor, Connection);

				if (Channel)
					ReplicateActor(Channel);
			}
		}

		ConsiderList.empty();
		ConsiderList.clear();
	}
}

/*class Replication
{
public:
	static inline UNetDriver* NetDriver;

	static int32 ServerReplicateActors_PrepConnections(const float DeltaSeconds);
	static void ServerReplicateActors_BuildConsiderList(std::vector<FNetworkObjectInfo*>& OutConsiderList, const float ServerTickTime);
	static int32 ServerReplicateActors_PrioritizeActors(UNetConnection* Connection, std::vector<::FNetViewer>& ConnectionViewers, std::vector<FNetworkObjectInfo*> ConsiderList, const bool bCPUSaturated, std::vector<FActorPriority*>& OutPriorityActors);
	static int32 ServerReplicateActors_ProcessPrioritizedActors(UNetConnection* Connection, std::vector<::FNetViewer>& ConnectionViewers, std::vector<FActorPriority*> PriorityActors, const int32 FinalSortedCount, int32& OutUpdated);
	static int32 ServerReplicateActors(float DeltaSeconds);
};*/