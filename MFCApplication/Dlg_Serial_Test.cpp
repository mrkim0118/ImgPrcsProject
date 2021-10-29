// Dlg_Serial_Test.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Serial_Test.h"
#include "afxdialogex.h"


// CDlg_Serial_Test ��ȭ �����Դϴ�.

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


// CDlg_Serial_Test �޽��� ó�����Դϴ�.


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
