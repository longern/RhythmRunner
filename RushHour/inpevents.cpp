#include "audio.h"
#include "osureader.h"
#include "welcome.h"
#include "animator.h"

extern LONG gameTimePass;

VOID setAccuracyIndicator(DOUBLE value)
{
	refreshAnimations();
	killAnimator(&global.accuracyIndicator);
	aniAdd(&global.accuracyIndicator, value, 150, ANIMATION::SINE);
}

VOID DoJump(int track)
{
	HERO *currHero = &global.heroes[track];
	if (currHero->jpCount >= 2)
		return;
	if (currHero->jpCount == 0)
		if (currHero->jpStartTime != INT_MIN &&
			(gameTimePass - currHero->jpStartTime) / global.currSong().msPerBeat / 0.4 <= 1. / 6)
			return;

	global.keySequence[track].push_back(gameTimePass);
	INT minDelta = INT_MAX;
	for (size_t j = 0; j < global.barriers[track].size(); j++)
		if (abs(minDelta) > abs(global.barriers[track][j].msecs - gameTimePass))
			minDelta = global.barriers[track][j].msecs - gameTimePass;
	if (abs(minDelta / (global.currSong().msPerBeat * 0.4)) <= 0.5)
		setAccuracyIndicator(minDelta / (global.currSong().msPerBeat * 0.4));
	else
		if (abs(minDelta / (global.currSong().msPerBeat * 0.4)) <= 1.)
			setAccuracyIndicator(minDelta > 0 ? 0.5 : -0.5);

	currHero->jpStartTime = global.timePass();
	currHero->startHeight = currHero->height;
	currHero->jpCount++;
}

VOID SongSelectKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_RETURN:
		GameInit();
		break;

	case VK_UP:
		if (global.currentSong <= 1)
			global.currentSong = global.totalSongCount;
		else
			global.currentSong--;
		PreviewSong();
		break;

	case VK_DOWN:
		if (global.currentSong >= global.totalSongCount)
			global.currentSong = 1;
		else
			global.currentSong++;
		PreviewSong();
		break;

	case VK_F1:
		settings.foggyMode = !settings.foggyMode;
		break;

	case VK_F2:
		settings.hideJudgeLine = !settings.hideJudgeLine;
		break;

	case VK_F3:
		settings.showAccuracyIndicator = !settings.showAccuracyIndicator;
		break;

	case VK_F4:
		settings.randomMode = !settings.randomMode;
		break;

	case VK_ESCAPE:
		global.status = global.GS_WELCOME;
		break;

	default:
		break;
	}
}

VOID togglePause()
{
	if (global.isGamePaused)
	{
		QueryPerformanceCounter(&global.beginTime);
		if (global.accummulatedTime >= 0)
			AudioResume();
		global.isGamePaused = false;
	}
	else
	{
		if (global.accummulatedTime == global.currSong().mciOffset - global.currSong().audioLeadIn ||
			gameTimePass - global.accummulatedTime >= 1)
		{
			global.isGamePaused = true;
			AudioPause();
			global.accummulatedTime = gameTimePass;
		}
	}
}

VOID GamePlayKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (lParam & 1 << 30)
		return;

	gameTimePass = global.timePass();

	switch (wParam)
	{
	case '5': case 'D':
		if (!global.isGamePaused)
		{
			DoJump(0);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case 'T': case 'F':
		if (!global.isGamePaused)
		{
			DoJump(1);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case 'G': case 'J':
		if (!global.isGamePaused)
		{
			DoJump(2);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case 'B': case 'K':
		if (!global.isGamePaused)
		{
			DoJump(3);
			InvalidateRect(hWnd, NULL, FALSE);
		}
		break;

	case VK_SPACE:
		togglePause();
		break;

	case VK_F3:
		settings.showAccuracyIndicator = !settings.showAccuracyIndicator;
		break;

	case VK_ESCAPE:
		PreviewSong();
		global.status = global.GS_SONGSELECT;
		break;

	default:
		break;
	}
}

VOID GameOverKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case VK_ESCAPE:
		PreviewSong();
		global.status = global.GS_SONGSELECT;
		break;
	}
}

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (global.status)
	{
	case global.GS_SONGSELECT:
		SongSelectKeyDown(hWnd, wParam, lParam);
		break;

	case global.GS_PLAYING:
		GamePlayKeyDown(hWnd, wParam, lParam);
		break;

	case global.GS_GAMEOVER:
		GameOverKeyDown(hWnd, wParam, lParam);
		break;
	}
}

