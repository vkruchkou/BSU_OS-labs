#include <windows.h>
#include <conio.h>
#include <iostream>

int main()
{
	setlocale(LC_ALL, "rus");
	TCHAR lpszComLine[80];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	HANDLE hWritePipe, hReadPipe;
	HANDLE R = CreateEvent(NULL, FALSE, FALSE, L"R");
	HANDLE SR = CreateEvent(NULL, FALSE, FALSE, L"SR");
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		std::cout << "Не удалось создать канал.\n";
		std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
		_getch();
		return GetLastError();
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	wsprintf(lpszComLine, L"Simple.exe %d %d", (int)hWritePipe, (int)hReadPipe);
	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		std::cout << "Сбой процесса создания.\n";
		std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
		_getch();
		return GetLastError();
	}

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	int n;
	std::cout << "Введите размер массива" << "\n";
	std::cin >> n;
	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, &n, sizeof(n), &dwBytesWritten, NULL))
	{
		std::cout << "Не удалось выполнить запись в файл.\n";
		std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
		_getch();
		return GetLastError();
	}
	std::cout << "Server отправил размер массива: " << n << "\n";

	SetEvent(R);

	int k = 0;
	WaitForSingleObject(SR, INFINITE);
	while (WaitForSingleObject(SR, 1000) == false) {
		k++;
		_int32 nData;
		DWORD dwBytesRead;
		if (!ReadFile(hReadPipe, &nData, sizeof(nData), &dwBytesRead, NULL))
		{
			std::cout << "Чтение из канала не удалось.\n";
			std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
			_getch();
			return GetLastError();
		}
		std::cout << "Полученное число от Simple " << nData << "\n";
		SetEvent(R);
	}
	if (0 == k) {
		std::cout << "Ничего не было получено" << "\n";
	}
	else {
		std::cout << "Все числа были получены" << "\n";
	}
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	std::cout << "Press any key to exit.\n";
	_getch();
	return 0;
}