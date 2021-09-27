#include "stdafx.h"
#include <string>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            EnableConsole();
			PtrCheck();
	    	Commands();
            SetMessageHandlers();
            break;
		default:
			break;
    }

    return TRUE;
}

struct SizeOfCode
{
    DWORD_PTR start;
    DWORD_PTR end;
};

// Fix InvalidPtrCheck for callbacks outside of .text section
void PtrCheck()
{
    SizeOfCode *s = reinterpret_cast<SizeOfCode*>(SIZE_OF_CODE_ADDR);
    s->start = 0x00000001;
    s->end = 0x7FFFFFFF;
}

// Enable Console
void EnableConsole()
{
    *reinterpret_cast<DWORD*>(ENABLE_CONSOLE_ADDR) = 1;	
}

void Commands()
{
    Console::RegisterCommand("GuildLeave", GuildLeave, CATEGORY_CONSOLE, "Usage : GuildLeave");
}