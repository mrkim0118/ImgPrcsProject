#pragma once

#include "Etc.h"
#include <WinSock2.h>

#define COM_MAXBLOCK     4095
#define COM_MAXPORTS        4


// Flow control flags

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions

#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13
#define ASCII_STX		0x02
#define ASCII_ETX		0xFE

/////////////////////////////////////////////////////////////////////////////
// CComm window
#define ZERO_MEMORY(s)	::ZeroMemory(&s, sizeof(s))

// flow control
#define FC_DTRDSR	0x01
#define FC_RTSCTS	0x02
#define FC_XONXOFF	0x04
#define	FC_NONE		0x00

#define ASCII_XON	0x11
#define ASCII_XOFF	0x13

// registry stuff
#define CS_REGKEY_SETTINGS     _T("통신환경")
#define CS_REGENTRY_PORT       _T("PORT")
#define CS_REGENTRY_PARITY     _T("PARITY")
#define CS_REGENTRY_BAUD       _T("BAUD")
#define CS_REGENTRY_DATABITS   _T("DATABITS")
#define CS_REGENTRY_STOPBITS   _T("STOPBITS")
#define CS_REGENTRY_FLOW       _T("FLOW")

#pragma  pack(push,1)

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
			BYTE nByteSize = 8;
			BYTE nParity = NOPARITY;
			BYTE nStopBit = ONESTOPBIT;
			BYTE nFlowCtrl;
			BYTE nXonXoff;
			BYTE nCommPort;
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



