#include "RectangleTarget.hpp"
#include <iostream>
#include "MotionObject.hpp"
#include "ScreenRectangle.hpp"
#include "Util.hpp"

RectangleTarget::RectangleTarget() : GameObject(new MotionObject(), nullptr) {
	screenObject = new ScreenRectangle(motionObject, Vector2D::fromCartesian(cornerX, cornerY), new Gdiplus::Color(255, 0, 0));
	objectType = GameObject::ObjectType::Projectile;
}

RectangleTarget::~RectangleTarget() {

}

void RectangleTarget::notifyCollision(GameObject::ObjectType objectType, GameObject* object) {
	//std::cout << "Hit!" << std::endl;
}