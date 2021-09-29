#pragma once
#include "afxcmn.h"
#include "Dlg_Expansion_View.h"

// CDlg_Teaching_Brightness ��ȭ �����Դϴ�.

class CDlg_Teaching_Brightness : public CDialogEx , public CDlgItem
{
	DECLARE_DYNAMIC(CDlg_Teaching_Brightness)

public:
	CDlg_Teaching_Brightness(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlg_Teaching_Brightness();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_BRIGHTNESS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	unique_ptr<COpenCV>m_pOpenCV;
	unique_ptr<CDlg_Expansion_View> m_pDlgExpansionView;

	Mat* m_pModelImg;

	int m_iEdit_Brightness;
	float m_fEdit_Contrast;
	int m_iEdit_Value_Brightness;
	float m_fEdit_Value_Contrast;

	CSliderCtrl m_Slider_Brightness;
	CSliderCtrl m_Slider_Contrast;
	CRect m_DlgRect_Dst;

public:
	void UpdateTestImg(CScrollBar* pScrollBar);
	virtual BOOL OnInitDialog();

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditBrightnessVal();
	afx_msg void OnEnChangeEditContrastVal();
	afx_msg LRESULT OnReceiveImg(WPARAM wParam, LPARAM lParam);


	int GetBrightness();
	float GetContrast();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
