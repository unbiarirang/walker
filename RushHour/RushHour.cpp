#include "RushHour.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("WalKer"),
			MB_OK);

		return 1;
	}

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create,~WS_THICKFRAME  fixed window size
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// WNDWIDTH, WNDHEIGHT: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX, //ÉèÖÃ´°¿ÚÑùÊ½£¬²»¿É¸Ä±ä´óÐ¡£¬²»¿É×î´ó»¯
		400, 200,
		WNDWIDTH, WNDHEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("WalKer"),
			MB_OK);

		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);


	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		//³õÊ¼»¯
		Init(hWnd, wParam, lParam);
		Sound_Setup();
		EffSound_Setup();

		//PlaySound(TEXT(".\\res\\sound\\raindrop_flower.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		break;
	case WM_MOUSEMOVE:

		POINT ptMouse;
		ptMouse.x = LOWORD(lParam);
		ptMouse.y = HIWORD(lParam);

		if (Now_screen_status == SCREEN_STATUS_START)
		{
			if (Start(ptMouse))
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else if (Developer(ptMouse))
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
		}
		else if (Now_screen_status == SCREEN_STATUS_DIFFICULTY)
		{
			if (Easy(ptMouse) || Hard(ptMouse) || Back(ptMouse))
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
		}
		else if (Now_screen_status == SCREEN_STATUS_CHAR)
		{
			if (Char1(ptMouse) || Char2(ptMouse) || Char3(ptMouse))
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else if (Back(ptMouse))
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
		}
		else if (Now_screen_status == SCREEN_STATUS_DEVELOPER)
		{
			if (Back(ptMouse))
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
		}
		else if (Now_screen_status == SCREEN_STATUS_GAME)
		{
			if (Paused(ptMouse) || SoundPaused(ptMouse))
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
		}
		else if (Now_screen_status == SCREEN_STATUS_END)
		{
			if (Again(ptMouse) || Finish(ptMouse))
			{
				SetCursor(LoadCursor(NULL, IDC_HAND));
			}
			else
			{
				SetCursor(LoadCursor(NULL, IDC_ARROW));
			}
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}

	case WM_PAINT:

		if (Now_screen_status == SCREEN_STATUS_START)
		{
			Render_Start(hWnd);
			break;
		}
		else if (Now_screen_status == SCREEN_STATUS_DEVELOPER)
		{
			Render_Start(hWnd);
			break;
		}
		else if (Now_screen_status == SCREEN_STATUS_CHAR)
		{
			Render_Start(hWnd);
			break;
		}
		else if (Now_screen_status == SCREEN_STATUS_DIFFICULTY)
		{
			Render_Start(hWnd);
			break;
		}
		else if (Now_screen_status == SCREEN_STATUS_GAME)
		{
			//»æÖÆ
			Render(hWnd);
			break;
		}
		else if (Now_screen_status == SCREEN_STATUS_END)
		{
			Render(hWnd);			
			break;
		}

	case WM_KEYDOWN:
		//¼üÅÌ°´ÏÂÊÂ¼þ
		KeyDown(hWnd, wParam, lParam);
		break;
	case WM_KEYUP:
		//¼üÅÌËÉ¿ªÊÂ¼þ
		//KeyUp(hWnd, wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		//×óÊó±êÊÂ¼þ
		LButtonDown(hWnd, wParam, lParam);
		break;
	case WM_TIMER:
		
		if (Now_screen_status == SCREEN_STATUS_GAME)
		{
			//¶¨Ê±Æ÷ÊÂ¼þ
			TimerUpdate(hWnd, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		ReleaseSound();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}

VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (Now_difficulty_status == DIFFICULTY_STATUS_HARD)
	{
		srand((unsigned)time(NULL));                   //³­ÀÌµµ HARD½Ã¿¡ ·£´ý ½Ãµå »ý¼º (EASY´Â ¸Ê ¾Èº¯ÇÔ)
	}

	//¼ÓÔØ±³¾°Î»Í¼
	//m_hBackgroundBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
	//MAKEINTRESOURCE(IDB_BACKGROUND));
	m_hPictureBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_PICTURE));
	m_hPicture2Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_PICTURE2));
	m_hPicture3Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_PICTURE3)); 
	m_hPicture4Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(IDB_PICTURE4));
	m_hPicture5Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_PICTURE5));
	//¼ÓÔØBuildingÎ»Í¼
	//m_hBuildingBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstancfe,
	//MAKEINTRESOURCE(IDB_BUILDING));
	m_hCloudBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_CLOUD1));
	m_hLightBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_LIGHT));
	//¼ÓÔØBuildingÎ»Í¼
	m_hHeroBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HERO));
	m_hBikeBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BIKE));
	//¼ÓÔØÓÎÏ·×´Ì¬Î»Í¼
	m_hGameStatusBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_GAME_STATUS));
	m_hStartBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_START));
	m_hMenuBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_MENU));
	m_hChar1Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HYJ1));
	m_hChar2Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HYJ2));
	m_hChar3Bmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HYJ3));
	m_hDeveloperBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_DEVELOPER));
	m_hAgainBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_AGAIN));
	m_hFinishBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_FINISH));
	m_hBackBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_BACK));
	m_hWingBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_WING));
	m_hSoundBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_SOUND));
	m_hStarBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_STAR));
	m_hLifeBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_LIFE));
	m_hEasyBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_EASY));
	m_hHardBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
		MAKEINTRESOURCE(IDB_HARD));


	//¼ÓÔØBlockÎ»Í¼
	int k;
	for (k = 0; k < BLOCK_COLOR_NUM; ++k)
	{
		m_hBlockBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(m_blockBmpNames[k]));
	}
	//¼ÓÔØÎÝ¶¥Î»Í¼
	for (k = 0; k < ROOF_COLOR_NUM; ++k)
	{
		m_hRoofkBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(m_roofBmpNames[k]));
	}

	for (k = 0; k < HERO_NUMBER; ++k)                                              //¼öÁ¤ À±Àç
	{
		m_hHeroCharBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance,
			MAKEINTRESOURCE(m_herocharBmpNames[k]));
	}

	//´´½¨Ó¢ÐÛ¡¢½¨Öþ
	m_hero = CreateHero(100, 0, 30, 50, m_hHeroBmp, 0, 3);
	m_bike = CreateBike(30, 0, 74, 50, m_hBikeBmp, 0, 3);
	//m_building = CreateBuilding(0, 70, 500, 250, m_hBuildingBmp);
	m_cloud = CreateCloud(100, 0, 76, 42, m_hCloudBmp);
	m_cloud2 = CreateCloud(250, 20, 76, 42, m_hCloudBmp);
	m_light = CreateCloud(350, 165, 140, 185, m_hLightBmp);
	m_menu = CreateMenu(100, 55, 400, 240, m_hMenuBmp);
	m_wing = CreateCloud(WNDWIDTH, 20, 30, 30, m_hWingBmp);
	m_life = CreateMenu(WNDWIDTH - 130,WNDHEIGHT - 76, 120, 40, m_hLifeBmp);

	//´´½¨µØÐÎ
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		if (k % 8 == 0)
		{
			m_terrian[k] = CreateTerrian(k * 65, WNDHEIGHT + 100, 65, 300, m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], 33, 65);
			m_star[k] = CreateCloud(k * 65 + 20, WNDHEIGHT + 100, 30, 30, m_hStarBmp);
		}
		else
		{
			m_terrian[k] = CreateTerrian(k * 65, 150 + 50 * (k % 3), 65, 300, m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], 33, 65);
			m_star[k] = CreateCloud(k * 65 + 20, 120 + 50 * (k % 3), 30, 30, m_hStarBmp);
		}
	}

	for (k = 0; k < HERO_NUMBER; ++k)                           //¼öÁ¤ À±Àç
	{
		m_herochar[k] = CreateHero(100, 0, 50, 60, m_hHeroCharBmp[k], 0, 9);
	}

	//´´½¨ÓÎÏ·×´Ì¬      //¹öÆ°µé//
	m_gameStatus = CreateGameStatus(0, 0, 40, 30, m_hGameStatusBmp);
	m_start = CreateGameStart(210, 150, 180, 70, m_hStartBmp);
	m_developer = CreateGameStart(210, 230, 180, 70, m_hDeveloperBmp);
	m_again = CreateGameStart(350, 40, 79, 36, m_hAgainBmp);
	m_finish = CreateGameStart(440, 40, 79, 36, m_hFinishBmp);
	m_back = CreateGameStart(500, 243, 60, 60, m_hBackBmp);
	m_char1 = CreateGameStart(205, 140, 50, 60, m_hChar1Bmp);
	m_char2 = CreateGameStart(275, 140, 50, 60, m_hChar2Bmp);
	m_char3 = CreateGameStart(350, 140, 50, 60, m_hChar3Bmp);
	m_sound = CreateGameStatus(40, 0, 40, 40, m_hSoundBmp);
	m_easy = CreateGameStart(240, 125, 130, 40, m_hEasyBmp);
	m_hard = CreateGameStart(240, 185, 130, 40, m_hHardBmp);

	//Æô¶¯¼ÆÊ±Æ÷
	SetTimer(hWnd, TIMER, SET_TIMER_VALUE, NULL);
}

