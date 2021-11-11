#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Dlg_Expansion_View.h"

// CDlg_Teaching_Histogram 대화 상자입니다.

class CDlg_Teaching_Histogram : public CDialogEx , public CDlgItem
{
	DECLARE_DYNAMIC(CDlg_Teaching_Histogram)

private:
	CDlg_Teaching_Histogram(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_Teaching_Histogram();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	static CDlg_Teaching_Histogram* m_pDlgHistogram;

	unique_ptr<COpenCV> m_pOpenCV;
	unique_ptr<CDlg_Expansion_View> m_pDlgExpansionView;

	UINT m_iEdit_Bin_Num;
	UINT m_iEdit_Value_Bin_Number;
	UINT m_iEdit_Value_Min;
	UINT m_iEdit_Value_Max;

	CButton m_Chk_Equalize;
	CButton m_Chk_Stretch;

	CSliderCtrl m_Slider_Histogram;

	CRect m_DlgRect_Dst;


public:

	static CDlg_Teaching_Histogram* GetInstance();
	static void DestroyInstance();

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedChkEqualize();
	afx_msg void OnBnClickedChkStretch();
	afx_msg void OnEnChangeEditValueBinNumber();
	afx_msg LRESULT OnReceiveImg(WPARAM wParam, LPARAM lParam);

	void UpdateTestImg();
	int GetEqualizeUse();
	int GetStretchUse();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
