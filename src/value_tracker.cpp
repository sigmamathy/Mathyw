#include <Mathyw/value_tracker.hpp>
#include <Mathyw/numeric.hpp>

namespace Mathyw {

ValueTracker::ValueTracker(float init)
	: value(init), now(0.0f), length(0.0f)
{
	tasklist.reserve(10);
}

void ValueTracker::Set(float target, float duration, EasingFunction const& easing)
{
	MATHYW_ASSERT(duration >= 0.0f,
		"The \"duration\" parameter of \"ValueTracker::Set\" must be non-negative");
	tasklist.emplace_back(target, 0.0f, length, duration, easing, true);
	length += duration;
}

void ValueTracker::Set(float target, EasingFunction const& easing)
{
	Set(target, 1.0f, easing);
}

void ValueTracker::Wait(float duration)
{
	MATHYW_ASSERT(duration >= 0.0f,
		"The \"duration\" parameter of \"ValueTracker::Wait\" must be non-negative");
	length += duration;
}

void ValueTracker::Update(float elapsed, bool loop)
{
	for (auto& task : tasklist)
	{
		if (now < task.begin
			|| (!task.first_call && now > task.begin + task.length))
			continue;
		if (task.first_call) task.init_value = value;
		float t = now - task.begin;
		t = t <= task.length ? (t / task.length) : 1.0f;
		float res = task.easefn(t);
		value = task.init_value + res * (task.target - task.init_value);
		task.first_call = false;
	}

	now += elapsed;
	if (loop && now > length)
	{
		now = 0.0f;
		for (auto& task : tasklist)
			task.first_call = true;
	}
}

float ValueTracker::Get() const
{
	return value;
}

float Linear(float x)
{
	return x;
}

float EaseInSine(float x)
{
	return 1 - std::cos(x * constant::Pi / 2);
}

float EaseInCubic(float x)
{
	return Power(x, 3u);
}

float EaseInQuint(float x)
{
	return Power(x, 5u);
}

float EaseInCirc(float x)
{
	return 1 - std::sqrt(1 - Power(x, 2u));
}

float EaseInElastic(float x)
{
	constexpr float c4 = 2.0f * constant::Pi / 3;
	return x == 0.0f || x == 1.0f ? x : -Power(2, 10 * x - 10) * std::sin((x * 10 - 10.75f) * c4);
}

float EaseInBack(float x)
{
	constexpr float c1 = 1.70158f, c3 = c1 + 1;
	return c3 * Power(x, 3) - c1 * x * x;
}

float EaseInBounce(float x)
{
	return 1 - EaseOutBounce(1 - x);
}

float EaseOutSine(float x)
{
	return std::sin(x * constant::Pi / 2);
}

float EaseOutCubic(float x)
{
	return 1 - Power(1 - x, 3u);
}

float EaseOutQuint(float x)
{
	return 1 - Power(1 - x, 5u);
}

float EaseOutCirc(float x)
{
	return std::sqrt(1 - Power(x - 1, 2u));
}

float EaseOutElastic(float x)
{
	constexpr float c4 = 2 * constant::Pi / 3;
	return x == 0.0f || x == 1.0f
		? x
		: Power(2, -10 * x) * std::sin((x * 10 - 0.75f) * c4) + 1;
}

float EaseOutBack(float x)
{
	constexpr float c1 = 1.70158f, c3 = c1 + 1;
	return 1 + c3 * Power(x - 1, 3u) + c1 * Power(x - 1, 2u);
}

float EaseOutBounce(float x)
{
	constexpr float n1 = 7.5625f, d1 = 2.75f;
	if (x < 1 / d1) return n1 * x * x;
	if (x < 2 / d1) return n1 * (x -= 1.5f / d1) * x + 0.75f;
	if (x < 2.5 / d1) return n1 * (x -= 2.25f / d1) * x + 0.9375f;
	return n1 * (x -= 2.625f / d1) * x + 0.984375f;
}

float EaseInOutSine(float x)
{
	return -(std::cos(constant::Pi * x) - 1) / 2;
}

float EaseInOutCubic(float x)
{
	return x < 0.5f ? 4 * Power(x, 3u) : 1 - Power(-2 * x + 2, 3u) / 2;
}

float EaseInOutQuint(float x)
{
	return x < 0.5f ? 16 * Power(x, 5u) : 1 - Power(-2 * x + 2, 5) / 2;
}

float EaseInOutCirc(float x)
{
	return x < 0.5f
		? (1 - std::sqrt(1 - Power(2 * x, 2u))) / 2
		: (std::sqrt(1 - Power(-2 * x + 2, 2u)) + 1) / 2;
}

float EaseInOutElastic(float x)
{
	constexpr float c5 = 2 * constant::Pi / 4.5f;
	if (x == 0.0f || x == 1.0f) return x;
	return x < 0.5f
		? -(Power(2, 20 * x - 10) * std::sin((20 * x - 11.125f) * c5)) / 2
		: (Power(2, -20 * x + 10) * std::sin((20 * x - 11.125f) * c5)) / 2 + 1;
}

float EaseInOutBack(float x)
{
	constexpr float c2 = 1.70158f * 1.525f;
	return x < 0.5f
		? (Power(2 * x, 2u) * ((c2 + 1) * 2 * x - c2)) / 2
		: (Power(2 * x - 2, 2u) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float EaseInOutBounce(float x)
{
	return x < 0.5f
		? (1 - EaseOutBounce(1 - 2 * x)) / 2
		: (1 + EaseOutBounce(2 * x - 1)) / 2;
}

}