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
		if(global.currentSong <= 1)
			global.currentSong = global.totalSongCount;
		else
			global.currentSong--;
		SwitchSong();
		break;

	case VK_DOWN:
		if(global.currentSong >= global.totalSongCount)
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

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch(global.status)
	{
	case global.GS_SONGSELECT:
		SongSelectKeyDown(hWnd, wParam, lParam);
		break;

	case global.GS_PLAYING:
		switch (wParam)
		{
		case VK_UP:
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		default:
			break;
		}
		break;
	}
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);
}
