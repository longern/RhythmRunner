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

/*ȫ�ֱ���*/
extern GLOBAL global;

extern TCHAR szWindowClass[];
extern TCHAR szTitle[];

/*����λͼ���*/
extern HBITMAP m_hBackgroundBmp;
extern HBITMAP m_hBuildingBmp;
extern HBITMAP m_hHeroBmp;
extern HBITMAP m_hGameStatusBmp;
extern HBITMAP m_hBlockBmp[BLOCK_COLOR_NUM];
extern HBITMAP m_hRoofkBmp[ROOF_COLOR_NUM];

/*���巽����ɫ���飬��m_hBlockBmp[BLOCK_COLOR_NUM]������Ӧ��0��ʾ��ɫ���飬1��ʾ��ɫ���飬2��ʾ��ɫ���飬3��ʾ��ɫ����*/
extern int m_blockBmpNames[];
/*�����ݶ���ɫ���飬��m_hRoofkBmp[ROOF_COLOR_NUM]������Ӧ��0��ʾ��ɫ�ݶ���1��ʾ��ɫ�ݶ�*/
extern int m_roofBmpNames[];

/*����Ӣ�ۡ����������Ρ���Ϸ״̬*/
extern Hero          m_hero;
extern Building      m_building;
extern Terrian       m_terrian[MAX_TERRIAN_NUM];
extern GameStatus    m_gameStatus;

/*ȫ�ֺ���*/

VOID GlobalInit();

//������̺���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//��ʼ��
VOID WindowInit(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : ����Ӣ��
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
	curFrameIndex��ǰ֡
	maxFrameSize���֡��
*************************************************/ 
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : ������������
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
*************************************************/ 
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : ������Ϸ״̬
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
*************************************************/ 
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : ����Ӣ�۷ɻ�
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBlockBmp��ʾ����λͼ���
	hRoofBmp��ʾ�ݶ�λͼ���
	roofWidth��roofHeight�ݶ���Ⱥ͸߶�
	blockWidth��blockHeight�����Ⱥ͸߶�
*************************************************/ 
VOID CreateTerrian();

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
VOID SwitchSong();

#endif //  MAIN_H