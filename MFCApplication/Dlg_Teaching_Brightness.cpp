// Dlg_Teaching_Brightness.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Teaching_Brightness.h"
#include "afxdialogex.h"


// CDlg_Teaching_Brightness 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_Teaching_Brightness, CDialogEx)

CDlg_Teaching_Brightness::CDlg_Teaching_Brightness(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_BRIGHTNESS, pParent)
	, m_iEdit_Value_Brightness(0)
	, m_fEdit_Value_Contrast(0)
	, m_iEdit_Brightness(0)
	, m_fEdit_Contrast(0)
{
	m_pOpenCV = make_unique<COpenCV>();
}

CDlg_Teaching_Brightness::~CDlg_Teaching_Brightness()
{
}

void CDlg_Teaching_Brightness::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_Slider_Brightness);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_Slider_Contrast);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS_VAL, m_iEdit_Value_Brightness);
	DDX_Text(pDX, IDC_EDIT_CONTRAST_VAL, m_fEdit_Value_Contrast);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_iEdit_Brightness);
	DDX_Text(pDX, IDC_EDIT_CONTRAST, m_fEdit_Contrast);
}


BEGIN_MESSAGE_MAP(CDlg_Teaching_Brightness, CDialogEx)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_BRIGHTNESS_VAL, &CDlg_Teaching_Brightness::OnEnChangeEditBrightnessVal)
	ON_EN_CHANGE(IDC_EDIT_CONTRAST_VAL, &CDlg_Teaching_Brightness::OnEnChangeEditContrastVal)
	ON_MESSAGE(WM_BRIGHTNESS, OnReceiveImg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlg_Teaching_Brightness 메시지 처리기입니다.


void CDlg_Teaching_Brightness::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (IDC_SLIDER_BRIGHTNESS == pScrollBar->GetDlgCtrlID())
	{
		UpdateData(TRUE);

		m_iEdit_Value_Brightness = m_Slider_Brightness.GetPos() -255;

		if (m_ViewData_Src.img->empty() != TRUE)
		{
			UpdateTestImg(pScrollBar);
		}

		UpdateData(FALSE);
	}
	else if (IDC_SLIDER_CONTRAST == pScrollBar->GetDlgCtrlID())
	{
		UpdateData(TRUE);

		m_fEdit_Value_Contrast = (m_Slider_Contrast.GetPos() - 100) / 10;

		if (m_ViewData_Src.img->empty() != TRUE)
		{
			UpdateTestImg(pScrollBar);
		}

		UpdateData(FALSE);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlg_Teaching_Brightness::UpdateTestImg(CScrollBar* pScrollBar)
{

	COpenCV::BrightnessParams tBrightnessParams;

	tBrightnessParams.fContrast = m_fEdit_Value_Contrast;
	tBrightnessParams.iBrightness = m_iEdit_Value_Brightness;

	m_pOpenCV->Brightness(*m_ViewData_Src.img, *m_ViewData_Dst.img, tBrightnessParams);
	DrawViewData(m_ViewData_Dst);

}


BOOL CDlg_Teaching_Brightness::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pWnd = GetDlgItem((IDC_STATIC_BRIGHTNESS_TEST));
	InitViewData(m_pWnd);

	m_Slider_Brightness.SetRange(0, 512);
	m_Slider_Brightness.SetPos(m_iEdit_Value_Brightness+256);
	m_Slider_Brightness.SetLineSize(10);
	m_Slider_Brightness.SetPageSize(10);

	m_Slider_Contrast.SetRange(0, 200);
	m_Slider_Contrast.SetPos((m_fEdit_Value_Contrast+100));
	m_Slider_Contrast.SetLineSize(10);
	m_Slider_Contrast.SetPageSize(10);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlg_Teaching_Brightness::OnEnChangeEditBrightnessVal()
{
	int iMin = m_Slider_Brightness.GetRangeMin();
	int iMax = m_Slider_Brightness.GetRangeMax();

	UpdateData(TRUE);

	if (m_iEdit_Value_Brightness < iMin)
		m_iEdit_Value_Brightness = iMin;

	else if (m_iEdit_Value_Brightness > iMax)
		m_iEdit_Value_Brightness = iMax;

	m_Slider_Brightness.SetPos(m_iEdit_Value_Brightness);

	if (m_ViewData_Src.img != NULL)
	{
		UpdateTestImg((CScrollBar*)GetDlgItem(IDC_SLIDER_BRIGHTNESS));
	}

	UpdateData(FALSE);
}


void CDlg_Teaching_Brightness::OnEnChangeEditContrastVal()
{
	int iMin = m_Slider_Contrast.GetRangeMin() / 10;
	int iMax = m_Slider_Contrast.GetRangeMax() / 10;

	UpdateData(TRUE);

	if (m_fEdit_Value_Contrast < iMin)
		m_fEdit_Value_Contrast = iMin;

	else if (m_fEdit_Value_Contrast > iMax)
		m_fEdit_Value_Contrast = iMax;

	m_Slider_Contrast.SetPos(m_fEdit_Value_Contrast);

	if (m_ViewData_Src.img != NULL)
	{
		UpdateTestImg((CScrollBar*)GetDlgItem(IDC_SLIDER_CONTRAST));
	}

	UpdateData(FALSE);
}

LRESULT CDlg_Teaching_Brightness::OnReceiveImg(WPARAM wParam, LPARAM lParam)
{
	m_ViewData_Src.img = (Mat*)lParam;
	DrawViewData(m_ViewData_Src);

	return 0;
}

int CDlg_Teaching_Brightness::GetBrightness()
{
	UpdateData(TRUE);
	return m_iEdit_Brightness;
	UpdateData(FALSE);
}

float CDlg_Teaching_Brightness::GetContrast()
{
	UpdateData(TRUE);
	return m_fEdit_Contrast;
	UpdateData(FALSE);
}




void CDlg_Teaching_Brightness::OnPaint()
{
	CPaintDC dc(this); 

	DrawImage(m_ViewData_Dst);
}


void CDlg_Teaching_Brightness::OnDestroy()
{
	CDialogEx::OnDestroy();

	ReleaseViewData();

}
