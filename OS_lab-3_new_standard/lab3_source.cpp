#include <iostream>
#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>

void printMas(int* mas, int ssize) {
    for (int i = 0; i < ssize; i++) {
        std::cout << mas[i] << " ";
    }
    std::cout << "\n";
}

void writeMas(int* mas, int ssize) {
    for (int i = 0; i < ssize; i++) {
        std::cin >> mas[i];
    }
}

void Work(std::mutex& m1, int& ttime, int* mas, int k, int ssize)
{
	std::unique_lock<std::mutex> lk1(m1, std::defer_lock);
	lk1.lock();
	std::cout << "Введите временной интервал, требуемый для отдых" << "\n";
	std::cin >> ttime;
	bool key = false;
	int rs = 0;
	bool r = true;
	for (int i = 0; i < ssize; i++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ttime));
		key = false;
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
			if (rs != i)
			{
				int t = mas[rs];
				mas[rs] = mas[i];
				mas[i] = t;
				rs++;
			}
			else
				rs++;
		}
		if (rs == k && r) {
			r = false;
			lk1.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			lk1.lock();
		}
	}
	lk1.unlock();
}

void SumElement(std::mutex& m2, std::atomic<int>& ready_flag, int& ttime, int* mas, int k, int& sum) {
	std::unique_lock<std::mutex> lk1(m2, std::defer_lock);
	lk1.lock();
	while (ready_flag.load(std::memory_order_acquire) == 0) {
		std::this_thread::yield();
	}
	for (int i = 0; i < k; i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ttime));
		sum += mas[i];
	}
	lk1.unlock();
}

int main()
{
	setlocale(LC_ALL, "RUS");
	srand(time(0));

	int k, sum = 0, ssize, ttime;
	auto Imas = std::make_unique<int>();
	int* mas = Imas.get();
	std::mutex m1, m2;
	std::atomic<int> ready_flag = 0;

    std::cout << "Введите размер массива" << "\n";
    std::cin >> ssize;
    mas = new int[ssize];

    std::cout << "Введите массив размерности " << ssize << "\n";
    writeMas(mas, ssize);

    std::cout << "Исходный массив размерности " << ssize << "\n";
    printMas(mas, ssize);

    std::cout << "Bведите k" << "\n";
    std::cin >> k;

	std::thread tWork([&m1, &ttime, mas, k, ssize]() {
		Work(m1, ttime, mas, k, ssize);
		});

	std::unique_lock<std::mutex> lk1(m1, std::defer_lock);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	lk1.lock();

	std::thread tSumElement([&m2, &ready_flag, &ttime, mas, k, &sum]() {
		SumElement(m2, ready_flag, ttime, mas, k, sum);
    });
	ready_flag.store(1, std::memory_order_release);

	std::cout << "Промежуточный вывод " << "\n";
	printMas(mas, ssize);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	lk1.unlock();
	std::unique_lock<std::mutex> lk2(m2, std::defer_lock);
	lk2.lock();
	std::cout << "результат работы потока SumElement: sum = " << sum << "\n";
	lk2.unlock();

	lk1.lock();
	std::cout << "Итоговый массив" << "\n";
	printMas(mas, ssize);
	lk1.unlock();

	tWork.join();
	tSumElement.join();
	return 0;
}