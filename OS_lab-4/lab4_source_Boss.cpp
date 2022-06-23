#include <iostream>
#include<stdio.h>
#include<string.h>
#include <windows.h>
#include <tchar.h>
#include <conio.h>

int main()
{
	setlocale(LC_ALL, "rus");

	HANDLE hMutex = CreateMutex(NULL, FALSE, L"hM");
	HANDLE A = CreateEvent(NULL, FALSE, FALSE, L"A");
	HANDLE B = CreateEvent(NULL, FALSE, FALSE, L"B");
	HANDLE C = CreateEvent(NULL, FALSE, FALSE, L"C");
	HANDLE D = CreateEvent(NULL, FALSE, FALSE, L"D");


	TCHAR lpszCommandLine1[256] = L"Parent.exe";
	TCHAR lpszCommandLine2[256] = L"Child.exe";

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));

	int ps, cs, ms;

	std::cout << "Количество процессоров Parent" << "\n";
	std::cin >> ps;
	std::cout << "Количество процессоров  Child" << "\n";
	std::cin >> cs;
	std::cout << "Количество сообщений, принятых от Parent или Child" << "\n";
	std::cin >> ms;

	HANDLE semaphore = CreateSemaphore(NULL, 2, 2, L"S");
	PROCESS_INFORMATION *piApp = new PROCESS_INFORMATION[ps + cs];
	HANDLE *piH = new HANDLE[ps + cs];

	int pi = 0;
	TCHAR buf[300];
	_stprintf_s(buf, TEXT(" %d"), ms);
	_tcscat_s(lpszCommandLine1, buf);
	_tcscat_s(lpszCommandLine2, buf);
	for (int i = 0; i < ps; i++) {
		if (!CreateProcess(NULL, lpszCommandLine1, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp[pi]))
		{
			std::cout << "Новый процесс не создается.\n";
			std::cout << "Проверьте имя процесса.\n";
			std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
			_getch();
			return 0;
		}
		piH[pi] = piApp[pi].hProcess;
		pi++;
	}
	for (int i = 0; i < cs; i++) {
		if (!CreateProcess(NULL, lpszCommandLine2, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp[pi]))
		{
			std::cout << "Новый процесс не создается.\n";
			std::cout << "Проверьте имя процесса.\n";
			std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
			_getch();
			return 0;
		}
		piH[pi] = piApp[pi].hProcess;
		pi++;
	}
	HANDLE *hMas = new HANDLE[4];
	for (int i = 0; i < (ps+cs)*ms; i++) {
		hMas[0] = A;
		hMas[1] = B;
		hMas[2] = C;
		hMas[3] = D;
		DWORD dw = WaitForMultipleObjects(4, hMas, FALSE, INFINITE);
		if (dw == WAIT_OBJECT_0 + 0) {
			std::cout << "Process Parent: A" << "\n";
		}
		if (dw == WAIT_OBJECT_0 + 1) {
			std::cout << "Process Parent: B" << "\n";
		}
		if (dw == WAIT_OBJECT_0 + 2) {
			std::cout << "Process Child: C" << "\n";
		}
		if (dw == WAIT_OBJECT_0 + 3) {
			std::cout << "Process Child: D" << "\n";
		}
	}

	WaitForMultipleObjects(ps + cs, piH, TRUE, INFINITE);
	for (int i = 0; i < ps + cs; i++) {
		CloseHandle(piApp[i].hThread);
		CloseHandle(piApp[i].hProcess);
	}

	CloseHandle(hMutex);
	CloseHandle(semaphore);
	delete[] piH;

	return 0;
}