VOID Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//¿ªÊ¼»æÖÆ
	hdc = BeginPaint(hWnd, &ps);

	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	//SelectObject(hdcBmp, m_hBackgroundBmp);
	if (Now_char_status == CHAR_STATUS_CHAR1)
	{
		SelectObject(hdcBmp, m_hPictureBmp);
	}
	else if (Now_char_status == CHAR_STATUS_CHAR2)
	{
		SelectObject(hdcBmp, m_hPicture4Bmp);
	}
	else  //Now_char_status == CHAR_STATUS_CHAR3
	{
		SelectObject(hdcBmp, m_hPicture5Bmp);
	}

	//»æÖÆ±³¾°µ½»º´æ
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcBmp, 0, 0, SRCCOPY);

	//»æÖÆ½¨Öþµ½»º´æ
	//SelectObject(hdcBmp, m_building.hBmp);
	//TransparentBlt(
	//hdcBuffer, m_building.pos.x, m_building.pos.y,
	//m_building.size.cx, m_building.size.cy,
	//hdcBmp, 0, 0, m_building.size.cx, m_building.size.cy,
	//RGB(255, 255, 255)
	//);

	SelectObject(hdcBmp, m_cloud.hBmp);                          //±¸¸§
	TransparentBlt(
		hdcBuffer, m_cloud.pos.x, m_cloud.pos.y,
		m_cloud.size.cx, m_cloud.size.cy,
		hdcBmp, 0, 0, m_cloud.size.cx, m_cloud.size.cy,
		RGB(255, 255, 255)
		);

	SelectObject(hdcBmp, m_cloud2.hBmp);
	TransparentBlt(
		hdcBuffer, m_cloud2.pos.x, m_cloud2.pos.y,
		m_cloud2.size.cx, m_cloud2.size.cy,
		hdcBmp, 0, 0, m_cloud2.size.cx, m_cloud2.size.cy,
		RGB(255, 255, 255)
		);

	if (wing_cool_count == WING_COOL_TIME && wing_appear_count > 0)            // ³¯°³ ¾ÆÀÌÅÛ
	{
		SelectObject(hdcBmp, m_wing.hBmp);
		TransparentBlt(
			hdcBuffer, m_wing.pos.x, m_wing.pos.y,
			m_wing.size.cx, m_wing.size.cy,
			hdcBmp, 0, 0, m_wing.size.cx, m_wing.size.cy,
			RGB(255, 255, 255)
			);
	}

	//»æÖÆHeroµ½»º´æ
	
	
	Hero hero = m_herochar[Now_char_status];                                                //ÄÉ¸¯ÅÍ ¼öÁ¤
	SelectObject(hdcBmp, m_herochar[Now_char_status].hBmp);
	TransparentBlt(
		hdcBuffer, m_herochar[Now_char_status].pos.x, m_herochar[Now_char_status].pos.y,
		m_herochar[Now_char_status].size.cx, m_herochar[Now_char_status].size.cy,
		hdcBmp, m_herochar[Now_char_status].size.cx * m_herochar[Now_char_status].curFrameIndex, 0,
		m_herochar[Now_char_status].size.cx, m_herochar[Now_char_status].size.cy,
		RGB(255, 255, 255)
		);
	

	//if (m_gameStatus.totalDist > 300)                // ÀÚÀü°Å 300Á¡ ³ÑÀ»½Ã »ý±è ³ªÁß¿¡ Á¶Á¤
	//{
	//	SelectObject(hdcBmp, m_bike.hBmp);
	//	TransparentBlt(
	//		hdcBuffer, m_bike.pos.x, m_bike.pos.y,
	//		m_bike.size.cx, m_bike.size.cy,
	//		hdcBmp, 0, m_bike.size.cy * m_bike.curFrameIndex, m_bike.size.cx, m_bike.size.cy,
	//		RGB(255, 255, 255)
	//		);
	//}

	//»æÖÆµØÐÎ
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		Terrian terrian = m_terrian[k];                                             //°Ç¹°
		SelectObject(hdcBmp, terrian.hRoofBmp);
		TransparentBlt(
			hdcBuffer, terrian.pos.x, terrian.pos.y,
			terrian.roofWidth, terrian.roofHeight,
			hdcBmp, 0, 0, terrian.roofWidth, terrian.roofHeight,
			RGB(255, 255, 255)
			);
		SelectObject(hdcBmp, terrian.hBlockBmp);
		int t;
		for (t = 0; t < terrian.blockNum; ++t)                                       //ÁöºØ
		{
			TransparentBlt(
				hdcBuffer, terrian.pos.x, terrian.pos.y + terrian.roofHeight + terrian.blockHeight * t,
				terrian.blockWidth, terrian.blockHeight,
				hdcBmp, 0, 0, terrian.blockWidth, terrian.blockHeight,
				RGB(255, 255, 255)
				);
		}
	}

	for (k = 0; k < MAX_TERRIAN_NUM; ++k)                               //º°µé
	{
		if (star_appear[k] == 1)
		{
			SelectObject(hdcBmp, m_star[k].hBmp);
			TransparentBlt(hdcBuffer, m_star[k].pos.x, m_star[k].pos.y, m_star[k].size.cx, m_star[k].size.cy,
				hdcBmp, m_star[k].size.cx * (k % 9), 0,
				m_star[k].size.cx, m_star[k].size.cy, RGB(255, 255, 255));
		}
	}

	SelectObject(hdcBmp, m_light.hBmp);                                 //°¡·Îµî
	TransparentBlt(
		hdcBuffer, m_light.pos.x, m_light.pos.y,
		m_light.size.cx, m_light.size.cy,
		hdcBmp, 0, 0, m_light.size.cx, m_light.size.cy,
		RGB(255, 255, 255)
		);

	//»æÖÆÓÎÏ·×´Ì¬
	//ÔÝÍ£»ò¼ÌÐøÎ»Í¼
	SelectObject(hdcBmp, m_gameStatus.hBmp);                             //½ÃÀÛ/Á¤Áö
	TransparentBlt(hdcBuffer, m_gameStatus.pos.x, m_gameStatus.pos.y, m_gameStatus.size.cx, m_gameStatus.size.cy,
		hdcBmp, 0, m_gameStatus.size.cy * m_gameStatus.isPaused,
		m_gameStatus.size.cx, m_gameStatus.size.cy, RGB(255, 255, 255));

	SelectObject(hdcBmp, m_sound.hBmp);                                 //À½¼Ò°Å
	TransparentBlt(hdcBuffer, m_sound.pos.x, m_sound.pos.y, m_sound.size.cx, m_sound.size.cy,
		hdcBmp, m_sound.size.cx * m_sound.isPaused, 0,
		m_sound.size.cx, m_sound.size.cy, RGB(255, 255, 255));

	if (Now_difficulty_status == DIFFICULTY_STATUS_EASY)
	{
		SelectObject(hdcBmp, m_life.hBmp);                                 //»ý¸í
		TransparentBlt(hdcBuffer, m_life.pos.x, m_life.pos.y,
			m_life.size.cx, m_life.size.cy,
			hdcBmp, m_life.size.cx * life_frame, 0, m_life.size.cx, m_life.size.cy,
			RGB(255, 255, 255));
	}

	//»æÖÆ·ÖÊý
	TCHAR	szDist[13];
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);
	TextOut(hdcBuffer, WNDWIDTH - 120, 15, szDist, wsprintf(szDist, _T("SCORE:%d"), m_gameStatus.totalDist));

	if (Now_screen_status == SCREEN_STATUS_END)
	{
		//TCHAR	szDist1[30];
		//SetTextColor(hdcBuffer, RGB(255, 51, 0));
		//SetBkMode(hdcBuffer, TRANSPARENT);
		//extOut(hdcBuffer, WNDWIDTH - 270, 30, szDist1, wsprintf(szDist1, _T("TRY AGAIN^^!")));

		SelectObject(hdcBmp, m_again.hBmp);
		TransparentBlt(
			hdcBuffer, m_again.pos.x, m_again.pos.y,
			m_again.size.cx, m_again.size.cy,
			hdcBmp, 0, 0, m_again.size.cx, m_again.size.cy,
			RGB(255, 255, 255));

		SelectObject(hdcBmp, m_finish.hBmp);
		TransparentBlt(
			hdcBuffer, m_finish.pos.x, m_finish.pos.y,
			m_finish.size.cx, m_finish.size.cy,
			hdcBmp, 0, 0, m_finish.size.cx, m_finish.size.cy,
			RGB(255, 255, 255));
	}

	if (Now_difficulty_status == DIFFICULTY_STATUS_EASY)
	{
		if (m_gameStatus.totalDist < ReadHighScore())
		{
			TCHAR	highscore[30];
			SetTextColor(hdcBuffer, RGB(0, 0, 0));
			SetBkMode(hdcBuffer, TRANSPARENT);
			TextOut(hdcBuffer, WNDWIDTH - 270, 15, highscore, wsprintf(highscore, _T("HIGH SCORE:%d"), ReadHighScore()));
		}
		else                                                         //½Å±â·Ï °»½Å½Ã HIGH SCORE = m_gameStatus.totalDist
		{
			TCHAR	highscore[30];
			SetTextColor(hdcBuffer, RGB(0, 0, 0));
			SetBkMode(hdcBuffer, TRANSPARENT);
			TextOut(hdcBuffer, WNDWIDTH - 270, 15, highscore, wsprintf(highscore, _T("HIGH SCORE:%d"), m_gameStatus.totalDist));
		}
	}

	else
	{
		if (m_gameStatus.totalDist < ReadHighScoreHard())
		{
			TCHAR	highscore[30];
			SetTextColor(hdcBuffer, RGB(0, 0, 0));
			SetBkMode(hdcBuffer, TRANSPARENT);
			TextOut(hdcBuffer, WNDWIDTH - 270, 15, highscore, wsprintf(highscore, _T("HIGH SCORE:%d"), ReadHighScoreHard()));
		}
		else
		{
			TCHAR	highscore[30];
			SetTextColor(hdcBuffer, RGB(0, 0, 0));
			SetBkMode(hdcBuffer, TRANSPARENT);
			TextOut(hdcBuffer, WNDWIDTH - 270, 15, highscore, wsprintf(highscore, _T("HIGH SCORE:%d"), m_gameStatus.totalDist));
		}
	}

	//×îºó½«ËùÓÐµÄÐÅÏ¢»æÖÆµ½ÆÁÄ»ÉÏ
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	//»ØÊÕ×ÊÔ´ËùÕ¼µÄÄÚ´æ
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);

	//½áÊø»æÖÆ
	EndPaint(hWnd, &ps);
}

