#include <iostream>
#include <thread>
#include <chrono>
#include<ctime>

int getRandomInt() {
	return -100 + rand() % 200;
}

void printMas(int* mas, int ssize) {
	for (int i = 0; i < ssize; i++) {
		std::cout << mas[i] << " ";
	}
	std::cout << "\n";
}

void worker(int size, int* mas, int x) {

	std::this_thread::sleep_for(std::chrono::milliseconds(200));

	int k = 0;
	for (int i = 0; i < size; i++) {
		if (mas[i] > x) {
			k++;
		}
	}

	std::cout << "Количество элементов больших X = " << k << "\n";
	return;
}

void count()
{
	int* mass = new int[20];
	mass[0] = 0;

	std::cout << "Числа Фибоначчи" << "\n";

	std::cout << mass[0] << " ";
	mass[1] = 1;
	std::cout << mass[1] << " ";
	for (int i = 2; i < 20; i++)
	{
		mass[i] = mass[i - 1] + mass[i - 2];
		std::cout << mass[i] << " ";
	}
	std::cout << "\n";
	delete[] mass;
	return;
}

int main() {
	setlocale(LC_ALL, "Rus");
	srand(time(0));

	auto Imas = std::make_unique<int>();
	int* mas = Imas.get();
	int n,x;

	std::cout << "Введите размерность массива" << "\n";
	std::cin >> n;

	mas = new int[n];

	srand(time(0));
	for (int i = 0; i < n; i++) {
		mas[i] = getRandomInt();
	}

	std::cout << "Сгенерированный массив" << "\n";
	printMas(mas, n);

	std::cout << "Введите элемент Х" << "\n";
	std::cin >> x;

	std::thread tWorker(worker, n, mas, x);
	tWorker.join();

	std::thread tCount(count);
	tCount.join();
	return 0;
}