#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include <iostream>

int main()
{
	setlocale(LC_ALL, "rus");

	TCHAR lpszAppName[] = _T("lab2_Child.exe");
	TCHAR lpszAppName2[] = _T("Count.exe");
	TCHAR lpszCommandLine[512];

	_tcscpy_s(lpszCommandLine, lpszAppName);
	_tcscat_s(lpszCommandLine, _T(" 10 5 4 3 7 13 19 35 22 9 12 5 34 45"));

	STARTUPINFO si; 
	STARTUPINFO si2;
	ZeroMemory(&si2, sizeof(STARTUPINFO));
	PROCESS_INFORMATION piApp;
	PROCESS_INFORMATION piApp2;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwY = 500;
	si.dwFlags = STARTF_USEPOSITION;
	CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp);

	std::cout << "Создан новый процесс.\n";

	WaitForSingleObject(piApp.hProcess, INFINITE);

	TerminateProcess(piApp.hProcess, 1);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);

	CreateProcess(NULL, lpszAppName2, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si2, &piApp2);

	std::cout << "Создан новый процесс.\n";

	SetThreadPriority(piApp2.hProcess, HIGH_PRIORITY_CLASS);
	WaitForSingleObject(piApp2.hProcess, INFINITE);
	CloseHandle(piApp2.hThread);
	CloseHandle(piApp2.hProcess);
	return 0;
}
