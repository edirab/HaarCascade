#pragma once

#include "opencv2/imgproc.hpp"
#include <opencv2/core.hpp>

using namespace cv;


#define RED Scalar(0, 0, 255)
#define BLU Scalar(255, 0, 0)
#define GRN Scalar(0, 255, 0)
#define PNK Scalar(255, 0, 255)
#define YEL Scalar(0, 255, 255)
#define CYA Scalar(255, 255, 0)
#define WHT Scalar(255, 255, 255)
#define BLK Scalar(0, 0, 0)

enum class markerType {

	black_circle = 1,
	white_circle
};

class Marker {

public:
	// координаты центра окружности
	int x;
	int y;

	markerType t;
	bool isValid = true;

	//int roi_x, roi_y, roi_w, roi_h;
	Rect roi;

	Marker();
	Marker(int _x, int _y, enum markerType _t, Rect roi_rect);

	static Mat get_template_t1(int w, int h);
	static Mat get_template_t2(int w, int h);
};