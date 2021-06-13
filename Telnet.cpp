#include <winsock2.h>
#include<iostream>

using namespace std;

DWORD WINAPI ClientThread(LPVOID param);
CRITICAL_SECTION cs;

int main(int argc, char** argv) {
	if (argc < 2) {
		return EXIT_FAILURE;
	} else {
		InitializeCriticalSection(&cs);
		short port = atoi(argv[1]);
		cout << argv[0] << " " << argv[1] << endl;
		// system("dir D:\temp > D:\\temp\\out.txt");
		WSADATA Data;
		WSAStartup(MAKEWORD(2, 2), &Data);
		SOCKADDR_IN sAddr;
		SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		sAddr.sin_family = AF_INET;
		sAddr.sin_addr.S_un.S_addr = INADDR_ANY;
		sAddr.sin_port = htons(port);
		bind(s, (sockaddr*)&sAddr, sizeof(sAddr));
		listen(s, 10);

		while (true) {
			SOCKADDR_IN cAddr;
			int clen = sizeof(cAddr);
			SOCKET c = accept(s, (sockaddr*)&cAddr, &clen);
			CreateThread(NULL, 0, ClientThread, (LPVOID)c, 0, NULL);
		}

	}
	return EXIT_SUCCESS;
}

DWORD WINAPI ClientThread(LPVOID param) {
	SOCKET c = (SOCKET)param;
	char* welcome = (char*)"enter user and password [username password]: ";
	send(c, welcome, strlen(welcome), 0);
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	recv(c, buffer, sizeof(buffer), 0);  // can not use sizeof with dynamic locate
	while (buffer[strlen(buffer) - 1] == '\r' || buffer[strlen(buffer) - 1] == '\n') {
		buffer[strlen(buffer) - 1] = 0;
	}
	// split string to username and password
	char user[1024]; memset(user, 0, sizeof(user));
	char password[1024]; memset(password, 0, sizeof(password));
	sscanf(buffer, "%s%s", user, password);

	// matching
	bool matched = false;
	FILE* f = fopen("D:\\temp\\user.txt", "rt");
	while (!feof(f)) {
		char line[1024];
		memset(buffer, 0, sizeof(line));
		fgets(line, sizeof(line), f);
		char _user[1024]; memset(_user, 0, sizeof(_user));
		char _password[1024]; memset(_password, 0, sizeof(_password));
		sscanf(line, "%s%s", _user, _password);
		if (strcmp(user, _user) == 0 && strcmp(password, _password) == 0) {
			matched = true;
			break;
		}
	}

	fclose(f);
	if (!matched) {
		char* failed = (char*)"Fail to login !";
		send(c, failed, strlen(failed), 0);
		closesocket(c);
	} else {
		char* successed = (char*)"Please command to execute: ";
		send(c, successed, strlen(successed), 0);
		memset(buffer, 0, sizeof(buffer));
		recv(c, buffer, sizeof(buffer), 0);
		while (buffer[strlen(buffer) - 1] == '\r' || buffer[strlen(buffer) - 1] == '\n') {
			buffer[strlen(buffer) - 1] = 0;
		}
		sprintf(buffer + strlen(buffer), "%s", "> D:\\temp\\out.txt");
		EnterCriticalSection(&cs);
		system(buffer);
		FILE* f = fopen("D:\\temp\\out.txt", "rt");
		fseek(f, 0, SEEK_END); // move pointer to end of file
		int flen = ftell(f); // get pointer address
		fseek(f, 0, SEEK_SET); // move pointer back to head of file
		char* fdata = (char*)calloc(flen, 1);
		fread(fdata, 1, flen, f);

		fclose(f);
		LeaveCriticalSection(&cs);
		send(c, fdata, flen, 0);
		free(fdata);
		closesocket(c);
	}

	return 0;
}