#include "AUV.h"


AUV::AUV(string path1, string path2) {

	marker_1_path = path1;
	marker_2_path = path2;

	m1.resize(2);
	m2.resize(2);

	/*
	Simplified solution

	double focal_length = frame_gray.cols; // Approximate focal length.
	Point2d center = cv::Point2d(frame_gray.cols / 2, frame_gray.rows / 2);
	cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << focal_length, 0, center.x, 0, focal_length, center.y, 0, 0, 1);
	*/

	cMatrix640 = (Mat_<double>(3, 3) << 5.3226273868525448e+02, 0, 3.2590522394049350e+02, 0, 5.3226273868525448e+02, 2.6946997900677803e+02, 0, 0, 1);
	cMatrix1280 = (Mat_<double>(3, 3) << 8.6155235630774325e+02, 0, 6.2961522415048103e+02 ,0, 8.6155235630774325e+02, 3.9881978167213623e+02,0, 0, 1);


	distortion640 = (Mat_<double>(1, 5) << 0, -6.1539772782054671e-02, 0, 0, 1.7618036793466491e-02);
	distortion1280 = (Mat_<double>(1, 5) << 0, -6.5524123635067169e-02, 0, 0, 0 );

	// Задание координат маркеров
	model_points.push_back(cv::Point3d(-50, 50, 0));  // left up corner
	model_points.push_back(cv::Point3d(50, 50, 0));   // right up corner
	model_points.push_back(cv::Point3d(50, -50, 0));  // left up corner
	model_points.push_back(cv::Point3d(-50, -50, 0)); // left down corner

	//-- 1. Load the cascades
	if (!marker_type_1.load(marker_1_path)) {
		cout << "--(!)Error loading first cascade\n";
	}
	else if (!marker_type_2.load(marker_2_path)) {
		cout << "--(!)Error loading second cascade\n";
	}
	fout.open("E:/University/10sem/nirs/haar_3_4_6/haar_navigation/filtering_output.dat");

	if (!fout) {
		cout << "Error opening fout \n";
	}
}

AUV ::~AUV() {
	

	fout.flush();
	fout.close();
}


/*
	Определяет наклон камеры по крену (ну почти)
	в диапазоне от -90 до +90 градусов
*/
void AUV::rotate_over_normal(Mat& frame) {

	static double delta_x = 0;
	static double delta_y = 0;
	static double alpha = 0;

	if (m2.size() == 2) {

		//PointA
		delta_x = abs(m2[0].x - m2[1].x);
		delta_y = abs(m2[0].y - m2[1].y);

		//не известно, в каком порядке детектируются точки: сначала левая, а потом правая, или наоборот
		// Если ОДНА точка левее и выше, то считаем поворот по ч.с. со знаком "+"
		if ((m2[0].x < m2[1].x && m2[0].y > m2[1].y) || (m2[1].x < m2[0].x && m2[1].y > m2[0].y)) {
			alpha = atan(delta_y / delta_x);
		}
		else {
			alpha = -atan(delta_y / delta_x);
		}
	}

	if (abs(alpha) >= 0 && abs(alpha) < 2 * 3.1415926535) {
		this->d_roll = alpha * 180 / 3.1415926535;
	}

	ostringstream strstream;
	strstream << setprecision(2);
	strstream << "rotation: ";
	strstream << this->d_roll;
	strstream << " deg";

	//string str = "Rotation over " + to_string(degs) + "degs";

	String text(strstream.str());
	int text_y = int(frame.rows * 0.6);
	putText(frame, text, Point(100, text_y), 0, 1, BLK, 2);

	return;
}


