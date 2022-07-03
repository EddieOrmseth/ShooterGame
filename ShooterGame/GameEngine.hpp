#pragma once
#include <vector>

class GraphicsEngine;
class InputRegistry;
class PhysicsEngine;

class GameObject;

class GameEngine {

public:

	GameEngine();
	~GameEngine();

	void update();

	void addObject(GameObject* object);
	void removeObject(GameObject* object);

	GraphicsEngine* getGraphicsEngine();
	InputRegistry* getInputRegistry();
	PhysicsEngine* getPhysicsEngine();

private:

	GraphicsEngine* graphicsEngine;
	InputRegistry* inputRegistry;
	PhysicsEngine* physicsEngine;

	std::vector<GameObject*>* playerObjects;
	std::vector<GameObject*>* projectiles;

};
