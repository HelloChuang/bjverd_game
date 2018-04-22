#pragma once


typedef struct PICTUREELEMENT
{
	int ncol;
	int nrow;
	int nPicNum;
}PICELEM;
#define GAME_ROW 8
#define GAME_COL 8

#define INIT_PIC_NUM -1
//屏幕宽高
#define WIDTH 700
#define  HEIGHT 500
//游戏地图的起始坐标
#define  GAME_X 20
#define  GAME_Y 20
//图片元素宽高
#define  PIC_WIDTH 50
#define  PIC_HEIGHT 50

#define ELEM_NUM 7
#define MOVE_PIXEL 10