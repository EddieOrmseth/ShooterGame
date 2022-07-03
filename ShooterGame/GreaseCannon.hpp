#pragma once
#include "StandardCannon.hpp"

class GameEngine;
class ShootCapable;

class GreaseCannon : public StandardCannon {

public:

	GreaseCannon(GameEngine* gameEngine, ShootCapable* shootCapable);

	GameObject* getProjectile(Vector2D* position, double angle, Vector2D* addedVel);

private:

	double greaseRadius = 15;
	double greaseSpeed = 800;
	double greaseMaxCollisions = 1;
	double greaseMaxMillis = 2500;
	double greaseTime = 2000;

};
