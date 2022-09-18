#include "Util.h"

void Util::SetupConsole()
{
	AllocConsole();

	FILE* pFile;
	freopen_s(&pFile, "CONOUT$", "w", stdout);
}