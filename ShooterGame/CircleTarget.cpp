#include "CircleTarget.hpp"
#include <iostream>
#include "MotionObject.hpp"
#include "ScreenCircle.hpp"

CircleTarget::CircleTarget() : GameObject(new MotionObject(), nullptr) {
	screenObject = new ScreenCircle(motionObject, radius, Gdiplus::Color(255, 0, 0));
	objectType = GameObject::ObjectType::Projectile;
}

CircleTarget::~CircleTarget() {

}

void CircleTarget::notifyCollision(GameObject::ObjectType objectType, GameObject* object) {
	//std::cout << "Hit!" << std::endl;
}
