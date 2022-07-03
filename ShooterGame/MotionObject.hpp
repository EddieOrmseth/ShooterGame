#pragma once
#include "Greasable.hpp"

class Vector2D;

class MotionObject : public Greasable {

public:

	MotionObject();
	~MotionObject();

	virtual void update(const double time, const double timeSquared);

	void updateTranslation(const double time, const double timeSquared);
	void updateTranslationPositionVelocity(Vector2D* position, Vector2D* velocity, Vector2D* acceleration, double time, double timeSquared);

	void updateRotation(const double time, const double timeSquared);
	void updateRotationalPositionVelocity(double position, double velocity, double acceleration, double time, double timeSquared);

	double getTimeToSpeed(Vector2D* velocity, Vector2D* acceleration, double speed);
	double getTimeToSpeed(double velocity, double acceleration, double speed);

	void setPosition(double x, double y);
	void setPosition(Vector2D* position);
	void setRotation(double rotation);

	void setTranslationalVelocity(double x, double y);
	void setTranslationalVelocity(Vector2D* translationalVelocity);
	void setRotationalVelocity(double rotationalVelocity);

	void setTranslationalAcceleration(double x, double y);
	void setTranslationalAcceleration(Vector2D* translationalAcceleration);
	void setRotationalAcceleration(double rotationalAcceleration);

	void setMaxTranslationalVelocity(double maxTranslationalVelocity);
	void setMaxRotationalVelocity(double maxRotationalVelocity);

	Vector2D* getPosition();
	double getRotation();

	Vector2D* getTranslationalVelocity();
	double getRotationalVelocity();

	Vector2D* getTranslationalAcceleration();
	double getRotationalAcceleration();

	double getMaxTranslationalVelocity();
	double getMaxRotationalVelocity();

	virtual void startGrease(long millisDuration);
	virtual void endGrease();

protected:

	Vector2D* position;
	double rotation;

	Vector2D* translationalVelocity;
	double rotationalVelocity;

	Vector2D* translationalAcceleration;
	double rotationalAcceleration;

	double maxTranslationalVelocity = 0;
	double maxRotationalVelocity = 0;

	double translationalFriction = 0;
	double rotationalFriction = 0;

	Vector2D* addedPosition;
	Vector2D* addedVecloity;

	double previousTranslationalFriction = 0;
	double previousRotationalFriction = 0;

};
