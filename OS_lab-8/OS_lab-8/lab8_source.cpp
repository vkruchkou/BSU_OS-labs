#include <windows.h>
#include <conio.h>
#include <iostream>

class cnFreeSeat {
private:
	int nFreeSeat;
public:
	cnFreeSeat(int nFreeSeat) {
		this->nFreeSeat = nFreeSeat;
	}

	void add();
	void del();
	int get();
};

void cnFreeSeat::add() {
	nFreeSeat++;
}
void cnFreeSeat::del() {
	nFreeSeat--;
}
int cnFreeSeat::get() {
	std::cout << "Колиество свободных мест "<< nFreeSeat <<"\n";
	return nFreeSeat;
}


class dataForThread {
public:
	cnFreeSeat* nFreeSeat;
	HANDLE hAccessSeat;
	HANDLE hCustomer;
	HANDLE hBarber;
	int number;

	dataForThread(HANDLE hAccessSeat, HANDLE hCustomer, HANDLE hBarber, cnFreeSeat*& nFreeSeat, int number)
	{
		this->hAccessSeat = hAccessSeat;
		this->hCustomer = hCustomer;
		this->hBarber = hBarber;
		this->nFreeSeat = nFreeSeat;
		this->number = number;
	}
};



void cuttingHair() {
	std::cout << "Барбер стрижет волосы посетителю\n";
};


void haveCutingHair(int i) {
	std::cout << "Посетитель номер " << i << " стрижется\n";
}

void exit(int i){
	std::cout << "Посетитель номер " << i << " ушел\n";
}

DWORD WINAPI barber(LPVOID lpParam)
{
	dataForThread* param = (dataForThread*)lpParam;
	
	WaitForSingleObject(param->hCustomer, INFINITE);
	WaitForSingleObject(param->hAccessSeat, INFINITE);
	param->nFreeSeat->add();
	param->nFreeSeat->get();
	ReleaseMutex(param->hAccessSeat);
	ReleaseSemaphore(param->hBarber, 1, NULL);
	Sleep(1);
	return 0;
}

DWORD WINAPI customer(LPVOID lpParam)
{
	dataForThread* param = (dataForThread*)lpParam;

	WaitForSingleObject(param->hAccessSeat, INFINITE);
	if (param->nFreeSeat->get() > 0) {
		param->nFreeSeat->del();
		param->nFreeSeat->get();
		haveCutingHair(param->number);
		exit(param->number);
		ReleaseSemaphore(param->hCustomer, 1, NULL);
		ReleaseMutex(param->hAccessSeat);
		WaitForSingleObject(param->hBarber, INFINITE);
		Sleep(1);
	}
	else {
		exit(param->number);
		ReleaseMutex(param->hAccessSeat);
	}
	return 0;
}

int main()
{
	srand(time(0));
	setlocale(LC_ALL, "Rus");
	int  customerSize, nFreeSeat;

	std::cout << "Введите количество посетителей: ";
	std::cin >> customerSize;

	std::cout << "Введите количество мест: ";
	std::cin >> nFreeSeat;
	std::cout << std::endl;
	

	cnFreeSeat* nf = new cnFreeSeat(nFreeSeat);

	HANDLE hAccessSeat = CreateMutex(NULL, FALSE, L"hAccessSeat");
	HANDLE hCustomer = CreateSemaphore(NULL, 0, 1, L"hCustomer");
	HANDLE hBarber = CreateSemaphore(NULL, 1, 1, L"hBarber");


	dataForThread* param = new dataForThread(hAccessSeat, hCustomer, hBarber, nf,0);


	HANDLE* hCustomers = new HANDLE[customerSize];
	HANDLE* hBarbers = new HANDLE[customerSize];

	DWORD* IDCustomer = new DWORD[customerSize];
	DWORD* IDBarber = new DWORD[customerSize];

	for (int i = 0; i < customerSize; i++) {
		hBarbers[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)barber, (void*&)param, 0, &IDBarber[i]);
		if (NULL == hBarbers[i]) {
			return GetLastError();
		}
	}

	for (int i = 0; i < customerSize; i++) {
		dataForThread* paramc = new dataForThread(hAccessSeat, hCustomer, hBarber, nf, i + 1);
		hCustomers[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)customer, (void*&)paramc, 0, &IDCustomer[i]);
		if (NULL == hCustomers[i]) {
			return GetLastError();
		}
	}
	WaitForMultipleObjects(customerSize, hCustomers, TRUE, INFINITE);
	

	CloseHandle(hAccessSeat);
	CloseHandle(hCustomer);
	CloseHandle(hBarber);

	for (int i = 0; i < customerSize; i++) {
		CloseHandle(hCustomers[i]);
	}
	for (int i = 0; i < customerSize; i++) {
		CloseHandle(hBarbers[i]);
	}
	return 0;

	delete[] IDBarber;
	delete[] IDCustomer;
	delete[] hBarbers;
	delete[] hCustomers;
	delete[] param;
	delete nf;
}