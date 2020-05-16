#include "AUV.h"

AUV::AUV() {

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