#include "main.h"
#include "audio.h"

static LONG gameTimePass;
static DOUBLE barrierX;
static HDC hdcBuffer;

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
	double x;
	int track;
	for (UINT j = 0; j < global.barriers.size(); j++)
	{
		x = (global.barriers[j].msecs - gameTimePass) / global.currSong().msPerBeat / 0.4;
		track = global.barriers[j].track;
		if (global.barriers[j].type)
			continue;
		if (x < 0)
		{
			if (global.barriers[j].type == 0 && global.heroes[track].height <= 0)
			{
				if (x < 0 && x >= -1. / 3 || x < -2. / 3 && x >= -1)
					global.blood -= 0.2;
				else if (x < -1. / 3 && x >= -2. / 3)
					global.blood -= 10;
			}
		}
		else
			break;
	}
	global.blood += 0.05;
	if (global.blood > 100)
		global.blood = 100;
	if (global.blood <= 0)
		GameOverInit();
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

VOID RenderWelcome(HDC hdcBmp)
{

}

VOID RenderSongSelect(HDC hdcBmp)
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

VOID RenderOptions(HDC hdcBmp)
{

}

VOID DrawSpikes(double bottom)
{
	INT pitWidth = ToWindowX(0.8 / 8) - global.heroWidth;
	INT spikeWidth = pitWidth / 4;
	POINT spike[3];
	for (int i = 0; i < 4; i++) {
		spike[0].x = ToWindowX(barrierX) + global.heroWidth / 2 + i * spikeWidth;
		spike[1].x = ToWindowX(barrierX) + global.heroWidth / 2 + i * spikeWidth + spikeWidth / 2;
		spike[2].x = ToWindowX(barrierX) + global.heroWidth / 2 + (i + 1) * spikeWidth;
		spike[0].y = ToWindowY(bottom);
		spike[1].y = ToWindowY(bottom - 0.05);
		spike[2].y = ToWindowY(bottom);
		Polygon(hdcBuffer, spike, 3);
	}
}

VOID DrawCliff(int j)
{
	int track = global.barriers[j].track;
	DOUBLE trackBottom = (track + 1) * 0.25 + ((track + 1) % 2) * 0.01;
	Rectangle(hdcBuffer, ToWindowX(barrierX), ToWindowY(trackBottom - 0.05) - 1,
		ToWindowX(barrierX + 0.8 / 8), ToWindowY(trackBottom) + 1);
	//Rectangle(hdcBuffer, ToWindowX(barrierX + 0.8 / 8 / 2) - 1, ToWindowY(trackBottom - 0.05) - 1,
	//	ToWindowX(barrierX + 0.8 / 8), ToWindowY(trackBottom) + 1);
}

VOID DrawBarriers(int i)
{
	DOUBLE trackTop = i * 0.25 + (i % 2) * 0.01;
	DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;

	HGDIOBJ backBrush;

	for (UINT j = 0; j < global.barriers.size(); j++)
	{
		barrierX = 0.05 + (global.barriers[j].msecs - gameTimePass) / global.currSong().msPerBeat / 4.;
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
			Rectangle(hdcBuffer, ToWindowX(barrierX) + global.heroWidth / 2, ToWindowY(trackTop),
				ToWindowX(barrierX + 0.8 / 8) - global.heroWidth / 2, ToWindowY(trackBottom) + 1);

			SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));
			switch (global.barriers[j].type)
			{
			case 0:
				DrawSpikes(trackBottom);
				break;
			case 1:
				DrawCliff(j);
				break;
			default:
				DrawSpikes(trackBottom);
				break;
			}
		}
	}
}

VOID RenderPlaying(HDC hdcBmp)
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
		DrawBarriers(i);

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

	WCHAR timeText[20];
	wsprintf(timeText, _T("%d %d"), gameTimePass, (int)global.blood);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
	TextOut(hdcBuffer, ToWindowX(0.8), ToWindowY(0.05), timeText, wcslen(timeText));
}

VOID Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hWnd, &ps);

	HDC	hdcBmp;
	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);

	switch (global.status)
	{
	case global.GS_WELCOME:
		RenderWelcome(hdcBmp);
		break;
	case global.GS_SONGSELECT:
		RenderSongSelect(hdcBmp);
		break;
	case global.GS_OPTIONS:
		RenderOptions(hdcBmp);
		break;
	case global.GS_PLAYING:
		RenderPlaying(hdcBmp);
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
