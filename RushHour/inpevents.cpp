#include "audio.h"
#include "osureader.h"
#include "welcome.h"
#include "animator.h"

VOID DoJump(int track)
{
	HERO *currHero = &global.heroes[track];
	if (currHero->jpCount <= 1)
	{
		currHero->jpStartTime = global.timePass();
		currHero->startHeight = currHero->height;
		currHero->jpCount++;
	}
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

	case VK_ESCAPE:
		global.status = global.GS_WELCOME;
		break;

	default:
		break;
	}
}

VOID GamePlayKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case '5':
		DoJump(0);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 'T':
		DoJump(1);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 'G':
		DoJump(2);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 'B':
		DoJump(3);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case VK_SPACE:
		if (global.isGamePaused)
		{
			QueryPerformanceCounter(&global.beginTime);
			AudioPlay();
			global.isGamePaused = false;
		}
		else
		{
			if (global.accummulatedTime == 0 || global.timePass() - global.accummulatedTime >= 3000)
			{
				global.isGamePaused = true;
				AudioPause();
				global.accummulatedTime = global.timePass();
			}
		}
		break;

	case VK_ESCAPE:
		AudioClose();
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
		if (PtInRect(&PlayButton, ptMouse) && !isPlayButtonHover)
		{
			isPlayButtonHover = true;
			aniAdd(&PlayButtonZoom, 1.05, 250, ANIMATION::SINE);
		}
		else if (PtInRect(&OptionButton, ptMouse) && !isOptionButtonHover)
		{
			isOptionButtonHover = true;
			aniAdd(&OptionButtonZoom, 1.05, 250, ANIMATION::SINE);
		}
		if(!PtInRect(&PlayButton, ptMouse) && isPlayButtonHover)
		{
			isPlayButtonHover = false;
			aniAdd(&PlayButtonZoom, 1., 250, ANIMATION::SINE);
		}
		if (!PtInRect(&OptionButton, ptMouse) && isOptionButtonHover)
		{
			isOptionButtonHover = false;
			aniAdd(&OptionButtonZoom, 1., 250, ANIMATION::SINE);
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
		if (PtInRect(&PlayButton, ptMouse))
			global.status = global.GS_SONGSELECT;
		else if (PtInRect(&ExitButton, ptMouse))
			global.status = global.GS_SONGSELECT;
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
	;
}
