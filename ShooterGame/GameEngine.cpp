#include "GameEngine.hpp"
#include "GameObject.hpp"
#include "GraphicsEngine.hpp"
#include "InputRegistry.hpp"
#include "PhysicsEngine.hpp"

GameEngine::GameEngine() {
	playerObjects = new std::vector<GameObject*>();
	projectiles = new std::vector<GameObject*>();
	inputRegistry = new InputRegistry();
	graphicsEngine = new GraphicsEngine(inputRegistry);
	physicsEngine = new PhysicsEngine();
}

GameEngine::~GameEngine() {
	delete inputRegistry;
	delete graphicsEngine;
	delete physicsEngine;
}

void GameEngine::update() {
	physicsEngine->update(playerObjects, projectiles);

	for (int i = 0; i < playerObjects->size(); i++) {
		if (playerObjects->at(i)->shouldBeRemoved()) {
			graphicsEngine->removePaintable((Paintable*)playerObjects->at(i)->getScreenObject());
			playerObjects->erase(playerObjects->begin() + i);
		}
	}
	for (int i = 0; i < projectiles->size(); i++) {
		if (projectiles->at(i)->shouldBeRemoved()) {
			graphicsEngine->removePaintable((Paintable*)projectiles->at(i)->getScreenObject());
			projectiles->erase(projectiles->begin() + i);
		}
	}
}

void GameEngine::addObject(GameObject* object) {
	if (object->getObjectType() == GameObject::ObjectType::Player) {
		playerObjects->push_back(object);
	}
	else {
		projectiles->push_back(object);
	}
	graphicsEngine->addPaintable((Paintable*)object->getScreenObject());
}

void GameEngine::removeObject(GameObject* object) {
	graphicsEngine->removePaintable((Paintable*)object->getScreenObject());
	std::vector<GameObject*>* objects = object->getObjectType() == GameObject::ObjectType::Player ? playerObjects : projectiles;
	for (int i = 0; i < objects->size(); i++) {
		if (objects->at(i) == object) {
			objects->erase(objects->begin() + i);
		}
	}
}

GraphicsEngine* GameEngine::getGraphicsEngine() {
	return graphicsEngine;
}

InputRegistry* GameEngine::getInputRegistry() {
	return inputRegistry;
}

PhysicsEngine* GameEngine::getPhysicsEngine() {
	return physicsEngine;
}

