#include <iostream>
#include "ScreenRectangle.hpp"
#include "Util.hpp"
#include "MotionObject.hpp"

ScreenRectangle::ScreenRectangle(MotionObject* motionObject, Vector2D* toCorner, Gdiplus::Color* color) {
	objectType = ObjectType::Rectangle;
	this->motionObject = motionObject;
	this->toCorner = toCorner;
	int size = ceil(toCorner->getMagnitude());
	toCenter = Vector2D::fromCartesian(size, size);

	c1 = Vector2D::fromCartesian(0, 0);
	c2 = Vector2D::fromCartesian(0, 0);
	c3 = Vector2D::fromCartesian(0, 0);
	c4 = Vector2D::fromCartesian(0, 0);

	c1Toc2 = new Line2D(c1, c2);
	c2Toc3 = new Line2D(c2, c3);
	c3Toc4 = new Line2D(c3, c4);
	c4Toc1 = new Line2D(c4, c1);

	positiveLines = new Line2D * [2];
	negativeLines = new Line2D * [2];

	updateCorners();

	image = new Gdiplus::Bitmap(2 * size, 2 * size, PixelFormat32bppARGB);
	carColor = color;
	absentColor = new Gdiplus::Color(0, 0, 0, 0);
}

ScreenRectangle::~ScreenRectangle() {
	delete toCorner;
	delete c1;
	delete c2;
	delete c3;
	delete c4;
}

bool ScreenRectangle::contains(Vector2D* point) {
	return contains(point->getX(), point->getY());
}

bool ScreenRectangle::contains(double x, double y) {
	bool aboveP0 = c1Toc2->isPointAboveOrOn(x, y);
	bool aboveP1 = c3Toc4->isPointAboveOrOn(x, y);
	bool aboveN0 = c2Toc3->isPointAboveOrOn(x, y);
	bool aboveN1 = c4Toc1->isPointAboveOrOn(x, y);

	return aboveP0 != aboveP1 && aboveN0 != aboveN1;
}

bool ScreenRectangle::isNotBetweenAny(Vector2D* point) {
	return isNotBetweenAny(point->getX(), point->getY());
}

bool ScreenRectangle::isNotBetweenAny(double x, double y) {
	bool aboveP0 = c1Toc2->isPointAboveOrOn(x, y);
	bool aboveP1 = c3Toc4->isPointAboveOrOn(x, y);
	bool aboveN0 = c2Toc3->isPointAboveOrOn(x, y);
	bool aboveN1 = c4Toc1->isPointAboveOrOn(x, y);

	return aboveP0 == aboveP1 && aboveN0 == aboveN1;
}

bool ScreenRectangle::isAboveC1ToC2(Vector2D* point) {
	return isAboveC1ToC2(point->getX(), point->getY());
}

bool ScreenRectangle::isAboveC1ToC2(double x, double y) {
	return c1Toc2->isPointAbove(x, y);
}

bool ScreenRectangle::isAboveC2ToC3(Vector2D* point) {
	return isAboveC2ToC3(point->getX(), point->getY());
}

bool ScreenRectangle::isAboveC2ToC3(double x, double y) {
	return c2Toc3->isPointAbove(x, y);
}

bool ScreenRectangle::isAboveC3ToC4(Vector2D* point) {
	return isAboveC3ToC4(point->getX(), point->getY());
}

bool ScreenRectangle::isAboveC3ToC4(double x, double y) {
	return c3Toc4->isPointAbove(x, y);
}

bool ScreenRectangle::isAboveC4ToC1(Vector2D* point) {
	return isAboveC4ToC1(point->getX(), point->getY());
}

bool ScreenRectangle::isAboveC4ToC1(double x, double y) {
	return c4Toc1->isPointAbove(x, y);
}

void ScreenRectangle::updateCorners() {

	double rotation = motionObject->getRotation();

	c1->set(toCorner);
	c1->addTheta(rotation);
	c1->add(toCenter);

	c2->set(toCorner);
	c2->setX(-c2->getX());
	c2->addTheta(rotation);
	c2->add(toCenter);

	c3->set(toCorner);
	c3->setX(-c3->getX());
	c3->setY(-c3->getY());
	c3->addTheta(rotation);
	c3->add(toCenter);

	c4->set(toCorner);
	c4->setY(-c4->getY());
	c4->addTheta(rotation);
	c4->add(toCenter);

	c1Toc2->set(c1, c2);
	c2Toc3->set(c2, c3);
	c3Toc4->set(c3, c4);
	c4Toc1->set(c4, c1);

	//std::cout << c1Toc2->toString() << std::endl << c2Toc3->toString() << std::endl << c3Toc4->toString() << std::endl << c4Toc1->toString() << std::endl;
}

void putLineInCorrectArray(Line2D** positiveLines, Line2D** negativeLines, int* posIndex, int* negIndex, Line2D* line) {
	if (line->getSlope() > 0 || line->isLineVertical()) {
		positiveLines[*posIndex] = line;
		(*posIndex)++;
		std::cout << "Array Put In Positive List" << std::endl;
	}
	else {
		negativeLines[*posIndex] = line;
		(*negIndex)++;
		std::cout << "Array Put In Negative List" << std::endl;
	}
}

void ScreenRectangle::colorImage() {
	updateCorners();

	int totalPixels = image->GetWidth() * image->GetHeight();
	int inside = 0;
	int outside = 0;

	bool aboveP0;
	bool aboveP1;
	bool aboveN0;
	bool aboveN1;
	for (int y = 0; y < image->GetHeight(); y++) {
		for (int x = 0; x < image->GetWidth(); x++) {

			aboveP0 = c1Toc2->isPointAboveOrOn(x, y);
			aboveP1 = c3Toc4->isPointAboveOrOn(x, y);
			aboveN0 = c2Toc3->isPointAboveOrOn(x, y);
			aboveN1 = c4Toc1->isPointAboveOrOn(x, y);

			if (aboveP0 != aboveP1 && aboveN0 != aboveN1) { // Inside Rectangle
				image->SetPixel(x, y, *carColor);
				inside++;
			}
			else { // Outside Rectangle
				image->SetPixel(x, y, *absentColor);
				outside++;
			}

		}
	}

	int sum = inside + outside;

}

void ScreenRectangle::paint(Gdiplus::Graphics* graphics) {
	colorImage();
	graphics->DrawImage(image, (int)motionObject->getPosition()->getX(), (int)motionObject->getPosition()->getY(), image->GetWidth(), image->GetHeight());
}

double ScreenRectangle::getImageWidth() {
	return image->GetWidth();
}

double ScreenRectangle::getImageHeight() {
	return image->GetHeight();
}

MotionObject* ScreenRectangle::getMotionObject() {
	return motionObject;
}

Vector2D* ScreenRectangle::getCorner1() {
	Vector2D* corner = new Vector2D(c1);
	corner->add(motionObject->getPosition());
	return corner;
}

Vector2D* ScreenRectangle::getCorner2() {
	Vector2D* corner = new Vector2D(c2);
	corner->add(motionObject->getPosition());
	return corner;
}

Vector2D* ScreenRectangle::getCorner3() {
	Vector2D* corner = new Vector2D(c3);
	corner->add(motionObject->getPosition());
	return corner;
}

Vector2D* ScreenRectangle::getCorner4() {
	Vector2D* corner = new Vector2D(c4);
	corner->add(motionObject->getPosition());
	return corner;
}
