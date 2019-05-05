
// PEDlg.cpp : ʵ���ļ�
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

// CPEDlg �Ի���



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


// CPEDlg ��Ϣ�������

BOOL CPEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//�˵�
	CMenu m_menu;
	m_menu.LoadMenu(IDR_MENU_HOME);
	SetMenu(&m_menu);

	//ע���ݼ�
	::RegisterHotKey(m_hWnd, 1234, MOD_CONTROL | MOD_ALT, 'H');

	//��PE�ļ���ʾ
	SetDlgItemText(IDC_MFCEDITBROWSE1, L"ѡ��PE�ļ�...");

	//tab�ؼ�=========================================================
	m_Tab_Home.InsertItem(0, L"��ҳ");
	m_Tab_Home.InsertItem(1, L"������");
	m_Tab_Home.InsertItem(2, L"��������");
	m_Tab_Home.InsertItem(3, L"���Լ���");
	m_Tab_Home.InsertItem(4, L"���������");
	m_Tab_Home.InsertItem(5, L"������");




	//���Ӵ���ָ�븳ֵ
	m_Tab_Home.m_Dia[0] = new CHome;
	m_Tab_Home.m_Dia[1] = new CAntiVirus;
	m_Tab_Home.m_Dia[2] = new CClean;
	m_Tab_Home.m_Dia[3] = new CSpeed;
	m_Tab_Home.m_Dia[4] = new CTaskMgr;
	m_Tab_Home.m_Dia[5] = new CTools;




	//�����Ӵ���
	m_Tab_Home.m_Dia[0]->Create(IDD_DIALOG_HOME, &m_Tab_Home);
	m_Tab_Home.m_Dia[1]->Create(IDD_DIALOG_ANTI_VIRUS, &m_Tab_Home);
	m_Tab_Home.m_Dia[2]->Create(IDD_DIALOG_CLEAN, &m_Tab_Home);
	m_Tab_Home.m_Dia[3]->Create(IDD_DIALOG_SPEED, &m_Tab_Home);
	m_Tab_Home.m_Dia[4]->Create(IDD_DIALOG_TASKMGR, &m_Tab_Home);
	m_Tab_Home.m_Dia[5]->Create(IDD_DIALOG_TOOL, &m_Tab_Home);


	//���������Ӵ��ڴ�С
	CRect rc;
	m_Tab_Home.GetClientRect(rc);
	rc.DeflateRect(2, 25, 2, 2);
	m_Tab_Home.m_Dia[0]->MoveWindow(rc);
	m_Tab_Home.m_Dia[1]->MoveWindow(rc);
	m_Tab_Home.m_Dia[2]->MoveWindow(rc);
	m_Tab_Home.m_Dia[3]->MoveWindow(rc);
	m_Tab_Home.m_Dia[4]->MoveWindow(rc);
	m_Tab_Home.m_Dia[5]->MoveWindow(rc);


	//��ʾ��һ���Ӵ���
	m_Tab_Home.m_Dia[0]->ShowWindow(SW_SHOW);
	m_Tab_Home.m_Dia[1]->ShowWindow(SW_HIDE);
	m_Tab_Home.m_Dia[2]->ShowWindow(SW_HIDE);
	m_Tab_Home.m_Dia[3]->ShowWindow(SW_HIDE);
	m_Tab_Home.m_Dia[4]->ShowWindow(SW_HIDE);
	m_Tab_Home.m_Dia[5]->ShowWindow(SW_HIDE);

	//����ԱȨ��=============================================================
	CWnd   *pWnd = GetDlgItem(IDC_BUTTON_ADMIN); // ȡ�ÿؼ���ָ��
	HWND hButton = pWnd->GetSafeHwnd();  // ȡ�ÿؼ��ľ��
	CButton *pButton = (CButton*)GetDlgItem(IDC_BUTTON_ADMIN);
	//�жϵ�ǰ�Ƿ��Թ���Ա�������
	if (!IsAdmin(GetCurrentProcess()))
	{
		//������ǣ���һ����ť����Ϊ�����Ʊ�־�İ�ť
		Button_SetElevationRequiredState(hButton, TRUE);
 	}
	else
	{
		//����Ѿ�����Ա������У��򽫰�ť����
		pButton->ShowWindow(FALSE);
	}

	//���̱���=======================================================================Start
	//HookLoad();	//����HOOK
	//���̱���=======================================================================End


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CPEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��ק��ȡPE�ļ�
void CPEDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ


	TCHAR szPath[MAX_PATH] = {};
	DragQueryFile(hDropInfo, 0, szPath, MAX_PATH);
	m_szPath = szPath;
	

	
	//��ȡ�ļ����ڴ�
	CPE* m_pPE=new CPE;

	if (m_pPE->ReadFileToMem(m_szPath))
	{
		if (m_pPE->IsPEFile())	//�ж��Ƿ�PE�ļ�
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���̱���=======================================================================Start
	//HookUnload();    // �˳����ڣ�Ҫж��HOOK
	//���̱���=======================================================================End

	CDialogEx::OnClose();
}


