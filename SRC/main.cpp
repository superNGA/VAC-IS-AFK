#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <thread>
#include <iostream>

#include "Threads/ENEMY THREAD.h"
#include "Threads/AIMBOT THREAD.h"

BOOL WINAPI DllMain(HINSTANCE Instance, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
        DisableThreadLibraryCalls(Instance);
        auto ENEMY_THREAD = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(EnemyThread), Instance, 0, nullptr);
        auto AIMBOT_THREAD = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(AimbotThread), Instance, 0, nullptr);

        if (ENEMY_THREAD) CloseHandle(ENEMY_THREAD);
        if (AIMBOT_THREAD) CloseHandle(AIMBOT_THREAD);
	}

	return TRUE;
}