VOID Render_Start(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//¿ªÊ¼»æÖÆ
	hdc = BeginPaint(hWnd, &ps);

	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);
	SelectObject(hdcBmp, m_hPictureBmp);

	//»æÖÆ±³¾°µ½»º´æ
	BitBlt(hdcBuffer, 0, 0, WNDWIDTH, WNDHEIGHT,
		hdcBmp, 0, 0, SRCCOPY);

	if (Now_screen_status == SCREEN_STATUS_START)
	{
		SelectObject(hdcBmp, m_start.hBmp);
		TransparentBlt(hdcBuffer, m_start.pos.x, m_start.pos.y, m_start.size.cx, m_start.size.cy,
			hdcBmp, 0, m_start.size.cy * m_start.isStart,
			m_start.size.cx, m_start.size.cy, RGB(255, 255, 255));

		SelectObject(hdcBmp, m_developer.hBmp);
		TransparentBlt(hdcBuffer, m_developer.pos.x, m_developer.pos.y, m_developer.size.cx, m_developer.size.cy,
			hdcBmp, 0, m_developer.size.cy * m_developer.isStart,
			m_developer.size.cx, m_developer.size.cy, RGB(255, 255, 255));
	}
	else if (Now_screen_status == SCREEN_STATUS_DEVELOPER)
	{
		SelectObject(hdcBmp, m_menu.hBmp);
		TransparentBlt(
			hdcBuffer, m_menu.pos.x, m_menu.pos.y,
			m_menu.size.cx, m_menu.size.cy,
			hdcBmp, 0, 0, m_menu.size.cx, m_menu.size.cy,
			RGB(255, 255, 255));

		SelectObject(hdcBmp, m_back.hBmp);
		TransparentBlt(
			hdcBuffer, m_back.pos.x, m_back.pos.y,
			m_back.size.cx, m_back.size.cy,
			hdcBmp, 0, 0, m_back.size.cx, m_back.size.cy,
			RGB(255, 255, 255));

		TCHAR	szDist[30];
		TCHAR	szDist2[35];
		//TCHAR	szDist3[32];
		TCHAR	szDist4[30];
		TCHAR   szDist5[48], szDist6[42], szDist7[44], szDist8[18], szDist9[20];

		SetTextColor(hdcBuffer, RGB(0, 0, 0));
		SetBkMode(hdcBuffer, TRANSPARENT);
		TextOut(hdcBuffer, 110, 70, szDist9, wsprintf(szDist9, _T("---Developers---")));
		TextOut(hdcBuffer, 110, 90, szDist, wsprintf(szDist, _T("Developed by Eun Bi Choi")));
		TextOut(hdcBuffer, 110, 110, szDist2, wsprintf(szDist2, _T("Character Designed by Mun Kyu Choi")));
		TextOut(hdcBuffer, 110, 130, szDist4, wsprintf(szDist4, _T("Character Model Yoon Jae Han")));
		TextOut(hdcBuffer, 110, 170, szDist8, wsprintf(szDist8, _T("---BGM Sources---")));
		TextOut(hdcBuffer, 110, 190, szDist5, wsprintf(szDist5, _T("SoundTrack1 : Raindrop Flower (Maple Story OST)")));
		TextOut(hdcBuffer, 110, 210, szDist6, wsprintf(szDist6, _T("SoundTrack2 : Motivity (Talse Weaver OST)")));
		TextOut(hdcBuffer, 110, 230, szDist7, wsprintf(szDist7, _T("SoundTrack3 : Second Run (Talse Weaver OST)")));
	}
	else if (Now_screen_status == SCREEN_STATUS_DIFFICULTY)
	{
		SelectObject(hdcBmp, m_menu.hBmp);
		TransparentBlt(
			hdcBuffer, m_menu.pos.x, m_menu.pos.y,
			m_menu.size.cx, m_menu.size.cy,
			hdcBmp, 0, 0, m_menu.size.cx, m_menu.size.cy,
			RGB(255, 255, 255));

		SelectObject(hdcBmp, m_easy.hBmp);
		TransparentBlt(hdcBuffer, m_easy.pos.x, m_easy.pos.y, m_easy.size.cx, m_easy.size.cy,
			hdcBmp, 0, 0, m_easy.size.cx, m_easy.size.cy, RGB(255, 255, 255));

		SelectObject(hdcBmp, m_hard.hBmp);
		TransparentBlt(hdcBuffer, m_hard.pos.x, m_hard.pos.y, m_hard.size.cx, m_hard.size.cy,
			hdcBmp, 0, 0, m_hard.size.cx, m_hard.size.cy, RGB(255, 255, 255));

		SelectObject(hdcBmp, m_back.hBmp);
		TransparentBlt(
			hdcBuffer, m_back.pos.x, m_back.pos.y,
			m_back.size.cx, m_back.size.cy,
			hdcBmp, 0, 0, m_back.size.cx, m_back.size.cy,
			RGB(255, 255, 255));
	}
	else if (Now_screen_status == SCREEN_STATUS_CHAR)
	{
		SelectObject(hdcBmp, m_menu.hBmp);
		TransparentBlt(
			hdcBuffer, m_menu.pos.x, m_menu.pos.y,
			m_menu.size.cx, m_menu.size.cy,
			hdcBmp, 0, 0, m_menu.size.cx, m_menu.size.cy,
			RGB(255, 255, 255));

		SelectObject(hdcBmp, m_char1.hBmp);
		TransparentBlt(hdcBuffer, m_char1.pos.x, m_char1.pos.y, m_char1.size.cx, m_char1.size.cy,
			hdcBmp, 0, m_char1.size.cy * m_char1.isStart,
			m_char1.size.cx, m_char1.size.cy, RGB(255, 255, 254));

		SelectObject(hdcBmp, m_char2.hBmp);
		TransparentBlt(hdcBuffer, m_char2.pos.x, m_char2.pos.y, m_char2.size.cx, m_char2.size.cy,
			hdcBmp, 0, m_char2.size.cy * m_char2.isStart,
			m_char2.size.cx, m_char2.size.cy, RGB(255, 255, 254));

		SelectObject(hdcBmp, m_char3.hBmp);
		TransparentBlt(hdcBuffer, m_char3.pos.x, m_char3.pos.y, m_char3.size.cx, m_char3.size.cy,
			hdcBmp, 0, m_char3.size.cy * m_char3.isStart,
			m_char3.size.cx, m_char3.size.cy, RGB(255, 255, 254));

		SelectObject(hdcBmp, m_back.hBmp);
		TransparentBlt(
			hdcBuffer, m_back.pos.x, m_back.pos.y,
			m_back.size.cx, m_back.size.cy,
			hdcBmp, 0, 0, m_back.size.cx, m_back.size.cy,
			RGB(255, 255, 255));
	}

	//×îºó½«ËùÓÐµÄÐÅÏ¢»æÖÆµ½ÆÁÄ»ÉÏ
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	//»ØÊÕ×ÊÔ´ËùÕ¼µÄÄÚ´æ
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);

	//½áÊø»æÖÆ
	EndPaint(hWnd, &ps);
}

Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize)
{
	Hero hero;
	hero.hBmp = hBmp;
	hero.pos.x = posX;
	hero.pos.y = posY;
	hero.size.cx = sizeX;
	hero.size.cy = sizeY;
	hero.curFrameIndex = curFrameIndex;
	hero.maxFrameSize = maxFrameSize;
	return hero;
}

