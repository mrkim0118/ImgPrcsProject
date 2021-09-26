#pragma once
#include "stdafx.h"
#include"Etc.h"

class CFileCtrl
{
public:
	CFileCtrl();
	~CFileCtrl();

	CEtc m_Etc;
	CFileFind m_finder;
private:

	BOOL m_bRet;
	CString m_strLog;


public:
	BOOL CopyFileFunc(CString strSrcPath, CString strDstPath, BOOL bExist = FALSE);
	BOOL DeleteFileFunc(CString strFilePath);

	BOOL CreateFolderFunc(CString strFolderPath);
	BOOL DeleteFilesInFolderFunc(CString strFolderPath);


	// 3번째 파라메터 TRUE 일경우 동일한 파일이 있을때 Fail , FALSE일 경우 덮어쓰기.
};

