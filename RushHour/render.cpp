#include <windows.h>
#include <gdiplus.h>
#pragma comment (lib, "gdiplus")

#include "render.h"
#include "audio.h"
#include "animator.h"

DOUBLE barrierX;
HDC hdcBuffer;
HDC hdcBmp;
DOUBLE quakeScreen = 0.;

const double beatPerScreen = 4.0;

VOID RenderNoSong()
{
	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT);

	TextOut(hdcBuffer, ToWindowX(0.4), ToWindowY(0.4), _T("You don't have any songs."), 25);
	TextOut(hdcBuffer, ToWindowX(0.4), ToWindowY(0.45), _T("Please download from http://osu.ppy.sh/"), 39);
}

VOID RenderSongSelect()
{
	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT);

	WCHAR bgFilePath[256];
	wsprintf(bgFilePath, _T("%s/%s"), global.currSong().name.data(), global.currSong().bgImgFile.data());

	Gdiplus::Graphics gra(hdcBuffer);
	Gdiplus::Image bgImage(bgFilePath);
	gra.DrawImage(&bgImage, 0, 0, WNDWIDTH, WNDHEIGHT);
	Gdiplus::Image sonSelPic(_T("res/ui/sonsel.png"));
	gra.DrawImage(&sonSelPic, 0, 0, WNDWIDTH, WNDHEIGHT);

	SetTextColor(hdcBuffer, RGB(21, 21, 21));
	TextOut(hdcBuffer, 50, 355, global.currSong().title.data(), global.currSong().title.length());
}

VOID RenderOptions()
{

}

VOID RenderGameOver()
{
	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT);

	if (global.blood <= 0)
		TextOut(hdcBuffer, ToWindowX(0.38), ToWindowY(0.43), _T("Failed"), 6);
	else
		TextOut(hdcBuffer, ToWindowX(0.38), ToWindowY(0.43), _T("You won!"), 8);
	WCHAR outputText[30];
	wsprintf(outputText, _T("Your Distance: %d m"), global.finalTime / 10);
	TextOut(hdcBuffer, ToWindowX(0.38), ToWindowY(0.48), outputText, wcslen(outputText));
	swprintf(outputText, 30, _T("Your Score: %.2lf%%"), global.finalScore * 100.);
	TextOut(hdcBuffer, ToWindowX(0.38), ToWindowY(0.53), outputText, wcslen(outputText));
}

VOID Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;

	hdc = BeginPaint(hWnd, &ps);

	HFONT font = CreateFont(32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_PITCH, _T("Fantique Four"));

	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBuffer, font);

	SetBkMode(hdcBuffer, TRANSPARENT);

	switch (global.status)
	{
	case global.GS_NOSONG:
		RenderNoSong();
		break;
	case global.GS_WELCOME:
		RenderWelcome();
		break;
	case global.GS_SONGSELECT:
		RenderSongSelect();
		break;
	case global.GS_OPTIONS:
		RenderOptions();
		break;
	case global.GS_PLAYING:
		RenderPlaying();
		break;
	case global.GS_GAMEOVER:
		RenderGameOver();
	default:
		break;
	}

	BitBlt(hdc, 0, (INT)(quakeScreen), WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);

	DeleteObject(font);

	EndPaint(hWnd, &ps);
}
