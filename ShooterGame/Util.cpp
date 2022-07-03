#include "Util.hpp"
#include "math.h"

Vector2D::Vector2D() {
	x = 0;
	y = 0;
}

Vector2D::Vector2D(Vector2D* v1, Vector2D* v2) {
	set(v1, v2);
}

Vector2D::Vector2D(Vector2D* vector) {
	set(vector);
}

Vector2D::Vector2D(double x, double y) {
	setX(x);
	setY(y);
}

Vector2D* Vector2D::fromCartesian(double x, double y) {
	return new Vector2D(x, y);
}

Vector2D* Vector2D::fromPolar(double radius, double theta) {
	return new Vector2D(radius * cos(theta), radius * sin(theta));
}

void Vector2D::set(Vector2D* vector) {
	setX(vector->getX());
	setY(vector->getY());
}

void Vector2D::set(Vector2D* v1, Vector2D* v2) {
	setX(v2->getX() - v1->getX());
	setY(v2->getY() - v1->getY());
}

const double Vector2D::getX() {
	return x;
}

const double Vector2D::getY() {
	return y;
}

const double Vector2D::getMagnitude() {
	return sqrt(x * x + y * y);
}

const double Vector2D::getTheta() {
	return atan2(y, x);
}

const double Vector2D::getDistanceFrom(Vector2D* point) {
	return getDistanceFrom(point->getX(), point->getY());
}

const double Vector2D::getDistanceFrom(double x, double y) {
	return sqrt((this->x - x)*(this->x - x) + (this->y - y)*(this->y - y));
}

void Vector2D::setX(double x) {
	this->x = x;
}

void Vector2D::setY(double y) {
	this->y = y;
}

void Vector2D::setXY(double x, double y) {
	setX(x);
	setY(y);
}

void Vector2D::setMagnitude(double magnitude) {
	double theta = getTheta();
	setX(magnitude * cos(theta));
	setY(magnitude * sin(theta));
}

void Vector2D::setTheta(double theta) {
	double magnitude = getMagnitude();
	setX(magnitude * cos(theta));
	setY(magnitude * sin(theta));
}

void Vector2D::addTheta(double addedTheta) {
	setTheta(getTheta() + addedTheta);
}

void Vector2D::setMagTheta(double magnitude, double theta) {
	setMagnitude(magnitude);
	theta = magnitude < 0 ? theta + 3.14159265358979323846 : theta;
	setTheta(theta);
}

Vector2D* Vector2D::add(Vector2D* vector) {
	setX(getX() + vector->getX());
	setY(getY() + vector->getY());
	return this;
}

Vector2D* Vector2D::subtract(Vector2D* vector) {
	setX(getX() - vector->getX());
	setY(getY() - vector->getY());
	return this;
}

const double Vector2D::dot(Vector2D* vector) {
	return getX() * vector->getX() + getY() * vector->getY();
}

Vector2D* Vector2D::scale(double scalar) {
	x *= scalar;
	y *= scalar;
	return this;
}

Vector2D* Vector2D::zeroVectorObj = Vector2D::fromCartesian(0, 0);
Vector2D* Vector2D::zeroVector() {
	return Vector2D::zeroVectorObj;
}

const std::string Vector2D::toString() {
	return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

Line2D::Line2D(double yIntercept) {
	this->yIntercept = yIntercept;
	isVertical = true;
	slope = 0;
}

Line2D::Line2D(double slope, double yIntercept) {
	this->slope = slope;
	this->yIntercept = yIntercept;
	isVertical = true;
}

Line2D::Line2D(Vector2D* vector) {
	set(vector);
}

Line2D::Line2D(Vector2D* v1, Vector2D* v2) {
	set(v1, v2);
}

const double Line2D::getSlope() {
	return slope;
}

const double Line2D::getYIntercept() {
	return yIntercept;
}

const bool Line2D::isLineVertical() {
	return isVertical;
}

void Line2D::set(Line2D* line) {
	slope = line->getSlope();
	yIntercept = line->getYIntercept();
	isVertical = line->isLineVertical();
}

void Line2D::set(Vector2D* vector) {
	if (vector->getX() != 0) {
		isVertical = false;
		slope = vector->getY() / vector->getX();
		yIntercept = vector->getY() - (slope * vector->getX());
	}
	else {
		isVertical = true;
		slope == 1;
		yIntercept = 0;
	}
}

void Line2D::set(Vector2D* v1, Vector2D* v2) {
	Vector2D* vector = new Vector2D(v1, v2);
	if (vector->getX() != 0) {
		isVertical = false;
		slope = vector->getY() / vector->getX();
		yIntercept = v1->getY() - (slope * v1->getX());
	}
	else {
		isVertical = true;
		slope = 1;
		yIntercept = v1->getX();
	}
	delete vector;
}

double Line2D::getYForX(double x) {
	return !isVertical ? (x * slope + yIntercept) : yIntercept;
}

double Line2D::getXForY(double y) {
	return slope == 0 ? (y - yIntercept) / slope : yIntercept;
}

const bool Line2D::isOnLine(Vector2D* vector) {
	return isOnLine(vector->getX(), vector->getY());
}

const bool Line2D::isOnLine(double x, double y) {
	return !isVertical ? y == x * slope + yIntercept : x == yIntercept;
}

const bool Line2D::isPointAbove(Vector2D* vector) {
	return isPointAbove(vector->getX(), vector->getY());
}

const bool Line2D::isPointAbove(double x, double y) {
	return !isVertical ? y > x * slope + yIntercept : x > yIntercept;
}

const bool Line2D::isPointAboveOrOn(Vector2D* vector) {
	return isPointAboveOrOn(vector->getX(), vector->getY());
}

const bool Line2D::isPointAboveOrOn(double x, double y) {
	return !isVertical ? y >= x * slope + yIntercept : x >= yIntercept;
}

std::string Line2D::toString() {
	return !isVertical ? "y = " + std::to_string(slope) + " * x + " + std::to_string(yIntercept) : "x = " + std::to_string(yIntercept);
}