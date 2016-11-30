#pragma once
#include <WinSock2.h>
#include <ctime>
#define DEFAULT_BUFLEN 8192
#define DEFAULT_PORT 7890
#define MAX_SESSION_LIMIT 100
struct SessionInfo {
	SOCKET Socket; /// Socket Handle
	char IPAddress[16]; /// IP Address, xxx.xxx.xxx.xxxÅE
	char MessageBuffer[DEFAULT_BUFLEN]; /// Message buffer
	int MessageSize; /// Message size in the buffer
	HANDLE hThread;
	DWORD dwThreadID;
	CRITICAL_SECTION cs_SessionList;
	time_t currTime = 0;
	// more information, what you need for your server.
};