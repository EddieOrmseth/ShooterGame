#include "ScreenCircleTank.hpp"
#include "Util.hpp"
#include "MotionObject.hpp"

ScreenCircleTank::ScreenCircleTank(MotionObject* motionObject, double radius, Gdiplus::Color color, Gdiplus::Color cannonColor, double cannonLength, double cannonWidth) : ScreenCircle(motionObject, radius, color) {
	stripeBrush = new Gdiplus::SolidBrush(cannonColor);
	pen = new Gdiplus::Pen(stripeBrush, cannonWidth);
	p1 = new Gdiplus::Point();
	p2 = new Gdiplus::Point();
	cannonVector = Vector2D::fromPolar(cannonLength, 0);
}

ScreenCircleTank::~ScreenCircleTank() {
	delete stripeBrush;
	delete pen;
	delete p1;
	delete p2;
	delete cannonVector;
}

void ScreenCircleTank::paint(Gdiplus::Graphics* graphics) {
	ScreenCircle::paint(graphics);

	p1->X = motionObject->getPosition()->getX();
	p1->Y = motionObject->getPosition()->getY();
	cannonVector->setTheta(motionObject->getRotation());
	p2->X = p1->X + cannonVector->getX();
	p2->Y = p1->Y + cannonVector->getY();

	graphics->DrawLine(pen, *p1, *p2);
}