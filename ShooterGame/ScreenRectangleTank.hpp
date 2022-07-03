#pragma once
#include "ScreenRectangle.hpp"

class ScreenRectangleTank : public ScreenRectangle {

public:

	ScreenRectangleTank(MotionObject* motionObject, Vector2D* toCorner, Gdiplus::Color* color, Gdiplus::Color cannonColor, double cannonLength, double cannonWidth);
	~ScreenRectangleTank();

	virtual void paint(Gdiplus::Graphics* graphics);

private:

	Gdiplus::SolidBrush* stripeBrush;
	Gdiplus::Pen* pen;
	Gdiplus::Point* p1;
	Gdiplus::Point* p2;

	Vector2D* cannonVector;

};

