// Dlg_Expansion_View.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Expansion_View.h"
#include "afxdialogex.h"


// CDlg_Expansion_View ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlg_Expansion_View, CDialogEx)

CDlg_Expansion_View::CDlg_Expansion_View(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_EXPANSION_VIEW, pParent)
{

}

CDlg_Expansion_View::~CDlg_Expansion_View()
{
}

void CDlg_Expansion_View::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Expansion_View, CDialogEx)
	ON_MESSAGE(WM_EXPANSION_VIEW, OnReceiveImg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
END_MESSAGE_MAP()

LRESULT CDlg_Expansion_View::OnReceiveImg(WPARAM wParam, LPARAM lParam)
{

	m_ViewData_Src.img = (Mat*)lParam;
	DrawViewData(m_ViewData_Src);

	return 0;
}


BOOL CDlg_Expansion_View::OnInitDialog()
{
	__super::OnInitDialog();

	m_pWnd = GetDlgItem((IDC_STATIC_EXP_VIEW));
	InitViewData(m_pWnd);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDlg_Expansion_View::OnDestroy()
{
	__super::OnDestroy();

	ReleaseViewData();
}

void CDlg_Expansion_View::OnPaint()
{
	CPaintDC dc(this); 
	DrawImage(m_ViewData_Src);
}

void CDlg_Expansion_View::RefreshView(Mat img)
{
	m_ViewData_Src.img = &img;
	DrawViewData(m_ViewData_Src);

	this->ShowWindow(SW_SHOW);
}