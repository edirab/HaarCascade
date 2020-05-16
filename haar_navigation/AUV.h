#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "functions.h"

using namespace cv;
using namespace std;

class AUV {

	double detection_coeff; // px * coeff = cm
	CascadeClassifier marker_type_1, marker_type_2;

	String marker_1_path;
	String marker_2_path;

	int marker1Counter = 0, marker2Counter = 0;
	int marker1Counter_prev = 0, marker2Counter_prev = 0;

public:
	double delta_x;
	double delta_y;
	double delta_z;

	double d_yaw;
	double d_pitch;
	double d_roll;

	AUV();
	~AUV ();

	void get_orientation();
	void rotate_over_normal(Mat& frame, vector<Rect> m1, vector<Rect> m2);
	void detect_and_display(Mat frame, int cascadeNum, bool saveFalsePositive = false);
};

