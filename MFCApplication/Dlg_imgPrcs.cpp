// Dlg_ImgPrcs.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_ImgPrcs.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CDlg_ImgPrcs 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_ImgPrcs, CDialogEx)

CDlg_ImgPrcs::CDlg_ImgPrcs(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_IMG_PRCS, pParent)
{
}

CDlg_ImgPrcs::~CDlg_ImgPrcs()
{

}

void CDlg_ImgPrcs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_MODE, m_Cmb_Mode);
	DDX_Control(pDX, IDC_TEACHING_TAB, m_Teaching_Tab);
}

void CDlg_ImgPrcs::HideAllTeachingDlg()
{
	m_pDlgMorphology->ShowWindow(SW_HIDE);
	m_pDlgThreshold->ShowWindow(SW_HIDE);
	m_pDlgTemplateMatch->ShowWindow(SW_HIDE);
	m_pDlgHistogram->ShowWindow(SW_HIDE);
	m_pDlgBrightness->ShowWindow(SW_HIDE);
}

void CDlg_ImgPrcs::InitTeachingTab()
{
	m_Teaching_Tab.InsertItem(0, _T("Threshold"));
	m_Teaching_Tab.InsertItem(1, _T("Morphology"));
	m_Teaching_Tab.InsertItem(2, _T("TemplateMatch"));
	m_Teaching_Tab.InsertItem(3, _T("Histogram"));
	m_Teaching_Tab.InsertItem(4, _T("Brightness"));
	m_Teaching_Tab.SetCurSel(0);

	CRect rect;
	m_Teaching_Tab.GetWindowRect(&rect);

	m_pDlgThreshold = make_unique<CDlg_Teaching_Threshold>();
	m_pDlgThreshold->Create(IDD_DLG_THRESHOLD, &m_Teaching_Tab);
	m_pDlgThreshold->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pDlgThreshold->ShowWindow(SW_HIDE);

	m_pDlgMorphology = make_unique<CDlg_Teaching_Morphology>();
	m_pDlgMorphology->Create(IDD_DLG_MORPHOLOGY, &m_Teaching_Tab);
	m_pDlgMorphology->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pDlgMorphology->ShowWindow(SW_HIDE);

	m_pDlgTemplateMatch = make_unique<CDlg_Teaching_Template_Match>();
	m_pDlgTemplateMatch->Create(IDD_DLG_TEMPLATE_MATCH, &m_Teaching_Tab);
	m_pDlgTemplateMatch->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pDlgTemplateMatch->ShowWindow(SW_HIDE);

	m_pDlgHistogram = make_unique<CDlg_Teaching_Histogram>();
	m_pDlgHistogram->Create(IDD_DLG_HISTOGRAM, &m_Teaching_Tab);
	m_pDlgHistogram->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pDlgHistogram->ShowWindow(SW_HIDE);

	m_pDlgBrightness = make_unique<CDlg_Teaching_Brightness>();
	m_pDlgBrightness->Create(IDD_DLG_BRIGHTNESS, &m_Teaching_Tab);
	m_pDlgBrightness->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pDlgBrightness->ShowWindow(SW_HIDE);


}

int CDlg_ImgPrcs::GetInspMode()
{
	CString strMode = _T("");
	m_Cmb_Mode.GetLBText(m_Cmb_Mode.GetCurSel(), strMode);

	if (strMode == _T("Morphorogy"))
		m_iInspMode = CImgPrcs::_MODE_MORPHOLOGY_;
	else if (strMode == _T("Threshold"))
		m_iInspMode = CImgPrcs::_MODE_THRESHOLD_;
	else if (strMode == _T("TemplateMatch"))
		m_iInspMode = CImgPrcs::_MODE_TEMPLATE_MATCH_;
	else if (strMode == _T("Histogram"))
		m_iInspMode = CImgPrcs::_MODE_HISTOGRAM_;
	else if (strMode == _T("Brightness"))
		m_iInspMode = CImgPrcs::_MODE_BRIGHTNESS_;
	else if (strMode == _T("Contour"))
		m_iInspMode = CImgPrcs::_MODE_CONTOUR_;
	else if (strMode == _T("Labeling"))
		m_iInspMode = CImgPrcs::_MODE_LABELING_; 
	else if (strMode == _T("Mask"))
		m_iInspMode = CImgPrcs::_MODE_MASK_;

	return m_iInspMode;
}

