#include "main.h"

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch(global.status)
	{
	case global.GS_SONGSELECT:
		switch (wParam)
		{
		case VK_RETURN:
			global.status = global.GS_PLAYING;
			break;

		case VK_UP:
			if(global.currentSong <= 1)
				global.currentSong = global.totalSongCount;
			else
				global.currentSong--;
			break;

		case VK_DOWN:
			if(global.currentSong >= global.totalSongCount)
				global.currentSong = 1;
			else
				global.currentSong++;
			break;

		case VK_ESCAPE:
			global.status = global.GS_WELCOME;
			break;

		default:
			break;
		}

	case global.GS_PLAYING:
		switch (wParam)
		{
		case VK_UP:
			m_hero.pos.y -= 50;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		default:
			break;
		}
		break;
	}
}

VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:
		m_hero.pos.y += 50;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case VK_DOWN:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		break;
	}
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	//如果点击了继续或暂停图标
	if (Paused(ptMouse))
	{
		if (!m_gameStatus.isPaused)
		{
			KillTimer(hWnd, TIMER);
			m_gameStatus.isPaused = TRUE;
		}
		else
		{
			SetTimer(hWnd, TIMER, 50, NULL);
			m_gameStatus.isPaused = FALSE;
		}
		InvalidateRect(hWnd, NULL, FALSE);
	}
}
