﻿// haar_3_4_6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

/*
	Идея: 
	создать метод, который сохранял бы кадры, на которых распознан мусор
	(в простейшем случае те, где указано больше 2-х целевых объектов).
	Впоследствии дообучить каскады с учётом наработанных ошибок

	NB2: Файлы bad.dat могут быть разными для 1-го и 2-го маркеров.
	Маркер №1 является фоном для маркера №2 и наоборот. Это стоит учесть

	На данном этапе маркеры №2 работают много лучше, даже несмотря на то что
	Каскад /haar_navigation_m2_v1 обучен на вдвое меньшем количестве позитивных изображений
*/

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <iomanip>
#include "functions.h"

#include "AUV.h"

using namespace std;
using namespace cv;


int false_positive_counter = 0;


int main(int argc, const char** argv) {

	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{model_cascade|E:/University/10sem/nirs/haar_3_4_6/preparing navigation/haar_navigation_m1_v3/cascade.xml|Path to cascade No 1.}"
		"{model_cascade_2|E:/University/10sem/nirs/haar_3_4_6/preparing navigation/haar_navigation_m2_v1/cascade.xml|Path to cascade No 2.}"
		"{camera|0|Camera device number.}");
	parser.about("\nThis program demonstrates using the cv::CascadeClassifier class to detect objects in a video stream.\n"
		"You can use Haar or LBP features.\n\n");
	parser.printMessage();

	String model_cascade_name = parser.get<String>("model_cascade");
	String model_cascade_name_2 = parser.get<String>("model_cascade_2");

	AUV auv(model_cascade_name, model_cascade_name_2);

	int camera_device = parser.get<int>("camera");
	//VideoCapture capture(camera_device);
	VideoCapture capture("E:/University/10sem/nirs/haar_3_4_6/pyramid_test.mp4");

	Size S = Size((int)capture.get(CAP_PROP_FRAME_WIDTH), (int)capture.get(CAP_PROP_FRAME_HEIGHT));
	string abs_path = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/videos/pyramid_test_demo.mp4";

	//VideoWriter video(abs_path, CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(1280, 720));
	VideoWriter video(abs_path, CV_FOURCC('M', 'P', '4', 'V'), 30, Size(1280, 720));

	
	//capture.open(camera_device);

	if (!capture.isOpened()) {
		cout << "--(!)Error opening video capture\n";
		return -1;
	}
	Mat frame;

	while (1) {
		capture.read(frame);
		//frame.copyTo(frame_2);

		if (frame.empty()) {
			cout << "--(!) No captured frame -- Break!\n";
			break;
		}

		auv.get_orientation(frame);

		imshow("Orientation ", frame);

		//video.write(frame);
	
		if (waitKey(25) == 27)
			break;
	}
	// When everything done, release the video capture object
	capture.release();
	video.release();

	// Closes all the frames
	destroyAllWindows();
	return 0;
}
