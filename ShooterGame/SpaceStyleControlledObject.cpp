#include "SpaceStyleControlledObject.hpp"
#include <functional>
#include <windows.h>
#include "Util.hpp"
#include "InputRegistry.hpp"

double signum1(double number) {
	if (number < 0) {
		return -1.0;
	}
	else if (number > 0) {
	return 1.0;
	}
	else {
	return 0.0;
	}
}

SpaceStyleControlledObject::SpaceStyleControlledObject() : MotionObject() {
	thrustForward = false;
	rotateRight = false;
	rotateLeft = false;

	previousThrustForward = false;
	previousRotateRight = false;
	previousRotateLeft = false;

	maxTranslationalVelocity = 500;
	maxRotationalVelocity = 4;

	translationalFriction = 300;
	rotationalFriction = 5;
}

SpaceStyleControlledObject::~SpaceStyleControlledObject() {

}

void SpaceStyleControlledObject::registerKeys(InputRegistry* inputRegistry, bool useWASD) {
	std::function<void(bool)>* thrustHandler = new std::function<void(bool)>();
	*thrustHandler = [this](bool pressed) {
		thrustForward = pressed;
	};
	inputRegistry->addListener(thrustHandler, useWASD ? VK_W : VK_UP);

	std::function<void(bool)>* rotateRightHandler = new std::function<void(bool)>();
	*rotateRightHandler = [this](bool pressed) {
		rotateRight = pressed;
	};
	inputRegistry->addListener(rotateRightHandler, useWASD ? VK_D : VK_RIGHT);

	std::function<void(bool)>* rotateLeftHandler = new std::function<void(bool)>();
	*rotateLeftHandler = [this](bool pressed) {
		rotateLeft = pressed;
	};
	inputRegistry->addListener(rotateLeftHandler, useWASD ? VK_A : VK_LEFT);
}

void SpaceStyleControlledObject::update(const double time, const double timeSquared) {

	if (greased()) {
		greaseUpdate(time);
		MotionObject::update(time, timeSquared);
		return;
	}

	if (previousThrustForward != thrustForward) {
		if (thrustForward) {

		}
		else {
			setTranslationalAcceleration(0, 0);
		}
		previousThrustForward = thrustForward;
	}
	if (previousRotateRight != rotateRight) {
		if (rotateRight) {
			setRotationalAcceleration(getRotationalAcceleration() + objectRotationalAcceleration);
		}
		else {
			setRotationalAcceleration(getRotationalAcceleration() - objectRotationalAcceleration);
			//setRotationalAcceleration(0);
			//setRotationalVelocity(0);
		}
		previousRotateRight = rotateRight;
	}
	if (previousRotateLeft != rotateLeft) {
		if (rotateLeft) {
			setRotationalAcceleration(getRotationalAcceleration() - objectRotationalAcceleration);
		}
		else {
			setRotationalAcceleration(getRotationalAcceleration() + objectRotationalAcceleration);
			//setRotationalAcceleration(0);
			//setRotationalVelocity(0);
		}
		previousRotateLeft = rotateLeft;
	}

	if (thrustForward) {
		getTranslationalAcceleration()->setMagTheta(objectThrustAcceleration, getRotation());
	}

	noMathUpdate(time, timeSquared);
}

void SpaceStyleControlledObject::noMathUpdate(const double time, const double timeSquared) {
	if (translationalAcceleration->getMagnitude() != 0) {
		addedVecloity->set(translationalAcceleration);
		addedVecloity->scale(time);
		translationalVelocity->add(addedVecloity);
	}
	else {
		double transSpeed = translationalVelocity->getMagnitude();
		double newTransSpeed = transSpeed - time * translationalFriction;
		newTransSpeed = newTransSpeed > 0 ? newTransSpeed : 0;
		translationalVelocity->setMagnitude(newTransSpeed);
	}

	if (maxTranslationalVelocity > 0 && translationalVelocity->getMagnitude() > maxTranslationalVelocity) {
		translationalVelocity->setMagnitude(maxTranslationalVelocity);
	}

	addedPosition->set(translationalVelocity);
	addedPosition->scale(time);
	position->add(addedPosition);


	updateRotation(time, timeSquared);

	//if (rotationalAcceleration != 0) {
	//	rotationalVelocity += rotationalAcceleration * time;
	//}
	//else {
	//	double newRotSpeed = abs(rotationalVelocity) - time * rotationalFriction;
	//	newRotSpeed = newRotSpeed > 0 ? newRotSpeed : 0;
	//	rotationalVelocity = newRotSpeed * signum1(rotationalVelocity);
	//}
	//if (maxRotationalVelocity > 0 && abs(rotationalVelocity) > maxRotationalVelocity) {
	//	rotationalVelocity = maxRotationalVelocity * signum1(rotationalVelocity);
	//}
	//rotation += rotationalVelocity * time;
}

void SpaceStyleControlledObject::endGrease() {
	MotionObject::endGrease();
	rotationalAcceleration = 0;
	if (rotateRight) {
		rotationalAcceleration += objectRotationalAcceleration;
	}
	if (rotateLeft) {
		rotationalAcceleration -= objectRotationalAcceleration;
	}
	previousRotateRight = rotateRight;
	previousRotateLeft = rotateLeft;
}