
### Перевод вектора поворота в углы Эйлера

https://www.learnopencv.com/rotation-matrix-to-euler-angles/

```cpp

cv::aruco::detectMarkers(frame, dictionary, corners, ids);
cv::aruco::drawDetectedMarkers(frame_copy, corners, ids);

cv::aruco::estimatePoseSingleMarkers(corners, 30, cMatrix640, distortion640, rvecs, tvecs);

Rodrigues(rvecs[0], rotMat);
Eul = rotationMatrixToEulerAngles(rotMat);

```


### О системе координат

Для Аруко маркеров относительно левого верхнего угла:

- ось Х (красная) направлена направо,
- ось У (зелёная) - вверх,
- ось Z (синяя) на нас.

Оси камеры совпадают с осями кадра:

- ось Х направо,
- ось У - вниз,
- ось Z - внутрь кадра (от камеры).

