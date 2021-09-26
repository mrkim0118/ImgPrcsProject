#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DlgItem.h"


// CDlg_Teaching_Threshold 대화 상자입니다.

class CDlg_Teaching_Threshold : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Teaching_Threshold)

public:
	CDlg_Teaching_Threshold(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_Teaching_Threshold();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_THRESHOLD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	unique_ptr<CDlgItem> m_pDlgItem;
	unique_ptr<COpenCV> m_pOpenCV;

	UINT m_iEdit_Adp_BlockSize;
	UINT m_iEdit_Adp_C;
	UINT m_iEdit_Threshold;

	CComboBox m_Cmb_Adp_Method;
	CComboBox m_Cmb_Adp_Type;
	CComboBox m_Cmb_Method;
	CButton m_Chk_Adaptive_Use;
	CSliderCtrl m_Slider_Threshold;

	int m_iEdit_Threshold_Value;

public:

	virtual BOOL OnInitDialog();

	afx_msg void OnDestroy();
	afx_msg void OnEnChangeEditAdaptiveBlockSize();
	afx_msg void OnEnChangeEditAdaptiveC();
	afx_msg void OnEnChangeEditThreshold();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditThresholdVal();
	afx_msg void OnPaint();
	afx_msg LRESULT OnReceiveImg(WPARAM wParam , LPARAM lParam);

	int GetThreshold();
	int GetAdaptiveC();
	int GetAdaptive_BlockSize();
	int GetThresholdMethod();
	int GetAdpThresholdType();
	int GetAdpThresholdMethod();
	int GetAdaptiveUse();

	void UpdateTestImg();
};
