#include "stdafx.h"
#include "DlgItem.h"


CDlgItem::CDlgItem()
{
}


CDlgItem::~CDlgItem()
{
}


void CDlgItem::CreateBitMapInfo(ViewData &ViewData)
{
	int bpp = ViewData.img->channels() * 8;
	if (ViewData.BitMapInfo != NULL)
	{
		delete ViewData.BitMapInfo;
		ViewData.BitMapInfo = NULL;
	}

	if (bpp == 8)
		ViewData.BitMapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD)];
	else // 24 or 32bit
		ViewData.BitMapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO)];

	ViewData.BitMapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	ViewData.BitMapInfo->bmiHeader.biPlanes = 1;
	ViewData.BitMapInfo->bmiHeader.biBitCount = bpp;
	ViewData.BitMapInfo->bmiHeader.biCompression = BI_RGB;
	ViewData.BitMapInfo->bmiHeader.biSizeImage = 0;
	ViewData.BitMapInfo->bmiHeader.biXPelsPerMeter = 0;
	ViewData.BitMapInfo->bmiHeader.biYPelsPerMeter = 0;
	ViewData.BitMapInfo->bmiHeader.biClrUsed = 0;
	ViewData.BitMapInfo->bmiHeader.biClrImportant = 0;

	if (bpp == 8)
	{
		for (int i = 0; i < 256; i++)
		{
			ViewData.BitMapInfo->bmiColors[i].rgbBlue = (BYTE)i;
			ViewData.BitMapInfo->bmiColors[i].rgbGreen = (BYTE)i;
			ViewData.BitMapInfo->bmiColors[i].rgbRed = (BYTE)i;
			ViewData.BitMapInfo->bmiColors[i].rgbReserved = 0;
		}
	}

	ViewData.BitMapInfo->bmiHeader.biWidth = ViewData.ScreenImg->cols;
	ViewData.BitMapInfo->bmiHeader.biHeight = -ViewData.ScreenImg->rows;
}

void CDlgItem::DrawImage(ViewData &View)
{
	SetStretchBltMode(View.dc->GetSafeHdc(), COLORONCOLOR);
	StretchDIBits(View.dc->GetSafeHdc(), 0, 0, View.rect.Width(), View.rect.Height(), 0, 0, View.ScreenImg->cols, View.ScreenImg->rows, View.ScreenImg->data, View.BitMapInfo , DIB_RGB_COLORS, SRCCOPY);
}

void CDlgItem::CorrectBitMapWidth(Mat SrcImg , Mat& DstImg)
{
	int iModelSpanWidth = SrcImg.cols;
	if (iModelSpanWidth % 4 != 0)
	{
		iModelSpanWidth = SrcImg.cols + (4 - SrcImg.cols % 4);
	}
	else
	{
		iModelSpanWidth = SrcImg.cols;
	}

	Mat NewImg(SrcImg.rows, iModelSpanWidth, SrcImg.type());

	for (int j = 0; j < NewImg.rows; j++)
	{
		uchar *p = NewImg.ptr<uchar>(j);
		for (int i = 0; i < iModelSpanWidth; i++)
		{
			if(SrcImg.channels() == 1)
			{
				if (i >= SrcImg.cols)
				{
					p[i] = 0;
				}
				else
				{
					p[i] = SrcImg.at<uchar>(j, i);
				}
			}
			else
			{
				if (i >= SrcImg.cols)
				{
					p[i * 3 + 0] = 0;
					p[i * 3 + 1] = 0;
					p[i * 3 + 2] = 0;
				}
				else
				{
					p[i * 3 + 0] = SrcImg.at<uchar>(j, (i * 3 + 0));
					p[i * 3 + 1] = SrcImg.at<uchar>(j, (i * 3 + 1));
					p[i * 3 + 2] = SrcImg.at<uchar>(j, (i * 3 + 2));
				}
			}
		}
	}
	DstImg = NewImg.clone();
}

void CDlgItem::InitViewData(CWnd* pWnd, CWnd* pWnd_Ext /*= NULL*/)
{
	m_ViewData_Src.BitMapInfo = NULL;
	m_ViewData_Dst.BitMapInfo = NULL;
	m_ViewData_Src.img = new Mat;
	m_ViewData_Dst.img = new Mat;
	m_ViewData_Src.ScreenImg = new Mat;
	m_ViewData_Dst.ScreenImg = new Mat;
	m_ViewData_Src.dc = new CClientDC(pWnd);

	m_pMessageImg = new Mat;

	pWnd->GetClientRect(&m_ViewData_Src.rect);

	if (pWnd_Ext == NULL)
	{
		m_ViewData_Dst.dc = new CClientDC(pWnd);
		pWnd->GetClientRect(&m_ViewData_Dst.rect);
	}
	else
	{
		m_ViewData_Dst.dc = new CClientDC(pWnd_Ext);
		pWnd_Ext->GetClientRect(&m_ViewData_Dst.rect);
	}

	CreateBitMapInfo(m_ViewData_Dst);
	CreateBitMapInfo(m_ViewData_Src);
}

void CDlgItem::ReleaseViewData()
{
	delete m_ViewData_Src.BitMapInfo;
	delete m_ViewData_Dst.BitMapInfo;
	delete m_ViewData_Src.ScreenImg;
	delete m_ViewData_Dst.ScreenImg;
	delete m_ViewData_Src.dc;
	delete m_ViewData_Dst.dc;

	if (m_ViewData_Src.img != NULL)
	{
		delete m_ViewData_Src.img;
		m_ViewData_Src.img = NULL;
	}
	if (m_ViewData_Dst.img != NULL)
	{
		delete m_ViewData_Dst.img;
		m_ViewData_Dst.img = NULL;
	}
	if (m_pMessageImg != NULL)
	{
		delete m_pMessageImg;
		m_pMessageImg = NULL;
	}
}

void CDlgItem::DrawViewData(ViewData & View)
{
	CorrectBitMapWidth(*View.img, *View.ScreenImg);
	CreateBitMapInfo(View);
	DrawImage(View);
}

void CDlgItem::UpdateTestImg()
{
}