void AUV::arrange_markers(Mat& frame, bool debug) {

	//assert(m1.size() == 2 && m2.size() == 2);

	if ((m1.size() == 2 && m2.size() == 2)) {

		if (this->d_roll > 0) {

			//if (m1[0].x > m1[1].x && m1[0].y < m1[1].y) {
			if (m1[0].y < m1[1].y) {
				swap(m1[0], m1[1]);
			}
			if (m2[0].y < m2[1].y) {
				swap(m2[0], m2[1]);
			}
		}
		else if (this->d_roll <= 0) {

			if (m1[0].y > m1[1].y) {
				swap(m1[0], m1[1]);
			}
			if (m2[0].y > m2[1].y) {
				swap(m2[0], m2[1]);
			}
		}

		Scalar COLOR;
		if (frame.channels() == 1) {
			COLOR = WHT;
		}
		else {
			COLOR = RED;
		}

		if (debug) {
			putText(frame, String("11"), Point(m1[0].x + 10, m1[0].y - 10), 1, 1, COLOR);
			putText(frame, String("12"), Point(m1[1].x + 10, m1[1].y - 10), 1, 1, COLOR);
			putText(frame, String("21"), Point(m2[0].x + 10, m2[0].y - 10), 1, 1, COLOR);
			putText(frame, String("22"), Point(m2[1].x + 10, m2[1].y - 10), 1, 1, COLOR);

			draw_configuration(frame, this->m1, this->m2);
		}
	}
}


void AUV::calculate_distance(Mat& frame, bool debug) {
	
	if (m1.size() == 2) {
		//Point a, b;
		upper = sqrt(pow(abs(m1[0].x - m1[1].x), 2) + pow(abs(m1[0].y - m1[1].y), 2));
	}

	if (m2.size() == 2) {
		lower = sqrt(pow(abs(m2[0].x - m2[1].x), 2) + pow(abs(m2[0].y - m2[1].y), 2));
	}

	if (debug) {
		cout << "upper = " << upper << " lower = " << lower << "\n";
	}

	int w = frame.cols;
	float scale = float(w) / 640;

	double average = (upper + lower) / 2;
	double calculated_distance = 50 * 100 / (average / scale);

	if (calculated_distance > 0 && calculated_distance <= 200)
		this->dist = calculated_distance;

	ostringstream strstream;
	//strstream << setprecision(0);
	strstream << "d, cm: ";
	strstream << setw(3) << int(dist);// << " " << setw(3) << int(lower);

	String text(strstream.str());
	//putText(frame, text, Point(10, 400), 0, 1, Scalar(255, 255, 255), 2);
	int text_y = int(frame.rows * 0.5);
	putText(frame, text, Point(100, text_y), 0, 1, BLK, 2);	
}


void AUV::line_equation(double& k, double& b, bool mainDiag) {
	// y = k*x + b
	Eigen::Matrix2d A;
	Eigen::Vector2d B;
	Eigen::Vector2d X;

	if (m1.size() == 2 && m2.size() == 2) {

		if (mainDiag) {

			A.row(0) << m1[0].x, 1;
			A.row(1) << m2[1].x, 1;

			B << m1[0].y, m2[1].y;
		}
		else {
			A.row(0) << m1[1].x, 1;
			A.row(1) << m2[0].x, 1;

			B << m1[1].y, m2[0].y;
		}
		//cout << A << "\n\n";
		//cout << B << "\n\n";

		X = A.lu().solve(B);

		k = X[0];
		b = X[1];
	}
}


void AUV::calculate_deltas(Mat& frame, bool debug) {

	static double k1 = 0;
	static double k2 = 0;
	static double b1 = 0;
	static double b2 = 0;

	if (m1.size() == 2) {
		// y = k*x + b
		this->line_equation(k1, b1, true);
		this->line_equation(k2, b2, false);

		//Eigen::Matrix2d A;
		//Eigen::Vector2d B;
		//Eigen::Vector2d X;

		//A.row(0) << m1[0].x, 1;
		//A.row(1) << m2[1].x, 1;

		//B << m1[0].y, m2[1].y;

		////cout << A << "\n\n";
		////cout << B << "\n\n";

		//X = A.lu().solve(B);

		//k1 = X[0];
		//b1 = X[1];

		cout << k1 << " " << b1 << " " << m1[0].x << " " << m1[0].y << " ";
		cout << m2[1].x << " " << m2[1].y << "\n";

		cout << k2 << " " << b2 << " " << m1[1].x << " " << m1[1].y << " ";
		cout << m2[0].x << " " << m2[0].y << "\n";
	}

	return;
}

