#include <iostream>
#include <direct.h>
#include <windows.h>
#include <tlhelp32.h>

using namespace std;

char* GetCurrentDir()
{
	char* szRet = (char*)malloc(MAX_PATH);

	_getcwd(szRet, MAX_PATH);

	return szRet;
}

LPCTSTR SzToLPCTSTR(char* szString)
{
	LPTSTR lpszRet;
	size_t size = strlen(szString) + 1;

	lpszRet = (LPTSTR)malloc(MAX_PATH);
	mbstowcs_s(NULL, lpszRet, size, szString, _TRUNCATE);

	return lpszRet;
}

void CheckProcess(LPCTSTR lpczProc, DWORD dwDelay)
{
	HANDLE			hSnap;
	PROCESSENTRY32	peProc;
	BOOL			bAppeared = FALSE;

	if (!bAppeared)
	{
		if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != INVALID_HANDLE_VALUE)
		{
			peProc.dwSize = sizeof(PROCESSENTRY32);
			if (Process32First(hSnap, &peProc))
			while (Process32Next(hSnap, &peProc) && !bAppeared)
			if (!lstrcmp(lpczProc, peProc.szExeFile))
				bAppeared = TRUE;
		}

		CloseHandle(hSnap);
		Sleep(dwDelay);
	}
}

DWORD GetProcessIdByName(LPCTSTR lpczProc)
{
	HANDLE			hSnap;
	PROCESSENTRY32	peProc;
	DWORD			dwRet = -1;

	if ((hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0)) != INVALID_HANDLE_VALUE)
	{
		peProc.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnap, &peProc))
		while (Process32Next(hSnap, &peProc))
		if (!lstrcmp(lpczProc, peProc.szExeFile))
			dwRet = peProc.th32ProcessID;
	}
	CloseHandle(hSnap);

	return dwRet;
}

BOOL InjectDll(DWORD dwPid, char* szDllPath)
{
	DWORD	dwMemSize;
	HANDLE	hProc;
	LPVOID	lpRemoteMem, lpLoadLibrary;
	BOOL	bRet = FALSE;

	if ((hProc = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_CREATE_THREAD, FALSE, dwPid)) != NULL)
	{
		dwMemSize = strlen(szDllPath) + 1;
		if ((lpRemoteMem = VirtualAllocEx(hProc, NULL, dwMemSize, MEM_COMMIT, PAGE_READWRITE)) != NULL)
		if (WriteProcessMemory(hProc, lpRemoteMem, (LPCVOID)szDllPath, dwMemSize, NULL))
		{
			lpLoadLibrary = GetProcAddress(GetModuleHandleA("Kernel32.dll"), "LoadLibraryA");
			if (CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)lpLoadLibrary, lpRemoteMem, 0, NULL) != NULL)
				bRet = TRUE;
		}
	}
	CloseHandle(hProc);

	return bRet;
}

int main(void)
{
	char*   szDllPath = (char*)malloc(MAX_PATH);
	LPTSTR	lpszProc = NULL;

	szDllPath = GetCurrentDir();
	strcat_s(szDllPath, MAX_PATH, "\\");
	strcat_s(szDllPath, MAX_PATH, "AzerothCore.dll");

	cout << "Injecting AzerothCore.dll...\n" << endl;
	Sleep(1000);
	CheckProcess(SzToLPCTSTR("Wow.exe"), 100);

	if (InjectDll(GetProcessIdByName(SzToLPCTSTR("Wow.exe")), szDllPath))
		cout << "> Injection Done !" << endl;
	else
		cout << "> Injection Failed !" << endl;

	cout << "\n";
	Sleep(500);
	return EXIT_SUCCESS;
}
