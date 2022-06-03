#include <windows.h>
#include <conio.h>
#include <iostream>

__int32 getRandomInt32() {
	return __int32(-100 + rand() % 200);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");
	HANDLE writePipe = (HANDLE)atoi(argv[1]);
	HANDLE readPipe = (HANDLE)atoi(argv[2]);

	HANDLE R = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"R");
	HANDLE SR = OpenEvent(EVENT_ALL_ACCESS, TRUE, L"SR");

	std::cout << "В ожидание получения размерности массива" << "/n";
	WaitForSingleObject(R, INFINITE);
	int n;
	DWORD dwBytesRead;
	if (!ReadFile(readPipe, &n, sizeof(n), &dwBytesRead, NULL))
	{
		std::cout << "Чтение из канала не удалось.\n";
		std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
		_getch();
		return 0;
	}
	system("cls");

	std::cout << "Simple получил размер массива: " << n << "/n";
	_int32* mas = new _int32[n];
	std::cout << "Сгенерированный массив" << "/n";

	srand(time(0));
	for (int i = 0; i < n; i++) {
		mas[i] = getRandomInt32();
		std::cout << mas[i] << " ";
	}

	SetEvent(SR);

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
			std::cout << mas[i] << endl;
			DWORD dwBytesWritten;
			if (!WriteFile(writePipe, &mas[i], sizeof(mas[i]), &dwBytesWritten, NULL))
			{
				std::cout << "Не удалось выполнить запись в файл.\n";
				std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
				_getch();
				return GetLastError();
			}
			SetEvent(SR);
			WaitForSingleObject(R, INFINITE);
		}
	}


	CloseHandle(writePipe);
	CloseHandle(readPipe);
	delete[] mas;

	std::cout << "\nВсе найденные числа были отправлены\n";
	std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
	_getch();
	return 0;
}