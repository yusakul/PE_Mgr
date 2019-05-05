// Dia_Details.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Details.h"
#include "afxdialogex.h"
#include <string>



// CDia_Details 对话框

IMPLEMENT_DYNAMIC(CDia_Details, CDialogEx)

CDia_Details::CDia_Details(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DETAILS, pParent)
{

}

CDia_Details::~CDia_Details()
{
}

void CDia_Details::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Details, m_ListCtrl_De);

}


BEGIN_MESSAGE_MAP(CDia_Details, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WinProc, &CDia_Details::OnLvnItemchangedListWinproc)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Details, &CDia_Details::OnDblclkListDetails)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_Details, &CDia_Details::OnRclickListDetails)
END_MESSAGE_MAP()


// CDia_Details 消息处理程序


void CDia_Details::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


//更新列表
void CDia_Details::UpdataList()
{
	m_ListCtrl_De.DeleteAllItems();

	typedef long(_stdcall*FUN)
		(
			UINT SystemInformationClass,
			PVOID SystemInformation,
			ULONG SystemInformationLength,
			PULONG ReturnLength
			);
	FUN fun;
	HMODULE  hNtDll = LoadLibrary(L"Ntdll.dll");
	if (!hNtDll)
	{
		win32pError();
		return;
	}
	fun = (FUN)GetProcAddress(hNtDll, "NtQuerySystemInformation");
	ULONG dwNeedSize = 0;
	int status = fun(5, NULL, 0, &dwNeedSize);
	BYTE* buf = new BYTE[dwNeedSize];

	PSYSTEM_PROCESS_INFORMATION pSpi = (PSYSTEM_PROCESS_INFORMATION)buf;
	status = fun(5, (PVOID)buf, dwNeedSize, NULL);

	for (int i = 0; pSpi->NextEntryDelta != 0; i++)
	{
		if (pSpi->ProcessName.Buffer == NULL)
		{
			i--;
			pSpi = (PSYSTEM_PROCESS_INFORMATION)
				(((PUCHAR)pSpi) + pSpi->NextEntryDelta);
			continue;
		}

		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pSpi->ProcessId);

		BOOL Wow64Process = FALSE;
		IsWow64Process(handle, &Wow64Process);//Wow64Process [out] 如果进程在WOW64下运行，则指向设置为TRUE的值的指针。如果进程在32位Windows下运行，则该值设置为FALSE。如果该进程是在64位Windows下运行的64位应用程序，则该值也设置为FALSE。

		TCHAR temp[MAX_PATH] = {};
		//显示进程名
		wcscpy_s(temp, pSpi->ProcessName.Length, pSpi->ProcessName.Buffer);
		//显示32位
		if (Wow64Process == TRUE)
		{
			wcscat_s(temp, MAX_PATH, L"（32位）");
		}
		m_ListCtrl_De.InsertItem(i, temp);

		//显示进程PID
		_itow_s(pSpi->ProcessId, temp, 10);//value，进制，必须在范围 2-36。
		m_ListCtrl_De.SetItemText(i, 1, temp);

		//线程数
		_itow_s(pSpi->ThreadCount, temp, 10);
		m_ListCtrl_De.SetItemText(i, 2, temp);

		//进程占用内存
		_itow_s(pSpi->VmCounters.WorkingSetSize / 1024, temp, 10);
		wcscat_s(temp, MAX_PATH, L"K");
		m_ListCtrl_De.SetItemText(i, 3, temp);

		pSpi = (PSYSTEM_PROCESS_INFORMATION)
			(((PUCHAR)pSpi) + pSpi->NextEntryDelta);
	}
	delete buf;
}

//初始化
BOOL CDia_Details::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//设置定时时间
	SetTimer(IDC_LIST_Details, 20000, NULL);

	m_ListCtrl_De.SetExtendedStyle(m_ListCtrl_De.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_DOUBLEBUFFER);
	m_ListCtrl_De.InsertColumn(0, _T("进程"), 0, 200);
	m_ListCtrl_De.InsertColumn(1, _T("PID"), 0, 200);
	m_ListCtrl_De.InsertColumn(2, _T("总线程数"), 0, 200);
	m_ListCtrl_De.InsertColumn(3, _T("占用内存"), 0, 200);
	UpdataList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//定时
