﻿// distance_calibration.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iomanip>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace cv;

CascadeClassifier marker_1, marker_2;
String marker_1_name = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/haar_navigation_m1_v3/cascade.xml";
String marker_2_name = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/haar_navigation_m2_v1/cascade.xml";

//void calculate(string path, int nominal_distance);

void draw_objects(Mat &frame, vector<Rect> objects, Scalar color) {
	
	for (size_t i = 0; i < objects.size(); i++) {
		//cout << "X = " << objects[i].x << " Y = " << objects[i].y << "\n";
		Point center(objects[i].x + objects[i].width / 2, objects[i].y + objects[i].height / 2);
		ellipse(frame, center, Size(objects[i].width / 2, objects[i].height / 2), 0, 0, 360, color, 2);
	}
}

void print_objects(vector<Rect> ob, string title = "Objects") {

	for (int i = 0; i < ob.size(); i++) {
		cout << setw(2) << i << " x: " << setw(5) << ob[i].x << " y: " << setw(5) << ob[i].y << " w: " << setw(5) << ob[i].width << " h:" << setw(5) << ob[i].height << "\n";
	}
	cout << "\n";
}

bool compar(Rect a, Rect b) {
		return (a.width > b.width);
	}

void filter_objects(vector<Rect> objects) {

	sort(objects.begin(), objects.end(), compar);
	/*
	Дополнительная фильтрация будет добавлена вскоре
	*/
	print_objects(objects);
	return;
}

double calculate(string path, string filename, int nominal_distance) {

	Mat frame = imread(path+filename);
	Mat frame_gray;

	int delta_x_1 = 0, delta_x_2 = 0;
	int delta_y_1 = 0, delta_y_2 = 0;
	double average = 0;

	if (frame.empty()) {
		cout << "--(!) Can't open image" << path << filename << "\n";
		return 0;
	}
	else {
		cout << "Reading " << filename << "\n";
	}

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	vector<Rect> objs1, objs2;
	Scalar* color1 = new Scalar(0, 255, 255); // B G R
	Scalar* color2 = new Scalar(255, 0, 255);

	marker_1.detectMultiScale(frame_gray, objs1);
	marker_2.detectMultiScale(frame_gray, objs2);

	draw_objects(frame, objs1, *color1);
	draw_objects(frame, objs2, *color2);

	namedWindow("Detected markers", WINDOW_NORMAL);
	resizeWindow("Detected markers", 1280, 720);
	imshow("Detected markers", frame);

	filter_objects(objs1);
	filter_objects(objs2);
	//print_objects(objs1);
	//print_objects(objs2);


	delta_x_1 = abs(objs1[0].x - objs1[1].x);
	delta_x_2 = abs(objs2[0].x - objs2[1].x);

	Rect left_1 = objs1[0].x < objs1[1].x ? objs1[0] : objs1[1];
	Rect left_2 = objs2[0].x < objs2[1].x ? objs2[0] : objs2[1];
	delta_y_1 = abs(left_1.y - left_2.y);

	Rect right_1 = objs1[0].x > objs1[1].x ? objs1[0] : objs1[1];
	Rect right_2 = objs2[0].x > objs2[1].x ? objs2[0] : objs2[1];
	delta_y_2 = abs(right_1.y - right_2.y);

	cout << "dx1 = " << setw(4) << delta_x_1 << " dx2 = " << setw(4) << delta_x_2 << "\n";
	cout << "dy1 = " << setw(4) << delta_y_1 << " dy2 = " << setw(4) << delta_y_2 << "\n";

	average = (delta_x_1 + delta_x_2 + delta_y_1 + delta_y_2) / 4;
	cout << "Average: " << average << "\n\n";

	//waitKey(0);
	return average;
}

int main() {

	//-- 1. Load the cascades
	if (!marker_1.load(marker_1_name)) {
		cout << "--(!)Error loading first cascade\n";
		return -1;
	}
	else if (!marker_2.load(marker_2_name)) {
		cout << "--(!)Error loading second cascade\n";
		return -1;
	}

	string path = "E:/University/10sem/nirs/haar_3_4_6/distance_calibration/images/50cm/";
	//string name = "dist50cm01.jpg";

	vector<double> average_50;

	for (int i = 1; i < 5; i++) {
		string name = "dist50cm0" + to_string(i) + ".jpg";
		double temp = calculate(path, name, 50);
		average_50.push_back(temp);
	}

	for (int j = 0; j < average_50.size(); j++)
		cout << average_50[j] << "\n";

	waitKey(0);

	return 0;
}


