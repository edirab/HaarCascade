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
}

AUV ::~AUV() {

}


/*
	Определяет наклон камеры по крену (ну почти)
	в диапазоне от -90 до +90 градусов
*/
void AUV::rotate_over_normal(Mat& frame, vector<Rect> m1, vector<Rect> m2) {

	//double x1, x2 = 0;
	//double y1, y2 = 0;

	double delta_x, delta_y = 0;
	double alpha = 0;

	//Scalar* color = new Scalar(0, 255, 255); // B G R

	//model_cascade.detectMultiScale(frame_gray, objs);
	//model_cascade_2.detectMultiScale(frame_gray, objs);

	draw_objects(frame, m1, YEL);
	draw_objects(frame, m2, PNK);

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
			alpha = -atan(delta_y / delta_x);
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
	double upper = 0, lower = 0;
	if (m1.size() == 2) {
		//Point a, b;
		upper = sqrt(pow(abs(m1[0].x - m1[1].x), 2) + pow(abs(m1[0].y - m1[1].y), 2));
	}

	if (m2.size() == 2) {
		lower = sqrt(pow(abs(m2[0].x - m2[1].x), 2) + pow(abs(m2[0].y - m2[1].y), 2));
	}

	if (debug) {
		cout << "upper = " << upper << " lower = " << lower << "\n";

		ostringstream strstream;
		//strstream << setprecision(0);
		strstream << "Dist px ";
		strstream << setw(3) << int(upper) << " " << setw(3) << int(lower);

		String text(strstream.str());
		//putText(frame, text, Point(10, 400), 0, 1, Scalar(255, 255, 255), 2);
		putText(frame, text, Point(10, 400), 0, 1, Scalar(30, 30, 30), 2);
	}
}

void AUV::get_orientation(Mat &frame) {

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	vector<Rect> markers1, markers2;
	//Scalar* color = new Scalar(0, 255, 255); // B G R

	marker_type_1.detectMultiScale(frame_gray, markers1);
	marker_type_2.detectMultiScale(frame_gray, markers2);

	markers1 = filter_objects(markers1, frame, frame_gray, false);
	markers2 = filter_objects(markers2, frame, frame_gray, false);

	this->rotate_over_normal(frame, markers1, markers2);
	this->calculate_distance(frame, markers1, markers2, true);
}