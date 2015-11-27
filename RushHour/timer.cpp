#include "main.h"
#include "audio.h"
#include "animator.h"

LONG gameTimePass;

VOID GameStatusUpdate()
{
	gameTimePass = global.timePass();
	if (global.accummulatedTime < 0 && gameTimePass >= 0)
	{
		AudioPlayOnce();
		QueryPerformanceCounter(&global.beginTime);
		global.accummulatedTime = gameTimePass;
	}
	if ((gameTimePass - global.barriers.back().msecs) / global.currSong().msPerBeat >= 8)
		GameOverInit();
}

VOID HeroUpdate()
{
	for (int i = 0; i < 4; i++)
	{
		if (!global.heroes[i].jpCount)
			continue;
		int jpTime = gameTimePass - global.heroes[i].jpStartTime;
		DOUBLE jumpCent = jpTime / global.currSong().msPerBeat / 0.4;
		//  Jump will last for 0.4 Beats
		global.heroes[i].height = jumpCent * (1 - jumpCent) + global.heroes[i].startHeight;
		if (global.heroes[i].height <= 0)
			global.heroes[i].height = global.heroes[i].jpCount = 0;
	}
}

VOID DetectCollision()
{
	double x;
	int track;
	for (UINT j = 0; j < global.barriers.size(); j++)
	{
		x = (global.barriers[j].msecs - gameTimePass) / global.currSong().msPerBeat / 0.4;
		track = global.barriers[j].track;
		if (global.barriers[j].type)
			continue;
		if (x < 0)
		{
			if (global.barriers[j].type == 0 && global.heroes[track].height <= 0)
			{
				if (x < 0 && x >= -1. / 4 || x < -3. / 4 && x >= -1)
					global.blood -= 0.2;
				else if (x < -1. / 4 && x >= -3. / 4)
					global.blood -= 4;
			}
		}
		else
			break;
	}
	global.blood += 0.05;
	if (global.blood > 100)
		global.blood = 100;
	else if (global.blood <= 0)
		GameOverInit();
}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (global.status)
	{
	case global.GS_PLAYING:
		if (!global.isGamePaused)
		{
			GameStatusUpdate();
			HeroUpdate();
			DetectCollision();
		}
		break;
	}
	refreshAnimations();
	InvalidateRect(hWnd, NULL, FALSE);
}
