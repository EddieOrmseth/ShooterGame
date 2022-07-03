#include "GameObject.hpp"

GameObject::GameObject(MotionObject* motionObject, ScreenObject* screenObject) {
	this->motionObject = motionObject;
	this->screenObject = screenObject;
	objectType = ObjectType::Player;
}

GameObject::~GameObject() {
	delete motionObject;
	delete screenObject;
}

MotionObject* GameObject::getMotionObject() {
	return motionObject;
}

ScreenObject* GameObject::getScreenObject() {
	return screenObject;
}

bool GameObject::shouldBeRemoved() {
	return false;
}

void GameObject::notifyCollision(GameObject::ObjectType objectType, GameObject* object) {

}

GameObject::ObjectType GameObject::getObjectType() {
	return objectType;
}

bool GameObject::isGreasable() {
	return false;
}
