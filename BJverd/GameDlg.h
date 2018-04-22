#pragma once
#include "CGameLogic.h"



// CGameDlg 对话框

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGameDlg();
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//响应鼠标左键点击消息

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	void InitWnd();
	void InitPicture();//初始化图片元素
	void DrawGameArea();
	void ChangeCartoon();//实现动画交换效果

	void GetElemUxy(PICELEM picElem,int &nX,int &nY);//计算图片元素左上角坐标
	CRect GetElemRect(PICELEM picElem);//计算图片元素所在的矩形区域
	void DrawTipFrame(PICELEM picElem);//给图片元素绘制提示框
	CDC m_dcMem;
	CDC m_dcBg;
	CGameLogic m_GameLogic;

	CDC m_dcElem;
	CDC m_dcMask;

	CRect m_rtGame; //游戏地图大小
	BOOL m_bFirstElem;//标识是否为第一个图片元素
	PICELEM m_picElemFirst;//第一张图片元素
	PICELEM m_picElemSecond;//第二章图片元素

public:
	
	
};
