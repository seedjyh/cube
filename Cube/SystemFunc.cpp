#include <windows.h>
#include <vector>
#include <vector>
#include "Function.h"
#include "BasicDS.h"
int	Initialization(TCHAR	*szFileName, vector <OBJECT> &voList)
{
	int		iObjNum;
	int		AreaSize;
	int		iDirNum;
	OBJECT	NewObj;
	AREA	NewArea;
	int	i, j;
	FILE	*fp;
	TCHAR	Info[MAX_OBJ_SIZE][MAX_OBJ_SIZE];
	voList.clear();
	if (NULL != (fp = fopen(szFileName, "r")))
	{
		fscanf(fp, "%d", &iObjNum);
		while (iObjNum--)
		{
			fscanf(fp, "%d%d", &AreaSize, &iDirNum);
			while (iDirNum--)
			{
				memset(Info, '\0', sizeof(Info));
				NewArea.clear();
				for (i=0 ; i<AreaSize ; i++)
				{
					fgets(Info[i], MAX_OBJ_SIZE, fp);
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
			NewObj.pTLC.iColumn	= 0;
			NewObj.pTLC.iLine	= 0;
			voList.push_back(NewObj);
		}
	}
	return	voList.size();
}