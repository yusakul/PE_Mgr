// SoftMgr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "SoftMgr.h"
#include "afxdialogex.h"


// CSoftMgr �Ի���

IMPLEMENT_DYNAMIC(CSoftMgr, CDialogEx)

CSoftMgr::CSoftMgr(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SOFTWARE, pParent)
{
	m_SubKey1 = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	m_SubKey2 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	m_hRootKey = HKEY_LOCAL_MACHINE;

	m_KeyName = L"DisplayName";
	m_KeyNameVer = L"DisplayVersion";
	m_KeyNameSize = L"EstimatedSize";
	m_KeyNameDate = L"InstallDate";
	m_KeyNamePub = L"Publisher";
	m_KeyNameInsPath = L"InstallLocation";
	m_KeyNameUniPath = L"UninstallString";
}

CSoftMgr::~CSoftMgr()
{
}

void CSoftMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SOFTWARE, m_list_soft);
}


BEGIN_MESSAGE_MAP(CSoftMgr, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SOFTWARE, &CSoftMgr::OnNMRClickListSoftware)
END_MESSAGE_MAP()


// CSoftMgr ��Ϣ�������


void CSoftMgr::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


BOOL CSoftMgr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_list_soft.AddColumn(7, L"����", 350, L"������", 150, L"��װ����", 80, L"��С", 80, L"�汾", 150, L"��װ·��", 250, L"ж��·��", 250);

	//��ȡ�����Ϣ
	UpdateSoftInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



void CSoftMgr::GetSoftwareInfo(CString SubKey)
{
	//���������Ϣ
	HKEY hKey = NULL;
	HKEY hSubKey = NULL;
	LONG lRet;
	CString strBuffer;
	CString strMidReg;

	DWORD dwIndex = 0;
	DWORD dwSoftSize = 0;
	WCHAR szKeyName[MAX_PATH] = { 0 };
	WCHAR szBuffer[MAX_PATH] = { 0 };
	DWORD dwKeyLen = MAXBYTE;
	DWORD dwNameLen = MAXBYTE;
	DWORD dwType = REG_DWORD | REG_EXPAND_SZ | REG_SZ;

	//���Ӽ�
	lRet = RegOpenKeyEx(m_hRootKey, SubKey, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);

	if (lRet == ERROR_SUCCESS)
	{
		//ö�����Ŀ¼�Ӽ�
		while (ERROR_NO_MORE_ITEMS != RegEnumKeyEx(hKey, dwIndex, szKeyName, &dwKeyLen, 0, NULL, NULL, NULL))
		{

			strBuffer.Format(L"%s", szKeyName);
			if (!strBuffer.IsEmpty())
			{
				//��ʽ��ƴ���Ӽ�
				strMidReg = SubKey + L"\\" + strBuffer;
				//�򿪵�������Ӽ�
				if (ERROR_SUCCESS == RegOpenKeyEx(m_hRootKey, strMidReg, 0, KEY_READ | KEY_WOW64_64KEY, &hSubKey))
				{
					//��ȡ��Ϣ
					SOFTINFO1 SoftInfo = { 0 };

					//����
					RegQueryValueEx(hSubKey, m_KeyName, 0, &dwType, (LPBYTE)SoftInfo.szSoftName, &dwNameLen);
					dwNameLen = MAXBYTE;	//��Ҫ�ָ���ֵ

											//����
					RegQueryValueEx(hSubKey, m_KeyNamePub, 0, &dwType, (LPBYTE)SoftInfo.szSoftPub, &dwNameLen);
					dwNameLen = MAXBYTE;

					//����
					RegQueryValueEx(hSubKey, m_KeyNameDate, 0, &dwType, (LPBYTE)SoftInfo.szSoftDate, &dwNameLen);
					dwNameLen = MAXBYTE;

					//�汾
					RegQueryValueEx(hSubKey, m_KeyNameVer, 0, &dwType, (LPBYTE)SoftInfo.szSoftVer, &dwNameLen);
					dwNameLen = MAXBYTE;

					//��װ·��
					RegQueryValueEx(hSubKey, m_KeyNameInsPath, 0, &dwType, (LPBYTE)SoftInfo.strSoftInsPath, &dwNameLen);
					dwNameLen = MAXBYTE;

					//ж��·��
					RegQueryValueEx(hSubKey, m_KeyNameUniPath, 0, &dwType, (LPBYTE)SoftInfo.strSoftUniPath, &dwNameLen);
					dwNameLen = MAXBYTE;

					//��СΪDWORDֵ
					RegQueryValueEx(hSubKey, m_KeyNameSize, 0, &dwType, (LPBYTE)&dwSoftSize, &dwNameLen);
					dwNameLen = MAXBYTE;

					//ת���ļ���С
					CString strSize;
					if (dwSoftSize > 0 && dwSoftSize < 1024)
					{
						strSize.Format(L"%dKB", dwSoftSize);
					}
					else if (dwSoftSize > 1024 && dwSoftSize < 1024 * 1024)
					{
						strSize.Format(L"%dMB", dwSoftSize / 1024);
					}
					else if (dwSoftSize > 1024 * 1024)
					{
						strSize.Format(L"%dGB", dwSoftSize / 1024 / 1024);
					}
					else
					{
						strSize = L" ";
					}
					memcpy_s(SoftInfo.szSoftSize, (strSize.GetLength() + 1) * sizeof(WCHAR),
						strSize.GetBuffer(), (strSize.GetLength() + 1) * sizeof(WCHAR));

					CString strName = SoftInfo.szSoftName;
					if (!strName.IsEmpty())
					{
						m_vecSoftInfo.push_back(SoftInfo);
					}
				}
				dwIndex++;
				dwKeyLen = MAXBYTE;
				dwNameLen = MAXBYTE;
				dwSoftSize = 0;
				ZeroMemory(szKeyName, MAX_PATH);
			}

		}

	}

}

