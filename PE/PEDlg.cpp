
// PEDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PEDlg.h"
#include "afxdialogex.h"
#include <afxwin.h>
#include <CommCtrl.h>
#include <shlobj.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

// CPEDlg 对话框



CPEDlg::CPEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_HOME, m_Tab_Home);
}

BEGIN_MESSAGE_MAP(CPEDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_WM_CLOSE()
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CPEDlg::OnEnChangeMfceditbrowse1)
	//ON_BN_CLICKED(IDC_BUTTON_OPENTASK, &CPEDlg::OnBnClickedButtonOpentask)
	ON_COMMAND(ID_CANCEL, &CPEDlg::OnCancel)
	ON_COMMAND(ID_LOGOUT, &CPEDlg::OnLogout)
	ON_COMMAND(ID_SHUTDOWN, &CPEDlg::OnShutdown)
	ON_COMMAND(ID_REBOOT, &CPEDlg::OnReboot)
	ON_COMMAND(ID_HOTKEY, &CPEDlg::OnHotkey)
	ON_BN_CLICKED(IDC_BUTTON_ADMIN, &CPEDlg::OnBnClickedButtonAdmin)
	ON_BN_CLICKED(IDC_BUTTON_PROCTECT, &CPEDlg::OnBnClickedButtonProctect)
END_MESSAGE_MAP()


// CPEDlg 消息处理程序

BOOL CPEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//菜单
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU_HOME);
	SetMenu(&m_menu);

	//注册快捷键
	::RegisterHotKey(m_hWnd, 1234, MOD_CONTROL | MOD_ALT, 'H');

	//打开PE文件提示
	SetDlgItemText(IDC_MFCEDITBROWSE1, L"选择PE文件...");

	//tab控件=========================================================
	m_Tab_Home.InsertItem(0, L"主页");
	m_Tab_Home.InsertItem(1, L"反病毒");
	m_Tab_Home.InsertItem(2, L"垃圾清理");
	m_Tab_Home.InsertItem(3, L"电脑加速");
	m_Tab_Home.InsertItem(4, L"任务管理器");
	m_Tab_Home.InsertItem(5, L"工具箱");




	//给子窗口指针赋值
	m_Tab_Home.m_Dia[0] = new CHome;
	m_Tab_Home.m_Dia[1] = new CAntiVirus;
	m_Tab_Home.m_Dia[2] = new CClean;
	m_Tab_Home.m_Dia[3] = new CSpeed;
	m_Tab_Home.m_Dia[4] = new CTaskMgr;
	m_Tab_Home.m_Dia[5] = new CTools;




	//创建子窗口
	m_Tab_Home.m_Dia[0]->Create(IDD_DIALOG_HOME, &m_Tab_Home);
	m_Tab_Home.m_Dia[1]->Create(IDD_DIALOG_ANTI_VIRUS, &m_Tab_Home);
	m_Tab_Home.m_Dia[2]->Create(IDD_DIALOG_CLEAN, &m_Tab_Home);
	m_Tab_Home.m_Dia[3]->Create(IDD_DIALOG_SPEED, &m_Tab_Home);
	m_Tab_Home.m_Dia[4]->Create(IDD_DIALOG_TASKMGR, &m_Tab_Home);
	m_Tab_Home.m_Dia[5]->Create(IDD_DIALOG_TOOL, &m_Tab_Home);


	//控制两个子窗口大小
	CRect rc;
	m_Tab_Home.GetClientRect(rc);
	rc.DeflateRect(2, 25, 2, 2);
	m_Tab_Home.m_Dia[0]->MoveWindow(rc);
	m_Tab_Home.m_Dia[1]->MoveWindow(rc);
	m_Tab_Home.m_Dia[2]->MoveWindow(rc);
	m_Tab_Home.m_Dia[3]->MoveWindow(rc);
	m_Tab_Home.m_Dia[4]->MoveWindow(rc);
	m_Tab_Home.m_Dia[5]->MoveWindow(rc);


	//显示第一个子窗口
	m_Tab_Home.m_Dia[0]->ShowWindow(SW_SHOW);
	m_Tab_Home.m_Dia[1]->ShowWindow(SW_HIDE);
	m_Tab_Home.m_Dia[2]->ShowWindow(SW_HIDE);
	m_Tab_Home.m_Dia[3]->ShowWindow(SW_HIDE);
	m_Tab_Home.m_Dia[4]->ShowWindow(SW_HIDE);
	m_Tab_Home.m_Dia[5]->ShowWindow(SW_HIDE);

	//管理员权限=============================================================
	CWnd   *pWnd = GetDlgItem(IDC_BUTTON_ADMIN); // 取得控件的指针
	HWND hButton = pWnd->GetSafeHwnd();  // 取得控件的句柄
	CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_ADMIN);
	//判断当前是否以管理员身份运行
	if (!IsAdmin(GetCurrentProcess()))
	{
		//如果不是，则讲一个按钮设置为带盾牌标志的按钮
		Button_SetElevationRequiredState(hButton, TRUE);
 	}
	else
	{
		//如果已经管理员身份运行，则将按钮隐藏
		pButton->ShowWindow(FALSE);
	}

	//进程保护=======================================================================Start
	//HookLoad();	//加载HOOK
	//进程保护=======================================================================End


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CPEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//拖拽读取PE文件
void CPEDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	TCHAR szPath[MAX_PATH] = {};
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);
	m_szPath = szPath;
	

	
	//读取文件到内存
	CPE* m_pPE=new CPE;

	if (m_pPE->ReadFileToMem(m_szPath))
	{
		if (m_pPE->IsPEFile())	//判断是否PE文件
		{
			PEINFO m_peInfo(m_pPE, m_szPath);
			m_peInfo.DoModal();
			// 		m_peInfo.Create(IDD_PEINFO, this);
			// 		ShowWindow(SW_SHOW);
		}
	}


	

	CDialogEx::OnDropFiles(hDropInfo);
}


void CPEDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//进程保护=======================================================================Start
	//HookUnload();    // 退出窗口，要卸载HOOK
	//进程保护=======================================================================End

	CDialogEx::OnClose();
}


//EditBrowseCtrl 控件打开PE文件
void CPEDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	GetDlgItemText(IDC_MFCEDITBROWSE1, m_szPath);
	if (m_szPath == L"选择PE文件...")
	{
		return;
	}
	//读取文件到内存
	CPE* m_pPE = new CPE;

	if (m_pPE->ReadFileToMem(m_szPath))
	{
		if (m_pPE->IsPEFile())	//判断是否PE文件
		{
			PEINFO m_peInfo(m_pPE, m_szPath);
			m_peInfo.DoModal();

			// 		m_peInfo.Create(IDD_PEINFO, this);
			// 		ShowWindow(SW_SHOW);
		}
	}

	SetDlgItemText(IDC_MFCEDITBROWSE1, L"选择PE文件...");
}


//打开任务管理器
void CPEDlg::OnBnClickedButtonOpentask()
{
	// TODO: 在此添加控件通知处理程序代码
	task.Create(IDD_DIALOG_TASKMGR, this);
	task.ShowWindow(SW_SHOW);
}


//菜单——退出
void CPEDlg::OnCancel()
{
	// TODO: 在此添加命令处理程序代码
	DestroyWindow();//非模态窗口
}


//菜单——注销
void CPEDlg::OnLogout()
{
	// TODO: 在此添加命令处理程序代码
	AdjustPrivilege();

	int resule = MessageBox(TEXT("确认注销"), TEXT("提示"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_LOGOFF, 0);
	}
}


//菜单——关机
void CPEDlg::OnShutdown()
{
	// TODO: 在此添加命令处理程序代码
	//提权
	AdjustPrivilege();
	int resule = MessageBox(TEXT("确认关机"), TEXT("提示"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_SHUTDOWN, 0);
	}
}


//菜单——重启
void CPEDlg::OnReboot()
{
	// TODO: 在此添加命令处理程序代码
	AdjustPrivilege();
	int resule = MessageBox(TEXT("确认重启"), TEXT("提示"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_REBOOT, 0);
	}
}


