// Dlg_Teaching_Morphology.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "Dlg_Teaching_Morphology.h"
#include "afxdialogex.h"


// CDlg_Teaching_Morphology 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlg_Teaching_Morphology, CDialogEx)
CDlg_Teaching_Morphology* CDlg_Teaching_Morphology::m_pDlgMorphology = NULL;


CDlg_Teaching_Morphology::CDlg_Teaching_Morphology(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_MORPHOLOGY, pParent)
	, m_iEdit_Element_AnchorX(-1)
	, m_iEdit_Element_AnchorY(-1)
	, m_iEdit_Morph_AnchorX(-1)
	, m_iEdit_Morph_AnchorY(-1)
	, m_iEdit_Element_Size(1)
	, m_iEdit_Morph_Size_Value(0)
{
	m_pOpenCV = make_unique<COpenCV>();
}

CDlg_Teaching_Morphology::~CDlg_Teaching_Morphology()
{
}

void CDlg_Teaching_Morphology::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CMB_ELEMENT_SHAPE, m_Cmb_Element_Shape);
	DDX_Control(pDX, IDC_CMB_MORPHOLOGY_OPERATION, m_Cmb_Morph_Operation);
	DDX_Control(pDX, IDC_SLIDER_MORPH_SIZE, m_Slider_Morph_Size);

	DDX_Text(pDX, IDC_EDIT_ELEMENT_ANCHOR_X, m_iEdit_Element_AnchorX);
	DDX_Text(pDX, IDC_EDIT_ELEMENT_ANCHOR_Y, m_iEdit_Element_AnchorY);
	DDX_Text(pDX, IDC_EDIT_MORPHOLOGY_ANCHOR_X, m_iEdit_Morph_AnchorX);
	DDX_Text(pDX, IDC_EDIT_MORPHOLOGY_ANCHOR_Y, m_iEdit_Morph_AnchorY);
	DDX_Text(pDX, IDC_EDIT_ELEMENT_SIZE, m_iEdit_Element_Size);
	DDX_Text(pDX, IDC_EDIT_MORPH_SIZE_VALUE, m_iEdit_Morph_Size_Value);
}


BEGIN_MESSAGE_MAP(CDlg_Teaching_Morphology, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_ELEMENT_ANCHOR_X, &CDlg_Teaching_Morphology::OnEnChangeEditElementAnchorX)
	ON_EN_CHANGE(IDC_EDIT_ELEMENT_ANCHOR_Y, &CDlg_Teaching_Morphology::OnEnChangeEditElementAnchorY)
	ON_EN_CHANGE(IDC_EDIT_MORPHOLOGY_ANCHOR_X, &CDlg_Teaching_Morphology::OnEnChangeEditMorphologyAnchorX)
	ON_EN_CHANGE(IDC_EDIT_MORPHOLOGY_ANCHOR_Y, &CDlg_Teaching_Morphology::OnEnChangeEditMorphologyAnchorY)
	ON_EN_CHANGE(IDC_EDIT_ELEMENT_SIZE, &CDlg_Teaching_Morphology::OnEnChangeEditElementSize)
	ON_EN_CHANGE(IDC_EDIT_MORPH_SIZE_VALUE, &CDlg_Teaching_Morphology::OnEnChangeEditMorphSizeValue)
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_MORPHOLOGY_TEST, OnReceiveImg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CDlg_Teaching_Morphology 메시지 처리기입니다.

CDlg_Teaching_Morphology * CDlg_Teaching_Morphology::GetInstance()
{
	if (m_pDlgMorphology == NULL)
	{
		m_pDlgMorphology = new CDlg_Teaching_Morphology;
	}
	return m_pDlgMorphology;
}

void CDlg_Teaching_Morphology::DestroyInstance()
{
	if (m_pDlgMorphology != NULL)
	{
		delete m_pDlgMorphology;
		m_pDlgMorphology = NULL;
	}
}

