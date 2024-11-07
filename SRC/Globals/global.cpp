#include "global.h"

namespace COMMON_STORAGE
{
	std::mutex DATA_PROTECTOR;
	std::atomic<bool> IN_GAME = false;
	std::atomic<bool> LOADING_ENDED = true;
	std::vector<uintptr_t> ENEMY_PTRS;
};

void ADD_ENEMY_POINTER(uintptr_t ptr)
{
	std::lock_guard<std::mutex> lock(COMMON_STORAGE::DATA_PROTECTOR);
	COMMON_STORAGE::ENEMY_PTRS.push_back(ptr);
}

void CLEAR_ENEMY_LIST()
{
	std::lock_guard<std::mutex> lock(COMMON_STORAGE::DATA_PROTECTOR);
	COMMON_STORAGE::ENEMY_PTRS.clear();
}

void GET_ENTITY_LIST(std::vector<uintptr_t>& storage)
{
	storage.clear();
	std::lock_guard<std::mutex> lock(COMMON_STORAGE::DATA_PROTECTOR);
	storage = COMMON_STORAGE::ENEMY_PTRS;
}

int GET_LENGTH()
{
	std::lock_guard<std::mutex> lock(COMMON_STORAGE::DATA_PROTECTOR);
	return COMMON_STORAGE::ENEMY_PTRS.size();
}