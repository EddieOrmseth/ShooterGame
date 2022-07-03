#include "Afterburner.hpp"
#include "AfterburnCapable.hpp"

Afterburner::Afterburner(double newMaxVel, double newMaxAcc, double maxMilliTime, double refreshRatio, AfterburnCapable* afterburnCapable) {
	this->newMaxVel = newMaxVel;
	this->newMaxAcc = newMaxAcc;
	this->maxMilliTime = maxMilliTime;
	this->refreshRate = refreshRate;
	object = afterburnCapable;
	afterburnCooldownFriction = 0;
}

Afterburner::Afterburner(double newMaxVel, double newMaxAcc, double afterburnCooldownFriction, double maxMilliTime, double refreshRatio, AfterburnCapable* afterburnCapable) : Afterburner(newMaxVel, newMaxAcc, maxMilliTime, refreshRatio, afterburnCapable) {
	this->afterburnCooldownFriction = afterburnCooldownFriction;
}

void Afterburner::startAfterburner() {
	object->startAfterburn();
}

void Afterburner::stopAfterburner() {

}