BOOL CDlg_Teaching_Morphology::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pWnd = GetDlgItem((IDC_STATIC_MORPHOLOGY_TEST));
	InitViewData(m_pWnd);

	m_pDlgExpansionView = make_unique<CDlg_Expansion_View>();
	m_pDlgExpansionView->Create(IDD_DLG_EXPANSION_VIEW, this);
	m_pDlgExpansionView->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_STATIC_MORPHOLOGY_TEST)->GetWindowRect(&m_DlgRect_Dst);

	m_Cmb_Element_Shape.AddString(_T("MORPH_RECT"));
	m_Cmb_Element_Shape.AddString(_T("MORPH_CROSS"));
	m_Cmb_Element_Shape.AddString(_T("MORPH_ELIPSE"));
	m_Cmb_Element_Shape.SetCurSel(0);

	m_Cmb_Morph_Operation.AddString(_T("MORPH_ERODE"));
	m_Cmb_Morph_Operation.AddString(_T("MORPH_DILATE"));
	m_Cmb_Morph_Operation.AddString(_T("MORPH_OPEN"));
	m_Cmb_Morph_Operation.AddString(_T("MORPH_CLOSE"));
	m_Cmb_Morph_Operation.AddString(_T("MORPH_GREDIENT"));
	m_Cmb_Morph_Operation.SetCurSel(0);

	m_Slider_Morph_Size.SetRange(1, 20);
	m_Slider_Morph_Size.SetPos(m_iEdit_Element_Size);
	m_Slider_Morph_Size.SetLineSize(1);
	m_Slider_Morph_Size.SetPageSize(1);

	m_iEdit_Morph_Size_Value = m_Slider_Morph_Size.GetPos();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlg_Teaching_Morphology::OnEnChangeEditElementAnchorX()
{
	UpdateData(TRUE);

	if (m_iEdit_Element_AnchorX < -1)
		m_iEdit_Element_AnchorX = -1;

	UpdateData(FALSE);
}


void CDlg_Teaching_Morphology::OnEnChangeEditElementAnchorY()
{
	UpdateData(TRUE);

	if (m_iEdit_Element_AnchorY < -1)
		m_iEdit_Element_AnchorY = -1;

	UpdateData(FALSE);
}


void CDlg_Teaching_Morphology::OnEnChangeEditMorphologyAnchorX()
{
	UpdateData(TRUE);

	if (m_iEdit_Morph_AnchorX< -1)
		m_iEdit_Morph_AnchorX = -1;

	UpdateData(FALSE);
}


void CDlg_Teaching_Morphology::OnEnChangeEditMorphologyAnchorY()
{
	UpdateData(TRUE);

	if (m_iEdit_Morph_AnchorY < -1)
		m_iEdit_Morph_AnchorY = -1;

	UpdateData(FALSE);
}
int CDlg_Teaching_Morphology::GetElementShape()
{
	CString strMode = _T("");
	int iShape = 0;
	m_Cmb_Element_Shape .GetLBText(m_Cmb_Element_Shape.GetCurSel(), strMode);

	if (strMode == _T("MORPH_RECT"))
		iShape = MorphShapes::MORPH_RECT;
	else if (strMode == _T("MORPH_CROSS"))
		iShape = MorphShapes::MORPH_CROSS;
	else if (strMode == _T("MORPH_ELLIPSE"))
		iShape = MorphShapes::MORPH_ELLIPSE;


	return iShape;
}

int CDlg_Teaching_Morphology::GetMorphologyOperation()
{
	CString strMode = _T("");
	int iType = 0;
	m_Cmb_Morph_Operation.GetLBText(m_Cmb_Morph_Operation.GetCurSel(), strMode);

	if (strMode == _T("MORPH_ERODE"))
		iType = MorphTypes::MORPH_ERODE;
	else if (strMode == _T("MORPH_DILATE"))
		iType = MorphTypes::MORPH_DILATE;
	else if (strMode == _T("MORPH_OPEN"))
		iType = MorphTypes::MORPH_OPEN;
	else if (strMode == _T("MORPH_CLOSE"))
		iType = MorphTypes::MORPH_CLOSE;
	else if (strMode == _T("MORPH_GRADIENT"))
		iType = MorphTypes::MORPH_GRADIENT;

	return iType;
}

void CDlg_Teaching_Morphology::OnEnChangeEditElementSize()
{
	UpdateData(TRUE);

	if (m_iEdit_Element_Size< 1)
		m_iEdit_Element_Size = 1;

	UpdateData(FALSE);
}




