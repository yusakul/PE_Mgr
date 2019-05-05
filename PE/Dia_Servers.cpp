// Dia_Servers.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Servers.h"
#include "afxdialogex.h"
#include <winsvc.h>


// CDia_Servers 对话框

IMPLEMENT_DYNAMIC(CDia_Servers, CDialogEx)

CDia_Servers::CDia_Servers(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SERVERS, pParent)
{

}

CDia_Servers::~CDia_Servers()
{
}

void CDia_Servers::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Servers, m_list_servers);
}


BEGIN_MESSAGE_MAP(CDia_Servers, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_Servers, &CDia_Servers::OnNMRClickListServers)
	//ON_CONTROL_RANGE(ID_32783, ID_32784, ID_32785, &CDia_Servers::OnMenuServers)
END_MESSAGE_MAP()


// CDia_Servers 消息处理程序


void CDia_Servers::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL CDia_Servers::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_list_servers.AddColumn(5, L"名称", 100, L"描述", 200, L"状态", 100, L"类型", 70, L"路径", 250);
	//刷新服务
	UpdataServers();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//刷新服务
void CDia_Servers::UpdataServers()
{
	m_list_servers.DeleteAllItems();
	GetServers();
}

//获取服务
void CDia_Servers::GetServers()
{
	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//第一次调用，获取所需内存大小
	DWORD dwServerNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(
		hSCM, 
		SC_ENUM_PROCESS_INFO,		
		SERVICE_WIN32,
		SERVICE_STATE_ALL,		//所有服务状态
		NULL,					//缓冲区
		0,						//缓冲区大小
		&dwSize,				//需要的大小
		&dwServerNum,			//缓冲区中的服务个数
		NULL,
		NULL);

	//申请所需内存，第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = 
		(LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];

	//第二次枚举
	bool bStasus = FALSE;
	bStasus = EnumServicesStatusEx(
		hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//所有服务状态
		(PBYTE)pEnumService,	//缓冲区
		dwSize,					//缓冲区大小
		&dwSize,				//需要的大小
		&dwServerNum,			//缓冲区中的服务个数
		NULL, NULL);

	//遍历信息
	for (DWORD i = 0; i < dwServerNum; i++)
	{
		//获取基础信息
		//服务名
		m_ServersName = pEnumService[i].lpServiceName;

		//服务状态 有已停止，正在运行， 正在暂停
		//根据得到的值手动输出字符串，具体值的含义可以再该结构体上按F1查MSDN
		DWORD dwStatus = pEnumService[i].ServiceStatusProcess.dwCurrentState;
		m_ServersStatus = ServerStatus[dwStatus];

		//服务类型
		// 有 文件系统驱动服务 驱动服务 独立进程服务
		pEnumService[i].ServiceStatusProcess.dwServiceType;

		//获取服务的详细信息
		//打开服务
		SC_HANDLE hService = OpenService(
			hSCM,	//服务控制管理器句柄
			pEnumService[i].lpServiceName,	//服务名
			SERVICE_QUERY_CONFIG	//打开权限
		);

		//第一次调用获取需要的缓冲区大小
		QueryServiceConfig(hService, NULL, 0, &dwSize);

		//分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig =
			(LPQUERY_SERVICE_CONFIG)new char[dwSize];

		//第二次调用,获取信息
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);

		//通过上面获取到的结构体信息得到想要的值

		//获取路径信息
		LPWSTR pPath = pServiceConfig->lpBinaryPathName;
		m_ServersPath = pPath;

		//获取启动类型
		DWORD dwType = pServiceConfig->dwStartType;
		if (dwType <= 5)
		{
			m_ServersType = ServerStartType[dwType];

		}
		else
			m_ServersType = L" ";

		if (dwType > 0)
		{
			//获取描述信息大小
			QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwSize);
			LPSERVICE_DESCRIPTION pServiceDesc = (LPSERVICE_DESCRIPTION)new char[dwSize];
			//获取描述
			QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)pServiceDesc, dwSize, &dwSize);
			m_ServersDesc = pServiceDesc->lpDescription;
		}
		else
		{
			//由于权限或什么原因，可能获取不到杀软服务的信息
			m_ServersDesc = L" ";
		}

		m_list_servers.AddItem(0, 5, m_ServersName, m_ServersDesc, m_ServersStatus, m_ServersType, m_ServersPath);

	}
}


