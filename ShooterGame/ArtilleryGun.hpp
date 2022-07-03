#pragma once
#include "StandardCannon.hpp"

class GameEngine;
class ShootCapable;

class ArtilleryGun : public StandardCannon {

public:

	ArtilleryGun(GameEngine* gameEngine, ShootCapable* shootCapable);
	
	GameObject* getProjectile(Vector2D* position, double angle, Vector2D* addedVel);

private:



};
