// Dia_Servers.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Servers.h"
#include "afxdialogex.h"
#include <winsvc.h>


// CDia_Servers �Ի���

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


// CDia_Servers ��Ϣ�������


void CDia_Servers::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


BOOL CDia_Servers::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_list_servers.AddColumn(5, L"����", 100, L"����", 200, L"״̬", 100, L"����", 70, L"·��", 250);
	//ˢ�·���
	UpdataServers();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//ˢ�·���
void CDia_Servers::UpdataServers()
{
	m_list_servers.DeleteAllItems();
	GetServers();
}

//��ȡ����
void CDia_Servers::GetServers()
{
	//�򿪷�����ƹ�����
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	//��һ�ε��ã���ȡ�����ڴ��С
	DWORD dwServerNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(
		hSCM, 
		SC_ENUM_PROCESS_INFO,		
		SERVICE_WIN32,
		SERVICE_STATE_ALL,		//���з���״̬
		NULL,					//������
		0,						//��������С
		&dwSize,				//��Ҫ�Ĵ�С
		&dwServerNum,			//�������еķ������
		NULL,
		NULL);

	//���������ڴ棬�ڶ��ε���
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = 
		(LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];

	//�ڶ���ö��
	bool bStasus = FALSE;
	bStasus = EnumServicesStatusEx(
		hSCM,
		SC_ENUM_PROCESS_INFO,
		SERVICE_WIN32,
		SERVICE_STATE_ALL,	//���з���״̬
		(PBYTE)pEnumService,	//������
		dwSize,					//��������С
		&dwSize,				//��Ҫ�Ĵ�С
		&dwServerNum,			//�������еķ������
		NULL, NULL);

	//������Ϣ
	for (DWORD i = 0; i < dwServerNum; i++)
	{
		//��ȡ������Ϣ
		//������
		m_ServersName = pEnumService[i].lpServiceName;

		//����״̬ ����ֹͣ���������У� ������ͣ
		//���ݵõ���ֵ�ֶ�����ַ���������ֵ�ĺ�������ٸýṹ���ϰ�F1��MSDN
		DWORD dwStatus = pEnumService[i].ServiceStatusProcess.dwCurrentState;
		m_ServersStatus = ServerStatus[dwStatus];

		//��������
		// �� �ļ�ϵͳ�������� �������� �������̷���
		pEnumService[i].ServiceStatusProcess.dwServiceType;

		//��ȡ�������ϸ��Ϣ
		//�򿪷���
		SC_HANDLE hService = OpenService(
			hSCM,	//������ƹ��������
			pEnumService[i].lpServiceName,	//������
			SERVICE_QUERY_CONFIG	//��Ȩ��
		);

		//��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig(hService, NULL, 0, &dwSize);

		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig =
			(LPQUERY_SERVICE_CONFIG)new char[dwSize];

		//�ڶ��ε���,��ȡ��Ϣ
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);

		//ͨ�������ȡ���Ľṹ����Ϣ�õ���Ҫ��ֵ

		//��ȡ·����Ϣ
		LPWSTR pPath = pServiceConfig->lpBinaryPathName;
		m_ServersPath = pPath;

		//��ȡ��������
		DWORD dwType = pServiceConfig->dwStartType;
		if (dwType <= 5)
		{
			m_ServersType = ServerStartType[dwType];

		}
		else
			m_ServersType = L" ";

		if (dwType > 0)
		{
			//��ȡ������Ϣ��С
			QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &dwSize);
			LPSERVICE_DESCRIPTION pServiceDesc = (LPSERVICE_DESCRIPTION)new char[dwSize];
			//��ȡ����
			QueryServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)pServiceDesc, dwSize, &dwSize);
			m_ServersDesc = pServiceDesc->lpDescription;
		}
		else
		{
			//����Ȩ�޻�ʲôԭ�򣬿��ܻ�ȡ����ɱ��������Ϣ
			m_ServersDesc = L" ";
		}

		m_list_servers.AddItem(0, 5, m_ServersName, m_ServersDesc, m_ServersStatus, m_ServersType, m_ServersPath);

	}
}