void CDia_Details::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdataList();
	CDialogEx::OnTimer(nIDEvent);
}


void CDia_Details::OnLvnItemchangedListWinproc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


//列表双击打开模块列表窗口
void CDia_Details::OnDblclkListDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	std::wstring strPID;

	if (-1 != pNMItemActivate->iItem)
	{
		strPID = m_ListCtrl_De.GetItemText(pNMItemActivate->iItem, 1);
	}
	DWORD dwPID = _wtoi(strPID.c_str());
	if (m_mod.Create(IDD_DIALOG_MODULE, this))
	{
		m_mod.ShowWindow(SW_SHOW);
		m_mod.ListProcessModules(dwPID);
	}

}

//列表右键获取菜单
void CDia_Details::OnRclickListDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU_RClickList);
	pmenu = menu.GetSubMenu(0);//指定包含菜单中的弹出菜单的位置。 位置值从0开始第一个菜单项。 
	ClientToScreen(&pNMItemActivate->ptAction);//在客户端坐标中指示发生事件的位置
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);

	//获取PID
	int nRanNum = rand() / 2;
	DWORD dwPID = 0;
	if (nRanNum == 0)
	{
		WCHAR wcProcsessID[MAX_PATH] = {};
		if (pNMItemActivate->iItem != -1)//如果GetItemText项目索引未用于通知，则此成员将包含-1。
		{
			m_ListCtrl_De.GetItemText(pNMItemActivate->iItem, 1, wcProcsessID, MAX_PATH);//GetItemText:1.索引， 2.检索对象， 3， 接收文本的指针， 4， 缓冲区长度；返回检索字符串的长度(int)
		}
		dwPID = _wtoi(wcProcsessID);
	}
	else
	{
		std::wstring strPID;
		if (pNMItemActivate->iItem != -1)
		{
			strPID = m_ListCtrl_De.GetItemText(pNMItemActivate->iItem, 1);//GetItemText:1.索引， 2.检索对象。返回一个CString的文本
		}
		dwPID = _wtoi(strPID.c_str());
	}
	//菜单项分支
	switch (Select)
	{
	case ID_32778:	//遍历线程
	{
		m_Thread.Create(IDD_DIALOG_THREAD, this);
		m_Thread.ShowWindow(SW_SHOW);
		m_Thread.ListProcessThreads(dwPID);
	}
	break;
	case ID_32779:	//遍历模块
	{
		m_mod.Create(IDD_DIALOG_MODULE, this);
		m_mod.ShowWindow(SW_SHOW);
		m_mod.ListProcessModules(dwPID);
	}
	break;
	case ID_32780:	//遍历堆
	{
		m_Heap.Create(IDD_DIALOG_HEAP, this);
		m_Heap.ShowWindow(SW_SHOW);
		m_Heap.ListProcessHeap(dwPID);
	}
	break;
	case ID_IDR_32775:	//结束进程
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
		TerminateProcess(hProcess, 0);//结束进程
	}
	break;
	case ID_IDR_32776:	//打开文件所在位置
	{

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
		WCHAR wcFileName[MAX_PATH] = {};
		GetModuleFileNameEx(hProcess, NULL, wcFileName, MAX_PATH);

		PVOID pTemp = wcsrchr(wcFileName, '\\');//定位到路径XXX\\XXX\\xxx.exe中exe前\\的地址
		memset(pTemp, 0, 1);//将pTemp所指向的某一块内存中的后2个字节的内容全部设置为0的ASCII值,这里是删掉exe前的\\,只需要获取到文件夹路径为止
		ShellExecute(NULL, L"explore", wcFileName, NULL, NULL, SW_SHOW);
		CloseHandle(hProcess);
	}
	break;
	case ID_IDR_32777:	//刷新
	{
		UpdataList();
	}
	break;
	default:
		break;
	}
	*pResult = 0;
}