//右键打开菜单
void CDia_Servers::OnNMRClickListServers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CMenu menu, *pmenu;
	menu.LoadMenuW(IDR_MENU2);
	pmenu = menu.GetSubMenu(0);//指定包含菜单中的弹出菜单的位置。 位置值从0开始第一个菜单项。 
	ClientToScreen(&pNMItemActivate->ptAction);//在客户端坐标中指示发生事件的位置
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);


	//获取服务状态
	CString stStatus;
	if (pNMItemActivate->iItem == -1)//如果GetItemText项目索引未用于通知，则此成员将包含-1。
	{
		return;
	}
	stStatus = m_list_servers.GetItemText(pNMItemActivate->iItem, 0);


	//判断服务状态
	if (stStatus == L"正在运行")
	{
		//MF_BYCOMMAND为默认， MF_GRAYED表示该菜单项已禁用并呈灰色，因此无法选择。
		EnableMenuItem(pmenu->m_hMenu, ID_32783, MF_BYCOMMAND | MF_GRAYED);
	}
	else if (stStatus == L"已停止")
	{
		EnableMenuItem(pmenu->m_hMenu, ID_32784, MF_BYCOMMAND | MF_GRAYED);
	}

	
	switch (Select)
	{
	case ID_32783:	//开始服务
		ServersKey(true);
		break;
	case ID_32784:	//停止服务
		ServersKey(false);
		break;
	case ID_32785:	//重新启动
		ServersKey(false);
		ServersKey(true);
		break;
	case ID_32786:	//刷新
		UpdataServers();
		break;
	default:
		break;
	}

}




//服务开关
void CDia_Servers::ServersKey(bool Keyflag)
{
	//获取选中项
	int nSelcet = m_list_servers.GetSelectionMark();
	if (nSelcet < 0)
	{
		return;
	}
	//获取服务名
	CString strServiceName = m_list_servers.GetItemText(nSelcet, 0);

	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(L"获取服务对象信息失败...");
		return;
	}

	//打开服务
	SC_HANDLE hService = OpenService(hSCM, (LPCTSTR )strServiceName,
		SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		MessageBox(L"打开服务失败...");
		CloseServiceHandle(hSCM);
		return;
	}

	//获取服务状态
	SERVICE_STATUS status;
	if (QueryServiceStatus(hService, &status) == FALSE)
	{
		MessageBox(L"获取服务状态失败...");
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return;
	}

	//判断是开始还是停止服务
	if (Keyflag)	//true开始
	{
		bool tem = StartService(hService, NULL, NULL);
		if (tem == FALSE)
		{
			MessageBox(L"启动服务失败...");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}
		
		//开始启动，等待中
		while (QueryServiceStatus(hService, &status)==TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox(L"启动服务成功...");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}

	else //关闭服务
	{
		if (ControlService(hService, SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			MessageBox(L"停止服务失败...");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}

		//等待服务停止
		while (QueryServiceStatus(hService, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"停止服务成功...");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}

}




/*

//服务开关
void CDia_Servers::ServersKey(bool Keyflag)
{
	//获取选中项
	int nSelcet = m_list_servers.GetSelectionMark();
	if (nSelcet < 0)
	{
		return;
	}
	//获取服务名
	CString strServiceName = m_list_servers.GetItemText(nSelcet, 0);

	//打开服务控制管理器
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(L"获取服务对象信息失败...");
		return;
	}

	//打开服务
	SC_HANDLE hService = OpenService(hSCM, (LPCTSTR)strServiceName,
		SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		MessageBox(L"打开服务失败...");
		CloseServiceHandle(hSCM);
		return;
	}

	//获取服务状态
	SERVICE_STATUS status;
	if (QueryServiceStatus(hService, &status) == FALSE)
	{
		MessageBox(L"获取服务状态失败...");
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return;
	}

	//判断是开始还是停止服务
	if (Keyflag)	//true开始
	{
		bool tem = StartService(hService, NULL, NULL);
		if (tem == FALSE)
		{
			MessageBox(L"启动服务失败...");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}

		//开始启动，等待中
		while (QueryServiceStatus(hService, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox(L"启动服务成功...");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}

	else //关闭服务
	{
		if (ControlService(hService, SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			MessageBox(L"停止服务失败...");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}

		//等待服务停止
		while (QueryServiceStatus(hService, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"停止服务成功...");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}

}
*/
