#include<Windows.h> 
#include<iostream> 
#include<ctime> 
#include<locale>
using namespace std;

// ���� ���������� ����������
#define Producers_data 5
#define Consumers_data 20
#define BufferSize 50
HANDLE hFull, hEmpty, hAccess;

struct CircularBuffer {
	int* Buffer;				// �������� ������.
	int freeSpace;     // ��������� ����� � �������.
	int occupiedSpace; // ������� �����.
	int rands;			// ������
};

DWORD WINAPI Producer(LPVOID circlBuf) {
	CircularBuffer* buf = (CircularBuffer*)circlBuf;
	int Count = 0;
	while (true) {
		Count++;
		cout << "����� �������������: " << endl << GetCurrentThreadId();
		srand(rand() % 45 + buf->rands);
		for (int i = 0; i != Producers_data; i++) {
			WaitForSingleObject(hEmpty, INFINITE); // ��� ��������� ��������� ������
			WaitForSingleObject(hAccess, INFINITE); // �������� ������ � ����������
			buf->Buffer[buf->freeSpace] = (int)(rand() % 45 + 1);
			cout << "i=" << i + 1 << " [" << buf->freeSpace << "]=" << buf->Buffer[buf->freeSpace] << endl;
			buf->freeSpace = (buf->freeSpace + 1) % BufferSize;
			ReleaseSemaphore(hAccess, 1, NULL); // ��������� ������ � �����������
			ReleaseSemaphore(hFull, 1, NULL); // ������������� � ��������� ����������� ������
		}
	}
	return 0;
}
DWORD WINAPI Consumer(LPVOID circlBuf) {
	CircularBuffer* buf = (CircularBuffer*)circlBuf;
	int Count = 0;
	while (true) {
		Count++;
		cout << "\t\t\t����� �����������: " << GetCurrentThreadId() << endl;
		for (int i = 0; i != Consumers_data; i++) {
			WaitForSingleObject(hFull, INFINITE); // ��� ��������� ����������� ������
			WaitForSingleObject(hAccess, INFINITE); // �������� ������ � ����������
			cout << "\t\t\ti=" << i + 1 << " [" << buf->occupiedSpace << "]=" << buf->Buffer[buf->occupiedSpace] << endl;
			buf->occupiedSpace = (buf->occupiedSpace + 1) % BufferSize;
			ReleaseSemaphore(hAccess, 1, NULL); // ��������� ������ � �����������
			ReleaseSemaphore(hEmpty, 1, NULL); // ������������� � ��������� ��������� ������
		}
	}
	return 0;
}

int main() {
	setlocale(LC_ALL, "");
	HANDLE hThreads[2]; // ������������� � �����������
	DWORD ThreadId;
	hAccess = CreateSemaphoreA(NULL, 1, 1, "Access"); // ���������� �������� � ����������� ������
	hFull = CreateSemaphoreA(NULL, 0, BufferSize, "Full"); // ���������� ����������� �������
	hEmpty = CreateSemaphoreA(NULL, BufferSize, BufferSize, "Empty"); // ���������� ������ �������
	srand(time(NULL));
	CircularBuffer all;
	all.Buffer = new int[BufferSize];
	for (int i = 0; i < BufferSize; i++)
	{
		all.Buffer[i] = 0;
	}
	all.rands = rand() % 100;
	all.freeSpace = 0;
	all.occupiedSpace = 0;
	hThreads[0] = CreateThread(0, 0, Producer, &all, 0, &ThreadId);
	hThreads[1] = CreateThread(0, 0, Consumer, &all, 0, &ThreadId);

	WaitForMultipleObjects(2, hThreads, true, 1000);

	for (int i = 0; i < 2; i++)
	{
		CloseHandle(hThreads[i]);
	}
	CloseHandle(hAccess);
	CloseHandle(hFull);
	CloseHandle(hEmpty);

	system("pause");
	return 0;
}
