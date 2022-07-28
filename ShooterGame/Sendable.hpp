#pragma once
#include <iostream>
#include <vector>

template <class T>
class Array;

class Sendable {

public:

	Sendable(std::string name, int id);

	std::string toString();

	Array<char>* toArray();
	static Sendable* FromArray(std::vector<Array<char>*>* data);

private:

	std::string name;
	int id;

};
