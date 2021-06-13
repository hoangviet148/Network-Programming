#include <winsock2.h> 
#include <stdio.h>

int SendData(SOCKET c, char* data, int len);
int RecvData(SOCKET c, char** buffer);

int main() {
	WSADATA wsaData;
	SOCKET s;
	SOCKADDR_IN serverAddr;
	int port = 8888;

	// init winsock 2.2
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// init socket client
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// init SOCKADDR_IN structure that have address is 202.191.56.69 and port = 8888
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// connect to server via socket
	connect(s, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	// start send and receive data
	char* data = (char*)"Hello Network Programming\n";
	SendData(s, data, strlen(data));
	char* buffer = NULL;
	RecvData(s, &buffer);
	printf("%s\n", buffer);
	free(buffer);
	buffer = NULL;
	 
	// close socket
	closesocket(s);

	// free winsock
	WSACleanup();

	return 0;
}


int SendData(SOCKET c, char* data, int len) {
	int byteSent = 0;
	while (byteSent < len) {
		byteSent += send(c, data + byteSent, len - byteSent, 0);
	}
	return byteSent;
}


int RecvData(SOCKET c, char** buffer) {
	const int MAX_SIZE = 1024; // < MTU
	char tmp[MAX_SIZE];
	int totalReceived = 0;
	int tmpReceived = 0;
	do {
		tmpReceived = recv(c, tmp, MAX_SIZE, 0);
		if (tmpReceived > 0) {
			*buffer = (char*)realloc(*buffer, totalReceived + tmpReceived + 1);
			memset(*buffer + totalReceived, 0, tmpReceived + 1);
			memcpy(*buffer + totalReceived, tmp, tmpReceived);
			totalReceived += tmpReceived;
		}
	} while (tmpReceived == MAX_SIZE);

	return totalReceived;
}