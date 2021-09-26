#include "stdafx.h"
#include "FileCtrl.h"


CFileCtrl::CFileCtrl()
{
	m_bRet = FALSE;
	m_strLog = _T("");
}


CFileCtrl::~CFileCtrl()
{
}

BOOL CFileCtrl::DeleteFileFunc(CString strFilePath)
{
	m_bRet = DeleteFile(strFilePath);
	
	if (m_bRet != TRUE)
	{
		m_strLog.Format(_T("[DeleteFile] - FilePath : %s"), strFilePath);
		m_Etc.WriteLog(m_strLog);
	}
	return m_bRet;
}


BOOL CFileCtrl::CopyFileFunc(CString strSrcPath, CString strDstPath, BOOL bExist)
{
	m_bRet = CopyFile(strSrcPath, strDstPath, bExist);

	if (m_bRet != TRUE)
	{
		m_strLog.Format(_T("[CopyFile] - SrcFile : %s , DstFile : %S") , strSrcPath , strDstPath );
		m_Etc.WriteLog(m_strLog);
	}
	return m_bRet;
}

BOOL CFileCtrl::CreateFolderFunc(CString strFolderPath)
{
	m_bRet = CreateDirectory(strFolderPath, NULL);
	if (m_bRet != TRUE)
	{
		m_strLog.Format(_T("[CreateFolder] - FolderPath : %s"), strFolderPath);
		m_Etc.WriteLog(m_strLog);
	}

	return m_bRet;
}

BOOL CFileCtrl::DeleteFilesInFolderFunc(CString strFolderPath)
{
	m_bRet = TRUE;
	BOOL bWorking = m_finder.FindFile(strFolderPath + _T("/*.*"));

	while (bWorking)
	{
		bWorking = m_finder.FindNextFile();
		if (m_finder.IsDots())
		{
			continue;
		}

		CString filePath = m_finder.GetFilePath();
		m_bRet = DeleteFileFunc(filePath);
	}
	m_finder.Close();

	return m_bRet;
}
