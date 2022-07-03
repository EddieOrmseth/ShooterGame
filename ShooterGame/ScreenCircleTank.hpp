#pragma once
#include "ScreenCircle.hpp"

class Vector2D;

class ScreenCircleTank : public ScreenCircle {

public:

	ScreenCircleTank(MotionObject* motionObject, double radius, Gdiplus::Color color, Gdiplus::Color cannonColor, double cannonLength, double cannonWidth);
	~ScreenCircleTank();

	virtual void paint(Gdiplus::Graphics* graphics);

private:

	Gdiplus::SolidBrush* stripeBrush;
	Gdiplus::Pen* pen;
	Gdiplus::Point* p1;
	Gdiplus::Point* p2;

	Vector2D* cannonVector;
	
};
