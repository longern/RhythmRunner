#ifndef MAIN_H
#define MAIN_H

#define _CRT_SECURE_NO_WARNINGS
const double PI = 3.1415926535897932;

#include <windows.h>

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <string>
#include <tchar.h>

#include "resource.h"

/*定义宏变量，WNDWIDTH、WNDHEIGHT为窗口的宽和高*/
#define TIMER                1
#define WNDWIDTH             1280
#define WNDHEIGHT            768
#define WNDTITLEBARHEIGHT    32
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      10

typedef struct
{
	int beatmapSetId;
	std::wstring name;
	std::wstring title;
	std::wstring audioFilename;
	std::wstring osuFile;
	std::wstring bgImgFile;
	DOUBLE msPerBeat;
	INT audioLeadIn;
	INT mciOffset;
	UINT previewTime;
	DOUBLE sliderMultiplier;
} SONGINFO;

typedef struct
{
	LONG msecs;
	int type;
	int height;
} BARRIERINFO;

typedef struct
{
	DOUBLE height;
	INT jpStartTime;
	UINT jpCount;
	DOUBLE startHeight;
} HERO;

struct GLOBAL
{
	enum { GS_NOSONG = 0, GS_WELCOME, GS_SONGSELECT, GS_OPTIONS, GS_PLAYING, GS_GAMEOVER } status;
	std::vector<SONGINFO> songs;
	UINT totalSongCount;
	UINT currentSong;

	LONG accummulatedTime;
	LARGE_INTEGER beginTime;
	LARGE_INTEGER clockFrequency;
	BOOL isGamePaused;
	std::vector<BARRIERINFO> barriers[4];
	HERO heroes[4];
	DOUBLE blood;
	DOUBLE fHeight[4];
	std::vector<INT> keySequence[4];
	LONG finalTime;
	DOUBLE finalScore;

	MCI_OPEN_PARMS ae; //Audio Engine

	SONGINFO &currSong() { return songs[currentSong - 1]; }

	LONG timePass();

	UINT heroWidth;
	UINT heroHeight;
};

typedef struct
{
	HBITMAP hero[9];
	HBITMAP wHero[9];
	HBITMAP note[2];
	HBITMAP noise[2];
} GAMERESOURCE;

typedef struct
{
	BOOL foggyMode;
	BOOL hideJudgeLine;
	INT universalOffset;
} GAMESETTINGS;

/*  Global Variants  */
extern GLOBAL global;
extern GAMERESOURCE resource;
extern GAMESETTINGS settings;

extern TCHAR szWindowClass[];
extern TCHAR szTitle[];

extern ULONG_PTR m_gdiplusToken;

/*  Global Functions  */
VOID GlobalInit();
VOID GameInit();
VOID GameOverInit();

//  Process Events;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//  Init window, load image resources
VOID WindowInit(HWND hWnd, WPARAM wParam, LPARAM lParam);

//  Core Rendering Function
VOID Render(HWND hWnd);
//  Timer Triggered Event
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//  Key Press Down Event
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//  Mouse Move Event
VOID MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
//  Left Mouse Button Press Down Event
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//  Touch Screen Event
LRESULT TouchEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//Global Functions
int ToWindowX(double rx);
int ToWindowY(double ry);
BOOL Circle(HDC hdc, int x, int y, int r);
BOOL DrawSquare(HDC hdc, int cx, int cy, int ha);
VOID PreviewSong();
VOID SwitchSong();

#endif //  MAIN_H
