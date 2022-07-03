#include "StandardCannon.hpp"
#include <iostream>
#include "ShootCapable.hpp"
#include "BulletObject.hpp"
#include "GameEngine.hpp"
#include "Util.hpp"
#include "InputRegistry.hpp"

StandardCannon::StandardCannon(GameEngine* gameEngine, ShootCapable* shootCapable) {
	this->gameEngine = gameEngine;
	this->shootCapable = shootCapable;
	currentShotTime = std::chrono::high_resolution_clock::now();
	previousShotTime = std::chrono::high_resolution_clock::now();
}

void StandardCannon::registerKeys(InputRegistry* inputRegistry, int keyCode) {
	std::function<void(bool)>* shootButton = new std::function<void(bool)>();
	*shootButton = [this](bool pressed) {
		if (!pressed) {
			return;
		}

		fireShot();
	};
	inputRegistry->addListener(shootButton, keyCode);
}

void StandardCannon::fireShot() {
	if (!shootCapable->canShoot() || !cooldownPassed()) {
		return;
	}
	previousShotTime = std::chrono::high_resolution_clock::now();

	Vector2D* newPosition = shootCapable->getShotPosition();
	double angle = shootCapable->getShotAngle();
	Vector2D* addedVel = shootCapable->getShotAddedVelocity();

	//gameEngine->addObject(new BulletObject(newPosition, angle, addedVel));
	gameEngine->addObject(getProjectile(newPosition, angle, addedVel));
}

bool StandardCannon::cooldownPassed() {
	currentShotTime = std::chrono::high_resolution_clock::now();
	double elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentShotTime - previousShotTime).count();
	return elapsedTime >= cooldownMilliseconds;
}

void StandardCannon::setCooldown(double cooldownMilliseconds) {
	this->cooldownMilliseconds = cooldownMilliseconds;
}

double StandardCannon::getCooldown() {
	return cooldownMilliseconds;
}