#pragma once

#include "Clock.hpp"
#include <string>

class ScopedTimer {
public:
	ScopedTimer(const std::string& name);
	~ScopedTimer();

public:
	void start();
	void stop();

private:
	std::string mName;
	Clock::time_point mStart;
};