void CDlg_ImgPrcs::OnDrawROI(CDlgItem::ViewData& viewdata)
{
	// Picture Control DC를 생성.
	// IDC_PC_IMAGE는 Picture Control의 Resource ID.

	// Picture Control 크기를 얻는다.
	//CRect rect;
	//GetDlgItem(IDC_STATIC_SRC_VIEW)->GetClientRect(&rect);

	//CWnd* pWnd = NULL;
	//pWnd = GetDlgItem(IDC_STATIC_SRC_VIEW);
	//CDC *pDCc = pWnd->GetDC();

	CDC memDC;
	CBitmap *pOldBitmap, bitmap;
	CPen *pOldPen = NULL;
	CBrush *pOldBrush = NULL;
	CPen pen;
	if (m_iInspMode == CImgPrcs::_MODE_TEMPLATE_MATCH_)
		pen.CreatePen(PS_SOLID, 2, COLOR_RED);
	else
		pen.CreatePen(PS_SOLID, 2, COLOR_YELLOW);

	CBrush brush;
	brush.CreateStockObject(NULL_BRUSH);

	// Picture Control DC에 호환되는 새로운 CDC를 생성. (임시 버퍼)
	memDC.CreateCompatibleDC(viewdata.dc);
	// Picture Control의 크기와 동일한 비트맵을 생성.
	bitmap.CreateCompatibleBitmap(viewdata.dc, viewdata.rect.Width(), viewdata.rect.Height());
	// 임시 버퍼에서 방금 생성한 비트맵을 선택하면서, 이전 비트맵을 보존.
	pOldBitmap = memDC.SelectObject(&bitmap);

	SetStretchBltMode(memDC.GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(memDC.GetSafeHdc(), 0, 0, viewdata.rect.Width(), viewdata.rect.Height(), 0, 0, viewdata.ScreenImg->cols, viewdata.ScreenImg->rows, viewdata.ScreenImg->data, viewdata.BitMapInfo, DIB_RGB_COLORS, SRCCOPY);

	pOldPen = memDC.SelectObject(&pen);
	pOldBrush = memDC.SelectObject(&brush);

	memDC.Rectangle(m_ptROI_Start.x, m_ptROI_Start.y, m_ptROI_End.x, m_ptROI_End.y);
	
	memDC.SelectObject(pOldPen);
	memDC.SelectObject(pOldBrush);

	// 임시 버퍼에 검은색으로 채움.
	//memDC.PatBlt(0, 0, viewdata.rect.Width(), viewdata.rect.Height(), BLACKNESS);
	// 임시 버퍼를 Picture Control에 그린다.
	viewdata.dc->BitBlt(0, 0, viewdata.rect.Width(), viewdata.rect.Height(), &memDC, 0, 0, SRCCOPY);

	// 이전 비트맵으로 재설정.
    memDC.SelectObject(pOldBitmap);

	pOldBrush->DeleteObject();
	pOldPen->DeleteObject();

	// 생성한 리소스 해제.
	memDC.DeleteDC();
	bitmap.DeleteObject();

}


BEGIN_MESSAGE_MAP(CDlg_ImgPrcs, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_LOAD_IMG, &CDlg_ImgPrcs::OnBnClickedBtnLoadImg)
	ON_COMMAND(ID_MENU_IMG_PRCS, &CDlg_ImgPrcs::OnMenuImgPrcs)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TEACHING_TAB, &CDlg_ImgPrcs::OnTcnSelchangeTeachingTab)
	ON_BN_CLICKED(IDC_BTN_SAVE_IMG, &CDlg_ImgPrcs::OnBnClickedBtnSaveImg)
	ON_BN_CLICKED(IDC_BTN_IMG_PRCS_START, &CDlg_ImgPrcs::OnBnClickedBtnImgPrcsStart)
	ON_BN_CLICKED(IDC_BTN_DST_TO_SRC, &CDlg_ImgPrcs::OnBnClickedBtnDstToSrc)
	ON_BN_CLICKED(IDC_BTN_DST_TO_THRESHOLD_DLG, &CDlg_ImgPrcs::OnBnClickedBtnSrcToTeachingDlg)
	ON_BN_CLICKED(IDC_BTN_CVT_GRAY, &CDlg_ImgPrcs::OnBnClickedBtnCvtGray)
	ON_CBN_SELCHANGE(IDC_CMB_MODE, &CDlg_ImgPrcs::OnCbnSelchangeCmbMode)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CROP_IMG, &CDlg_ImgPrcs::OnBnClickedBtnCropImg)
