// Boot.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Boot.h"
#include "afxdialogex.h"


// CBoot 对话框

IMPLEMENT_DYNAMIC(CBoot, CDialogEx)

CBoot::CBoot(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BOOT, pParent)
{

	//需要遍历的路径
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


// CBoot 消息处理程序


void CBoot::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL CBoot::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list_boot.AddColumn(4, L"名称", 100, L"键值", 250, L"主键", 160, L"子键", 350);
	//获取数据
	UpDataBoot();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



//获取单个子键数据
void CBoot::GetBootInfo(HKEY hRootKey, CString Path)
{
	//判断根键
	CString strRoot;
	if (hRootKey == HKEY_LOCAL_MACHINE)
	{
		strRoot = m_Root1;
	}
	else
	{
		strRoot = m_Root2;
	}

	//打开键
	HKEY hKey = NULL;
	if (ERROR_SUCCESS != RegOpenKeyEx(hRootKey, Path, 0,
		KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_WOW64_64KEY, &hKey))
	{
		return;
	}

	//查询类型
	int i = 0;
	DWORD dwType = REG_SZ | REG_EXPAND_SZ;
	//循环获取

	while (true)
	{
		DWORD dwBuffSize = MAXBYTE;
		DWORD dwKeySize = MAXBYTE;
		WCHAR szValueNme[MAXBYTE] = { 0 };
		WCHAR szValueKey[MAXBYTE] = { 0 };

		//枚举键名
		int rect = RegEnumValue(hKey, i, szValueNme, &dwBuffSize,
			NULL, &dwType, (LPBYTE)szValueKey, &dwKeySize);

		//查询键值
		int rect2 = RegQueryValueEx(hKey, szValueNme, 0,
			&dwType, (LPBYTE)szValueKey, &dwKeySize);

		//完毕结束
		if (rect == ERROR_NO_MORE_ITEMS)
		{
			break;
		}

		//输出
		m_Name = szValueNme;
		m_Key = szValueKey;
		m_list_boot.AddItem(0, 4, m_Name, m_Key, strRoot, Path);

		//恢复数据
		i++;

	}
	//完毕键
	RegCloseKey(hKey);
}


//删除启动项
void CBoot::DeleBoot()
{
	//获取选中项
	int nSel = m_list_boot.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
	}
	//获取路径
	CString strName = m_list_boot.GetItemText(nSel, 0);
	CString strRoot = m_list_boot.GetItemText(nSel, 2);
	CString strSub = m_list_boot.GetItemText(nSel, 3);
	HKEY hRoot;

	//获取主键
	if (strRoot == m_Root1)
	{
		hRoot = m_hRootKey1;
	}
	else
	{
		hRoot = m_hRootKey2;
	}

	//临时数据
	DWORD dwType = 0;
	DWORD dwBuffSize = MAXBYTE;
	DWORD dwKeySize = MAXBYTE;
	WCHAR szValueNme[MAXBYTE] = { 0 };
	WCHAR szValueKey[MAXBYTE] = { 0 };
	HKEY hKey = NULL;

	//打开键，此处需要设置权限
	if (RegOpenKeyEx(hRoot, strSub, 0,
		KEY_SET_VALUE | KEY_WOW64_64KEY, &hKey) != ERROR_SUCCESS)
	{
		return;
	}

	RegDeleteValue(hKey, strName);

	UpDataBoot();
}

//获取数据
void CBoot::UpDataBoot()
{
	m_list_boot.DeleteAllItems();

	//主键子键组合获取启动项
	GetBootInfo(m_hRootKey1, m_SubKey1);

	GetBootInfo(m_hRootKey1, m_SubKey2);

	GetBootInfo(m_hRootKey2, m_SubKey1);
}

//添加启动项
void CBoot::AddBoot()
{
	CFileDialog dlg(TRUE, _T("exe"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("可执行程序|*.exe|所有文件|*||"));
	if (dlg.DoModal() == IDOK) {

		m_Key = dlg.GetPathName();

		//获取文件名
		int nPos = m_Key.ReverseFind(L'\\');
		m_Name = m_Key.Right(m_Key.GetLength() - nPos - 1);

		HKEY hKey = NULL;

		//创建注册表键值
		RegCreateKeyEx(m_hRootKey1, m_SubKey1, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

		if (ERROR_SUCCESS == RegSetValueEx(hKey, m_Name, 0, REG_SZ,
			(LPBYTE)m_Key.GetBuffer(), (m_Key.GetLength() + 1) * sizeof(TCHAR)))
		{
			MessageBox(L"添加成功！");
		}
		else
		{
			MessageBox(L"添加失败！");
		}
		RegCloseKey(hKey);

		//刷新
		UpDataBoot();
	}
}


//右键打开菜单
void CBoot::OnNMRClickListBoot(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU2);
	pmenu = menu.GetSubMenu(2);//指定包含菜单中的弹出菜单的位置。 位置值从0开始第一个菜单项。 
	ClientToScreen(&pNMItemActivate->ptAction);//在客户端坐标中指示发生事件的位置
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);



// 	if (pNMItemActivate->iItem != -1)
// 	{
// 		//m_SoftInsPath = m_list_soft.GetItemText(pNMItemActivate->iItem, 5);//GetItemText:1.索引， 2.检索对象。返回一个CString的文本
// 		//m_SoftUniPath = m_list_soft.GetItemText(pNMItemActivate->iItem, 6);//GetItemText:1.索引， 2.检索对象。返回一个CString的文本
// 
// 	}


	//菜单项分支
	switch (Select)
	{
	case ID_32803:	//添加
	{
		AddBoot();
	}
	break;
	case ID_32804:	//禁止
	{
		DeleBoot();
	}
	break;
	case ID_32805:	//刷新
	{
		UpDataBoot();
	}
	break;
	default:
		break;
	}
}
