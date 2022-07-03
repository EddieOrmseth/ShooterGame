#include "CarControlledObject.hpp"
#include <windows.h>
#include "InputRegistry.hpp"
#include "Util.hpp"

CarControlledObject::CarControlledObject() {
	driveAcceleration = 500;
	usedRotationalAcceleration = 15;

	maxTranslationalVelocity = 500;
	maxRotationalVelocity = 4;

	translationalFriction = 500;
	rotationalFriction = 10;

	goForward = false;
	goBackward = false;
	rotateRight = false;
	rotateLeft = false;

	previousGoForward = false;
	previousGoBackward = false;
	previousRotateRight = false;
	previousRotateLeft = false;
}

CarControlledObject::~CarControlledObject() {

}

void CarControlledObject::registerKeys(InputRegistry* inputRegistry, bool useAWSD) {
	std::function<void(bool)>* forwardHandler = new std::function<void(bool)>();
	*forwardHandler = [this](bool pressed) {
		goForward = pressed;
	};
	inputRegistry->addListener(forwardHandler, useAWSD ? VK_W : VK_UP);

	//std::function<void(bool)>* backwardHandler = new std::function<void(bool)>();
	//*backwardHandler = [this](bool pressed) {
	//	goBackward = pressed;
	//};
	//inputRegistry->addListener(backwardHandler, useAWSD ? VK_S : VK_DOWN);

	std::function<void(bool)>* rotateRightHandler = new std::function<void(bool)>();
	*rotateRightHandler = [this](bool pressed) {
		rotateRight = pressed;
	};
	inputRegistry->addListener(rotateRightHandler, useAWSD ? VK_D : VK_RIGHT);

	std::function<void(bool)>* rotateLeftHandler = new std::function<void(bool)>();
	*rotateLeftHandler = [this](bool pressed) {
		rotateLeft = pressed;
	};
	inputRegistry->addListener(rotateLeftHandler, useAWSD ? VK_A : VK_LEFT);
}

double correctDomain(double angle) {
	double PI2 = 2 * 3.14159265358979323846;
	return fmod(fmod(angle, PI2) + PI2, PI2);
}

void CarControlledObject::update(const double time, const double timeSquared) {

	if (greased()) {
		greaseUpdate(time);
		MotionObject::update(time, timeSquared);
		return;
	}

	bool checkRot = false;

	if (previousGoForward != goForward) {
		if (goForward) {
			
		}
		else {
			
		}
		previousGoForward = goForward;
		checkRot = true;
	}

	if (previousGoBackward != goBackward) {
		if (goBackward) {
			
		}
		else {
			
		}
		previousGoBackward = goBackward;
		checkRot = true;
	}

	if (previousRotateRight != rotateRight) {
		if (rotateRight) {
			//rotationalAcceleration += usedRotationalAcceleration;
		}
		else {
			//rotationalAcceleration -= usedRotationalAcceleration;
		}
		previousRotateRight = rotateRight;
		checkRot = true;
	}

	if (previousRotateLeft != rotateLeft) {
		if (rotateLeft) {
			//rotationalAcceleration -= usedRotationalAcceleration;
		}
		else {
			//rotationalAcceleration += usedRotationalAcceleration;
		}
		previousRotateLeft = rotateLeft;
		checkRot = true;
	}

	if (translationalVelocity->getMagnitude() == 0) {
		rotationalVelocity = 0;
		rotationalAcceleration = 0;
	} else if (rotateRight && rotateLeft) {
		rotationalAcceleration = 0;
	}
	else if (rotateRight) {
		rotationalAcceleration = usedRotationalAcceleration;
	}
	else if (rotateLeft) {
		rotationalAcceleration = -usedRotationalAcceleration;
	}
	else {
		rotationalAcceleration = 0;
	}
	//if (checkRot) {
		
	//}


	double transAcc = 0;
	transAcc += goForward ? driveAcceleration : 0;
	transAcc += goBackward ? -driveAcceleration : 0;
	getTranslationalAcceleration()->setMagTheta(transAcc, getRotation());
	// (abs(correctDomain(getRotation()) - correctDomain(translationalVelocity->getTheta())) < 3.14159265358979323846 / 2.0 ? 0 : 3.14159265358979323846)
	getTranslationalVelocity()->setTheta(getRotation());

	MotionObject::update(time, timeSquared);
}