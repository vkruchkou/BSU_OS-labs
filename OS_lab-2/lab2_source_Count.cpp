#include <math.h>
#include <windows.h>
#include <iostream>
#include <conio.h>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	int* mass = new int[20];
	std::cout << "����� ���������\n";
	mass[0] = 0;
	mass[1] = 1;
	std::cout << mass[0] << " ";
	std::cout << mass[1] << " ";
	for (int i = 2; i < 20; i++)
	{
		mass[i] = mass[i - 1] + mass[i - 2];
		std::cout << mass[i] << " ";
	}

	std::cout << "\n";
	std::cout << "������� ����� �������, ����� ���������.\n";

	delete[] mass;
	_getch();
	ExitProcess(1);
	return 0;
}