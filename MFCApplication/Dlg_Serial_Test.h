#pragma once

#include "../MFC/Comm.h"
#include "afxwin.h"
// CDlg_Serial_Test ��ȭ �����Դϴ�.

#define COMORT_MAX = 20;


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
public:
	CComm::Serial::DCBParam m_DCBparam;
private:
	CComm::Serial m_Serial;
	int m_iComport;
	int m_iBaudrate;
	int m_iParity;
	int m_iStopbit;
public:

	afx_msg void OnBnClickedBtnSerialSendData();
	afx_msg void OnBnClickedBtnSerialOpen();
	afx_msg void OnBnClickedBtnSerialClose();
	CComboBox m_Cmb_Comport;
	CComboBox m_Cmb_Baudrate;
	CComboBox m_Cmb_Parity;
	CComboBox m_Cmb_StopBit;
	virtual BOOL OnInitDialog();

	int GetComport();
	int GetBaudrate();
	int GetStopBit();
	int GetParity();
};
