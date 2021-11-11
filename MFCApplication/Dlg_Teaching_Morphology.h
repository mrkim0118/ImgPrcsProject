#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "Dlg_Expansion_View.h"

// CDlg_Teaching_Morphology 대화 상자입니다.

class CDlg_Teaching_Morphology : public CDialogEx , public CDlgItem
{
	DECLARE_DYNAMIC(CDlg_Teaching_Morphology)

private:
	CDlg_Teaching_Morphology(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_Teaching_Morphology();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MORPHOLOGY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:

	static CDlg_Teaching_Morphology* m_pDlgMorphology;
	unique_ptr<COpenCV> m_pOpenCV;
	unique_ptr<CDlg_Expansion_View> m_pDlgExpansionView;

	CComboBox m_Cmb_Morph_Operation;
	CComboBox m_Cmb_Element_Shape;
	CSliderCtrl m_Slider_Morph_Size;

	CRect m_DlgRect_Dst;

	int m_iEdit_Morph_AnchorX;
	int m_iEdit_Morph_AnchorY;
	int m_iEdit_Morph_Size_Value;
	int m_iEdit_Element_AnchorX;
	int m_iEdit_Element_AnchorY;

	UINT m_iEdit_Element_Size;
public:
	static CDlg_Teaching_Morphology* GetInstance();
	static void DestroyInstance();
public:

	virtual BOOL OnInitDialog();
	
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeEditElementAnchorX();
	afx_msg void OnEnChangeEditElementAnchorY();
	afx_msg void OnEnChangeEditMorphologyAnchorX();
	afx_msg void OnEnChangeEditMorphologyAnchorY();
	afx_msg void OnEnChangeEditElementSize();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditMorphSizeValue();
	afx_msg LRESULT OnReceiveImg(WPARAM wParam, LPARAM lParam);

	int GetMorphAnchorX();
	int GetMorphAnchorY();
	int GetElemetSIze();
	int GetElementAnchorX();
	int GetElementAnchorY();
	int GetElementShape();
	int GetMorphologyOperation();
	void UpdateTestImg();

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
