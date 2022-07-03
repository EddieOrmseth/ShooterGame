#pragma once
#include "GameObject.hpp"
#include "ShootCapable.hpp"
#include "CarControlledObject.hpp"
#include "ScreenRectangleTank.hpp"

class CarStyleFixedCannon : public GameObject, public ShootCapable {

public:

	CarStyleFixedCannon();
	~CarStyleFixedCannon();

	CarControlledObject* getCarControlledObject();
	ScreenRectangleTank* getScreenCircleTank();

	Vector2D* getShotPosition();
	double getShotAngle();
	Vector2D* getShotAddedVelocity();

	virtual void notifyCollision(GameObject::ObjectType objectType, GameObject* object);

	virtual bool isGreasable();

private:

	double cannonLength = 50;
	Vector2D* cannonPositionVector;

	CarControlledObject* player;
	ScreenRectangleTank* sprite;

};

