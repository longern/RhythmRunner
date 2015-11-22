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

/*����������WNDWIDTH��WNDHEIGHTΪ���ڵĿ�͸�*/
#define TIMER                1
#define WNDWIDTH             1280
#define WNDHEIGHT            768
#define WNDTITLEBARHEIGHT    32
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      10

/*Ӣ�۽ṹ�壬Ӣ��λͼ��λ�á���С*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
} Hero;

/*���νṹ�壬���η���λͼ���ݶ�λͼ��λ�á���С���ݶ��Լ������С���������*/
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

/*���������ṹ�壬����λͼ��λ�á���С������*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
} Building;

/*��Ϸ״̬�ṹ��*/
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
	DOUBLE height;
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
	enum { GS_WELCOME = 0, GS_SONGSELECT, GS_OPTIONS, GS_PLAYING } status;
	std::vector<SONGINFO> songs;
	UINT totalSongCount;
	UINT currentSong;

	LARGE_INTEGER beginTime;
	LARGE_INTEGER clockFrequency;
	std::vector<BARRIERINFO> barriers;
	HERO heroes[4];

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

/*ȫ�ֱ���*/
extern GLOBAL global;
extern GAMERESOURCE resource;

extern TCHAR szWindowClass[];
extern TCHAR szTitle[];

/*����Ӣ�ۡ����������Ρ���Ϸ״̬*/
extern Hero          m_hero;
extern Building      m_building;
extern Terrian       m_terrian[MAX_TERRIAN_NUM];
extern GameStatus    m_gameStatus;

/*ȫ�ֺ���*/

VOID GlobalInit();
VOID GameInit();

//������̺���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//��ʼ��
VOID WindowInit(HWND hWnd, WPARAM wParam, LPARAM lParam);

//˫�������
VOID Render(HWND hWnd);
//��ʱ���¼�
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//hero����
VOID HeroUpdate();
//���θ���
VOID TerrianUpdate();
//��Ϸ״̬����
VOID GameStatusUpdate();
//�ж��Ƿ�����ͣ
BOOL Paused(POINT);
//���̰����¼�����
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//��������¼�
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

//Global Functions
int ToWindowX(double rx);
int ToWindowY(double ry);
BOOL Circle(HDC hdc, int x, int y, int r);
VOID PreviewSong();
VOID SwitchSong();

#endif //  MAIN_H