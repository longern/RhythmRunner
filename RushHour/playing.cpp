#include "render.h"

VOID DrawSpikes(double bottom)
{
	INT pitWidth = ToWindowX(0.4 / beatPerScreen) - global.heroWidth;
	INT spikeWidth = pitWidth / 4;
	POINT spike[3];
	for (int i = 0; i < 4; i++)
	{
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
		ToWindowX(barrierX + 0.4 / beatPerScreen), ToWindowY(trackBottom) + 1);
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
		barrierX = 0.05 +  //  Position of Stickman
			(global.barriers[j].msecs - gameTimePass) / global.currSong().msPerBeat / beatPerScreen;
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
				ToWindowX(barrierX + 0.4 / beatPerScreen) - global.heroWidth / 2, ToWindowY(trackBottom) + 1);

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