Bike CreateBike(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int maxFrameSize)
{
	Bike bike;
	bike.hBmp = hBmp;
	bike.pos.x = posX;
	bike.pos.y = posY;
	bike.size.cx = sizeX;
	bike.size.cy = sizeY;
	bike.curFrameIndex = curFrameIndex;
	bike.maxFrameSize = maxFrameSize;
	return bike;
}

Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Building building;
	building.hBmp = hBmp;
	building.pos.x = posX;
	building.pos.y = posY;
	building.size.cx = sizeX;
	building.size.cy = sizeY;
	return building;
}

Menu CreateMenu(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Menu menu;
	menu.hBmp = hBmp;
	menu.pos.x = posX;
	menu.pos.y = posY;
	menu.size.cx = sizeX;
	menu.size.cy = sizeY;
	return menu;
}

Cloud CreateCloud(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Cloud cloud;
	cloud.hBmp = hBmp;
	cloud.pos.x = posX;
	cloud.pos.y = posY;
	cloud.size.cx = sizeX;
	cloud.size.cy = sizeY;
	return cloud;
}

GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	GameStatus gameStatus;
	gameStatus.pos.x = posX;
	gameStatus.pos.y = posY;
	gameStatus.size.cx = sizeX;
	gameStatus.size.cy = sizeY;
	gameStatus.hBmp = hBmp;
	gameStatus.totalDist = 0;
	gameStatus.isPaused = false;
	return gameStatus;
}

