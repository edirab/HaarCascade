#pragma once



class Marker {

public:
	int x;
	int y;
	enum markerType
	{
		black_circle = 1,
		white_circle
	} t;
	
	Marker(int, int, markerType);
};