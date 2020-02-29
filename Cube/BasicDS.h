#pragma once

#define	SPACE_EMPTY		0
#define SPACE_DEAD		1

#define	TIMER_SEC		1001
#define	TIME_REFRESH	10

#define TIMER_MOVE		1002
#define TIME_MOVE		800

#define TIMER_FALL		1003
#define TIME_FALL		1

#define MOVE_WAIT		6

#define MAX_SPEED		99
#define MIN_SPEED		1
#define DEFAULT_SPEED	2

#define TIMER_LEFTFALL	1005
#define TIME_LEFTFALL	50

#define GRID_WIDE_NUM	10
#define GRID_HEIGHT_NUM	20
#define GRID_SIZE		20

#define MAX_OBJ_SIZE	5

#define DIRECTION_UP	0
#define DIRECTION_LEFT	1
#define DIRECTION_DOWN	2
#define DIRECTION_RIGHT	3

#define BRUSH_NUM		16

#define WINDOW_WIDE		1024
#define WINDOW_HEIGHT	768

#define NUM_BMP_WIDE	600
#define NUM_BMP_HEIGHT	100

#define GRID_BMP_WIDE	320
#define GRID_STYLE_NUM	16
#define GRID_BMP_HEIGHT	20
#define DEAD_GRID_STYLE	9

#define GAMEWIN_TLC_X	412
#define GAMEWIN_TLC_Y	200

#define NEXT_OBJ_TLC_X	800
#define NEXT_OBJ_TLC_Y	100

#define MARK_TLC_X		800
#define MARK_TLC_Y		200
#define MARK_DIGIT_NUM	4
#define MARK_DIGIT_DIS	50
#define GOOD_MARK		300
#define BAD_MARK		15

#define SPEED_TLC_X		100
#define SPEED_TLC_Y		200
#define SPEED_DIGIT_NUM	2
#define SPEED_DIGIT_DIS	50

#define COUNT_TIME		1000
#define COUNT_TLC_X		412
#define COUNT_TLC_Y		200
#define COUNT_DIGIT_NUM	3
#define COUNT_DIGIT_DIS	50

#define GAME_OPENING	0
#define GAME_RUNNING	3
#define GAME_OVER		4

#define BUTTON_FREE		-1
#define BUTTON_START	0
#define BUTTON_EXIT		1
#define BUTTON_RESTART	0
#define BUTTON_RETURN	1

#pragma comment(lib, "winmm")
#pragma comment(lib, "comctl32.lib")
#include <math.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
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
#include "BasicFunc.h"
using namespace std;

struct  POSITION{
	int	iLine;
	int	iColumn;
	POSITION	() {}
	POSITION	(int il, int ic)
	{
		this->iLine	= il;
		this->iColumn = ic;
	}
};

typedef  vector <POSITION>	AREA;

class CANVAS
{
public:
	HDC		hCDC, hOldCDC;
	HBITMAP	hCBitmap, hOldCBitmap;
	int		*iPixel;
	TCHAR	ImageName[128];
	CANVAS	()
	{
		this->iPixel	= NULL;
	}
	CANVAS	(TCHAR	*FileName)
	{
		lstrcpy(FileName, ImageName);
		if (NULL == this->iPixel)
		{
			iPixel	= (int*)malloc(sizeof(int)*WINDOW_WIDE*WINDOW_HEIGHT*4);
		}
		this->hCBitmap	= (HBITMAP)LoadImage(NULL,
			FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetBitmapBits(hCBitmap,
			WINDOW_WIDE*WINDOW_HEIGHT*4,
			this->iPixel) ;
	}
	void	Draw(HDC	hDC, HDC	hMemDC)
	{
		this->hCDC		= CreateCompatibleDC(hDC);

		SetBitmapBits(hCBitmap,
			WINDOW_WIDE*WINDOW_HEIGHT*4,
			this->iPixel) ;
		hOldCBitmap = (HBITMAP)SelectObject(hCDC,hCBitmap) ;

		BitBlt(hMemDC,
			0,
			0,
			WINDOW_WIDE,
			WINDOW_HEIGHT,
			hCDC,0,0,SRCCOPY) ;

		SelectObject(hCDC,hOldCBitmap) ;
		DeleteDC(hCDC) ;
		return;
	}
protected:
private:
};

class OPBUTTON
{
public:
	HDC		hOPB_DC, HOPB_OldDC;
	HBITMAP	hOPB_Bitmap, hOPB_OldBitmap;
	int		*iPixel;

