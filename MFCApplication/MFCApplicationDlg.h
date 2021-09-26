
// MFCApplicationDlg.h : 헤더 파일
//
#include "Dlg_ImgPrcs.h"

#pragma once
#define SFC_GETSET(ret, fname, var)\
	ret Get##fname()\
	{\
	return var;\
	}\
	void Set##fname(ret tmp)\
	{\
	var = tmp;\
	}

// CMFCApplicationDlg 대화 상자
class CMFCApplicationDlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplicationDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

private:
	CEtc         *m_pEtc;
	CDlg_ImgPrcs *m_pDlg_ImgPrcs;

// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnMenuImgPrcs();
	afx_msg void OnDestroy();

public:
	CDlg_ImgPrcs* getDlg_ImgPrcsPtr();
	CEtc* getEtcPtr();
};