//�Ҽ��򿪲˵�
void CDia_Servers::OnNMRClickListServers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CMenu menu, *pmenu;
	menu.LoadMenuW(IDR_MENU2);
	pmenu = menu.GetSubMenu(0);//ָ�������˵��еĵ����˵���λ�á� λ��ֵ��0��ʼ��һ���˵�� 
	ClientToScreen(&pNMItemActivate->ptAction);//�ڿͻ���������ָʾ�����¼���λ��
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);


	//��ȡ����״̬
	CString stStatus;
	if (pNMItemActivate->iItem == -1)//���GetItemText��Ŀ����δ����֪ͨ����˳�Ա������-1��
	{
		return;
	}
	stStatus = m_list_servers.GetItemText(pNMItemActivate->iItem, 0);


	//�жϷ���״̬
	if (stStatus == L"��������")
	{
		//MF_BYCOMMANDΪĬ�ϣ� MF_GRAYED��ʾ�ò˵����ѽ��ò��ʻ�ɫ������޷�ѡ��
		EnableMenuItem(pmenu->m_hMenu, ID_32783, MF_BYCOMMAND | MF_GRAYED);
	}
	else if (stStatus == L"��ֹͣ")
	{
		EnableMenuItem(pmenu->m_hMenu, ID_32784, MF_BYCOMMAND | MF_GRAYED);
	}

	
	switch (Select)
	{
	case ID_32783:	//��ʼ����
		ServersKey(true);
		break;
	case ID_32784:	//ֹͣ����
		ServersKey(false);
		break;
	case ID_32785:	//��������
		ServersKey(false);
		ServersKey(true);
		break;
	case ID_32786:	//ˢ��
		UpdataServers();
		break;
	default:
		break;
	}

}




//���񿪹�
void CDia_Servers::ServersKey(bool Keyflag)
{
	//��ȡѡ����
	int nSelcet = m_list_servers.GetSelectionMark();
	if (nSelcet < 0)
	{
		return;
	}
	//��ȡ������
	CString strServiceName = m_list_servers.GetItemText(nSelcet, 0);

	//�򿪷�����ƹ�����
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(L"��ȡ���������Ϣʧ��...");
		return;
	}

	//�򿪷���
	SC_HANDLE hService = OpenService(hSCM, (LPCTSTR )strServiceName,
		SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		MessageBox(L"�򿪷���ʧ��...");
		CloseServiceHandle(hSCM);
		return;
	}

	//��ȡ����״̬
	SERVICE_STATUS status;
	if (QueryServiceStatus(hService, &status) == FALSE)
	{
		MessageBox(L"��ȡ����״̬ʧ��...");
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return;
	}

	//�ж��ǿ�ʼ����ֹͣ����
	if (Keyflag)	//true��ʼ
	{
		bool tem = StartService(hService, NULL, NULL);
		if (tem == FALSE)
		{
			MessageBox(L"��������ʧ��...");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}
		
		//��ʼ�������ȴ���
		while (QueryServiceStatus(hService, &status)==TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox(L"��������ɹ�...");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}

	else //�رշ���
	{
		if (ControlService(hService, SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			MessageBox(L"ֹͣ����ʧ��...");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}

		//�ȴ�����ֹͣ
		while (QueryServiceStatus(hService, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"ֹͣ����ɹ�...");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}

}




/*

//���񿪹�
void CDia_Servers::ServersKey(bool Keyflag)
{
	//��ȡѡ����
	int nSelcet = m_list_servers.GetSelectionMark();
	if (nSelcet < 0)
	{
		return;
	}
	//��ȡ������
	CString strServiceName = m_list_servers.GetItemText(nSelcet, 0);

	//�򿪷�����ƹ�����
	SC_HANDLE hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(L"��ȡ���������Ϣʧ��...");
		return;
	}

	//�򿪷���
	SC_HANDLE hService = OpenService(hSCM, (LPCTSTR)strServiceName,
		SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		MessageBox(L"�򿪷���ʧ��...");
		CloseServiceHandle(hSCM);
		return;
	}

	//��ȡ����״̬
	SERVICE_STATUS status;
	if (QueryServiceStatus(hService, &status) == FALSE)
	{
		MessageBox(L"��ȡ����״̬ʧ��...");
		CloseServiceHandle(hSCM);
		CloseServiceHandle(hService);
		return;
	}

	//�ж��ǿ�ʼ����ֹͣ����
	if (Keyflag)	//true��ʼ
	{
		bool tem = StartService(hService, NULL, NULL);
		if (tem == FALSE)
		{
			MessageBox(L"��������ʧ��...");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}

		//��ʼ�������ȴ���
		while (QueryServiceStatus(hService, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				MessageBox(L"��������ɹ�...");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}

	else //�رշ���
	{
		if (ControlService(hService, SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			MessageBox(L"ֹͣ����ʧ��...");
			CloseServiceHandle(hSCM);
			CloseServiceHandle(hService);
			return;
		}

		//�ȴ�����ֹͣ
		while (QueryServiceStatus(hService, &status) == TRUE)
		{
			Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				MessageBox(L"ֹͣ����ɹ�...");
				CloseServiceHandle(hSCM);
				CloseServiceHandle(hService);
				return;
			}
		}
	}

}
*/
