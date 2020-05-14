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

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <iomanip>
#include "functions.h"

using namespace std;
using namespace cv;

void detect_and_display(Mat frame, int CascadeNum, bool saveFalsePositive);
void rotate_over_normal(Mat& frame, vector<Rect> m1, vector<Rect> m2);
//void rotate_over_normal(Mat frame);

CascadeClassifier model_cascade, model_cascade_2;
int cascadeCounter_1 = 0, cascadeCounter_2 = 0;
int cascadeCounterPrev_1 = 0, cascadeCounterPrev_2 = 0;

double alpha = 0;
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

	//-- 1. Load the cascades
	if (!model_cascade.load(model_cascade_name)) {
		cout << "--(!)Error loading first cascade\n";
		return -1;
	}
	else if (!model_cascade_2.load(model_cascade_name_2)) {
		cout << "--(!)Error loading second cascade\n";
		return -1;
	}

	int camera_device = parser.get<int>("camera");
	//VideoCapture capture(camera_device);
	VideoCapture capture("E:/University/10sem/nirs/haar_3_4_6/preparing navigation/videos/pyramid_test.mp4");

	Size S = Size((int)capture.get(CAP_PROP_FRAME_WIDTH), (int)capture.get(CAP_PROP_FRAME_HEIGHT));
	

	string abs_path = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/videos/pyramid_test_demo.mp4";
	//VideoWriter video(abs_path, CV_FOURCC('M', 'J', 'P', 'G'), 30, Size(1280, 720));
	VideoWriter video(abs_path, CV_FOURCC('M', 'P', '4', 'V'), 30, Size(1280, 720));

	
	//video.open(NAME, ex, inputVideo.get(CAP_PROP_FPS), S, true);


	//E:\University\10sem\nirs\haar_3_4_6\preparing navigation\videos

	//capture.set(CAP_PROP_FRAME_WIDTH, 1280); capture.set(CAP_PROP_FRAME_HEIGHT, 720);
	//capture.set(CAP_PROP_FRAME_WIDTH, 1920); capture.set(CAP_PROP_FRAME_HEIGHT, 1080);

	//-- 2. Read the video stream
	//capture.open(camera_device);

	if (!capture.isOpened()) {
		cout << "--(!)Error opening video capture\n";
		return -1;
	}
	Mat frame, frame_2, frame_gray;

	while (1) {
		capture.read(frame);
		frame.copyTo(frame_2);

		if (frame.empty() || frame_2.empty()) {
			cout << "--(!) No captured frame -- Break!\n";
			break;
		}
		//imshow("Captured", frame);
		//-- 3. Apply the classifier to the frame
		//detect_and_display(frame, 1, true);
		//detect_and_display(frame_2, 2);

		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		vector<Rect> markers1, markers2;
		//Scalar* color = new Scalar(0, 255, 255); // B G R

		model_cascade.detectMultiScale(frame_gray, markers1);
		model_cascade_2.detectMultiScale(frame_gray, markers2);

		markers1 = filter_objects(markers1, frame, false);
		markers2 = filter_objects(markers2, frame, false);

		rotate_over_normal(frame, markers1, markers2);
		calculate_distance(frame, markers1, markers2, true);

		video.write(frame);
		imshow("Rotation ", frame);

		if (cascadeCounter_1 != cascadeCounterPrev_1 || cascadeCounter_2 != cascadeCounterPrev_2) {
			cout << setw(6) << cascadeCounter_1 << setw(6) << cascadeCounter_2 << "\n";
			cascadeCounterPrev_1 = cascadeCounter_1;
			cascadeCounterPrev_2 = cascadeCounter_2;
		}

		// Press  ESC on keyboard to exit
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



/*
	Определяет наклон камеры по крену (ну почти) 
	в диапазоне от -90 до +90 градусов
*/
void rotate_over_normal(Mat &frame, vector<Rect> m1, vector<Rect> m2) {

	//double x1, x2 = 0;
	//double y1, y2 = 0;

	double delta_x, delta_y = 0;

	//Scalar* color = new Scalar(0, 255, 255); // B G R

	//model_cascade.detectMultiScale(frame_gray, objs);
	//model_cascade_2.detectMultiScale(frame_gray, objs);

	draw_objects(frame, m1, Scalar(0, 255, 255));
	draw_objects(frame, m2, Scalar(255, 0, 255));

	if (m2.size() == 2) {

		//PointA
		delta_x = abs(m2[0].x - m2[1].x);
		delta_y = abs(m2[0].y - m2[1].y);

		//не известно, вкаком порядке детектируются точки: сначала левая, а потом правая, или наоборот
		// Если ОДНА точка левее и выше, то считаем поворот по ч.с. со знаком "+"
		if ((m2[0].x < m2[1].x && m2[0].y > m2[1].y) || (m2[1].x < m2[0].x && m2[1].y > m2[0].y)) {
			alpha = atan(delta_y / delta_x);
		}
		else {
			alpha = - atan(delta_y / delta_x);
		}
	}
	double degs = alpha * 180 / 3.1415926535;

	ostringstream strstream;
	strstream << setprecision(2);
	strstream << "Rotation over ";
	strstream << degs;
	strstream << "degs";

	string str = strstream.str();
	//string str = "Rotation over " + to_string(degs) + "degs";

	String text(str);

	putText(frame, text, Point(10, 40), 0, 1.2, Scalar(255, 255, 0), 2);	
	
	return;
}


void detect_and_display(Mat frame, int cascadeNum, bool saveFalsePositive = false) {
	//cout << "Inside detect_Display";

	Mat frame_gray;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//imshow("Grayscale", frame_gray);

	//-- Detect object
	std::vector<Rect> objects;
	Scalar *color = new Scalar(255, 0, 0);

	switch (cascadeNum) {

	case 1:
		model_cascade.detectMultiScale(frame_gray, objects);
		color = new Scalar(255, 0, 255);
		break;
	case 2:
		model_cascade_2.detectMultiScale(frame_gray, objects);
		color = new Scalar(0, 255, 255);
		break;
	default:
		cout << "Error in switch-case detecting block \n";
		break;
	}

	draw_objects(frame, objects, *color);

	//-- Show what you got
	switch (cascadeNum) {

	case 1:
		imshow("Markers #1 ", frame);
		if (objects.size() == 2)
			cascadeCounter_1++;

		if (objects.size() > 2 && saveFalsePositive) {
			//string filename = "../../preparing navigation/false_positive_1/" + to_string(false_positive_counter++) + ".jpg";
			string filename = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/false_positive_1/" + to_string(false_positive_counter++) + ".jpg";

			imwrite(filename, frame);
		}
		break;
	case 2:
		imshow("Marker #2", frame);
		if (objects.size() == 2)
			cascadeCounter_2++;

		if (objects.size() > 2 && saveFalsePositive) {
			//string filename = "../../preparing navigation/false_positive_2/" + to_string(false_positive_counter++) + ".jpg";
			string filename = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/false_positive_2/" + to_string(false_positive_counter++) + ".jpg";
			imwrite(filename, frame);
		}
		break;
	default:
		cout << "Error in switch-case showing block \n";
		break;
	}
	return;
}