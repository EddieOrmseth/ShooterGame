#include "PhysicsEngine.hpp"
#include "MotionObject.hpp"
#include "ScreenCircle.hpp"
#include "ScreenRectangle.hpp"
#include "GameObject.hpp"
#include "Util.hpp"

void PhysicsEngine::update(std::vector<GameObject*>* playerObjects, std::vector<GameObject*>* projectiles) {
	auto deltaTime = std::chrono::high_resolution_clock::now() - previousTime;
	double deltaTimeCount = std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count() / 1000.0;
	double timeSquared = deltaTimeCount * deltaTimeCount;
	previousTime = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < playerObjects->size(); i++) {
		playerObjects->at(i)->getMotionObject()->update(deltaTimeCount, timeSquared);
	}
	for (int i = 0; i < projectiles->size(); i++) {
		projectiles->at(i)->getMotionObject()->update(deltaTimeCount, timeSquared);
	}

	for (int player = 0; player < playerObjects->size(); player++) {
		for (int proj = 0; proj < projectiles->size(); proj++) {
			if (detectCollision(playerObjects->at(player)->getScreenObject(), projectiles->at(proj)->getScreenObject())) {
				playerObjects->at(player)->notifyCollision(GameObject::ObjectType::Projectile, projectiles->at(proj));
				projectiles->at(proj)->notifyCollision(GameObject::ObjectType::Player, playerObjects->at(player));
			}
		}
	}

	for (int proj1 = 0; proj1 < projectiles->size(); proj1++) {
		for (int proj2 = proj1 + 1; proj2 < projectiles->size(); proj2++) {
			if (detectCollision(projectiles->at(proj1)->getScreenObject(), projectiles->at(proj2)->getScreenObject())) {
				projectiles->at(proj1)->notifyCollision(GameObject::ObjectType::Projectile, projectiles->at(proj2));
				projectiles->at(proj2)->notifyCollision(GameObject::ObjectType::Projectile, projectiles->at(proj1));
			}
		}
	}

}

bool PhysicsEngine::detectCollision(ScreenObject* obj1, ScreenObject* obj2) {
	if (obj1->getObjectType() == ScreenObject::ObjectType::Circle) {
		if (obj2->getObjectType() == ScreenObject::ObjectType::Circle) {
			return detectCollisionCircOnCirc((ScreenCircle*)obj1, (ScreenCircle*)obj2);
		}
		else {
			return detectCollisionRectOnCirc((ScreenRectangle*)obj2, (ScreenCircle*)obj1);
		}
	}
	else {
		if (obj2->getObjectType() == ScreenObject::ObjectType::Circle) {
			return detectCollisionRectOnCirc((ScreenRectangle*)obj1, (ScreenCircle*)obj2);
		}
		else {
			return detectCollisionRectOnRect((ScreenRectangle*)obj1, (ScreenRectangle*)obj2);
		}
	}
}

bool PhysicsEngine::detectCollisionRectOnRect(ScreenRectangle* screenRectangle1, ScreenRectangle* screenRectangle2) {
	Vector2D sr1Corner1 = Vector2D(screenRectangle1->getCorner1());
	Vector2D sr1Corner2 = Vector2D(screenRectangle1->getCorner2());
	Vector2D sr1Corner3 = Vector2D(screenRectangle1->getCorner3());
	Vector2D sr1Corner4 = Vector2D(screenRectangle1->getCorner4());

	Vector2D sr2Corner1 = Vector2D(screenRectangle2->getCorner1());
	Vector2D sr2Corner2 = Vector2D(screenRectangle2->getCorner2());
	Vector2D sr2Corner3 = Vector2D(screenRectangle2->getCorner3());
	Vector2D sr2Corner4 = Vector2D(screenRectangle2->getCorner4());

	sr1Corner1.subtract(screenRectangle2->getMotionObject()->getPosition());
	sr1Corner2.subtract(screenRectangle2->getMotionObject()->getPosition());
	sr1Corner3.subtract(screenRectangle2->getMotionObject()->getPosition());
	sr1Corner4.subtract(screenRectangle2->getMotionObject()->getPosition());

	sr2Corner1.subtract(screenRectangle1->getMotionObject()->getPosition());
	sr2Corner2.subtract(screenRectangle1->getMotionObject()->getPosition());
	sr2Corner3.subtract(screenRectangle1->getMotionObject()->getPosition());
	sr2Corner4.subtract(screenRectangle1->getMotionObject()->getPosition());


	return screenRectangle1->contains(&sr2Corner1) ||
		screenRectangle1->contains(&sr2Corner2) ||
		screenRectangle1->contains(&sr2Corner3) ||
		screenRectangle1->contains(&sr2Corner4) ||
		screenRectangle2->contains(&sr1Corner1) ||
		screenRectangle2->contains(&sr1Corner2) ||
		screenRectangle2->contains(&sr1Corner3) ||
		screenRectangle2->contains(&sr1Corner4);
}

