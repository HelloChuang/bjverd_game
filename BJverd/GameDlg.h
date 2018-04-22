#pragma once
#include "CGameLogic.h"



// CGameDlg �Ի���

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGameDlg();
	HICON m_hIcon;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);//��Ӧ�����������Ϣ

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	void InitWnd();
	void InitPicture();//��ʼ��ͼƬԪ��
	void DrawGameArea();
	void ChangeCartoon();//ʵ�ֶ�������Ч��

	void GetElemUxy(PICELEM picElem,int &nX,int &nY);//����ͼƬԪ�����Ͻ�����
	CRect GetElemRect(PICELEM picElem);//����ͼƬԪ�����ڵľ�������
	void DrawTipFrame(PICELEM picElem);//��ͼƬԪ�ػ�����ʾ��
	CDC m_dcMem;
	CDC m_dcBg;
	CGameLogic m_GameLogic;

	CDC m_dcElem;
	CDC m_dcMask;

	CRect m_rtGame; //��Ϸ��ͼ��С
	BOOL m_bFirstElem;//��ʶ�Ƿ�Ϊ��һ��ͼƬԪ��
	PICELEM m_picElemFirst;//��һ��ͼƬԪ��
	PICELEM m_picElemSecond;//�ڶ���ͼƬԪ��

public:
	
	
};
