#include <math.h>
#include <iostream>
#include <conio.h>
#include <boost/array.hpp>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	boost::array<int, 20> mass;
	std::cout << "Числа Фибоначчи\n";
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
	std::cout << "Нажмите любую клавишу, чтобы завершить.\n";
	_getch();	
	return 0;
}