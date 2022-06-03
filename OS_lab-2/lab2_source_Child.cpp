#include <conio.h>
#include <math.h>
#include <windows.h>
#include <iostream>

void printMas(int* mas, int ssize) {
	for (int i = 0; i < ssize; i++) {
		std::cout << mas[i] << " ";
	}
	std::cout << "\n";
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "rus");

	int *m = new int[argc - 1];

	for (int i = 1; i < argc; i++)
		m[i - 1] = atoi(argv[i]);

	std::cout << "\nИсходный массив\n";
	printMas(m, argc - 1);

	bool key = false;
	int rs = 0;
	bool r = true;
	for (int i = 0; i < argc - 1; i++)
	{
		key = false;
		for (int j = 2; j <= sqrt((double)m[i]); j++)
		{
			if (m[i] % j == 0 && m[i] != j)
			{
				key = true;
				break;
			}
		}
		if (m[i] == 1)
			key = true;
		if (key == false) {
			if (rs != i)
			{
				int t = m[rs];
				m[rs] = m[i];
				m[i] = t;
				rs++;
			}
			else
				rs++;
		}
		if (r) {
			r = false;
		}
	}
	std::cout << "\nПолученный массив\n";
	printMas(m, argc - 1);

	delete[] m;

	std::cout << "Нажмите любую клавишу, чтобы завершить.\n";

	_getch();
    ExitProcess(1);
	return 0;
}