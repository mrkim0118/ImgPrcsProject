// Dlg_Teaching_Template_Match.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Teaching_Template_Match.h"
#include "afxdialogex.h"


// CDlg_Teaching_Template_Match 대화 상자입니다.

CDlg_Teaching_Template_Match* CDlg_Teaching_Template_Match::m_pDlgTemplateMatch = NULL;


IMPLEMENT_DYNAMIC(CDlg_Teaching_Template_Match, CDialogEx)

CDlg_Teaching_Template_Match::CDlg_Teaching_Template_Match(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_TEMPLATE_MATCH, pParent)
{
	m_pModelImg = new Mat();
}

CDlg_Teaching_Template_Match::~CDlg_Teaching_Template_Match()
{
	if (m_pModelImg != NULL)
	{
		delete m_pModelImg;
		m_pModelImg = NULL;
	}
}

void CDlg_Teaching_Template_Match::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_METHOD, m_Cmb_Method);
}


BEGIN_MESSAGE_MAP(CDlg_Teaching_Template_Match, CDialogEx)
	ON_MESSAGE(WM_TEMPLATE_MATCH_MODEL, OnReceiveImg)
	ON_MESSAGE(WM_TEMPLATE_MATCH_NORM, OnReceiveNorm)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CDlg_Teaching_Template_Match 메시지 처리기입니다.

CDlg_Teaching_Template_Match * CDlg_Teaching_Template_Match::GetInstance()
{
	if (m_pDlgTemplateMatch == NULL)
	{
		m_pDlgTemplateMatch = new CDlg_Teaching_Template_Match;
	}
	return m_pDlgTemplateMatch;
}

void CDlg_Teaching_Template_Match::DestroyInstance()
{
	if (m_pDlgTemplateMatch != NULL)
	{
		delete m_pDlgTemplateMatch;
		m_pDlgTemplateMatch = NULL;
	}
}

BOOL CDlg_Teaching_Template_Match::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pWnd = GetDlgItem(IDC_STATIC_TEMPLATE_MODEL);
	m_pWnd_Ext = GetDlgItem(IDC_STATIC_TEMPLATE_NORMALIZE);
	InitViewData(m_pWnd , m_pWnd_Ext);

	m_pDlgExpansionView = make_unique<CDlg_Expansion_View>();
	m_pDlgExpansionView->Create(IDD_DLG_EXPANSION_VIEW, this);
	m_pDlgExpansionView->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_TEMPLATE_MODEL)->GetWindowRect(&m_DlgRect_Dst);

	m_Cmb_Method.AddString(_T("TM_SQDIFF"));
	m_Cmb_Method.AddString(_T("TM_SQDIFF_NORMED"));
	m_Cmb_Method.AddString(_T("TM_CCORR"));
	m_Cmb_Method.AddString(_T("TM_CCORR_NORMED"));
	m_Cmb_Method.AddString(_T("TM_CCOEFF"));
	m_Cmb_Method.AddString(_T("TM_CCOEFF_NORMED"));
	m_Cmb_Method.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CDlg_Teaching_Template_Match::GetTemplateMatchMethod()
{
	CString strMode = _T("");
	int iMethod = 0;
	m_Cmb_Method.GetLBText(m_Cmb_Method.GetCurSel(), strMode);

	if (strMode == _T("TM_SQDIFF"))
		iMethod = TemplateMatchModes::TM_SQDIFF;
	else if (strMode == _T("TM_SQDIFF_NORMED"))
		iMethod = TemplateMatchModes::TM_SQDIFF_NORMED;
	else if (strMode == _T("TM_CCORR"))
		iMethod = TemplateMatchModes::TM_CCORR;
	else if (strMode == _T("TM_CCORR_NORMED"))
		iMethod = TemplateMatchModes::TM_CCORR_NORMED;
	else if (strMode == _T("TM_CCOEFF"))
		iMethod = TemplateMatchModes::TM_CCOEFF;
	else if (strMode == _T("TM_CCOEFF_NORMED"))
		iMethod = TemplateMatchModes::TM_CCOEFF_NORMED;

	return iMethod;
}
Mat CDlg_Teaching_Template_Match::GetModelImg()
{
	return *m_pModelImg;
}

LRESULT CDlg_Teaching_Template_Match::OnReceiveImg(WPARAM wParam, LPARAM lParam)
{
	Mat* Tmp = (Mat*)lParam;
	*m_ViewData_Src.img = Tmp->clone();
	*m_ViewData_Dst.img = Tmp->clone();
	DrawViewData(m_ViewData_Src);
	*m_pModelImg = m_ViewData_Src.img->clone();

	return 0;
}

LRESULT CDlg_Teaching_Template_Match::OnReceiveNorm(WPARAM wParam, LPARAM lParam)
{
	Mat* Tmp = (Mat*)lParam;
	*m_ViewData_Dst.img = Tmp->clone();
	DrawViewData(m_ViewData_Dst);
	return 0;
}

void CDlg_Teaching_Template_Match::OnPaint()
{
	CPaintDC dc(this);
	DrawImage(m_ViewData_Src);
	DrawImage(m_ViewData_Dst);
}


void CDlg_Teaching_Template_Match::OnDestroy()
{
	CDialogEx::OnDestroy();

	ReleaseViewData();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}




void CDlg_Teaching_Template_Match::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_DlgRect_Dst.PtInRect(point))
	{
		*m_pMessageImg = m_ViewData_Dst.img->clone();
		m_pDlgExpansionView->RefreshView(*m_pMessageImg);
		m_pDlgExpansionView->ShowWindow(SW_SHOW);
	}


	__super::OnRButtonDown(nFlags, point);
}
