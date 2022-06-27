#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

class Semaphore {
public:
	Semaphore(int count_ = 0)
		: count(count_) {}

	inline void notify()
	{
		std::unique_lock<std::mutex> lock(mtx);
		count++;
		cv.notify_one();
	}

	inline void wait()
	{
		std::unique_lock<std::mutex> lock(mtx);

		while (count == 0) {
			cv.wait(lock);
		}
		count--;
	}

private:
	std::mutex mtx;
	std::condition_variable cv;
	int count;
};

__int32 getRandomInt32() {
	return __int32(-100 + rand() % 200);
}

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

void producer(int count, std::mutex& m1, Semaphore& full, Semaphore& empty, MonitorStack*& stack)
{
	__int32 element;
	for (int i = 0; i < count; i++) {
		full.wait();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		m1.lock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		element = getRandomInt32();
		std::cout << "\nПроизведен элемент: " << element;
		stack->Push(element);
		m1.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		empty.notify();
	}
}

void consumer(int count, std::mutex& m1, Semaphore& full, Semaphore& empty, MonitorStack*& stack)
{
	for (int i = 0; i < count; i++) {
		empty.wait();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		m1.lock();
		std::cout << "\nУпотреблен элемент: " << stack->Pop();
		m1.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		full.notify();
	}
}

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Rus");
	int sizeStack, countProducer, countConsumer;
	std::cout << "Введите размер стека: ";
	std::cin >> sizeStack;

	std::cout << "Введите количество потоков producer: ";
	std::cin >> countProducer;

	std::vector<int> producerProductCountList(countProducer);
	for (int i = 0; i < countProducer; i++) {
		std::cout << "Введите количество элементов для " << i + 1 << "-ого потока producer:\n";
		int producerProductCount;
		std::cin >> producerProductCount;

		producerProductCountList[i] = producerProductCount;
	}

	std::cout << "Введите количество потоков consumer: ";
	std::cin >> countConsumer;

	std::vector<int> consumerProductCountList(countConsumer);
	for (int i = 0; i < countConsumer; i++) {
		std::cout << "Введите количество элементов для " << i + 1 << "-ого потока consumer:\n";
		int consumerProductCount;
		std::cin >> consumerProductCount;

		consumerProductCountList[i] = consumerProductCount;
	}

	std::mutex m1;
	std::unique_lock<std::mutex> lk1(m1, std::defer_lock);
	Semaphore full(sizeStack);
	Semaphore empty;
	std::thread** tProducer = new std::thread* [countProducer];
	std::thread** tConsumer = new std::thread* [countConsumer];
	MonitorStack* st = new MonitorStack(sizeStack);

	for (int i = 0; i < countProducer; i++) {
		int c = producerProductCountList[i];
		tProducer[i] = new std::thread([c, &m1, &full, &empty, &st]() {
			producer(c, m1, full, empty, st);
		});
	}
	for (int i = 0; i < countConsumer; i++) {
		int c = producerProductCountList[i];
		tConsumer[i] = new std::thread([c, &m1, &full, &empty, &st]() {
			consumer(c, m1, full, empty, st);
			});
	}
	for (int i = 0; i < countProducer; i++) {
		tProducer[i]->join();
	}
	for (int i = 0; i < countConsumer; i++) {
		tConsumer[i]->join();
	}
	for (int i = 0; i < countProducer; i++) {
		delete tProducer[i];
	}
	for (int i = 0; i < countConsumer; i++) {
		delete tConsumer[i];
	}
	return 0;
}