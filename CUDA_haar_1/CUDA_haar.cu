
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
//#include "opencv2/objdetect.hpp"
#include "opencv2/cudaobjdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>


using namespace std;
using namespace cv;

void detect_Display(Mat frame);

//cuda::CascadeClassifier model_cascade;
Ptr<cuda::CascadeClassifier> cascade_gpu;// = cuda::CascadeClassifier::create("E:/University/10sem/nirs/haar_3_4_6/x64/Debug/cascade.xml");

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
	if (!cascade_gpu->create(model_cascade_name)) {
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


	cudaError_t cudaStatus;

    // cudaDeviceReset must be called before exiting in order for profiling and
    // tracing tools such as Nsight and Visual Profiler to show complete traces.
    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceReset failed!");
        return 1;
    }

    return 0;
}

