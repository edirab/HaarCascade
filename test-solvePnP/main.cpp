
/*
	https://www.learnopencv.com/head-pose-estimation-using-opencv-and-dlib/
*/

#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <cstdlib>
#include <iomanip>


using namespace std;
using namespace cv;

bool debug_on_image = false;

Mat cMatrix640 = (Mat_<double>(3, 3) << 5.3226273868525448e+02, 0, 3.2590522394049350e+02, 0, 5.3226273868525448e+02, 2.6946997900677803e+02, 0, 0, 1);
Mat distortion640 = (Mat_<double>(1, 5) << 0, -6.1539772782054671e-02, 0, 0, 1.7618036793466491e-02);

//Mat rvecs, tvecs;
vector<Vec3d> rvecs, tvecs;

Mat frame;
Mat frame_copy;
//Mat image;
//Mat image_copy;
VideoCapture capture;

vector<int> ids;
vector<std::vector<cv::Point2f>> corners;

Mat rotMat(3, 3, CV_64F);
Mat Euler(3, 3, CV_64F);

Vec3f Eul;

void test() {

	// Read input image
	cv::Mat im = cv::imread("E:/University/10sem/nirs/haar_3_4_6/head.jpg");

	// 2D image points. If you change the image, you need to change vector
	std::vector<cv::Point2d> image_points;
	image_points.push_back(cv::Point2d(359, 391));    // Nose tip
	image_points.push_back(cv::Point2d(399, 561));    // Chin
	image_points.push_back(cv::Point2d(337, 297));     // Left eye left corner
	image_points.push_back(cv::Point2d(513, 301));    // Right eye right corner
	image_points.push_back(cv::Point2d(345, 465));    // Left Mouth corner
	image_points.push_back(cv::Point2d(453, 469));    // Right mouth corner

	// 3D model points.
	std::vector<cv::Point3d> model_points;
	model_points.push_back(cv::Point3d(0.0f, 0.0f, 0.0f));               // Nose tip
	model_points.push_back(cv::Point3d(0.0f, -330.0f, -65.0f));          // Chin
	model_points.push_back(cv::Point3d(-225.0f, 170.0f, -135.0f));       // Left eye left corner
	model_points.push_back(cv::Point3d(225.0f, 170.0f, -135.0f));        // Right eye right corner
	model_points.push_back(cv::Point3d(-150.0f, -150.0f, -125.0f));      // Left Mouth corner
	model_points.push_back(cv::Point3d(150.0f, -150.0f, -125.0f));       // Right mouth corner

	// Camera internals
	double focal_length = im.cols; // Approximate focal length.
	Point2d center = cv::Point2d(im.cols / 2, im.rows / 2);
	cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
	cv::Mat dist_coeffs = cv::Mat::zeros(4, 1, cv::DataType<double>::type); // Assuming no lens distortion

	cout << "Camera Matrix " << endl << camera_matrix << endl;
	// Output rotation and translation
	cv::Mat rotation_vector; // Rotation in axis-angle form
	cv::Mat translation_vector;

	// Solve for pose
	cv::solvePnP(model_points, image_points, camera_matrix, dist_coeffs, rotation_vector, translation_vector);


	// Project a 3D point (0, 0, 1000.0) onto the image plane.
	// We use this to draw a line sticking out of the nose

	vector<Point3d> nose_end_point3D;
	vector<Point2d> nose_end_point2D;
	nose_end_point3D.push_back(Point3d(0, 0, 1000.0));

	projectPoints(nose_end_point3D, rotation_vector, translation_vector, camera_matrix, dist_coeffs, nose_end_point2D);


	for (int i = 0; i < image_points.size(); i++)
	{
		circle(im, image_points[i], 3, Scalar(0, 0, 255), -1);
	}

	cv::line(im, image_points[0], nose_end_point2D[0], cv::Scalar(255, 0, 0), 2);

	cout << "Rotation Vector " << endl << rotation_vector << endl;
	cout << "Translation Vector" << endl << translation_vector << endl;

	cout << nose_end_point2D << endl;

	// Display image.
	cv::imshow("Output", im);
	cv::waitKey(0);

}


// Checks if a matrix is a valid rotation matrix.
bool isRotationMatrix(Mat& R) {

	Mat Rt;
	transpose(R, Rt);
	Mat shouldBeIdentity = Rt * R;
	Mat I = Mat::eye(3, 3, shouldBeIdentity.type());

	return  norm(I, shouldBeIdentity) < 1e-6;

}

// Calculates rotation matrix to euler angles
// The result is the same as MATLAB except the order
// of the euler angles ( x and z are swapped ).
Vec3f rotationMatrixToEulerAngles(Mat& R) {

	assert(isRotationMatrix(R));

	float sy = sqrt(R.at<double>(0, 0) * R.at<double>(0, 0) + R.at<double>(1, 0) * R.at<double>(1, 0));

	bool singular = sy < 1e-6; // If

	float x, y, z;
	if (!singular)
	{
		x = atan2(R.at<double>(2, 1), R.at<double>(2, 2));
		y = atan2(-R.at<double>(2, 0), sy);
		z = atan2(R.at<double>(1, 0), R.at<double>(0, 0));
	}
	else
	{
		x = atan2(-R.at<double>(1, 2), R.at<double>(1, 1));
		y = atan2(-R.at<double>(2, 0), sy);
		z = 0;
	}
	return Vec3f(x, y, z);
}


int main(int argc, char* argv[]) {

	//image = imread("E:/University/10sem/nirs/haar_3_4_6/comparioson/01.jpg");
	namedWindow("Marker", WINDOW_NORMAL);

	//image.copyTo(image_copy);

	Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_ARUCO_ORIGINAL);


	if (!debug_on_image) {
		capture.open(0);
	}
	else if (!debug_on_image) {
		capture.open("***.mp4");
	}

	if (!capture.isOpened() && !debug_on_image) {
		cout << "--(!)Error opening video capture\n";
		return -1;
	}

	if (debug_on_image) {
		frame = imread("E:/University/10sem/nirs/haar_3_4_6/comparioson/04.jpg");
	}

	do {
		if (!debug_on_image) {
			capture.read(frame);
		}

		if (frame.empty()) {
			cout << "--(!) No captured frame -- Break!\n";
			break;
		}
		frame.copyTo(frame_copy);

		cv::aruco::detectMarkers(frame, dictionary, corners, ids);
		cv::aruco::drawDetectedMarkers(frame_copy, corners, ids);

		cv::aruco::estimatePoseSingleMarkers(corners, 30, cMatrix640, distortion640, rvecs, tvecs);

		//cout << rvecs << "\n";

		Rodrigues(rvecs[0], rotMat);

	
		if (isRotationMatrix(rotMat)) {
			Eul = rotationMatrixToEulerAngles(rotMat);

			cout << setprecision(3);

			if (rvecs.size() != 0) {
				//cout << setw(7) << Eul[0]*180/3.1415926 << setw(7) << Eul[1] * 180 / 3.1415926 << setw(7) << Eul[2] * 180 / 3.1415926 << "\n";
				cout << Eul[1] * 180 / 3.1415926 << "\n";
			}
		}

		imshow("Marker", frame_copy);

		if (waitKey(25) == 27)
			break;

	}
	while (!debug_on_image);

	if (debug_on_image)
	waitKey(0);

	//imshow("Marker2", image_copy);
	//waitKey(0);

	return 0;
}

