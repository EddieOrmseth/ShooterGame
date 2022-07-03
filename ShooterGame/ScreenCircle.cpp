#include "ScreenCircle.hpp"
#include "Util.hpp"
#include "MotionObject.hpp"

ScreenCircle::ScreenCircle(MotionObject* motionObject, double radius, Gdiplus::Color color) {
	objectType = ObjectType::Circle;
	this->motionObject = motionObject;
	this->radius = radius;
	brush = new Gdiplus::SolidBrush(color);
	rect = new Gdiplus::Rect();
	rect->Width = radius * 2.0;
	rect->Height = radius * 2.0;
}

Vector2D* ScreenCircle::getPosition() {
	return motionObject->getPosition();
}

bool ScreenCircle::contains(Vector2D* point) {
	return contains(point->getX(), point->getY());
}

bool ScreenCircle::contains(double x, double y) {
	return motionObject->getPosition()->getDistanceFrom(x, y) <= radius;
}

double ScreenCircle::getRadius() {
	return radius;
}

MotionObject* ScreenCircle::getMotionObject() {
	return motionObject;
}

void ScreenCircle::paint(Gdiplus::Graphics* graphics) {
	rect->X = motionObject->getPosition()->getX() - rect->Width / 2.0;
	rect->Y = motionObject->getPosition()->getY() - rect->Height / 2.0;

	graphics->FillEllipse(brush, *rect);
}
