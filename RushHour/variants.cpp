#include "main.h"

GLOBAL global;

TCHAR szWindowClass[] = _T("win32app");
TCHAR szTitle[] = _T("Running Rhythm");

/*声明位图句柄*/
HBITMAP m_hBackgroundBmp;
HBITMAP m_hBuildingBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP	m_hBlockBmp[BLOCK_COLOR_NUM];
HBITMAP	m_hRoofkBmp[ROOF_COLOR_NUM];

/*定义方块颜色数组，与m_hBlockBmp[BLOCK_COLOR_NUM]个数对应，0表示蓝色方块，1表示绿色方块，2表示橙色方块，3表示粉色方块*/
int	m_blockBmpNames[] = { IDB_BLUE_BLOCK, IDB_GREEN_BLOCK, IDB_ORANGE_BLOCK, IDB_PINK_BLOCK };
/*定义屋顶颜色数组，与m_hRoofkBmp[ROOF_COLOR_NUM]个数对应，0表示黑色屋顶，1表示灰色屋顶*/
int	m_roofBmpNames[] = { IDB_BLACK_ROOF, IDB_GREY_ROOF };

/*声明英雄、建筑、地形、游戏状态*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus    m_gameStatus;