// haar_3_4_6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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

//#include <windows.h>
#include "FPS.h"
#include "Marker.h"
#include "AUV.h"
#include "functions.h"

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace std;
using namespace cv;


int false_positive_counter = 0;


int main(int argc, const char** argv) {

	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{model_cascade|E:/University/10sem/nirs/haar_3_4_6/preparing navigation/haar_navigation_m1_v3/cascade.xml|Path to cascade No 1.}"
		"{model_cascade_2|E:/University/10sem/nirs/haar_3_4_6/preparing navigation/haar_navigation_m2_v1/cascade.xml|Path to cascade No 2.}"
		"{camera|0|Camera device number.}");

	parser.printMessage();

	bool use_LBP = true;

	String model_cascade_name, model_cascade_name_2;
	
	if (use_LBP) {

		model_cascade_name = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/haar_navigation_m1_v4/cascade.xml";
		model_cascade_name_2 = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/haar_navigation_m2_v2/cascade.xml";
	}
	else {
		model_cascade_name = parser.get<String>("model_cascade");
		model_cascade_name_2  = parser.get<String>("model_cascade_2");
	}



	AUV auv(model_cascade_name, model_cascade_name_2);

	int camera_device = parser.get<int>("camera");
	//VideoCapture capture(camera_device);

	VideoCapture capture("E:/University/10sem/nirs/haar_3_4_6/pyramid_test.mp4");

	Size S = Size((int)capture.get(CAP_PROP_FRAME_WIDTH), (int)capture.get(CAP_PROP_FRAME_HEIGHT));
	string abs_path = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/videos/pyramid_test_demo.mp4";

	//VideoWriter video(abs_path, CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(1280, 720));
	//VideoWriter video(abs_path, CV_FOURCC('M', 'P', '4', 'V'), 30, Size(1280, 720));

	
	//capture.open(camera_device);

	if (!capture.isOpened()) {
		cout << "--(!)Error opening video capture\n";
		return -1;
	}
	Mat frame;
	int frameno = 0;



	while (1) {
		capture.read(frame);
		//frame.copyTo(frame_2);

		if (frame.empty()) {
			cout << "--(!) No captured frame -- Break!\n";
			break;
		}


		//Mat b = Mat::zeros(frame.rows, frame.cols, CV_8UC1);


		//Mat c = Mat::zeros(50, 50, CV_8UC1);
		////rectangle(c, Rect(5, 5, 45, 45))
		//circle(c, Point(c.rows / 2, c.cols / 2), 0.40 * c.rows, WHT, -1);
		//imshow("c", c);

		//imshow("T1", Marker::get_template_t1(50, 50));
		//imshow("T2", Marker::get_template_t2(50, 50));

		auv.get_orientation(frame);


		double start = CLOCK();
		double dur = CLOCK() - start;
		printf("avg time per frame %f ms. fps %f. frameno = %d\n", avgdur(dur), avgfps(), frameno++);


		imshow("Orientation ", frame);

		//video.write(frame);
	
		if (waitKey(25) == 27)
			break;
	}
	// When everything done, release the video capture object
	capture.release();
	//video.release();

	// Closes all the frames
	destroyAllWindows();
	return 0;
}
