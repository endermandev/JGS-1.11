#include "Replication.h"

/*int32 Replication::ServerReplicateActors_PrepConnections(const float DeltaSeconds)
{
	int32 NumClientsToTick = NetDriver->ClientConnections.Num();

	bool bFoundReadyConnection = false;

	for (int32 ConnIdx = 0; ConnIdx < NetDriver->ClientConnections.Num(); ConnIdx++)
	{
		UNetConnection* Connection = NetDriver->ClientConnections[ConnIdx];
		if (!Connection) continue;

		AActor* OwningActor = Connection->OwningActor;
		if (OwningActor != NULL && (Connection->Driver->Time - Connection->LastReceiveTime < 1.5f))
		{
			bFoundReadyConnection = true;

			AActor* DesiredViewTarget = OwningActor;
			if (Connection->PlayerController)
			{
				if (AActor* ViewTarget = Connection->PlayerController->GetViewTarget())
				{
					DesiredViewTarget = ViewTarget;
				}
			}
			Connection->ViewTarget = DesiredViewTarget;

			for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
			{
				UNetConnection* Child = Connection->Children[ChildIdx];
				APlayerController* ChildPlayerController = Child->PlayerController;
				if (ChildPlayerController != NULL)
				{
					Child->ViewTarget = ChildPlayerController->GetViewTarget();
				}
				else
				{
					Child->ViewTarget = NULL;
				}
			}
		}
		else
		{
			Connection->ViewTarget = NULL;
			for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
			{
				Connection->Children[ChildIdx]->ViewTarget = NULL;
			}
		}
	}

	return bFoundReadyConnection ? NumClientsToTick : 0;
}

void Replication::ServerReplicateActors_BuildConsiderList(std::vector<FNetworkObjectInfo*>& OutConsiderList, const float ServerTickTime)
{
	//LOG("ServerReplicateActors_BuildConsiderList, Building ConsiderList {}", UGameplayStatics::GetTimeSeconds(GetWorld()));

	int32 NumInitiallyDormant = 0;

	std::vector<AActor*> ActorsToRemove;

	auto ActiveNetworkObjects = GetNetworkObjectList(NetDriver).ActiveNetworkObjects;
	for (int i = 0; i < ActiveNetworkObjects.Num(); i++)
	{
		auto ActorInfo = ActiveNetworkObjects[i].Get();

		if (!ActorInfo->bPendingNetUpdate && UGameplayStatics::GetTimeSeconds(GetWorld()) <= ActorInfo->NextUpdateTime)
		{
			continue;
		}

		AActor* Actor = ActorInfo->Actor;

		if (Actor->RemoteRole == ENetRole::ROLE_None)
		{
			ActorsToRemove.push_back(Actor);
			continue;
		}

		if (Actor->NetDriverName != NetDriver->NetDriverName)
		{
			//LOG("Actor {} in wrong network actors list! (Has net driver '{}', expected '{}')", Actor->GetName(), Actor->NetDriverName.ToString(), NetDriver->NetDriverName.ToString());
			continue;
		}

		if (Actor->NetDormancy == ENetDormancy::DORM_Initial && Actor->bNetStartup)
		{
			NumInitiallyDormant++;
			ActorsToRemove.push_back(Actor);
			continue;
		}

		if (ActorInfo->LastNetReplicateTime == 0)
		{
			ActorInfo->LastNetReplicateTime = UGameplayStatics::GetTimeSeconds(GetWorld());
			ActorInfo->OptimalNetUpdateDelta = 1.0f / Actor->NetUpdateFrequency;
		}

		const float ScaleDownStartTime = 2.0f;
		const float ScaleDownTimeRange = 5.0f;

		const float LastReplicateDelta = UGameplayStatics::GetTimeSeconds(GetWorld()) - ActorInfo->LastNetReplicateTime;

		if (LastReplicateDelta > ScaleDownStartTime)
		{
			if (Actor->MinNetUpdateFrequency == 0.0f)
			{
				Actor->MinNetUpdateFrequency = 2.0f;
			}

			const float MinOptimalDelta = 1.0f / Actor->NetUpdateFrequency;
			const float MaxOptimalDelta = UKismetMathLibrary::Max(1.0f / Actor->MinNetUpdateFrequency, MinOptimalDelta);

			const float Alpha = UKismetMathLibrary::Clamp((LastReplicateDelta - ScaleDownStartTime) / ScaleDownTimeRange, 0.0f, 1.0f);
			ActorInfo->OptimalNetUpdateDelta = UKismetMathLibrary::Lerp(MinOptimalDelta, MaxOptimalDelta, Alpha);
		}

		if (!ActorInfo->bPendingNetUpdate)
		{
			//LOG("actor {} requesting new net update, time: {}", Actor->GetName(), UGameplayStatics::GetTimeSeconds(GetWorld()));

			const float NextUpdateDelta = Actor->NetUpdateFrequency;

			ActorInfo->NextUpdateTime = UGameplayStatics::GetTimeSeconds(GetWorld()) + UKismetMathLibrary::RandomFloat() * ServerTickTime + NextUpdateDelta;

			ActorInfo->LastNetUpdateTime = NetDriver->Time;
		}

		ActorInfo->bPendingNetUpdate = false;

		OutConsiderList.push_back(ActorInfo);

		CallPreReplication(Actor, NetDriver);
	}


}

int32 Replication::ServerReplicateActors_PrioritizeActors(UNetConnection* Connection, std::vector<::FNetViewer>& ConnectionViewers, std::vector<FNetworkObjectInfo*> ConsiderList, const bool bCPUSaturated, std::vector<FActorPriority*>& OutPriorityActors)
{
	int32 FinalSortedCount = 0;
	int32 DeletedCount = 0;

	const int32 MaxSortedActors = ConsiderList.size() + GetDestroyedStartupOrDormantActors(NetDriver).Num();
	if (MaxSortedActors > 0)
	{
		const bool bLowNetBandwidth = false;

		for (FNetworkObjectInfo* ActorInfo : ConsiderList)
		{
			AActor* Actor = ActorInfo->Actor;

			UActorChannel* Channel = FindChannel(Actor, Connection);

			if (!Channel)
			{
				if (!IsActorRelevantToConnection(Actor, ConnectionViewers))
				{
					continue;
				}
			}

			UNetConnection* PriorityConnection = Connection;

			if (Actor->bOnlyRelevantToOwner)
			{
				bool bHasNullViewTarget = false;

				PriorityConnection = IsActorOwnedByAndRelevantToConnection(Actor, ConnectionViewers, bHasNullViewTarget);

				if (PriorityConnection == nullptr)
				{
					if (!bHasNullViewTarget && Channel != NULL && NetDriver->Time - Channel->RelevantTime >= NetDriver->RelevantTimeout)
					{
						ActorChannelClose(Channel);
					}

					continue;
				}
			}

			//LOG("Consider {} alwaysrelevant {} frequency {}", Actor->GetName(), (bool)Actor->bAlwaysRelevant, Actor->NetUpdateFrequency);

			OutPriorityActors.push_back(new FActorPriority(PriorityConnection, Channel, ActorInfo, ConnectionViewers, bLowNetBandwidth));

			FinalSortedCount++;
		}
	}

	auto DestroyedStartupOrDormantActors = GetDestroyedStartupOrDormantActors(NetDriver);
	auto DestroyedStartupOrDormantActorGUIDs = GetDestroyedStartupOrDormantActorGUIDs(Connection);

	for (int i = 0; i < DestroyedStartupOrDormantActorGUIDs.Num(); i++)
	{
		auto DestructionGuid = DestroyedStartupOrDormantActorGUIDs[i];

		for (int i = 0; i < DestroyedStartupOrDormantActors.Num(); i++)
		{
			if (DestroyedStartupOrDormantActors[i]->NetGUID == DestructionGuid)
			{
				auto DestructionInfo = DestroyedStartupOrDormantActors[i];

				OutPriorityActors.push_back(new FActorPriority(Connection, DestructionInfo, ConnectionViewers));

				FinalSortedCount++;
				DeletedCount++;
			}
		}
	}

	//LOG("ServerReplicateActors_PrioritizeActors: Potential {} ConsiderList {} FinalSortedCount {}", MaxSortedActors, ConsiderList.size(), FinalSortedCount);

	return FinalSortedCount;
}

int32 Replication::ServerReplicateActors_ProcessPrioritizedActors(UNetConnection* Connection, std::vector<::FNetViewer>& ConnectionViewers, std::vector<FActorPriority*> PriorityActors, const int32 FinalSortedCount, int32& OutUpdated)
{
	int32 ActorUpdatesThisConnection = 0;
	int32 ActorUpdatesThisConnectionSent = 0;
	int32 FinalRelevantCount = 0;

	for (int32 j = 0; j < FinalSortedCount; j++)
	{
		FNetworkObjectInfo* ActorInfo = PriorityActors[j]->ActorInfo;

		if (ActorInfo == NULL && PriorityActors[j]->DestructionInfo)
		{
			UActorChannel* Channel = (UActorChannel*)CreateChannel(Connection, 2, 1, -1);
			if (Channel)
			{
				FinalRelevantCount++;
				LOG("Server replicate actor creating destroy channel for NetGUID <{},{}> Priority: {}", PriorityActors[j]->DestructionInfo->NetGUID, PriorityActors[j]->DestructionInfo->PathName.ToString(), PriorityActors[j]->Priority);

				//Channel->SetChannelActorForDestroy(PriorityActors[j]->DestructionInfo);
				GetDestroyedStartupOrDormantActorGUIDs(Connection).Remove(PriorityActors[j]->DestructionInfo->NetGUID);
			}
			continue;
		}

		UActorChannel* Channel = PriorityActors[j]->Channel;
		//LOG(" Maybe Replicate {}", ActorInfo->Actor->GetName());
		if (!Channel || Channel->Actor)
		{
			AActor* Actor = ActorInfo->Actor;
			bool bIsRelevant = false;

			if (!Actor->bTearOff && (!Channel || NetDriver->Time - Channel->RelevantTime > 1.f))
			{
				if (IsActorRelevantToConnection(Actor, ConnectionViewers))
				{
					bIsRelevant = true;
				}
			}

			const bool bIsRecentlyRelevant = bIsRelevant || (Channel && NetDriver->Time - Channel->RelevantTime < NetDriver->RelevantTimeout) || ActorInfo->bForceRelevantNextUpdate;

			ActorInfo->bForceRelevantNextUpdate = false;

			if (bIsRecentlyRelevant)
			{
				FinalRelevantCount++;

				if (Channel == NULL /*&& GuidCache->SupportsObject(Actor->GetClass()) && GuidCache->SupportsObject(Actor->IsNetStartupActor() ? Actor : Actor->GetArchetype()))
				{
					Channel = (UActorChannel*)CreateChannel(Connection, 2, 1, -1);
					if (Channel)
					{
						SetChannelActor(Channel, Actor);
					}

					if (Actor->NetUpdateFrequency < 1.0f)
					{
						//LOG("Unable to replicate {}", Actor->GetName());
						ActorInfo->NextUpdateTime = UGameplayStatics::GetTimeSeconds(GetWorld()) + 0.2f * UKismetMathLibrary::RandomFloat();
					}
				}

				if (Channel)
				{
					if (bIsRelevant)
					{
						Channel->RelevantTime = NetDriver->Time + 0.5f * UKismetMathLibrary::RandomFloat();
					}

					//LOG("- Replicate {}. {}", Actor->GetName(), PriorityActors[j]->Priority);

					if (ReplicateActor(Channel))
					{
						ActorUpdatesThisConnectionSent++;

						const float MinOptimalDelta = 1.0f / Actor->NetUpdateFrequency;
						const float MaxOptimalDelta = UKismetMathLibrary::Max(1.0f / Actor->MinNetUpdateFrequency, MinOptimalDelta);
						const float DeltaBetweenReplications = (UGameplayStatics::GetTimeSeconds(GetWorld()) - ActorInfo->LastNetReplicateTime);

						ActorInfo->OptimalNetUpdateDelta = UKismetMathLibrary::Clamp(DeltaBetweenReplications * 0.7f, MinOptimalDelta, MaxOptimalDelta);
						ActorInfo->LastNetReplicateTime = UGameplayStatics::GetTimeSeconds(GetWorld());
					}

					ActorUpdatesThisConnection++;
					OutUpdated++;
				}
			}

			if ((!bIsRecentlyRelevant || Actor->bTearOff) && Channel != NULL)
			{
				if (!Actor->bNetStartup)
				{
					//LOG("- Closing channel for no longer relevant actor {}", Actor->GetName());
					ActorChannelClose(Channel);
				}
			}
		}
	}

	return FinalSortedCount;
}

int32 Replication::ServerReplicateActors(float DeltaSeconds)
{
	if (NetDriver->ClientConnections.Num() == 0)
	{
		return 0;
	}

	int32 Updated = 0;

	GetReplicationFrame(NetDriver)++;

	const int32 NumClientsToTick = ServerReplicateActors_PrepConnections(DeltaSeconds);

	if (NumClientsToTick == 0)
	{
		return 0;
	}

	AWorldSettings* WorldSettings = GetWorld()->PersistentLevel->WorldSettings;

	bool bCPUSaturated = false;
	float ServerTickTime = GetMaxTickRate(GetEngine(), DeltaSeconds, false);
	if (ServerTickTime == 0.f)
	{
		ServerTickTime = DeltaSeconds;
	}
	else
	{
		ServerTickTime = 1.f / ServerTickTime;
		bCPUSaturated = DeltaSeconds > 1.2f * ServerTickTime;
	}

	std::vector<FNetworkObjectInfo*> ConsiderList;
	ServerReplicateActors_BuildConsiderList(ConsiderList, ServerTickTime);

	for (int32 i = 0; i < NetDriver->ClientConnections.Num(); i++)
	{
		UNetConnection* Connection = NetDriver->ClientConnections[i];

		if (i >= NumClientsToTick)
		{
			for (int32 ConsiderIdx = 0; ConsiderIdx < ConsiderList.size(); ConsiderIdx++)
			{
				AActor* Actor = ConsiderList[ConsiderIdx]->Actor;
				if (Actor != NULL && !ConsiderList[ConsiderIdx]->bPendingNetUpdate)
				{
					UActorChannel* Channel = FindChannel(ConsiderList[ConsiderIdx]->Actor, Connection);
					if (Channel != NULL && Channel->LastUpdateTime < ConsiderList[ConsiderIdx]->LastNetUpdateTime)
					{
						ConsiderList[ConsiderIdx]->bPendingNetUpdate = true;
					}
				}
			}

			GetIsTimeSensitity(Connection) = true;
		}
		else if (Connection->ViewTarget)
		{
			std::vector<::FNetViewer> ConnectionViewers;

			ConnectionViewers.push_back(::FNetViewer(Connection, DeltaSeconds));
			for (int32 ViewerIndex = 0; ViewerIndex < Connection->Children.Num(); ViewerIndex++)
			{
				if (Connection->Children[ViewerIndex]->ViewTarget != NULL)
				{
					ConnectionViewers.push_back(::FNetViewer(Connection->Children[ViewerIndex], DeltaSeconds));
				}
			}

			if (Connection->PlayerController)
			{
				SendClientAdjustment(Connection->PlayerController);
			}

			for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
			{
				if (Connection->Children[ChildIdx]->PlayerController != NULL)
				{
					SendClientAdjustment(Connection->Children[ChildIdx]->PlayerController);
				}
			}

			std::vector<FActorPriority*> PriorityActors;

			const int32 FinalSortedCount = ServerReplicateActors_PrioritizeActors(Connection, ConnectionViewers, ConsiderList, bCPUSaturated, PriorityActors);
			const int32 LastProcessedActor = ServerReplicateActors_ProcessPrioritizedActors(Connection, ConnectionViewers, PriorityActors, FinalSortedCount, Updated);

			for (int32 k = LastProcessedActor; k < FinalSortedCount; k++)
			{
				if (!PriorityActors[k]->ActorInfo)
				{
					continue;
				}

				AActor* Actor = PriorityActors[k]->ActorInfo->Actor;

				UActorChannel* Channel = PriorityActors[k]->Channel;

				//LOG("Saturated. {}", Actor->GetName());
				if (Channel != NULL && NetDriver->Time - Channel->RelevantTime <= 1.f)
				{
					//LOG(" Saturated. Mark {} NetUpdateTime to be checked for next tick", Actor->GetName());
					PriorityActors[k]->ActorInfo->bPendingNetUpdate = true;
				}
				else if (IsActorRelevantToConnection(Actor, ConnectionViewers))
				{
					//LOG(" Saturated. Mark {} NetUpdateTime to be checked for next tick", Actor->GetName());
					PriorityActors[k]->ActorInfo->bPendingNetUpdate = true;
					if (Channel != NULL)
					{
						Channel->RelevantTime = NetDriver->Time + 0.5f * UKismetMathLibrary::RandomFloat();
					}
				}
			}

			ConnectionViewers.clear();
			ConnectionViewers.empty();
			PriorityActors.clear();
			PriorityActors.empty();
		}
	}

	ConsiderList.clear();
	ConsiderList.empty();

	if (NumClientsToTick < NetDriver->ClientConnections.Num())
	{
		int32 NumConnectionsToMove = NumClientsToTick;
		while (NumConnectionsToMove > 0)
		{
			UNetConnection* Connection = NetDriver->ClientConnections[0];
			NetDriver->ClientConnections.RemoveAt(0, 1);
			NetDriver->ClientConnections.Add(Connection);
			NumConnectionsToMove--;
		}
	}

	return Updated;
}*/