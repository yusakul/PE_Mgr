// TabCtrl_TOOLS.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "TabCtrl_TOOLS.h"


// CTabCtrl_TOOLS

IMPLEMENT_DYNAMIC(CTabCtrl_TOOLS, CTabCtrl)

CTabCtrl_TOOLS::CTabCtrl_TOOLS()
{

}

CTabCtrl_TOOLS::~CTabCtrl_TOOLS()
{
}


BEGIN_MESSAGE_MAP(CTabCtrl_TOOLS, CTabCtrl)
	//ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TOOLS, &CTabCtrl_TOOLS::OnTcnSelchangeTabTools)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CTabCtrl_TOOLS::OnTcnSelchangeTabTools)
END_MESSAGE_MAP()



// CTabCtrl_TOOLS ��Ϣ�������




void CTabCtrl_TOOLS::OnTcnSelchangeTabTools(NMHDR *pNMHDR, LRESULT *pResult)
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


	}
	break;
	case 1:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_SHOW);
		m_Dia[2]->ShowWindow(SW_HIDE);


	}
	break;
	case 2:
	{
		m_Dia[0]->ShowWindow(SW_HIDE);
		m_Dia[1]->ShowWindow(SW_HIDE);
		m_Dia[2]->ShowWindow(SW_SHOW);


	}
	break;
	default:
		break;
	}
}