GameStart CreateGameStart(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	GameStart start;
	start.pos.x = posX;
	start.pos.y = posY;
	start.size.cx = sizeX;
	start.size.cy = sizeY;
	start.hBmp = hBmp;
	start.totalDist = 0;
	start.isStart = false;
	return start;
}

Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY,
	HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight)
{
	Terrian terrian;
	terrian.pos.x = posX;
	terrian.pos.y = posY;
	terrian.size.cx = sizeX;
	terrian.size.cy = sizeY;
	terrian.hBlockBmp = hBlockBmp;
	terrian.hRoofBmp = hRoofBmp;
	terrian.roofWidth = sizeX;
	terrian.roofHeight = roofHeight;
	terrian.blockWidth = sizeX;
	terrian.blockHeight = blockHeight;
	terrian.blockNum = (int)ceil((sizeY - roofHeight) * 1.0 / blockHeight);
	return terrian;
}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	GameStatusUpdate();
	HeroUpdate();
	BikeUpdate();
	CloudUpdate();
	TerrianUpdate();
	//GameStatusUpdate();
	InvalidateRect(hWnd, NULL, FALSE);
}

VOID HeroUpdate()
{
	int j = Now_char_status;
	
	//                           È÷¾î·Î (ÄÉ¸¯ÅÍ)                              //
	if (Up_key_pressed != 0)
	{
		Up_key_count++;
	}
	else if (m_herochar[j].pos.y < height_now - m_herochar[j].size.cy)				//ÀÚµ¿³«ÇÏ
	{
		m_herochar[j].pos.y += 12;													
	}

	if (flying_status == 0)															//³¯Áö ¾ÊÀ» ½Ã
	{
		m_herochar[j].pos.y -= HeroJumpY[Up_key_count] * 6;                       //¸ÞÀÎ ÄÉ¸¯ÅÍ Á¡ÇÁ
	}

	if (flying_status == 1)														 //³¯¶§ pos.y = 10
	{
		m_herochar[j].pos.y = 10;
	}

	if (m_herochar[j].pos.y + m_herochar[j].size.cy > height_now)                   //Áö¸é¿¡ ´êÀ» ½Ã pos.y Áö¸é°ú °°°Ô
	{
		m_herochar[j].pos.y = height_now - m_herochar[j].size.cy;
		Up_key_pressed = 0;
		Up_key_count = 0;
	}

	//                         ±¸Á¶¹° ³ôÀÌ                          //
	for (int k = 0; k < MAX_TERRIAN_NUM; ++k)           
	{
		if (m_terrian[k].pos.x >= 55 && m_terrian[k].pos.x < 120)
		{
			height_now = m_terrian[k].pos.y;

			if (k == MAX_TERRIAN_NUM - 1)
			{
				height_soon = m_terrian[0].pos.y;
				posx_soon = m_terrian[0].pos.x;
			}
			else
			{
				height_soon = m_terrian[k + 1].pos.y;
				posx_soon = m_terrian[k + 1].pos.x;
			}


			//                    º°                  //
			if (m_herochar[j].pos.x >= m_star[k].pos.x - m_herochar[j].size.cx
				&& m_herochar[j].pos.x <= m_star[k].pos.x + m_star[k].size.cx
				&& m_herochar[j].pos.y <= m_star[k].pos.y + m_star[k].size.cy
				&& m_herochar[j].pos.y >= m_star[k].pos.y - m_herochar[j].size.cy)
			{
				star_appear[k] = 0;
				m_gameStatus.totalDist += 10;
			}

			//                        Á×À½                        //
			if (m_herochar[j].pos.x + m_herochar[j].size.cx > posx_soon
				&& m_herochar[j].pos.y + m_herochar[j].size.cy > height_soon)
			{
				if (Now_difficulty_status == DIFFICULTY_STATUS_EASY)                 //EASY ¸ðµå
				{
					if (life_frame == 3)                                             //¸ñ¼û3°³ ´Ù¾¸ ¿ÏÀüÇÑ Á×À½ (2)
					{
						life_frame++;
						Now_screen_status = SCREEN_STATUS_END;                       // Á×À½ on/off
						if (EffSoundStatus == 1) PlayScreamSound();                  // Á×À½½Ã ºñ¸í¼Ò¸® on/off
						StopSound();
						high_score = ReadHighScore();
						if (m_gameStatus.totalDist > high_score)                     //ÇÏÀÌ ½ºÄÚ¾î
						{
							WriteHighScore(m_gameStatus.totalDist);
						}
					}
					else
					{
						if (EffSoundStatus == 1) PlayScreamSound();
						life_frame++;
						m_herochar[j].pos.y = 0;
						Up_key_pressed = 1;
						Up_key_count = 58;
					}
				}
				else if (Now_difficulty_status == DIFFICULTY_STATUS_HARD)        //HARD ¸ðµå 
				{
					Now_screen_status = SCREEN_STATUS_END;                       // Á×À½ on/off
					if (EffSoundStatus == 1) PlayScreamSound();                  // Á×À½½Ã ºñ¸í¼Ò¸® on/off
					StopSound();
					high_score_hard = ReadHighScoreHard();
					if (m_gameStatus.totalDist > high_score_hard)                     //ÇÏÀÌ ½ºÄÚ¾î
					{
						WriteHighScoreHard(m_gameStatus.totalDist);
					}
				}
			}
		}

	}

	//                ³¯°³                //
	if (m_herochar[j].pos.x >= m_wing.pos.x - m_herochar[j].size.cx                      
		&& m_herochar[j].pos.x <= m_wing.pos.x + m_wing.size.cx
		&& m_herochar[j].pos.y <= m_wing.pos.y + m_wing.size.cy
		&& m_herochar[j].pos.y >= m_wing.pos.y - m_herochar[j].size.cy)
	{
		m_wing.pos.x = m_wing.pos.x - 200;
		flying_status = 1;
	}

	if (flying_status == 0)                 //(³¯Áö ¾ÊÀ» ½Ã) ´Þ¸± ¶§ ÇÁ·¹ÀÓ ¹Ù²ãÁÜ
	{
		if (Level > 7) runningflag = 1;
		if (runningflag == 0)
		{
			runningflag = 1;
		}
		else if (runningflag == 1)
		{
			++m_herochar[j].curFrameIndex;
			runningflag = 0;
		}
		if (Up_key_pressed == 0)         // ¹Ù´Ú¿¡ ºÙ¾î¼­ ´Þ¸± ¶§
		{
			m_herochar[j].curFrameIndex = m_herochar[j].curFrameIndex >= 4 ? 0 : m_herochar[j].curFrameIndex;
		}

		if (Now_screen_status != SCREEN_STATUS_END)                    //Á¡ÇÁÇÒ¶§
		{
			if (m_herochar[j].curFrameIndex >= 8)
			{
				m_herochar[j].curFrameIndex = 4;
			}
		}
		else
		{
			m_herochar[j].curFrameIndex = 8;
		}
	}
	//¸üÐÂ¶¯×÷
}

