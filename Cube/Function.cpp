#include <windows.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "BasicDS.h"
using namespace std;


inline int	GetMarks(int LineNum);


int	Initialization(TCHAR	*szFileName, vector <OBJECT> &voList)
{
	int		iObjNum;
	int		AreaSize;
	int		iDirNum;
	OBJECT	NewObj;
	AREA	NewArea;
	int	i, j;
//	int		ColorR, ColorG, ColorB;
	int		PicIndex;
	FILE	*fp;
	TCHAR	Info[MAX_OBJ_SIZE][MAX_OBJ_SIZE];
	voList.clear();
	if (NULL != (fp = fopen(szFileName, "r")))
	{
		fscanf(fp, "%d", &iObjNum);
		while (iObjNum--)
		{
			fscanf(fp, "%d%d", &AreaSize, &iDirNum);
			/*	fscanf(fp, "%d%d%d", &ColorR, &ColorG, &ColorB);
			ColorR %= 256;
			ColorG %= 256;
			ColorB %= 256;
			NewObj.hObjBrush	= CreateSolidBrush(RGB(ColorR, ColorG, ColorB));*/
			fscanf(fp, "%d", &PicIndex);
			NewObj	= OBJECT();
			NewObj.vaBlock.clear();
			while (iDirNum--)
			{
				memset(Info, '\0', sizeof(Info));
				NewArea.clear();
				for (i=0 ; i<AreaSize ; i++)
				{
					fgets(Info[i], MAX_OBJ_SIZE, fp);
					if (AreaSize > lstrlen(Info[i]))
					{
						i--;
						continue;
					}
					for (j=0 ; j<MAX_OBJ_SIZE ; j++)
					{
						if ('#' == Info[i][j])
						{
							NewArea.push_back(POSITION(i, j));
						}
					}
				}
				NewObj.vaBlock.push_back(NewArea);
			}
			NewObj.iDirection	= 0;
			NewObj.pTLC.iColumn	= GRID_WIDE_NUM/2-AreaSize/2;
			NewObj.pTLC.iLine	= 0;
			NewObj.iGridStyle	= PicIndex;
			voList.push_back(NewObj);
		}
	}
	return	voList.size();
}

inline int	GetMarks(int LineNum)
{
	return (1 + LineNum) * LineNum / 2;
}

BOOL	CrashCheck(AREA	&Area, POSITION	&pTLC, SPACE	&Space)
{
	static	int	i;
	for (i=0 ; i<Area.size() ; i++)
	{
		if (pTLC.iColumn + Area[i].iColumn	<	0 ||
			pTLC.iColumn + Area[i].iColumn	>=	GRID_WIDE_NUM ||
			pTLC.iLine + Area[i].iLine	<	0 ||
			pTLC.iLine + Area[i].iLine	>=	GRID_HEIGHT_NUM ||
			SPACE_DEAD	==
			Space.iSpace[pTLC.iLine + Area[i].iLine][pTLC.iColumn + Area[i].iColumn])
		{
			return	TRUE;
		}
	}
	return	FALSE;
}

BOOL	InRect(POINT	p, RECT rect)
{
	return (
		p.x > rect.left &&
		p.x < rect.right &&
		p.y > rect.top &&
		p.y < rect.bottom);
}