#include <windows.h>
#include <conio.h>
#include <iostream>

int main()
{
	setlocale(LC_ALL, "rus");
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	HANDLE hNamedPipe;
	DWORD dwBytesRead;
	DWORD dwBytesWrite;

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;

	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;

	hNamedPipe = CreateNamedPipe(L"\\\\.\\pipe\\demo_pipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, &sa);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Сбой создания именованного канала.\n" <<
			"Последний код ошибки: \n" << GetLastError();
		std::cout << "Нажмите любой символ, чтобы завершить клиент : \n";
		_getch();
		return 0;
	}

	std::cout << "Сервер ожидает соединения с клиентом." << "\n";

	if (!ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL))
	{
		std::cerr << "Соединение не удалось установить." << "\n"
			<< "Последний код ошибки: " << GetLastError() << "\n";
		CloseHandle(hNamedPipe);
		std::cout << "Press any char to finish the server: ";
		_getch();
		return 0;
	}

	int n;
	std::cout << "Введите размер массива" << "\n";
	std::cin >> n;

	DWORD dwBytesWritten;

	if (!WriteFile(hNamedPipe, &n, sizeof(n), &dwBytesWrite, (LPOVERLAPPED)NULL))
	{
		std::cerr << "Не удалось выполнить запись в именованный канал: " << "\n"
			<< "Последний код ошибки: \n" << GetLastError();
		std::cout << "Нажмите любой символ, чтобы завершить клиент : \n";
		_getch();
		return 0;
	}

	std::cout << "\n";
	std::cout << "Server отправил размер массива: " << n << "\n";
	std::cout << "\n";

	int k = 0;
	_int32 nData = 1;

	while (true) {
		k++;
		DWORD dwBytesRead;
		if (!ReadFile(hNamedPipe, &nData, sizeof(nData), &dwBytesRead, (LPOVERLAPPED)NULL))
		{
			std::cerr << "Не удалось выполнить чтение в именованный канал: " <<
				"Последний код ошибки: \n" << GetLastError();
			std::cout << "Нажмите любой символ, чтобы завершить клиент : \n";
			CloseHandle(hNamedPipe);
			_getch();
			return 0;
		}
		if (nData == 0)
			break;
		std::cout << "Полученное число от Simple " << nData << "\n";
	}

	if (k == 0) {
		std::cout << "\n";
		std::cout << "Ничего не было получено" << "\n";
	}

	else {
		std::cout << "\n";
		std::cout << "Все числа были получены" << "\n";
	}

	CloseHandle(hNamedPipe);
	std::cout << "Нажмите любую клавишу для выхода.\n";
	_getch();
	return 0;
}