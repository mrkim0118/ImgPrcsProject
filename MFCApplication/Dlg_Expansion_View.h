#pragma once


// CDlg_Expansion_View 대화 상자입니다.

class CDlg_Expansion_View : public CDialogEx , public CDlgItem
{
	DECLARE_DYNAMIC(CDlg_Expansion_View)

public:
	CDlg_Expansion_View(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlg_Expansion_View();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EXPANSION_VIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnReceiveImg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	void RefreshView(Mat img);
};
