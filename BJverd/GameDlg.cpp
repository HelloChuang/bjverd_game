// GameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BJverd.h"
#include "GameDlg.h"
#include "afxdialogex.h"
#include "Global.h"


// CGameDlg �Ի���

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//��ʼ����Ϸ�������
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
	SetWindowPos(NULL, 0, 0, WIDTH, HEIGHT, SWP_NOMOVE);//������Ϸ���ڴ�С
	CClientDC dc(this);
	CBitmap bmpBg;//���屳��λͼ����
	bmpBg.LoadBitmapW(IDB_BITMAP1);//���ر���λͼ
	m_dcBg.CreateCompatibleDC(&dc);//��dc����
	m_dcBg.SelectObject(bmpBg);//��λͼѡ�뱳���ڴ�dc

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, WIDTH, HEIGHT);
	m_dcMem.CreateCompatibleDC(&dc);
	m_dcMem.SelectObject(bmpMem);
	m_dcMem.BitBlt(0, 0, WIDTH, HEIGHT, &m_dcBg, 0, 0, SRCCOPY);
}
void CGameDlg::InitPicture()
{
	CClientDC dc(this);
	//Ԫ��ͼƬ
	CBitmap bmpElem,bmpMask;
	bmpElem.LoadBitmap(IDB_BMP_ELEM);//����λͼ�ļ�
	m_dcElem.CreateCompatibleDC(&dc);//����ƵDC����
	m_dcElem.SelectObject(bmpElem);//��λͼ����ѡ��λͼDC
	//����ͼƬ
	bmpMask.LoadBitmap(IDB_BMP_MASK);//����λͼ�ļ�
	m_dcMask.CreateCompatibleDC(&dc);//����ƵDC����
	m_dcMask.SelectObject(bmpMask);//��λͼ����ѡ��λͼDC
}

void CGameDlg::DrawGameArea()
{
	//��m_dcMem��Ϸ����ԭ�ɱ���ͼƬ
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
			//�������������
			m_dcMem.BitBlt(nWndX, nWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcMask, nPicX, nPicY, SRCPAINT);
			//����ͼƬԪ������
			m_dcMem.BitBlt(nWndX, nWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcElem, nPicX, nPicY, SRCAND);

		}
	}

	InvalidateRect(&m_rtGame, FALSE);
}



BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CGameDlg ��Ϣ�������


BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	this->ModifyStyleEx(0, WS_EX_APPWINDOW);//��ӵײ�������
	InitPicture();
	InitWnd();//��ʼ����Ϸ����
	m_GameLogic.BuildMap();
	DrawGameArea();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
	
}




void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	dc.BitBlt(0, 0, WIDTH, HEIGHT, &m_dcMem, 0, 0, SRCCOPY);
}


void CGameDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	//�жϵ���������Ƿ񳬳���Ϸ��������
	if(!m_rtGame.PtInRect(point))
	{
		return CDialogEx::OnLButtonDown(nFlags, point);
	}
	//������ͼƬ���к���
	int nRow = (point.y - GAME_Y) / PIC_HEIGHT;  
	int nCol = (point.x - GAME_X) / PIC_WIDTH;
	//��ȡԪ��ͼƬ
	PICELEM picElem = m_GameLogic.GetPicElem(nRow, nCol);
	//���ƾ�����ʾ��
	DrawTipFrame(picElem);
	if(m_bFirstElem)//ֵΪTRUE����ʾ�ǵ�һ��ͼƬ
	{
		m_picElemFirst = picElem;
	}
	else//ֵΪFALSE���ڶ���ͼƬ
	{
		m_picElemSecond = picElem;
		//�ж��Ƿ�����
		if (m_GameLogic.IsNeighbour(m_picElemFirst, m_picElemSecond))
		{
			//��������
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
			//ȥ����ʾ��
			CRect rect = GetElemRect(m_picElemFirst) + GetElemRect(m_picElemSecond);
			InvalidateRect(&rect, false);
		}
	}
	m_bFirstElem = !m_bFirstElem;//�ı��ʶ����ֵ
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CGameDlg::GetElemUxy(PICELEM picElem, int &nX, int &nY)
{
	nX = GAME_X + picElem.ncol*PIC_WIDTH;
	nY = GAME_Y + picElem.nrow*PIC_HEIGHT;
}
CRect CGameDlg::GetElemRect(PICELEM picElem)
{
	//��ȡͼƬ����Ļ�����Ͻǵ�����
	int nX = 0;
	int nY = 0;
	GetElemUxy(picElem, nX, nY);
	//����Ԫ��ͼƬ����
	CRect rect;
	rect.top = nY;
	rect.left = nX;
	rect.right = rect.left + PIC_WIDTH;
	rect.bottom = rect.top + PIC_HEIGHT;
	//���ؾ���
	return rect;
}
void CGameDlg::DrawTipFrame(PICELEM picElem)
{
	CClientDC dc(this);
	//������ˢ
	CBrush brush(RGB(233,43,43));
	//������ʾ�����
	CRect rtTipFrame = GetElemRect(picElem);
	//���ƾ��ο�
	dc.FrameRect(rtTipFrame,&brush);
}



