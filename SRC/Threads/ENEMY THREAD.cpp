#include "ENEMY THREAD.h"

Memory mem("cs2.exe");

void CreateConsole()
{
	AllocConsole();
	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	std::cout << "Console created successfully!" << std::endl;
}


void Entity_Finder(uintptr_t CLIENT)
{
	CLEAR_ENEMY_LIST();
	
	uintptr_t LOCALPLAYER = mem.Read<uintptr_t>(CLIENT + cs2_dumper::offsets::client_dll::dwLocalPlayerPawn);

	int TEAM = mem.Read<int>(LOCALPLAYER + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum);

	uintptr_t EntityList = mem.Read<uintptr_t>(CLIENT + cs2_dumper::offsets::client_dll::dwEntityList);
	if (!EntityList) return;

	uintptr_t ListEntry = mem.Read<uintptr_t>(EntityList + 0x10); //check
	for (int i = 1; i < 64; i++)
	{
		if (!ListEntry) continue;

		uintptr_t CurrentController = mem.Read<uintptr_t>(ListEntry + (i * 0x78));
		if (!CurrentController) continue;

		int PawnHandle = mem.Read<int>(CurrentController + cs2_dumper::schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		if (!PawnHandle) continue;

		uintptr_t ListEntry2 = mem.Read<uintptr_t>(EntityList + (0x8 * ((PawnHandle & 0x7FFF) >> 9) + 0x10));

		uintptr_t PawnCurrent = mem.Read<uintptr_t>(ListEntry2 + (0x78 * (PawnHandle & 0x1FF)));
		if (TEAM == mem.Read<int>(PawnCurrent + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iTeamNum)) continue;
		if (mem.Read<int>(PawnCurrent + cs2_dumper::schemas::client_dll::C_BaseEntity::m_iHealth) < 1) continue;

		ADD_ENEMY_POINTER(PawnCurrent);
	}

	return;
}


void EnemyThread(HINSTANCE Instance)
{
	CreateConsole();
	uintptr_t CLIENT = (uintptr_t)GetModuleHandle("client.dll");
	uintptr_t ENGINE = (uintptr_t)GetModuleHandle("engine2.dll");
	std::cout << "client : " << CLIENT << "\nengine : " << ENGINE;

	int LAST_GAME_STATE = 6;
	//bool IN_GAME = false;
	while (!GetAsyncKeyState(VK_END) & 1)
	{
		system("cls");
		uintptr_t ENGINE_PTR = *(uintptr_t*)(ENGINE + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient);
		int GAME_STATE = *(int*)(ENGINE_PTR + cs2_dumper::offsets::engine2_dll::dwNetworkGameClient_signOnState);

		if (GAME_STATE != 6) { // <--- comment when done
			std::cout << "GAME STATE : " << GAME_STATE << "  , ";
		}

		if (GAME_STATE != 6 && LAST_GAME_STATE == 6)
		{
			COMMON_STORAGE::IN_GAME.store(!COMMON_STORAGE::IN_GAME);
			COMMON_STORAGE::LOADING_ENDED.store(!COMMON_STORAGE::LOADING_ENDED);
			//IN_GAME = !IN_GAME;
		}
		if (LAST_GAME_STATE != 6 && GAME_STATE == 6)
		{
			COMMON_STORAGE::LOADING_ENDED.store(!COMMON_STORAGE::LOADING_ENDED);
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}

		LAST_GAME_STATE = GAME_STATE;

		std::cout << "IN GAME : " << COMMON_STORAGE::IN_GAME; // <--- comment when done
		std::cout << "  , LOADING ENDED : " << COMMON_STORAGE::LOADING_ENDED;

		//won't go beyond this point if in lobby.
		if (!COMMON_STORAGE::IN_GAME) continue;
		if (!COMMON_STORAGE::LOADING_ENDED) continue;

		Entity_Finder(CLIENT);
		std::cout << "  , ENEMIES : " << GET_LENGTH() << "\n";

		//Performance
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); //Change time if not feeling nice.
	}

	FreeConsole();
	FreeLibraryAndExitThread(Instance, 0);
	return;
}