#include "main.h"

GLOBAL global;
GAMERESOURCE resource;

TCHAR szWindowClass[] = _T("win32app");
TCHAR szTitle[] = _T("Rhythm Runner");

/*����Ӣ�ۡ����������Ρ���Ϸ״̬*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus    m_gameStatus;
