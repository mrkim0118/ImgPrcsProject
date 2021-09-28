// Dlg_Teaching_Histogram.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Teaching_Histogram.h"
#include "afxdialogex.h"


// CDlg_Teaching_Histogram 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_Teaching_Histogram, CDialogEx)

CDlg_Teaching_Histogram::CDlg_Teaching_Histogram(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_HISTOGRAM, pParent)
	, m_iEdit_Bin_Num(256)
	, m_iEdit_Value_Bin_Number(0)
	, m_iEdit_Value_Min(0)
	, m_iEdit_Value_Max(256)
{
	m_pOpenCV = make_unique<COpenCV>();
}

CDlg_Teaching_Histogram::~CDlg_Teaching_Histogram()
{
}

void CDlg_Teaching_Histogram::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BIN_NUMBER, m_iEdit_Bin_Num);
	DDX_Control(pDX, IDC_SLIDER_HISTOGRAM, m_Slider_Histogram);
	DDX_Text(pDX, IDC_EDIT_VALUE_BIN_NUMBER, m_iEdit_Value_Bin_Number);
	DDX_Text(pDX, IDC_EDIT_HISTOGRAM_VALUE_MIN, m_iEdit_Value_Min);
	DDX_Text(pDX, IDC_EDIT_HISTOGRAM_VALUE_MAX, m_iEdit_Value_Max);
	DDX_Control(pDX, IDC_CHK_EQUALIZE, m_Chk_Equalize);
	DDX_Control(pDX, IDC_CHK_STRETCH, m_Chk_Stretch);
}


BEGIN_MESSAGE_MAP(CDlg_Teaching_Histogram, CDialogEx)
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_HISTOGRAM, OnReceiveImg)
	ON_EN_CHANGE(IDC_EDIT_VALUE_BIN_NUMBER, &CDlg_Teaching_Histogram::OnEnChangeEditValueBinNumber)
	ON_BN_CLICKED(IDC_CHK_EQUALIZE, &CDlg_Teaching_Histogram::OnBnClickedChkEqualize)
	ON_BN_CLICKED(IDC_CHK_STRETCH, &CDlg_Teaching_Histogram::OnBnClickedChkStretch)
END_MESSAGE_MAP()


// CDlg_Teaching_Histogram 메시지 처리기입니다.


void CDlg_Teaching_Histogram::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (IDC_SLIDER_HISTOGRAM == pScrollBar->GetDlgCtrlID())
	{
		UpdateData(TRUE);

		m_iEdit_Value_Bin_Number = m_Slider_Histogram.GetPos();

		if (m_ViewData_Src.img->empty() != TRUE)
		{
			UpdateTestImg();
		}

		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CDlg_Teaching_Histogram::UpdateTestImg()
{
	COpenCV::HistogramParams tHistogramParams;
	tHistogramParams.iBinNum = m_iEdit_Value_Bin_Number;
	tHistogramParams.iValueMin = m_iEdit_Value_Min;
	tHistogramParams.iValueMax = m_iEdit_Value_Max;

	m_pOpenCV->Histogram(*m_ViewData_Src.img, *m_ViewData_Dst.img, tHistogramParams);
	DrawViewData(m_ViewData_Dst);
}

BOOL CDlg_Teaching_Histogram::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pWnd = GetDlgItem((IDC_STATIC_HISTOGRAM_TEST));
	InitViewData(m_pWnd);

	m_Slider_Histogram.SetRange(1, 256);
	m_Slider_Histogram.SetPos(m_iEdit_Value_Bin_Number);
	m_Slider_Histogram.SetLineSize(1);
	m_Slider_Histogram.SetPageSize(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlg_Teaching_Histogram::OnEnChangeEditValueBinNumber()
{
	int iMin = m_Slider_Histogram.GetRangeMin();
	int iMax = m_Slider_Histogram.GetRangeMax();

	UpdateData(TRUE);

	if (m_iEdit_Value_Bin_Number < iMin)
		m_iEdit_Value_Bin_Number = iMin;

	else if (m_iEdit_Value_Bin_Number > iMax)
		m_iEdit_Value_Bin_Number = iMax;

	m_Slider_Histogram.SetPos(m_iEdit_Value_Bin_Number);

	if (m_ViewData_Src.img != NULL)
	{
		UpdateTestImg();
	}

	UpdateData(FALSE);
}

LRESULT CDlg_Teaching_Histogram::OnReceiveImg(WPARAM wParam, LPARAM lParam)
{
	m_ViewData_Src.img = (Mat*)lParam;
	DrawViewData(m_ViewData_Src);

	return 0;
}

void CDlg_Teaching_Histogram::OnBnClickedChkEqualize()
{
	if (m_Chk_Stretch.GetCheck() == TRUE)
		m_Chk_Equalize.SetCheck(FALSE);
}


void CDlg_Teaching_Histogram::OnBnClickedChkStretch()
{
	if (m_Chk_Equalize.GetCheck() == TRUE)
		m_Chk_Stretch.SetCheck(FALSE);
}

int CDlg_Teaching_Histogram::GetEqualizeUse()
{
	return m_Chk_Equalize.GetCheck();
}

int CDlg_Teaching_Histogram::GetStretchUse()
{
	return m_Chk_Stretch.GetCheck();
}
