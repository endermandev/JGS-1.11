#include "Net.h"

void Net::Listen()
{
	AOnlineBeaconHost* Beacon = SpawnActor<AOnlineBeaconHost>();
	Beacon->ListenPort = 6969;

	InitHost(Beacon);

	Beacon->NetDriver->NetDriverName = UKismetStringLibrary::Conv_StringToName(L"GameNetDriver");
	Beacon->NetDriver->World = GetWorld();
	GetWorld()->NetDriver = Beacon->NetDriver;

	FURL Url;
	Url.Port = 7777;
	
	FString Error;

	InitListen(Cast<UIpNetDriver>(Beacon->NetDriver), GetWorld(), Url, false, Error);
	SetWorld(Beacon->NetDriver, GetWorld());

	GetWorld()->LevelCollections[0].NetDriver = GetWorld()->NetDriver;
	GetWorld()->LevelCollections[1].NetDriver = GetWorld()->NetDriver;
}