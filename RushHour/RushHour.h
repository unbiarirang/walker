#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include <time.h> 
#include "resource.h"
#include <mmsystem.h>       //  澜厩 犁积      // 
#pragma comment(lib,"winmm.lib") 

//#include <fmod.h>                             //FMOD
//#include <fmod_errors.h>
//#include <fmod_output.h>
#include "inc/fmod.hpp"                       //FMOD
#pragma comment (lib,"fmod_vc.lib")
using namespace FMOD;


/*定义宏变量，WNDWIDTH、WNDHEIGHT为窗口的宽和高*/
#define TIMER                1
#define WNDWIDTH             600
#define WNDHEIGHT            350
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      10
#define HERO_NUMBER          3

/*英雄结构体，英雄位图、位置、大小*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
}Hero;

typedef struct           //官捞农
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     maxFrameSize;
}Bike;

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
}Terrian;

/*背景建筑结构体，建筑位图、位置、大小、类型*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
}Building;

typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
}Menu;

typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
}Cloud;

/*游戏状态结构体*/
typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isPaused;
	int     totalDist;
}GameStatus;

typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isStart;
	int     totalDist;
}GameStart;

/*全局变量*/
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Walker");

/*声明位图句柄*/
HBITMAP m_hBackgroundBmp;
HBITMAP m_hBuildingBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP m_hStartBmp;
HBITMAP	m_hBlockBmp[BLOCK_COLOR_NUM];
HBITMAP	m_hRoofkBmp[ROOF_COLOR_NUM];
HBITMAP m_hPictureBmp;
HBITMAP m_hBikeBmp;
HBITMAP m_hCloudBmp;
HBITMAP m_hCloud3Bmp;
HBITMAP m_hLightBmp;
HBITMAP m_hMenuBmp;
HBITMAP m_hChar1Bmp;
HBITMAP m_hChar2Bmp;
HBITMAP m_hChar3Bmp;
HBITMAP m_hHeroCharBmp[HERO_NUMBER];
HBITMAP m_hDeveloperBmp;
HBITMAP m_hPicture2Bmp;
HBITMAP m_hPicture3Bmp;
HBITMAP m_hPicture4Bmp;
HBITMAP m_hPicture5Bmp;
HBITMAP m_hAgainBmp;
HBITMAP m_hFinishBmp;
HBITMAP m_hBackBmp;
HBITMAP m_hWingBmp;
HBITMAP m_hSoundBmp;
HBITMAP m_hStarBmp;
HBITMAP m_hIslandBmp;
HBITMAP m_hLifeBmp;
HBITMAP m_hEasyBmp;
HBITMAP m_hHardBmp;
HBITMAP m_hHyj1Bmp;
HBITMAP m_hHyj2Bmp;
HBITMAP m_hHyj3Bmp;


/*定义方块颜色数组，与m_hBlockBmp[BLOCK_COLOR_NUM]个数对应，0表示蓝色方块，1表示绿色方块，2表示橙色方块，3表示粉色方块*/
int	m_blockBmpNames[] = { IDB_BLUE_BLOCK, IDB_GREEN_BLOCK, IDB_ORANGE_BLOCK, IDB_PINK_BLOCK };
/*定义屋顶颜色数组，与m_hRoofkBmp[ROOF_COLOR_NUM]个数对应，0表示黑色屋顶，1表示灰色屋顶*/
int	m_roofBmpNames[] = { IDB_BLACK_ROOF, IDB_GREY_ROOF };
int	m_herocharBmpNames[] = { IDB_HYJ1, IDB_HYJ2, IDB_HYJ3 };

/*声明英雄、建筑、地形、游戏状态*/
Hero          m_hero;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
GameStatus    m_gameStatus;
GameStatus    m_sound;
Bike		  m_bike;
Cloud		  m_cloud;
Cloud		  m_cloud2;
Cloud		  m_cloud3;
Cloud		  m_light;
GameStart	  m_start;
Menu		  m_menu;
GameStart	  m_char1;
GameStart	  m_char2;
GameStart     m_char3;
GameStart     m_developer;
Hero		  m_herochar[HERO_NUMBER];
GameStart     m_again;
GameStart     m_finish;
GameStart     m_back;
Cloud		  m_wing;
Cloud         m_star[MAX_TERRIAN_NUM];
Cloud         m_island[MAX_TERRIAN_NUM];
Menu		  m_life;
GameStart     m_easy;
GameStart     m_hard;

/*全局函数*/
//窗体过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//初始化
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : 创建英雄飞机
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
	curFrameIndex当前帧
	maxFrameSize最大帧率