VOID BikeUpdate()
{
	//TODO
	//¸üÐÂÎ»ÖÃ
	//m_bike.pos.x += 1;
	//m_bike.pos.x = m_bike.pos.x >= WNDWIDTH ? 0 : m_bike.pos.x;

	//Update y position
	//m_bike.pos.y -= BikeJumpY[Up_key_count] * 6;
	if (Now_char_status == 3)
	{
		m_bike.pos.y = m_hero.pos.y;
	}
	else
	{
		m_bike.pos.y = m_herochar[Now_char_status].pos.y;
	}

	//
	//if (Up_key_count == 0) {
	//if( building_height < m_hore.pos.y )
	//	m_bike.pos.y -= 6;
	//
	//}
	//¸üÐÂ¶¯×÷
	++m_bike.curFrameIndex;
	m_bike.curFrameIndex = m_bike.curFrameIndex >= m_bike.maxFrameSize ? 0 : m_bike.curFrameIndex;
}

VOID CloudUpdate()
{
	m_cloud.pos.x -= (3 + Level);
	m_cloud2.pos.x -= (4 + Level);
	m_light.pos.x -= (6 + Level);

	if (wing_cool_count == WING_COOL_TIME && wing_appear_count > 0)                       //³¯°³
	{
		m_wing.pos.x -= (4 + Level);
	}
	if (wing_cool_count == 0 && wing_appear_count == 0)
	{
		m_wing.pos.x = WNDWIDTH;
	}

	if (m_cloud.pos.x + m_cloud.size.cx < 0)
	{
		m_cloud.pos.x += WNDWIDTH + m_cloud.size.cx;
	}
	if (m_cloud2.pos.x + m_cloud2.size.cx < 0)
	{
		m_cloud2.pos.x += WNDWIDTH + m_cloud2.size.cx;
	}
	if (m_light.pos.x + m_light.size.cx < 0)
	{
		m_light.pos.x += WNDWIDTH + m_light.size.cx;
	}
}

VOID TerrianUpdate()
{
	//TODO
	int k;

	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		for (int i = 100; i >= 0; i--)
		{
			if (m_gameStatus.totalDist > 100 * i)			//¼Óµµ (³ªÁß¿¡ ¼ýÀÚ Á¶Á¤ÇÏ±â)
			{												//10000Á¡ ½Ã¿¡ ÃÖ´ë¼Óµµ´Â 14
				Level = sqrt(i);
				break;
			}
		}
		m_terrian[k].pos.x -= (4 + Level);
		m_star[k].pos.x -= (4 + Level);

		if (m_terrian[k].pos.x + m_terrian[k].size.cx < 0)                        //°Ç¹° & º°
		{
			m_terrian[k].pos.x += MAX_TERRIAN_NUM * m_terrian[k].size.cx;
			m_star[k].pos.x += MAX_TERRIAN_NUM * m_terrian[k].size.cx;

			star_appear[k] = 1;

			random_number = rand() % 4;                                            
			m_terrian[k].pos.y = terrian_height[random_number];						//°Ç¹° ³ôÀÌ ·£´ý
			m_star[k].pos.y = terrian_height[random_number] - 30;
		}
	}
}

VOID GameStatusUpdate()
{
	//TODO
	++m_gameStatus.totalDist;

	if (wing_cool_count < WING_COOL_TIME)      //³¯°³
	{
		++wing_cool_count;
	}
	if (wing_cool_count == WING_COOL_TIME)
	{
		if (wing_appear_count < WING_APPEAR_TIME)
		{
			++wing_appear_count;
		}
		if (wing_appear_count == WING_APPEAR_TIME)
		{
			wing_appear_count = 0;
			wing_cool_count = 0;
		}
	}

	if (flying_status == 1)
	{
		if (wing_maintain_count < WING_MAINTAIN_TIME)
		{
			wing_maintain_count++;
		}
		if (wing_maintain_count == WING_MAINTAIN_TIME)
		{
			wing_maintain_count = 0;
			flying_status = 0;
			Up_key_count = 35;
			Up_key_pressed = 1;
		}
		
	}
}

BOOL Paused(POINT ptMouse)
{
	RECT rPause;

	rPause.left = m_gameStatus.pos.x;
	rPause.top = m_gameStatus.pos.y;
	rPause.right = m_gameStatus.size.cx;
	rPause.bottom = m_gameStatus.size.cy;

	return PtInRect(&rPause, ptMouse);
}

