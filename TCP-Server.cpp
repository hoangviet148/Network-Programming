// deliver data using tcp
#include <winsock2.h> 
#include <stdio.h>

int SendData(SOCKET c, char* data, int len);
int RecvData(SOCKET c, char** buffer);

int main() {
	WSADATA wsaData;
	SOCKET listeningSocket;
	SOCKET newConnection;
	SOCKADDR_IN serverAddr; // save address of app need to connect
	SOCKADDR_IN clientAddr;
	int clientAddrLen;
	int port = 8888;

	// init winsock 2.2
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// init socket listen connect from client
	// af (address family)
	// sock_stream for tcp/ip and sock_dgram for udp/ip
	listeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// init structure SOCKADDR_IN of server
	// connect to port 8888
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port); // convert 2 byte from little-endian=>big-endian
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY); // convert 4 byte from little-endian=>big-endian

	// bind socket of server
	bind(listeningSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	// switch to 'wait to connect' state
	listen(listeningSocket, 5);

	// approve new connection
	clientAddrLen = sizeof(clientAddr);
	newConnection = accept(listeningSocket, (SOCKADDR*)&clientAddr, &clientAddrLen);
	
	// after accept connection, server can continue accept new con
	// or send, receive data with client via acceptance con
	if (newConnection != INVALID_SOCKET) {
		char* hello = (char*)"Hello Network Programming\n";
		int byteSent = SendData(newConnection, hello, strlen(hello));
		char* buffer = NULL;
		int byteReceived = RecvData(newConnection, &buffer);

		printf("Sent: %d\n", byteSent);
		printf("Received: %d\n", byteReceived);
		printf("%s", buffer);
		closesocket(newConnection);

		free(buffer);
		buffer = NULL;
	}
	 
	// close socket
	closesocket(listeningSocket);

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