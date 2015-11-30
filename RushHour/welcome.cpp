#include "render.h"
#include "welcome.h"

WELCOMEBUTTON WelcomeButtons[3] =
{
	{ { 300, 133, WNDWIDTH - 300, 233 }, FALSE, 1.0 },
	{ { 300, 325, WNDWIDTH - 300, 425 }, FALSE, 1.0 },
	{ { 300, 517, WNDWIDTH - 300, 617 }, FALSE, 1.0 }
};

VOID zoomWidget(RECT rect, double zoom, RECT *res)
{
	res->left = LONG((rect.left + rect.right) / 2. - (rect.right - rect.left) / 2. * zoom);
	res->right = LONG((rect.left + rect.right) / 2. + (rect.right - rect.left) / 2. * zoom);
	res->top = LONG((rect.top + rect.bottom) / 2. - (rect.bottom - rect.top) / 2. * zoom);
	res->bottom = LONG((rect.top + rect.bottom) / 2. + (rect.bottom - rect.top) / 2. * zoom);
}

VOID RenderWelcome(HDC hdcBmp)
{
	RECT rect;
	Rectangle(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT);
	SelectObject(hdcBuffer, GetStockObject(NULL_BRUSH));
	SelectObject(hdcBuffer, GetStockObject(BLACK_PEN));

	zoomWidget(WelcomeButtons[0].geo, WelcomeButtons[0].zoom, &rect);
	Rectangle(hdcBuffer, rect.left, rect.top, rect.right, rect.bottom);
	TextOut(hdcBuffer, ToWindowX(0.48), ToWindowY(0.23), _T("Play"), 4);

	zoomWidget(WelcomeButtons[1].geo, WelcomeButtons[1].zoom, &rect);
	Rectangle(hdcBuffer, rect.left, rect.top, rect.right, rect.bottom);
	TextOut(hdcBuffer, ToWindowX(0.48), ToWindowY(0.48), _T("Option"), 6);

	zoomWidget(WelcomeButtons[2].geo, WelcomeButtons[2].zoom, &rect);
	Rectangle(hdcBuffer, rect.left, rect.top, rect.right, rect.bottom);
	TextOut(hdcBuffer, ToWindowX(0.48), ToWindowY(0.73), _T("Exit"), 4);
}
