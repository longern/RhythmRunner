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
		TextOut(hdcBuffer, WNDWIDTH - 400, 15 + 25 * i, global.songNames[i], wcslen(global.songNames[i]));
		if (i + 1 == global.currentSong)
			SetTextColor(hdcBuffer, RGB(255, 255, 255));
	}
}

VOID RenderOptions(HDC hdcBuffer, HDC hdcBmp)
{

}

VOID RenderPlaying(HDC hdcBuffer, HDC hdcBmp)
{
	//绘制背景到缓存
	SelectObject(hdcBmp, m_hBackgroundBmp);
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcBmp, 0, 0, SRCCOPY);

	//绘制建筑到缓存
	SelectObject(hdcBmp, m_building.hBmp);
	TransparentBlt(
		hdcBuffer, m_building.pos.x, m_building.pos.y,
		m_building.size.cx, m_building.size.cy,
		hdcBmp, 0, 0, m_building.size.cx, m_building.size.cy,
		RGB(255, 255, 255)
		);

	//绘制Hero到缓存
	SelectObject(hdcBmp, m_hero.hBmp);
	TransparentBlt(
		hdcBuffer, m_hero.pos.x, m_hero.pos.y,
		m_hero.size.cx, m_hero.size.cy,
		hdcBmp, 0, m_hero.size.cy * m_hero.curFrameIndex, m_hero.size.cx, m_hero.size.cy,
		RGB(255, 255, 255)
		);

	//绘制地形
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		Terrian terrian = m_terrian[k];
		SelectObject(hdcBmp, terrian.hRoofBmp);
		TransparentBlt(
			hdcBuffer, terrian.pos.x, terrian.pos.y,
			terrian.roofWidth, terrian.roofHeight,
			hdcBmp, 0, 0, terrian.roofWidth, terrian.roofHeight,
			RGB(255, 255, 255)
			);
		SelectObject(hdcBmp, terrian.hBlockBmp);
		int t;
		for (t = 0; t < terrian.blockNum; ++t)
		{
			TransparentBlt(
				hdcBuffer, terrian.pos.x, terrian.pos.y + terrian.roofHeight + terrian.blockHeight * t,
				terrian.blockWidth, terrian.blockHeight,
				hdcBmp, 0, 0, terrian.blockWidth, terrian.blockHeight,
				RGB(255, 255, 255)
				);
		}
	}

	//绘制游戏状态
	//暂停或继续位图
	SelectObject(hdcBmp, m_gameStatus.hBmp);
	TransparentBlt(hdcBuffer, m_gameStatus.pos.x, m_gameStatus.pos.y, m_gameStatus.size.cx, m_gameStatus.size.cy,
		hdcBmp, 0, m_gameStatus.size.cy * m_gameStatus.isPaused,
		m_gameStatus.size.cx, m_gameStatus.size.cy, RGB(255, 255, 255));

	//绘制分数
	TCHAR	szDist[13];
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);
	TextOut(hdcBuffer, WNDWIDTH - 400, 15, szDist, wsprintf(szDist, _T("距离: %d"), m_gameStatus.totalDist));
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
