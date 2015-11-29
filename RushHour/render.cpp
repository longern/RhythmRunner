#include "render.h"
#include "audio.h"
#include "animator.h"

DOUBLE barrierX;
HDC hdcBuffer;

const double beatPerScreen = 4.0;

VOID RenderNoSong()
{
	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT);

	TextOut(hdcBuffer, ToWindowX(0.4), ToWindowY(0.4), _T("You don't have any songs."), 25);
	TextOut(hdcBuffer, ToWindowX(0.4), ToWindowY(0.45), _T("Please download from http://osu.ppy.sh/"), 39);
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

VOID RenderGameOver()
{
	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT);

	if (global.blood <= 0)
		TextOut(hdcBuffer, ToWindowX(0.4), ToWindowY(0.4), _T("Failed"), 6);
	else
		TextOut(hdcBuffer, ToWindowX(0.4), ToWindowY(0.4), _T("You won!"), 8);
	WCHAR timeText[20];
	wsprintf(timeText, _T("Your Score: %u"), global.finalScore);
	TextOut(hdcBuffer, ToWindowX(0.4), ToWindowY(0.45), timeText, wcslen(timeText));
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
	case global.GS_NOSONG:
		RenderNoSong();
		break;
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
	case global.GS_GAMEOVER:
		RenderGameOver();
	default:
		break;
	}

	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);

	EndPaint(hWnd, &ps);
}