void AUV::filter_objects_2(vector<Rect> objects, Mat& currentFrame, Mat& frame_gray, markerType m_type, Mat AUV_sees, bool debug = false) {

	vector<Marker> markers_;
	vector<Marker> hough_valid;
	Mat roi_mat;

	sort(objects.begin(), objects.end(), compar);
	//print_objects(objects);

	if (debug)
		cout << "objects.size() = " << objects.size() << "\n";


	// всё делается за один проход
	for (int i = 0; i < objects.size(); i++) {

		vector<Vec3f> circles;
		roi_mat = frame_gray(objects[i]);
		medianBlur(roi_mat, roi_mat, 5);

		//Mat t = Marker::get_template_t2(roi_mat.rows, roi_mat.cols);
		//imshow("Mat t", t);

		/*
		Ищем все кружочки внутри одного ROI
		*/
		HoughCircles(roi_mat, circles, HOUGH_GRADIENT, 1,
			frame_gray.rows / 16,  // change this value to detect circles with different distances to each other
			100, 30, 0.25 * roi_mat.rows, 0.50 * roi_mat.rows // change the last two parameters
			// (min_radius & max_radius) to detect larger circles
		);

		/*
		Если каскад указал на объект и детектор Хаффа нашёл кружочек, то скорее всего, это то что нужно
		*/
		if (circles.size() == 1) {

			Marker temp_m(objects[i].x + int(circles[0][0]), objects[i].y + int(circles[0][1]), m_type, objects[i]);
			hough_valid.push_back(temp_m);
		}
		/*
		В одном roi_mat кружочков больше одного. Что странно
		Этот блок практически ничего не делает. За всё тестовое видео сработал 4 раза
		*/
		else if (circles.size() > 1) {

			Mat t;
			if (m_type == markerType::black_circle) {
				t = Marker::get_template_t1(roi_mat.rows, roi_mat.cols);
				threshold(roi_mat, roi_mat, 200, 255, 0);
				absdiff(roi_mat, t, roi_mat);
				int nonZero = countNonZero(roi_mat);

				//if (nonZero < 0.1 * frame_gray.cols) {
				//	hough_valid.push_back(objects[i]);
				//}
			}
			else {
				t = Marker::get_template_t2(roi_mat.rows, roi_mat.cols);
				threshold(roi_mat, roi_mat, 200, 255, 0);
				absdiff(roi_mat, t, roi_mat);
				int nonZero = countNonZero(roi_mat);
				//if (nonZero < 0.15 * frame_gray.cols) {
				//	hough_valid.push_back(objects[i]);
				//}
			}
		}
		/*
		Ситуация: каскад утверждает, что есть объект, но детектор Хаффа кружочка не нашёл
		вот здесь и можно проверить маской
		*/
		else {
			Mat t;
			

			if (m_type == markerType::black_circle) {
				t = Marker::get_template_t1(roi_mat.rows, roi_mat.cols);
				threshold(roi_mat, roi_mat, 60, 255, THRESH_BINARY);
				//imshow("roi_mat m1 thresholded", roi_mat);

				absdiff(roi_mat, t, roi_mat);
				int nonZero = countNonZero(roi_mat);
				//cout << "m1 = " << setw(7) << nonZero << "\n";
				//fout << "m1 = " << nonZero << "\n";
			}
			else {
				t = Marker::get_template_t2(roi_mat.rows, roi_mat.cols);
				threshold(roi_mat, roi_mat, 200, 255, THRESH_BINARY);
				//imshow("roi_mat m2 thresholded", roi_mat);

				absdiff(roi_mat, t, roi_mat);
				int nonZero = countNonZero(roi_mat);
				//cout << "m2 = " << setw(7) << nonZero << "\n";
				//fout << "m2 = " << nonZero << "\n";
			}
		}
		// конец проверки маской
	}

	/*
	
	*/
	if (hough_valid.size() > 2) {
		for (size_t i = 0; i < hough_valid.size() - 1; i++) {

			int max_width = max(hough_valid[i].roi.width, hough_valid[i + 1].roi.width);
			int min_width = min(hough_valid[i].roi.width, hough_valid[i + 1].roi.width);

			int delta = max_width - min_width;
			//delta = abs(delta);
			double diff = double(delta) / double(max_width);

			if (debug)
				cout << "delta = " << delta << " diff = " << diff << "\n";

			if (diff < 0.1) {

				markers_.push_back(hough_valid[i]);
				markers_.push_back(hough_valid[i + 1]);
				break;
			}
		}
	}
	else if (hough_valid.size() == 2){

		if (m_type == markerType::black_circle) {
			m1[0] = hough_valid[0];
			m1[1] = hough_valid[1];
		}
		else {
			m2[0] = hough_valid[0];
			m2[1] = hough_valid[1];
		}
	}
	else {
		//pass
	}
	return;
}


