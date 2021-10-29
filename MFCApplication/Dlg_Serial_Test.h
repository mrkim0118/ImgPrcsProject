#pragma once

#include "../MFC/Comm.h"
// CDlg_Serial_Test 대화 상자입니다.

class CDlg_Serial_Test : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Serial_Test)

public:
	CDlg_Serial_Test(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_Serial_Test();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SERIAL_COMM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CComm::Serial m_Serial;
public:
	afx_msg void OnBnClickedBtnSerialSendData();
	afx_msg void OnBnClickedBtnSerialOpen();
	afx_msg void OnBnClickedBtnSerialClose();
};
