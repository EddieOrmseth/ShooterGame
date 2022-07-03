#pragma once
#include <string>

class Vector2D {

public:

	Vector2D();
	Vector2D(Vector2D* v1, Vector2D* v2);
	Vector2D(Vector2D* vector);

	static Vector2D* fromCartesian(double x, double y);
	static Vector2D* fromPolar(double radius, double theta);

	void set(Vector2D* vector);
	void set(Vector2D* v1, Vector2D* v2);

	const double getX();
	const double getY();

	const double getMagnitude();
	const double getTheta();

	const double getDistanceFrom(Vector2D* point);
	const double getDistanceFrom(double x, double y);

	void setX(double x);
	void setY(double y);
	void setXY(double x, double y);

	void setMagnitude(double magnitude);
	void setTheta(double theta);
	void addTheta(double addedTheta);
	void setMagTheta(double magnitude, double theta);

	Vector2D* add(Vector2D* vector);
	Vector2D* subtract(Vector2D* vector);
	double const dot(Vector2D* vector);
	Vector2D* scale(double scalar);

	static Vector2D* zeroVector();

	const std::string toString();

private:

	Vector2D(double x, double y);

	static Vector2D* zeroVectorObj;

	double x;
	double y;

};

class Line2D {

public:

	Line2D(double yIntercept);
	Line2D(double slope, double yIntercept);
	Line2D(Vector2D* vector);
	Line2D(Vector2D* v1, Vector2D* v2);

	const double getSlope();
	const double getYIntercept();
	const bool isLineVertical();

	void set(Line2D* line);
	void set(Vector2D* vector);
	void set(Vector2D* v1, Vector2D* v2);

	double getYForX(double x);
	double getXForY(double y);

	const bool isOnLine(Vector2D* vector);
	const bool isOnLine(double x, double y);
	const bool isPointAbove(Vector2D* vector);
	const bool isPointAbove(double x, double y);
	const bool isPointAboveOrOn(Vector2D* vector);
	const bool isPointAboveOrOn(double x, double y);

	std::string toString();

private:

	double slope;
	double yIntercept;
	bool isVertical;

};