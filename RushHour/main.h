#ifndef MAIN_H
#define MAIN_H

#define _CRT_SECURE_NO_WARNINGS
const double PI = 3.1415926536;

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
	std::wstring name;
	std::wstring title;
	std::wstring audioFilename;
	std::wstring osuFile;
	std::wstring bgImgFile;
	DOUBLE msPerBeat;
	INT audioLeadIn;
	UINT previewTime;
	DOUBLE sliderMultiplier;
} SONGINFO;

typedef struct
{
	LONG msecs;
	UINT track;
	UINT type;
	INT height;
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

	LARGE_INTEGER beginTime;
	LARGE_INTEGER clockFrequency;
	std::vector<BARRIERINFO> barriers;
	HERO heroes[4];
	DOUBLE blood;
	DOUBLE fHeight[4];
	UINT finalScore;

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
} GAMERESOURCE;

/*全局变量*/
extern GLOBAL global;
extern GAMERESOURCE resource;

extern TCHAR szWindowClass[];
extern TCHAR szTitle[];

/*全局函数*/
VOID GlobalInit();
VOID GameInit();
VOID GameOverInit();

//窗体过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//初始化
VOID WindowInit(HWND hWnd, WPARAM wParam, LPARAM lParam);

//双缓冲绘制
VOID Render(HWND hWnd);
//定时器事件
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//键盘按下事件处理
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//左鼠标点击事件
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

//Global Functions
int ToWindowX(double rx);
int ToWindowY(double ry);
BOOL Circle(HDC hdc, int x, int y, int r);
VOID PreviewSong();
VOID SwitchSong();

#endif //  MAIN_H
