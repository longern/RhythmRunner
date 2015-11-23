#include "main.h"

GLOBAL global;
GAMERESOURCE resource;

TCHAR szWindowClass[] = _T("win32app");
TCHAR szTitle[] = _T("Rhythm Runner");

/*声明英雄、建筑、地形、游戏状态*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus    m_gameStatus;
