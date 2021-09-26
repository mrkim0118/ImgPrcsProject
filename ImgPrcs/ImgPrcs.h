#pragma once

#include "opencv2/opencv.hpp"

class CImgPrcs
{
public:
	CImgPrcs();
	~CImgPrcs();

#define COLOR_WHITE RGB(255,255,255)
#define COLOR_BLACK RGB(0,0,0)
#define COLOR_YELLOW RGB(0,255,255)
#define COLOR_RED RGB(255,0,0)

	enum ImgPrcsMode
	{
		_MODE_THRESHOLD_ = 0,
		_MODE_MORPHOLOGY_,
		_MODE_TEMPLATE_MATCH_,
		_MODE_HISTOGRAM_,
		_MODE_BRIGHTNESS_,
		_MODE_MASK_,
		_MODE_LABELING_,
		_MODE_CONTOUR_,
		_MODE_COUNT_MAX
	};
};

