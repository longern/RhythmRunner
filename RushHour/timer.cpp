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
		global.accummulatedTime = 0;
		gameTimePass = global.timePass();
	}

	BOOL isGameOver = TRUE;
	for (int i = 0; i < 4; i++)
		if ((gameTimePass - global.barriers[i].back().msecs) / global.currSong().msPerBeat <= 8)
			isGameOver = FALSE;
	if (isGameOver)
		GameOverInit();
}

DOUBLE GetCurrentHeight(int i)
{
	DOUBLE currHeight = 0.;
	for (UINT j = 0; j < global.barriers[i].size(); j++)
		if (global.barriers[i][j].type == 0 || (global.barriers[i][j].type == 1 && global.barriers[i][j].height == 0.))
			if (global.barriers[i][j].msecs <= gameTimePass)
				currHeight = global.barriers[i][j].height;
			else
				break;
		else if (global.barriers[i][j].type == 1 && global.barriers[i][j].height == 1.)
			if (global.barriers[i][j].msecs + global.currSong().msPerBeat * 0.2 <= gameTimePass)
				currHeight = global.barriers[i][j].height;
			else
				break;
	//  O(n), can be decreased to O(logn)

	return currHeight;
}

VOID HeroUpdate()
{
	for (int i = 0; i < 4; i++)
	{
		DOUBLE currHeight = GetCurrentHeight(i);
		int jpTime;
		DOUBLE jumpCent;
		if (!global.heroes[i].jpCount && global.heroes[i].height == currHeight)
			continue;

		if (global.heroes[i].jpStartTime == INT_MIN)
			global.heroes[i].jpStartTime = gameTimePass;

		jpTime = gameTimePass - global.heroes[i].jpStartTime;
		jumpCent = jpTime / global.currSong().msPerBeat / 0.4;
		//  Jump will last for 0.4 Beats
		if (global.heroes[i].jpCount)
			global.heroes[i].height = jumpCent * (1 - jumpCent) / 0.25 + global.heroes[i].startHeight;
		else
		{
			jumpCent += 0.5;
			global.heroes[i].height = jumpCent * (1 - jumpCent) / 0.25;
		}
		//  Max height will be 1.

		if (global.heroes[i].height <= currHeight)
		{
			global.heroes[i].height = currHeight;
			global.heroes[i].startHeight = 0.;
			global.heroes[i].jpCount = 0;
			global.heroes[i].jpStartTime = INT_MIN;
		}
	}
}

VOID DetectCollision()
{
	double x;
	for (UINT i = 0; i < 4; i++)
	{
		DOUBLE currHeight = GetCurrentHeight(i);
		for (UINT j = 0; j < global.barriers[i].size(); j++)
		{
			x = (global.barriers[i][j].msecs - gameTimePass) / global.currSong().msPerBeat;
			if (x >= 0)
				break;
			if (global.barriers[i][j].type == 0 && global.heroes[i].height <= currHeight)
			{
				x /= 0.4;
				//  Width of danger zone is 0.4 beats
				if (x >= -1. / 6 || x < -5. / 6 && x >= -1)
					global.blood -= 0.2;
				else if (x < -1. / 6 && x >= -5. / 6)
					global.blood -= 4;
			}
			else if (global.barriers[i][j].type == 1)
			{
				if (global.barriers[i][j].height == 1. && global.heroes[i].height < currHeight)
				{
					if (currHeight == 1.)
					{
						if (global.heroes[i].height / currHeight <= 1. / 4)
							global.blood -= 20;
						else if (global.heroes[i].height / currHeight <= 3. / 4)
							global.blood -= 5;
						global.heroes[i].height = currHeight - 0.01;
					}
				}
				else if (global.barriers[i][j].height == 0. && currHeight == 0. &&
					global.heroes[i].height <= currHeight)
				{
					x /= 0.5;
					if (x >= -1. / 6 || x < -5. / 6 && x >= -1)
						global.blood -= 0.2;
					else if (x < -1. / 6 && x >= -5. / 6)
						global.blood -= 4;
				}
			}
		}
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
			DetectCollision();
			HeroUpdate();
		}
		break;
	}
	refreshAnimations();
	InvalidateRect(hWnd, NULL, FALSE);
}
