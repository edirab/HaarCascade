#include "AUV.h"

AUV::AUV(string path1, string path2) {

	marker_1_path = path1;
	marker_2_path = path2;

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
void AUV::rotate_over_normal(Mat& frame, vector<Rect> m1, vector<Rect> m2) {

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

void AUV::detect_and_display(Mat frame, int cascadeNum, bool saveFalsePositive = false) {
	//cout << "Inside detect_Display";

	//Mat frame_gray;

	//cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	//equalizeHist(frame_gray, frame_gray);

	////imshow("Grayscale", frame_gray);

	////-- Detect object
	//std::vector<Rect> objects;
	//Scalar* color = new Scalar(255, 0, 0);

	//switch (cascadeNum) {

	//case 1:
	//	marker_type_1.detectMultiScale(frame_gray, objects);
	//	color = new Scalar(255, 0, 255);
	//	break;
	//case 2:
	//	marker_type_2.detectMultiScale(frame_gray, objects);
	//	color = new Scalar(0, 255, 255);
	//	break;
	//default:
	//	cout << "Error in switch-case detecting block \n";
	//	break;
	//}

	//draw_objects(frame, objects, *color);

	////-- Show what you got
	//switch (cascadeNum) {

	//case 1:
	//	imshow("Markers #1 ", frame);
	//	if (objects.size() == 2)
	//		marker1Counter++;

	//	if (objects.size() > 2 && saveFalsePositive) {
	//		//string filename = "../../preparing navigation/false_positive_1/" + to_string(false_positive_counter++) + ".jpg";
	//		string filename = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/false_positive_1/" + to_string(false_positive_counter++) + ".jpg";

	//		imwrite(filename, frame);
	//	}
	//	break;
	//case 2:
	//	imshow("Marker #2", frame);
	//	if (objects.size() == 2)
	//		marker2Counter++;

	//	if (objects.size() > 2 && saveFalsePositive) {
	//		//string filename = "../../preparing navigation/false_positive_2/" + to_string(false_positive_counter++) + ".jpg";
	//		string filename = "E:/University/10sem/nirs/haar_3_4_6/preparing navigation/false_positive_2/" + to_string(false_positive_counter++) + ".jpg";
	//		imwrite(filename, frame);
	//	}
	//	break;
	//default:
	//	cout << "Error in switch-case showing block \n";
	//	break;
	//}
	return;
}

void AUV::calculate_distance(Mat& frame, vector<Rect> m1, vector<Rect> m2, bool debug) {
	
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


void AUV::calculate_deltas(Mat& frame, vector<Rect> m1, vector<Rect> m2, bool debug) {

	return;
}

vector<Rect> AUV::filter_objects_2(vector<Rect> objects, Mat& currentFrame, Mat& frame_gray, int m_type, Mat AUV_sees, bool debug = false) {

	vector<Rect> markers_;
	vector<Rect> hough_valid;
	Mat roi;

	sort(objects.begin(), objects.end(), compar);
	//print_objects(objects);

	if (debug)
		cout << "objects.size() = " << objects.size() << "\n";

	// всё делается за один проход
	for (int i = 0; i < objects.size(); i++) {

		vector<Vec3f> circles;
		roi = frame_gray(objects[i]);
		medianBlur(roi, roi, 5);

		/*
		Ищем все кружочки внутри одного ROI
		*/
		HoughCircles(roi, circles, HOUGH_GRADIENT, 1,
			frame_gray.rows / 16,  // change this value to detect circles with different distances to each other
			100, 30, 0.25 * roi.rows, 0.50 * roi.rows // change the last two parameters
			// (min_radius & max_radius) to detect larger circles
		);

		/*
		Если каскад указал на объект и детектор Хаффа нашёл кружочек, то скорее всего, это то что нужно
		*/
		if (circles.size() == 1) {
			hough_valid.push_back(objects[i]);
		}
		/*
		В одном roi кружочков больше одного. Что странно
		Этот блок практически ничего не делает. За всё тестовое видео сработал 4 раза
		*/
		else if (circles.size() > 1) {

			Mat t;
			if (m_type == 1) {
				t = Marker::get_template_t1(roi.rows, roi.cols);
				absdiff(roi, t, roi);
				int nonZero = countNonZero(roi);
				//cout  << "m1 =" << setw(5) << nonZero << "\n";
			}
			else {
				t = Marker::get_template_t1(roi.rows, roi.cols);
				absdiff(roi, t, roi);
				int nonZero = countNonZero(roi);
				//cout  << " m2 ="<< setw(5) << nonZero << "\n";
			}
		}
		/*
		Ситуация: каскад утверждает, что есть объект, но детектор Хаффа кружочка не нашёл
		вот здесь и можно проверить маской
		*/
		else {
			Mat t;
			

			if (m_type == 1) {
				t = Marker::get_template_t1(roi.rows, roi.cols);
				threshold(roi, roi, 60, 255, THRESH_BINARY);
				//imshow("roi m1 thresholded", roi);

				absdiff(roi, t, roi);
				int nonZero = countNonZero(roi);
				//cout << "m1 = " << setw(7) << nonZero << "\n";
				//fout << "m1 = " << nonZero << "\n";
			}
			else {
				t = Marker::get_template_t1(roi.rows, roi.cols);
				threshold(roi, roi, 200, 255, THRESH_BINARY);
				//imshow("roi m2 thresholded", roi);

				absdiff(roi, t, roi);
				int nonZero = countNonZero(roi);
				//cout << "m2 = " << setw(7) << nonZero << "\n";
				//fout << "m2 = " << nonZero << "\n";
			}
		}
		// конец проверки маской
	}

	if (hough_valid.size() > 2) {
		for (size_t i = 0; i < hough_valid.size() - 1; i++) {

			int max_width = max(hough_valid[i].width, hough_valid[i + 1].width);
			int min_width = min(hough_valid[i].width, hough_valid[i + 1].width);

			int delta = max_width - min_width;
			//delta = abs(delta);
			double diff = double(delta) / double(max_width);

			if (debug)
				cout << "delta = " << delta << " diff = " << diff << "\n";

			if (diff < 0.05) {

				markers_.push_back(hough_valid[i]);
				markers_.push_back(hough_valid[i + 1]);
				break;
			}
		}
	}
	else {
		markers_ = hough_valid;
	}

	for (int i = 0; i < markers_.size(); i++) {
		Point center(markers_[i].x + markers_[i].width / 2, markers_[i].y + markers_[i].height / 2);
		ellipse(currentFrame, center, Size(5, 5), 0, 0, 360, Scalar(0, 0, 255), 4);
	}

	if (debug)
		//print_objects(markers);
	return markers_;
}


void AUV::get_orientation(Mat &frame) {

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	vector<Rect> markers1, markers2;
	//Scalar* color = new Scalar(0, 255, 255); // B G R

	marker_type_1.detectMultiScale(frame_gray, markers1);
	marker_type_2.detectMultiScale(frame_gray, markers2);

	static Mat AUV_sees = Mat::zeros(frame.size(), CV_8UC1);

	vector<Rect> markers1_filtered = filter_objects_2(markers1, frame, frame_gray, 1, AUV_sees, false);
	vector<Rect> markers2_filtered = filter_objects_2(markers2, frame, frame_gray, 2, AUV_sees, false);

	AUV_sees = Mat::zeros(frame.size(), CV_8UC1);

	for (int i = 0; i < markers1_filtered.size(); i++) {
		rectangle(AUV_sees, markers1_filtered[i], WHT, -1);
	}
	for (int i = 0; i < markers2_filtered.size(); i++) {
		rectangle(AUV_sees, markers2_filtered[i], WHT, -1);
	}

	//imshow("AUV mask", AUV_sees);
	AUV_sees = AUV_sees & frame_gray;
	//AUV_sees = AUV_sees.mul(frame);
	imshow("AUV sees", AUV_sees);

	draw_objects(frame, markers1_filtered, YEL);
	draw_objects(frame, markers2_filtered, PNK);


	this->rotate_over_normal(frame, markers1, markers2);
	this->calculate_distance(frame, markers1, markers2, true);
}