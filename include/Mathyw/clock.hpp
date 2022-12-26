#pragma once

#include "./core.hpp"

namespace Mathyw {

// Measure a time duration of specific code (in seconds)
class Clock final
{
public:
	// Construct and restart the time point
	Clock();

	// returns the time passed since the last restart
	float Elapsed() const;

	// restart the timer and returns the time elapsed
	float Restart();

private:
	float timepoint;
};

} // !Mathyw