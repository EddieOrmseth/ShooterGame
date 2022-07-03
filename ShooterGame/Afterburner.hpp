#pragma once

class MotionObject;
class AfterburnCapable;

class Afterburner {

public:

	Afterburner(double newMaxVel, double newMaxAcc, double maxMilliTime, double refreshRatio, AfterburnCapable* afterburnCapable);
	Afterburner(double newMaxVel, double newMaxAcc, double afterburnCooldownFriction, double maxMilliTime, double refreshRatio, AfterburnCapable* afterburnCapable);

	void startAfterburner();
	void stopAfterburner();

private:

	AfterburnCapable* object;

	double newMaxVel;
	double newMaxAcc;

	double oldMaxVel;
	double oldMaxAcc;

	double afterburnCooldownFriction;

	double maxMilliTime;
	double refreshRate;

	double totalAfterburnMillisLeft;


};
