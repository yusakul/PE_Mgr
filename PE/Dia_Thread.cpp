// Dia_Thread.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Thread.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// Dia_Thread 对话框

IMPLEMENT_DYNAMIC(Dia_Thread, CDialogEx)

Dia_Thread::Dia_Thread(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_THREAD, pParent)
{

}

Dia_Thread::~Dia_Thread()
{
}

void Dia_Thread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Thread, m_List_Thread);
}


BEGIN_MESSAGE_MAP(Dia_Thread, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_Thread, &Dia_Thread::OnRclickListThread)
END_MESSAGE_MAP()


// Dia_Thread 消息处理程序


BOOL Dia_Thread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	UpdataList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void Dia_Thread::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	CDialogEx::OnClose();
}

//更新列表
void Dia_Thread::UpdataList()
{
	m_List_Thread.DeleteAllItems();
	m_List_Thread.SetExtendedStyle(m_List_Thread.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	m_List_Thread.InsertColumn(0, _T("线程ID"), 0, 200);
	m_List_Thread.InsertColumn(1, _T("优先级"), 0, 100);
	m_List_Thread.InsertColumn(2, _T("拥有者(线程)ID"), 0, 200);
	m_List_Thread.InsertColumn(3, _T("占用内存"), 0, 100);
}


BOOL Dia_Thread::ListProcessThreads(DWORD dwOwnerPID)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		win32pError();  // Show cause of failure
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return(FALSE);
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do
	{
		int i = 0;
		TCHAR *Temp = new TCHAR[MAX_PATH];

		if (te32.th32OwnerProcessID == dwOwnerPID)
		{

			//线程ID
			wsprintf(Temp, L"%d", te32.th32ThreadID);
			m_List_Thread.InsertItem(i, Temp);

			//线程优先级
			wsprintf(Temp, L"%d", te32.tpBasePri);
			m_List_Thread.SetItemText(i, 1, Temp);

			//拥有者线程ID
			wsprintf(Temp, L"%d", te32.th32OwnerProcessID);
			m_List_Thread.SetItemText(i, 2, Temp);

			//占用内存
			// 			wsprintf(Temp, L"%d", te32.dwSize);
			// 			m_List_Thread.SetItemText(i, 3, Temp);

			i++;
		}

	} while (Thread32Next(hThreadSnap, &te32));

	CloseHandle(hThreadSnap);
	return(TRUE);
}


void Dia_Thread::OnRclickListThread(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU_ThreadRCList);
	pmenu = menu.GetSubMenu(0);//指定包含菜单中的弹出菜单的位置。 位置值从0开始第一个菜单项。 
	ClientToScreen(&pNMItemActivate->ptAction);//在客户端坐标中指示发生事件的位置
	UINT Select = pmenu->TrackPopupMenu(
		TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);

	//获取进程ID
	DWORD dwPID = 0;
	WCHAR wcProcsessID[MAX_PATH] = {};
	if (pNMItemActivate->iItem != -1)//如果GetItemText项目索引未用于通知，则此成员将包含-1。
	{
		m_List_Thread.GetItemText(pNMItemActivate->iItem, 2, wcProcsessID, MAX_PATH);//GetItemText:1.索引（这里是列）， 2.检索对象（这里是第二列）， 3， 接收文本的指针， 4， 缓冲区长度；返回检索字符串的长度(int)
	}
	dwPID = _wtoi(wcProcsessID);
	//获取进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	//获取ThreadID
	DWORD dwTID = 0;
	WCHAR wcThreadID[MAX_PATH] = {};
	if (pNMItemActivate->iItem != -1)//如果GetItemText项目索引未用于通知，则此成员将包含-1。
	{
		m_List_Thread.GetItemText(pNMItemActivate->iItem, 0, wcThreadID, MAX_PATH);//GetItemText:1.索引， 2.检索对象， 3， 接收文本的指针， 4， 缓冲区长度；返回检索字符串的长度(int)
	}
	dwTID = _wtoi(wcThreadID);
	//获取线程句柄
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTID);

	//	//获取Thread句柄
	//	HANDLE  hThread;
	//	DuplicateHandle(GetCurrentProcess(), //当前进程句柄
	//		GetCurrentThread(),				 //当前线程句柄（得到的是伪句柄）
	//		GetCurrentProcess(),			 //目标进程（这里是自己）
	//		&hThread,						//要存到的目标线程句柄地址
	//		0,								//进程的操作权限，如果要用到则要填PROCESS_ALL_ACCESS或PROCESS_DUP_HANDLE
	//		FALSE,							//是否继承
	//		DUPLICATE_SAME_ACCESS);			//DUPLICATE_SAME_ACCESS 新句柄拥有与原始句柄相同的安全访问特征;DUPLICATE_CLOSE_SOURCE 原始句柄已经关闭。即使发生错误。它也要关闭

	switch (Select)
	{
	case ID_MOUSERCLICK_32771:	//挂起线程
	{
		SuspendThread(hThread);
		//CloseHandle(hThread); 
		UpdataList();
		ListProcessThreads(dwPID);
	}
	break;
	case ID_MOUSERCLICK_32773:	//恢复线程
	{
		ResumeThread(hThread);
		UpdataList();
		ListProcessThreads(dwPID);
	}
	break;
	case ID_MOUSERCLICK_32772:	//结束线程
	{
		TerminateThread(hThread, 0);
		//CloseHandle(hThread);
		UpdataList();
		ListProcessThreads(dwPID);
	}
	break;
	default:
		break;
	}
}
