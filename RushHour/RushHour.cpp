#define _CRT_SECURE_NO_WARNINGS

#include "RushHour.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#pragma comment(lib, "winmm.lib")

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	SetCurrentDirectory(_T("../Release"));

	GlobalInit();

	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Mega Plane"),
			MB_OK);

		return 1;
	}

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create,~WS_THICKFRAME  fixed window size
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// WNDWIDTH, WNDHEIGHT: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, //设置窗口样式，不可改变大小，不可最大化
		CW_USEDEFAULT, CW_USEDEFAULT,
		WNDWIDTH, WNDHEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Mega Plane"),
			MB_OK);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		//初始化
		WindowInit(hWnd, wParam, lParam);
		break;
	case WM_PAINT:
		//绘制
		Render(hWnd);
		break;
	case WM_KEYDOWN:
		//键盘按下事件
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		//键盘松开事件
		KeyUp(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		//左鼠标事件
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		//定时器事件
		TimerUpdate(hWnd, wParam, lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

VOID GlobalInit()
{
	global.totalSongCount = 0;
	HANDLE findFile;
	WIN32_FIND_DATA fNextInfo;
	findFile = FindFirstFile(_T("*"), &fNextInfo);
	while(FindNextFile(findFile, &fNextInfo))
	{
		if(fNextInfo.cFileName[0] == '.')
			continue;
		else if(fNextInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			wcscpy(global.songNames[global.totalSongCount], fNextInfo.cFileName);
			global.totalSongCount++;
		}
	}

	global.status = global.GS_SONGSELECT;

	MCI_OPEN_PARMS op;
    op.dwCallback = NULL;
    op.lpstrAlias = NULL;
    op.lpstrDeviceType =_T("MPEGAudio");
    op.lpstrElementName = _T("33688 DJ Okawari - Flower Dance/Flower Dance.mp3");
    op.wDeviceID = NULL;
    UINT rs;
    rs = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT ,(DWORD)&op);
    if (!rs)
    {
        MCI_PLAY_PARMS pp;
        pp.dwCallback = NULL;
        pp.dwFrom = 0;
        mciSendCommand(op.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)&pp);
	}
}

VOID WindowInit(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//加载背景位图
	m_hBackgroundBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BACKGROUND));
	//加载Building位图
	m_hBuildingBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BUILDING));
	//加载Building位图
	m_hHeroBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO));
	//加载游戏状态位图
	m_hGameStatusBmp =  LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GAME_STATUS));
	//加载Block位图
	int k;
	for (k = 0; k < BLOCK_COLOR_NUM; ++k)
	{
		m_hBlockBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(m_blockBmpNames[k]));
	}
	//加载屋顶位图
	for (k = 0; k < ROOF_COLOR_NUM; ++k)
	{
		m_hRoofkBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(m_roofBmpNames[k]));
	}

	//创建英雄、建筑
	m_hero = CreateHero(50, 100, 30, 50, m_hHeroBmp, 0, 3);
	m_building = CreateBuilding(0, 70, 500, 250, m_hBuildingBmp);
	//创建地形
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		if (k % 4 == 0)
		{
			continue;
		}
		m_terrian[k] = CreateTerrian(k * 65, 100 + 50 * (k % 2), 65, 300, m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], 33, 65);
	}

	//创建游戏状态
	m_gameStatus = CreateGameStatus(0, 0, 40, 30, m_hGameStatusBmp);

	//启动计时器
	SetTimer(hWnd, TIMER, 30, NULL);
}

VOID RenderWelcome(HDC hdcBuffer, HDC hdcBmp)
{

}

VOID RenderSongSelect(HDC hdcBuffer, HDC hdcBmp)
{
	UINT i;
	SetTextColor(hdcBuffer, RGB(255, 255, 255));
	SetBkMode(hdcBuffer, TRANSPARENT);
	for(i = 0; i < global.totalSongCount; i++)
		TextOut(hdcBuffer, WNDWIDTH - 400, 15 + 25 * i, global.songNames[i], wcslen(global.songNames[i]));
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

Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize)
{
	Hero hero;
	hero.hBmp = hBmp;
	hero.pos.x = posX;
	hero.pos.y = posY;
	hero.size.cx = sizeX;
	hero.size.cy = sizeY;
	hero.curFrameIndex = curFrameIndex;
	hero.maxFrameSize = maxFrameSize;
	return hero;
}

Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Building building;
	building.hBmp = hBmp;
	building.pos.x = posX;
	building.pos.y = posY;
	building.size.cx = sizeX;
	building.size.cy = sizeY;
	return building;
}

GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	GameStatus gameStatus;
	gameStatus.pos.x = posX;
	gameStatus.pos.y = posY;
	gameStatus.size.cx = sizeX;
	gameStatus.size.cy = sizeY;
	gameStatus.hBmp = hBmp;
	gameStatus.totalDist = 0;
	gameStatus.isPaused = false;
	return gameStatus;
}

Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight)
{
	Terrian terrian;
	terrian.pos.x = posX;
	terrian.pos.y = posY;
	terrian.size.cx = sizeX;
	terrian.size.cy = sizeY;
	terrian.hBlockBmp = hBlockBmp;
	terrian.hRoofBmp = hRoofBmp;
	terrian.roofWidth = sizeX;
	terrian.roofHeight = roofHeight;
	terrian.blockWidth = sizeX;
	terrian.blockHeight = blockHeight;
	terrian.blockNum = (int)ceil((sizeY - roofHeight) * 1.0 / blockHeight); 
	return terrian;
}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HeroUpdate();
	TerrianUpdate();
	GameStatusUpdate();
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

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch(global.status)
	{
	case global.GS_SONGSELECT:
		break;

	case global.GS_PLAYING:
		switch (wParam)
		{
		case VK_UP:
			m_hero.pos.y -= 50;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		default:
			break;
		}
		break;
	}
}

VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:
		m_hero.pos.y += 50;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case VK_DOWN:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		break;
	}
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	//如果点击了继续或暂停图标
	if (Paused(ptMouse))
	{
		if (!m_gameStatus.isPaused)
		{
			KillTimer(hWnd, TIMER);
			m_gameStatus.isPaused = TRUE;
		}
		else
		{
			SetTimer(hWnd, TIMER, 50, NULL);
			m_gameStatus.isPaused = FALSE;
		}
		InvalidateRect(hWnd, NULL, FALSE);
	}
}