// Dia_WinProcess1.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_WinProcess1.h"
#include "afxdialogex.h"



// CDia_WinProcess1 对话框

IMPLEMENT_DYNAMIC(CDia_WinProcess1, CDialogEx)

CDia_WinProcess1::CDia_WinProcess1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_WINPROCESS, pParent)
{

}

CDia_WinProcess1::~CDia_WinProcess1()
{
}

void CDia_WinProcess1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WinProc, m_ListCtrl_winproc);
}


BEGIN_MESSAGE_MAP(CDia_WinProcess1, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_WinProc, &CDia_WinProcess1::OnNMRClickListWinproc)
END_MESSAGE_MAP()


// CDia_WinProcess1 消息处理程序


void CDia_WinProcess1::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL CDia_WinProcess1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	SetTimer(IDC_LIST_WinProc, 30000, NULL);
	pImageList->Create(16, 16, ILC_COLOR32, 0, 0);

	m_ListCtrl_winproc.DeleteAllItems();
	m_ListCtrl_winproc.SetExtendedStyle(m_ListCtrl_winproc.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	m_ListCtrl_winproc.InsertColumn(0, _T("应用程序"), 0, 400);
	m_ListCtrl_winproc.InsertColumn(1, _T("句柄"), 0, 200);
	m_ListCtrl_winproc.InsertColumn(2, _T("PID"), 0, 200);

	UpdataList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDia_WinProcess1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnTimer(nIDEvent);
}


//遍历窗口进程
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	LRESULT WndClassex;//WNDCLASSEX包含指定结构大小的cbSize成员和包含与窗口类关联的小图标的句柄的hIconSm成员。
	CDia_WinProcess1* m_winproc = (CDia_WinProcess1*)lParam;
	MyWinProcStruct Temp_list;
	DWORD temp_PID;
	if (!GetWindow(hWnd, GW_OWNER) && IsWindowVisible(hWnd))//如果指定窗口，其父窗口，其父窗口等具有WS_VISIBLE样式，则返回值为非零值。否则，返回值为零。由于返回值指定窗口是否具有WS_VISIBLE样式，因此即使窗口完全被其他窗口遮挡，该窗口也可能不为零。
	{
		GetWindowText(hWnd, Temp_list.lpWinTitle, MAX_PATH - 1);//如果函数成功，则返回值是复制字符串的长度（以字符为单位），不包括终止空字符。如果窗口没有标题栏或文本，如果标题栏为空，或者窗口或控件句柄无效，则返回值为零。
		if (wcscmp(Temp_list.lpWinTitle, L"") == 0)//如果标题内容为空
		{
			return true;
		}
		else
		{
			GetWindowThreadProcessId(hWnd, &temp_PID);
			WndClassex = GetClassLong(hWnd, NULL); //GCL_HICONSM检索与该类关联的小图标的句柄。
												   //WndClassex = GetClassLong(hWnd,); //
			if (WndClassex == 0)
			{
				WndClassex = SendMessage(hWnd, WM_GETICON, 0, 0);
			}
			Temp_list.hIcon = (HICON)WndClassex;//获取图标
			Temp_list.hPID = temp_PID;//获取PID
			Temp_list.hWnd = hWnd;//获取句柄
			m_winproc->m_WinProcVec.push_back(Temp_list);
		}

	}
	return true;
}


void CDia_WinProcess1::UpdataList()
{
	m_ListCtrl_winproc.DeleteAllItems();

	m_ListCtrl_winproc.SetImageList(pImageList, LVSIL_SMALL);//设置小图标
	TCHAR* temp_buff = new TCHAR[MAX_PATH];

	m_WinProcVec.clear();
	EnumWindows(EnumWindowsProc, (LPARAM)this);
	for (UINT i = 0; i < m_WinProcVec.size(); i++)
	{
		//添加图标
		m_ListCtrl_winproc.InsertItem(i, m_WinProcVec[i].lpWinTitle, i);
		pImageList->Add(m_WinProcVec[i].hIcon);
		//添加pid
		wsprintf(temp_buff, L"%d", m_WinProcVec[i].hPID);
		m_ListCtrl_winproc.SetItemText(i, 2, temp_buff);
		//添加句柄
		wsprintf(temp_buff, L"%d", m_WinProcVec[i].hWnd);
		m_ListCtrl_winproc.SetItemText(i, 1, temp_buff);
		m_WinProcVec[i].ListID = i;
	}
	delete temp_buff;
}


//右键list列表呼出Menu结束进程
void CDia_WinProcess1::OnNMRClickListWinproc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU1);
	pmenu = menu.GetSubMenu(0);//指定包含菜单中的弹出菜单的位置。 位置值从0开始第一个菜单项。 
	ClientToScreen(&pNMItemActivate->ptAction);//在客户端坐标中指示发生事件的位置
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);

	//获取PID

	DWORD dwPID = 0;

	WCHAR wcProcsessID[MAX_PATH] = {};
	if (pNMItemActivate->iItem != -1)//如果GetItemText项目索引未用于通知，则此成员将包含-1。
	{
		m_ListCtrl_winproc.GetItemText(pNMItemActivate->iItem, 2, wcProcsessID, MAX_PATH);//GetItemText:1.索引， 2.检索对象， 3， 接收文本的指针， 4， 缓冲区长度；返回检索字符串的长度(int)
	}
	dwPID = _wtoi(wcProcsessID);

	//菜单项分支
	switch (Select)
	{
	case ID_KILLPROCESS_32781:	//结束任务
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
		TerminateProcess(hProcess, 0);//结束进程
	}
	break;
	case ID_KILLPROCESS_32782:	//刷新
	{
		UpdataList();
	}
	break;
	default:
		break;
	}


	*pResult = 0;
}
