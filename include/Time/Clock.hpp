#pragma once

#include <chrono>

class Clock
{
	private:
		Clock();
		~Clock();

	public:
		using clock = std::chrono::high_resolution_clock;
		using time_point = std::chrono::system_clock::time_point;

		static double getElapsedTime(time_point start) noexcept;
		static double getElapsedTime(time_point start, time_point end) noexcept;

		static double getElapsedTimeMS(time_point start) noexcept;
		static double getElapsedTimeMS(time_point start, time_point end) noexcept;

		static time_point getTime() noexcept;
};