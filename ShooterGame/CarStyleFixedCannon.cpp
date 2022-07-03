#include "CarStyleFixedCannon.hpp"
#include <iostream>
#include "Util.hpp"

CarStyleFixedCannon::CarStyleFixedCannon() : GameObject(nullptr, nullptr) {
	player = new CarControlledObject();
	motionObject = player;
	sprite = new ScreenRectangleTank(player, Vector2D::fromCartesian(30, 15), new Gdiplus::Color(255, 0, 0, 0), Gdiplus::Color(255, 255, 255), 50, 10);
	screenObject = sprite;
	cannonPositionVector = Vector2D::fromPolar(cannonLength, motionObject->getRotation());
}

CarStyleFixedCannon::~CarStyleFixedCannon() {

}

CarControlledObject* CarStyleFixedCannon::getCarControlledObject() {
	return player;
}

ScreenRectangleTank* CarStyleFixedCannon::getScreenCircleTank() {
	return sprite;
}

Vector2D* CarStyleFixedCannon::getShotPosition() {
	cannonPositionVector->setTheta(motionObject->getRotation());
	Vector2D* cannonEndPos = new Vector2D(cannonPositionVector);
	cannonEndPos->add(motionObject->getPosition());
	cannonEndPos->setX(cannonEndPos->getX() + sprite->getImageWidth() / 2.0);
	cannonEndPos->setY(cannonEndPos->getY() + sprite->getImageHeight() / 2.0);
	return cannonEndPos;
}

double CarStyleFixedCannon::getShotAngle() {
	return motionObject->getRotation();
}

Vector2D* CarStyleFixedCannon::getShotAddedVelocity() {
	return motionObject->getTranslationalVelocity();
}

void CarStyleFixedCannon::notifyCollision(GameObject::ObjectType objectType, GameObject* object) {
	std::cout << "Hit!" << std::endl;
}

bool CarStyleFixedCannon::isGreasable() {
	return true;
}