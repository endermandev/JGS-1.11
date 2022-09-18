#pragma once

#include "framework.h"

#define CREATEHOOK(Address, Hook, Og) \
MH_CreateHook((void*)(Address), Hook, (void**)(Og)); \
MH_EnableHook((void*)(Address));

#define LOG(Msg, ...) \
std::cout << "[+] " << std::format(Msg, __VA_ARGS__) << "\n";

class Util
{
public:
	static void SetupConsole();
};