VOID MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	switch (global.status)
	{
	case global.GS_WELCOME:
		for (int i = 0; i < 3; i++)
			if (PtInRect(&WelcomeButtons[i].geo, ptMouse) && !WelcomeButtons[i].isHover)
			{
				WelcomeButtons[i].isHover = TRUE;
				killAnimator(&WelcomeButtons[i].zoom);
				aniAdd(&WelcomeButtons[i].zoom, 1.1, 250, ANIMATION::SINE);
				break;
			}
		for (int i = 0; i < 3; i++)
			if (!PtInRect(&WelcomeButtons[i].geo, ptMouse) && WelcomeButtons[i].isHover)
			{
				WelcomeButtons[i].isHover = FALSE;
				killAnimator(&WelcomeButtons[i].zoom);
				aniAdd(&WelcomeButtons[i].zoom, 1., 250, ANIMATION::SINE);
			}
		break;
	case global.GS_SONGSELECT:
		break;
	case global.GS_PLAYING:
		break;
	}
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	switch (global.status)
	{
	case global.GS_WELCOME:
		if (PtInRect(&WelcomeButtons[0].geo, ptMouse))
		{
			PreviewSong();
			global.status = global.GS_SONGSELECT;
		}
		else if (PtInRect(&WelcomeButtons[2].geo, ptMouse))
			DestroyWindow(hWnd);
		break;

	case global.GS_SONGSELECT:
		if (ptMouse.y <= ToWindowY(0.33))
		{
			if (global.currentSong <= 1)
				global.currentSong = global.totalSongCount;
			else
				global.currentSong--;
			PreviewSong();
		}
		else if (ptMouse.y < ToWindowY(0.67))
			GameInit();
		else
		{
			if (global.currentSong >= global.totalSongCount)
				global.currentSong = 1;
			else
				global.currentSong++;
			PreviewSong();
		}
		break;

	case global.GS_PLAYING:
		if (ptMouse.x >= WNDWIDTH - 45 && ptMouse.y <= 31)
			togglePause();
		break;

	case global.GS_GAMEOVER:
		if (ptMouse.x >= ToWindowX(0.67) && ptMouse.y >= ToWindowY(0.67))
		{
			PreviewSong();
			global.status = global.GS_SONGSELECT;
		}
		break;
	}
}

LRESULT TouchEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (global.status)
	{
	case global.GS_PLAYING:
	{
		if (global.isGamePaused)
			return 0;

		UINT cInputs = LOWORD(wParam);
		PTOUCHINPUT pInputs = (PTOUCHINPUT)malloc(sizeof(TOUCHINPUT) * cInputs);
		if (pInputs)
		{
			POINT cliCoord = { 0 };
			ClientToScreen(hWnd, &cliCoord);
			if (GetTouchInputInfo((HTOUCHINPUT)lParam, cInputs, pInputs, sizeof(TOUCHINPUT)))
			{
				for (UINT i = 0; i < cInputs; i++)
				{
					int track;
					double ix = double(pInputs[i].x / 100 - cliCoord.x) / WNDWIDTH;
					double iy = double(pInputs[i].y / 100 - cliCoord.y) / WNDHEIGHT;
					if (ix < 0. || ix > 1.)
						continue;
					if (iy <= 0.26)
						track = 0;
					else if (iy <= 0.5)
						track = 1;
					else if (iy <= 0.76)
						track = 2;
					else
						track = 3;
					if (global.heroes[i].jpCount == 0 || (pInputs[i].dwFlags & TOUCHEVENTF_DOWN))
						DoJump(track);
				}
				CloseTouchInputHandle((HTOUCHINPUT)lParam);
			}
			free(pInputs);
		}
		return 0;
		break;
	}

	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
