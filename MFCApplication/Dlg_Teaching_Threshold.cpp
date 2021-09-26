// Dlg_Teaching_Threshold.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Teaching_Threshold.h"
#include "afxdialogex.h"


// CDlg_Teaching_Threshold 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_Teaching_Threshold, CDialogEx)

CDlg_Teaching_Threshold::CDlg_Teaching_Threshold(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_THRESHOLD, pParent)
	, m_iEdit_Adp_BlockSize(0)
	, m_iEdit_Adp_C(0)
	, m_iEdit_Threshold(0)
	, m_iEdit_Threshold_Value(0)
{
	m_pOpenCV = make_unique<COpenCV>();
	m_pDlgItem = make_unique<CDlgItem>();
}

CDlg_Teaching_Threshold::~CDlg_Teaching_Threshold()
{
}

void CDlg_Teaching_Threshold::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_ADAPTIVE_METHOD, m_Cmb_Adp_Method);
	DDX_Control(pDX, IDC_CMB_ADAPTIVE_TYPE, m_Cmb_Adp_Type);
	DDX_Text(pDX, IDC_EDIT_ADAPTIVE_BLOCK_SIZE, m_iEdit_Adp_BlockSize);
	DDX_Text(pDX, IDC_EDIT_ADAPTIVE_C, m_iEdit_Adp_C);
	DDX_Control(pDX, IDC_CMB_METHOD, m_Cmb_Method);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_iEdit_Threshold);
	DDX_Control(pDX, IDC_CHECK1, m_Chk_Adaptive_Use);
	DDX_Control(pDX, IDC_SLIDER_THRESHOLD, m_Slider_Threshold);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD_VAL, m_iEdit_Threshold_Value);
}


BEGIN_MESSAGE_MAP(CDlg_Teaching_Threshold, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_ADAPTIVE_BLOCK_SIZE, &CDlg_Teaching_Threshold::OnEnChangeEditAdaptiveBlockSize)
	ON_EN_CHANGE(IDC_EDIT_ADAPTIVE_C, &CDlg_Teaching_Threshold::OnEnChangeEditAdaptiveC)
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD, &CDlg_Teaching_Threshold::OnEnChangeEditThreshold)

	ON_MESSAGE(WM_THRESHOLD_TEST, OnReceiveImg)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_THRESHOLD_VAL, &CDlg_Teaching_Threshold::OnEnChangeEditThresholdVal)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlg_Teaching_Threshold 메시지 처리기입니다.


BOOL CDlg_Teaching_Threshold::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pDlgItem->m_pWnd = GetDlgItem((IDC_STATIC_THRESHOLD_TEST));
	m_pDlgItem->InitViewData(m_pDlgItem->m_pWnd);

	m_Cmb_Adp_Type.AddString(_T("ADAPTIVE_THRESH_MEAN_C"));
	m_Cmb_Adp_Type.AddString(_T("ADAPTIVE_THRESH_GAUSSIAN_C"));
	m_Cmb_Adp_Type.SetCurSel(0);

	m_Cmb_Adp_Method.AddString(_T("THRESH_BINARY"));
	m_Cmb_Adp_Method.AddString(_T("THRESH_BINARY_INV"));
	m_Cmb_Adp_Method.SetCurSel(0);

	m_Cmb_Method.AddString(_T("THRESH_BINARY"));
	m_Cmb_Method.AddString(_T("THRESH_BINARY_INV"));
	m_Cmb_Method.AddString(_T("THRESH_TRUNC"));
	m_Cmb_Method.AddString(_T("THRESH_TOZERO"));
	m_Cmb_Method.AddString(_T("THRESH_TOZERO_INV"));
	m_Cmb_Method.AddString(_T("THRESH_MASK"));
	m_Cmb_Method.AddString(_T("THRESH_OTSU"));
	m_Cmb_Method.AddString(_T("THRESH_TRIANGLE"));
	m_Cmb_Method.SetCurSel(0);

	m_iEdit_Adp_BlockSize = 3;
	m_iEdit_Adp_C = 5;

	m_Chk_Adaptive_Use.SetCheck(FALSE);

	m_Slider_Threshold.SetRange(0, 255);
	m_Slider_Threshold.SetPos(m_iEdit_Threshold);
	m_Slider_Threshold.SetLineSize(10);
	m_Slider_Threshold.SetPageSize(15);

	m_iEdit_Threshold_Value = m_Slider_Threshold.GetPos();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlg_Teaching_Threshold::OnEnChangeEditAdaptiveBlockSize()
{
	UpdateData(TRUE);

	if ((m_iEdit_Adp_BlockSize % 2) == 0)
	{
		m_iEdit_Adp_BlockSize -=1;
	}

	UpdateData(FALSE);
}


void CDlg_Teaching_Threshold::OnEnChangeEditAdaptiveC()
{
	UpdateData(TRUE);

	if (m_iEdit_Adp_C < 1)
		m_iEdit_Adp_C = 1;

	UpdateData(FALSE);
}

