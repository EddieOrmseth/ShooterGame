#include "MotionObject.hpp"
#include <iostream>
#include "Util.hpp"

double signum(double number) {
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

MotionObject::MotionObject() {
	position = Vector2D::fromCartesian(0, 0);
	rotation = 0;

	translationalVelocity = Vector2D::fromCartesian(0, 0);
	rotationalVelocity = 0;

	translationalAcceleration = Vector2D::fromCartesian(0, 0);
	rotationalAcceleration = 0;

	addedPosition = Vector2D::fromCartesian(0, 0);
	addedVecloity = Vector2D::fromCartesian(0, 0);
}

MotionObject::~MotionObject() {
	delete position;
	delete translationalVelocity;
	delete translationalAcceleration;

	delete addedPosition;
	delete addedVecloity;
}

void MotionObject::update(const double time, const double timeSquared) {
	updateTranslation(time, timeSquared);
	updateRotation(time, timeSquared);
}

void MotionObject::updateTranslation(const double time, const double timeSquared) {
	bool thrusting = translationalAcceleration->getMagnitude() != 0;
	double timeToSpeed;

	if (!thrusting) {
		translationalAcceleration->setMagTheta(-translationalFriction, translationalVelocity->getTheta());
		timeToSpeed = translationalVelocity->getMagnitude() / translationalFriction;
	}
	else {
		timeToSpeed = getTimeToSpeed(translationalVelocity, translationalAcceleration, maxTranslationalVelocity);
	}

	if (timeToSpeed >= time) {
		updateTranslationPositionVelocity(position, translationalVelocity, translationalAcceleration, time, timeSquared);
	}
	else {
		double timeAfterSpeed = time - timeToSpeed;
		updateTranslationPositionVelocity(position, translationalVelocity, translationalAcceleration, timeToSpeed, timeToSpeed * timeToSpeed);
		translationalVelocity->setMagnitude(thrusting ? maxTranslationalVelocity : 0);
		updateTranslationPositionVelocity(position, translationalVelocity, Vector2D::zeroVector(), timeAfterSpeed, timeAfterSpeed * timeAfterSpeed);
	}

	if (!thrusting) {
		translationalAcceleration->setXY(0, 0);
	}
}

void MotionObject::updateTranslationPositionVelocity(Vector2D* position, Vector2D* velocity, Vector2D* acceleration, double time, double timeSquared) {
	addedPosition->set(acceleration);
	addedPosition->scale(timeSquared)->scale(.5);
	position->add(addedPosition);
	addedPosition->set(velocity);
	addedPosition->scale(time);
	position->add(addedPosition);

	addedVecloity->set(acceleration);
	addedVecloity->scale(time);
	velocity->add(addedVecloity);
}

void MotionObject::updateRotation(const double time, const double timeSquared) {
	if (rotationalAcceleration != 0) {
		rotationalVelocity += rotationalAcceleration * time;
	}
	else {
		double newRotSpeed = abs(rotationalVelocity) - time * rotationalFriction;
		newRotSpeed = newRotSpeed > 0 ? newRotSpeed : 0;
		rotationalVelocity = newRotSpeed * signum(rotationalVelocity);
	}
	if (maxRotationalVelocity > 0 && abs(rotationalVelocity) > maxRotationalVelocity) {
		rotationalVelocity = maxRotationalVelocity * signum(rotationalVelocity);
	}
	rotation += rotationalVelocity * time;



	//bool thrusting = rotationalAcceleration != 0;
	//double timeToSpeed;

	//if (!thrusting) {
	//	rotationalAcceleration = -rotationalFriction;
	//	timeToSpeed = rotationalVelocity / rotationalFriction;
	//}
	//else {
	//	timeToSpeed = getTimeToSpeed(rotationalVelocity, rotationalAcceleration, maxRotationalVelocity);
	//}

	//if (timeToSpeed >= time) {
	//	updateRotationalPositionVelocity(rotation, rotationalVelocity, rotationalAcceleration, time, timeSquared);
	//}
	//else {
	//	double timeAfterSpeed = time - timeToSpeed;
	//	updateRotationalPositionVelocity(rotation, rotationalVelocity, rotationalAcceleration, timeToSpeed, timeToSpeed * timeToSpeed);
	//	rotationalVelocity = thrusting ? maxRotationalVelocity : 0;
	//	updateRotationalPositionVelocity(rotation, rotationalVelocity, 0, timeAfterSpeed, timeAfterSpeed * timeAfterSpeed);
	//}

	//if (!thrusting) {
	//	rotationalAcceleration = 0;
	//}
}

void MotionObject::updateRotationalPositionVelocity(double position, double velocity, double acceleration, double time, double timeSquared) {
	rotation += velocity * time + .5 * acceleration * timeSquared;
	rotationalVelocity = acceleration * time;
}

double MotionObject::getTimeToSpeed(Vector2D* velocity, Vector2D* acceleration, double speed) {
	double a = (acceleration->getX() * acceleration->getX()) + (acceleration->getY() * acceleration->getY());
	double b = 2 * ((acceleration->getX() * velocity->getX()) + (acceleration->getY() * velocity->getY()));
	double c = ((velocity->getX() * velocity->getX()) + (velocity->getY() * velocity->getY())) - (speed * speed);

	double discriminant = (b * b) - (4 * a * c);

	if (discriminant == 0) {
		return -b / (2 * a);
	}
	else if (discriminant > 0) {
		double t1 = (-b + sqrt(discriminant)) / (2 * a);
		double t2 = (-b - sqrt(discriminant)) / (2 * a);
		if (t1 > 0 && t2 > 0) {
			return std::min(t1, t2);
		}
		else {
			return std::max(t1, t2);
		}
	}
	else {
		return -1;
	}
}

double MotionObject::getTimeToSpeed(double velocity, double acceleration, double speed) {
	double discriminant = velocity * velocity - 4 * acceleration * -speed;

	if (discriminant == 0) {
		return -velocity / (2 * acceleration);
	}
	else if (discriminant > 0) {
		double t1 = (-velocity + sqrt(discriminant)) / (2 * acceleration);
		double t2 = (-velocity - sqrt(discriminant)) / (2 * acceleration);
		if (t1 > 0 && t2 > 0) {
			return std::min(t1, t2);
		}
		else {
			return std::max(t1, t2);
		}
	}
	else {
		return -1;
	}
}

void MotionObject::setPosition(double x, double y) {
	position->setXY(x, y);
}

void MotionObject::setPosition(Vector2D* position) {
	setPosition(position->getX(), position->getY());
}

void MotionObject::setRotation(double rotation) {
	this->rotation = rotation;
}

void MotionObject::setTranslationalVelocity(double x, double y) {
	translationalVelocity->setXY(x, y);
}

void MotionObject::setTranslationalVelocity(Vector2D* translationalVelocity) {
	setTranslationalVelocity(translationalVelocity->getX(), translationalVelocity->getY());
}

void MotionObject::setRotationalVelocity(double rotationalVelocity) {
	this->rotationalVelocity = rotationalVelocity;
}

void MotionObject::setTranslationalAcceleration(double x, double y) {
	translationalAcceleration->setXY(x, y);
}

void MotionObject::setTranslationalAcceleration(Vector2D* translationalAcceleration) {
	setTranslationalAcceleration(translationalAcceleration->getX(), translationalAcceleration->getY());
}

void MotionObject::setRotationalAcceleration(double rotationalAcceleration) {
	this->rotationalAcceleration = rotationalAcceleration;
}

void MotionObject::setMaxTranslationalVelocity(double maxTranslationalVelocity) {
	this->maxTranslationalVelocity = maxTranslationalVelocity;
}

void MotionObject::setMaxRotationalVelocity(double maxRotationalVelocity) {
	this->maxRotationalVelocity = maxRotationalVelocity;
}

Vector2D* MotionObject::getPosition() {
	return position;
}

double MotionObject::getRotation() {
	return rotation;
}

Vector2D* MotionObject::getTranslationalVelocity() {
	return translationalVelocity;
}

double MotionObject::getRotationalVelocity() {
	return rotationalVelocity;
}

Vector2D* MotionObject::getTranslationalAcceleration() {
	return translationalAcceleration;
}

double MotionObject::getRotationalAcceleration() {
	return rotationalAcceleration;
}

double MotionObject::getMaxTranslationalVelocity() {
	return maxTranslationalVelocity;
}

double MotionObject::getMaxRotationalVelocity() {
	return maxRotationalVelocity;
}

void MotionObject::startGrease(long millisDuration) {
	Greasable::startGrease(millisDuration);
	previousTranslationalFriction = translationalFriction;
	previousRotationalFriction = rotationalFriction;

	millisDuration = millisDuration / 1000.0;

	rotationalAcceleration = -(rotationalVelocity / millisDuration);
	translationalAcceleration->setMagTheta(-(translationalVelocity->getMagnitude() / millisDuration), translationalVelocity->getTheta());
}

void MotionObject::endGrease() {
	translationalFriction = previousTranslationalFriction;
	rotationalFriction = previousRotationalFriction;

	rotationalAcceleration = 0;
	translationalAcceleration->setMagnitude(0);
}
