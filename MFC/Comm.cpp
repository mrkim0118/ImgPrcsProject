#include "stdafx.h"
#include "Comm.h"

// Flow control flags
#define FC_DTRDSR       0x01   //데이터 단말기(DTR) 대기,데이터 세트(DSR) 대기를 위한 신호
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

CComm::CComm()
{
}
CComm::~CComm()
{
}

void CComm::Socket::InitInstance()
{
	WSAStartup(MAKEWORD(2, 2), &m_wsaData);
}
void CComm::Socket::ExitInstance()
{
	WSACleanup();
}
SOCKET CComm::Socket::CreateSocket(SocketParam stSocketParam, SOCKET &hSocket, SOCKADDR_IN &tAddr)
{
	//PF_INET : IPV4 , SOCK_STREAM : 연결지향방식
	hSocket = socket(stSocketParam.iIPType, stSocketParam.iSocketType, stSocketParam.eProtocol);
	tAddr = {};

	// AF_INET : InternetNetwork : UDP , TCP , ETC..
	tAddr.sin_family = AF_INET;
	//htons 빅엔디안 방식 데이터 변환
	tAddr.sin_port = htons(stSocketParam.iPort);
	//INADDR_ANY 현재 동작하는 컴퓨터의 IP주소
	return hSocket;
}

CComm::Server::Server()
{
	m_stSocketParam.strIPAddress = "";
	m_stSocketParam.iPort = 7000;
	m_stSocketParam.iIPType = PF_INET;
	m_stSocketParam.iSocketType = SOCK_STREAM;
	m_stSocketParam.eProtocol = IPPROTO_TCP;
};
CComm::Server::~Server()
{
}

BOOL CComm::Server::Listen(SOCKET hSocket, SOCKADDR_IN tAddr)
{
	CString strLog = _T("");
	int iRet = 0;

	tAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	iRet = ::bind(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));
	if (iRet != 0)
	{
		strLog.Format(_T("Bind Fail , Error Code - %d"), WSAGetLastError());
		this->WriteLog(strLog);
		closesocket(hSocket);
		return FALSE;
	}

	iRet = listen(hSocket, SOMAXCONN);
	if (iRet != 0)
	{
		strLog.Format(_T("Listen Fail , Error Code - %d"), WSAGetLastError());
		this->WriteLog(strLog);
		closesocket(hSocket);
		return FALSE;
	}
	return TRUE;
}

SOCKET CComm::Server::WaitConnect(SOCKET hListen)
{
	SOCKADDR_IN tCIntAddr = {};
	int iCIntSize = sizeof(tCIntAddr);
	//accept 접속 요청 수락
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tCIntAddr, &iCIntSize);

	return hClient;
}
UINT CComm::Server::ServerThreadFunc(LPVOID lpParam)
{
	CString strLog = _T("");

	Server* pThread = (Server*)lpParam;

	SOCKET hClient = pThread->WaitConnect(pThread->m_hSocket);
	pThread->SocketThread.m_eThreadStatus = Thread::THREAD_STATUS_RUNNING;

	char* cBuffer = new char[PACKET_SIZE];

	while (pThread->SocketThread.m_eThreadStatus == Thread::THREAD_STATUS_RUNNING)
	{
		//소켓으로부터 보내온 정보를 받아줌.
		int iRet = recv(hClient, cBuffer, PACKET_SIZE, 0);
		if (iRet > 0)
		{
			//printf("Recv Msg : %s\n", cBuffer);
			char cMsg[] = "Server Send";
			//send(hClient, cMsg, (int)strlen(cMsg), 0);
			strLog.Format(_T("Receive Data From Client : %s"), cBuffer);
			pThread->WriteLog(strLog);
			send(hClient, cMsg, (int)strlen(cMsg), 0);
		}
		else if (iRet == SOCKET_ERROR)
		{
			strLog.Format(_T("Receive Data Error From Client : %s"), cBuffer);
			pThread->WriteLog(strLog);
		}
		else if (iRet == INVALID_SOCKET)
		{
			strLog.Format(_T("Socket DisConnect : %s"), cBuffer);
			pThread->WriteLog(strLog);
		}

		if (iRet <= 0)
		{
			pThread->SocketThread.m_eThreadStatus = Thread::THREAD_STATUS_STOP;
			closesocket(hClient);
			closesocket(pThread->m_hSocket);
			break;
		}

		pThread->m_pTimer->Delay(1);
	};
	return 0;
}
CComm::Client::Client()
{
	m_stSocketParam.strIPAddress = "";
	m_stSocketParam.iPort = 7000;
	m_stSocketParam.iIPType = PF_INET;
	m_stSocketParam.iSocketType = SOCK_STREAM;
	m_stSocketParam.eProtocol = IPPROTO_TCP;
};
CComm::Client::~Client()
{
}

BOOL CComm::Client::Connect(SOCKET hSocket, SOCKADDR_IN tAddr, string strIPAddress)
{
	CString strLog = _T("");
	int iRet = 0;

	tAddr.sin_addr.s_addr = inet_addr(strIPAddress.c_str());

	iRet = connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));
	if (iRet != 0)
	{
		strLog.Format(_T("Client Connect Fail , Error Code - %d"), WSAGetLastError());
		this->WriteLog(strLog);
		closesocket(hSocket);
		return FALSE;
	}

	return TRUE;
}

