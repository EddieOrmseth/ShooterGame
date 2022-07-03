#include "GreaseCannon.hpp"
#include "Grease.hpp"
#include "MotionObject.hpp"
#include "Util.hpp"

GreaseCannon::GreaseCannon(GameEngine* gameEngine, ShootCapable* shootCapable) : StandardCannon(gameEngine, shootCapable) {
	cooldownMilliseconds = 2000;
}

GameObject* GreaseCannon::getProjectile(Vector2D* position, double angle, Vector2D* addedVel) {
	Grease* grease = new Grease(greaseRadius, Gdiplus::Color(0, 0, 0), greaseMaxMillis, greaseMaxCollisions, greaseTime);
	grease->getMotionObject()->setPosition(position);
	grease->getMotionObject()->getTranslationalVelocity()->setMagTheta(greaseSpeed, angle);
	grease->getMotionObject()->getTranslationalVelocity()->add(addedVel);
	return grease;
}
