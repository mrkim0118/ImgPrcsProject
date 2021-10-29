// Dlg_Serial_Test.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Serial_Test.h"
#include "afxdialogex.h"


// CDlg_Serial_Test 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_Serial_Test, CDialogEx)

CDlg_Serial_Test::CDlg_Serial_Test(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SERIAL_COMM, pParent)
{

}

CDlg_Serial_Test::~CDlg_Serial_Test()
{
}

void CDlg_Serial_Test::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Serial_Test, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SERIAL_SEND_DATA, &CDlg_Serial_Test::OnBnClickedBtnSerialSendData)
	ON_BN_CLICKED(IDC_BTN_SERIAL_OPEN, &CDlg_Serial_Test::OnBnClickedBtnSerialOpen)
	ON_BN_CLICKED(IDC_BTN_SERIAL_CLOSE, &CDlg_Serial_Test::OnBnClickedBtnSerialClose)
END_MESSAGE_MAP()


// CDlg_Serial_Test 메시지 처리기입니다.


void CDlg_Serial_Test::OnBnClickedBtnSerialSendData()
{

}


void CDlg_Serial_Test::OnBnClickedBtnSerialOpen()
{
	m_Serial.OpenConnection()
}


void CDlg_Serial_Test::OnBnClickedBtnSerialClose()
{
	
}
