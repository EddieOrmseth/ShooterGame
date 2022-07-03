#include <iostream>
#include "ScreenRect.hpp"
#include "Util.hpp"

ScreenRect::ScreenRect(Vector2D* position, double rotation, Vector2D* toCorner) {
	this->position = position;
	this->rotation = rotation;
	this->toCorner = toCorner;
	velocity = Vector2D::fromCartesian(0, 0);
	usedVelocity = Vector2D::fromCartesian(0, 0);
	rotationalVelocity = 0;
	usedRotationalVelocity = 0;
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

	image = new Gdiplus::Bitmap(2 * size, 2 * size, PixelFormat32bppARGB);
	carColor = new Gdiplus::Color(255, 0, 0, 255);
	absentColor = new Gdiplus::Color(0, 0, 0, 0);
}

ScreenRect::~ScreenRect() {
	delete position;
	delete toCorner;
	delete c1;
	delete c2;
	delete c3;
	delete c4;
}

void ScreenRect::updateCorners() {

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

void ScreenRect::colorImage() {
	updateCorners();

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
			}
			else { // Outside Rectangle
				image->SetPixel(x, y, *absentColor);
			}

		}
	}

}

void ScreenRect::paint(Gdiplus::Graphics* graphics) {
	colorImage();
	graphics->DrawImage(image, (int) position->getX(), (int) position->getY(), image->GetWidth(), image->GetHeight());
}

void ScreenRect::setPosition(double x, double y) {
	position->setXY(x, y);
}

void ScreenRect::setPosition(Vector2D* position) {
	setPosition(position->getX(), position->getY());
}

Vector2D* ScreenRect::getPosition() {
	return position;
}

void ScreenRect::setRotation(double rotation) {
	this->rotation = rotation;
}

double ScreenRect::getRotation() {
	return rotation;
}

void ScreenRect::setVelocity(double x, double y) {
	velocity->setXY(x, y);
}

void ScreenRect::setVelocity(Vector2D* velocity) {
	setVelocity(velocity->getX(), velocity->getY());
}

Vector2D* ScreenRect::getVelocity() {
	return velocity;
}

void ScreenRect::setRotationalVelocity(double rotationalVelocity) {
	this->rotationalVelocity = rotationalVelocity;
}

double ScreenRect::getRotationalVelocity() {
	return rotationalVelocity;
}

void ScreenRect::update(double timeDelta) {
	usedVelocity->set(velocity);
	usedVelocity->scale(timeDelta);
	position->add(usedVelocity);
	rotation += rotationalVelocity;
}
