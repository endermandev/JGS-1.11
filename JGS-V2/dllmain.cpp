#include "framework.h"
#include "Util.h"
#include "Hooks.h"
#include "Loot.h"

DWORD WINAPI MainThread(LPVOID)
{
    Util::SetupConsole();

    LOG("Setting up!");

    InitSDK();
    MH_Initialize();

    StaticFindObject = decltype(StaticFindObject)(GetBaseAddress() + 0xE07E60);

    Loot::InitLootTables();

    //((ULocalPlayer*)GetPlayerController(0)->Player)->ViewportClient->ViewportConsole = NewObject<UConsole>(((ULocalPlayer*)GetPlayerController(0)->Player)->ViewportClient);

    GetPlayerController(0)->SwitchLevel(L"Athena_Terrain");

    Hooks::Init();

    LOG("Setup!");

    return 0;
}

BOOL APIENTRY DllMain(HMODULE mod, DWORD reason, LPVOID res)
{
    if (reason == 1)
        CreateThread(0, 0, MainThread, mod, 0, 0);

    return TRUE;
}
