#include <conio.h>
#include <math.h>
#include <iostream>
#include <vector>

void printMas(std::vector<int>& mas) {
	for (auto i : mas)
		std::cout << i << ' ';
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	std::vector<int> m;
	for (int i = 1; i < argc; i++) {
		m.push_back(atoi(argv[i]));
	}
	std::cout << "\nИсходный массив\n";
	printMas(m);

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
	printMas(m);

	std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
	_getch();
	return 0;
}