//菜单——热键
void CPEDlg::OnHotkey()
{
	// TODO: 在此添加命令处理程序代码
}


//提权函数
BOOL CPEDlg::AdjustPrivilege()
{
	BOOL bRet = FALSE;
	TOKEN_PRIVILEGES tp = { 0 };//令牌权限结构
	HANDLE hToken = nullptr;//令牌句柄

	do
	{
		//打开当前进程令牌,并且获取它              //令牌权限修改和查询
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
			TOKEN_QUERY, &hToken))
			break;
		//获取关机注销重启的LUID(Locally Unique Identifier),局部唯一标识
		if (!LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid))
			break;
		tp.PrivilegeCount = 1;//修改权限的个数
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//激活SE_SHUTDOWN_NAME这个权限
														   //提升权限//FALSE表示可以修改权限//把需要修改的权限传进来
		if (!AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)nullptr, 0))
			break;
		bRet = TRUE;
	} while (FALSE);
	if (hToken)
		CloseHandle(hToken);
	return bRet;
}


//点击获取权限======================================================
void CPEDlg::OnBnClickedButtonAdmin()
{
	// TODO: 在此添加控件通知处理程序代码

	//获取本进程路径
	TCHAR path[MAX_PATH] = { 0 };
	DWORD dwSize = MAX_PATH;
	QueryFullProcessImageName(GetCurrentProcess(), 0, path, &dwSize);

	//隐藏窗口
	ShowWindow(SW_HIDE);

	//以管理员身份运行
	ShellExecute(
		m_hWnd,
		L"runas",	//系统命令，以管理员权限运行一个进程
		path,
		NULL, NULL, SW_SHOW
	);

	if (ERROR_SUCCESS == GetLastError())
	{
		//如创建成功则退出进程
		ExitProcess(0);
	}
	else
	{
		//如果创建进程失败， 则将窗口显示回来
		ShowWindow(SW_SHOW);
	}

}

//是否是管理员

BOOL CPEDlg::IsAdmin(HANDLE hProcess)
{
	HANDLE hToken = NULL;
	OpenProcessToken(hProcess, TOKEN_QUERY, &hToken);

	//用于接收令牌类型
	TOKEN_ELEVATION_TYPE tokenType = TokenElevationTypeDefault;

	DWORD dwRetSize = 0;	//用于接收函数输出信息的字节数

	//查询进程令牌中的权限提升值（这个值记录当前的令牌是何种类型
	GetTokenInformation(
		hToken,
		TokenElevationType,		//获取令牌的当前提升等级
		&tokenType,
		sizeof(tokenType),
		&dwRetSize	//所需缓冲区的字节数
	);

	//分局令牌的类型来输出相应的信息
	if (TokenElevationTypeFull == tokenType)
	{
		//如果令牌是TokenElevationTypeFull,则拥有最高权限，可以给令牌添加任何特权,返回第0步执行代码.
		return TRUE;
	}
	else if (TokenElevationTypeDefault == tokenType)
	{
		//默认要哪个虎可能是一个没有权限的标准用户
		//也可能是被UAC关闭
		//直接调用系统的函数IsUserAbAdmin来判断用户是否是管理员
		return IsUserAnAdmin();
	}
	else if (TokenElevationTypeLimited == tokenType)
	{
		return FALSE;
	}
	return FALSE;
}




//进程保护=======================================================================Start

