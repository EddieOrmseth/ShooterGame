#include "Grease.hpp"
#include "MotionObject.hpp"
#include "ScreenCircle.hpp"

Grease::Grease(double radius, Gdiplus::Color color, double durationMillis, int maxCollisions, double greaseTime) : GameObject(new MotionObject(), nullptr) {
	this->durationMillis = durationMillis;
	this->maxCollisions = maxCollisions;
	this->greaseTime = greaseTime;
	screenObject = new ScreenCircle(motionObject, radius, color);
	creationTime = std::chrono::high_resolution_clock::now();
	objectType = GameObject::ObjectType::Projectile;
}

Grease::Grease(double radius, Gdiplus::Color color, double durationMillis, double greaseTime) : Grease(radius, color, durationMillis, 0, greaseTime) {

}

Grease::Grease(double radius, Gdiplus::Color color, double greaseTime) : Grease(radius, color, 0, 0, greaseTime) {
	
}

Grease::~Grease() {

}

bool Grease::shouldBeRemoved() {
	return maxCollisions != 0 && collisions >= maxCollisions || durationMillis != 0 && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - creationTime).count() > durationMillis;
}

void Grease::notifyCollision(GameObject::ObjectType objectType, GameObject* object) {
	collisions++;
	if (object->isGreasable()) {
		Greasable* greasable = (Greasable*)object->getMotionObject();
		greasable->startGrease(greaseTime);
	}
}