#include <winsock2.h>
#include<iostream>

using namespace std;

const int MAX_Thread = 1000;
const int MAX_Client = 1024;
int clientCount = 0;

SOCKET c[MAX_Client];

DWORD WINAPI MyThread(LPVOID param) {
	cout << "MyThread ID: " << GetCurrentThreadId() << endl;
	SOCKET tmp = (SOCKET)param;
	char buffer[1024];
	while (true) {
		memset(buffer, 0, sizeof(buffer));
		recv(tmp, buffer, sizeof(buffer), 0);
		cout << buffer;
		for (int i = 0; i < clientCount; i++) {
			if (c[i] != tmp) {
				send(c[i], buffer, strlen(buffer), 0);
			}
		}
	}
	return 0;
}

int main() {
	cout << "Main Thread ID: " << GetCurrentThreadId() << endl;
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
	SOCKADDR_IN sAddr;
	sAddr.sin_addr.S_un.S_addr = 0;
	sAddr.sin_port = htons(8888);
	sAddr.sin_family = AF_INET;
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(s, (sockaddr*)&sAddr, sizeof(sAddr));
	listen(s, 10);

	while (TRUE) {
		SOCKADDR_IN cAddr;
		int clen = sizeof(cAddr);
		c[clientCount] = accept(s, (sockaddr*)&cAddr, &clen);
		CreateThread(NULL, 0, MyThread, (LPVOID)c[clientCount], 0, NULL);
		clientCount++;
	}
	HANDLE hThread[MAX_Thread];
	for (int i = 0; i < MAX_Thread; i++) {
		hThread[i] = CreateThread(NULL, 0, MyThread, NULL, 0, NULL);
	}

	cout << "Press any key to finish!" << endl;
	//WaitForSingleObject(hThread, INFINITE);
	WaitForMultipleObjects(MAX_Thread, hThread, TRUE, INFINITE);
	for (int i = 0; i < MAX_Thread; i++) {
		CloseHandle(hThread[i]);
	}
	//cout << "Sum = " << sum << endl;
	getchar();
	return 0;
}