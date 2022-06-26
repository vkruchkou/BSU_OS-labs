#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/foreach.hpp>
#include <boost/array.hpp>
#include <vector>
#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"

struct Random {
private:
	typedef boost::mt19937 RNGType;
	RNGType rng;
	boost::variate_generator< RNGType, boost::uniform_int<> > *dice;
	
public:
	Random(){
	boost::uniform_int<> newRange(-100, 100);
	boost::variate_generator< RNGType, boost::uniform_int<> >
		newDice(rng, newRange);
	dice = &newDice;
	}
	int getRandomInt() {
		return (* dice)();
	}
};
int getRandomInt() {
	return -100 + rand() % 200;
}

void printMas(std::vector<int> &mas) {
	for (auto i : mas)
		std::cout << i << ' ';
	std::cout << "\n";
}

int main() {
	setlocale(LC_ALL, "Rus");

	std::vector<int> mas;
	int n, x;

	std::cout << "Введите размерность массива" << "\n";
	std::cin >> n;

	Random rd;
	for (int i = 0; i < n; i++) {
		mas.push_back(rd.getRandomInt());
	}

	std::cout << "Сгенерированный массив" << "\n";
	printMas(mas);

	std::cout << "Введите элемент Х" << "\n";
	std::cin >> x;

	boost::thread tWorker([&]()
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(200));

			int k = 0;
			for (int i = 0; i < n; i++) {
				if (mas[i] > x) {
					k++;
				}
			}

			std::cout << "Количество элементов больших X = " << k << "\n";
		});

	tWorker.join();

	boost::thread tCount([&]() 
		{
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
		});
	tCount.join();
	return 0;
}