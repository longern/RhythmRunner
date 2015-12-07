#include "render.h"
#include <gdiplus.h>

DOUBLE stickmanX = 0.1;

VOID DrawSpikes(int i, int j)
{
	DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;
	INT pitWidth = ToWindowX(0.4 / beatPerScreen) - global.heroWidth;
	DOUBLE spikeWidth = pitWidth / 4.;
	POINT spike[3];

	if (global.barriers[i][j].height > 0)
	{
		Rectangle(hdcBuffer,
			ToWindowX(barrierX) - 1, ToWindowY(trackBottom - 0.05 * global.barriers[i][j].height) - 1,
			ToWindowX(barrierX + 0.4 / beatPerScreen) + 1, ToWindowY(trackBottom) + 1);
		trackBottom -= 0.05 * global.barriers[i][j].height;
	}

	Rectangle(hdcBuffer, ToWindowX(barrierX) - 1, ToWindowY(trackBottom - 0.05) - 1,
		ToWindowX(barrierX) + global.heroWidth / 2, ToWindowY(trackBottom) + 1);
	Rectangle(hdcBuffer,
		ToWindowX(barrierX + 0.4 / beatPerScreen) - global.heroWidth / 2 - 1, ToWindowY(trackBottom - 0.05) - 1,
		ToWindowX(barrierX + 0.4 / beatPerScreen) + 1, ToWindowY(trackBottom) + 1);

	for (int i = 0; i < 4; i++)
	{
		spike[0].x = ToWindowX(barrierX) + int(global.heroWidth / 2. + i * spikeWidth);
		spike[1].x = ToWindowX(barrierX) + int(global.heroWidth / 2. + i * spikeWidth + spikeWidth / 2.);
		spike[2].x = ToWindowX(barrierX) + int(global.heroWidth / 2. + (i + 1) * spikeWidth);
		spike[0].y = ToWindowY(trackBottom);
		spike[1].y = ToWindowY(trackBottom - 0.05);
		spike[2].y = ToWindowY(trackBottom);
		Polygon(hdcBuffer, spike, 3);
	}
}

VOID DrawCliff(int i, int j)
{
	DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;
	if (global.barriers[i][j].height == 1.)
	{
		Rectangle(hdcBuffer, ToWindowX(barrierX) - 1, ToWindowY(trackBottom - 0.05) - 1,
			ToWindowX(barrierX + 0.4 / beatPerScreen / 2), ToWindowY(trackBottom) + 1);
		Rectangle(hdcBuffer, ToWindowX(barrierX + 0.4 / beatPerScreen / 2) - 1, ToWindowY(trackBottom - 0.1) - 1,
			ToWindowX(barrierX + 0.4 / beatPerScreen) + 1, ToWindowY(trackBottom) + 1);
	}
	else
	{
		POINT spike[3];
		INT pitWidth = ToWindowX(0.4 / beatPerScreen);
		DOUBLE spikeWidth = pitWidth / 5.;
		for (int i = 0; i < 5; i++)
		{
			spike[0].x = ToWindowX(barrierX) + int(i * spikeWidth);
			spike[1].x = ToWindowX(barrierX) + int(i * spikeWidth + spikeWidth / 2);
			spike[2].x = ToWindowX(barrierX) + int((i + 1) * spikeWidth);
			spike[0].y = ToWindowY(trackBottom);
			spike[1].y = ToWindowY(trackBottom - 0.05);
			spike[2].y = ToWindowY(trackBottom);
			Polygon(hdcBuffer, spike, 3);
		}
	}
}

VOID DrawNoBarrier(int i, int j)
{
	DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;
	if (global.barriers[i][j].height == 1.)
		Rectangle(hdcBuffer, ToWindowX(barrierX) - 1, ToWindowY(trackBottom - 0.1) - 1,
			ToWindowX(barrierX + 0.4 / beatPerScreen) + 1, ToWindowY(trackBottom) + 1);
	else
		Rectangle(hdcBuffer, ToWindowX(barrierX) - 1, ToWindowY(trackBottom - 0.05) - 1,
			ToWindowX(barrierX + 0.4 / beatPerScreen) + 1, ToWindowY(trackBottom) + 1);
}

