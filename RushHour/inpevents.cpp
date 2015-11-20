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

VOID GamePlayKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 'D':
		if (global.heroes[0].jpCount <= 0)
		{
			global.heroes[0].jpStartTime = global.timePass();
			global.heroes[0].jpCount++;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 'F':
		if (global.heroes[1].jpCount <= 0)
		{
			global.heroes[1].jpStartTime = global.timePass();
			global.heroes[1].jpCount++;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 'J':
		if (global.heroes[2].jpCount <= 0)
		{
			global.heroes[2].jpStartTime = global.timePass();
			global.heroes[2].jpCount++;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;

	case 'K':
		if (global.heroes[3].jpCount <= 0)
		{
			global.heroes[3].jpStartTime = global.timePass();
			global.heroes[3].jpCount++;
		}
		InvalidateRect(hWnd, NULL, FALSE);
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
