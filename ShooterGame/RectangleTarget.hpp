#pragma once
#include "GameObject.hpp"

class RectangleTarget : public GameObject {

public:

	RectangleTarget();
	~RectangleTarget();

	void notifyCollision(GameObject::ObjectType objectType, GameObject* object);

private:

	double cornerX = 50;
	double cornerY = 50;

};
