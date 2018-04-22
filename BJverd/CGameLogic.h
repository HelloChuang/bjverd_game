#pragma once
#include <stdlib.h>
#include <time.h>
#include "Global.h"

class CGameLogic
{
public:
	CGameLogic();
	~CGameLogic();
	void BuildMap();
	int GetPicNum(int nRow,int nCol);
	PICELEM GetPicElem(int nRow, int nCol);//根据行列获得当前图片元素
	BOOL IsNeighbour(PICELEM picElem1,PICELEM picElem2);//判断这两张图片是不是相邻的
	void ExchangeMap(PICELEM &picElem1,PICELEM &picELEM2);
	BOOL IsExchange(PICELEM picElem1, PICELEM picElem2);
protected:
	int m_aMap[8][8];
	int NewPicNum();
	int RowGroupNum(PICELEM picElem);
	int ColGroupNum(PICELEM picElem);
	BOOL IsElemRemove(PICELEM picElem);
	

	
};

