#include "animator.h"
#include <vector>

static std::vector<ANIMATION> animations;

VOID aniAdd(DOUBLE *prop, DOUBLE finalValue, LONG lastTime, ANIMATION::CURVE curve, LONG delay)
{
	animations.push_back(ANIMATION());;
	QueryPerformanceCounter(&animations.back().begin);
	animations.back().lastTime = lastTime;
	animations.back().prop = prop;
	animations.back().fromProperty = *prop;
	animations.back().curve = curve;
	animations.back().delay = delay;
}

VOID refreshAnimations()
{
	LARGE_INTEGER ct;
	QueryPerformanceCounter(&ct);
	LONG tp;

	for (UINT i = 0; i < animations.size(); i++)
	{
		tp = (LONG)(ct.QuadPart - animations[i].begin.QuadPart);
		if (tp <= animations[i].delay)
			continue;
		tp -= animations[i].delay;
		if (tp >= animations[i].lastTime)
		{
			*animations[i].prop = animations[i].toProperty;
			animations.erase(animations.begin() + i);
			continue;
		}
		double x = tp / animations[i].lastTime;
		switch (animations[i].curve)
		{
		case ANIMATION::SINE:
			x = sin(PI / 2 * x);
			break;
		case ANIMATION::LINEAR:
		default:
			//  x = x;
			break;
		}
		*animations[i].prop = x * animations[i].toProperty - (1 - x) * animations[i].fromProperty;
	}
}
