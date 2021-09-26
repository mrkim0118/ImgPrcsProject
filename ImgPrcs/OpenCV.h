#pragma once

#include "ImgPrcs.h"
#include "OpenCV.h"
#include "../MFC/Etc.h"

using namespace cv;
using namespace std;
using namespace cv::ml;

typedef vector<vector<Point>> ContoursType;

#define SCALAR_COLOR_WHITE Scalar(255,255,255)
#define SCALAR_COLOR_BLACK Scalar(0,0,0)

#define SCALAR_COLOR_RED Scalar(0,0,255)
#define SCALAR_COLOR_GREEN Scalar(255,0,0)
#define SCALAR_COLOR_BLUE Scalar(0,255,0)

#define SCALAR_COLOR_YELLOW Scalar(0,255,255)
#define SCALAR_COLOR_LIGHT_SKY Scalar(234,255,18)

class COpenCV
{
public:
	COpenCV();
	~COpenCV();
private:
	CString m_strLog;
	CEtc  m_Etc;

public:
	enum ColorOrder
	{
		_COLOR_G_ = 0,
		_COLOR_B_,
		_COLOR_R_,
		_COLOR_COUNT_MAX
	};
	struct ThresHoldParams
	{
		int iThreshold = 128;
		int iMaxValue = 255;
		ThresholdTypes eType = THRESH_BINARY;
	};

	struct AdaptiveThresHoldParams
	{
		int iBlockSize = 3;
		int iC = 5;
		int iMaxValue = 255;
		AdaptiveThresholdTypes eMethod = ADAPTIVE_THRESH_MEAN_C;
		ThresholdTypes eType = THRESH_BINARY;
	};

	struct MorphologyParams
	{
		MorphTypes eOperation;
		Mat Kernel = Mat();
		Point Anchor = Point(-1, -1);
		int iIterator = 1;
		BorderTypes eBorderType = BORDER_CONSTANT;
		const Scalar& borderValue = morphologyDefaultBorderValue();
	};

	struct ElementParams
	{
		MorphShapes eShape = MORPH_RECT;
		Size ksize = Size(3, 3);
		Point anchor = Point(-1, -1);
	};
	
	struct LabelingParams
	{
		Mat labels;
		Mat stats;
		Mat centroids;
		bool bDrawRect = false;
		Scalar RectColor = SCALAR_COLOR_RED;
		int iConnectivity = 8;
		int iType = CV_32S;
	};

	struct ContourParams
	{
		bool bDrawLine = false;
		RetrievalModes eRetrievalMode = RETR_LIST;
		ContourApproximationModes eContourApproximationMode = CHAIN_APPROX_NONE;
	};

	struct TemplateMatchParams
	{
		Mat Model;
		Mat Normalize;
		TemplateMatchModes eTemplateMatchModes = TM_CCOEFF_NORMED;
	};

	struct HistogramParams
	{
		int iBinNum = 256;
		int iValueMin = 0;
		int iValueMax = 256;
	};
	
	struct BrightnessParams
	{
		int iBrightness = 0;
		float fContrast = 0;
	};
public:

	Mat LoadImg(String strImgPath, ImreadModes eMode = IMREAD_COLOR);
	
	int Labeling(InputArray SrcImg, Mat& DstImg, LabelingParams &tLabelingParams);

	bool CheckImg(Mat img);
	bool CheckImgPath(String strFilePath);
	bool SaveImg(String strImgPath, InputArray Arrayimg, vector<int> Param = {0});
	bool Mask(InputArray SrcImg, Mat& DstImg, InputArray MaskImg);
	bool Histogram(InputArray SrcImg, Mat& DstImg, HistogramParams &tHistogramParams);
	bool ThresHold_Adaptive(InputArray SrcImg , Mat& DstImg, AdaptiveThresHoldParams &tAdaptiveThresHoldParams);
	bool ThresHold(InputArray SrcImg, Mat& DstImg, ThresHoldParams &ThresHoldParams);
	bool Morphology(InputArray SrcImg , Mat& DstImg, MorphologyParams &tMorPhologyParams, ElementParams &tElementParams);
	bool TemplateMatching(InputArray SrcImg, Mat & DstImg, TemplateMatchParams tTemplateMatchParams , Mat& Normalize);
	bool Brightness(InputArray SrcImg, Mat& DstImg, BrightnessParams tBrightnessParams);
	bool Contrast(InputArray SrcImg, Mat& DstImg, float fValue);
	void GetHistogramImg(Mat & Img, Mat &imgHist , Scalar Color , HistogramParams &tHistogramParams);

	ContoursType Contour(InputArray SrcImg, Mat& DstImg, ContourParams tContourParams);

};