*************************************************/ 
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize);
Bike CreateBike(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize);

/*************************************************
Fucntion : 创建背景建筑
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
Cloud CreateCloud(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
Menu CreateMenu(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建游戏状态
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
GameStart CreateGameStart(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

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
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight);

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
BOOL Start(POINT);
BOOL Char1(POINT);
BOOL Char2(POINT);
BOOL Char3(POINT);
BOOL Developer(POINT);
BOOL Again(POINT);
BOOL Finish(POINT);
BOOL Back(POINT);
BOOL SoundPaused(POINT);
BOOL Easy(POINT);
BOOL Hard(POINT);

//键盘按下事件处理
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//键盘松开事件处理
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
//左鼠标点击事件
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//官捞农
VOID BikeUpdate();
VOID CloudUpdate();

//郴啊 父惦
#define SET_TIMER_VALUE			50                                    // 20 times / sec
#define COUNT_PER_SEC			(1000/SET_TIMER_VALUE)
#define WING_COOL_TIME          300                                   //朝俺 酒捞袍 酿鸥烙 15檬                   
#define WING_APPEAR_TIME        (WNDWIDTH + 100) / (4 + Level)        //朝俺 酒捞袍捞 免泅窍绰 矫埃             
#define WING_MAINTAIN_TIME		100                                   //朝俺 酒捞袍 5檬 瘤加

//
int RevivalY[] = { 0, 0, 0, 0, 0, 0, 0, 0, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2 };
int HeroJumpY[] = { 0, 3, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0,														//痢橇 惑氢
					-1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,     //痢橇饶 窍碍
					-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, //朝俺饶 窍碍
					0, 0, 0, 0, 0, 0, 0, 0, -1, -1, -1, -1, -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2  //何劝饶 窍碍
					-2, -2, -2, -2, -2, -2, -2, -2, -2, -2};											      

int Up_key_pressed = 0; // init value is 0
int Up_key_count = 0; // init value is 0

#define SCREEN_STATUS_START			1
#define SCREEN_STATUS_DIFFICULTY	2
#define SCREEN_STATUS_CHAR			3
#define SCREEN_STATUS_DEVELOPER		4
#define SCREEN_STATUS_GAME			5
#define SCREEN_STATUS_END			6

#define CHAR_STATUS_CHAR1		0
#define CHAR_STATUS_CHAR2		1
#define CHAR_STATUS_CHAR3		2

#define DIFFICULTY_STATUS_EASY   1
#define DIFFICULTY_STATUS_HARD   2

int Now_screen_status = SCREEN_STATUS_START; // init value
int Now_char_status = 0;
int Now_difficulty_status = 0;
int height_now, height_soon, posx_now, posx_soon;
int Level = 0, random_number;
int flying_status = 0, wing_appear_count = 0, wing_maintain_count = 0, wing_cool_count = 0;
int star_appear[10] = { 1,1,1,1,1,1,1,1,1,1 };
int terrian_height[] = { WNDHEIGHT + 100, 150, 200, 250 };
int life_frame = 0;

VOID Render_Start(HWND hWnd);
//HCURSOR WINAPI LoadCursor(HINSTANCE hInstance, LPCTSTR lpCursorName);

VOID WriteHighScore(int high_score);                                //窍捞 胶内绢
VOID WriteHighScoreHard(int high_score_hard);
int ReadHighScore();
int ReadHighScoreHard();

int high_score, high_score_hard;
int EffSoundStatus = 1;
int runningflag = 0;


//            FMOD            //

enum SOUNDKIND
{
	SD_1 = 0,
	SD_2,
	SD_3,
	SD_4,
	SD_5,
	SD_6,
	SD_END
};

enum EFFSOUNDKIND
{
	effSD_1 = 0,
	effSD_2,
	effSD_3,
	effSD_END
};

VOID Sound_Setup();                                      
VOID EffSound_Setup();   

VOID PlaySound(SOUNDKIND eSound);
VOID PlayEffSound(EFFSOUNDKIND eSound);
VOID PlayScreamSound();

VOID StopSound();

VOID ReleaseSound();

static FMOD_SYSTEM *g_pSystem;                       //FMOD sound
static FMOD_SOUND *g_pSound[SD_END];
static FMOD_CHANNEL *g_pChannel[SD_END];

static FMOD_SYSTEM *g_pEffSystem;                       //FMOD effsound
static FMOD_SOUND *g_pEffSound[effSD_END];
static FMOD_CHANNEL *g_pEffChannel[effSD_END];
