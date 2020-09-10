#include "pch.h"
#include "my2nddll.h"
#include <string>
#include <vector>
#include <iterator>
#include <sstream>

#include <iostream>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

using namespace std;

#define CURRENT_VERSION "1.0.0.1"

string var;

string write(string msg);
string read();

//--- Extension version information shown in .rpt file
void __stdcall RVExtensionVersion(char* output, int outputSize)
{
	//--- max outputSize is 32 bytes
	strncpy_s(output, outputSize, CURRENT_VERSION, _TRUNCATE);
}
//--- name callExtension function
void __stdcall RVExtension(char* output, int outputSize, const char* function)
{
	//string str = function;
	string odgovor = comm(function);
	//string str = comm(function);
	strncpy_s(output, outputSize,  odgovor.c_str(), _TRUNCATE);
}
//--- name callExtension [function, args]
int __stdcall RVExtensionArgs(char* output, int outputSize, const char* function, const char** args, int argsCnt)
{
	if (strcmp(function, "fnc1") == 0)
	{
		//--- Manually assemble output array
		int i = 0;
		string str = "[";

		//--- Each argument can be accessed via args[n]
		if (argsCnt > 0)
			str += args[i++];

		while (i < argsCnt)
		{
			str += ",";
			str += args[i++];
		}
		str += "]";
		str = comm(str.c_str());
		//--- Extension result
		strncpy_s(output, outputSize, str.c_str(), _TRUNCATE);
		//--- Extension return code
		
		return 100;
	}
	else if (strcmp(function, "fnc2") == 0)
	{
		//--- Parse args into vector
		vector<string> vec(args, next(args, argsCnt));
		ostringstream oss;
		if (!vec.empty())
		{
			//--- Assemble output array
			copy(vec.begin(), vec.end() - 1, ostream_iterator<string>(oss, ","));
			oss << vec.back();
		}
		//--- Extension result
		strncpy_s(output, outputSize, ("[" + oss.str() + "]").c_str(), _TRUNCATE);
		//--- Extension return code
		return 200;
	}
	else
	{
		strncpy_s(output, outputSize, "Avaliable Functions: fnc1, fnc2", outputSize - 1);
		return -1;
	}
}

string __stdcall comm(string message)
{
	string readStr = read();
	string writeStr = write(message);

	return readStr;
	//HANDLE hFileMap;
	//BOOL bResult;
	//PCHAR lpBuffer = NULL;
	//string dataServer = "";

	//hFileMap = OpenFileMapping(
	//	FILE_MAP_ALL_ACCESS,
	//	FALSE,
	//	L"Local\\MyFileMapServer");
	//if (hFileMap == NULL)
	//{
	//	string res = "  OpenFileMap err: " + to_string(GetLastError());
	//	return res;
	//	//cout << "OpenFileMap err: " << GetLastError() << endl;
	//}
	////cout << "OpenFileMap succesful" << endl;

	//lpBuffer = (PCHAR)MapViewOfFile(
	//	hFileMap,
	//	FILE_MAP_ALL_ACCESS,
	//	0,
	//	0,
	//	256);
	//if (lpBuffer == NULL)
	//{
	//	string res = "  MapViewOfFile err: " + to_string(GetLastError());
	//	return res;
	//	//cout << "MapViewOfFile err: " << GetLastError << endl;
	//}
	////cout << "MapViewOfFile succesful" << endl;

	////cout << "DATA FROM FILE SERVER: " << lpBuffer << endl;
	//dataServer = lpBuffer;
	//bResult = UnmapViewOfFile(lpBuffer);
	//if (bResult == FALSE)
	//{
	//	string res = "  UnmapViewOfFile err: " + to_string(GetLastError());
	//	return res;
	//	//cout << "UnmapViewOfFile err: " << GetLastError() << endl;
	//}
	////cout << "UnmapViewOfFile succesful" << endl;

	//CloseHandle(hFileMap);
	//return dataServer;

	//string ipAddress = "127.0.0.1";			// IP Address of the server
	//int port = 54000;						// Listening port # on the server
	//string err = "NULL";
	//string response = "NULL";
	//// Initialize WinSock
	//WSAData data;
	//WORD ver = MAKEWORD(2, 2);
	//int wsResult = WSAStartup(ver, &data);
	//if (wsResult != 0)
	//{
	//	err = "Can't start Winsock, Err #" + wsResult;
	//	//cerr << "Can't start Winsock, Err #" << wsResult << endl;
	//	return err;
	//}
	//// Create socket
	//SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	//if (sock == INVALID_SOCKET)
	//{
	//	err = "Can't create socket, Err #" + WSAGetLastError();
	//	//cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
	//	WSACleanup();
	//	return err;
	//}
	//// Fill in a hint structure
	//sockaddr_in hint;
	//hint.sin_family = AF_INET;
	//hint.sin_port = htons(port);
	//inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	//// Connect to server
	//int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	//if (connResult == SOCKET_ERROR)
	//{
	//	err = "Can't connect to server, Err #" + WSAGetLastError();
	//	//cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
	//	closesocket(sock);
	//	WSACleanup();
	//	return err;
	//}
	//// Do-while loop to send and receive data
	//char buf[4096];
	//// Send the text
	//int sendResult = send(sock, message.c_str(), message.size() + 1, 0);
	//if (sendResult != SOCKET_ERROR)
	//{
	//	// Wait for response
	//	ZeroMemory(buf, 4096);
	//	int bytesReceived = recv(sock, buf, 4096, 0);
	//	if (bytesReceived > 0)
	//	{
	//		// Echo response to console
	//		response = string(buf, 0, bytesReceived);
	//		//cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
	//	}
	//}
	//else
	//{
	//	response = "ERROR";
	//}
	//// Gracefully close down everything
	//closesocket(sock);
	//WSACleanup();
	//return response;
}