bool PhysicsEngine::detectCollisionCircOnCirc(ScreenCircle* screenCircle1, ScreenCircle* screenCircle2) {
	return screenCircle1->getPosition()->getDistanceFrom(screenCircle2->getPosition()) < (screenCircle1->getRadius() + screenCircle2->getRadius());
}

double getDistanceFromLine(Vector2D* lineP1, Vector2D* lineP2, Vector2D* point) {
	double diff = abs((lineP2->getX() - lineP1->getX())*(lineP1->getY() - point->getY()) - (lineP2->getY() - lineP1->getY())*(lineP1->getX() - point->getX())) / lineP1->getDistanceFrom(lineP2);
	return diff;
}

bool PhysicsEngine::detectCollisionRectOnCirc(ScreenRectangle* screenRectangle, ScreenCircle* screenCircle) {

	Vector2D vec = Vector2D(screenCircle->getMotionObject()->getPosition());
	vec.subtract(screenRectangle->getMotionObject()->getPosition());
	
	if (screenRectangle->contains(&vec)) {
		return true;
	}

	bool aboveP0 = screenRectangle->isAboveC1ToC2(&vec);
	bool aboveP1 = screenRectangle->isAboveC3ToC4(&vec);
	bool aboveN0 = screenRectangle->isAboveC2ToC3(&vec);
	bool aboveN1 = screenRectangle->isAboveC4ToC1(&vec);

	if (aboveP0 == aboveP1 && aboveN0 == aboveN1) {
		if (screenRectangle->getCorner1()->getDistanceFrom(screenCircle->getPosition()) < screenCircle->getRadius()) {
			return true;
		}
		if (screenRectangle->getCorner2()->getDistanceFrom(screenCircle->getPosition()) < screenCircle->getRadius()) {
			return true;
		}
		if (screenRectangle->getCorner3()->getDistanceFrom(screenCircle->getPosition()) < screenCircle->getRadius()) {
			return true;
		}
		if (screenRectangle->getCorner4()->getDistanceFrom(screenCircle->getPosition()) < screenCircle->getRadius()) {
			return true;
		}
	}
	else {
		if (aboveP0 == aboveP1) {
			if (getDistanceFromLine(screenRectangle->getCorner1(), screenRectangle->getCorner2(), screenCircle->getPosition()) < screenCircle->getRadius()) {
				return true;
			}
			if (getDistanceFromLine(screenRectangle->getCorner3(), screenRectangle->getCorner4(), screenCircle->getPosition()) < screenCircle->getRadius()) {
				return true;
			}
		}
		else {
			if (getDistanceFromLine(screenRectangle->getCorner2(), screenRectangle->getCorner3(), screenCircle->getPosition()) < screenCircle->getRadius()) {
				return true;
			}
			if (getDistanceFromLine(screenRectangle->getCorner4(), screenRectangle->getCorner1(), screenCircle->getPosition()) < screenCircle->getRadius()) {
				return true;
			}
		}
	}

	return false;
}
