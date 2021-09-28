#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlg_Teaching_Histogram ��ȭ �����Դϴ�.

class CDlg_Teaching_Histogram : public CDialogEx , public CDlgItem
{
	DECLARE_DYNAMIC(CDlg_Teaching_Histogram)

public:
	CDlg_Teaching_Histogram(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlg_Teaching_Histogram();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_HISTOGRAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	unique_ptr<COpenCV> m_pOpenCV;
public:

	UINT m_iEdit_Bin_Num;
	UINT m_iEdit_Value_Bin_Number;
	UINT m_iEdit_Value_Min;
	UINT m_iEdit_Value_Max;

	CButton m_Chk_Equalize;
	CButton m_Chk_Stretch;

	CSliderCtrl m_Slider_Histogram;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedChkEqualize();
	afx_msg void OnBnClickedChkStretch();
	afx_msg void OnEnChangeEditValueBinNumber();
	afx_msg LRESULT OnReceiveImg(WPARAM wParam, LPARAM lParam);

	void UpdateTestImg();
	int GetEqualizeUse();
	int GetStretchUse();
};
