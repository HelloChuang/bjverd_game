// GameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "BJverd.h"
#include "GameDlg.h"
#include "afxdialogex.h"
#include "Global.h"


// CGameDlg 对话框

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//初始化游戏区域矩形
	m_rtGame.top = GAME_Y;
	m_rtGame.left = GAME_X;
	m_rtGame.bottom = m_rtGame.top + PIC_HEIGHT*GAME_ROW;
	m_rtGame.right = m_rtGame.left + PIC_WIDTH*GAME_COL;

	m_bFirstElem = TRUE;
}

CGameDlg::~CGameDlg()
{
	m_dcMem.DeleteDC();
	m_dcBg.DeleteDC();

	m_dcElem.DeleteDC();
	m_dcMask.DeleteDC();
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
void CGameDlg::InitWnd()
{
	SetWindowPos(NULL, 0, 0, WIDTH, HEIGHT, SWP_NOMOVE);//设置游戏窗口大小
	CClientDC dc(this);
	CBitmap bmpBg;//定义背景位图对象
	bmpBg.LoadBitmapW(IDB_BITMAP1);//加载背景位图
	m_dcBg.CreateCompatibleDC(&dc);//与dc兼容
	m_dcBg.SelectObject(bmpBg);//将位图选入背景内存dc

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, WIDTH, HEIGHT);
	m_dcMem.CreateCompatibleDC(&dc);
	m_dcMem.SelectObject(bmpMem);
	m_dcMem.BitBlt(0, 0, WIDTH, HEIGHT, &m_dcBg, 0, 0, SRCCOPY);
}
void CGameDlg::InitPicture()
{
	CClientDC dc(this);
	//元素图片
	CBitmap bmpElem,bmpMask;
	bmpElem.LoadBitmap(IDB_BMP_ELEM);//加载位图文件
	m_dcElem.CreateCompatibleDC(&dc);//与视频DC兼容
	m_dcElem.SelectObject(bmpElem);//将位图对象选入位图DC
	//掩码图片
	bmpMask.LoadBitmap(IDB_BMP_MASK);//加载位图文件
	m_dcMask.CreateCompatibleDC(&dc);//与视频DC兼容
	m_dcMask.SelectObject(bmpMask);//将位图对象选入位图DC
}

void CGameDlg::DrawGameArea()
{
	//将m_dcMem游戏区域还原成背景图片
	m_dcMem.BitBlt(GAME_X,GAME_Y,PIC_WIDTH*GAME_COL,PIC_HEIGHT*GAME_ROW,&m_dcBg,GAME_X,GAME_Y,SRCCOPY);
	
	int nPicNum = INIT_PIC_NUM;
	int nWndX = 0;
	int nWndY = 0;

	int nPicX = 0;
	int nPicY = 0;
	for(int i=0;i<GAME_ROW;i++)
	{
		for(int j=0;j<GAME_COL;j++)
		{
			nPicNum = m_GameLogic.GetPicNum(i, j);
			nWndX = GAME_X + PIC_WIDTH * j;
			nWndY = GAME_Y + PIC_HEIGHT * i;

			nPicY = nPicNum*PIC_HEIGHT;
			//背景和掩码相或
			m_dcMem.BitBlt(nWndX, nWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcMask, nPicX, nPicY, SRCPAINT);
			//再与图片元素相与
			m_dcMem.BitBlt(nWndX, nWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcElem, nPicX, nPicY, SRCAND);

		}
	}

	InvalidateRect(&m_rtGame, FALSE);
}



BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CGameDlg 消息处理程序


BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	this->ModifyStyleEx(0, WS_EX_APPWINDOW);//添加底部任务栏
	InitPicture();
	InitWnd();//初始化游戏窗口
	m_GameLogic.BuildMap();
	DrawGameArea();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
	
}




void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	dc.BitBlt(0, 0, WIDTH, HEIGHT, &m_dcMem, 0, 0, SRCCOPY);
}


void CGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//判断点击的坐标是否超出游戏矩形区域
	if(!m_rtGame.PtInRect(point))
	{
		return CDialogEx::OnLButtonDown(nFlags, point);
	}
	//计算点击图片的行和列
	int nRow = (point.y - GAME_Y) / PIC_HEIGHT;  
	int nCol = (point.x - GAME_X) / PIC_WIDTH;
	//获取元素图片
	PICELEM picElem = m_GameLogic.GetPicElem(nRow, nCol);
	//绘制矩形提示框
	DrawTipFrame(picElem);
	if(m_bFirstElem)//值为TRUE，表示是第一张图片
	{
		m_picElemFirst = picElem;
	}
	else//值为FALSE，第二张图片
	{
		m_picElemSecond = picElem;
		//判断是否相邻
		if (m_GameLogic.IsNeighbour(m_picElemFirst, m_picElemSecond))
		{
			//互换操作
			ChangeCartoon();
			m_GameLogic.ExchangeMap(m_picElemFirst,m_picElemSecond);
			//DrawGameArea();
			if(m_GameLogic.IsExchange(m_picElemFirst,m_picElemSecond))
			{
				
			}
			else
			{
				ChangeCartoon();
				m_GameLogic.ExchangeMap(m_picElemFirst, m_picElemSecond);
			}
		}
		else
		{
			//去掉提示框
			CRect rect = GetElemRect(m_picElemFirst) + GetElemRect(m_picElemSecond);
			InvalidateRect(&rect, false);
		}
	}
	m_bFirstElem = !m_bFirstElem;//改变标识量的值
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CGameDlg::GetElemUxy(PICELEM picElem, int &nX, int &nY)
{
	nX = GAME_X + picElem.ncol*PIC_WIDTH;
	nY = GAME_Y + picElem.nrow*PIC_HEIGHT;
}
CRect CGameDlg::GetElemRect(PICELEM picElem)
{
	//获取图片在屏幕中左上角的坐标
	int nX = 0;
	int nY = 0;
	GetElemUxy(picElem, nX, nY);
	//定义元素图片矩形
	CRect rect;
	rect.top = nY;
	rect.left = nX;
	rect.right = rect.left + PIC_WIDTH;
	rect.bottom = rect.top + PIC_HEIGHT;
	//返回矩形
	return rect;
}
void CGameDlg::DrawTipFrame(PICELEM picElem)
{
	CClientDC dc(this);
	//创建画刷
	CBrush brush(RGB(233,43,43));
	//设置提示框矩形
	CRect rtTipFrame = GetElemRect(picElem);
	//绘制矩形框
	dc.FrameRect(rtTipFrame,&brush);
}



