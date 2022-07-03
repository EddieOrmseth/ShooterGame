#pragma once
#include "GameObject.hpp"

class CircleTarget : public GameObject {

public:

	CircleTarget();
	~CircleTarget();

	void notifyCollision(GameObject::ObjectType objectType, GameObject* object);

private:

	double radius = 150 / 4.0;

};
