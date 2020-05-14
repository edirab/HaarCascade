#include "functions.h"

/*
Отобразить объекты из вектора на изображении
*/
void draw_objects(Mat& frame, vector<Rect> objects, Scalar color) {

	for (size_t i = 0; i < objects.size(); i++) {
		//cout << "X = " << objects[i].x << " Y = " << objects[i].y << "\n";
		Point center(objects[i].x + objects[i].width / 2, objects[i].y + objects[i].height / 2);
		ellipse(frame, center, Size(objects[i].width / 2, objects[i].height / 2), 0, 0, 360, color, 2);
	}
}


/*
Вывод в консоль всех параметров прямоугольников
*/
void print_objects(vector<Rect> ob, string title = "Objects") {

	for (int i = 0; i < ob.size(); i++) {
		cout << setw(2) << i << " x: " << setw(5) << ob[i].x << " y: " << setw(5) << ob[i].y << " w: " << setw(5) << ob[i].width << " h:" << setw(5) << ob[i].height << "\n";
	}
	cout << "\n";
}


bool compar(Rect a, Rect b) {
	return (a.width > b.width);
}

vector<Rect> filter_objects(vector<Rect> objects, Mat& currentFrame, bool debug = false) {

	vector<Rect> markers;
	sort(objects.begin(), objects.end(), compar);
	//print_objects(objects);

	if (debug)
		cout << "objects.size() = " << objects.size() << "\n";

	if (objects.size() >= 2) {
		for (size_t i = 0; i < objects.size() - 1; i++) {

			int max_width = max(objects[i].width, objects[i + 1].width);
			int min_width = min(objects[i].width, objects[i + 1].width);

			int delta = max_width - min_width;
			//delta = abs(delta);
			double diff = double(delta) / double(max_width);

			if(debug)
				cout << "delta = " << delta << " diff = " << diff << "\n";

			if (diff < 0.16) {
				markers.push_back(objects[i]);
				markers.push_back(objects[i + 1]);
				break;
			}
		}
	}
	else if (objects.size() < 2 && debug) {
		cout << "Объектов меньше двух! \n";
	}

	for (int i = 0; i < markers.size(); i++) {
		Point center(markers[i].x + markers[i].width / 2, markers[i].y + markers[i].height / 2);
		ellipse(currentFrame, center, Size(5, 5), 0, 0, 360, Scalar(0, 0, 255), 4);
	}
	if (debug)
		print_objects(markers);
	return markers;
}


void calculate_distance(Mat& frame, vector<Rect> m1, vector<Rect> m2, bool debug) {
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

void writeVideo(int frame_width, int frame_height) {
	string abs_path = "E: / University / 10sem / nirs / haar_3_4_6 / preparing navigation / videos / pyramid_test_demo.avi";
	VideoWriter video(abs_path, CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));
}
