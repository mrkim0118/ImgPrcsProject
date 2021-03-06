#pragma once
#include "afxwin.h"
#include "Dlg_Expansion_View.h"

// CDlg_Teaching_Template_Match 대화 상자입니다.

class CDlg_Teaching_Template_Match : public CDialogEx , public CDlgItem
{
	DECLARE_DYNAMIC(CDlg_Teaching_Template_Match)

public:
	CDlg_Teaching_Template_Match(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_Teaching_Template_Match();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_TEMPLATE_MATCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	static CDlg_Teaching_Template_Match* m_pDlgTemplateMatch;
	Mat* m_pModelImg;
	CComboBox m_Cmb_Method;
	CRect m_DlgRect_Dst;
	unique_ptr<CDlg_Expansion_View> m_pDlgExpansionView;

public:

	static CDlg_Teaching_Template_Match* GetInstance();
	static void DestroyInstance();

	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnReceiveImg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveNorm(WPARAM wParam, LPARAM lParam);

	int GetTemplateMatchMethod();
	Mat GetModelImg();

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
