#pragma once
class CDlgItem
{
public:
	CDlgItem();
	virtual ~CDlgItem();

	struct ViewData
	{
		CClientDC *dc;
		CRect rect;
		Mat *img;
		Mat *ScreenImg;
		BITMAPINFO *BitMapInfo;
	};
	CWnd * m_pWnd;
	CWnd * m_pWnd_Ext = NULL;

	ViewData m_ViewData_Src;
	ViewData m_ViewData_Dst;

	void CorrectBitMapWidth(Mat SrcImg, Mat& DstImg);
	void CreateBitMapInfo(ViewData &View);
	void DrawImage(ViewData &View);

	void InitViewData(CWnd* pWnd , CWnd* pWnd_Ext = NULL);
	void ReleaseViewData();
	void DrawViewData(ViewData &View);
	
};