//SOCKET Client::CreateSocket(SocketParam stSocketParam)
//{
//	m_hSocket = hSocket;
//
//	if (SocketThread.m_pThread == NULL)
//	{
//		SocketThread.m_pThread = AfxBeginThread(SocketThreadFunc, this);
//
//		if (SocketThread.m_pThread == NULL)
//		{
//			//error
//		}
//		SocketThread.m_pThread->m_bAutoDelete = FALSE;
//
//		SocketThread.m_eThreadStatus = Thread::THREAD_STATUS_RUNNING;
//	}
//	else
//	{
//		if (SocketThread.m_eThreadStatus = Thread::THREAD_STATUS_PAUSE)
//		{
//			SocketThread.m_pThread->ResumeThread();
//			SocketThread.m_eThreadStatus = Thread::THREAD_STATUS_RUNNING;
//		}
//	}
//	return hSocket;
//}

CComm::Serial::Serial(void)
{
	m_bConnect = FALSE;
	m_bFlowCtrl = FC_XONXOFF;
	m_stSocketParam.dwBaudrate = CBR_19200;
	m_stSocketParam.nSize = 8;
	m_stSocketParam.nParity = NOPARITY;
	m_stSocketParam.nStopBit = ONESTOPBIT;
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
};


CComm::Serial::~Serial(void)
{
}

BOOL CComm::Serial::OpenConnection(BYTE nPort, DCBParam m_stSocketParam)
{
	//이미 통신이 연결되어 있다면 통신 연결 해제 후 진행
	if (m_hComm != NULL)
		CloseConnection();

	//드라이버 핸들 생성
	//char			szPort[15];
	//wsprintf(szPort, _T("\\\\.\\COM%d"), nPort);
	//strcat(temp, port_name);
	CString strPort = _T("");
	strPort.Format(_T("\\\\.\\COM%d"), nPort);
	m_hComm = CreateFile(strPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	
	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_osRead.hEvent);
		CloseHandle(m_osWrite.hEvent);
	}

	SetCommMask(m_hComm, EV_RXCHAR);
	SetupComm(m_hComm, 4096, 4096);	// 버퍼 설정
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);	// 버퍼의 모든 데이타를 지운다

	//TIME-OUT
	if (SetTimeouts() == FALSE)
	{
		//Failed to set Comm Timeouts
		CloseHandle(m_hComm);
		return FALSE;
	}

	//DCB Structure
	m_bConnect = SetDCB(m_stSocketParam);
	if (m_bConnect == FALSE)
	{
		//Failed to set DCB Structure
		CloseHandle(m_hComm);
		return FALSE;
	}

	m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	return TRUE;
}

void CComm::Serial::CloseConnection(void)
{
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	if (m_osRead.hEvent != NULL)
	{
		CloseHandle(m_osRead.hEvent);
		m_osRead.hEvent = NULL;
	}
	if (m_osWrite.hEvent != NULL)
	{
		CloseHandle(m_osWrite.hEvent);
		m_osWrite.hEvent = NULL;
	}
}

BOOL CComm::Serial::SetDCB(DCBParam m_stSocketParam)
{
	BYTE bSet;
	DCB dcb;

	dcb.DCBlength = sizeof(DCB);
	if (GetCommState(m_hComm, &dcb) == 0)
	{
		//GetCommState Error
		CloseHandle(m_hComm);
		return false;
	}

	dcb.BaudRate = m_stSocketParam.dwBaudrate;
	dcb.ByteSize = m_stSocketParam.nSize;
	dcb.Parity = m_stSocketParam.nParity;
	dcb.StopBits = m_stSocketParam.nStopBit;

	bSet = (BYTE)((m_bFlowCtrl & FC_DTRDSR) != 0);
	dcb.fOutxDsrFlow = bSet;
	if (bSet)
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE;
	else
		dcb.fDtrControl = DTR_CONTROL_ENABLE;

	bSet = (BYTE)((m_bFlowCtrl & FC_RTSCTS) != 0);
	dcb.fOutxCtsFlow = bSet;
	if (bSet)
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	else
		dcb.fRtsControl = RTS_CONTROL_ENABLE;

	// setup software flow control

	bSet = (BYTE)((m_bFlowCtrl & FC_XONXOFF) != 0);

	dcb.fInX = dcb.fOutX = bSet;
	dcb.XonChar = ASCII_XON;
	dcb.XoffChar = ASCII_XOFF;
	dcb.XonLim = 100;
	dcb.XoffLim = 100;

	// other various settings

	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;

	return (SetCommState(m_hComm, &dcb) != 0);	//C4800 해결
}

BOOL CComm::Serial::SetTimeouts(void)
{
	COMMTIMEOUTS	CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 1000;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;
	return (SetCommTimeouts(m_hComm, &CommTimeOuts) != 0);	//C4800 해결
}
int CComm::Serial::ReadCommPort(unsigned char *message, DWORD length) 
{ 
	COMSTAT ComStat; 
	DWORD dwErrorFlags;
	DWORD dwLength; 
	DWORD dwReadLength = 0; 
	CStringA strTemp; 
	strTemp.Format("%s", message); 
	if (m_bConnect == FALSE)
	{
		return 0;
	}
	else 
	{
		ClearCommError(m_hComm, &dwErrorFlags, &ComStat);
		dwLength = min((DWORD)length, ComStat.cbInQue);
		ReadFile(m_hComm, message, dwLength, &dwReadLength, &m_osRead);
	} 
	if (dwReadLength == 0)
	{
		CStringA str; str.Format("%s", message); 
		if (strTemp != str) 
		{ 
			return str.GetLength();
		}
	}
	return dwReadLength; 
}

int CComm::Serial::WriteCommPort(unsigned char *message, DWORD dwLength) 
{ 
	int iRet; 
	iRet = WriteFile(m_hComm, message, dwLength, &dwLength, &m_osWrite);
	return iRet; 
}