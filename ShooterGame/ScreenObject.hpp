#pragma once
#include "GraphicsEngine.hpp"

class Vector2D;

class ScreenObject : public Paintable {

public:

	enum class ObjectType {
		Circle,
		Rectangle
	};

	virtual bool contains(Vector2D* point) = 0;
	virtual bool contains(double x, double y) = 0;

	ObjectType getObjectType();

protected:

	ObjectType objectType;

};
