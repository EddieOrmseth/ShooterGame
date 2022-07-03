#pragma once
#include <chrono>
#include <vector>

class ScreenObject;
class ScreenRectangle;
class ScreenCircle;

class GameObject;

class PhysicsEngine {

public:

	void update(std::vector<GameObject*>* playerObjects, std::vector<GameObject*>* projectiles);

	bool detectCollision(ScreenObject* obj1, ScreenObject* obj2);

private:

	std::chrono::high_resolution_clock::time_point previousTime;

	bool detectCollisionRectOnRect(ScreenRectangle* screenRectangle1, ScreenRectangle* screenRectangle2);
	bool detectCollisionCircOnCirc(ScreenCircle* screenCirc1, ScreenCircle* screenCircle2);
	bool detectCollisionRectOnCirc(ScreenRectangle* screenRectangle, ScreenCircle* screenCircle);

};
