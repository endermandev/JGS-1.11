#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <vector>
#include <cstdint>
#include <format>
#include <iostream>

#include "SDK.hpp"

using namespace SDK;

#include "minhook/MinHook.h"

#pragma comment(lib, "minhook/minhook.lib")

#include "Util.h"

#define ANY_PACKAGE (UObject*)-1;

inline UObject* (*StaticFindObject)(UClass* Class, UObject* Package, const wchar_t* OrigInName, bool ExactClass);

template <typename T>
static T* Cast(void* ToCast)
{
	return (T*)(ToCast);
}

template <typename T>
static T* NewObject(UObject* Outer, UClass* ObjClass = T::StaticClass())
{
	return (T*)(UGameplayStatics::SpawnObject(ObjClass, Outer));
}

static UFortEngine* GetEngine()
{
	return Engine;
}

static UWorld* GetWorld()
{
	return Engine->GameViewport->World;
}

static APlayerController* GetPlayerController(int Index)
{
	return UGameplayStatics::GetPlayerController(GetWorld(), Index);
}

template <typename T>
static T* GetPlayerController(int Index)
{
	return Cast<T>(GetPlayerController(Index));
}

static uintptr_t GetBaseAddress()
{
	return uintptr_t(GetModuleHandle(0));
}

template <typename T>
static T* SpawnActor(FVector Location = {}, FRotator Rotation = {}, UClass* ActorClass = T::StaticClass())
{
	FQuat Quat;
	FTransform Transform;

	auto DEG_TO_RAD = 3.14159 / 180;
	auto DIVIDE_BY_2 = DEG_TO_RAD / 2;

	auto SP = sin(Rotation.Pitch * DIVIDE_BY_2);
	auto CP = cos(Rotation.Pitch * DIVIDE_BY_2);

	auto SY = sin(Rotation.Yaw * DIVIDE_BY_2);
	auto CY = cos(Rotation.Yaw * DIVIDE_BY_2);

	auto SR = sin(Rotation.Roll * DIVIDE_BY_2);
	auto CR = cos(Rotation.Roll * DIVIDE_BY_2);

	Quat.X = CR * SP * SY - SR * CP * CY;
	Quat.Y = -CR * SP * CY - SR * CP * SY;
	Quat.Z = CR * CP * SY - SR * SP * CY;
	Quat.W = CR * CP * CY + SR * SP * SY;

	Transform.Rotation = Quat;
	Transform.Scale3D = FVector{ 1,1,1 };
	Transform.Translation = Location;

	auto Actor = UGameplayStatics::BeginSpawningActorFromClass(GetWorld(), ActorClass, Transform, false, nullptr);
	UGameplayStatics::FinishSpawningActor(Actor, Transform);
	return (T*)Actor;
}

template <typename T>
static T* FindObject(const std::string& ObjectName, UClass* Class = nullptr, UObject* Package = nullptr)
{
	return (T*)StaticFindObject(Class, Package, std::wstring(ObjectName.begin(), ObjectName.end()).c_str(), false);
}

static FVector GetPlayerStartLocation()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFortPlayerStartWarmup::StaticClass(), &Actors);

	return Actors[rand() % Actors.Num()]->K2_GetActorLocation();
}