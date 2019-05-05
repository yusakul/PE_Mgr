// Dia_Mod.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Mod.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// Dia_Mod �Ի���

IMPLEMENT_DYNAMIC(Dia_Mod, CDialogEx)

Dia_Mod::Dia_Mod(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MODULE, pParent)
{

}

Dia_Mod::~Dia_Mod()
{
}

void Dia_Mod::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Mod, m_List_Mod);
}


BEGIN_MESSAGE_MAP(Dia_Mod, CDialogEx)
END_MESSAGE_MAP()


// Dia_Mod ��Ϣ�������

//ģ���б�
BOOL Dia_Mod::ListProcessModules(DWORD dwPID)
{
	m_List_Mod.DeleteAllItems();

	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;
	//  Take a snapshot of all modules in the specified process. 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		/*MessageBox(NULL, L"CreateToolhelp32Snapshot(of modules)",NULL);*/
		win32pError();
		//CloseHandle(hModuleSnap);     // Must clean up the snapshot object! //
		DestroyWindow();
		return(FALSE);
	}

	//  Set the size of the structure before using it. 
	me32.dwSize = sizeof(MODULEENTRY32);

	//  Retrieve information about the first module, 
	//  and exit if unsuccessful 
	if (!Module32First(hModuleSnap, &me32))
	{
		/*		printError(TEXT("Module32First"));  // Show cause of failure */
		win32pError();
		//CloseHandle(hModuleSnap);     // Must clean up the snapshot object! 
		DestroyWindow();
		return(FALSE);
	}

	//  Now walk the module list of the process, 
	//  and display information about each module 
	do
	{
		int i = 0;
		TCHAR *Temp = new TCHAR[MAX_PATH];

		//ģ����
		m_List_Mod.InsertItem(i, me32.szModule);

		//ģ��·��
		m_List_Mod.SetItemText(i, 1, me32.szExePath);

		//����ַ
		wsprintf(Temp, L"%08X", me32.modBaseAddr);
		m_List_Mod.SetItemText(i, 2, Temp);

		//ģ���С
		wsprintf(Temp, L"%d", me32.modBaseSize / 1024);
		wcscat_s(Temp, MAX_PATH, L"K");
		m_List_Mod.SetItemText(i, 3, Temp);

		i++;
	} while (Module32Next(hModuleSnap, &me32));


	//  Do not forget to clean up the snapshot object. 
	CloseHandle(hModuleSnap);
	return(TRUE);
}


BOOL Dia_Mod::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetTimer(IDC_LIST_Details, 100000, NULL);

	m_List_Mod.SetExtendedStyle(m_List_Mod.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	m_List_Mod.InsertColumn(0, _T("ģ����"), 0, 200);
	m_List_Mod.InsertColumn(1, _T("ģ��·��"), 0, 300);
	m_List_Mod.InsertColumn(2, _T("����ַ"), 0, 100);
	m_List_Mod.InsertColumn(3, _T("ռ���ڴ�"), 0, 100);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void Dia_Mod::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow();
	CDialogEx::OnClose();
}
