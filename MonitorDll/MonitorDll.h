// MonitorDll.h : MonitorDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMonitorDllApp
// �йش���ʵ�ֵ���Ϣ������� MonitorDll.cpp
//

class CMonitorDllApp : public CWinApp
{
public:
	CMonitorDllApp();

// ��д
public:
	virtual BOOL InitInstance();
	int ExitInstance();
	DECLARE_MESSAGE_MAP()
};
