#pragma once
#include <chrono>
#include "GameObject.hpp"
#include "GraphicsEngine.hpp"

class Vector2D;

class Grease : public GameObject {

public:

	Grease(double radius, Gdiplus::Color color, double durationMillis, int maxCollisions, double greaseTime);
	Grease(double radius, Gdiplus::Color color, double durationMillis, double greaseTime);
	Grease(double radius, Gdiplus::Color color, double greaseTime);
	~Grease();

	virtual bool shouldBeRemoved();
	virtual void notifyCollision(GameObject::ObjectType objectType, GameObject* object);

private:

	std::chrono::high_resolution_clock::time_point creationTime;

	double greaseTime;

	double durationMillis;
	int maxCollisions;
	int collisions = 0;

};