void AUV::estimatePos(Mat &frame, bool draw_perp) {

	if (this->m1.size() == 2 && this->m2.size() == 2) {

		vector<Point2d> corners = {
									Point2d(m1[0].x, m1[0].y),
									Point2d(m1[1].x, m1[1].y),
									Point2d(m2[1].x, m2[1].y),
									Point2d(m2[0].x, m2[0].y)
		};

		//estimatePoseSingleMarkers(corners, markerLen, cMatrix640, distortion640, Rvec, Tvec);

		// Solve for pose
		solvePnP(model_points, corners, cMatrix640, distortion640, this->Rvec, this->Tvec);
		//solvePnP(model_points, corners, camera_matrix, distortion640, Rvec, Tvec);

		//cout << "Rotation Vector " << endl << Rvec << endl;
		//cout << "Translation Vector" << endl << Tvec << endl;

		cout << setprecision(5);

		for (int j = 0; j < Tvec.rows; j++) {
			cout << setw(8) << Tvec.at<double>(j, 0);
		}
		cout << "\n";

		if (draw_perp) {

			vector<Point2d> perpendicular_point2D;
			vector<Point3d> perpendicular_point3D;
				perpendicular_point3D.push_back(Point3d(0, 0, 0));
				perpendicular_point3D.push_back(Point3d(0, 0, 100));

			projectPoints(perpendicular_point3D, Rvec, Tvec, cMatrix640, distortion640, perpendicular_point2D);
			line(frame, perpendicular_point2D[0], perpendicular_point2D[1], cv::Scalar(255, 0, 0), 2);
		}
	}
	else {
		cout << "Less than 4 markers\n";
		cout << m1.size() << " "  << m2.size() << "\n";
	}
}


void AUV::get_orientation(Mat &frame) {

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	vector<Rect> markers1, markers2;

	marker_type_1.detectMultiScale(frame_gray, markers1);
	marker_type_2.detectMultiScale(frame_gray, markers2);


	filter_objects_2(markers1, frame, frame_gray, markerType::black_circle, AUV_sees, false);
	filter_objects_2(markers2, frame, frame_gray, markerType::white_circle, AUV_sees, false);

	//print_objects(m1, "-");

	Mat our_markers = Mat::zeros(frame.size(), CV_8UC1);

	this->rotate_over_normal(frame);
	this->arrange_markers(our_markers, true);
	this->calculate_distance(frame, false);
	////this->calculate_deltas(frame, true);

	this->estimatePos(frame, false);

	AUV_sees = Mat::zeros(frame.size(), CV_8UC1);

	for (int i = 0; i < m1.size(); i++) {
		rectangle(AUV_sees, m1[i].roi, WHT, -1);
	}
	for (int i = 0; i < m2.size(); i++) {
		rectangle(AUV_sees, m2[i].roi, WHT, -1);
	}

	////cout << m1.size() << " " << m2.size() << "\n";
	////imshow("AUV mask", AUV_sees);
	//AUV_sees = AUV_sees & frame_gray;

	//imshow("AUV sees", AUV_sees);
	imshow("our markers", our_markers);

	draw_objects(frame, m1, YEL);
	draw_objects(frame, m2, PNK);
}
