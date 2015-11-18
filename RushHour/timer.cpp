#include "main.h"

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch(global.status)
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
	LONG gameTimePass = global.timePass();
	UINT i;

	//  Draw Background
	SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, (int)(WNDHEIGHT * 0.26));
	Rectangle(hdcBuffer, 0, (int)(WNDHEIGHT * 0.5), WNDWIDTH, (int)(WNDHEIGHT * 0.76));

	for (i = 0; i < 4; i++) //  Four tracks
	{
		//  Draw Floor
		SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));
		DOUBLE trackTop = i * 0.25 + (i % 2) * 0.01;
		DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;
		Rectangle(hdcBuffer, 0, ToWindowY(trackBottom - 0.05) - 1, WNDWIDTH, ToWindowY(trackBottom) + 1);

		//  Draw Barriers
		for(UINT j = 0; j < global.barriers.size(); j++)
		{
			DOUBLE barrierX = 0.05 + (global.barriers[j].msecs - gameTimePass) / 1500.;
			if(barrierX < 0)
				continue;
			if(barrierX > 1)
				break;
			if(global.barriers[j].track == i)
				Rectangle(hdcBuffer, ToWindowX(barrierX), ToWindowY(trackTop), ToWindowX(barrierX) + 2, ToWindowY(trackBottom) + 1);
		}

		//  Draw StickMan
		if(i % 2)
			SelectObject(hdcBuffer, GetStockObject(WHITE_BRUSH));
		else
			SelectObject(hdcBuffer, GetStockObject(BLACK_BRUSH));
		Circle(hdcBuffer, ToWindowX(0.05), ToWindowY(trackBottom - 0.1), 7);
	}

}

VOID Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//开始绘制
	hdc = BeginPaint(hWnd, &ps);

	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);

	switch(global.status)
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

	//最后将所有的信息绘制到屏幕上
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	//回收资源所占的内存
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);

	//结束绘制
	EndPaint(hWnd, &ps);
}
