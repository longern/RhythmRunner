#include "main.h"

struct ANIMATION
{
	LARGE_INTEGER begin;
	DOUBLE fromProperty;
	DOUBLE toProperty;
	LONG lastTime;
	BOOL loop;
	LONG delay;
	DOUBLE *prop;
	enum CURVE { LINEAR = 0, SINE, EXPR, QUAKE8 } curve;
};

VOID aniAdd(DOUBLE *prop, DOUBLE finalValue, LONG lastTime, ANIMATION::CURVE curve, BOOL loop = FALSE, LONG delay = 0);

VOID refreshAnimations();

VOID killAnimator(DOUBLE *prop);

VOID finishAnimator(DOUBLE *prop);

VOID resetAnimator(DOUBLE *prop);
