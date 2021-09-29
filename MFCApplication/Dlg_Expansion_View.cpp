// Dlg_Expansion_View.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Expansion_View.h"
#include "afxdialogex.h"


// CDlg_Expansion_View 대화 상자입니다.

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
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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