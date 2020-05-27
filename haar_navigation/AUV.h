#pragma once

#include <Eigen/LU>
#include <fstream>

#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/cudaobjdetect.hpp>

#include <opencv2/aruco.hpp>

#include "Marker.h"
#include "functions.h"


using namespace cv;
using namespace aruco;
using namespace std;

class AUV {

	double detection_coeff; // px * coeff = cm
	CascadeClassifier marker_type_1, marker_type_2;

	String marker_1_path;
	String marker_2_path;

	int marker1Counter = 0, marker2Counter = 0;
	int marker1Counter_prev = 0, marker2Counter_prev = 0;

	Mat frame_gray;

public:

	vector<Marker> m1;
	vector<Marker> m2;

	double delta_x;
	double delta_y;
	double delta_z;

	double upper;
	double lower;
	double dist; //aka наклонная дальность

	double d_yaw;
	double d_pitch;
	double d_roll;

	// debug purposes
	ofstream fout;
	//Mat AUV_sees;

	AUV(string, string);
	~AUV ();

	void get_orientation(Mat& frame);

	void rotate_over_normal(Mat& frame, vector<Rect> m1, vector<Rect> m2);
	void detect_and_display(Mat frame, int cascadeNum, bool saveFalsePositive);
	void calculate_distance(Mat& frame, vector<Rect> m1, vector<Rect> m2, bool debug);
	void calculate_deltas(Mat& frame, bool debug);
	void line_equation(double &k, double &b, bool mainDiag);

	vector<Rect> filter_objects_2(vector<Rect> objects, Mat& currentFrame, Mat& frame_gray, markerType m_type, Mat AUV_sees, bool debug);
	void arrange_markers(Mat& frame);
	void estimatePos();
};

