#pragma once


// CDlg_Expansion_View ��ȭ �����Դϴ�.

class CDlg_Expansion_View : public CDialogEx , public CDlgItem
{
	DECLARE_DYNAMIC(CDlg_Expansion_View)

public:
	CDlg_Expansion_View(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlg_Expansion_View();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EXPANSION_VIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnReceiveImg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	void RefreshView(Mat img);
};
