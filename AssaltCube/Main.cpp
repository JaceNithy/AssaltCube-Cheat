#include <Windows.h>
#include "Game.h"
#include "ESP.h"
#include "Hook.h"
#include "Main.h"

Player*			pLocalPlayer = *(Player**)0x50F4F4;
EnemyList*		pEnemyList = (EnemyList*)0x0510d90;
Game			game = Game(0x501AE8, 0x510D98);

ESP* esp = new ESP;


typedef void(WINAPI * twglSwapBuffers) (HDC hDc);
twglSwapBuffers owglSwapBuffers;


void WINAPI hkwglSwapBuffs(_In_ HDC hDc){
	if (esp->State())
		esp->Draw();

	//	if (Menu)
	//		DrawMenu();

	return owglSwapBuffers(hDc);
}


DWORD WINAPI ThreadController(LPVOID)
{

	HMODULE hMod = GetModuleHandleA("opengl32.dll");
	Hook wGlSwapBuffHook = Hook(GetProcAddress(hMod, "wglSwapBuffers"), hkwglSwapBuffs, 5);
	owglSwapBuffers = (twglSwapBuffers)wGlSwapBuffHook.CreateDetour();


	while (true)
	{
		if (GetAsyncKeyState(VK_INSERT))
		{
			esp->Toggle();
		}

		if (esp->State()) pLocalPlayer->SetHealth(999);

		Sleep(100);
	}
}

DWORD WINAPI DllMain(HMODULE hDll, DWORD dwReasonForCall, LPVOID lpReserved)
{
	if (dwReasonForCall == DLL_PROCESS_ATTACH)
	{
		CreateThread(nullptr, NULL, ThreadController, nullptr, NULL, nullptr);
		return TRUE;
	}
	else if (dwReasonForCall == DLL_PROCESS_DETACH)
	{
		return TRUE;
	}
	return FALSE;
}