#include <windows.h>
#include <conio.h>
#include <iostream>
#include <time.h>

class MonitorStack
{
private:
	int size;
	__int32* stack;
	int top;
public:
	MonitorStack() {
		stack = nullptr;
		top = 0;
		size = 0;
	}
	MonitorStack(int nSize) {
		size = nSize;
		stack = new __int32(size);
		top = 0;
	}
	~MonitorStack() {
		delete[] stack;
	}

	void addTop();
	void delTop();
	void Push(__int32& nElement);
	__int32 Pop();
};

void MonitorStack::addTop() {
	top++;
}
void MonitorStack::delTop() {
	top--;
}
void MonitorStack::Push(__int32& nElement) {
		stack[top] = nElement;
		addTop();
}

__int32 MonitorStack::Pop()
{
	__int32 nElement;
	delTop();
	nElement = stack[top];
	return nElement;
}

__int32 getRandomElement() {
	return __int32(-100 + rand() % 200);
}


class dataForThread{
public:
	int count;
	int iterator;
	HANDLE hAccess;
	HANDLE hFull;
	HANDLE hEmpty;
	MonitorStack *stack;

	dataForThread(int count, int size, HANDLE hAccess, HANDLE hFull, HANDLE hEmpty, MonitorStack*& stack)
	{
		iterator = -1;
		this->count = count;
		this->hAccess = hAccess;
		this->hFull = hFull;
		this->hEmpty = hEmpty;
		this->stack = stack;
	}

	~dataForThread() {
	}
};

DWORD WINAPI producer(LPVOID lpParam)
{
	dataForThread* param = (dataForThread*)lpParam;
	__int32 element;
	for (int i = 0; i < param->count; i++) {
		WaitForSingleObject(param->hFull, INFINITE);
		WaitForSingleObject(param->hAccess, INFINITE);
		element = getRandomElement();
		std::cout << "\nПроизведен элемент: " << element;
		param->stack->Push(element);
		ReleaseMutex(param->hAccess);
		ReleaseSemaphore(param->hEmpty, 1, NULL);
	}
	return 0;
}

DWORD WINAPI consumer(LPVOID lpParam)
{
	dataForThread* param = (dataForThread*)lpParam;
	for (int i = 0; i < param->count; i++) {
		WaitForSingleObject(param->hEmpty, INFINITE);
		WaitForSingleObject(param->hAccess, INFINITE);
		std::cout << "\nУпотреблен элемент: " << param->stack->Pop();
		ReleaseMutex(param->hAccess);
		ReleaseSemaphore(param->hFull, 1, NULL);
	}
	return 0;
}

struct ThreadData {
	HANDLE handle;
	DWORD id;
};

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Rus");
	int sizeStack, countProducer, countConsumer;
	std::cout << "Введите размер стека: ";
	std::cin >> sizeStack;

	std::cout << "Введите количество потоков producer: ";
	std::cin >> countProducer;

	int* producerProductCountList = new int[countProducer];
	for (int i = 0; i < countProducer; i++) {
		std::cout << "Введите количество элементов для " << i + 1 << "-ого потока producer:\n";
		int producerProductCount;
		std::cin >> producerProductCount;

		producerProductCountList[i] = producerProductCount;
	}

	std::cout << "Введите количество потоков consumer: ";
	std::cin >> countConsumer;

	int* consumerProductCountList = new int[countConsumer];
	for (int i = 0; i < countConsumer; i++) {
		std::cout << "Введите количество элементов для " << i + 1 << "-ого потока consumer:\n";
		int consumerProductCount;
		std::cin >> consumerProductCount;

		consumerProductCountList[i] = consumerProductCount;
	}

	HANDLE* hProducer = new HANDLE[countProducer];
	HANDLE* hConsumer = new HANDLE[countConsumer];

	DWORD* IDProducer = new DWORD[countProducer];
	DWORD* IDConsumer = new DWORD[countConsumer];

	HANDLE hAccess = CreateMutex(NULL, FALSE, L"hAccess");
	HANDLE hFull = CreateSemaphore(NULL, sizeStack, sizeStack, L"hFull");
	HANDLE hEmpty = CreateSemaphore(NULL, 0, sizeStack, L"hEmpty");

	MonitorStack *st = new MonitorStack(sizeStack);
	

	for (int i = 0; i < countProducer; i++) {
		dataForThread* param = new dataForThread(producerProductCountList[i], sizeStack, hAccess, hFull, hEmpty, st);
		hProducer[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)producer, (void*&)param, 0, &IDProducer[i]);
		if (NULL == hProducer[i]) {
			return GetLastError();
		}
	}

	for (int i = 0; i < countConsumer; i++) {
		dataForThread* param = new dataForThread(consumerProductCountList[i], sizeStack, hAccess, hFull, hEmpty, st);
		hConsumer[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)consumer, (void*&)param, 0, &IDConsumer[i]);
		if (NULL == hConsumer[i]) {
			return GetLastError();
		}
	}

	WaitForMultipleObjects(countProducer, hProducer, TRUE, INFINITE);
	WaitForMultipleObjects(countConsumer, hConsumer, TRUE, INFINITE);

	CloseHandle(hAccess);
	CloseHandle(hFull);
	CloseHandle(hEmpty);

	for (int i = 0; i < countProducer; i++) {
		CloseHandle(hProducer[i]);
	}
	for (int i = 0; i < countConsumer; i++) {
		CloseHandle(hConsumer[i]);
	}
	delete[] producerProductCountList;
	delete[] consumerProductCountList;
	delete[] IDConsumer;
	delete[] IDProducer;
	delete[] hConsumer;
	delete[] hProducer;

	return 0;
}