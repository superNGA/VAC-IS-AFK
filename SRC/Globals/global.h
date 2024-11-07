#pragma once

#include <Windows.h>
#include <atomic>
#include <mutex>
#include <vector>

namespace COMMON_STORAGE
{
	extern std::atomic<bool> IN_GAME;
	extern std::atomic<bool> LOADING_ENDED;
	extern std::vector<uintptr_t> ENEMY_PTRS;
};

void ADD_ENEMY_POINTER(uintptr_t ptr);

void CLEAR_ENEMY_LIST();

void GET_ENTITY_LIST(std::vector<uintptr_t>& storage);

int GET_LENGTH();