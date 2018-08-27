#undef UNICODE
#include <cstdio>
#include <windows.h>
#include <detours.h>
#include <ctime>
#include <string>
#include <iostream>
#include <TlHelp32.h>
#include <Psapi.h>
#include <cstdio>
#include <stdio.h>
#include <conio.h>

__declspec(dllexport) void __Init__();

HMODULE current_mod;

void *lol;

typedef void(__stdcall *t_addCollectedExpAmount)(void *ptr, int a1);
t_addCollectedExpAmount addCollectedExpAmount = (t_addCollectedExpAmount)0x022341F0;


void __fastcall fast_addCollectedExpAmount(void *ptr, void *Unknown, int a1)
{
	lol = ptr;
	_cprintf("--------addCollectedExpAmount-----------\n");
	_cprintf("Argument 01 : %p\n", ptr);
	_cprintf("Argument 02 : %p\n", a1);
	_cprintf("-------------------------\n");
	return addCollectedExpAmount(ptr, a1);
}


INT APIENTRY DllMain(HMODULE hDLL, DWORD Reason, LPVOID Reserved)
{

	switch (Reason)
	{
	case DLL_PROCESS_ATTACH:

		current_mod = hDLL; // We take it here

		__Init__(); // Init the main thread.

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)addCollectedExpAmount, fast_addCollectedExpAmount);
		DetourTransactionCommit();

		break;

	case DLL_PROCESS_DETACH:

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)addCollectedExpAmount, fast_addCollectedExpAmount);
		DetourTransactionCommit();

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}

void  __Init__()
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	SetConsoleTitle("-- Trine 2 -- Injection Console --");

	int Data_Of_Thread_1 = 1;
	int Data_Of_Main_Thread = 1;
	HANDLE Handle_Of_Thread_1 = 0;
	DWORD WINAPI Main_Thread(LPVOID lpParam);

	CreateThread(NULL, 0, Main_Thread, &Data_Of_Main_Thread, 0, NULL);
}


DWORD WINAPI Main_Thread(LPVOID lpParam)
{
	_cprintf("-- Main Thread Initialized --\n");

	while (1)
	{

		if (GetAsyncKeyState(VK_F4))
		{

			fast_addCollectedExpAmount(lol, lol, 0x01);
			Sleep(1000);
		}

		if (GetAsyncKeyState(VK_F5))
		{
			FreeConsole();
			FreeLibraryAndExitThread(current_mod, 0);
			Sleep(1000);
		}

		Sleep(100);
	}


	return 0;
}