string write(string msg)
{
	//cout << "\t\t....FILEMAPPING SERVER or PARENT...." << endl;
	//Local Variable Definitions
	HANDLE hFileMap;
	BOOL bResult;
	PCHAR lpBuffer = NULL;
	char Buffer[1024];
	strcpy_s(Buffer, msg.c_str());
	size_t szBuffer = sizeof(Buffer);
	//cout << "Buffer: " << Buffer << " szBuffer: " << szBuffer << endl;
	hFileMap = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		256,
		L"Local\\MyFileMapClient");
	if (hFileMap == FALSE)
	{
		string res = "  CreateFileMapping err: " + to_string(GetLastError());
		return "---";
		//cout << "CreateFileMapping err: " << GetLastError() << endl;
	}
	//cout << "CreatingFileMapping succesful" << endl;

	lpBuffer = (PCHAR)MapViewOfFile(
		hFileMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		256);
	if (lpBuffer == NULL)
	{
		string res = "  MapViewOfFile err: " + to_string(GetLastError());
		return "---";
		//cout << "MapViewOfFile err: " << GetLastError << endl;
	}
	//cout << "MapViewOfFile succesful" << endl;

	CopyMemory(lpBuffer, Buffer, szBuffer);

	bResult = UnmapViewOfFile(lpBuffer);
	if (bResult == FALSE)
	{
		//cout << "UnmapViewOfFile err: " << GetLastError() << endl;
	}
	//cout << "UnmapViewOfFile succesful" << endl;
	return "response";

}
string read()
{
	HANDLE hFileMap;
	BOOL bResult;
	PCHAR lpBuffer = NULL;
	string dataServer = "";

	hFileMap = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,
		FALSE,
		L"Local\\MyFileMapServer");
	if (hFileMap == NULL)
	{
		string res = "  OpenFileMap err: " + to_string(GetLastError());
		return "+++";
		//cout << "OpenFileMap err: " << GetLastError() << endl;
	}
	//cout << "OpenFileMap succesful" << endl;

	lpBuffer = (PCHAR)MapViewOfFile(
		hFileMap,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		256);
	if (lpBuffer == NULL)
	{
		string res = "  MapViewOfFile err: " + to_string(GetLastError());
		return "+++";
		//cout << "MapViewOfFile err: " << GetLastError << endl;
	}
	//cout << "MapViewOfFile succesful" << endl;

	//cout << "DATA FROM FILE SERVER: " << lpBuffer << endl;
	dataServer = lpBuffer;
	bResult = UnmapViewOfFile(lpBuffer);
	if (bResult == FALSE)
	{
		string res = "  UnmapViewOfFile err: " + to_string(GetLastError());
		return "+++";
		//cout << "UnmapViewOfFile err: " << GetLastError() << endl;
	}
	//cout << "UnmapViewOfFile succesful" << endl;

	CloseHandle(hFileMap);
	return dataServer;
}