//EditBrowseCtrl �ؼ���PE�ļ�
void CPEDlg::OnEnChangeMfceditbrowse1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	GetDlgItemText(IDC_MFCEDITBROWSE1, m_szPath);
	if (m_szPath == L"ѡ��PE�ļ�...")
	{
		return;
	}
	//��ȡ�ļ����ڴ�
	CPE* m_pPE = new CPE;

	if (m_pPE->ReadFileToMem(m_szPath))
	{
		if (m_pPE->IsPEFile())	//�ж��Ƿ�PE�ļ�
		{
			PEINFO m_peInfo(m_pPE, m_szPath);
			m_peInfo.DoModal();

			// 		m_peInfo.Create(IDD_PEINFO, this);
			// 		ShowWindow(SW_SHOW);
		}
	}

	SetDlgItemText(IDC_MFCEDITBROWSE1, L"ѡ��PE�ļ�...");
}


//�����������
void CPEDlg::OnBnClickedButtonOpentask()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	task.Create(IDD_DIALOG_TASKMGR, this);
	task.ShowWindow(SW_SHOW);
}


//�˵������˳�
void CPEDlg::OnCancel()
{
	// TODO: �ڴ���������������
	DestroyWindow();//��ģ̬����
}


//�˵�����ע��
void CPEDlg::OnLogout()
{
	// TODO: �ڴ���������������
	AdjustPrivilege();

	int resule = MessageBox(TEXT("ȷ��ע��"), TEXT("��ʾ"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_LOGOFF, 0);
	}
}


//�˵������ػ�
void CPEDlg::OnShutdown()
{
	// TODO: �ڴ���������������
	//��Ȩ
	AdjustPrivilege();
	int resule = MessageBox(TEXT("ȷ�Ϲػ�"), TEXT("��ʾ"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_SHUTDOWN, 0);
	}
}


//�˵���������
void CPEDlg::OnReboot()
{
	// TODO: �ڴ���������������
	AdjustPrivilege();
	int resule = MessageBox(TEXT("ȷ������"), TEXT("��ʾ"), MB_OKCANCEL);
	if (resule == IDOK)
	{
		ExitWindowsEx(EWX_REBOOT, 0);
	}
}


//�˵������ȼ�
void CPEDlg::OnHotkey()
{
	// TODO: �ڴ���������������
}


//��Ȩ����
BOOL CPEDlg::AdjustPrivilege()
{
	BOOL bRet = FALSE;
	TOKEN_PRIVILEGES tp = { 0 };//����Ȩ�޽ṹ
	HANDLE hToken = nullptr;//���ƾ��

	do
	{
		//�򿪵�ǰ��������,���һ�ȡ��              //����Ȩ���޸ĺͲ�ѯ
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES |
			TOKEN_QUERY, &hToken))
			break;
		//��ȡ�ػ�ע��������LUID(Locally Unique Identifier),�ֲ�Ψһ��ʶ
		if (!LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid))
			break;
		tp.PrivilegeCount = 1;//�޸�Ȩ�޵ĸ���
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;//����SE_SHUTDOWN_NAME���Ȩ��
														   //����Ȩ��//FALSE��ʾ�����޸�Ȩ��//����Ҫ�޸ĵ�Ȩ�޴�����
		if (!AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)nullptr, 0))
			break;
		bRet = TRUE;
	} while (FALSE);
	if (hToken)
		CloseHandle(hToken);
	return bRet;
}


//�����ȡȨ��======================================================
void CPEDlg::OnBnClickedButtonAdmin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//��ȡ������·��
	TCHAR path[MAX_PATH] = { 0 };
	DWORD dwSize = MAX_PATH;
	QueryFullProcessImageName(GetCurrentProcess(), 0, path, &dwSize);

	//���ش���
	ShowWindow(SW_HIDE);

	//�Թ���Ա�������
	ShellExecute(
		m_hWnd,
		L"runas",	//ϵͳ����Թ���ԱȨ������һ������
		path,
		NULL, NULL, SW_SHOW
	);

	if (ERROR_SUCCESS == GetLastError())
	{
		//�紴���ɹ����˳�����
		ExitProcess(0);
	}
	else
	{
		//�����������ʧ�ܣ� �򽫴�����ʾ����
		ShowWindow(SW_SHOW);
	}

}

//�Ƿ��ǹ���Ա

