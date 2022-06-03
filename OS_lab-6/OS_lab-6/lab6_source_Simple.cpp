#include <windows.h>
#include <conio.h>
#include <iostream>

__int32 getRandomInt32() {
	return __int32(-100 + rand() % 200);
}


int main()
{
	setlocale(LC_ALL, "rus");
	HANDLE hNamedPipe;
	DWORD dwBytesWritten;
	
	char machineName[256];
	TCHAR pipeName[256];
	std::cout << "������� ��� ��������� ������: ";
	std::cin >> machineName;
	wsprintf(pipeName, L"\\\\%s\\pipe\\demo_pipe",
		machineName);

	hNamedPipe = CreateFile(pipeName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		std::cerr << "���� ���������� � ����������� �������.\n" <<
			"��������� ��� ������: \n" << GetLastError();
		std::cout << "������� ����� ������, ����� ��������� ������ : \n";
		_getch();
		return 0;
	}

	int n;
	std::cout << "� �������� ��������� ����������� �������\n";

	DWORD dwBytesRead;
	if (!ReadFile(hNamedPipe, &n, sizeof(n), &dwBytesRead, (LPOVERLAPPED)NULL))
	{
		std::cerr << "�� ������� ��������� ������ � ����������� �����: " <<
			"��������� ��� ������: \n" << GetLastError();
		std::cout << "������� ����� ������, ����� ��������� ������ : \n";
		_getch();
		CloseHandle(hNamedPipe);
		return 0;
	}

	system("cls");
	std::cout << "Simple ������� ������ �������: " << n << "\n";
	_int32* mas = new _int32[n];
	std::cout << "��������������� ������" << "\n";


	srand(time(0));
	for (int i = 0; i < n; i++) {
		mas[i] = getRandomInt32();
		std::cout << mas[i] << " ";
	}
	std::cout << "\n" << "\n";


	bool key;
	bool keyl;
	int rs = 0;
	for (int i = 0; i < n; i++)
	{
		key = false;
		keyl = false;
		if (mas[i] < 0) {
			mas[i] = mas[i] * (-1);
			keyl = true;
		}
		for (int j = 2; j <= sqrt((double)mas[i]); j++)
		{
			if (mas[i] % j == 0 && mas[i] != j)
			{
				key = true;
				break;
			}
		}
		if (mas[i] == 1)
			key = true;
		if (key == false) {
			if (keyl == true) {
				mas[i] = mas[i] * (-1);
			}
			std::cout << "����� " << mas[i] << " ����������" << "\n";
			DWORD dwBytesWritten;
			if (!WriteFile(hNamedPipe, &mas[i], sizeof(mas[i]), &dwBytesWritten, (LPOVERLAPPED)NULL))
			{
				std::cerr << "�� ������� ��������� ������ � ����������� �����: " << "\n"
					<< "��������� ��� ������: \n" << GetLastError();
				std::cout << "������� ����� ������, ����� ��������� ������ : \n";
				_getch();
				CloseHandle(hNamedPipe);
				return 0;
			}
		}
	}
	int finish = 0;
	if (!WriteFile(hNamedPipe, &finish, sizeof(finish), &dwBytesWritten, (LPOVERLAPPED)NULL))
	{
		std::cerr << "�� ������� ��������� ������ � ����������� �����: " << "\n"
			<< "��������� ��� ������: \n" << GetLastError();
		std::cout << "������� ����� ������, ����� ��������� ������ : \n";
		_getch();
		CloseHandle(hNamedPipe);
		return 0;
	}
	std::cout << "\n��� ��������� ����� ���� ����������\n";


	CloseHandle(hNamedPipe);
	delete[] mas;

	std::cout << "������� ����� ������� ��� ������.\n";
	_getch();
	return 0;
}