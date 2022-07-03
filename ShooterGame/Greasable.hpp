#pragma once
#include <chrono>

class Greasable {

public:

	Greasable();
	~Greasable();

	virtual void startGrease(long millisDuration);
	virtual void greaseUpdate(const double time);
	virtual void endGrease() = 0;

	bool greased();

private:

	std::chrono::high_resolution_clock::time_point startTime;
	long millisDuration;
	bool greasing;

};