int CDlg_Teaching_Threshold::GetThresholdMethod()
{
	CString strMode = _T("");
	int iMethod = 0;
	m_Cmb_Method.GetLBText(m_Cmb_Method.GetCurSel(), strMode);

	if (strMode == _T("THRESH_BINARY"))
		iMethod = ThresholdTypes::THRESH_BINARY;
	else if (strMode == _T("THRESH_BINARY_INV"))
		iMethod = ThresholdTypes::THRESH_BINARY_INV;
	else if (strMode == _T("THRESH_TRUNC"))
		iMethod = ThresholdTypes::THRESH_TRUNC;
	else if (strMode == _T("THRESH_TOZERO"))
		iMethod = ThresholdTypes::THRESH_TOZERO;
	else if (strMode == _T("THRESH_TOZERO_INV"))
		iMethod = ThresholdTypes::THRESH_TOZERO_INV;
	else if (strMode == _T("THRESH_MASK"))
		iMethod = ThresholdTypes::THRESH_MASK;
	else if (strMode == _T("THRESH_OTSU"))
		iMethod = ThresholdTypes::THRESH_OTSU;
	else if (strMode == _T("THRESH_TRIANGLE"))
		iMethod = ThresholdTypes::THRESH_TRIANGLE;
	return iMethod;
}

int CDlg_Teaching_Threshold::GetAdpThresholdType()
{
	CString strMode = _T("");
	int iMethod = 0;
	m_Cmb_Adp_Method.GetLBText(m_Cmb_Adp_Method.GetCurSel(), strMode);

	if (strMode == _T("THRESH_BINARY"))
		iMethod = ThresholdTypes::THRESH_BINARY;
	else if (strMode == _T("THRESH_BINARY_INV"))
		iMethod = ThresholdTypes::THRESH_BINARY_INV;

	return iMethod;
}

int CDlg_Teaching_Threshold::GetAdpThresholdMethod()
{
	CString strMode = _T("");
	int iType = 0;
	m_Cmb_Adp_Type.GetLBText(m_Cmb_Adp_Type.GetCurSel(), strMode);

	if (strMode == _T("ADAPTIVE_THRESH_MEAN_C"))
		iType = AdaptiveThresholdTypes::ADAPTIVE_THRESH_MEAN_C;
	else if (strMode == _T("ADAPTIVE_THRESH_GAUSSIAN_C"))
		iType = AdaptiveThresholdTypes::ADAPTIVE_THRESH_GAUSSIAN_C;

	return iType;

}

int CDlg_Teaching_Threshold::GetAdaptiveUse()
{
	return m_Chk_Adaptive_Use.GetCheck();
}

void CDlg_Teaching_Threshold::UpdateTestImg()
{
	COpenCV::ThresHoldParams tThresholdParams;
	tThresholdParams.iThreshold = m_iEdit_Threshold_Value;
	tThresholdParams.eType = (ThresholdTypes)GetThresholdMethod();

	m_pOpenCV->ThresHold(*m_pDlgItem->m_ViewData_Src.img, *m_pDlgItem->m_ViewData_Dst.img, tThresholdParams);
	m_pDlgItem->DrawViewData(m_pDlgItem->m_ViewData_Dst);
}


void CDlg_Teaching_Threshold::OnEnChangeEditThreshold()
{
	UpdateData(TRUE);

	if (m_iEdit_Threshold < 1)
		m_iEdit_Threshold = 1;
	if (m_iEdit_Threshold > 255)
		m_iEdit_Threshold = 255;

	UpdateData(FALSE);
}

LRESULT CDlg_Teaching_Threshold::OnReceiveImg(WPARAM wParam, LPARAM lParam)
{
	m_pDlgItem->m_ViewData_Src.img = (Mat*)lParam;
	m_pDlgItem->DrawViewData(m_pDlgItem->m_ViewData_Src);

	return 0;
}

int CDlg_Teaching_Threshold::GetThreshold()
{
	return m_iEdit_Threshold;
}

int CDlg_Teaching_Threshold::GetAdaptiveC()
{
	return m_iEdit_Adp_C;
}

int CDlg_Teaching_Threshold::GetAdaptive_BlockSize()
{

	if (m_iEdit_Adp_BlockSize < 3)
		m_iEdit_Adp_BlockSize = 3;
	
	UpdateData(FALSE);

	return m_iEdit_Adp_BlockSize;
}


void CDlg_Teaching_Threshold::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (IDC_SLIDER_THRESHOLD == pScrollBar->GetDlgCtrlID())
	{
		UpdateData(TRUE);

		m_iEdit_Threshold_Value = m_Slider_Threshold.GetPos();

		if (m_pDlgItem->m_ViewData_Src.img->empty() != TRUE)
		{
			UpdateTestImg();
		}

		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlg_Teaching_Threshold::OnEnChangeEditThresholdVal()
{
	int iMin = m_Slider_Threshold.GetRangeMin();
	int iMax = m_Slider_Threshold.GetRangeMax();

	UpdateData(TRUE);

	if (m_iEdit_Threshold_Value < iMin)
		m_iEdit_Threshold_Value = iMin;
	else if (m_iEdit_Threshold_Value > iMax)
		m_iEdit_Threshold_Value = iMax;

	m_Slider_Threshold.SetPos(m_iEdit_Threshold_Value);

	if (m_pDlgItem->m_ViewData_Src.img != NULL)
	{
		UpdateTestImg();
	}

	UpdateData(FALSE);
}


void CDlg_Teaching_Threshold::OnPaint()
{
	CPaintDC dc(this); 
	m_pDlgItem->DrawImage(m_pDlgItem->m_ViewData_Dst);
}


void CDlg_Teaching_Threshold::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_pDlgItem->ReleaseViewData();
}
