#pragma once
#include "afxwin.h"


// CDlg_Teaching_Template_Match 대화 상자입니다.

class CDlg_Teaching_Template_Match : public CDialogEx
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
	unique_ptr<CDlgItem> m_pDlgItem;
	Mat* m_pModelImg;
	CComboBox m_Cmb_Method;

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnReceiveImg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceiveNorm(WPARAM wParam, LPARAM lParam);

	int GetTemplateMatchMethod();
	Mat GetModelImg();

	void CreateModelImg(Mat SrcImg , Mat& DstImg, CPoint ptStart , CPoint ptEnd , CRect rect);

};
