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

	HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"S");
	HANDLE C = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"C");
	HANDLE D = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"D");

	std::cout << "Процесс ожидает" << "\n";
	WaitForSingleObject(semaphore, INFINITE);
	system("cls");
	std::cout << "Процесс работает" << "\n";
	for (int i = 0; i < ms; i++) {
		cout << "C or D\n";
		char input = ' ';
		while (input != 'C' && input != 'D') {
			cin >> input;
			if (input == 'C') {
				SetEvent(C);
				break;
			}
			if (input == 'D') {
				SetEvent(D);
				break;
			}
			if (input != 'C' && input != 'D')
				std::cout << "Неверный ввод" << "\n";
		}
	}
	std::cout << "\nНажмите любую клавишу, чтобы завершить.\n";
	_getch();

	ReleaseSemaphore(semaphore, 1, NULL);
	ExitProcess(1);

	return 0;
}