END_MESSAGE_MAP()


// CDlg_ImgPrcs 메시지 처리기입니다.


void CDlg_ImgPrcs::OnBnClickedBtnLoadImg()
{
	DWORD MAXFILE = 2562;

	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, _T("image file(*.jpg;*.bmp;*.png;)|*.jpg;*.bmp;*.png;|All Files(*.*)|*.*||"));
	//FileDlg.m_ofn.nMaxFile = MAXFILE;


	if (FileDlg.DoModal() == IDOK)
	{
		m_ImgFileList.clear();

		for (POSITION pos = FileDlg.GetStartPosition(); pos != NULL;)
			m_ImgFileList.push_back(FileDlg.GetNextPathName(pos));
	}
	if (m_ImgFileList.size() == 1)
	{
		CString path = *m_ImgFileList.begin();
		CT2CA pszString(path);
		string strPath(pszString);

		*m_ViewData_Src.img =  m_pOpenCV->LoadImg(strPath);
		
		DrawViewData(m_ViewData_Src);
	}
}




void CDlg_ImgPrcs::OnMenuImgPrcs()
{
	this->DoModal();
}


void CDlg_ImgPrcs::OnBnClickedBtnSaveImg()
{
	CFileDialog fileDlg(TRUE, L"bmp", L"*.bmp", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT /*| OFN_NOCHANGEDIR*/ | OFN_ALLOWMULTISELECT, L"All Files (*.*)|*.*|Bmp Files (*.bmp)|*.bmp|", this);

	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetPathName();
		CT2CA pszString(path);
		string strPath(pszString);
		m_pOpenCV->SaveImg(strPath, *m_ViewData_Dst.img);
	}
}


