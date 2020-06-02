#pragma once
#include "Marker.h"

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <opencv2/core.hpp>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;


void draw_objects(Mat& frame, vector<Marker> objects, Scalar color);
void draw_configuration(Mat& frame, vector<Marker> m1, vector<Marker> m2);
void print_objects(vector<Marker> ob, string title);

bool compar(Rect a, Rect b);

vector<Rect> filter_objects(vector<Rect> objects, Mat& currentFrame, Mat& frame_gray, int m_type, bool debug);

//void accumulate_false_pos(Mat frame);