void CDlg_Teaching_Morphology::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (IDC_SLIDER_MORPH_SIZE == pScrollBar->GetDlgCtrlID())
	{
		UpdateData(TRUE);

		m_iEdit_Morph_Size_Value = m_Slider_Morph_Size.GetPos();

		if (m_ViewData_Src.img->empty() != TRUE)
		{
			UpdateTestImg();
		}

		UpdateData(FALSE);
		m_pDlgExpansionView->RefreshView(*m_ViewData_Dst.img);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlg_Teaching_Morphology::OnEnChangeEditMorphSizeValue()
{
	int iMin = m_Slider_Morph_Size.GetRangeMin();
	int iMax = m_Slider_Morph_Size.GetRangeMax();

	UpdateData(TRUE);

	if (m_iEdit_Morph_Size_Value < iMin)
		m_iEdit_Morph_Size_Value = iMin;

	else if (m_iEdit_Morph_Size_Value > iMax)
		m_iEdit_Morph_Size_Value = iMax;

	m_Slider_Morph_Size.SetPos(m_iEdit_Morph_Size_Value);

	if (m_ViewData_Src.img != NULL)
	{
		UpdateTestImg();
	}

	UpdateData(FALSE);
}

void CDlg_Teaching_Morphology::UpdateTestImg()
{
	COpenCV::ElementParams tElementParams;
	tElementParams.eShape = (MorphShapes)GetElementShape();
	tElementParams.anchor = Point(m_iEdit_Element_AnchorX, m_iEdit_Element_AnchorY);
	tElementParams.ksize = Size(m_iEdit_Morph_Size_Value, m_iEdit_Morph_Size_Value);

	COpenCV::MorphologyParams tMorphologyParams;
	tMorphologyParams.eOperation = (MorphTypes)GetMorphologyOperation();
	tMorphologyParams.Anchor = Point(m_iEdit_Morph_AnchorX, m_iEdit_Morph_AnchorY);
	tMorphologyParams.Kernel = getStructuringElement(tElementParams.eShape, tElementParams.ksize, tElementParams.anchor);

	m_pOpenCV->Morphology(*m_ViewData_Src.img, *m_ViewData_Dst.img, tMorphologyParams, tElementParams);

	DrawViewData(m_ViewData_Dst);
}

LRESULT CDlg_Teaching_Morphology::OnReceiveImg(WPARAM wParam, LPARAM lParam)
{
	Mat* Tmp = (Mat*)lParam;
	*m_ViewData_Src.img = Tmp->clone();
	*m_ViewData_Dst.img = Tmp->clone();
	DrawViewData(m_ViewData_Src);

	return 0;
}

int CDlg_Teaching_Morphology::GetMorphAnchorX()
{
	UpdateData(TRUE);
	return m_iEdit_Morph_AnchorX;
	UpdateData(FALSE);
}

int CDlg_Teaching_Morphology::GetMorphAnchorY()
{
	UpdateData(TRUE);
	return m_iEdit_Morph_AnchorY;
	UpdateData(FALSE);
}

int CDlg_Teaching_Morphology::GetElemetSIze()
{
	UpdateData(TRUE);
	return m_iEdit_Element_Size;
	UpdateData(FALSE);
}

int CDlg_Teaching_Morphology::GetElementAnchorX()
{
	UpdateData(TRUE);
	return m_iEdit_Element_AnchorX;
	UpdateData(FALSE);
}

int CDlg_Teaching_Morphology::GetElementAnchorY()
{
	UpdateData(TRUE);
	return m_iEdit_Element_AnchorY;
	UpdateData(FALSE);
}


void CDlg_Teaching_Morphology::OnPaint()
{
	CPaintDC dc(this);
	DrawImage(m_ViewData_Dst);
}


void CDlg_Teaching_Morphology::OnDestroy()
{
	CDialogEx::OnDestroy();

	ReleaseViewData();
}



void CDlg_Teaching_Morphology::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_DlgRect_Dst.PtInRect(point))
	{
		*m_pMessageImg = m_ViewData_Dst.img->clone();
		m_pDlgExpansionView->RefreshView(*m_pMessageImg);
		m_pDlgExpansionView->ShowWindow(SW_SHOW);
	}


	__super::OnRButtonDown(nFlags, point);
}
