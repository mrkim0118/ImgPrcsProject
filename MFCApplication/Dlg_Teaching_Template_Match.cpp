// Dlg_Teaching_Template_Match.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Teaching_Template_Match.h"
#include "afxdialogex.h"


// CDlg_Teaching_Template_Match 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_Teaching_Template_Match, CDialogEx)

CDlg_Teaching_Template_Match::CDlg_Teaching_Template_Match(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_TEMPLATE_MATCH, pParent)
{
	m_pDlgItem = make_unique<CDlgItem>();
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
END_MESSAGE_MAP()


// CDlg_Teaching_Template_Match 메시지 처리기입니다.


BOOL CDlg_Teaching_Template_Match::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pDlgItem->m_pWnd = GetDlgItem(IDC_STATIC_TEMPLATE_MODEL);
	m_pDlgItem->m_pWnd_Ext = GetDlgItem(IDC_STATIC_TEMPLATE_NORMALIZE);
	m_pDlgItem->InitViewData(m_pDlgItem->m_pWnd , m_pDlgItem->m_pWnd_Ext);

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
void CDlg_Teaching_Template_Match::CreateModelImg(Mat SrcImg, Mat& DstImg, CPoint ptStart, CPoint ptEnd, CRect rect)
{
	int iPoint_Left, iPoint_Right, iPoint_Top, iPoint_Bottom;

	if (ptStart.x > ptEnd.x)
	{
		iPoint_Left = ptEnd.x;
		iPoint_Right = ptStart.x;
	}
	else
	{
		iPoint_Left = ptStart.x;
		iPoint_Right = ptEnd.x;
	}

	if (ptStart.y > ptEnd.y)
	{
		iPoint_Top = ptEnd.y;
		iPoint_Bottom = ptStart.y;
	}
	else
	{
		iPoint_Top = ptStart.y;
		iPoint_Bottom = ptEnd.y;
	}

	double rx = ((double)iPoint_Left / rect.right);
	double rx2 = ((double)iPoint_Right / rect.right);

	double ry = ((double)iPoint_Top / rect.bottom);
	double ry2 = ((double)iPoint_Bottom / rect.bottom);

	int iStartX = (int)SrcImg.cols*rx;
	int IStartY = (int)SrcImg.rows*ry;

	int iWidth = (int)(SrcImg.cols*rx2 - SrcImg.cols*rx);
	int iHeight = (int)(SrcImg.rows*ry2 - SrcImg.rows*ry);

	Rect ROI(iStartX, IStartY, iWidth, iHeight);
	DstImg = SrcImg(ROI);

	//m_pDlgItem->CorrectBitMapWidth()

	//// Width 값 4의 배수로 맞추기.
	//int iModelSpanWidth = 0;

	//iModelSpanWidth = iWidth;
	//if (iModelSpanWidth % 4 != 0)
	//{
	//	iModelSpanWidth = iWidth + (4 - iWidth % 4);
	//}
	//else
	//{
	//	iModelSpanWidth = iWidth;
	//}

	//Mat newimg(iHeight, iModelSpanWidth, rectimg.type());

	//for (int j = 0; j < rectimg.rows; j++)
	//{
	//	uchar *p = newimg.ptr<uchar>(j);
	//	for (int i = 0; i < iModelSpanWidth; i++)
	//	{
	//		if (i >= rectimg.cols)
	//		{
	//			p[i * 3 + 0] = 0;
	//			p[i * 3 + 1] = 0;
	//			p[i * 3 + 2] = 0;
	//		}
	//		else
	//		{
	//			p[i * 3 + 0] = rectimg.at<uchar>(j, (i * 3 + 0));
	//			p[i * 3 + 1] = rectimg.at<uchar>(j, (i * 3 + 1));
	//			p[i * 3 + 2] = rectimg.at<uchar>(j, (i * 3 + 2));
	//		}
	//	}
	//}
	//DstImg = newimg.clone();

}
LRESULT CDlg_Teaching_Template_Match::OnReceiveImg(WPARAM wParam, LPARAM lParam)
{
	m_pDlgItem->m_ViewData_Src.img = (Mat*)lParam;
	m_pDlgItem->DrawViewData(m_pDlgItem->m_ViewData_Src);
	*m_pModelImg = m_pDlgItem->m_ViewData_Src.img->clone();

	return 0;
}

LRESULT CDlg_Teaching_Template_Match::OnReceiveNorm(WPARAM wParam, LPARAM lParam)
{
	m_pDlgItem->m_ViewData_Dst.img = (Mat*)lParam;
	m_pDlgItem->DrawViewData(m_pDlgItem->m_ViewData_Dst);
	return 0;
}

void CDlg_Teaching_Template_Match::OnPaint()
{
	CPaintDC dc(this);
	m_pDlgItem->DrawImage(m_pDlgItem->m_ViewData_Src);
	m_pDlgItem->DrawImage(m_pDlgItem->m_ViewData_Dst);
}


void CDlg_Teaching_Template_Match::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_pDlgItem->ReleaseViewData();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