VOID DrawBonus(int i, int j)
{
	DrawNoBarrier(i, j);
	DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;
	SelectObject(hdcBmp, resource.note[i & 1]);
	StretchBlt(hdcBuffer, ToWindowX(barrierX + 0.4 / beatPerScreen / 2) - 6,
		ToWindowY(trackBottom - 0.1 - global.barriers[i][j].height * 0.05) - 30 - global.heroHeight + 8,
		12, 30,
		hdcBmp, 0, 0, 83, 230, SRCCOPY);
}

VOID DrawStar(int i, int j)
{
	DrawNoBarrier(i, j);

	HGDIOBJ foreBrush;
	HGDIOBJ backBrush;
	if (i & 1)
	{
		foreBrush = GetStockObject(WHITE_BRUSH);
		backBrush = GetStockObject(BLACK_BRUSH);
	}
	else
	{
		foreBrush = GetStockObject(BLACK_BRUSH);
		backBrush = GetStockObject(WHITE_BRUSH);
	}

	SelectObject(hdcBuffer, foreBrush);
	DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;
	Circle(hdcBuffer, ToWindowX(barrierX + 0.4 / beatPerScreen / 2),
		ToWindowY(trackBottom - 0.1 - global.barriers[i][j].height * 0.05) - global.heroHeight + 4
		, 8);
	SelectObject(hdcBuffer, backBrush);
	DrawSquare(hdcBuffer, ToWindowX(barrierX + 0.4 / beatPerScreen / 2),
		ToWindowY(trackBottom - 0.1 - global.barriers[i][j].height * 0.05) - global.heroHeight + 4
		, 4);
}

VOID DrawNoise(int i, int j)
{
	DrawNoBarrier(i, j);
	DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;
	SelectObject(hdcBmp, resource.noise[i & 1]);
	StretchBlt(hdcBuffer, ToWindowX(barrierX + 0.4 / beatPerScreen / 2) - 16,
		ToWindowY(trackBottom - 0.075 - global.barriers[i][j].height * 0.05) - 28,
		32, 32,
		hdcBmp, 0, 0, 64, 64, SRCCOPY);
}

