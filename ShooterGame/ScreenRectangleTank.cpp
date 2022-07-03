#include "ScreenRectangleTank.hpp"
#include "MotionObject.hpp"
#include "Util.hpp"

ScreenRectangleTank::ScreenRectangleTank(MotionObject* motionObject, Vector2D* toCorner, Gdiplus::Color* color, Gdiplus::Color cannonColor, double cannonLength, double cannonWidth) : ScreenRectangle(motionObject, toCorner, color) {
	stripeBrush = new Gdiplus::SolidBrush(cannonColor);
	pen = new Gdiplus::Pen(stripeBrush, cannonWidth);
	p1 = new Gdiplus::Point();
	p2 = new Gdiplus::Point();
	cannonVector = Vector2D::fromPolar(cannonLength, 0);
}

ScreenRectangleTank::~ScreenRectangleTank() {
	delete stripeBrush;
	delete pen;
	delete p1;
	delete p2;
	delete cannonVector;
}

void ScreenRectangleTank::paint(Gdiplus::Graphics* graphics) {
	ScreenRectangle::paint(graphics);

	p1->X = motionObject->getPosition()->getX() + image->GetWidth() / 2.0;
	p1->Y = motionObject->getPosition()->getY() + image->GetHeight() / 2.0;
	cannonVector->setTheta(motionObject->getRotation());
	p2->X = p1->X + cannonVector->getX();
	p2->Y = p1->Y + cannonVector->getY();

	graphics->DrawLine(pen, *p1, *p2);
}