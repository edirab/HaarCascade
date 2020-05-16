#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;

void draw_objects(Mat& frame, vector<Rect> objects, Scalar color);
void print_objects(vector<Rect> ob, string title);

bool compar(Rect a, Rect b);

vector<Rect> filter_objects(vector<Rect> objects, Mat& currentFrame, bool debug);
void calculate_distance(Mat& frame, vector<Rect> m1, vector<Rect> m2, bool debug);

//void accumulate_false_pos(Mat frame);
