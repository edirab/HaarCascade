
### Перевод вектора поворота в углы Эйлера

https://www.learnopencv.com/rotation-matrix-to-euler-angles/

		cv::aruco::detectMarkers(frame, dictionary, corners, ids);
		cv::aruco::drawDetectedMarkers(frame_copy, corners, ids);

		cv::aruco::estimatePoseSingleMarkers(corners, 30, cMatrix640, distortion640, rvecs, tvecs);

		Rodrigues(rvecs[0], rotMat);
		Eul = rotationMatrixToEulerAngles(rotMat);
