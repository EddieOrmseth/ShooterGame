#include "Greasable.hpp"

Greasable::Greasable() {
	greasing = false;
}

Greasable::~Greasable() {

}

void Greasable::startGrease(long millisDuration) {
	this->millisDuration = millisDuration;
	startTime = std::chrono::high_resolution_clock::now();
	greasing = true;
}

void Greasable::greaseUpdate(const double time) {
	auto now = std::chrono::high_resolution_clock::now();
	greasing = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count() < millisDuration;
	if (!greasing) {
		endGrease();
	}
}

bool Greasable::greased() {
	return greasing;
}
