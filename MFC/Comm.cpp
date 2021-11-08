#include "stdafx.h"
#include "Comm.h"


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
	//PF_INET : IPV4 , SOCK_STREAM : ����������
	hSocket = socket(stSocketParam.iIPType, stSocketParam.iSocketType, stSocketParam.eProtocol);
	tAddr = {};

	// AF_INET : InternetNetwork : UDP , TCP , ETC..
	tAddr.sin_family = AF_INET;
	//htons �򿣵�� ��� ������ ��ȯ
	tAddr.sin_port = htons(stSocketParam.iPort);
	//INADDR_ANY ���� �����ϴ� ��ǻ���� IP�ּ�
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
	//accept ���� ��û ����
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
		//�������κ��� ������ ������ �޾���.
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
	m_stSocketParam.nByteSize = 8;
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

BOOL CComm::Serial::OpenConnection(DCBParam m_stSocketParam)
{
	//�̹� ����� ����Ǿ� �ִٸ� ��� ���� ���� �� ����
	if (m_hComm != NULL)
		CloseConnection();

	CString strPort = _T("");
	strPort.Format(_T("\\\\.\\COM%d"), m_stSocketParam.nCommPort);
	m_hComm = CreateFile(strPort, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL);
	
	if(m_hComm == INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_osRead.hEvent);
		CloseHandle(m_osWrite.hEvent);
	}

	SetCommMask(m_hComm, EV_RXCHAR);
	SetupComm(m_hComm, 4096, 4096);	// ���� ����

	if (PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR) != TRUE)// ������ ��� ����Ÿ�� �����
	{
		// ��� ���� �ʱ�ȭ ����
		return FALSE;
	}
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
	m_bPortOpen = TRUE;

	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ComReadProcess, this, 0, &dwThreadID);

	//m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	//m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	return TRUE;
}

void CComm::Serial::CloseConnection(void)
{

	m_bPortOpen = FALSE;
	SetCommMask(m_hComm, 0);
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

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
	dcb.ByteSize = m_stSocketParam.nByteSize;
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

	return (SetCommState(m_hComm, &dcb) != 0);	//C4800 �ذ�
}

BOOL CComm::Serial::SetTimeouts(void)
{
	COMMTIMEOUTS	CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 1000;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;
	return (SetCommTimeouts(m_hComm, &CommTimeOuts) != 0);	//C4800 �ذ�
}
DWORD WINAPI CComm::Serial::ComReadProcess(void *arg)
{
	DWORD dwEvent;
	char buff[2048];
	DWORD dwRead;
	DWORD dwReadAll;

	if (!SetCommMask(m_hComm, EV_RXCHAR))
	{
		return FALSE;
	}
	// ��Ʈ�� ����Ǿ� �ִ� ���� ���� ��Ʈ
	while (m_bPortOpen)
	{
		dwEvent = 0;
		WaitCommEvent(m_hComm, &dwEvent, NULL);
		// Serial ��Ʈ�� ���� �����Ͱ� ���� ������ ���
		Sleep(50);
		if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
			dwReadAll = 0;
			do // ���ۿ� �����Ͱ� ���� ������ �б�
			{
				ReadFile(m_hComm, buff, 2048 - dwReadAll, &dwRead, NULL); // �����͸� �о� ��
				dwReadAll += dwRead;
			} while (dwRead);
		}
		// �о� �� �����͸� ReceiveData�� ������ ó��
		ReceiveData(buff, dwReadAll);
	}

	return TRUE;
}

char CComm::Serial::MakeCRC(char * SendBuff, int BuffSize)
{
	char CRC;
	short i = 0;

	CRC = *(SendBuff);

	for (i = 1; i < BuffSize; i++)
	{
		CRC ^= *(SendBuff + i);
	}
	return CRC;
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
		CStringA str; 
		str.Format("%s", message); 
		if (strTemp != str) 
		{ 
			return str.GetLength();
		}
	}
	return dwReadLength; 
}

BOOL CComm::Serial::GetPortOpen()
{
	return m_bPortOpen;
}

int CComm::Serial::WriteCommPort(unsigned char *message, DWORD dwLength) 
{ 
	int iRet; 
	iRet = WriteFile(m_hComm, message, dwLength, &dwLength, &m_osWrite);
	return iRet; 
}

CString CComm::Serial::ReceiveData(char * buff, DWORD dwReadAll)
{
	CString strText;
	char ReceiveData[256];

	memset(ReceiveData, 0, sizeof(ReceiveData));

	if (MakeCRC(buff, dwReadAll - 1) != buff[dwReadAll - 1])
	{
		return FALSE;
	}
	memcpy(ReceiveData, buff + 1, dwReadAll - 3);
	strText.Format(_T("%s"), ReceiveData);
	//m_List_ReceiveData.AddString(strText);

	return strText;
}