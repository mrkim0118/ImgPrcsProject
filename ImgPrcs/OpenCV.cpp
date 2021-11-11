
#include "stdafx.h"
#include "OpenCV.h"



int main()
{
	return 0;
}


COpenCV::COpenCV()
{
}


COpenCV::~COpenCV()
{
}

bool COpenCV::CheckImg(Mat img)
{
	bool bRet = TRUE;

	if (img.empty() == TRUE)
	{
		m_strLog.Format(_T("[Img is NULL (OpenCV)]"));
		m_Etc.WriteLog(m_strLog);
		return FALSE;
	}

	return bRet;
}

bool COpenCV::CheckImgPath(String strFilePath)
{
	bool bRet = TRUE;

	if (strFilePath == "")
	{
		m_strLog.Format(_T("[Img Path is Blank (OpenCV)]"), strFilePath);
		m_Etc.WriteLog(m_strLog);
		return FALSE;
	}

	return bRet;
}

bool COpenCV::SaveImg(String strImgPath,InputArray Arrayimg, vector<int> Param)
{
	Mat img = Arrayimg.getMat();
	if (CheckImg(img) == TRUE && CheckImgPath(strImgPath) == TRUE)
	{
		if (imwrite(strImgPath, img, Param) == FALSE)
		{
			m_strLog.Format(_T("[Save Img Fail (OpenCV)] - FilePath : %s"), strImgPath);
			m_Etc.WriteLog(m_strLog);
			return FALSE;
		}
	}
	return TRUE;
}

Mat COpenCV::LoadImg(String strImgPath, ImreadModes eMode)
{
	Mat img;
	if (CheckImgPath(strImgPath) == TRUE)
	{
		img = imread(strImgPath, eMode);
		if (CheckImg(img) != TRUE)
		{
			m_strLog.Format(_T("[Load Img Fail (OpenCV)] - FilePath : %s"), strImgPath);
			m_Etc.WriteLog(m_strLog);
		}
		else
		{
		}
	}
	return img;
}

bool COpenCV::Mask(InputArray SrcImg, Mat& DstImg, InputArray MaskImg)
{
	Mat mSrc = SrcImg.getMat();
	Mat mMask = MaskImg.getMat();

	if (CheckImg(mSrc) == TRUE && CheckImg(mMask) == TRUE)
	{
		mSrc.setTo(Scalar(COLOR_WHITE), mMask);
		DstImg = mSrc.clone();
	}
	return TRUE;
}

bool COpenCV::Histogram(InputArray SrcImg , Mat& DstImg , HistogramParams &tHistogramParams)
{
	Mat mSrc = SrcImg.getMat();
	if (CheckImg(mSrc) == TRUE)
	{
		int iChannelCnt = mSrc.channels();

		int iChannels[] = { 0 };
		int idimensions = 1; // ���Ϸ��� ������׷��� ä�� ����
		const int iHistSize[] = { tHistogramParams.iBinNum }; // bin�� ����
		float fGrayLevel[] = { (float)tHistogramParams.iValueMin , (float)tHistogramParams.iValueMax }; // �����ϰ��� �ִ� �ּҰ�
		const float* fRanges[] = { fGrayLevel };
		Scalar Color = SCALAR_COLOR_BLACK;

		Mat imgHist(tHistogramParams.iValueMax, tHistogramParams.iBinNum, CV_8UC3, SCALAR_COLOR_WHITE);

		DstImg = imgHist.clone();
		if (iChannelCnt == 1)
		{
			Mat GrayHist;

			calcHist(&mSrc, 1, iChannels, noArray(), GrayHist, idimensions, iHistSize, fRanges);
			GetHistogramImg(GrayHist, DstImg , Color , tHistogramParams);
		}
		else
		{
			Mat BGR_Hist[3];
			for (int i = 0; i < iChannelCnt; i++)
			{
				if (i == _SCALAR_B_)
				{
					Color = SCALAR_COLOR_BLUE;
					iChannels[0] = i;
				}
				else if (i == _SCALAR_G_)
				{
					Color = SCALAR_COLOR_GREEN;
					iChannels[0] = i;
				}
				else if (i == _SCALAR_R_)
				{
					Color = SCALAR_COLOR_RED;
					iChannels[0] = i;
				}
				calcHist(&mSrc, 1, iChannels, noArray(), BGR_Hist[i], idimensions, iHistSize, fRanges);
				GetHistogramImg(BGR_Hist[i], DstImg , Color , tHistogramParams);
			}

		}
	}
	return 0;
}

bool COpenCV::Histogram_Strecth(InputArray SrcImg, Mat & DstImg)
{
	Mat mSrc = SrcImg.getMat();
	if (CheckImg(mSrc) == TRUE)
	{
		double dMin, dMax;
		minMaxLoc(mSrc, &dMin, &dMax);

		DstImg = (mSrc - dMin) * 255 / (dMax - dMin);

	}
	return true;
}