BOOL CDlg_ImgPrcs::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pOpenCV = make_unique<COpenCV>();
	InitTeachingTab();

	m_pDlgExpansionView = make_unique<CDlg_Expansion_View>();
	m_pDlgExpansionView->Create(IDD_DLG_EXPANSION_VIEW, this);
	m_pDlgExpansionView->ShowWindow(SW_HIDE);
	

	m_Cmb_Mode.AddString(_T("Threshold")); 
	m_Cmb_Mode.AddString(_T("Morphorogy")); 
	m_Cmb_Mode.AddString(_T("TemplateMatch"));
	m_Cmb_Mode.AddString(_T("Histogram"));
	m_Cmb_Mode.AddString(_T("Brightness"));
	m_Cmb_Mode.AddString(_T("Contour"));
	m_Cmb_Mode.AddString(_T("Labeling")); 
	m_Cmb_Mode.AddString(_T("Mask")); 
	m_Cmb_Mode.SetCurSel(0);

	m_pWnd = GetDlgItem(IDC_STATIC_SRC_VIEW);
	m_pWnd_Ext = GetDlgItem(IDC_STATIC_DST_VIEW);
	InitViewData(m_pWnd, m_pWnd_Ext);
	m_pDlgThreshold->ShowWindow(SW_SHOW);

	GetDlgItem(IDC_STATIC_SRC_VIEW)->GetWindowRect(&m_DlgRect_Src);
	GetDlgItem(IDC_STATIC_DST_VIEW)->GetWindowRect(&m_DlgRect_Dst);

	ScreenToClient(&m_DlgRect_Src);
	ScreenToClient(&m_DlgRect_Dst);

	//m_pExpansion_View->Create(IDD_DLG_EXPANSION_VIEW, this);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlg_ImgPrcs::OnBnClickedBtnImgPrcsStart()
{

	m_iInspMode = GetInspMode();

	switch (m_iInspMode)
	{
	case CImgPrcs::_MODE_MORPHOLOGY_:
	{
		COpenCV::ElementParams tElementParams;
		tElementParams.eShape = (MorphShapes)m_pDlgMorphology->GetElementShape();
		tElementParams.anchor = Point(m_pDlgMorphology->GetElementAnchorX(), m_pDlgMorphology->GetElementAnchorY());
		tElementParams.ksize = Size(m_pDlgMorphology->GetElemetSIze(), m_pDlgMorphology->GetElemetSIze());

		COpenCV::MorphologyParams tMorphologyParams;
		tMorphologyParams.eOperation = (MorphTypes)m_pDlgMorphology->GetMorphologyOperation();
		tMorphologyParams.Anchor = Point(m_pDlgMorphology->GetMorphAnchorX(), m_pDlgMorphology->GetMorphAnchorY());
		tMorphologyParams.Kernel = getStructuringElement(tElementParams.eShape, tElementParams.ksize, tElementParams.anchor);

		m_pOpenCV->Morphology(*m_ViewData_Src.img, *m_ViewData_Dst.img, tMorphologyParams, tElementParams);
		break;
	}
	case CImgPrcs::_MODE_THRESHOLD_:
	{

		if (m_pDlgThreshold->GetAdaptiveUse() == TRUE)
		{
			COpenCV::AdaptiveThresHoldParams tAdaptiveThresHoldParams;
			tAdaptiveThresHoldParams.eMethod = (AdaptiveThresholdTypes)m_pDlgThreshold->GetAdpThresholdMethod();
			tAdaptiveThresHoldParams.eType = (ThresholdTypes)m_pDlgThreshold->GetAdpThresholdType();
			tAdaptiveThresHoldParams.iBlockSize = m_pDlgThreshold->GetAdaptive_BlockSize();
			tAdaptiveThresHoldParams.iC = m_pDlgThreshold->GetAdaptiveC();

			m_pOpenCV->ThresHold_Adaptive(*m_ViewData_Src.img, *m_ViewData_Dst.img, tAdaptiveThresHoldParams);
		}
		else
		{
			COpenCV::ThresHoldParams tThresHoldParams;
			tThresHoldParams.eType = (ThresholdTypes)m_pDlgThreshold->GetThresholdMethod();
			tThresHoldParams.iThreshold = m_pDlgThreshold->GetThreshold();

			m_pOpenCV->ThresHold(*m_ViewData_Src.img, *m_ViewData_Dst.img, tThresHoldParams);
		}
		break;
	}
	case CImgPrcs::_MODE_MASK_:
		break;
	case CImgPrcs::_MODE_LABELING_:
	{
		COpenCV::LabelingParams tLabelingParams;
		m_pOpenCV->Labeling(*m_ViewData_Dst.img, *m_ViewData_Dst.img, tLabelingParams);
		break;
	}
	case CImgPrcs::_MODE_CONTOUR_:
	{
		COpenCV::ContourParams tContourParams;
		m_pOpenCV->Contour(*m_ViewData_Dst.img, *m_ViewData_Dst.img, tContourParams);
		break;
	}
	case CImgPrcs::_MODE_HISTOGRAM_:
	{
		if (m_pDlgHistogram->GetEqualizeUse() == TRUE)
			m_pOpenCV->Histogram_Equalize(*m_ViewData_Src.img, *m_ViewData_Dst.img);
		else if(m_pDlgHistogram->GetStretchUse() == TRUE)
			m_pOpenCV->Histogram_Strecth(*m_ViewData_Src.img, *m_ViewData_Dst.img);
		else
		{
			COpenCV::HistogramParams tHistogramParams;
			m_pOpenCV->Histogram(*m_ViewData_Src.img, *m_ViewData_Dst.img, tHistogramParams);
		}
		break;
	}
	case CImgPrcs::_MODE_TEMPLATE_MATCH_:
	{
		COpenCV::TemplateMatchParams tTemplateMatchParams;
		tTemplateMatchParams.Model = m_pDlgTemplateMatch->GetModelImg();
		tTemplateMatchParams.eTemplateMatchModes = (TemplateMatchModes)m_pDlgTemplateMatch->GetTemplateMatchMethod();
		m_pOpenCV->TemplateMatching(*m_ViewData_Src.img, *m_ViewData_Dst.img, tTemplateMatchParams, tTemplateMatchParams.Normalize);

		*m_pMessageImg = tTemplateMatchParams.Normalize.clone();
		::SendMessage(m_pDlgTemplateMatch->GetSafeHwnd(), WM_TEMPLATE_MATCH_NORM, NULL, (LPARAM)m_pMessageImg);
		break;
	}
	case CImgPrcs::_MODE_BRIGHTNESS_:
	{
		COpenCV::BrightnessParams tBrightnessParams;
		tBrightnessParams.iBrightness = m_pDlgBrightness->GetBrightness();
		tBrightnessParams.fContrast = m_pDlgBrightness->GetContrast();
		m_pOpenCV->Brightness(*m_ViewData_Src.img, *m_ViewData_Dst.img, tBrightnessParams);
		break;
	}
	}
	DrawViewData(m_ViewData_Dst);
}

