#pragma once
#pragma comment(lib, "ws2_32")

#include "stdafx.h"

#include <TlHelp32.h>

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <future>

#define PACKET_SIZE 1024
#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;

class CEtc
{
public:
	CEtc();
	virtual ~CEtc();

private :

public:
	BOOL KillProcessFunc(CString strProcessName);
	void WriteLog(CString strLog, CString strSavePath = _T(""));
	void SaveCSV(CString strFilePath, vector<CString> Data , CString strTitle = _T(""));
public:
	class CTimer
	{
	public:
		CTimer();
		virtual ~CTimer();
	private:
		int m_QueryInitialized;
		__int64 m_Frequency;
		double m_100msFrequency;
		double m_msFrequency;
		double m_usFrequency;
		__int64 m_Begin;
	private:
		BOOL InitTimer();
	public:
		void Delay(int time);
		void StartTimer() { QueryPerformanceCounter((LARGE_INTEGER *)&m_Begin); }
		long Get_secTime();
		long Get_usTime();
		long Get_msTime();
		long Get_100msTime();
		BOOL TimeOver(long time);
		BOOL TimeOverus(long time);
	};
	CTimer *m_pTimer;

	class Convert
	{
	public:
		string CString_to_string(CString strSrc);
		string char_to_string(char *chSrc);
		CString string_to_CString(string strSrc);
		CString char_to_CString(char *chSrc);
		const char* string_to_char(string strSrc);
	};
	Convert *m_pConvert;

};

class Thread
{
public:
	Thread();
	virtual ~Thread();
public:
	enum ThreadStatus
	{
		THREAD_STATUS_STOP = 0,
		THREAD_STATUS_RUNNING,
		THREAD_STATUS_PAUSE,
		THREAD_STATUS_COUNT_MAX
	};

public:
	BOOL m_bThreadStart;
	CWinThread *m_pThread;
	ThreadStatus m_eThreadStatus;
public:
	//UINT ThreadFunc(LPVOID lpvoid);
};


class ThreadPool
{
public:
	ThreadPool(size_t num_threads);
	virtual ~ThreadPool();

	template <class F, class... Args>
	future<typename result_of<F(Args...)>::type> EnqueueJob(F&& f, Args&&... args);

private:
	size_t num_threads_;
	vector<thread> m_vThreads;
	queue<function<void()>> m_qJobs;
	condition_variable m_cv_job;
	mutex m_mtJob;

	bool m_bStop_All;
public:
	void WorkerThread();
	//void EnqueueJob(function<void()> job);




};


