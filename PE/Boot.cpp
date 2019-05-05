// Boot.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Boot.h"
#include "afxdialogex.h"


// CBoot �Ի���

IMPLEMENT_DYNAMIC(CBoot, CDialogEx)

CBoot::CBoot(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BOOT, pParent)
{

	//��Ҫ������·��
	m_SubKey1 = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	m_SubKey2 = L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Run";
	m_Root1 = L"HKEY_LOCAL_MACHINE";
	m_Root2 = L"HKEY_CURRENT_USER";
	m_hRootKey1 = HKEY_LOCAL_MACHINE;
	m_hRootKey2 = HKEY_CURRENT_USER;
}

CBoot::~CBoot()
{
}

void CBoot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOOT, m_list_boot);
}


BEGIN_MESSAGE_MAP(CBoot, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_BOOT, &CBoot::OnNMRClickListBoot)
END_MESSAGE_MAP()


// CBoot ��Ϣ�������


void CBoot::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


BOOL CBoot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_list_boot.AddColumn(4, L"����", 100, L"��ֵ", 250, L"����", 160, L"�Ӽ�", 350);
	//��ȡ����
	UpDataBoot();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



//��ȡ�����Ӽ�����
void CBoot::GetBootInfo(HKEY hRootKey, CString Path)
{
	//�жϸ���
	CString strRoot;
	if (hRootKey == HKEY_LOCAL_MACHINE)
	{
		strRoot = m_Root1;
	}
	else
	{
		strRoot = m_Root2;
	}

	//�򿪼�
	HKEY hKey = NULL;
	if (ERROR_SUCCESS != RegOpenKeyEx(hRootKey, Path, 0,
		KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_WOW64_64KEY, &hKey))
	{
		return;
	}

	//��ѯ����
	int i = 0;
	DWORD dwType = REG_SZ | REG_EXPAND_SZ;
	//ѭ����ȡ

	while (true)
	{
		DWORD dwBuffSize = MAXBYTE;
		DWORD dwKeySize = MAXBYTE;
		WCHAR szValueNme[MAXBYTE] = { 0 };
		WCHAR szValueKey[MAXBYTE] = { 0 };

		//ö�ټ���
		int rect = RegEnumValue(hKey, i, szValueNme, &dwBuffSize,
			NULL, &dwType, (LPBYTE)szValueKey, &dwKeySize);

		//��ѯ��ֵ
		int rect2 = RegQueryValueEx(hKey, szValueNme, 0,
			&dwType, (LPBYTE)szValueKey, &dwKeySize);

		//��Ͻ���
		if (rect == ERROR_NO_MORE_ITEMS)
		{
			break;
		}

		//���
		m_Name = szValueNme;
		m_Key = szValueKey;
		m_list_boot.AddItem(0, 4, m_Name, m_Key, strRoot, Path);

		//�ָ�����
		i++;

	}
	//��ϼ�
	RegCloseKey(hKey);
}


//ɾ��������
void CBoot::DeleBoot()
{
	//��ȡѡ����
	int nSel = m_list_boot.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"��ѡ������");
	}
	//��ȡ·��
	CString strName = m_list_boot.GetItemText(nSel, 0);
	CString strRoot = m_list_boot.GetItemText(nSel, 2);
	CString strSub = m_list_boot.GetItemText(nSel, 3);
	HKEY hRoot;

	//��ȡ����
	if (strRoot == m_Root1)
	{
		hRoot = m_hRootKey1;
	}
	else
	{
		hRoot = m_hRootKey2;
	}

	//��ʱ����
	DWORD dwType = 0;
	DWORD dwBuffSize = MAXBYTE;
	DWORD dwKeySize = MAXBYTE;
	WCHAR szValueNme[MAXBYTE] = { 0 };
	WCHAR szValueKey[MAXBYTE] = { 0 };
	HKEY hKey = NULL;

	//�򿪼����˴���Ҫ����Ȩ��
	if (RegOpenKeyEx(hRoot, strSub, 0,
		KEY_SET_VALUE | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
	{
		return;
	}

	RegDeleteValue(hKey, strName);

	UpDataBoot();
}

//��ȡ����
void CBoot::UpDataBoot()
{
	m_list_boot.DeleteAllItems();

	//�����Ӽ���ϻ�ȡ������
	GetBootInfo(m_hRootKey1, m_SubKey1);

	GetBootInfo(m_hRootKey1, m_SubKey2);

	GetBootInfo(m_hRootKey2, m_SubKey1);
}

//���������
void CBoot::AddBoot()
{
	CFileDialog dlg(TRUE, _T("exe"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("��ִ�г���|*.exe|�����ļ�|*||"));
	if (dlg.DoModal() == IDOK) {

		m_Key = dlg.GetPathName();

		//��ȡ�ļ���
		int nPos = m_Key.ReverseFind(L'\\');
		m_Name = m_Key.Right(m_Key.GetLength() - nPos - 1);

		HKEY hKey = NULL;

		//����ע����ֵ
		RegCreateKeyEx(m_hRootKey1, m_SubKey1, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

		if (ERROR_SUCCESS == RegSetValueEx(hKey, m_Name, 0, REG_SZ,
			(LPBYTE)m_Key.GetBuffer(), (m_Key.GetLength() + 1) * sizeof(TCHAR)))
		{
			MessageBox(L"��ӳɹ���");
		}
		else
		{
			MessageBox(L"���ʧ�ܣ�");
		}
		RegCloseKey(hKey);

		//ˢ��
		UpDataBoot();
	}
}


//�Ҽ��򿪲˵�
void CBoot::OnNMRClickListBoot(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;


	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU2);
	pmenu = menu.GetSubMenu(2);//ָ�������˵��еĵ����˵���λ�á� λ��ֵ��0��ʼ��һ���˵�� 
	ClientToScreen(&pNMItemActivate->ptAction);//�ڿͻ���������ָʾ�����¼���λ��
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);



// 	if (pNMItemActivate->iItem != -1)
// 	{
// 		//m_SoftInsPath = m_list_soft.GetItemText(pNMItemActivate->iItem, 5);//GetItemText:1.������ 2.�������󡣷���һ��CString���ı�
// 		//m_SoftUniPath = m_list_soft.GetItemText(pNMItemActivate->iItem, 6);//GetItemText:1.������ 2.�������󡣷���һ��CString���ı�
// 
// 	}


	//�˵����֧
	switch (Select)
	{
	case ID_32803:	//���
	{
		AddBoot();
	}
	break;
	case ID_32804:	//��ֹ
	{
		DeleBoot();
	}
	break;
	case ID_32805:	//ˢ��
	{
		UpDataBoot();
	}
	break;
	default:
		break;
	}
}
