// MyTab.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "MyTab.h"


// CMyTab

IMPLEMENT_DYNAMIC(CMyTab, CTabCtrl)

CMyTab::CMyTab()
{

}

CMyTab::~CMyTab()
{
}


BEGIN_MESSAGE_MAP(CMyTab, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTab::OnTcnSelchange)
END_MESSAGE_MAP()



// CMyTab ��Ϣ�������




void CMyTab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	switch (GetCurSel())
	{
	case 0:
	{
		m_Dia[0]->ShowWindow(SW_SHOW);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_HIDE);
		m_Dia[3]->ShowWindow(SW_HIDE);
		m_Dia[4]->ShowWindow(SW_HIDE);
		m_Dia[5]->ShowWindow(SW_HIDE);
	}
	break;
	case 1:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_SHOW);
		m_Dia[2]->ShowWindow(SW_HIDE);
		m_Dia[3]->ShowWindow(SW_HIDE);
		m_Dia[4]->ShowWindow(SW_HIDE);
		m_Dia[5]->ShowWindow(SW_HIDE);
	}
	break;
	case 2:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_SHOW);
		m_Dia[3]->ShowWindow(SW_HIDE);
		m_Dia[4]->ShowWindow(SW_HIDE);
		m_Dia[5]->ShowWindow(SW_HIDE);

	}
	break;
	case 3:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_HIDE);
		m_Dia[3]->ShowWindow(SW_SHOW);
		m_Dia[4]->ShowWindow(SW_HIDE);
		m_Dia[5]->ShowWindow(SW_HIDE);

	}
	break;
	case 4:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_HIDE);
		m_Dia[3]->ShowWindow(SW_HIDE);
		m_Dia[4]->ShowWindow(SW_SHOW);
		m_Dia[5]->ShowWindow(SW_HIDE);

	}
	break;
	case 5:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_HIDE);
		m_Dia[3]->ShowWindow(SW_HIDE);
		m_Dia[4]->ShowWindow(SW_HIDE);
		m_Dia[5]->ShowWindow(SW_SHOW);

	}
	break;
	default:
		break;
	}
}
