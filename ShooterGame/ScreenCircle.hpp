#pragma once
#include "ScreenObject.hpp"

class Vector2D;
class MotionObject;

class ScreenCircle : public ScreenObject {

public:

	ScreenCircle(MotionObject* motionObject, double radius, Gdiplus::Color color);

	Vector2D* getPosition();

	bool contains(Vector2D* point);
	bool contains(double x, double y);

	double getRadius();

	MotionObject* getMotionObject();

	virtual void paint(Gdiplus::Graphics* graphics);

protected:

	MotionObject* motionObject;

	double radius;

	Gdiplus::SolidBrush* brush;
	Gdiplus::Rect* rect;

};