VOID DrawBarriers(int i)
{
	DOUBLE trackTop = i * 0.25 + (i % 2) * 0.01;
	DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;

	HGDIOBJ backBrush;

	SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));

	for (UINT j = 0; j < global.barriers[i].size(); j++)
	{
		double lastBarrierX = -1.;
		double lastBarrierHeight = 0.;

		barrierX = stickmanX +  //  Position of Stickman
			(global.barriers[i][j].msecs - gameTimePass) / global.currSong().msPerBeat / beatPerScreen;
		if (barrierX < -0.2 && j < global.barriers[i].size() - 1)
			continue;
		if (j >= 1)
		{
			lastBarrierX = stickmanX +  //  Position of Stickman
				(global.barriers[i][j - 1].msecs - gameTimePass) / global.currSong().msPerBeat / beatPerScreen;
			lastBarrierHeight = global.barriers[i][j - 1].height;
		}

		if (lastBarrierX >= 1.)
			break;

		if (i & 1)
			backBrush = GetStockObject(BLACK_BRUSH);
		else
			backBrush = GetStockObject(WHITE_BRUSH);

		//  Draw the Floor before this Barrier
		SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));
		Rectangle(hdcBuffer,
			ToWindowX(lastBarrierX + 0.4 / beatPerScreen), ToWindowY(trackBottom - 0.05 * (lastBarrierHeight + 1.)) - 1,
			ToWindowX(barrierX), ToWindowY(trackBottom) + 1);

		//  Draw Judge Line
		if (!settings.hideJudgeLine)
		{
			SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));
			Rectangle(hdcBuffer, ToWindowX(barrierX) - 2, ToWindowY(trackTop) - 1,
				ToWindowX(barrierX) + 2, ToWindowY(trackBottom) + 1);
		}

		//  Clear Barrier Area
		SelectObject(hdcBuffer, backBrush);
		Rectangle(hdcBuffer, ToWindowX(barrierX) + global.heroWidth / 2, ToWindowY(trackTop),
			ToWindowX(barrierX + 0.4 / beatPerScreen) - global.heroWidth / 2, ToWindowY(trackBottom) + 1);

		//  Draw Barrier
		SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));
		switch (global.barriers[i][j].type)
		{
		case 0:
			DrawSpikes(i, j);
			break;
		case 1:
			DrawCliff(i, j);
			break;
		case 2:
			DrawBonus(i, j);
			break;
		case 3:
			DrawStar(i, j);
			break;
		case 4:
			DrawNoise(i, j);
			break;
		case INT_MAX:
			DrawNoBarrier(i, j);
			break;
		default:
			DrawSpikes(i, j);
			break;
		}

		SelectObject(hdcBuffer, GetStockObject(GRAY_BRUSH));
		if (j == global.barriers[i].size() - 1)
		{
			Rectangle(hdcBuffer,
				ToWindowX(barrierX + 0.4 / beatPerScreen), ToWindowY(trackBottom - 0.05 * (global.barriers[i][j].height + 1.)) - 1,
				ToWindowX(1) + 1, ToWindowY(trackBottom) + 1);
		}
	}
}