bool COpenCV::Histogram_Equalize(InputArray SrcImg, Mat & DstImg)
{
	Mat mSrc = SrcImg.getMat();
	if (CheckImg(mSrc) == TRUE)
	{
		cvtColor(mSrc, mSrc, COLOR_RGB2GRAY);
		equalizeHist(mSrc, DstImg);
	}

	return true;
}

void COpenCV::GetHistogramImg( Mat & SrcImg , Mat &Dst , Scalar Color , HistogramParams &tHistogramParams)
{
	//CV_Assert(Img.type() == CV_32FC1);
	//CV_Assert(SrcImg.size() == Size(1,256));

	double dHistMax;
	minMaxLoc(SrcImg, 0, &dHistMax);

	for (int i = 0; i < tHistogramParams.iBinNum; i++)
	{
		//int iRefVal = cvRound(SrcImg.at<float>(i, 0) * tHistogramParams.iValueMax / dHistMax);
		line(Dst, Point(i, tHistogramParams.iValueMax+1 - cvRound(SrcImg.at<float>(i, 0) * tHistogramParams.iValueMax / dHistMax)), Point(i, tHistogramParams.iValueMax - cvRound(SrcImg.at<float>(i, 0) * tHistogramParams.iValueMax / dHistMax)), Color,2);

	}
}

bool COpenCV::ThresHold(InputArray SrcImg, Mat& DstImg, ThresHoldParams &tThresHoldParams)
{
	Mat mSrc = SrcImg.getMat();
	if (CheckImg(mSrc) == true)
	{
		if (SrcImg.channels() != 1)
		{
			//rgb = bgr[:, : , :: - 1]
			cvtColor(mSrc, mSrc, COLOR_RGB2GRAY);
		}
		threshold(mSrc, DstImg, tThresHoldParams.iThreshold,tThresHoldParams.iMaxValue, tThresHoldParams.eType);
	}
	else
	{
		return false;
	}
	return true;
}


bool COpenCV::ThresHold_Adaptive(InputArray SrcImg, Mat& DstImg, AdaptiveThresHoldParams &tAdaptiveThresHoldParams)
{
	Mat mSrc = SrcImg.getMat();
	if (CheckImg(mSrc) == true)
	{
		if (SrcImg.channels() != 1)
		{
			cvtColor(mSrc, mSrc, COLOR_RGB2GRAY);
		}
		adaptiveThreshold(mSrc, DstImg, tAdaptiveThresHoldParams.iMaxValue, tAdaptiveThresHoldParams.eMethod, tAdaptiveThresHoldParams.eType, tAdaptiveThresHoldParams.iBlockSize, tAdaptiveThresHoldParams.iC);
	}
	else
	{
		return false;
	}
	return true;
}

bool COpenCV::Morphology(InputArray SrcImg, Mat& DstImg, MorphologyParams &tMorPhologyParams, ElementParams &tElementParams)
{
	Mat mSrc = SrcImg.getMat();
	if (CheckImg(mSrc) == true)
	{
		if (SrcImg.channels() != 1)
		{
			cvtColor(mSrc, mSrc, COLOR_RGB2GRAY);
		}

		tMorPhologyParams.Kernel = getStructuringElement(tElementParams.eShape, tElementParams.ksize, tElementParams.anchor);
		morphologyEx(mSrc, DstImg, tMorPhologyParams.eOperation, tMorPhologyParams.Kernel, tMorPhologyParams.Anchor, tMorPhologyParams.iIterator, tMorPhologyParams.eBorderType, tMorPhologyParams.borderValue);
	}
	else
	{
		return false;
	}
	return true;
}

bool COpenCV::TemplateMatching(InputArray SrcImg, Mat & DstImg, TemplateMatchParams tTemplateMatchParams , Mat& Normalize)
{
	Mat mSrc = SrcImg.getMat();
	Mat Result;
	if (CheckImg(mSrc) == true)
	{
		matchTemplate(mSrc, tTemplateMatchParams.Model, Result, tTemplateMatchParams.eTemplateMatchModes);
		normalize(Result, Normalize, 0, 255, NORM_MINMAX, CV_8U);

		double dMax = 0.0;
		Point MaxLoc;

		minMaxLoc(Result, 0 ,&dMax, 0 , &MaxLoc);
		DstImg =  mSrc.clone();
		string strResult = format("Result Max : %.3f", dMax);
		putText(DstImg, strResult, Point((int)(DstImg.cols*0.05), (int)(DstImg.rows*0.05)), FONT_HERSHEY_DUPLEX, 1, SCALAR_COLOR_LIGHT_SKY, 2);
		rectangle(DstImg, Rect(MaxLoc.x, MaxLoc.y, tTemplateMatchParams.Model.cols, tTemplateMatchParams.Model.rows), SCALAR_COLOR_LIGHT_SKY, 2);
	}
	return true;
}

