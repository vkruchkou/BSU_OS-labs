#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <iostream>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	int ms = atoi(argv[1]);

	std::cout << "Процесс ожидает" << "\n";

	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"hM");
	HANDLE A = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"A");
	HANDLE B = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"B");

	WaitForSingleObject(hMutex, INFINITE);
	system("cls");
	std::cout << "Процесс работает" << "\n";
	for (int i = 0; i < ms; i++) {
		std::cout << "А или В\n";
		char input = ' ';
		while (input != 'A' && input != 'B') {
			std::cin >> input;
			if (input == 'A') {
				SetEvent(A);
				break;
			}
			if (input == 'B') {
				SetEvent(B);
				break;
			}
			if(input != 'A' && input != 'B')
				std::cout << "Неверный ввод" << "\n";
		}
	}
	std::cout << "\nНажмите любую клавишу, чтобы завершить.\n";
	_getch();

	ReleaseMutex(hMutex);
	ExitProcess(1);

	return 0;
}