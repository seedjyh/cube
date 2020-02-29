#pragma comment(lib, "winmm")
#pragma comment(lib, "comctl32.lib")
// #include <Dmusici.h>
#include <math.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include <wtypes.h>
#include <windowsx.h>
#include <winbase.h>
#include <windows.h>
#include <winuser.h>
#include <tchar.h>
#include <psapi.h>
#include <time.h>
#include <dos.h>
#include <conio.h>
#include "resource.h"
#include "Function.h"
#include "BasicDS.h"
using namespace std;


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;
//窗口消息处理程序	（窗口句柄，消息类型，消息数据1，消息数据2）

int WINAPI WinMain (HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					PSTR		szCmdLine,
					int			iCmdShow)
{
	int	cxClient, cyClient;
	RECT	rectMain;


	static		TCHAR szAppName[] = TEXT ("Cube") ;
	HWND		hwnd ;
	MSG			msg ;
	WNDCLASS	wndclass ;
	wndclass.style			= CS_HREDRAW | CS_VREDRAW ;

	wndclass.lpfnWndProc	= WndProc ;
	wndclass.cbClsExtra		= 0 ;
	wndclass.cbWndExtra		= 0 ;
	wndclass.hInstance		= hInstance ;
	wndclass.hIcon			= LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor		= LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground	= (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName	= NULL ;
	wndclass.lpszClassName	= szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL,
			TEXT ("This program requires Windows NT!"),
			szAppName,
			MB_ICONERROR) ;
		return 0 ;
	}
	hwnd	= CreateWindow(	szAppName,					// window class name
		TEXT ("Cube"),				// window caption
		WS_POPUP|WS_MAXIMIZE|WS_EX_TOPMOST|WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_CAPTION,		// window style
		0,				// initial x position
		0,				// initial y position
		WINDOW_WIDE,				// initial x size
		WINDOW_HEIGHT,				// initial y size
		NULL,						// parent window handle
		NULL,						// window menu handle
		hInstance,					// program instance handle
		NULL) ;						// creation parameters
	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	GetClientRect(hwnd, &rectMain);
	cxClient	= rectMain.right - rectMain.left;
	cyClient	= rectMain.bottom- rectMain.top;

	/////////////////////////////////////////////////////////////////////////////
	/*
	TCHAR	FileName[128] = "MO_BGM\\MO_BGM_OP.mid";
	MCI_OPEN_PARMS OpenParms;
	OpenParms.lpstrDeviceType = (LPCSTR) MCI_DEVTYPE_SEQUENCER; //MIDI类型
	OpenParms.lpstrElementName = (LPCSTR)FileName;
	OpenParms.wDeviceID = 10;
	mciSendCommand (NULL, MCI_OPEN, MCI_WAIT | MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD)(LPVOID) &OpenParms);
	MCI_PLAY_PARMS PlayParms;
	PlayParms.dwFrom = 0;
	// 指定从什么地方（时间）播放
	mciSendCommand (10, MCI_PLAY, MCI_FROM, (DWORD)(LPVOID)&PlayParms);*/
	/////////////////////////////////////////////////////////////////////////////

	MessageBox(hwnd, "俄罗斯方块，蒋甬杭制作SeedJYH@Gmail.com","关于", MB_OK);
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}


LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT			rectMain;
	static	BOOL	bKeyDown[1024];
	static	HDC		hDC, hMemDC;
	static	HBITMAP	hBitmap, hOldBitmap;



	PAINTSTRUCT		ps ;

	static	HINSTANCE	hInstance ;
	static	RECT	rect, hrect, rectText;


	static	int		cxChar, cyChar;
	static	int		cxClient, cyClient;	//窗口宽度和高度
	static	BOOL	NoUser;
	static	GAMEENGINE	GameEngine;
	static	NUMBER	NumEngine	= NUMBER();
	static	GRID	GridEngine	= GRID();
	static	int		iCleanLineNum;

	static	BOOL	bCursorState;

	AREA	AreaTemp;
	POINT	pTemp;
	POSITION	PositionTemp;
	FILE	*fp;
	int	i, j;
	static	int	LeftMoveCountDown;
	static	int	RightMoveCountDown;
	static	int	DownMoveCountDown;

	static	TCHAR	szStr[128];

	static	vector <OBJECT>	voList;
	static	OBJECT	NextObj;
	static	OBJECT	ThisObj;
	static	vector <OBJECT>	LeftObjList;
	static	SPACE	Space;

	static	CANVAS	Canvas	= CANVAS();
	static	OPBUTTON	opbGameStart= OPBUTTON();
	static	OPBUTTON	opbExit		= OPBUTTON();
	static	OPBUTTON	opbRestart	= OPBUTTON();
	static	OPBUTTON	opbReturn	= OPBUTTON();
	
	static	POINT	pMouse;
	static	HBRUSH	hBrushList[BRUSH_NUM];

	static	int		iCountDown;
	switch (message)
	{
	case WM_CREATE:
		SendMessage(hwnd,
			WM_SETICON,
			(WPARAM)TRUE,
			(LPARAM)LoadIcon(GetModuleHandle(NULL),
			(LPCTSTR)IDI_ICON1));
		bCursorState	= TRUE;
		//initialization
		//////////////////////////////////////////////brush
		hBrushList[0]	= CreateSolidBrush(RGB(200, 200, 255));
		hBrushList[1]	= CreateSolidBrush(RGB(200, 255, 200));
		hBrushList[2]	= CreateSolidBrush(RGB(200, 200, 200));
		hBrushList[3]	= CreateSolidBrush(RGB(200, 255, 255));
		hBrushList[4]	= CreateSolidBrush(RGB(255, 200, 255));
		hBrushList[5]	= CreateSolidBrush(RGB(255, 255, 200));
		Initialization("BlockInfo.dat", voList);

		
		opbGameStart	= OPBUTTON("MO_BMP\\MO_TitleGameStart.bmp",
			450, 350,
			560, 150,
			540, 400,
			830, 440);
		opbExit			= OPBUTTON("MO_BMP\\MO_TitleExit.bmp",
			480, 450,
			300, 150,
			540, 480,
			650, 520);
		opbRestart		= OPBUTTON("MO_BMP\\MO_TryAgain.bmp",
			350, 300,
			450, 200,
			430, 340,
			720, 400);
		opbReturn		= OPBUTTON("MO_BMP\\MO_Return.bmp",
			400, 430,
			580, 120,
			435, 453,
			860, 500);

		NumEngine	= NUMBER(TEXT("MO_BMP\\MO_Number.bmp"));
		GridEngine	= GRID(TEXT("MO_BMP\\MO_Grid.bmp"));

		GameEngine	= GAMEENGINE(hwnd);
		GameEngine.SwitchState(GAME_OPENING, Canvas, Space, -1);
		iCountDown	= COUNT_TIME;

		SetTimer(hwnd, TIMER_SEC, TIME_REFRESH, NULL) ;
		SetTimer(hwnd, TIMER_MOVE, TIME_MOVE, NULL) ;
		SetTimer(hwnd, TIMER_FALL, TIME_FALL * (1000/DEFAULT_SPEED), NULL) ;
		SetTimer(hwnd, TIMER_LEFTFALL, TIME_LEFTFALL, NULL) ;

		GetClientRect (hwnd, &rect) ;	//
		cxChar	= LOWORD(GetDialogBaseUnits());
		cyChar	= HIWORD(GetDialogBaseUnits());
		memset(bKeyDown, false, sizeof(bKeyDown));

		LeftObjList.clear();
		//方块生成
		srand((unsigned)time(NULL));
		NextObj	= voList[rand()%voList.size()];
		NoUser	= FALSE;

		//MIDI播放
		MCI_OPEN_PARMS OpenParms;
		OpenParms.lpstrDeviceType = (LPCSTR) MCI_DEVTYPE_SEQUENCER; //MIDI类型
		OpenParms.lpstrElementName = (LPCSTR)TEXT("BGM_OP.mid");
		OpenParms.wDeviceID = 0;
		mciSendCommand (NULL, MCI_OPEN, MCI_WAIT | MCI_OPEN_TYPE | MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT, (DWORD)(LPVOID) &OpenParms);
		///////////////////////////////////////////////////
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case TIMER_SEC:	//Draw
			hDC		= GetDC(hwnd);
			hMemDC	= CreateCompatibleDC(hDC);
			GetClientRect(hwnd, &rectMain);
			hBitmap	= CreateCompatibleBitmap(hDC, rectMain.right, rectMain.bottom);
			hOldBitmap	= (HBITMAP)SelectObject(hMemDC, hBitmap);

			//////Move Operator
			if (bKeyDown[VK_LEFT])
			{
				if (0 == LeftMoveCountDown)
				{
					ThisObj.pTLC.iColumn--;
					if (CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
					{
						ThisObj.pTLC.iColumn++;
					}
				}
				else
				{
					LeftMoveCountDown--;
				}
			}
			else if (bKeyDown[VK_RIGHT])
			{
				if (0 == RightMoveCountDown)
				{
					ThisObj.pTLC.iColumn++;
					if (CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
					{
						ThisObj.pTLC.iColumn--;
					}
				}
				else
				{
					RightMoveCountDown--;
				}
			}
			//////Draw start
			Canvas.Draw(hDC, hMemDC);
			switch (GameEngine.GameState)
			{
			case	GAME_OPENING:
				/////////////////////////////
				pTemp.x	= pTemp.y	= 100;
				//////////////////////////////////////////////////////////////////////////
				
				switch (GameEngine.iButtonID)
				{
				case	BUTTON_START:
					opbGameStart.Draw(hMemDC);
					break;
				case	BUTTON_EXIT:
					opbExit.Draw(hMemDC);
					break;
				}
				break;
			case	GAME_RUNNING:
				Space.Draw(hMemDC, GridEngine);
				if (GameEngine.bPause)
				{
					GameEngine.ShowPause(hMemDC);
				}
				if (iCountDown > 0)
				{
					if (iCountDown<=TIME_REFRESH)
					{
						fp= fopen("MO_BGM\\MO_BGM_Run.wav", "r");
						if (NULL != fp)
						{
							fclose(fp);
							PlaySound("MO_BGM\\MO_BGM_Run.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
						}
						ThisObj	= NextObj;
						NextObj	= voList[rand()%voList.size()];
					}
					GameEngine.ShowCountDown(hMemDC, NumEngine, iCountDown);
					iCountDown-=TIME_REFRESH;
				}
				else
				{
					GameEngine.ShowMark(hMemDC, NumEngine);
					GameEngine.ShowSpeed(hMemDC, NumEngine);
				}

				if (FALSE == NoUser && iCountDown <= 0)
				{
					ThisObj.Draw(hMemDC, GridEngine);
				}
				for (i=0 ; i<LeftObjList.size() ; i++)
				{
					LeftObjList[i].Draw(hMemDC, GridEngine);
				}
				NextObj.NextDraw(hMemDC, GridEngine);
				//word
				
				break;
			case	GAME_OVER:
				switch (GameEngine.iButtonID)
				{
				case	BUTTON_RESTART:
					opbRestart.Draw(hMemDC);
					break;
				case	BUTTON_RETURN:
					opbReturn.Draw(hMemDC);
					break;
				}
				GameEngine.ShowMark(hMemDC, NumEngine);
				break;
			}
			//////Draw over
			BitBlt(hDC,	0,	0,	cxClient,	cyClient,
				hMemDC,	0, 0, SRCCOPY);

			SelectObject(hMemDC, hOldBitmap);
			DeleteDC(hMemDC);
			ReleaseDC(hwnd, hDC);
			DeleteObject(hBitmap);
			break;
		case	TIMER_LEFTFALL:
			if (GameEngine.bPause || iCountDown > 0)
				break;
			if (GAME_RUNNING != GameEngine.GameState)
			{
				break;
			}
			if (0 < LeftObjList.size())
			{
				for (i=0 ; i<LeftObjList.size() ; i++)
				{
					LeftObjList[i].pTLC.iLine++;
					if (CrashCheck(LeftObjList[i].vaBlock[0], LeftObjList[i].pTLC, Space))
					{
						LeftObjList[i].pTLC.iLine--;
						AreaTemp	= LeftObjList[i].vaBlock[0];
						for (j=0 ; j<AreaTemp.size() ; j++)
						{
							PositionTemp	= AreaTemp[j];
							Space.iSpace[LeftObjList[i].pTLC.iLine+PositionTemp.iLine][LeftObjList[i].pTLC.iColumn+PositionTemp.iColumn]	= SPACE_DEAD;
						}
						iCleanLineNum	= Space.CleanLine();
						if (0 < iCleanLineNum)
						{
							GameEngine.iMark	+= iCleanLineNum*iCleanLineNum;
							Space.LeftFallSearch(LeftObjList, hBrushList);
						}
						LeftObjList.erase(LeftObjList.begin()+i);
						i--;
					}
				}
			}
			else if (NoUser)
			{
				ThisObj	= NextObj;
				NextObj	= voList[rand()%voList.size()];
				NoUser	= FALSE;
			}
			break;
		case	TIMER_FALL:
			if (GameEngine.bPause || iCountDown > 0)
				break;
			if (GAME_RUNNING != GameEngine.GameState)
			{
				break;
			}
			ThisObj.pTLC.iLine++;
			if (FALSE == NoUser &&
				CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
			{
				NoUser	= TRUE;
				ThisObj.pTLC.iLine--;
				AreaTemp	= ThisObj.vaBlock[ThisObj.iDirection];
				for (i=0 ; i<AreaTemp.size() ; i++)
				{
					PositionTemp	= AreaTemp[i];
					Space.iSpace[ThisObj.pTLC.iLine+PositionTemp.iLine][ThisObj.pTLC.iColumn+PositionTemp.iColumn]	= SPACE_DEAD;
				}
				iCleanLineNum	= Space.CleanLine();
				//如果能收
				if (iCleanLineNum > 0)
				{
					GameEngine.iMark	+= iCleanLineNum*iCleanLineNum;
					Space.LeftFallSearch(LeftObjList, hBrushList);
				}
				else if (0 == ThisObj.pTLC.iLine)
				{
					GameEngine.SwitchState(GAME_OVER, Canvas, Space, -1);
					break;
				}
			}
			break;
		}
		return 0;
	case	WM_KEYDOWN:
		switch (wParam)
		{
		case	VK_LEFT:
			bKeyDown[VK_LEFT]	= TRUE;
			LeftMoveCountDown	= MOVE_WAIT;
			ThisObj.pTLC.iColumn--;
			if (CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
			{
				ThisObj.pTLC.iColumn++;
			}
			break;
		case	VK_DOWN:
			while (1)
			{
				ThisObj.pTLC.iLine++;
				if (CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
				{
					ThisObj.pTLC.iLine--;
					if (0 == ThisObj.pTLC.iLine)
					{
						GameEngine.SwitchState(GAME_OVER, Canvas, Space, VK_DOWN);
						return 0;
					}
					else
					{
						SetTimer(hwnd, TIMER_FALL, TIME_FALL * 1000 / GameEngine.iSpeed, NULL);
					}
					break;
				}
			}
			if (GameEngine.bPause || iCountDown > 0)
				break;
			if (GAME_RUNNING != GameEngine.GameState)
			{
				break;
			}
			ThisObj.pTLC.iLine++;
			if (FALSE == NoUser &&
				CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
			{
				NoUser	= TRUE;
				ThisObj.pTLC.iLine--;
				AreaTemp	= ThisObj.vaBlock[ThisObj.iDirection];
				for (i=0 ; i<AreaTemp.size() ; i++)
				{
					PositionTemp	= AreaTemp[i];
					Space.iSpace[ThisObj.pTLC.iLine+PositionTemp.iLine][ThisObj.pTLC.iColumn+PositionTemp.iColumn]	= SPACE_DEAD;
				}
				iCleanLineNum	= Space.CleanLine();
				//如果能收
				if (iCleanLineNum > 0)
				{
					GameEngine.iMark	+= iCleanLineNum*iCleanLineNum;
					Space.LeftFallSearch(LeftObjList, hBrushList);
				}
				else if (0 == ThisObj.pTLC.iLine)
				{
					GameEngine.SwitchState(GAME_OVER, Canvas, Space, -1);
					break;
				}
			}
			break;
		case	VK_SPACE:
			ThisObj.pTLC.iLine++;
			if (CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
			{
				ThisObj.pTLC.iLine--;
			}
			break;
		case	VK_RIGHT:
			bKeyDown[VK_RIGHT]	= TRUE;
			RightMoveCountDown	= MOVE_WAIT;
			ThisObj.pTLC.iColumn++;
			if (CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
			{
				ThisObj.pTLC.iColumn--;
			}
			break;
		case	VK_UP:
			ThisObj.Turn(TRUE);
			if (CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
			{
				ThisObj.Turn(TRUE);
			}
			break;
		case	'A':
			ThisObj.Turn(TRUE);
			if (CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
			{
				ThisObj.Turn(FALSE);
			}
			break;
		case	'S':
			ThisObj.Turn(FALSE);
			if (CrashCheck(ThisObj.vaBlock[ThisObj.iDirection], ThisObj.pTLC, Space))
			{
				ThisObj.Turn(TRUE);
			}
			break;
		case	'P':
			GameEngine.bPause	= !GameEngine.bPause;
			break;
		case	VK_ADD:
			GameEngine.SwitchSpeed(TRUE);
			break;
		case	VK_SUBTRACT:
			GameEngine.SwitchSpeed(FALSE);
			break;
		case	VK_ESCAPE:
			switch (GameEngine.GameState)
			{
			case	GAME_OPENING:
			case	GAME_OVER:
				PostQuitMessage(0);
				break;
			case	GAME_RUNNING:
				GameEngine.SwitchState(GAME_OVER, Canvas, Space, VK_ESCAPE);
				break;
			}
			break;
		}
		return 0;
	case	WM_KEYUP:
		switch (wParam)
		{
		case	VK_LEFT:
			bKeyDown[VK_LEFT]	= FALSE;
			break;
		case	VK_RIGHT:
			bKeyDown[VK_RIGHT]	= FALSE;
			break;
		}
		break;
	case	WM_MOUSEMOVE:
		if (GAME_RUNNING == GameEngine.GameState)
		{
			if(bCursorState == TRUE)
			{
				bCursorState = FALSE;
				ShowCursor(FALSE);
			}
			// 然后做别的事情
		}
		else
		{
			if(bCursorState == FALSE)
			{
				bCursorState = TRUE;
				ShowCursor(TRUE);
			}
		}
		pMouse.x	= LOWORD(lParam);
		pMouse.y	= HIWORD(lParam);
		GameEngine.iLastButtonID	= GameEngine.iButtonID;
		switch(GameEngine.GameState)
		{
		case GAME_OPENING:
			if (InRect(pMouse, opbGameStart.rectInduce))
			{
				GameEngine.iButtonID	= BUTTON_START;
			}
			else if (InRect(pMouse, opbExit.rectInduce))
			{
				GameEngine.iButtonID	= BUTTON_EXIT;
			}
			else
			{
				GameEngine.iButtonID	= BUTTON_FREE;
				GameEngine.iLastButtonID= BUTTON_FREE;
			}
			break;
		case	GAME_RUNNING:
			break;
		case GAME_OVER:
			if (InRect(pMouse, opbRestart.rectInduce))
			{
				GameEngine.iButtonID	= BUTTON_RESTART;
			}
			else if (InRect(pMouse, opbReturn.rectInduce))
			{
				GameEngine.iButtonID	= BUTTON_RETURN;
			}
			else
			{
				GameEngine.iButtonID	= BUTTON_FREE;
				GameEngine.iLastButtonID= BUTTON_FREE;
			}
			break;
		}
		if (GameEngine.iButtonID != GameEngine.iLastButtonID)
		{
			PlaySound("MO_BGM\\MO_Button.wav", NULL, SND_FILENAME | SND_ASYNC);
		}
		return 0;
	case   WM_COMMAND :
		return 0;
	case	WM_LBUTTONDOWN:
		switch(GameEngine.GameState)
		{
		case GAME_OPENING:
			switch (GameEngine.iButtonID)
			{
			case	BUTTON_START:
				PlaySound("MO_BGM\\MO_Start.wav", NULL, SND_FILENAME | SND_ASYNC);
				GameEngine.SwitchState(GAME_RUNNING, Canvas, Space, BUTTON_START);
				iCountDown	= COUNT_TIME;
				break;
			case	BUTTON_EXIT:
				PostQuitMessage (0) ;
				break;
			}
			break;
		case GAME_RUNNING:
			break;
		case GAME_OVER:
			switch (GameEngine.iButtonID)
			{
			case	BUTTON_RESTART:
				GameEngine.SwitchState(GAME_RUNNING, Canvas, Space, BUTTON_RESTART);
				iCountDown	= COUNT_TIME;

				NextObj	= voList[rand()%voList.size()];
				ThisObj	= voList[rand()%voList.size()];
				NoUser	= FALSE;
				break;
			case	BUTTON_RETURN:
				GameEngine.SwitchState(GAME_OPENING, Canvas, Space, BUTTON_RETURN);
				break;
			}
			break;
		}
		return 0;
	case	WM_RBUTTONUP:
		return 0;
	case	WM_SIZE:
		cxClient = LOWORD (lParam) ;
		cyClient = HIWORD (lParam) ;
		rect.right	= rect.left + cxClient;
		rect.bottom	= rect.top + cyClient;
		return 0 ;
	case   WM_PAINT:
		hDC = BeginPaint (hwnd, &ps) ;
		EndPaint (hwnd, &ps) ;
		return 0 ;
	case   WM_DESTROY:
		PostQuitMessage (0) ;
		mciSendCommand (0, MCI_CLOSE, NULL, NULL);
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}