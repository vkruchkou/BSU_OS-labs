#include <Windows.h>
#include <iostream>
#include <ctime>
#include <process.h>

CRITICAL_SECTION cs;
CRITICAL_SECTION cs2;
HANDLE sumElement;
HANDLE hOutEvent;
int k;
int sum;
int ssize;
int* mas;
int ttime;

void writeMas() {
	for (int i = 0; i < ssize; i++) {
		std::cin >> mas[i];
	}
}

void printMas() {
	for (int i = 0; i < ssize; i++) {
		std::cout << mas[i] << " ";
	}
	std::cout << "\n";
}

DWORD WINAPI Work(LPVOID par)
{
	EnterCriticalSection(&cs);
	std::cout << "Введите временной интервал, требуемый для отдых" << "\n";
	std::cin >> ttime;
	bool key = false;
	int rs = 0;
	bool r = true;
	for (int i = 0; i < ssize; i++)
	{
		Sleep(ttime);
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
			LeaveCriticalSection(&cs);
			Sleep(100);
			EnterCriticalSection(&cs);
		}
	}	
	LeaveCriticalSection(&cs);
	return 0;
}

DWORD WINAPI SumElement(LPVOID par) {
	EnterCriticalSection(&cs2);
	WaitForSingleObject(hOutEvent, INFINITE);
	for (int i = 0; i < k; i++) {
		Sleep(ttime);
		sum += mas[i];
	}
	LeaveCriticalSection(&cs2);
	return 0;
}

int main()
{
	setlocale(LC_ALL, "RUS");

	HANDLE work, sumElement;
	DWORD IDwork, IDs;
	HANDLE hOutEvent;

	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&cs2);

	std::cout << "Введите размер массива" << "\n";
	std::cin >> ssize;

	mas = new int[ssize];

	std::cout << "Введите массив размерности " << ssize << "\n";
	writeMas();

	std::cout << "Исходный массив размерности " << ssize << "\n";
	printMas();
	

	work = CreateThread(NULL, 0, Work, NULL, CREATE_SUSPENDED, &IDwork);
	if (work == NULL) return GetLastError();
	hOutEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (hOutEvent == NULL)
		return GetLastError();
	sumElement = CreateThread(NULL, 0, SumElement, NULL, CREATE_SUSPENDED, &IDs);
	if (sumElement == NULL) return GetLastError();

	std::cout << "Bведите k" << "\n";
	std::cin >> k;

	ResumeThread(work);

	Sleep(5);
	EnterCriticalSection(&cs);

	ResumeThread(sumElement);
	SetEvent(hOutEvent);

	std::cout << "Промежуточный вывод " << "\n";
	printMas();

	LeaveCriticalSection(&cs);
	EnterCriticalSection(&cs2);

	std::cout << "результат работы потока SumElement: sum = " << sum << "\n";

	LeaveCriticalSection(&cs2);
	DeleteCriticalSection(&cs2);
	EnterCriticalSection(&cs);

	std::cout << "Итоговый массив" << "\n";
	printMas();

	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
	delete[] mas;

	return 0;
}