void CPEDlg::HookLoad() {
	m_hinstHookDll = ::LoadLibrary(_T("MonitorDll.dll"));
	CString loginfo;

	if (NULL == m_hinstHookDll) {
		loginfo.Format(_T("加载 MonitorDll.dll失败，错误代码 = [%d] "), GetLastError());
		AfxMessageBox(loginfo);
		return;
	}

	typedef BOOL(WINAPI* LoadMonitor)(HWND hwnd, DWORD dwProcessId);
	LoadMonitor loadMonitor = NULL;
	loadMonitor = (LoadMonitor)::GetProcAddress(m_hinstHookDll, "HookLoad");
	if (NULL == loadMonitor) {
		loginfo.Format(_T("获取函数 HookLoad 失败，错误代码 = [%d]"), GetLastError());
		AfxMessageBox(loginfo);
	}
	if (loadMonitor(m_hWnd, GetCurrentProcessId())) {
		loginfo.Format(_T("HOOK加载成功"));
		AfxMessageBox(loginfo);
	}
	else {
		loginfo.Format(_T("HOOK加载失败"));
		AfxMessageBox(loginfo);
	}
}

/*
卸载HOOKDLL
*/

void CPEDlg::HookUnload() {
	CString logInfo;
	if (m_hinstHookDll == NULL) {
		m_hinstHookDll = LoadLibrary(_T("MonitorDll.dll"));
		if (NULL == m_hinstHookDll) {
			logInfo.Format(_T("加载 MonitorDll.dll失败，错误代码 = [%d]"), GetLastError());
			AfxMessageBox(logInfo);
			return;
		}
	}

	typedef VOID(WINAPI* UnloadHook)();
	UnloadHook unloadHook = NULL;
	unloadHook = (UnloadHook)::GetProcAddress(m_hinstHookDll, "HookUnload");
	if (NULL == unloadHook) {
		logInfo.Format(_T("获取函数 HookUnload 失败，错误代码 = [%d]"), GetLastError());
		AfxMessageBox(logInfo);
		return;
	}
	unloadHook();
}

//进程保护=======================================================================End




//快捷键
BOOL CPEDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 1234))
	{
		if (IsWindowVisible() == TRUE)
			ShowWindow(SW_HIDE);
		else
			ShowWindow(SW_SHOW);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


//进程保护=============================================================================================
void CPEDlg::OnBnClickedButtonProctect()
{
	// TODO: 在此添加控件通知处理程序代码
	if (StartInsert("任务管理器", NULL, "H:\\MFCLibrary1.dll") == false)
	{
		::MessageBox(NULL, _T("查找窗口失败"), _T("error"), MB_OK);
	}
}


//开始注入
bool CPEDlg::StartInsert(char *title, char *winclass, char *dllpath)
{
	//开始注入
	HWND hwnd = FindWindowExA(NULL, NULL, winclass, title);

	if (hwnd == 0)
		return false;
	HANDLE hp = GetThePidOfTargetProcess(hwnd);
	StartInject();

	if (DoInjection(dllpath, hp))
	{


	}
	return true;
}

//重写开始注入
void CPEDlg::StartInject()
{
	HANDLE hMap;
	hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 12, TEXT("hook"));
	LPVOID lpmem;
	if (hMap == NULL)
	{
		return;
	}
	lpmem = MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (lpmem == NULL)
	{
		return;
	}
	*(DWORD *)lpmem = GetCurrentProcessId();
	//StartInsert("endprocess", NULL, "D:\\hook.dll");
}


//获取目标进程PID
HANDLE CPEDlg::GetThePidOfTargetProcess(HWND hwnd)
{
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hProcee = ::OpenProcess(PROCESS_ALL_ACCESS | PROCESS_CREATE_THREAD, 0, pid);
	return hProcee;
}

//注入
BOOL CPEDlg::DoInjection(char *DllPath, HANDLE hProcess)
{
	DWORD BufSize = strlen(DllPath) + 1;
	LPVOID AllocAddr = VirtualAllocEx(hProcess, NULL, BufSize, MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(hProcess, AllocAddr, DllPath, BufSize, NULL);
	PTHREAD_START_ROUTINE pfnStartAddr = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");

	HANDLE hRemoteThread;
	hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, pfnStartAddr, AllocAddr, 0, NULL);
	if (hRemoteThread)
	{
		::MessageBox(NULL, TEXT("注入成功"), TEXT("提示"), MB_OK);
		return true;
	}
	else
	{
		::MessageBox(NULL, TEXT("注入失败"), TEXT("提示"), MB_OK);
		return false;
	}


}
