#include "main.h"

GLOBAL global;

TCHAR szWindowClass[] = _T("win32app");
TCHAR szTitle[] = _T("Running Rhythm");

/*����λͼ���*/
HBITMAP m_hBackgroundBmp;
HBITMAP m_hBuildingBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP	m_hBlockBmp[BLOCK_COLOR_NUM];
HBITMAP	m_hRoofkBmp[ROOF_COLOR_NUM];

/*���巽����ɫ���飬��m_hBlockBmp[BLOCK_COLOR_NUM]������Ӧ��0��ʾ��ɫ���飬1��ʾ��ɫ���飬2��ʾ��ɫ���飬3��ʾ��ɫ����*/
int	m_blockBmpNames[] = { IDB_BLUE_BLOCK, IDB_GREEN_BLOCK, IDB_ORANGE_BLOCK, IDB_PINK_BLOCK };
/*�����ݶ���ɫ���飬��m_hRoofkBmp[ROOF_COLOR_NUM]������Ӧ��0��ʾ��ɫ�ݶ���1��ʾ��ɫ�ݶ�*/
int	m_roofBmpNames[] = { IDB_BLACK_ROOF, IDB_GREY_ROOF };

/*����Ӣ�ۡ����������Ρ���Ϸ״̬*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus    m_gameStatus;