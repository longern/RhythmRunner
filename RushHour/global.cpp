#include "main.h"
#include "audio.h"

int ToWindowX(double rx)
{
	return (int)(WNDWIDTH * rx);
}

int ToWindowY(double ry)
{
	return (int)(WNDHEIGHT * ry);
}

BOOL Circle(HDC hdc, int x, int y, int r)
{
	return Ellipse(hdc, x - r, y - r, x + r, y + r);
}

BOOL DrawSquare(HDC hdc, int cx, int cy, int ha)
{
	return Rectangle(hdc, cx - ha, cy - ha, cx + ha, cy + ha);
}

VOID PreviewSong()
{
	WCHAR firstMp3File[200];
	wsprintf(firstMp3File, TEXT("%s/%s"), global.currSong().name.data(), global.currSong().audioFilename.data());
	AudioOpen(firstMp3File);
	AudioPlay(global.currSong().previewTime);
}

VOID SwitchSong()
{
	WCHAR firstMp3File[200];
	wsprintf(firstMp3File, TEXT("%s/%s"), global.currSong().name.data(), global.currSong().audioFilename.data());
	AudioOpen(firstMp3File);
	AudioPlayOnce();
}

LONG GLOBAL::timePass()
{
	LARGE_INTEGER ct;
	QueryPerformanceCounter(&ct);
	if (global.isGamePaused)
		return global.accummulatedTime;
	return global.accummulatedTime
		+ (long)((ct.QuadPart - beginTime.QuadPart) / (double)clockFrequency.QuadPart * 1000);
}
