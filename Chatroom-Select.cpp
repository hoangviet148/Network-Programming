#include <winsock2.h>
#include<iostream>

using namespace std;
#define MAX 1024

SOCKET clients[MAX];
int clientCount = 0; // number of clients

int main() {
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
	SOCKADDR_IN sAddr;
	sAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	sAddr.sin_port = htons(8888);
	sAddr.sin_family = AF_INET;
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bind(s, (sockaddr*)&sAddr, sizeof(sAddr));
	listen(s, 10);
	fd_set fdread;

	while (true) {
		FD_ZERO(&fdread); // delete socket
		FD_SET(s, &fdread);
		for (int i = 0; i < clientCount; i++) {
			FD_SET(clients[i], &fdread);
		}
		select(0, &fdread, NULL, NULL, NULL);
		if (FD_ISSET(s, &fdread)) {
			SOCKADDR_IN cAddr;
			int clen = sizeof(cAddr);
			SOCKET tmp = accept(s, (sockaddr*)&cAddr, &clen);
			clients[clientCount] = tmp;
			clientCount++;
		}
		
		for (int i = 0; i < clientCount; i++) {
			if (FD_ISSET(clients[i], &fdread)) {
				char buffer[1024];
				memset(buffer, 0, sizeof(buffer));
				recv(clients[i], buffer, sizeof(buffer), 0);
				for (int j = 0; j < clientCount; j++) {
					if (j != i) {
						send(clients[j], buffer, strlen(buffer), 0);
					}
				}
			}
		}
	}

	return 0;
}