BOOL SoundPaused(POINT ptMouse)
{
	RECT rSoundPause;

	rSoundPause.left = m_sound.pos.x;
	rSoundPause.top = m_sound.pos.y;
	rSoundPause.right = m_sound.pos.x + m_sound.size.cx;
	rSoundPause.bottom = m_sound.pos.y + m_sound.size.cy;

	return PtInRect(&rSoundPause, ptMouse);
}

BOOL Start(POINT ptMouse)
{
	RECT rStart;

	rStart.left = m_start.pos.x;
	rStart.top = m_start.pos.y;
	rStart.right = m_start.pos.x + m_start.size.cx;
	rStart.bottom = m_start.pos.y + m_start.size.cy;

	return PtInRect(&rStart, ptMouse);
}

BOOL Developer(POINT ptMouse)
{
	RECT rDeveloper;

	rDeveloper.left = m_developer.pos.x;
	rDeveloper.top = m_developer.pos.y;
	rDeveloper.right = m_developer.pos.x + m_developer.size.cx;
	rDeveloper.bottom = m_developer.pos.y + m_developer.size.cy;

	return PtInRect(&rDeveloper, ptMouse);
}

BOOL Again(POINT ptMouse)
{
	RECT rAgain;

	rAgain.left = m_again.pos.x;
	rAgain.top = m_again.pos.y;
	rAgain.right = m_again.pos.x + m_again.size.cx;
	rAgain.bottom = m_again.pos.y + m_again.size.cy;

	return PtInRect(&rAgain, ptMouse);
}

BOOL Finish(POINT ptMouse)
{
	RECT rFinish;

	rFinish.left = m_finish.pos.x;
	rFinish.top = m_finish.pos.y;
	rFinish.right = m_finish.pos.x + m_finish.size.cx;
	rFinish.bottom = m_finish.pos.y + m_finish.size.cy;

	return PtInRect(&rFinish, ptMouse);
}

BOOL Back(POINT ptMouse)
{
	RECT rBack;

	rBack.left = m_back.pos.x;
	rBack.top = m_back.pos.y;
	rBack.right = m_back.pos.x + m_back.size.cx;
	rBack.bottom = m_back.pos.y + m_back.size.cy;

	return PtInRect(&rBack, ptMouse);
}

BOOL Char1(POINT ptMouse)
{
	RECT char1;

	char1.left = m_char1.pos.x;
	char1.top = m_char1.pos.y;
	char1.right = m_char1.pos.x + m_char1.size.cx;
	char1.bottom = m_char1.pos.y + m_char1.size.cy;

	return PtInRect(&char1, ptMouse);
}

BOOL Char2(POINT ptMouse)
{
	RECT char2;

	char2.left = m_char2.pos.x;
	char2.top = m_char2.pos.y;
	char2.right = m_char2.pos.x + m_char2.size.cx;
	char2.bottom = m_char2.pos.y + m_char2.size.cy;

	return PtInRect(&char2, ptMouse);
}

BOOL Char3(POINT ptMouse)
{
	RECT char3;

	char3.left = m_char3.pos.x;
	char3.top = m_char3.pos.y;
	char3.right = m_char3.pos.x + m_char3.size.cx;
	char3.bottom = m_char3.pos.y + m_char3.size.cy;

	return PtInRect(&char3, ptMouse);
}

BOOL Easy(POINT ptMouse)
{
	RECT rEasy;

	rEasy.left = m_easy.pos.x;
	rEasy.top = m_easy.pos.y;
rEasy.right = m_easy.pos.x + m_easy.size.cx;
rEasy.bottom = m_easy.pos.y + m_easy.size.cy;

return PtInRect(&rEasy, ptMouse);
}

BOOL Hard(POINT ptMouse)
{
	RECT rHard;

	rHard.left = m_hard.pos.x;
	rHard.top = m_hard.pos.y;
	rHard.right = m_hard.pos.x + m_hard.size.cx;
	rHard.bottom = m_hard.pos.y + m_hard.size.cy;

	return PtInRect(&rHard, ptMouse);
}

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:

		Up_key_pressed++;
		if (Up_key_pressed <= 2)
		{
			Up_key_count = 0;
			if (EffSoundStatus == 1) PlayEffSound(effSD_1);
		}
		break;

	case VK_DOWN:
		
		if (m_hero.pos.y != height_now)
		{
			m_herochar[Now_char_status].pos.y += 50;
			Up_key_count = COUNT_PER_SEC;
		}
		break;

	default:
		break;
	}
}

VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
		//case VK_UP:
		//m_hero.pos.y += 50;
		//InvalidateRect(hWnd, NULL, FALSE);
		//break;
	case VK_DOWN:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		break;
	}
}

VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	if (EffSoundStatus == 1) PlayEffSound(effSD_2);

	//Èç¹ûµã»÷ÁË¼ÌÐø»òÔÝÍ£Í¼±ê

	if (Now_screen_status == SCREEN_STATUS_START)
	{
		if (Start(ptMouse))
		{
			Now_screen_status = SCREEN_STATUS_DIFFICULTY;
			InvalidateRect(hWnd, NULL, FALSE);
		}

		if (Developer(ptMouse))
		{
			Now_screen_status = SCREEN_STATUS_DEVELOPER;
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}

	else if (Now_screen_status == SCREEN_STATUS_DIFFICULTY)
	{
		if (Easy(ptMouse))
		{
			Now_difficulty_status = DIFFICULTY_STATUS_EASY;
			Now_screen_status = SCREEN_STATUS_CHAR;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (Hard(ptMouse))
		{
			Now_difficulty_status = DIFFICULTY_STATUS_HARD;
			Now_screen_status = SCREEN_STATUS_CHAR;
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (Back(ptMouse))
		{
			Now_screen_status = SCREEN_STATUS_START;
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}

	else if (Now_screen_status == SCREEN_STATUS_CHAR)
	{
		if (Char1(ptMouse))
		{
			Now_screen_status = SCREEN_STATUS_GAME;
			Now_char_status = CHAR_STATUS_CHAR1;
			PlaySound(SD_1);
			InvalidateRect(hWnd, NULL, FALSE);
		}

		if (Char2(ptMouse))
		{
			Now_screen_status = SCREEN_STATUS_GAME;
			Now_char_status = CHAR_STATUS_CHAR2;
			PlaySound(SD_2);
			InvalidateRect(hWnd, NULL, FALSE);
		}

		if (Char3(ptMouse))
		{
			Now_screen_status = SCREEN_STATUS_GAME;
			Now_char_status = CHAR_STATUS_CHAR3;
			PlaySound(SD_3);
			InvalidateRect(hWnd, NULL, FALSE);
		}

		if (Back(ptMouse))
		{
			Now_screen_status = SCREEN_STATUS_DIFFICULTY;
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}

	else if (Now_screen_status == SCREEN_STATUS_DEVELOPER)
	{
		if (Back(ptMouse))
		{
			Now_screen_status = SCREEN_STATUS_START;
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}

	else if (Now_screen_status == SCREEN_STATUS_GAME)
	{
		if (Paused(ptMouse))
		{
			if (!m_gameStatus.isPaused)
			{
				KillTimer(hWnd, TIMER);
				m_gameStatus.isPaused = TRUE;
			}
			else
			{
				SetTimer(hWnd, TIMER, SET_TIMER_VALUE, NULL);
				m_gameStatus.isPaused = FALSE;
			}
			InvalidateRect(hWnd, NULL, FALSE);
		}

		if (SoundPaused(ptMouse))
		{
			if (!m_sound.isPaused)
			{
				StopSound();
				m_sound.isPaused = TRUE;
			}
			else
			{
				if (Now_char_status == CHAR_STATUS_CHAR1)
				{
					PlaySound(SD_1);
					EffSoundStatus = 1;
				}
				else if (Now_char_status == CHAR_STATUS_CHAR2)
				{
					PlaySound(SD_2);
					EffSoundStatus = 1;
				}
				else
				{
					PlaySound(SD_3);
					EffSoundStatus = 1;
				}
				m_sound.isPaused = FALSE;
			}
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}

	else if (Now_screen_status == SCREEN_STATUS_END)
	{
		if (Again(ptMouse))
		{
			Now_screen_status = SCREEN_STATUS_CHAR;
			m_gameStatus.totalDist = 0;
			m_herochar[Now_char_status].pos.y = 0;
			life_frame = 0;
			m_sound.isPaused = 0;
			wing_cool_count = 0;
			Up_key_pressed = 0;
			Up_key_count = 0;
			EffSoundStatus = 1;
			
			InvalidateRect(hWnd, NULL, FALSE);
		}
		else if (Finish(ptMouse))
		{
			DestroyWindow(hWnd);
		}
	}
}

VOID Sound_Setup()
{
	FMOD_System_Create(&g_pSystem);       //»ç¿îµå ½Ã½ºÅÛ »ý¼º

	FMOD_System_Init(g_pSystem, 3, FMOD_INIT_NORMAL, 0);  //Ã¤³Î¼ö, ¸ðµå, 0

	char str[128];    //»ç¿îµå °æ·Î

	for (int i = 0; i < SD_END; i++)
	{
		wsprintf(str, ".\\res\\sound\\sound%d.mp3", i + 1);
		FMOD_System_CreateStream(g_pSystem, str, FMOD_LOOP_NORMAL, 0, &g_pSound[SD_1 + i]);    //FMOD_DEFAULT, FMOD_LOOP_NORMAL
		FMOD_Channel_SetVolume(g_pChannel[i], 0.5);   //º¼·ý Á¶Àý 0.0 ~ 1.0
	}
}

VOID EffSound_Setup()
{
	FMOD_System_Create(&g_pEffSystem);       //»ç¿îµå ½Ã½ºÅÛ »ý¼º

	FMOD_System_Init(g_pEffSystem, 3, FMOD_INIT_NORMAL, 0);  //Ã¤³Î¼ö, ¸ðµå, 0

	char str[128];    //»ç¿îµå °æ·Î

	for (int i = 0; i < effSD_END; i++)
	{
		wsprintf(str, ".\\res\\sound\\effsound%d.mp3", i + 1);
		FMOD_System_CreateStream(g_pEffSystem, str, FMOD_DEFAULT, 0, &g_pEffSound[effSD_1 + i]);    //FMOD_DEFAULT, FMOD_LOOP_NORMAL
		FMOD_Channel_SetVolume(g_pEffChannel[i], 0.5);   //º¼·ý Á¶Àý 0.0 ~ 1.0
	}
}

VOID PlaySound(SOUNDKIND eSound)
{
	FMOD_System_PlaySound(g_pSystem, FMOD_CHANNEL_FREE, g_pSound[eSound], 0, &g_pChannel[eSound]);
	FMOD_Channel_SetVolume(g_pChannel[eSound], 1.0);
}

VOID PlayEffSound(EFFSOUNDKIND eSound)
{
	FMOD_System_PlaySound(g_pEffSystem, FMOD_CHANNEL_FREE, g_pEffSound[eSound], 0, &g_pEffChannel[eSound]);
	FMOD_Channel_SetVolume(g_pEffChannel[eSound], 0.2);
}

VOID PlayScreamSound()
{
	FMOD_System_PlaySound(g_pEffSystem, FMOD_CHANNEL_FREE, g_pEffSound[effSD_3], 0, &g_pEffChannel[effSD_3]);
	FMOD_Channel_SetVolume(g_pEffChannel[effSD_3], 1.0);
}

VOID StopSound()
{
	for (int i = 0; i < SD_END; i++)
	{
		FMOD_Channel_Stop(g_pChannel[i]);
	}

	EffSoundStatus = 0;
}

VOID ReleaseSound()
{
	for (int i = 0; i < SD_END; i++)
	{
		FMOD_Sound_Release(g_pSound[i]);
	}
	for (int i = 0; i < effSD_END; i++)
	{
		FMOD_Sound_Release(g_pEffSound[i]);
	}
	FMOD_System_Close(g_pSystem);
	FMOD_System_Release(g_pSystem);
	FMOD_System_Close(g_pEffSystem);
	FMOD_System_Release(g_pEffSystem);
}

VOID WriteHighScore (int high_score)
{
	FILE *fp = fopen("high_score.txt", "w");

	if (fp != NULL)
	{
		fprintf(fp, "%d", high_score);
	}

	fclose(fp);
}

VOID WriteHighScoreHard(int high_score_hard)
{
	FILE *fp = fopen("high_score_hard.txt", "w");

	if (fp != NULL)
	{
		fprintf(fp, "%d", high_score_hard);
	}

	fclose(fp);
}

int ReadHighScore()
{
	int high_score = -1;

	FILE *fp = fopen("high_score.txt", "r");

	if (fp != NULL)
	{
		fscanf(fp, "%d", &high_score);
	}

	fclose(fp);

	return high_score;
}

int ReadHighScoreHard()
{
	int high_score_hard = -1;

	FILE *fp = fopen("high_score_hard.txt", "r");

	if (fp != NULL)
	{
		fscanf(fp, "%d", &high_score_hard);
	}

	fclose(fp);

	return high_score_hard;
}