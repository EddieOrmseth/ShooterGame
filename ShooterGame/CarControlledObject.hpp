#pragma once
#include "MotionObject.hpp"

class InputRegistry;

class CarControlledObject : public MotionObject {

public:

	CarControlledObject();
	~CarControlledObject();

	void registerKeys(InputRegistry* inputRegistry, bool useAWSD);

	virtual void update(const double time, const double timeSquared);

private:

	double driveAcceleration;
	double usedRotationalAcceleration;

	bool goForward;
	bool goBackward;
	bool rotateRight;
	bool rotateLeft;

	bool previousGoForward;
	bool previousGoBackward;
	bool previousRotateRight;
	bool previousRotateLeft;

};
