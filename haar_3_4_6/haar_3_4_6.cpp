// haar_3_4_6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void detect_Display(Mat frame);

CascadeClassifier model_cascade;

int main(int argc, const char** argv) {

	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{model_cascade|E:/University/10sem/nirs/haar_3_4_6/x64/Debug/cascade.xml|Path to face cascade.}"
		"{camera|0|Camera device number.}");
	parser.about("\nThis program demonstrates using the cv::CascadeClassifier class to detect objects in a video stream.\n"
		"You can use Haar or LBP features.\n\n");
	parser.printMessage();

	String model_cascade_name = parser.get<String>("model_cascade");

	//-- 1. Load the cascades
	if (!model_cascade.load(model_cascade_name)) {
		cout << "--(!)Error loading face cascade\n";
		return -1;
	};

	int camera_device = parser.get<int>("camera");
	VideoCapture capture(camera_device);

	//-- 2. Read the video stream
	//capture.open(camera_device);

	if (!capture.isOpened()) {
		cout << "--(!)Error opening video capture\n";
		return -1;
	}
	Mat frame;

	while (1) {

	capture.read(frame);

		if (frame.empty()) {
			cout << "--(!) No captured frame -- Break!\n";
			break;
		}
		imshow("Captured", frame);

		//-- 3. Apply the classifier to the frame
		detect_Display(frame);

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
void detect_Display(Mat frame)
{
	//cout << "Inside detect_Display";

	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	imshow("Grayscale", frame_gray);

	//-- Detect object
	std::vector<Rect> objects;

	model_cascade.detectMultiScale(frame_gray, objects);

	for (size_t i = 0; i < objects.size(); i++)
	{
		cout << "X = " << objects[i].x << " Y = " << objects[i].y;

		Point center(objects[i].x + objects[i].width / 2, objects[i].y + objects[i].height / 2);

		ellipse(frame, center, Size(objects[i].width / 2, objects[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);

		//Mat faceROI = frame_gray(objects[i]);

	}
	//-- Show what you got
	imshow("Capture - Face detection", frame);

	return;
}