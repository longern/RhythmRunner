#include "audio.h"
#include "osureader.h"

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
		SwitchSong();
		break;

	case VK_DOWN:
		if (global.currentSong >= global.totalSongCount)
			global.currentSong = 1;
		else
			global.currentSong++;
		SwitchSong();
		break;

	case VK_ESCAPE:
		global.status = global.GS_WELCOME;
		break;

	default:
		break;
	}
}

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

VOID GamePlayKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 'D':
		DoJump(0);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 'F':
		DoJump(1);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 'J':
		DoJump(2);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 'K':
		DoJump(3);
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case VK_ESCAPE:
		break;

	default:
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
	}
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);
}
