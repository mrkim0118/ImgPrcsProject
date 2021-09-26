#pragma once

#include "Etc.h"
#include <WinSock2.h>
class CComm
{
public:
	CComm();
	virtual ~CComm();

	class Serial
	{
	public:
		Serial(void);
		~Serial(void);
	private:
		HANDLE m_hComm;			//Handle of COM Port
		OVERLAPPED  m_osWrite, m_osRead;	// Overlapped I/O를 위한 구조체
		BOOL m_bFlowCtrl;
		BOOL m_bConnect;
		struct DCBParam
		{
			DWORD dwBaudrate = CBR_19200;
			BYTE nSize = 8;
			BYTE nParity = NOPARITY;
			BYTE nStopBit = ONESTOPBIT;
		};
		DCBParam m_stSocketParam;
	public:
		BOOL OpenConnection(BYTE nPort, DCBParam m_stSocketParam);
		void CloseConnection();
		BOOL SetDCB(DCBParam m_stSocketParam);
		BOOL SetTimeouts();

		int WriteCommPort(unsigned char *Message, DWORD dwLength);
		int ReadCommPort(unsigned char *Message, DWORD dwLength);

	};



	class Socket : public CEtc
	{
	public:
		WSADATA m_wsaData;
		SOCKET m_hSocket;

		struct SocketParam
		{
			string strIPAddress;
			int iPort;
			int iIPType;
			int iSocketType;
			IPPROTO eProtocol;
		};
		SocketParam m_stSocketParam;
	public:
		void InitInstance();
		void ExitInstance();
		SOCKET CreateSocket(SocketParam stSocketParam, SOCKET &hSocket, SOCKADDR_IN &tAddr);

	};

	class Server : Socket
	{
	public:
		Server();
		virtual ~Server();
	private:
		Thread SocketThread;
	public:
		BOOL Listen(SOCKET hSocket, SOCKADDR_IN tAddr);
		SOCKET WaitConnect(SOCKET hListen);
		static UINT ServerThreadFunc(LPVOID lpvoid);
	};

	class Client : Socket
	{
	public:
		Client();
		virtual ~Client();
	public:
		BOOL Connect(SOCKET hSocket, SOCKADDR_IN tAddr, string strIPAddress);

	};


};



