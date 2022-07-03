#include "SpaceStyleFixedCannon.hpp"
#include "Util.hpp"

SpaceStyleFixedCannon::SpaceStyleFixedCannon() : GameObject(nullptr, nullptr) {
	player = new SpaceStyleControlledObject();
	motionObject = player;
	sprite = new ScreenCircleTank(player, 25, Gdiplus::Color(0, 0, 0), Gdiplus::Color(255, 255, 255), cannonLength, 10);
	screenObject = sprite;
	cannonPositionVector = Vector2D::fromPolar(cannonLength, motionObject->getRotation());
}

SpaceStyleFixedCannon::~SpaceStyleFixedCannon() {

}

SpaceStyleControlledObject* SpaceStyleFixedCannon::getSpaceStyleControlledObject() {
	return player;
}

ScreenCircleTank* SpaceStyleFixedCannon::getScreenCircleTank() {
	return sprite;
}

Vector2D* SpaceStyleFixedCannon::getShotPosition() {
	cannonPositionVector->setTheta(motionObject->getRotation());
	Vector2D* cannonEndPos = new Vector2D(cannonPositionVector);
	cannonEndPos->add(motionObject->getPosition());
	return cannonEndPos;
}

double SpaceStyleFixedCannon::getShotAngle() {
	return motionObject->getRotation();
}

Vector2D* SpaceStyleFixedCannon::getShotAddedVelocity() {
	return motionObject->getTranslationalVelocity();
}

void SpaceStyleFixedCannon::notifyCollision(GameObject::ObjectType objectType, GameObject* object) {
	std::cout << "Hit!" << std::endl;
}

bool SpaceStyleFixedCannon::isGreasable() {
	return true;
}