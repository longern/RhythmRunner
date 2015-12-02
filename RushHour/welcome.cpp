#include "render.h"
#include "welcome.h"

WELCOMEBUTTON WelcomeButtons[3] =
{
	{ { 300, 133, WNDWIDTH - 300, 233 }, FALSE, 1.0 },
	{ { 300, 325, WNDWIDTH - 300, 425 }, FALSE, 1.0 },
	{ { 300, 517, WNDWIDTH - 300, 617 }, FALSE, 1.0 }
};

VOID RenderWelcome(HDC hdcBmp)
{
	RECT rect;
	HFONT font = CreateFont(32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_PITCH, _T("Times New Roman"));
	SelectObject(hdcBuffer, font);

	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT);
	SelectObject(hdcBuffer, GetStockObject(NULL_PEN));
	SetBkMode(hdcBuffer, TRANSPARENT);

	HBRUSH hBrush = CreateSolidBrush(RGB(127 * WelcomeButtons[0].zoom, 127 * WelcomeButtons[0].zoom, 127 * WelcomeButtons[0].zoom));
	SelectObject(hdcBuffer, hBrush);
	rect = WelcomeButtons[0].geo;
	Rectangle(hdcBuffer, rect.left, rect.top, rect.right, rect.bottom);
	TextOut(hdcBuffer, ToWindowX(0.48), ToWindowY(0.223), _T("Play"), 4);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(127 * WelcomeButtons[1].zoom, 127 * WelcomeButtons[1].zoom, 127 * WelcomeButtons[1].zoom));
	SelectObject(hdcBuffer, hBrush);
	rect = WelcomeButtons[1].geo;
	Rectangle(hdcBuffer, rect.left, rect.top, rect.right, rect.bottom);
	TextOut(hdcBuffer, ToWindowX(0.47), ToWindowY(0.473), _T("Option"), 6);
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(RGB(127 * WelcomeButtons[2].zoom, 127 * WelcomeButtons[2].zoom, 127 * WelcomeButtons[2].zoom));
	SelectObject(hdcBuffer, hBrush);
	rect = WelcomeButtons[2].geo;
	Rectangle(hdcBuffer, rect.left, rect.top, rect.right, rect.bottom);
	TextOut(hdcBuffer, ToWindowX(0.48), ToWindowY(0.723), _T("Exit"), 4);
	DeleteObject(hBrush);

	DeleteObject(font);
}
