#pragma once
#include "GameObject.hpp"
#include "ShootCapable.hpp"
#include "SpaceStyleControlledObject.hpp"
#include "ScreenCircleTank.hpp"

class SpaceStyleFixedCannon : public GameObject, public ShootCapable {

public:

	SpaceStyleFixedCannon();
	~SpaceStyleFixedCannon();

	SpaceStyleControlledObject* getSpaceStyleControlledObject();
	ScreenCircleTank* getScreenCircleTank();

	Vector2D* getShotPosition();
	double getShotAngle();
	Vector2D* getShotAddedVelocity();

	virtual void notifyCollision(GameObject::ObjectType objectType, GameObject* object);

	virtual bool isGreasable();

private:

	double cannonLength = 45;
	Vector2D* cannonPositionVector;

	SpaceStyleControlledObject* player;
	ScreenCircleTank* sprite;

};
