#include <winsock2.h>
#include <iostream>

using namespace std;

int main() {
	WSADATA Data;
	WSAStartup(MAKEWORD(2, 2), &Data);
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	char* welcome = (char*)"hello UDP programming!";
	SOCKADDR_IN toAddr;
	toAddr.sin_family = AF_INET;
	toAddr.sin_port = htons(8888);
	toAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	int sent = sendto(s, welcome, strlen(welcome), 0, (sockaddr*)&toAddr, sizeof(toAddr));
	cout << "Sent " << sent << "bytes" << endl;
	getchar();
	return 0;
}