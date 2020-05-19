#include "Marker.h"

Marker::Marker(int _x, int _y, enum markerType _t) {

	this->x = _x;
	this->y = _y;
	this->t = _t;
}

Mat Marker::get_template_t1(int width, int height) {

	Mat temp = Mat(width, height, CV_8UC1, 255.0);
	circle(temp, Point(width / 2, height / 2), 0.35 * width, BLK, -1);
	return temp;
}

Mat Marker::get_template_t2(int width, int height) {

	Mat temp = Mat::zeros(width, height, CV_8UC1);
	circle(temp, Point(width/2, height/2), 0.35 * width, WHT, -1);
	//imshow("c", c);
	return temp;
}