#include "BulletObject.hpp"
#include "MotionObject.hpp"
#include "ScreenCircle.hpp"
#include "Util.hpp"
#include "GraphicsEngine.hpp"

BulletObject::BulletObject(Vector2D* position, double angle, Vector2D* addedVel) : GameObject(new MotionObject(), nullptr) {
	motionObject->getPosition()->set(position);
	motionObject->getTranslationalVelocity()->setMagTheta(bulletSpeed, angle);
	motionObject->getTranslationalVelocity()->add(addedVel);

	ScreenCircle* screenCircle = new ScreenCircle(motionObject, bulletRadius, Gdiplus::Color(0, 0, 0));
	screenObject = screenCircle;

	objectType = GameObject::ObjectType::Projectile;
	creationTime = std::chrono::high_resolution_clock::now();
}

bool BulletObject::shouldBeRemoved() {
	auto now = std::chrono::high_resolution_clock::now();
	return hasCollided || std::chrono::duration_cast<std::chrono::milliseconds>(now - creationTime).count() > destroyMilliseconds;
	//return false;
}

void BulletObject::notifyCollision(GameObject::ObjectType objectType, GameObject* object) {
	hasCollided = true;
}
