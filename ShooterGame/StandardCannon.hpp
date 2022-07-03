#pragma once
#include <chrono>

class GameEngine;
class ShootCapable;
class Vector2D;
class InputRegistry;
class GameObject;

class StandardCannon {

public:

	StandardCannon(GameEngine* gameEngine, ShootCapable* shootCapable);

	void registerKeys(InputRegistry* inputRegistry, int keyCode);

	void fireShot();
	virtual GameObject* getProjectile(Vector2D* position, double angle, Vector2D* addedVel) = 0;

	bool cooldownPassed();
	void setCooldown(double cooldownMilliseconds);
	double getCooldown();

protected:

	GameEngine* gameEngine;
	ShootCapable* shootCapable;

	std::chrono::high_resolution_clock::time_point currentShotTime;
	std::chrono::high_resolution_clock::time_point previousShotTime;
	double cooldownMilliseconds = 500;

};
