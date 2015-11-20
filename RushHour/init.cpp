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
	while (FindNextFile(findFile, &fNextInfo))
	{
		if (fNextInfo.cFileName[0] == '.')
			continue;
		else if (fNextInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			SetCurrentDirectory(fNextInfo.cFileName);
			HANDLE findOsuFile;
			WIN32_FIND_DATA fOsuInfo;
			wsprintf(firstOsuFile, TEXT("%s.osu"), _T("*"));
			findOsuFile = FindFirstFile(firstOsuFile, &fOsuInfo);
			global.songs.push_back(SONGINFO());
			if (findOsuFile != INVALID_HANDLE_VALUE)
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
	//加载Hero位图
	resource.hero[0] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO1));
	resource.hero[1] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO2));
	resource.hero[2] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO3));
	resource.hero[3] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO4));
	resource.hero[4] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO5));
	resource.hero[5] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO6));
	resource.hero[6] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO7));
	resource.hero[7] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO8));
	resource.hero[8] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO9));

	//启动计时器
	SetTimer(hWnd, TIMER, 20, NULL);
}

VOID GameInit()
{
	readBeats((global.currSong().name + _T("/") + global.currSong().osuFile).data());
	QueryPerformanceCounter(&global.beginTime);
	SwitchSong();
	global.status = global.GS_PLAYING;
}
