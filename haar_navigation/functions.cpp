#include "functions.h"


/*
Точками обозначаем сами центры кружочков
*/
void draw_configuration(Mat& frame, vector<Marker> m1, vector<Marker> m2) {

	for (int i = 0; i < m1.size(); i++) {
		circle(frame, Point(m1[i].x, m1[i].y), 4, WHT, -1);
	}
	for (int i = 0; i < m2.size(); i++) {
		circle(frame, Point(m2[i].x, m2[i].y), 4, WHT, -1);
	}
}


/*
Отобразить объекты из вектора на изображении
*/
void draw_objects(Mat& frame, vector<Rect> objects, Scalar color) {

	for (size_t i = 0; i < objects.size(); i++) {
		//cout << "X = " << objects[i].x << " Y = " << objects[i].y << "\n";
		//Point center(objects[i].x + objects[i].width / 2, objects[i].y + objects[i].height / 2);
		//ellipse(frame, center, Size(objects[i].width / 2, objects[i].height / 2), 0, 0, 360, color, 2);
		rectangle(frame, objects[i], color);
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
/*
	Старая функция фильтрации

	Идея для фильтрации: не нужно никакое преобразование Хаффа, оно медленное
	Можно воспользоваться априорной информацией о форме и типе маркеров:
	Взять логическое "И" ака свёртку с шаблоном
*/
vector<Rect> filter_objects(vector<Rect> objects, Mat& currentFrame, Mat& frame_gray, int m_type, bool debug = false) {

	vector<Rect> markers;
	sort(objects.begin(), objects.end(), compar);
	//print_objects(objects);

	if (debug)
		cout << "objects.size() = " << objects.size() << "\n";



	//vector<Vec3f> circles;
	//Mat roi;

	//for (int i = 0; i < objects.size(); i++) {

	//	roi = frame_gray(objects[i]);
	//		

	//	HoughCircles(roi, circles, HOUGH_GRADIENT, 1,
	//	frame_gray.rows / 16,  // change this value to detect circles with different distances to each other
	//	100, 30, 5, 30 // change the last two parameters
	//	// (min_radius & max_radius) to detect larger circles
	//	);

	//}

	////imshow("roi show", roi);


	//for (size_t i = 0; i < circles.size(); i++)
	//{
	//	Vec3i c = circles[i];
	//	Point center = Point(c[0], c[1]);
	//	// circle center
	//	//circle(currentFrame, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
	//	// circle outline
	//	int radius = c[2];
	//	circle(currentFrame, center, radius, RED, 2, LINE_AA);
	//}


	for (size_t i = 0; i < objects.size() - 1; i++) {

		int max_width = max(objects[i].width, objects[i + 1].width);
		int min_width = min(objects[i].width, objects[i + 1].width);

		int delta = max_width - min_width;
		//delta = abs(delta);
		double diff = double(delta) / double(max_width);

		if(debug)
			cout << "delta = " << delta << " diff = " << diff << "\n";

		if (diff < 0.05) {

			Mat roi1 = frame_gray(objects[i]);
			//Mat roi2 = currentFrame(objects[i+1]);

			

			//Mat f1 = Mat::zeros(currentFrame.rows, currentFrame.cols, CV_8UC1);
			//Mat f2 = Mat::zeros(currentFrame.rows, currentFrame.cols, CV_8UC1);
			

			if (m_type == 1) {
				//roi1.copyTo(f1);
				//imshow("roi1", f1);
				Mat s;
				subtract(Marker::get_template_t1(roi1.rows, roi1.cols), roi1, s);
				int su = sum(s)[0];
				cout << "Sum = " << su << "\n";
			}
			else {
				//roi1.copyTo(f2);
				//imshow("roi2", f2);
				Mat s;
				subtract(Marker::get_template_t2(roi1.rows, roi1.cols), roi1, s);
				int su = sum(s)[0];
				cout << "Sum = " << su << "\n";
			}

			markers.push_back(objects[i]);
			markers.push_back(objects[i + 1]);
			break;
		}
	}
	


	for (int i = 0; i < markers.size(); i++) {
		Point center(markers[i].x + markers[i].width / 2, markers[i].y + markers[i].height / 2);
		ellipse(currentFrame, center, Size(5, 5), 0, 0, 360, Scalar(0, 0, 255), 4);
	}

	if (debug)
		print_objects(markers);
	return markers;
}


void writeVideo(int frame_width, int frame_height) {
	string abs_path = "E: / University / 10sem / nirs / haar_3_4_6 / preparing navigation / videos / pyramid_test_demo.avi";
	VideoWriter video(abs_path, CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));
}