void CGameDlg::ChangeCartoon()
{
	int nFlag = 0;//����һ����־
	PICELEM elemLU = m_picElemFirst;//�������ϵ�ͼƬԪ��
	PICELEM elemRD = m_picElemSecond;//�������µ�ͼƬԪ��
	if (m_picElemFirst.nrow == m_picElemSecond.nrow)//�������ȣ����Ǻ��򽻻�
	{
		nFlag = 1;//��ʶ�Ǻ���

		if (m_picElemFirst.ncol > m_picElemSecond.ncol)//��һ��ͼ���ұߵ�ͼƬ
		{
			elemRD = m_picElemFirst;//����һ��ͼ���Ϊ����ͼ
			elemLU = m_picElemSecond;
		}
	}
	if(m_picElemFirst.ncol == m_picElemSecond.ncol)//���������ȣ��������򽻻�
	{
		nFlag = 2;//��ʶ������
		if(m_picElemFirst.nrow>m_picElemSecond.nrow)//�жϵ�һ���Ƿ��������ͼƬ
		{
			elemRD = m_picElemFirst;
			elemLU = m_picElemSecond;
		}
		
		//�������ϱ�ʯ��Ŀ������
		int nLUWndX = 0;
		int nLUWndY = 0;
		GetElemUxy(elemLU, nLUWndX, nLUWndY);
		//�������ϱ�ʯ��ԭ����
		int nLUPicX = 0;
		int nLUPicY = elemLU.nPicNum * PIC_HEIGHT;
		//��ȡ���±�ʯ��Ŀ�������ԭ����
		int nRDWndX = 0;
		int nRDWndY = 0;
		GetElemUxy(elemRD,nRDWndX,nRDWndY);
		//�������±�ʯ��ԭ����
		int nRDPicX = 0;
		int nRDPicY = elemRD.nPicNum*PIC_HEIGHT;

		int nNum = 0;//ƫ�ƴ���
		while(nNum<5)//ͼƬ����50��ÿ���ƶ�10���ƶ�5��
		{
			nNum++;
			if(nFlag == 1)//����
			{
				//����Ҫ������������ʯλ�õı�������
				m_dcMem.BitBlt(nLUWndX, nLUWndY, PIC_WIDTH * 2, PIC_HEIGHT, &m_dcBg, nLUWndX, nLUWndY, SRCCOPY);
				m_dcMem.BitBlt((nLUWndX + nNum*MOVE_PIXEL), nLUWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcMask, nLUPicX, nLUPicY, SRCPAINT);
				m_dcMem.BitBlt((nLUWndX + nNum*MOVE_PIXEL), nLUWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcElem, nLUPicX, nLUPicY, SRCAND);

				m_dcMem.BitBlt((nRDWndX - nNum*MOVE_PIXEL), nRDWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcMask, nRDPicX, nRDPicY, SRCPAINT);
				m_dcMem.BitBlt((nRDWndX - nNum*MOVE_PIXEL), nRDWndY, PIC_WIDTH, PIC_HEIGHT, &m_dcElem, nRDPicX, nRDPicY, SRCAND);
			}
			else//����
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