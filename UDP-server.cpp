#include <winsock2.h>
#include <iostream>

using namespace std;

const int MAX_CLIENT = 1024;

int main() {
	SOCKADDR_IN cAddrs[MAX_CLIENT];
	int clientCount = 0;

	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	SOCKADDR_IN sAddr;
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons(8888);
	sAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(s, (sockaddr*)&sAddr, sizeof(sAddr));

	// for loop to wait package
	while (true) {
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		SOCKADDR_IN cAddr;
		int cLen = sizeof(cAddr);

		// return num of byte received if success and socket_error if fail
		int received = recvfrom(s, buffer, sizeof(buffer), 0, (sockaddr*)&sAddr, &cLen);
		memcpy(&cAddrs[clientCount].sin_addr, &cAddr, sizeof(cAddr));
		clientCount++;
		for (int i = 0; i < clientCount; i++) {
			SOCKET c = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			sendto(c, buffer, strlen(buffer), 0, (sockaddr*)&cAddrs[i], sizeof(SOCKADDR_IN));
			closesocket(c);
		}
		cout << buffer << endl;
		cout << "Received " << received << " Bytes!" << endl;
	}
	
	// int sent = sendto(s, welcome, strlen(welcome), 0, (sockaddr*)&sAddr, sizeof(sAddr));
	// cout << "Sent " << sent << "bytes" << endl;
	getchar();
	return 0;
}