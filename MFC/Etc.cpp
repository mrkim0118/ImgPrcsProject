#include "stdafx.h"
#include "Etc.h"


#define _WINSOCK_DEPRECATED_NO_WARNINGS
CEtc::CEtc()
{
	m_pTimer = new CTimer;
	m_pConvert = new Convert;
}


CEtc::~CEtc()
{

	if (m_pConvert != NULL)
	{
		delete m_pConvert;
		m_pConvert = NULL;
	}
	if (m_pTimer != NULL)
	{
		delete m_pTimer;
		m_pTimer = NULL;
	}
}

BOOL CEtc::KillProcessFunc(CString strProcessName)
{
	CString strLog = _T("");

	HANDLE hProcessSnap = NULL;
	DWORD dwReturn = -1;
	PROCESSENTRY32 pe32 = { 0 };

	CString ProcessName = strProcessName;
	ProcessName.MakeLower();

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return FALSE;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcessSnap, &pe32))
	{
		DWORD Code = 0;
		DWORD dwPriorityClass;

		do {
			HANDLE hProcess;
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
			dwPriorityClass = GetPriorityClass(hProcess);

			CString Temp = pe32.szExeFile;
			Temp.MakeLower();

			if (Temp == ProcessName)
			{
				strLog.Format(_T("[Kill Process] - ProcessName : %s"), strProcessName);
				WriteLog(strLog);

				if (TerminateProcess(hProcess, 0))
				{
					GetExitCodeProcess(hProcess, &Code);
				}
				else
				{
					CloseHandle(hProcess);
					CloseHandle(hProcessSnap);
					return GetLastError();
				}
			}
			CloseHandle(hProcess);
		} while (Process32Next(hProcessSnap, &pe32));
		dwReturn = TRUE;
	}
	else
	{
		dwReturn = FALSE;
	}

	CloseHandle(hProcessSnap);

	return dwReturn;
}

void CEtc::WriteLog(CString strSavePath ,CString strLog)
{
	if (&strLog == NULL)
		return;

	if (strSavePath == "")
		strSavePath = "D:\\log.txt";

	time_t Time;
	Time = time(NULL);

	struct tm* pTimeInfo = NULL;
	localtime_s(pTimeInfo , &Time);

	int iYear = pTimeInfo->tm_year + 1900;
	int iMonth = pTimeInfo->tm_mon + 1;
	int iDay = pTimeInfo->tm_mday;
	int iHour = pTimeInfo->tm_hour; 
	int iMin = pTimeInfo->tm_min;
	int iSec = pTimeInfo->tm_sec;

	
	CString strTemp = _T("");

	strTemp.Format(_T("[%d-%d-%d %d:%d:%d.] %s" ), iYear , iMonth , iDay , iHour , iMin , iSec , strLog);

	FILE *fp = NULL;

	fopen_s(&fp , CStringA(strSavePath), "a+");
	
	fprintf_s(fp, CStringA("%s\n"), CStringA(strTemp));

	fclose(fp);
}

void CEtc::SaveCSV(CString strFilePath, vector<CString> Data , CString strTitle /*= _T(""))*/)
{
	FILE *hFP = NULL;

	const char *chTmp;
	chTmp = (CStringA)strFilePath;

	if (fopen_s(&hFP, chTmp, "a+") != 0)
	{
		return;
	}

	fseek(hFP, 0, SEEK_END);
	int lSize = ftell(hFP);

	if (lSize < 1)
	{
		chTmp = (CStringA)strTitle;
		fprintf(hFP, "%s\n", chTmp);
	}
	for (vector<CString>::iterator itr = Data.begin(); itr != Data.end(); itr++)
	{
		chTmp = (CStringA)*itr;
		fprintf(hFP, "%s\n", chTmp);
	}
	fclose(hFP);

	if (chTmp != NULL)
	{
		delete chTmp;
		chTmp = NULL;
	}
}

CEtc::CTimer::CTimer()
{
	InitTimer();
}
CEtc::CTimer::~CTimer()
{
}
BOOL CEtc::CTimer::InitTimer()
{
	m_QueryInitialized = QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
	if (!m_QueryInitialized)
	{
		m_Frequency = 1;
	}
	m_100msFrequency = m_Frequency / 10.;
	m_msFrequency = m_Frequency / 1000.;
	m_usFrequency = m_Frequency / 1000000.;
	if (!m_QueryInitialized)
		return FALSE;
	else
		return TRUE;
}

void CEtc::CTimer::Delay(int d)
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_Begin);
	while (Get_msTime() < d);
}

long CEtc::CTimer::Get_secTime()
{
	__int64 End; QueryPerformanceCounter((LARGE_INTEGER*)&End);
	return (long)((End - m_Begin) / m_Frequency);
}

long CEtc::CTimer::Get_usTime()
{
	__int64 End; QueryPerformanceCounter((LARGE_INTEGER*)&End);
	return (long)((End - m_Begin) / m_usFrequency);
}

long CEtc::CTimer::Get_msTime()
{
	__int64 End; QueryPerformanceCounter((LARGE_INTEGER*)&End);
	return (long)((End - m_Begin) / m_msFrequency);
}

long CEtc::CTimer::Get_100msTime()
{
	__int64 End;
	QueryPerformanceCounter((LARGE_INTEGER*)&End);
	return (long)((End - m_Begin) / m_100msFrequency);
}

BOOL CEtc::CTimer::TimeOver(long time)
{
	if (Get_msTime() > time) return TRUE;
	else return FALSE;
}

BOOL CEtc::CTimer::TimeOverus(long time)
{
	if (Get_usTime() > time)
		return TRUE;
	else return FALSE;
}




Thread::Thread()
{
	m_bThreadStart = FALSE;
	m_pThread = NULL;
	m_eThreadStatus = THREAD_STATUS_STOP;
}


Thread::~Thread()
{
	if (m_pThread != NULL)
	{
		delete m_pThread;
		m_pThread = NULL;
	}
}

string CEtc::Convert::CString_to_string(CString strSrc)
{

	string str = string(CT2CA(strSrc));
	return str;
}

string CEtc::Convert::char_to_string(char * chSrc)
{
	string str(chSrc);
	return str;
}

CString CEtc::Convert::string_to_CString(string strSrc)
{
	CString str(strSrc.c_str());
	return str;
}

CString CEtc::Convert::char_to_CString(char * chSrc)
{
	CString str;
	str.Format(_T("%s") , chSrc);
	return str;
}

const char* CEtc::Convert::string_to_char(string strSrc)
{
	const char *ch = strSrc.c_str();
	return ch;
}