//��ȡ�����Ϣ
void CSoftMgr::UpdateSoftInfo()
{
	m_list_soft.DeleteAllItems();
	m_vecSoftInfo.clear();

	GetSoftwareInfo(m_SubKey1);

	GetSoftwareInfo(m_SubKey2);


	for (size_t i = 0; i < m_vecSoftInfo.size(); i++)
	{
		m_SoftName = m_vecSoftInfo[i].szSoftName;
		m_SoftPub = m_vecSoftInfo[i].szSoftPub;
		m_SoftDate = m_vecSoftInfo[i].szSoftDate;
		m_SoftSize = m_vecSoftInfo[i].szSoftSize;
		m_SoftVer = m_vecSoftInfo[i].szSoftVer;
		m_SoftInsPath = m_vecSoftInfo[i].strSoftInsPath;
		m_SoftUniPath = m_vecSoftInfo[i].strSoftUniPath;


		m_list_soft.AddItem(0, 7,
			m_SoftName,
			m_SoftPub,
			m_SoftDate,
			m_SoftSize,
			m_SoftVer,
			m_SoftInsPath,
			m_SoftUniPath);
	}

}

//���ļ���
void CSoftMgr::OpenExploer()
{
	//��ȡ·��
	if (m_SoftInsPath.IsEmpty())
	{
		MessageBox(L"��·��ʧ�ܣ�");
	}
	else
	{
		ShellExecute(NULL, _T("open"), _T("explorer.exe"), m_SoftInsPath, NULL, SW_SHOWNORMAL);
	}
	m_SoftInsPath = L"";
}

//ж�����
void CSoftMgr::UnistallSoft()
{
	//��ȡѡ����

	if (m_SoftUniPath.IsEmpty())
	{
		MessageBox(L"ж��ʧ�ܣ�");
		return;
	}
	else
	{
		ShellExecute(NULL, _T("open"), m_SoftUniPath, NULL, NULL, SW_SHOW);
	}
	m_SoftUniPath = L"";
}


//�Ҽ��б�����˵�
void CSoftMgr::OnNMRClickListSoftware(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU2);
	pmenu = menu.GetSubMenu(1);//ָ�������˵��еĵ����˵���λ�á� λ��ֵ��0��ʼ��һ���˵�� 
	ClientToScreen(&pNMItemActivate->ptAction);//�ڿͻ���������ָʾ�����¼���λ��
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);

	
	
 	if (pNMItemActivate->iItem != -1)
 	{
		m_SoftInsPath = m_list_soft .GetItemText(pNMItemActivate->iItem, 5);//GetItemText:1.������ 2.�������󡣷���һ��CString���ı�
		m_SoftUniPath = m_list_soft.GetItemText(pNMItemActivate->iItem, 6);//GetItemText:1.������ 2.�������󡣷���һ��CString���ı�

	}

 	
	//�˵����֧
	switch (Select)
	{
	case ID_32799:	//ж��
	{
		UnistallSoft();
	}
	break;
	case ID_32801:	//ˢ��
	{
		UpdateSoftInfo();
	}
	break;
	case ID_32802:	//���ļ���
	{
		OpenExploer();
	}
	break;
	default:
		break;
	}
}
