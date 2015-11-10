#include "main.h"

VOID GlobalInit()
{
	global.totalSongCount = 0;
	global.currentSong = 1;
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

    global.ae.dwCallback = NULL;
    global.ae.lpstrAlias = NULL;
    global.ae.lpstrDeviceType =_T("MPEGAudio");
    global.ae.lpstrElementName = _T("50669 fripSide - only my railgun (TV Size)/only my railgun (TV size).mp3");
    global.ae.wDeviceID = NULL;
    UINT rs;
    rs = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&global.ae);
    if (!rs)
    {
        MCI_PLAY_PARMS pp;
        pp.dwCallback = NULL;
        pp.dwFrom = 0;
		mciSendCommand(global.ae.wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_DGV_PLAY_REPEAT, (DWORD)&pp);
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