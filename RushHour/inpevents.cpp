#include "audio.h"
#include "osureader.h"
#include "welcome.h"
#include "animator.h"

extern LONG gameTimePass;

VOID DoJump(int track)
{
	HERO *currHero = &global.heroes[track];
	if (currHero->jpCount >= 2)
		return;
	if (currHero->jpCount == 0)
		if (currHero->jpStartTime != INT_MIN &&
			(gameTimePass - currHero->jpStartTime) / global.currSong().msPerBeat / 0.4 <= 1. / 6)
			return;

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

	case VK_ESCAPE:
		global.status = global.GS_WELCOME;
		break;

	default:
		break;
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
		if (global.isGamePaused)
		{
			QueryPerformanceCounter(&global.beginTime);
			AudioResume();
			global.isGamePaused = false;
		}
		else
		{
			if (global.accummulatedTime == 0 || gameTimePass - global.accummulatedTime >= 3000)
			{
				global.isGamePaused = true;
				AudioPause();
				global.accummulatedTime = gameTimePass;
			}
		}
		break;

	case VK_ESCAPE:
		AudioClose();
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
		if (ptMouse.y <= ToWindowY(0.26))
			DoJump(0);
		else if (ptMouse.y <= ToWindowY(0.5))
			DoJump(1);
		else if (ptMouse.y <= ToWindowY(0.76))
			DoJump(2);
		else
			DoJump(3);
		break;
	}
}

VOID TouchEvent(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptTouch;
	ptTouch.x = LOWORD(lParam);
	ptTouch.y = HIWORD(lParam);
	
	switch (global.status)
	{
	case global.GS_PLAYING:
		if (ptTouch.y <= ToWindowY(0.26))
			DoJump(0);
		else if (ptTouch.y <= ToWindowY(0.5))
			DoJump(1);
		else if (ptTouch.y <= ToWindowY(0.76))
			DoJump(2);
		else
			DoJump(3);
		break;
	}
}
