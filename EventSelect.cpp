#include <winsock2.h>
#include<iostream>

using namespace std;

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
		FD_ZERO(&fdread);
		FD_SET(s, &fdread);
		select(0, &fdread, NULL, NULL, NULL);
		if (FD_ISSET(s, &fdread)) {
			SOCKADDR_IN cAddr;
			int clen = sizeof(cAddr);
			SOCKET c = accept(s, (sockaddr*)&cAddr, &clen);
			cout << "Client " << c << endl;
		}
	}

	return 0;
}