void CDlg_ImgPrcs::OnTcnSelchangeTeachingTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	int iSelected = m_Teaching_Tab.GetCurSel();

	HideAllTeachingDlg();

	switch (iSelected)
	{
	case CImgPrcs::_MODE_THRESHOLD_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Teaching Dlg"));
		m_pDlgThreshold->ShowWindow(SW_SHOW);
		break;
	}
	case CImgPrcs::_MODE_MORPHOLOGY_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Teaching Dlg"));
		m_pDlgMorphology->ShowWindow(SW_SHOW);
		break;
	}
	case CImgPrcs::_MODE_TEMPLATE_MATCH_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Model"));
		m_pDlgTemplateMatch->ShowWindow(SW_SHOW);
		break;
	}
	case CImgPrcs::_MODE_HISTOGRAM_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Teaching Dlg"));
		m_pDlgHistogram->ShowWindow(SW_SHOW);
		break;
	}
	case CImgPrcs::_MODE_BRIGHTNESS_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Teaching Dlg"));
		m_pDlgBrightness->ShowWindow(SW_SHOW);
		break;
	}
	}

	m_Cmb_Mode.SetCurSel(iSelected);
	*pResult = 0;
}


void CDlg_ImgPrcs::OnBnClickedBtnDstToSrc()
{
	*m_ViewData_Src.img = m_ViewData_Dst.img->clone();
	DrawViewData(m_ViewData_Src);


}


void CDlg_ImgPrcs::OnBnClickedBtnSrcToTeachingDlg()
{
	m_iInspMode = GetInspMode();

	switch (m_iInspMode)
	{
	case CImgPrcs::_MODE_THRESHOLD_:
	{
		if (m_pDlgThreshold != NULL)
		{
			*m_pMessageImg = m_ViewData_Src.img->clone();
			::SendMessage(m_pDlgThreshold->GetSafeHwnd(), WM_THRESHOLD_TEST, NULL, (LPARAM)m_pMessageImg);
		}
		break;
	}
	case CImgPrcs::_MODE_MORPHOLOGY_:
	{
		if (m_pDlgMorphology != NULL)
		{
			*m_pMessageImg = m_ViewData_Src.img->clone();
			::SendMessage(m_pDlgMorphology->GetSafeHwnd(), WM_MORPHOLOGY_TEST, NULL, (LPARAM)m_pMessageImg);
		}
		break;
	}
	case CImgPrcs::_MODE_TEMPLATE_MATCH_:
	{
		if (m_pDlgTemplateMatch != NULL)
		{
			Mat Src = m_ViewData_Src.img->clone();
			m_pDlgTemplateMatch->CreateModelImg(Src, *m_pMessageImg, m_ptROI_Start, m_ptROI_End, m_ViewData_Src.rect);
			::SendMessage(m_pDlgTemplateMatch->GetSafeHwnd(), WM_TEMPLATE_MATCH_MODEL, NULL, (LPARAM)m_pMessageImg);
		}
		break;
	}
	case CImgPrcs::_MODE_HISTOGRAM_:
	{
		if (m_pDlgHistogram != NULL)
		{
			*m_pMessageImg = m_ViewData_Src.img->clone();
			::SendMessage(m_pDlgHistogram->GetSafeHwnd(), WM_HISTOGRAM, NULL, (LPARAM)m_pMessageImg);
		}
		break;
	}
	case CImgPrcs::_MODE_BRIGHTNESS_:
	{
		if (m_pDlgBrightness != NULL)
		{
			*m_pMessageImg = m_ViewData_Src.img->clone();
			::SendMessage(m_pDlgBrightness->GetSafeHwnd(), WM_BRIGHTNESS, NULL, (LPARAM)m_pMessageImg);
		}
		break;
	}
	}

}

