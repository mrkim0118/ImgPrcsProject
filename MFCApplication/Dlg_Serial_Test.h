#pragma once

#include "../MFC/Comm.h"
// CDlg_Serial_Test ��ȭ �����Դϴ�.

class CDlg_Serial_Test : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Serial_Test)

public:
	CDlg_Serial_Test(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlg_Serial_Test();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SERIAL_COMM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	CComm::Serial m_Serial;
public:
	afx_msg void OnBnClickedBtnSerialSendData();
	afx_msg void OnBnClickedBtnSerialOpen();
	afx_msg void OnBnClickedBtnSerialClose();
};
