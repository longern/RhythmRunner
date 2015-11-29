#include "main.h"

extern LONG gameTimePass;
extern DOUBLE barrierX;
extern HDC hdcBuffer;

extern const double beatPerScreen;

VOID RenderWelcome(HDC hdcBmp);
VOID RenderPlaying(HDC hdcBmp);
