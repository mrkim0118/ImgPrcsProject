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
	DDX_Control(pDX, IDC_CMB_COMPORT, m_Cmb_Comport);
	DDX_Control(pDX, IDC_CMB_BAUDRATE, m_Cmb_Baudrate);
	DDX_Control(pDX, IDC_CMB_PARITY_BIT, m_Cmb_Parity);
	DDX_Control(pDX, IDC_CMB_STOP_BIT, m_Cmb_StopBit);
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
	m_DCBparam.dwBaudrate = GetBaudrate();
	m_DCBparam.nCommPort = GetComport();
	m_DCBparam.nParity = GetParity();
	m_DCBparam.nStopBit = GetStopBit();

	m_Serial.OpenConnection(m_DCBparam);
}


void CDlg_Serial_Test::OnBnClickedBtnSerialClose()
{
	m_Serial.CloseConnection();
}


BOOL CDlg_Serial_Test::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString strTmp = _T("");

	for (int i = 0; i < 20; i++)
	{
		strTmp.Format(_T("COM%d") ,i + 1);
		m_Cmb_Comport.AddString(strTmp);
	}
	m_Cmb_Baudrate.AddString(_T("110"));
	m_Cmb_Baudrate.AddString(_T("300"));
	m_Cmb_Baudrate.AddString(_T("600"));
	m_Cmb_Baudrate.AddString(_T("1200"));
	m_Cmb_Baudrate.AddString(_T("2400"));
	m_Cmb_Baudrate.AddString(_T("4800"));
	m_Cmb_Baudrate.AddString(_T("9600"));
	m_Cmb_Baudrate.AddString(_T("14400"));
	m_Cmb_Baudrate.AddString(_T("19200"));
	m_Cmb_Baudrate.AddString(_T("38400"));
	m_Cmb_Baudrate.AddString(_T("56000"));
	m_Cmb_Baudrate.AddString(_T("57600"));
	m_Cmb_Baudrate.AddString(_T("115200"));
	m_Cmb_Baudrate.AddString(_T("128000"));
	m_Cmb_Baudrate.AddString(_T("256000"));

	m_Cmb_Parity.AddString(_T("0_NO"));
	m_Cmb_Parity.AddString(_T("1_ODD"));
	m_Cmb_Parity.AddString(_T("2_EVEN"));
	m_Cmb_Parity.AddString(_T("3_MARK"));
	m_Cmb_Parity.AddString(_T("4_SPACE"));

	m_Cmb_StopBit.AddString(_T("0"));
	m_Cmb_StopBit.AddString(_T("1"));
	m_Cmb_StopBit.AddString(_T("2"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CDlg_Serial_Test::GetComport()
{
	CString strTmp = _T("");
	CString strComport = _T("");
	
	m_Cmb_Comport.GetLBText(m_Cmb_Comport.GetCurSel(), strTmp);

	AfxExtractSubString(strComport, strTmp,1,'M');

	return _tstoi(strComport);

}

int CDlg_Serial_Test::GetBaudrate()
{
	CString strTmp = _T("");

	m_Cmb_Baudrate.GetLBText(m_Cmb_Baudrate.GetCurSel(), strTmp);

	return _tstoi(strTmp);
}

int CDlg_Serial_Test::GetStopBit()
{
	CString strTmp = _T("");

	m_Cmb_StopBit.GetLBText(m_Cmb_StopBit.GetCurSel(), strTmp);

	return _tstoi(strTmp);
}

int CDlg_Serial_Test::GetParity()
{
	CString strTmp = _T("");
	CString strParity = _T("");

	m_Cmb_Parity.GetLBText(m_Cmb_Parity.GetCurSel(), strTmp);

	AfxExtractSubString(strParity, strTmp, 0, '_');

	return _tstoi(strTmp);
}