BOOL CPEDlg::IsAdmin(HANDLE hProcess)
{
	HANDLE hToken = NULL;
	OpenProcessToken(hProcess, TOKEN_QUERY, &hToken);

	//���ڽ�����������
	TOKEN_ELEVATION_TYPE tokenType = TokenElevationTypeDefault;

	DWORD dwRetSize = 0;	//���ڽ��պ��������Ϣ���ֽ���

	//��ѯ���������е�Ȩ������ֵ�����ֵ��¼��ǰ�������Ǻ�������
	GetTokenInformation(
		hToken,
		TokenElevationType,		//��ȡ���Ƶĵ�ǰ�����ȼ�
		&tokenType,
		sizeof(tokenType),
		&dwRetSize	//���軺�������ֽ���
	);

	//�־����Ƶ������������Ӧ����Ϣ
	if (TokenElevationTypeFull == tokenType)
	{
		//���������TokenElevationTypeFull,��ӵ�����Ȩ�ޣ����Ը���������κ���Ȩ,���ص�0��ִ�д���.
		return TRUE;
	}
	else if (TokenElevationTypeDefault == tokenType)
	{
		//Ĭ��Ҫ�ĸ���������һ��û��Ȩ�޵ı�׼�û�
		//Ҳ�����Ǳ�UAC�ر�
		//ֱ�ӵ���ϵͳ�ĺ���IsUserAbAdmin���ж��û��Ƿ��ǹ���Ա
		return IsUserAnAdmin();
	}
	else if (TokenElevationTypeLimited == tokenType)
	{
		return FALSE;
	}
	return FALSE;
}




//���̱���=======================================================================Start

void CPEDlg::HookLoad() {
	m_hinstHookDll = ::LoadLibrary(_T("MonitorDll.dll"));
	CString loginfo;

	if (NULL == m_hinstHookDll) {
		loginfo.Format(_T("���� MonitorDll.dllʧ�ܣ�������� = [%d] "), GetLastError());
		AfxMessageBox(loginfo);
		return;
	}

	typedef BOOL(WINAPI* LoadMonitor)(HWND hwnd, DWORD dwProcessId);
	LoadMonitor loadMonitor = NULL;
	loadMonitor = (LoadMonitor)::GetProcAddress(m_hinstHookDll, "HookLoad");
	if (NULL == loadMonitor) {
		loginfo.Format(_T("��ȡ���� HookLoad ʧ�ܣ�������� = [%d]"), GetLastError());
		AfxMessageBox(loginfo);
	}
	if (loadMonitor(m_hWnd, GetCurrentProcessId())) {
		loginfo.Format(_T("HOOK���سɹ�"));
		AfxMessageBox(loginfo);
	}
	else {
		loginfo.Format(_T("HOOK����ʧ��"));
		AfxMessageBox(loginfo);
	}
}

/*
ж��HOOKDLL
*/

void CPEDlg::HookUnload() {
	CString logInfo;
	if (m_hinstHookDll == NULL) {
		m_hinstHookDll = LoadLibrary(_T("MonitorDll.dll"));
		if (NULL == m_hinstHookDll) {
			logInfo.Format(_T("���� MonitorDll.dllʧ�ܣ�������� = [%d]"), GetLastError());
			AfxMessageBox(logInfo);
			return;
		}
	}

	typedef VOID(WINAPI* UnloadHook)();
	UnloadHook unloadHook = NULL;
	unloadHook = (UnloadHook)::GetProcAddress(m_hinstHookDll, "HookUnload");
	if (NULL == unloadHook) {
		logInfo.Format(_T("��ȡ���� HookUnload ʧ�ܣ�������� = [%d]"), GetLastError());
		AfxMessageBox(logInfo);
		return;
	}
	unloadHook();
}

//���̱���=======================================================================End




//��ݼ�
BOOL CPEDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == 1234))
	{
		if (IsWindowVisible() == TRUE)
			ShowWindow(SW_HIDE);
		else
			ShowWindow(SW_SHOW);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


//���̱���=============================================================================================
void CPEDlg::OnBnClickedButtonProctect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (StartInsert("���������", NULL, "H:\\MFCLibrary1.dll") == false)
	{
		::MessageBox(NULL, _T("���Ҵ���ʧ��"), _T("error"), MB_OK);
	}
}


//��ʼע��
bool CPEDlg::StartInsert(char *title, char *winclass, char *dllpath)
{
	//��ʼע��
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

//��д��ʼע��
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


//��ȡĿ�����PID
HANDLE CPEDlg::GetThePidOfTargetProcess(HWND hwnd)
{
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE hProcee = ::OpenProcess(PROCESS_ALL_ACCESS | PROCESS_CREATE_THREAD, 0, pid);
	return hProcee;
}

//ע��
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
		::MessageBox(NULL, TEXT("ע��ɹ�"), TEXT("��ʾ"), MB_OK);
		return true;
	}
	else
	{
		::MessageBox(NULL, TEXT("ע��ʧ��"), TEXT("��ʾ"), MB_OK);
		return false;
	}


}
