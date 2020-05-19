#pragma once

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;

#define RED Scalar(0, 0, 255)
#define BLU Scalar(255, 0, 0)
#define GRN Scalar(0, 255, 0)
#define PNK Scalar(255, 0, 255)
#define YEL Scalar(0, 255, 255)
#define CYA Scalar(255, 255, 0)


void draw_objects(Mat& frame, vector<Rect> objects, Scalar color);
void print_objects(vector<Rect> ob, string title);

bool compar(Rect a, Rect b);

vector<Rect> filter_objects(vector<Rect> objects, Mat& currentFrame, Mat& frame_gray, bool debug);

//void accumulate_false_pos(Mat frame);
