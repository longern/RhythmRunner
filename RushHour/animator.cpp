#include "animator.h"
#include <vector>

static std::vector<ANIMATION> animations;

VOID aniAdd(DOUBLE *prop, DOUBLE finalValue, LONG lastTime, ANIMATION::CURVE curve, BOOL loop, LONG delay)
{
	animations.push_back(ANIMATION());;
	QueryPerformanceCounter(&animations.back().begin);
	animations.back().lastTime = lastTime;
	animations.back().prop = prop;
	animations.back().fromProperty = *prop;
	animations.back().toProperty = finalValue;
	animations.back().curve = curve;
	animations.back().loop = loop;
	animations.back().delay = delay;
}

VOID refreshAnimations()
{
	LARGE_INTEGER ct;
	QueryPerformanceCounter(&ct);
	LONG tp;

	for (UINT i = 0; i < animations.size(); i++)
	{
		tp = (LONG)((ct.QuadPart - animations[i].begin.QuadPart) / double(global.clockFrequency.QuadPart) * 1000);
		if (tp <= animations[i].delay)
			continue;
		tp -= animations[i].delay;
		if (tp >= animations[i].lastTime)
		{
			if (animations[i].loop)
			{
				animations[i].begin.QuadPart += tp / animations[i].lastTime * animations[i].lastTime;
				tp %= animations[i].lastTime;
			}
			else
			{
				*animations[i].prop = animations[i].toProperty;
				animations.erase(animations.begin() + i);
				continue;
			}
		}
		double x = double(tp) / animations[i].lastTime;
		switch (animations[i].curve)
		{
		case ANIMATION::SINE:
			x = sin(PI / 2 * x);
			break;
		case ANIMATION::QUAKE8:
			x = int(x * 16) % 2;
			break;
		case ANIMATION::LINEAR:
		default:
			//  x = x;
			break;
		}
		*animations[i].prop = x * animations[i].toProperty + (1 - x) * animations[i].fromProperty;
	}
}

VOID killAnimator(DOUBLE *prop)
{
	for (UINT i = 0; i < animations.size();)
		if (animations[i].prop == prop)
			animations.erase(animations.begin() + i);
		else
			i++;
}

VOID finishAnimator(DOUBLE *prop)
{
	for (UINT i = 0; i < animations.size();)
		if (animations[i].prop == prop)
		{
			*prop = animations[i].toProperty;
			animations.erase(animations.begin() + i);
		}
		else
			i++;
}

VOID resetAnimator(DOUBLE *prop)
{
	for (UINT i = 0; i < animations.size();)
		if (animations[i].prop == prop)
		{
			*prop = animations[i].fromProperty;
			animations.erase(animations.begin() + i);
		}
		else
			i++;
}
