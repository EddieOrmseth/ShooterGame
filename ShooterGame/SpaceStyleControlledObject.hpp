#pragma once
#include <iostream>
#include "MotionObject.hpp"

class InputRegistry;
class Vector2D;

class SpaceStyleControlledObject : public MotionObject {

public:

	SpaceStyleControlledObject();
	~SpaceStyleControlledObject();

	void registerKeys(InputRegistry* inputRegistry, bool useWASD);

	void update(const double time, const double timeSquared);
	void noMathUpdate(const double time, const double timeSquared);

	virtual void endGrease();

protected:

	double objectThrustAcceleration = 1000;
	double objectRotationalAcceleration = 9;

	bool thrustForward;
	bool rotateRight;
	bool rotateLeft;

	bool previousThrustForward;
	bool previousRotateRight;
	bool previousRotateLeft;

};