void CDlg_ImgPrcs::OnPaint()
{
	CPaintDC dc(this); 
	DrawImage(m_ViewData_Dst);
	DrawImage(m_ViewData_Src);
}


void CDlg_ImgPrcs::OnCbnSelchangeCmbMode()
{
	m_iInspMode = GetInspMode();

	HideAllTeachingDlg();
	switch (m_iInspMode)
	{
	case CImgPrcs::_MODE_THRESHOLD_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Teaching Dlg"));
		m_pDlgThreshold->ShowWindow(SW_SHOW);
		break;
	}
	case CImgPrcs::_MODE_MORPHOLOGY_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Teaching Dlg"));
		m_pDlgMorphology->ShowWindow(SW_SHOW);
		break;
	}
	case CImgPrcs::_MODE_TEMPLATE_MATCH_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Model"));
		m_pDlgTemplateMatch->ShowWindow(SW_SHOW);
		break;
	}
	case CImgPrcs::_MODE_HISTOGRAM_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Teaching Dlg"));
		m_pDlgHistogram->ShowWindow(SW_SHOW);
		break;
	}
	case CImgPrcs::_MODE_BRIGHTNESS_:
	{
		SetDlgItemText(IDC_BTN_SRC_TO_TEACHING_DLG, _T("Src to Teaching Dlg"));
		m_pDlgBrightness->ShowWindow(SW_SHOW);
		break;
	}
	}

	m_Teaching_Tab.SetCurSel(m_iInspMode);
}


void CDlg_ImgPrcs::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_iInspMode = GetInspMode();
		if(m_DlgRect_Src.PtInRect(point))
		{
			point.x = point.x - m_DlgRect_Src.left;
			point.y = point.y - m_DlgRect_Src.top;

			m_ptROI_Start = point;
			m_bClicked = true;
		}
	
}


void CDlg_ImgPrcs::OnLButtonUp(UINT nFlags, CPoint point)
{

		if (m_DlgRect_Src.PtInRect(point))
		{
			point.x = point.x - m_DlgRect_Src.left;
			point.y = point.y - m_DlgRect_Src.top;

			m_bClicked = false;
			m_ptROI_End = point;
			OnDrawROI(m_ViewData_Src);
		}


}


void CDlg_ImgPrcs::OnMouseMove(UINT nFlags, CPoint point)
{

	if (m_DlgRect_Src.PtInRect(point))
	{
		point.x = point.x - m_DlgRect_Src.left;
		point.y = point.y - m_DlgRect_Src.top;

		if (m_bClicked == true)
		{
			m_ptROI_End = point;
			OnDrawROI(m_ViewData_Src);
		}
	}


	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlg_ImgPrcs::OnDestroy()
{
	CDialogEx::OnDestroy();

	ReleaseViewData();

}


void CDlg_ImgPrcs::OnBnClickedBtnCvtGray()
{
	if (m_ViewData_Src.img != NULL)
	{
		cvtColor(*m_ViewData_Src.img, *m_ViewData_Src.img, COLOR_RGB2GRAY);
		DrawViewData(m_ViewData_Src);
	}
}


void CDlg_ImgPrcs::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_DlgRect_Dst.PtInRect(point))
	{
		*m_pMessageImg = m_ViewData_Dst.img->clone();
	}
	else if (m_DlgRect_Src.PtInRect(point))
	{
		*m_pMessageImg = m_ViewData_Src.img->clone();
	}
	m_pDlgExpansionView->RefreshView(*m_pMessageImg);
	m_pDlgExpansionView->ShowWindow(SW_SHOW);

	__super::OnRButtonDown(nFlags, point);
}


void CDlg_ImgPrcs::OnBnClickedBtnCropImg()
{
	cv::Rect rect(m_DlgRect_Src.left, m_DlgRect_Src.right, m_DlgRect_Src.Width(), m_DlgRect_Src.Height());
	Mat temp = *m_ViewData_Src.img;
	*m_ViewData_Src.img = temp(rect);
	DrawViewData(m_ViewData_Src);
}
