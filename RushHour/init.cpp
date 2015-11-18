#include "audio.h"
#include "osureader.h"

VOID GameFolderInit()
{
	global.totalSongCount = 0;
	global.currentSong = 1;
	HANDLE findFile;
	WIN32_FIND_DATA fNextInfo;
	findFile = FindFirstFile(_T("*"), &fNextInfo);
	WCHAR firstOsuFile[60];
	while(FindNextFile(findFile, &fNextInfo))
	{
		if(fNextInfo.cFileName[0] == '.')
			continue;
		else if(fNextInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			SetCurrentDirectory(fNextInfo.cFileName);
			HANDLE findOsuFile;
			WIN32_FIND_DATA fOsuInfo;
			wsprintf(firstOsuFile, TEXT("%s.osu"), _T("*"));
			findOsuFile = FindFirstFile(firstOsuFile, &fOsuInfo);
			global.songs.push_back(SONGINFO());
			if(findOsuFile != INVALID_HANDLE_VALUE)
			{
				global.songs[global.totalSongCount].osuFile = fOsuInfo.cFileName;
				readBasicInfo(global.songs[global.totalSongCount].osuFile.data(), &global.songs[global.totalSongCount]);
			}
			FindClose(findOsuFile);
			SetCurrentDirectory(_T(".."));

			global.songs[global.totalSongCount].name = fNextInfo.cFileName;
			global.totalSongCount++;
		}
	}
	FindClose(findFile);
}

VOID GlobalInit()
{
	QueryPerformanceFrequency(&global.clockFrequency);

	GameFolderInit();
	AudioInit();
	SwitchSong();

	global.status = global.GS_SONGSELECT;
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
	}

	//创建游戏状态
	m_gameStatus = CreateGameStatus(0, 0, 40, 30, m_hGameStatusBmp);

	//启动计时器
	SetTimer(hWnd, TIMER, 20, NULL);
}

VOID GameInit()
{
	SwitchSong();
	readBeats((global.currSong().name + _T("/") + global.currSong().osuFile).data());
	QueryPerformanceCounter(&global.beginTime);
	global.status = global.GS_PLAYING;
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

VOID CreateTerrians()
{
	;
}