void CGameDlg::ChangeCartoon()
{
	int nFlag = 0;//定义一个标志
	PICELEM elemLU = m_picElemFirst;//定义左上的图片元素
	PICELEM elemRD = m_picElemSecond;//定义右下的图片元素
	if (m_picElemFirst.nrow == m_picElemSecond.nrow)//如果行相等，就是横向交换
	{
		nFlag = 1;//标识是横向

		if (m_picElemFirst.ncol > m_picElemSecond.ncol)//第一张图是右边的图片
		{
			elemRD = m_picElemFirst;//将第一张图标记为右下图
			elemLU = m_picElemSecond;
		}
	}
	if(m_picElemFirst.ncol == m_picElemSecond.ncol)//如果是列相等，就是纵向交换
	{
		nFlag = 2;//标识是纵向
		if(m_picElemFirst.nrow>m_picElemSecond.nrow)//判断第一张是否是下面的图片
		{
			elemRD = m_picElemFirst;
			elemLU = m_picElemSecond;
		}
		
		//计算左上宝石的目的坐标
		int nLUWndX = 0;
		int nLUWndY = 0;
		GetElemUxy(elemLU, nLUWndX, nLUWndY);
		//计算左上宝石的原坐标
		int nLUPicX = 0;
		int nLUPicY = elemLU.nPicNum * PIC_HEIGHT;
		//获取右下宝石的目的坐标和原坐标
		int nRDWndX = 0;
		int nRDWndY = 0;
		GetElemUxy(elemRD,nRDWndX,nRDWndY);
		//计算右下宝石的原坐标
		int nRDPicX = 0;
		int nRDPicY = elemRD.nPicNum*PIC_HEIGHT;

		int nNum = 0;//偏移次数
		while(nNum<5)//图片宽是50，每次移动10，移动5次
		{
			nNum++;
			if(nFlag == 1)//横向
			{
				//把需要交换的两个宝石位置的背景拷贝
				m_dcMem.BitBlt(nLUWndX, nLUWndY, PIC_WIDTH * 2, PIC_HEIGHT, &m_dcBg, nLUWndX, nLUWndY, SRCCOPY);
				m_dcMem.BitBlt((nLUWndX + nNum*MOVE_PIXEL), nLUWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcMask, nLUPicX, nLUPicY, SRCPAINT);
				m_dcMem.BitBlt((nLUWndX + nNum*MOVE_PIXEL), nLUWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcElem, nLUPicX, nLUPicY, SRCAND);

				m_dcMem.BitBlt((nRDWndX - nNum*MOVE_PIXEL), nRDWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcMask, nRDPicX, nRDPicY, SRCPAINT);
				m_dcMem.BitBlt((nRDWndX - nNum*MOVE_PIXEL), nRDWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcElem, nRDPicX, nRDPicY, SRCAND);
			}
			else//纵向
			{
				m_dcMem.BitBlt(nLUWndX, nLUWndY, PIC_WIDTH , PIC_HEIGHT*2, &m_dcBg, nLUWndX, nLUWndY, SRCCOPY);
				m_dcMem.BitBlt(nLUWndX, (nLUWndY + nNum*MOVE_PIXEL), PIC_WIDTH, PIC_HEIGHT, &m_dcMask, nLUPicX, nLUPicY, SRCPAINT);
				m_dcMem.BitBlt(nLUWndX, (nLUWndY + nNum*MOVE_PIXEL), PIC_WIDTH, PIC_HEIGHT, &m_dcElem, nLUPicX, nLUPicY, SRCAND);

				m_dcMem.BitBlt(nLUWndX, (nLUWndY - nNum*MOVE_PIXEL), PIC_WIDTH, PIC_HEIGHT, &m_dcMask, nRDPicX, nRDPicY, SRCPAINT);
				m_dcMem.BitBlt(nLUWndX, (nLUWndY - nNum*MOVE_PIXEL), PIC_WIDTH, PIC_HEIGHT, &m_dcElem, nRDPicX, nRDPicY, SRCAND);
			}
			CRect rect = GetElemRect(elemLU) + GetElemRect(elemRD);
			InvalidateRect(&rect, FALSE);
			UpdateWindow();
			Sleep(50);
		}
	
	}
}