	POINT	pDrawTLC;
	POINT	pDrawSize;
	RECT	rectInduce;

	OPBUTTON	()
	{
		this->iPixel	= NULL;
	}
	OPBUTTON	(TCHAR	*FileName,
		int	pdtlc_x,
		int	pdtlc_y,
		int	pds_wide,
		int	pds_height,
		int	ri_left,
		int	ri_top,
		int	ri_right,
		int	ri_bottom)
	{
		if (NULL == iPixel)
		{
			this->iPixel	= (int*)malloc(sizeof(int)*WINDOW_WIDE*WINDOW_HEIGHT*4);
		}
		this->pDrawTLC.x	= pdtlc_x;
		this->pDrawTLC.y	= pdtlc_y;
		this->pDrawSize.x	= pds_wide;
		this->pDrawSize.y	= pds_height;
		this->rectInduce.left	= ri_left;
		this->rectInduce.top	= ri_top;
		this->rectInduce.right	= ri_right;
		this->rectInduce.bottom	= ri_bottom;

		this->hOPB_Bitmap	= (HBITMAP)LoadImage(NULL,
			FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetBitmapBits(hOPB_Bitmap,
			WINDOW_WIDE*WINDOW_HEIGHT*4,
			this->iPixel) ;
	}
	VOID	Draw(HDC	hMemDC)
	{
		this->hOPB_DC		= CreateCompatibleDC(hMemDC);

		SetBitmapBits(hOPB_Bitmap,
			WINDOW_WIDE*WINDOW_HEIGHT*4,
			this->iPixel) ;
		hOPB_OldBitmap = (HBITMAP)SelectObject(hOPB_DC,hOPB_Bitmap) ;

		BitBlt(hMemDC,
			pDrawTLC.x,
			pDrawTLC.y,
			pDrawSize.x,
			pDrawSize.y,
			hOPB_DC,
			pDrawTLC.x,
			pDrawTLC.y,
			SRCCOPY) ;

		SelectObject(hOPB_DC,hOPB_OldBitmap) ;
		DeleteDC(hOPB_DC) ;
		return;
	}
protected:
private:
};

typedef vector <AREA> VALIST;


class GRID
{
public:
	HDC	hGridDC, hOldGridDC;
	HBITMAP	hGridBitmap, hOldGridBitmap;
	int	*iPixel;
	TCHAR	szStr[128];
	int	i, j, k;
	GRID	()
	{
		this->iPixel	= NULL;
	}
	GRID	(TCHAR	*FileName)
	{
		if (NULL == this->iPixel)
		{
			this->iPixel	= (int*)malloc(sizeof(int)*GRID_BMP_WIDE*GRID_BMP_HEIGHT*4);
		}

		this->hGridBitmap= (HBITMAP)LoadImage(NULL,
			FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetBitmapBits(hGridBitmap,
			GRID_BMP_WIDE*GRID_BMP_HEIGHT*4,
			this->iPixel) ;
	}
	VOID	Draw(HDC	hMemDC, POSITION	pGridTLC, AREA	&Area, int	ColorID)
	{
		if (ColorID < GRID_STYLE_NUM)
		{
			this->hGridDC	= CreateCompatibleDC(hMemDC);
			SetBitmapBits(hGridBitmap, GRID_BMP_WIDE*GRID_BMP_HEIGHT*4,
				this->iPixel);
			hOldGridBitmap	= (HBITMAP)SelectObject(hGridDC, hGridBitmap);
			for (i=0 ; i<Area.size() ; i++)
			{
				BitBlt(hMemDC,
					GAMEWIN_TLC_X+(pGridTLC.iColumn+Area[i].iColumn)*GRID_SIZE,
					GAMEWIN_TLC_Y+(pGridTLC.iLine+Area[i].iLine)*GRID_SIZE,
					GRID_SIZE, GRID_SIZE,
					hGridDC, GRID_BMP_WIDE/GRID_STYLE_NUM*(ColorID), 0, SRCCOPY);
			}


			SelectObject(hGridDC, hOldGridBitmap);
			DeleteDC(hGridDC);
		}
		return;
	}
protected:
private:
};

class OBJECT
{
public:
	VALIST	vaBlock;
	int		iDirection;
	POSITION	pTLC;
	int		iTemp;
	HBRUSH	hObjBrush;
	int		iGridStyle;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	OBJECT	()	{}
	BOOL	Turn(bool bClockwise)
	{
		if (FALSE == bClockwise)
		{
			this->iDirection	= (this->iDirection + 1)%vaBlock.size();
		}
		else
		{
			this->iDirection	= (this->iDirection + vaBlock.size()-1)%vaBlock.size();
		}
		return	TRUE;
	}
	VOID	Draw(HDC	hMemDC, GRID	&GridEngine)
	{
		static	int i;
	/*	SelectObject(hMemDC, this->hObjBrush);
		for (i=0 ; i<this->vaBlock[this->iDirection].size() ; i++)
		{
			RoundRect(hMemDC,
				GAMEWIN_TLC_X+(this->vaBlock[this->iDirection][i].iColumn + this->pTLC.iColumn) * GRID_SIZE,		//left
				GAMEWIN_TLC_Y+(this->vaBlock[this->iDirection][i].iLine + this->pTLC.iLine) * GRID_SIZE,			//top
				GAMEWIN_TLC_X+(this->vaBlock[this->iDirection][i].iColumn + this->pTLC.iColumn + 1) * GRID_SIZE,	//right
				GAMEWIN_TLC_Y+(this->vaBlock[this->iDirection][i].iLine + this->pTLC.iLine + 1) * GRID_SIZE,		//bottom
				GRID_SIZE/3,	//xr
				GRID_SIZE/3);	//yr
		}*/
		GridEngine.Draw(hMemDC,
				this->pTLC,
				(this->vaBlock[this->iDirection]),
				this->iGridStyle);
		return;
	}
	VOID	NextDraw(HDC	hMemDC, GRID	&GridEngine)
	{
		static	int	i;
		/*SelectObject(hMemDC, this->hObjBrush);
		for (i=0 ; i<this->vaBlock[this->iDirection].size() ; i++)
		{
			RoundRect(hMemDC,
				NEXT_OBJ_TLC_X+(this->vaBlock[this->iDirection][i].iColumn + this->pTLC.iColumn) * GRID_SIZE,		//left
				NEXT_OBJ_TLC_Y+(this->vaBlock[this->iDirection][i].iLine + this->pTLC.iLine) * GRID_SIZE,			//top
				NEXT_OBJ_TLC_X+(this->vaBlock[this->iDirection][i].iColumn + this->pTLC.iColumn + 1) * GRID_SIZE,	//right
				NEXT_OBJ_TLC_Y+(this->vaBlock[this->iDirection][i].iLine + this->pTLC.iLine + 1) * GRID_SIZE,		//bottom
				GRID_SIZE/3,	//xr
				GRID_SIZE/3);	//yr
		}*/
		this->pTLC.iLine	= 0;
		this->iTemp			= this->pTLC.iColumn;
		this->pTLC.iColumn	= 15;
		GridEngine.Draw(hMemDC,
				this->pTLC,
				(this->vaBlock[this->iDirection]),
				this->iGridStyle);
		this->pTLC.iColumn	= this->iTemp;
		return;
	}
};


struct	SPACE{
	int	iSpace[GRID_HEIGHT_NUM][GRID_WIDE_NUM];
	int	iState;
	int	LowestCleanLine;
	POSITION	pTemp, pPush;
	int	CleanLine(VOID)
	{
		int	iCleanNum;
		int	i, j;
		this->LowestCleanLine	= -1;
		for (i=0, iCleanNum	= 0 ; i<GRID_HEIGHT_NUM ; i++)
		{
			for (j=0 ; j<GRID_WIDE_NUM && SPACE_DEAD == iSpace[i][j] ; j++);
			if (j == GRID_WIDE_NUM)
			{
				if (this->LowestCleanLine < i)
				{
					this->LowestCleanLine	= i;
				}
				iCleanNum++;
				for (j=0 ; j<GRID_WIDE_NUM ; iSpace[i][j]=SPACE_EMPTY, j++);
			}
		}
		return	iCleanNum;
	}
	VOID	LeftFallSearch(vector <OBJECT> &voList, HBRUSH	*hBrushList)
	{
		OBJECT	NewObj;
		AREA	NewArea;
		queue <POSITION>	PQ;
		int	Dir[4][2]	= {-1, 0, 1, 0, 0, -1, 0, 1};
		int	i, j, k, d;
		for (i=GRID_HEIGHT_NUM-1 ; i>=0 ; i--)
		{
			for (j=0 ; j<GRID_WIDE_NUM ; j++)
			{
				if (SPACE_DEAD == iSpace[i][j])
				{
					////////////////////////////
					NewObj.vaBlock.clear();
					NewObj.iDirection	= 0;
					NewObj.pTLC.iLine	= 0;
					NewObj.pTLC.iColumn	= 0;
					NewArea.clear();
					////////////////////////////
					while (false == PQ.empty())		PQ.pop();
					PQ.push(POSITION(i, j));
					NewArea.push_back(POSITION(i, j));
					iSpace[i][j]	= SPACE_EMPTY;
					while (false == PQ.empty())
					{
						pTemp	= PQ.front();
						PQ.pop();
						for (k=0 ; k<4 ; k++)
						{
							pPush.iLine		= pTemp.iLine	+ Dir[k][0];
							pPush.iColumn	= pTemp.iColumn + Dir[k][1];
							if (0 <= pPush.iLine &&
								pPush.iLine < GRID_HEIGHT_NUM &&
								0 <= pPush.iColumn &&
								pPush.iColumn < GRID_WIDE_NUM &&
								SPACE_DEAD == iSpace[pPush.iLine][pPush.iColumn])
							{
								PQ.push(pPush);
								iSpace[pPush.iLine][pPush.iColumn]	= SPACE_EMPTY;
								NewArea.push_back(pPush);
							}
						}
					}
					for (k=0 ; k<NewArea.size() ; k++)
					{
						if (this->LowestCleanLine > NewArea[k].iLine)
						{
							break;
						}
					}
					if (k < NewArea.size())
					{
						NewObj.vaBlock.push_back(NewArea);
						NewObj.hObjBrush	= hBrushList[voList.size()%BRUSH_NUM];
						NewObj.iGridStyle	= voList.size()%BRUSH_NUM;
						voList.push_back(NewObj);
					}
					else
					{
						for (k=0 ; k<NewArea.size() ; k++)
						{
							this->iSpace[NewArea[k].iLine][NewArea[k].iColumn]	= SPACE_DEAD;
						}
					}
				}
			}
		}
		return;
	}
	VOID	Draw(HDC	hMemDC, GRID	&GridEngine)
	{
		int	 i, j;
		static	AREA Area;
		Area.clear();
		for (i=0 ; i<GRID_HEIGHT_NUM ; i++)
		{
			for (j=0 ; j<GRID_WIDE_NUM ; j++)
			{
				if (SPACE_DEAD == iSpace[i][j])
				{
					/*RoundRect(hMemDC,
						GAMEWIN_TLC_X+j*GRID_SIZE,		//left
						GAMEWIN_TLC_Y+i*GRID_SIZE,		//top
						GAMEWIN_TLC_X+(j+1)*GRID_SIZE,	//right
						GAMEWIN_TLC_Y+(i+1)*GRID_SIZE,	//bottom
						GRID_SIZE/3,	//xr
						GRID_SIZE/3);	//yr*/
					pTemp.iLine		= i;
					pTemp.iColumn	= j;
					Area.push_back(pTemp);
				}
			}
		}
		pPush.iLine	= pPush.iColumn	= 0;
		GridEngine.Draw(hMemDC, pPush, Area, DEAD_GRID_STYLE);
	}
};

class NUMBER
{
public:
	HDC	hNumDC, hOldNumDC;
	HBITMAP	hNumBitmap, hOldNumBitmap;
	int	*iPixel;
	TCHAR	szStr[128];
	int	i, j, k;
	NUMBER	()
	{
		this->iPixel	= NULL;
	}
	NUMBER	(TCHAR *FileName)
	{
		if (NULL == iPixel)
		{
			this->iPixel	= (int*)malloc(sizeof(int)*NUM_BMP_WIDE*NUM_BMP_HEIGHT*4);
		}

		this->hNumBitmap= (HBITMAP)LoadImage(NULL,
			FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		GetBitmapBits(hNumBitmap,
			NUM_BMP_WIDE*NUM_BMP_HEIGHT*4,
			this->iPixel) ;
	}
	VOID	Draw(HDC	hMemDC, POINT	pTLC, TCHAR	cNum)
	{
		if ('0' <= cNum && cNum <= '9')
		{
			this->hNumDC	= CreateCompatibleDC(hMemDC);
			SetBitmapBits(hNumBitmap, NUM_BMP_WIDE*NUM_BMP_HEIGHT*4,
				this->iPixel);
			hOldNumBitmap	= (HBITMAP)SelectObject(hNumDC, hNumBitmap);

			BitBlt(hMemDC,
				pTLC.x, pTLC.y,
				NUM_BMP_WIDE/10, NUM_BMP_HEIGHT,
				hNumDC, NUM_BMP_WIDE/10*(cNum-'0'), 0, SRCPAINT);

			SelectObject(hNumDC, hOldNumBitmap);
			DeleteDC(hNumDC);
		}
		return;
	}
protected:
private:
};
struct GAMEENGINE {
	bool	bPause;
	int		GameState;
	int		iMark;
	int		iButtonID;
	int		iLastButtonID;
	int		iSpeed;
	int		i, j;
	HWND	hwndMain;
	POINT	pTLC;
	TCHAR	szStr[128];
	GAMEENGINE	()	{}
	GAMEENGINE	(HWND hwnd)
	{
		hwndMain = hwnd;
		this->iSpeed	= DEFAULT_SPEED;
	}
	VOID	ShowPause(HDC	hMemDC)
	{
		SetTextColor(hMemDC, RGB(200, 200, 0));
		SetBkMode(hMemDC, TRANSPARENT);
		TextOut(hMemDC, GAMEWIN_TLC_X, GAMEWIN_TLC_Y, TEXT("PAUSE"), 5);
		return;
	}
	VOID	ShowTitle(HDC	hMemDC)
	{
		return;
	}
	VOID	ShowRestart(HDC	hMemDC)
	{
		return;
	}
	VOID	ShowCountDown(HDC	hMemDC, NUMBER	&NumEngine, int iCountDown)
	{
		Int2Str(iCountDown/TIME_REFRESH, COUNT_DIGIT_NUM, this->szStr);
		for (i=lstrlen(szStr)+1, j=0 ; j<COUNT_DIGIT_NUM ; szStr[i] = szStr[i-1], i--, j++);
		szStr[i]	= '.';
		this->pTLC.x	= COUNT_TLC_X;
		this->pTLC.y	= COUNT_TLC_Y+iCountDown%1000/10;
		for (i=0 ; i<=COUNT_DIGIT_NUM ; i++, this->pTLC.x+=COUNT_DIGIT_DIS)
		{
			NumEngine.Draw(hMemDC,
				this->pTLC,
				szStr[i]);
		}
		return;
	}
	VOID	ShowSpeed(HDC	hMemDC, NUMBER &NumEngine)
	{
		Int2Str(this->iSpeed, SPEED_DIGIT_NUM, this->szStr);
		for (i=0 ; i<SPEED_DIGIT_NUM && '0' == this->szStr[i] ; this->szStr[i] = ' ', i++);
		if (i == SPEED_DIGIT_NUM)
		{
			this->szStr[SPEED_DIGIT_NUM-1]	= '0';
		}
		this->pTLC.x	= SPEED_TLC_X;
		this->pTLC.y	= SPEED_TLC_Y;
		for (i=0 ; i<SPEED_DIGIT_NUM ; i++, this->pTLC.x+=SPEED_DIGIT_DIS)
		{
			NumEngine.Draw(hMemDC,
				this->pTLC,
				szStr[i]);
		}
		return;
	}
	VOID	ShowMark(HDC	hMemDC, NUMBER &NumEngine)
	{
		Int2Str(this->iMark, MARK_DIGIT_NUM, this->szStr);
		for (i=0 ; i<MARK_DIGIT_NUM && '0' == this->szStr[i] ; this->szStr[i] = ' ', i++);
		if (i == MARK_DIGIT_NUM)
		{
			this->szStr[MARK_DIGIT_NUM-1]	= '0';
		}
		this->pTLC.x	= MARK_TLC_X;
		this->pTLC.y	= MARK_TLC_Y;
		for (i=0 ; i<MARK_DIGIT_NUM ; i++, this->pTLC.x+=MARK_DIGIT_DIS)
		{
			NumEngine.Draw(hMemDC,
				this->pTLC,
				szStr[i]);
		}
		return;
	}
	VOID	SwitchState(int	NewState, CANVAS &Canvas, SPACE &Space, int lParam)
	{
		this->GameState	= NewState;
		this->bPause	= FALSE;
		switch (NewState)
		{
		case	GAME_OPENING:
			Canvas	= CANVAS("MO_BMP\\MO_Title.bmp");
			break;
		case	GAME_RUNNING:
			PlaySound("MO_BGM\\MO_Start.wav", NULL, SND_FILENAME | SND_ASYNC);
			this->iMark		= 0;
			Canvas	= CANVAS("MO_BMP\\MO_BackGround.bmp");
			memset(Space.iSpace, SPACE_EMPTY, sizeof(Space.iSpace));
			break;
		case	GAME_OVER:
			if (VK_ESCAPE != lParam)
			{
				if (this->iMark >= GOOD_MARK)
				{
					PlaySound("MO_BGM\\MO_GameGoodOver.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
				else if (this->iMark < BAD_MARK)
				{
					PlaySound("MO_BGM\\MO_GameBadOver.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
				else
				{
					PlaySound("MO_BGM\\MO_GameOver.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else
			{
				PlaySound("MO_BGM\\MO_End.wav", NULL, SND_FILENAME | SND_ASYNC);
			}
			Canvas	= CANVAS("MO_BMP\\MO_GAMEOVER.bmp");
			break;
		}
		return;
	}
	VOID	SwitchSpeed(BOOL	bFaster)
	{
		if (TRUE	== bFaster &&
			this->iSpeed < MAX_SPEED)
		{
			this->iSpeed++;
			SetTimer(this->hwndMain, TIMER_FALL, TIME_FALL * 1000 / this->iSpeed, NULL);
		}
		else if (FALSE == bFaster &&
			this->iSpeed > MIN_SPEED)
		{
			this->iSpeed--;
			SetTimer(this->hwndMain, TIMER_FALL, TIME_FALL * 1000 / this->iSpeed, NULL);
		}
		return;
	}
};

