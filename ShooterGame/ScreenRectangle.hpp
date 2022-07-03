#pragma once
#include "GraphicsEngine.hpp"
#include "ScreenObject.hpp"

class Vector2D;
class Line2D;
class MotionObject;

class ScreenRectangle : public ScreenObject {

public:

	ScreenRectangle(MotionObject* motionObject, Vector2D* toCorner, Gdiplus::Color* color);
	~ScreenRectangle();

	bool contains(Vector2D* point);
	bool contains(double x, double y);

	bool isNotBetweenAny(Vector2D* point);
	bool isNotBetweenAny(double x, double y);

	bool isAboveC1ToC2(Vector2D* point);
	bool isAboveC1ToC2(double x, double y);
	bool isAboveC2ToC3(Vector2D* point);
	bool isAboveC2ToC3(double x, double y);
	bool isAboveC3ToC4(Vector2D* point);
	bool isAboveC3ToC4(double x, double y);
	bool isAboveC4ToC1(Vector2D* point);
	bool isAboveC4ToC1(double x, double y);

	void colorImage();
	void paint(Gdiplus::Graphics* graphics);

	double getImageWidth();
	double getImageHeight();

	MotionObject* getMotionObject();

	Vector2D* getCorner1();
	Vector2D* getCorner2();
	Vector2D* getCorner3();
	Vector2D* getCorner4();

protected:

	void updateCorners();

	MotionObject* motionObject;

	Vector2D* toCorner;
	Vector2D* toCenter;

	Vector2D* c1;
	Vector2D* c2;
	Vector2D* c3;
	Vector2D* c4;

	Line2D* c1Toc2;
	Line2D* c2Toc3;
	Line2D* c3Toc4;
	Line2D* c4Toc1;

	Line2D** positiveLines;
	Line2D** negativeLines;

	Gdiplus::Bitmap* image;
	Gdiplus::Color* carColor;
	Gdiplus::Color* absentColor;

};
