#include "Sendable.hpp"
#include <string>
#include "Array.hpp"

Sendable::Sendable(std::string name, int id) {
	this->name = name;
	this->id = id;
}

std::string Sendable::toString() {
	return std::string("Name: " + name + "\tID: " + std::to_string(id));
}

Array<char>* Sendable::toArray() {

	int dataSize = name.size() + 4;

	Array<char>* data = new Array<char>(dataSize);
	for (int i = 0; i < name.size(); i++) {
		(*data)[i] = name[i];
	}

	((int*)(data->getData() + name.size()))[0] = id;

	return data;
}

Sendable* Sendable::FromArray(std::vector<Array<char>*>* data) {
	Array<char>* arr = data->at(0);
	return new Sendable(std::string(arr->getData(), arr->size() - 4), ((int*)(arr->getData() + arr->size() - 4))[0]);
}