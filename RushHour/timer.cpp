#include "main.h"
#include "audio.h"

static LONG gameTimePass;

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (global.status)
	{
	case global.GS_PLAYING:
		HeroUpdate();
		TerrianUpdate();
		GameStatusUpdate();
		break;
	}
	InvalidateRect(hWnd, NULL, FALSE);
}

VOID HeroUpdate()
{
	for (int i = 0; i < 4; i++)
	{
		if (!global.heroes[i].jpCount)
			continue;
		int jpTime = gameTimePass - global.heroes[i].jpStartTime;
		if (jpTime > global.currSong().msPerBeat / 2)
		{
			global.heroes[i].height = global.heroes[i].jpCount = 0;
			continue;
		}
		DOUBLE jpCent = jpTime / global.currSong().msPerBeat * 2;
		global.heroes[i].height = jpCent * (1 - jpCent);
	}
	//TODO
	//更新动作
	++m_hero.curFrameIndex;
	m_hero.curFrameIndex = m_hero.curFrameIndex >= m_hero.maxFrameSize ? 0 : m_hero.curFrameIndex;
}

VOID TerrianUpdate()
{
	//TODO
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		m_terrian[k].pos.x -= 5;
		if (m_terrian[k].pos.x + m_terrian[k].size.cx < 0)
		{
			m_terrian[k].pos.x += MAX_TERRIAN_NUM * m_terrian[k].size.cx;
		}
	}
}

VOID GameStatusUpdate()
{
	//TODO
	++m_gameStatus.totalDist;
}

BOOL Paused(POINT ptMouse)
{
	RECT rPause;

	rPause.left = m_gameStatus.pos.x;
	rPause.top = m_gameStatus.pos.y;
	rPause.right = m_gameStatus.size.cx;
	rPause.bottom = m_gameStatus.size.cy;

	return PtInRect(&rPause, ptMouse);
}

VOID RenderWelcome(HDC hdcBuffer, HDC hdcBmp)
{

}

VOID RenderSongSelect(HDC hdcBuffer, HDC hdcBmp)
{
	UINT i;

	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, (int)(WNDHEIGHT * 0.26));
	Rectangle(hdcBuffer, 0, WNDHEIGHT / 2, WNDWIDTH, (int)(WNDHEIGHT * 0.76));

	SetTextColor(hdcBuffer, RGB(255, 255, 255));
	SetBkMode(hdcBuffer, TRANSPARENT);
	for (i = 0; i < global.totalSongCount; i++)
	{
		if (i + 1 == global.currentSong)
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
		TextOut(hdcBuffer, WNDWIDTH - 400, 15 + 25 * i, global.songs[i].name.data(), global.songs[i].name.length());
		if (i + 1 == global.currentSong)
			SetTextColor(hdcBuffer, RGB(255, 255, 255));
	}
}

VOID RenderOptions(HDC hdcBuffer, HDC hdcBmp)
{

}

VOID RenderPlaying(HDC hdcBuffer, HDC hdcBmp)
{
	gameTimePass = global.timePass();
	UINT i;

	//  Draw Background
	SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, (int)(WNDHEIGHT * 0.26));
	Rectangle(hdcBuffer, 0, (int)(WNDHEIGHT * 0.5), WNDWIDTH, (int)(WNDHEIGHT * 0.76));

	for (i = 0; i < 4; i++) //  Four tracks. Want more?
	{
		//  Draw Floor
		SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));
		DOUBLE trackTop = i * 0.25 + (i % 2) * 0.01;
		DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;
		Rectangle(hdcBuffer, 0, ToWindowY(trackBottom - 0.05) - 1, WNDWIDTH, ToWindowY(trackBottom) + 1);

		//  Draw Barriers
		HBRUSH redBrush = CreateSolidBrush(RGB(200, 0, 0));
		SelectObject(hdcBuffer, redBrush);
		for (UINT j = 0; j < global.barriers.size(); j++)
		{
			DOUBLE barrierX = 0.05 + (global.barriers[j].msecs - gameTimePass) / 3000.;
			if (barrierX < -0.2)
				continue;
			if (barrierX > 1)
				break;

			if (global.barriers[j].track == i)
			{
				Rectangle(hdcBuffer, ToWindowX(barrierX) + 19, ToWindowY(trackBottom - 0.05) - 1,
					ToWindowX(barrierX + 0.025) + 19, ToWindowY(trackBottom) + 1);
				Rectangle(hdcBuffer, ToWindowX(barrierX), ToWindowY(trackTop) - 1,
					ToWindowX(barrierX) + 2, ToWindowY(trackBottom) + 1);
			}
		}
		DeleteObject(redBrush);

		//  Draw StickMan
		UINT heroFrame = (int)(gameTimePass / (global.currSong().msPerBeat / 2) * 8 + 3) % 8;
		if (heroFrame >= 6)
			heroFrame++;
		SelectObject(hdcBmp, resource.hero[heroFrame]);
		TransparentBlt(
			hdcBuffer,
			ToWindowX(0.05) - 21, ToWindowY(trackBottom - 0.1) - 8 - global.heroes[i].height * 200, 38, 45,
			hdcBmp,
			0, 0, 420, 504,
			RGB(255, 255, 255)
			)
	}

	WCHAR timeText[10];
	wsprintf(timeText, _T("%d"), gameTimePass);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
	TextOut(hdcBuffer, ToWindowX(0.8), ToWindowY(0.05), timeText, wcslen(timeText));
}

VOID Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hWnd, &ps);

	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);

	switch (global.status)
	{
	case global.GS_WELCOME:
		RenderWelcome(hdcBuffer, hdcBmp);
		break;
	case global.GS_SONGSELECT:
		RenderSongSelect(hdcBuffer, hdcBmp);
		break;
	case global.GS_OPTIONS:
		RenderOptions(hdcBuffer, hdcBmp);
		break;
	case global.GS_PLAYING:
		RenderPlaying(hdcBuffer, hdcBmp);
		break;
	default:
		break;
	}

	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);

	EndPaint(hWnd, &ps);
}
