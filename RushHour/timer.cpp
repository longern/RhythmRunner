#include "main.h"
#include "audio.h"

static LONG gameTimePass;

VOID GameStatusUpdate()
{
	gameTimePass = global.timePass();
}

VOID HeroUpdate()
{
	for (int i = 0; i < 4; i++)
	{
		if (!global.heroes[i].jpCount)
			continue;
		int jpTime = gameTimePass - global.heroes[i].jpStartTime;
		DOUBLE jpCent = jpTime / global.currSong().msPerBeat / 0.4;
		//  Jump will last for 0.4 Beats
		global.heroes[i].height = jpCent * (1 - jpCent) + global.heroes[i].startHeight;
		if (global.heroes[i].height <= 0)
			global.heroes[i].height = global.heroes[i].jpCount = 0;
	}
}

VOID DetectCollision()
{

}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (global.status)
	{
	case global.GS_PLAYING:
		GameStatusUpdate();
		HeroUpdate();
		DetectCollision();
		break;
	}
	InvalidateRect(hWnd, NULL, FALSE);
}

VOID RenderWelcome(HDC hdcBuffer, HDC hdcBmp)
{

}

VOID RenderSongSelect(HDC hdcBuffer, HDC hdcBmp)
{
	UINT i;

	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT);

	SetTextColor(hdcBuffer, RGB(255, 255, 255));
	SetBkMode(hdcBuffer, TRANSPARENT);
	for (i = 0; i < global.totalSongCount; i++)
	{
		if (i + 1 == global.currentSong)
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
		TextOut(hdcBuffer, WNDWIDTH - 500, 15 + 25 * i, global.songs[i].name.data(), global.songs[i].name.length());
		if (i + 1 == global.currentSong)
			SetTextColor(hdcBuffer, RGB(255, 255, 255));
	}
}

VOID RenderOptions(HDC hdcBuffer, HDC hdcBmp)
{

}

VOID DrawSpikes(HDC hdcBuffer, double x, double bottom)
{
	INT pitWidth = ToWindowX(0.8 / 8) - global.heroWidth;
	INT spikeWidth = pitWidth / 3;
	POINT spike[3];
	for (int i = 0; i < 3; i++) {
		spike[0].x = ToWindowX(x) + global.heroWidth / 2 + i * spikeWidth;
		spike[1].x = ToWindowX(x) + global.heroWidth / 2 + i * spikeWidth + spikeWidth / 2;
		spike[2].x = ToWindowX(x) + global.heroWidth / 2 + (i + 1) * spikeWidth;
		spike[0].y = ToWindowY(bottom);
		spike[1].y = ToWindowY(bottom - 0.05);
		spike[2].y = ToWindowY(bottom);
		Polygon(hdcBuffer, spike, 3);
	}
}

VOID DrawBarriers(HDC hdcBuffer, int i)
{
	DOUBLE trackTop = i * 0.25 + (i % 2) * 0.01;
	DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;

	HGDIOBJ backBrush;

	for (UINT j = 0; j < global.barriers.size(); j++)
	{
		DOUBLE barrierX = 0.05 + (global.barriers[j].msecs - gameTimePass) / global.currSong().msPerBeat / 4.;
		if (barrierX < -0.2)
			continue;
		if (barrierX > 1)
			break;

		if (i & 1)
			backBrush = GetStockObject(BLACK_BRUSH);
		else
			backBrush = GetStockObject(WHITE_BRUSH);

		if (global.barriers[j].track == i)
		{
			SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));
			Rectangle(hdcBuffer, ToWindowX(barrierX), ToWindowY(trackTop) - 1,
				ToWindowX(barrierX) + 2, ToWindowY(trackBottom) + 1);

			SelectObject(hdcBuffer, backBrush);
			Rectangle(hdcBuffer, ToWindowX(barrierX) + global.heroWidth / 2, ToWindowY(trackBottom - 0.05) - 1,
				ToWindowX(barrierX + 0.8 / 8) - global.heroWidth / 2, ToWindowY(trackBottom) + 1);

			SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));
			switch (global.barriers[j].type)
			{
			default:
				DrawSpikes(hdcBuffer, barrierX, trackBottom);
				break;
			}
		}
	}
}

VOID RenderPlaying(HDC hdcBuffer, HDC hdcBmp)
{
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
		DrawBarriers(hdcBuffer, i);

		//  Draw StickMan
		UINT heroFrame = (int)(gameTimePass / (global.currSong().msPerBeat * 0.4) * 8 + 3) % 8;
		if (heroFrame >= 6)
			heroFrame++;
		if (i & 1)
		{
			SelectObject(hdcBmp, resource.wHero[heroFrame]);
			TransparentBlt(
				hdcBuffer,
				ToWindowX(0.05) - 21, ToWindowY(trackBottom - 0.1 - global.heroes[i].height / 4) - 8, global.heroWidth, global.heroHeight,
				hdcBmp,
				0, 0, 420, 504,
				RGB(0, 0, 0)
				);
		}
		else
		{
			SelectObject(hdcBmp, resource.hero[heroFrame]);
			TransparentBlt(
				hdcBuffer,
				ToWindowX(0.05) - 21, ToWindowY(trackBottom - 0.1 - global.heroes[i].height / 4) - 8, global.heroWidth, global.heroHeight,
				hdcBmp,
				0, 0, 420, 504,
				RGB(255, 255, 255)
				);
		}
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
