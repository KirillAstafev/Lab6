#include <Windows.h>
#include <stdio.h>

#define PRODUCER_DATA_SIZE 5
#define CONSUMER_DATA_SIZE 20
#define BUFFER_SIZE 50

struct Buffer {
	char* buffer;
	int freePosition;
	int busyPosition;
};

VOID ProducerThread(LPVOID param);
VOID ConsumerThread(LPVOID param);

int main(int argc, char** argv) {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HANDLE hFree, hBusy;
	hFree = CreateSemaphoreA(NULL, BUFFER_SIZE, BUFFER_SIZE, "Free");
	hBusy = CreateSemaphoreA(NULL, 0, BUFFER_SIZE, "Busy");


	
	system("pause");
	return 0;
}

VOID ProducerThread(LPVOID param)
{
	HANDLE hFree = OpenSemaphoreA(0, FALSE, "Free");
	if (hFree == INVALID_HANDLE_VALUE) {
		printf_s("ньхайю! мер яелютнпю дкъ ябнандмшу онгхжхи!");
		return;
	}

	Buffer* buffer = (Buffer*)param;

	while (true) {

	}

	return;
}

VOID ConsumerThread(LPVOID param)
{
	HANDLE hBusy = OpenSemaphoreA(0, FALSE, "Busy");
	if (hBusy == INVALID_HANDLE_VALUE) {
		printf_s("ньхайю! мер яелютнпю дкъ гюмършу онгхжхи!");
		return;
	}

	Buffer* buffer = (Buffer*)param;
	return;
}