bool COpenCV::Brightness(InputArray SrcImg, Mat & DstImg, BrightnessParams tBrightnessParams)
{
	Mat mSrc = SrcImg.getMat();
	if (CheckImg(mSrc) == true)
	{
		Mat Dst(mSrc.rows, mSrc.cols, mSrc.type());

		for (int j = 0; j < mSrc.rows; j++)
		{
			for (int i = 0; i < mSrc.cols; i++)
			{
				if (mSrc.channels() == 1)
				{
					Dst.at<uchar>(j, i) = saturate_cast<uchar>(mSrc.at<uchar>(j, i) + tBrightnessParams.iBrightness);
				}
				else if (mSrc.channels() == 3)
				{
					Dst.at<uchar>(j, (i * mSrc.channels() + _RGB_R_)) = saturate_cast<uchar>(mSrc.at<uchar>(j, (i * mSrc.channels() + _RGB_R_)) + tBrightnessParams.iBrightness);
					Dst.at<uchar>(j, (i * mSrc.channels() + _RGB_G_)) = saturate_cast<uchar>(mSrc.at<uchar>(j, (i * mSrc.channels() + _RGB_G_)) + tBrightnessParams.iBrightness);
					Dst.at<uchar>(j, (i * mSrc.channels() + _RGB_B_)) = saturate_cast<uchar>(mSrc.at<uchar>(j, (i * mSrc.channels() + _RGB_B_)) + tBrightnessParams.iBrightness);
				}
			}	
		} 
		Contrast(Dst, DstImg, tBrightnessParams.fContrast);

		/*DstImg = Dst.clone();*/
	}
	return true;
}

bool COpenCV::Contrast(InputArray SrcImg, Mat & DstImg, float fValue)
{
	Mat mSrc = SrcImg.getMat();

	if (CheckImg(mSrc) == true)
	{
		DstImg = mSrc + ((mSrc - GRAYSCALE_HALF) * fValue);
	}
	return true;
}

int COpenCV::Labeling(InputArray SrcImg, Mat &DstImg, LabelingParams &tLabelingParams)
{
	int iLabelCount = 0;
	Mat mSrc = SrcImg.getMat();
	if (CheckImg(mSrc) == true)
	{
		iLabelCount = connectedComponentsWithStats(SrcImg, tLabelingParams.labels, tLabelingParams.stats, tLabelingParams.centroids);
	}
	if (tLabelingParams.bDrawRect == true)
	{
		Mat Dst;
		cvtColor(SrcImg, Dst, COLOR_GRAY2BGR);
		for (int i = 0; i < iLabelCount; i++)
		{
			int *p = tLabelingParams.stats.ptr<int>(i);

			rectangle(Dst, Rect(p[0], p[1], p[2], p[3]) , tLabelingParams.RectColor, 2);
		}
		DstImg = Dst.clone();
	}
	else
	{
		DstImg = SrcImg.getMat();
	}

	return iLabelCount;
}



ContoursType COpenCV::Contour(InputArray SrcImg, Mat& DstImg, ContourParams tContourParams)
{	
	vector<vector<Point>> Contours;
	Mat mSrc = SrcImg.getMat();
	if (CheckImg(mSrc) == true)
	{
		findContours(SrcImg, Contours, tContourParams.eRetrievalMode, tContourParams.eContourApproximationMode);
	}
	cvtColor(SrcImg, DstImg, COLOR_GRAY2BGR);

	if (tContourParams.bDrawLine == true)
	{
		for (int i = 0; i < Contours.size(); i++)
		{
			Scalar c(rand() & 255, rand() & 255, rand() & 255);
			drawContours(DstImg, Contours, i, c, 2);
		}
	}

	return Contours;
}


void COpenCV::CreateROIImg(Mat SrcImg, Mat& DstImg, CPoint ptStart, CPoint ptEnd, CRect rect)
{
	int iPoint_Left, iPoint_Right, iPoint_Top, iPoint_Bottom;

	if (ptStart.x > ptEnd.x)
	{
		iPoint_Left = ptEnd.x;
		iPoint_Right = ptStart.x;
	}
	else
	{
		iPoint_Left = ptStart.x;
		iPoint_Right = ptEnd.x;
	}

	if (ptStart.y > ptEnd.y)
	{
		iPoint_Top = ptEnd.y;
		iPoint_Bottom = ptStart.y;
	}
	else
	{
		iPoint_Top = ptStart.y;
		iPoint_Bottom = ptEnd.y;
	}

	double rx = ((double)iPoint_Left / rect.right);
	double rx2 = ((double)iPoint_Right / rect.right);

	double ry = ((double)iPoint_Top / rect.bottom);
	double ry2 = ((double)iPoint_Bottom / rect.bottom);

	int iStartX = (int)(SrcImg.cols*rx);
	int IStartY = (int)(SrcImg.rows*ry);

	int iWidth = (int)(SrcImg.cols*rx2 - SrcImg.cols*rx);
	int iHeight = (int)(SrcImg.rows*ry2 - SrcImg.rows*ry);

	Rect ROI(iStartX, IStartY, iWidth, iHeight);
	DstImg = SrcImg(ROI);
}