VOID RenderPlaying()
{
	UINT i;

	//  Draw Background
	SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, (int)(WNDHEIGHT * 0.26));
	Rectangle(hdcBuffer, 0, (int)(WNDHEIGHT * 0.5), WNDWIDTH, (int)(WNDHEIGHT * 0.76));

	INT totalLength = INT_MIN;
	for (i = 0; i < 4; i++)
		if (global.barriers[i].size() && totalLength < global.barriers[i].back().msecs)
			totalLength = global.barriers[i].back().msecs;
	totalLength += INT(global.currSong().msPerBeat * 0.5);

	for (i = 0; i < 4; i++) //  Four tracks. Want more?
	{
		DOUBLE trackTop = i * 0.25 + (i % 2) * 0.01;
		DOUBLE trackBottom = (i + 1) * 0.25 + ((i + 1) % 2) * 0.01;

		//  Draw Barriers
		DrawBarriers(i);

		//  Draw Card Machine
		if (i == 0)
			for (int k = 0; k <= 2; k++)
			{
				DOUBLE machineX = (totalLength * k / 2 - gameTimePass) / global.currSong().msPerBeat / beatPerScreen;
				if (machineX < 1.02 && machineX > -0.18)
				{
					int height = 0;
					for (size_t j = 0; j < global.barriers[0].size(); j++)
						if (global.barriers[0][j].msecs <= totalLength * k / 2)
							height = global.barriers[0][j].height;
						else
							break;
					SelectObject(hdcBmp, machineX > 0 ? resource.machine : resource.machineok);
					TransparentBlt(hdcBuffer, ToWindowX(machineX + stickmanX) - 24, ToWindowY(0.21 - height * 0.05) - 49,
						48, 48,
						hdcBmp, 0, 0, 48, 48, RGB(255, 255, 255));
				}
			}

		//  Draw StickMan
		DOUBLE beatPassed = gameTimePass / global.currSong().msPerBeat;
		DOUBLE beatPerLoop;
		if (global.currSong().msPerBeat <= 1000. / 3)
			beatPerLoop = 1.0;
		else
			beatPerLoop = 0.5;
		int heroTotalFrame = int(beatPassed / beatPerLoop * 8);
		UINT heroFrame = ((heroTotalFrame + 3) % 8 + 8) % 8;
		//  loop begin from 3rd frame
		//  prevent negative time (audioLeadIn)
		if (heroFrame >= 6)
			heroFrame++;

		COLORREF backColor;
		if (i & 1)
		{
			SelectObject(hdcBmp, resource.wHero[heroFrame]);
			backColor = RGB(0, 0, 0);
		}
		else
		{
			SelectObject(hdcBmp, resource.hero[heroFrame]);
			backColor = RGB(255, 255, 255);
		}
		TransparentBlt(
			hdcBuffer,
			ToWindowX(stickmanX) - 21, ToWindowY(trackBottom - 0.1 - global.heroes[i].height * 0.05) - 8, global.heroWidth, global.heroHeight,
			hdcBmp,
			0, 0, 420, 504,
			backColor
			);
	}

	SelectObject(hdcBmp, resource.cloud);
	TransparentBlt(hdcBuffer, ToWindowX(1 - gameTimePass  * 0.99 / totalLength), 10, 48, 48,
		hdcBmp, 0, 0, 48, 48, RGB(255, 255, 255));

	if (settings.foggyMode)
	{
		Gdiplus::Graphics gra(hdcBuffer);
		Gdiplus::Image sonSelPic(_T("res/ui/foggy.png"));
		gra.DrawImage(&sonSelPic, 0, 0, WNDWIDTH, WNDHEIGHT);
	}

	// Draw blood
	SelectObject(hdcBuffer, GetStockObject(WHITE_BRUSH));
	Rectangle(hdcBuffer, 0, 0, ToWindowX(0.01), WNDHEIGHT);
	HBRUSH darkRedBrush = CreateSolidBrush(RGB(194, 70, 49));
	SelectObject(hdcBuffer, darkRedBrush);
	Rectangle(hdcBuffer, 0, ToWindowY(1. - global.blood / 100.), ToWindowX(0.01), WNDHEIGHT);
	SelectObject(hdcBuffer, GetStockObject(WHITE_BRUSH));
	DeleteObject(darkRedBrush);

	//  Draw accuracy indicator
	if (settings.showAccuracyIndicator)
	{
		HBRUSH indicatorYellowBrush = CreateSolidBrush(RGB(242, 215, 80));
		SelectObject(hdcBuffer, indicatorYellowBrush);
		Rectangle(hdcBuffer, ToWindowX(0.01) - 1, 0, ToWindowX(0.02), WNDHEIGHT);

		HBRUSH indicatorGreenBrush = CreateSolidBrush(RGB(36, 180, 85));
		SelectObject(hdcBuffer, indicatorGreenBrush);
		DeleteObject(indicatorYellowBrush);
		Rectangle(hdcBuffer, ToWindowX(0.01) - 1, ToWindowY(0.25), ToWindowX(0.02), ToWindowY(0.75));

		HBRUSH indicatorBrush = CreateSolidBrush(RGB(6, 145, 205));
		SelectObject(hdcBuffer, indicatorBrush);
		DeleteObject(indicatorGreenBrush);
		Rectangle(hdcBuffer, ToWindowX(0.01) - 1, ToWindowY(0.5 - global.accuracyIndicator) - 5,
			ToWindowX(0.02), ToWindowY(0.5 - global.accuracyIndicator) + 5);

		SelectObject(hdcBuffer, GetStockObject(WHITE_BRUSH));
		DeleteObject(indicatorBrush);
	}

	WCHAR timeText[20];
	wsprintf(timeText, _T("%d"), gameTimePass);
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
	TextOut(hdcBuffer, ToWindowX(0.8), ToWindowY(0.05), timeText, wcslen(timeText));

	//  Draw pause button
	SelectObject(hdcBmp, resource.pauseButton);
	TransparentBlt(hdcBuffer, WNDWIDTH - 45, 10, 15, 21,
		hdcBmp, 0, 0, 124, 175, RGB(255, 255, 255));
}
