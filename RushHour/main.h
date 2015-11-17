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

/*英雄结构体，英雄位图、位置、大小*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
} Hero;

/*地形结构体，地形方块位图、屋顶位图、位置、大小、屋顶以及方块大小、方块个数*/
typedef struct
{
	HBITMAP hBlockBmp;
	HBITMAP hRoofBmp;
	POINT	pos;
	SIZE    size;
	int     roofWidth;
	int     roofHeight;
	int     blockWidth;
	int     blockHeight;
	int     blockNum;
} Terrian;

/*背景建筑结构体，建筑位图、位置、大小、类型*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
} Building;

/*游戏状态结构体*/
typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isPaused;
	int     totalDist;
} GameStatus;

typedef struct
{
	std::wstring name;
	std::wstring title;
	std::wstring audioFilename;
	std::wstring osuFile;
	std::wstring bgImgFile;
} SONGINFO;

typedef struct
{
	UINT msecs;
	UINT track;
	UINT type;
} BARRIERINFO;

typedef struct
{
	std::vector<int> timingPoints;
} BEATSINFO;

typedef struct 
{
	enum { GS_WELCOME = 0, GS_SONGSELECT, GS_OPTIONS, GS_PLAYING } status;
	std::vector<SONGINFO> songs;
	std::vector<BARRIERINFO> barriers;
	UINT totalSongCount;
	UINT currentSong;

	SONGINFO &currSong() { return songs[currentSong - 1]; }

	MCI_OPEN_PARMS ae; //Audio Engine
} GLOBAL;

/*全局变量*/
extern GLOBAL global;

extern TCHAR szWindowClass[];
extern TCHAR szTitle[];

/*声明位图句柄*/
extern HBITMAP m_hBackgroundBmp;
extern HBITMAP m_hBuildingBmp;
extern HBITMAP m_hHeroBmp;
extern HBITMAP m_hGameStatusBmp;
extern HBITMAP m_hBlockBmp[BLOCK_COLOR_NUM];
extern HBITMAP m_hRoofkBmp[ROOF_COLOR_NUM];

/*定义方块颜色数组，与m_hBlockBmp[BLOCK_COLOR_NUM]个数对应，0表示蓝色方块，1表示绿色方块，2表示橙色方块，3表示粉色方块*/
extern int m_blockBmpNames[];
/*定义屋顶颜色数组，与m_hRoofkBmp[ROOF_COLOR_NUM]个数对应，0表示黑色屋顶，1表示灰色屋顶*/
extern int m_roofBmpNames[];

/*声明英雄、建筑、地形、游戏状态*/
extern Hero          m_hero;
extern Building      m_building;
extern Terrian       m_terrian[MAX_TERRIAN_NUM];
extern GameStatus    m_gameStatus;

/*全局函数*/

VOID GlobalInit();

//窗体过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//初始化
VOID WindowInit(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : 创建英雄
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
	curFrameIndex当前帧
	maxFrameSize最大帧率
*************************************************/ 
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : 创建背景建筑
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建游戏状态
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建英雄飞机
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBlockBmp表示方块位图句柄
	hRoofBmp表示屋顶位图句柄
	roofWidth、roofHeight屋顶宽度和高度
	blockWidth、blockHeight方块宽度和高度
*************************************************/ 
VOID CreateTerrian();

//双缓冲绘制
VOID Render(HWND hWnd);
//定时器事件
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//hero更新
VOID HeroUpdate();
//地形更新
VOID TerrianUpdate();
//游戏状态更新
VOID GameStatusUpdate();
//判断是否点击暂停
BOOL Paused(POINT);
//键盘按下事件处理
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//左鼠标点击事件
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

//Global Functions
int ToWindowX(double rx);
int ToWindowY(double ry);
BOOL Circle(HDC hdc, int x, int y, int r);
VOID SwitchSong();

#endif //  MAIN_H