// haar_3_4_6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace cv;

void detect_and_display(Mat frame, int CascadeNum);

CascadeClassifier model_cascade, model_cascade_2;
int cascadeCounter_1 = 0, cascadeCounter_2 = 0;
int cascadeCounterPrev_1 = 0, cascadeCounterPrev_2 = 0;


int main(int argc, const char** argv) {

	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{model_cascade|E:/University/10sem/nirs/haar_3_4_6/preparing navigation/haar_navigation_m1_v1/cascade.xml|Path to cascade No 1.}"
		"{model_cascade_2|E:/University/10sem/nirs/haar_3_4_6/preparing data/haar_results_v6/cascade.xml|Path to cascade No 2.}"
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
	VideoCapture capture(camera_device);

	//-- 2. Read the video stream
	//capture.open(camera_device);

	if (!capture.isOpened()) {
		cout << "--(!)Error opening video capture\n";
		return -1;
	}
	Mat frame, frame_2;

	while (1) {
		capture.read(frame);
		frame.copyTo(frame_2);

		if (frame.empty() || frame_2.empty()) {
			cout << "--(!) No captured frame -- Break!\n";
			break;
		}
		//imshow("Captured", frame);

		//-- 3. Apply the classifier to the frame
		detect_and_display(frame, 1);
		detect_and_display(frame_2, 2);

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

	// Closes all the frames
	destroyAllWindows();
	return 0;
}


void detect_and_display(Mat frame, int cascadeNum) {
	//cout << "Inside detect_Display";

	Mat frame_gray;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//imshow("Grayscale", frame_gray);

	//-- Detect object
	std::vector<Rect> objects;

	switch (cascadeNum) {

	case 1:
		model_cascade.detectMultiScale(frame_gray, objects);
		break;
	case 2:
		model_cascade_2.detectMultiScale(frame_gray, objects);
		break;
	default:
		cout << "Error in switch-case detecting block \n";
		break;
	}


	for (size_t i = 0; i < objects.size(); i++) {

		//cout << "X = " << objects[i].x << " Y = " << objects[i].y << "\n";

		Point center(objects[i].x + objects[i].width / 2, objects[i].y + objects[i].height / 2);
		ellipse(frame, center, Size(objects[i].width / 2, objects[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);

		//Mat faceROI = frame_gray(objects[i]);
	}

	//-- Show what you got
	switch (cascadeNum) {

	case 1:
		imshow("Markers #1 ", frame);
		if (objects.size() != 0)
			cascadeCounter_1++;
		break;
	case 2:
		imshow("Waiter", frame);
		if (objects.size() != 0)
			cascadeCounter_2++;
		break;
	default:
		cout << "Error in switch-case showing block \n";
		break;
	}
	return;
}