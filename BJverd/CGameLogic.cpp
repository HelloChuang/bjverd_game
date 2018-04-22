#include "stdafx.h"
#include "CGameLogic.h"
#include "Global.h"


CGameLogic::CGameLogic()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			m_aMap[i][j] = -1;

	srand((unsigned)time(NULL));
}


CGameLogic::~CGameLogic()
{
}
void CGameLogic::BuildMap()
{
	int aData[GAME_ROW][GAME_COL] = {
	0,1,1,0,0,0,1,0,
	1,0,0,1,0,0,1,0,
	0,1,1,1,0,0,1,0,
	0,1,1,0,1,0,1,0,
	1,1,1,0,1,0,1,0,
	1,1,1,0,0,0,1,0,
	1,1,1,0,1,0,1,0,
	0,1,1,0,0,0,1,1};

	for (int i = 0; i < GAME_ROW; i++)
		for (int j = 0; j < GAME_COL; j++)
			m_aMap[i][j] = NewPicNum();
}
int CGameLogic::GetPicNum(int nRow, int nCol)
{
	return m_aMap[nRow][nCol];
}
int CGameLogic::NewPicNum()
{
	return rand() % ELEM_NUM;
}
PICELEM CGameLogic::GetPicElem(int nRow, int nCol)
{
	PICELEM picElem;
	picElem.nrow = nRow;
	picElem.ncol = nCol;
	picElem.nPicNum = GetPicNum(nRow, nCol);

	return picElem;
}
void CGameLogic::ExchangeMap(PICELEM &picElem1, PICELEM &picELEM2)
{
	//将两张图片元素结构体的图片编号交换
	int nTemp = picElem1.nPicNum;
	picElem1.nPicNum = picELEM2.nPicNum;
	picELEM2.nPicNum = nTemp;
	//再将数组中的值更新为交换后的图片编号
	m_aMap[picElem1.nrow][picElem1.ncol] = picElem1.nPicNum;
	m_aMap[picELEM2.nrow][picELEM2.ncol] = picELEM2.nPicNum;
}
BOOL CGameLogic::IsNeighbour(PICELEM picElem1, PICELEM picElem2)
{
	//行相等，列相减的绝对值为1。左右关系
	if(picElem1.nrow==picElem2.nrow&&abs(picElem1.ncol-picElem2.ncol)==1)
	{
		return TRUE;
	}
	//列相等，行相减的绝对值为1.上下关系
	if(picElem1.ncol == picElem2.ncol&&abs(picElem1.nrow - picElem2.nrow)==1)
	{
		return TRUE;
	}
	return FALSE;
}
int CGameLogic::RowGroupNum(PICELEM picElem)
{
	int nFlag = 1;//记录一组的个数
	//如果宝石和左边第一个相同
	if (picElem.ncol>0&&picElem.nPicNum==GetPicNum(picElem.nrow,picElem.ncol-1))
	{
		nFlag++;
		if(picElem.ncol>1&&picElem.nPicNum== GetPicNum(picElem.nrow, picElem.ncol - 2))//左边第二个
		{
			nFlag++;
		}
	}
	if (picElem.ncol < GAME_COL - 1 && picElem.nPicNum == GetPicNum(picElem.nrow, picElem.ncol + 1)) {//右边第一个
		nFlag++;
		if(picElem.ncol < GAME_COL - 2 && picElem.nPicNum == GetPicNum(picElem.nrow, picElem.ncol + 2))//右边第二个
		{
			nFlag++;
		}
	}
	return nFlag;
}
int CGameLogic::ColGroupNum(PICELEM picElem)
{
	int nFlag = 1;//记录一组的个数
				  //如果宝石和上边第一个相同
	if (picElem.nrow>0 && picElem.nPicNum == GetPicNum(picElem.nrow-1, picElem.ncol))
	{
		nFlag++;
		if (picElem.nrow>1 && picElem.nPicNum == GetPicNum(picElem.nrow-2, picElem.ncol))//上边第二个
		{
			nFlag++;
		}
	}
	if (picElem.nrow < GAME_ROW - 1 && picElem.nPicNum == GetPicNum(picElem.nrow+1, picElem.ncol)) {//上边第一个
		nFlag++;
		if (picElem.nrow < GAME_ROW - 2 && picElem.nPicNum == GetPicNum(picElem.nrow+2, picElem.ncol))//上边第二个
		{
			nFlag++;
		}
	}
	return nFlag;
}

BOOL CGameLogic::IsElemRemove(PICELEM picElem)
{
	int nRowNum = RowGroupNum(picElem);
	int nColNum = ColGroupNum(picElem);

	if(nRowNum>=3||nColNum>=3)
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CGameLogic::IsExchange(PICELEM picElem1, PICELEM picElem2)
{
	if(IsElemRemove(picElem1)||IsElemRemove(picElem2))
	{
		return TRUE;
	}
	return FALSE;
}





