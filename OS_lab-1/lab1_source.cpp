#include <windows.h>
#include <process.h>
#include <iostream>

int getRandomInt() {
	return -100 + rand() % 200;
}

void printMas(int* mas, int ssize) {
	for (int i = 0; i < ssize; i++) {
		std::cout << mas[i] << " ";
	}
	std::cout << "\n";
}

struct Data {
	int size;
	int* mas;
	int x;
};

UINT WINAPI worker(LPVOID par)
{
	Sleep(200);

	Data* param = (Data*)par;
	int n = param->size;
	int* mas = param->mas;
	int x = param->x;

	int k = 0;
	for (int i = 0; i < n; i++) {
		if (mas[i] > x) {
			k++;
		}
	}
	std::cout << "Количество элементов больших X = " << k << "\n";
	return 0;
}

DWORD WINAPI Count()
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
	return 0;
}

DWORD WINAPI threadMain(LPVOID par)
{
	int n, x;
	int* mas;
	HANDLE hThread1, hThread2;
	DWORD IDThread1;
	UINT IDThread2;

	n = (int)par;
	mas = new int[n];
	
	srand(time(0));
	for (int i = 0; i < n; i++) {
		mas[i] = getRandomInt();
	}

	std::cout << "Сгенерированный массив" << "\n";
	printMas(mas, n);

	std::cout << "Введите элемент Х" << "\n";
	std::cin >> x;

	Data m;
	m.size = n;
	m.mas = mas;
	m.x = x;

	hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Count, 0, CREATE_SUSPENDED, &IDThread1);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, worker, (void*)&m, 0, &IDThread2);
	
	SuspendThread(worker);
	ResumeThread(worker);

	WaitForSingleObject(hThread2, INFINITE);
	
	ResumeThread(hThread1);

	WaitForSingleObject(hThread1, INFINITE);

	CloseHandle(hThread2);
	CloseHandle(hThread1);

	delete[] mas;

	return 0;
}


int main()
{
	setlocale(LC_ALL, "rus");

	HANDLE hThread;
	DWORD IDThread;
	int n;

	std::cout << "Введите размерность массива" << "\n";
	std::cin >> n;

	hThread = CreateThread(NULL, 0, threadMain, (void*)n, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	return 0;
}