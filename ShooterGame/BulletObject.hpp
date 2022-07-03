#pragma once
#include <chrono>
#include "GameObject.hpp"

class Vector2D;

class BulletObject : public GameObject {

public:

	BulletObject(Vector2D* position, double angle, Vector2D* addedVel);

	bool shouldBeRemoved();
	void notifyCollision(GameObject::ObjectType objectType, GameObject* object);

private:

	std::chrono::high_resolution_clock::time_point creationTime;
	double destroyMilliseconds = 2500;

	double bulletSpeed = 1000;
	double bulletRadius = 5;

	bool hasCollided = false;

};
