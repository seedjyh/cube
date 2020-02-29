#include <string.h>
#include <stdio.h>
#include <windows.h>
VOID	Int2Str(int iInt, int iLen, TCHAR *szStr)
{
	szStr	+= iLen;
	(*szStr)	= '\0';
	szStr--;
	while (iLen--)
	{
		(*szStr)	= '0' + iInt%10;
		iInt/=10;
		szStr--;
	}
	return;
}