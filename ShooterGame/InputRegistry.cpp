#include "InputRegistry.hpp"
#include <string>
#include <utility>

InputRegistry::InputRegistry() {
	inputQueue = new std::queue<KeyInfo*>;
	queueMutex = new std::mutex();

	listeners = new std::unordered_map<int, std::vector<Listener>*>;
	keyStates = new std::unordered_map<int, bool>();

	defaultRegistry = this;
}

InputRegistry::~InputRegistry() {
	while (inputQueue->size() > 0) {
		KeyInfo* info = inputQueue->front();
		inputQueue->pop();
		delete info;
	}
	delete inputQueue;
	delete queueMutex;

	listeners->clear();
	delete listeners;

	keyStates->clear();
	delete keyStates;
}

void InputRegistry::addListener(Listener listener, int keyCode) {
	if (listeners->find(keyCode) == listeners->end()) {
		std::vector<Listener>* keyListener = new std::vector<Listener>();
		keyListener->push_back(listener);
		listeners->emplace(keyCode, keyListener);
		return;
	}

	std::vector<Listener>* keyListeners = listeners->at(keyCode);
	keyListeners->push_back(listener);
}

void InputRegistry::resgisterKeyInput(KeyInfo* info) {
	queueMutex->lock();
	inputQueue->push(info);
	queueMutex->unlock();
}

void InputRegistry::handleAllKeyInput() {
	while (inputQueue->size() > 0) {
		handleKeyInput();
	}
}

void InputRegistry::handleKeyInput() {
	if (inputQueue->size() == 0) {
		return;
	}

	queueMutex->lock();
	KeyInfo* info = inputQueue->front();
	inputQueue->pop();
	queueMutex->unlock();
	handleKeyInput(info);
}

void InputRegistry::handleKeyInput(KeyInfo* info) {

	if (keyStates->find(info->keyCode) == keyStates->end()) {
		keyStates->emplace(info->keyCode, info->pressed);
		updateListeners(info);
		return;
	}

	bool currentState = keyStates->at(info->keyCode);

	if (currentState == info->pressed) {
		updateListeners(info);
		return;
	}
	else {
		keyStates->at(info->keyCode) = info->pressed;
		updateListeners(info);
	}

}

void InputRegistry::updateListeners() {
	if (inputQueue->size() == 0) {
		return;
	}

	KeyInfo* info;
	queueMutex->lock();
	info = inputQueue->front();
	inputQueue->pop();
	queueMutex->unlock();
	updateListeners(info);
	delete info;
}

void InputRegistry::updateListeners(KeyInfo* info) {
	if (listeners->find(info->keyCode) != listeners->end()) {
		std::vector<Listener>* keyListeners = listeners->at(info->keyCode);
		for (int i = 0; i < keyListeners->size(); i++) {
			(*keyListeners->at(i))(info->pressed);
		}
	}
}

void InputRegistry::completelyUpdateListeners() {
	while (inputQueue->size() > 0) {
		updateListeners();
	}
}

bool InputRegistry::readKey(int keyCode) {
	if (keyStates->find(keyCode) == keyStates->end()) {
		return false;
	}

	return keyStates->at(keyCode);
}

InputRegistry* InputRegistry::defaultRegistry;
InputRegistry* InputRegistry::getDefault() {
	return InputRegistry::defaultRegistry;
}
