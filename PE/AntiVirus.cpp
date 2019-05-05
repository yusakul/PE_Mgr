// AntiVirus.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "AntiVirus.h"
#include "afxdialogex.h"


// CAntiVirus 对话框

IMPLEMENT_DYNAMIC(CAntiVirus, CDialogEx)

CAntiVirus::CAntiVirus(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ANTI_VIRUS, pParent)
{

}

CAntiVirus::~CAntiVirus()
{
}

void CAntiVirus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAntiVirus, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_FASTANTIVIRUS, &CAntiVirus::OnBnClickedButtonFastantivirus)
	ON_BN_CLICKED(IDC_BUTTON_ALLANTIVIRUS, &CAntiVirus::OnBnClickedButtonAllantivirus)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOMANTIVIRUS, &CAntiVirus::OnBnClickedButtonCustomantivirus)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOMFILE, &CAntiVirus::OnBnClickedButtonCustomfile)
	
	ON_BN_CLICKED(IDC_BUTTON4, &CAntiVirus::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CAntiVirus::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON_DEL_VIRUS, &CAntiVirus::OnBnClickedButtonnote)
END_MESSAGE_MAP()


// CAntiVirus 消息处理程序

BOOL CAntiVirus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



//快速杀毒
void CAntiVirus::OnBnClickedButtonFastantivirus()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePath = L"C:\\Windows";
	m_scantype = 0;

	static	CAntiVirusScan AntiVirusScan(FilePath, m_scantype);
	AntiVirusScan.Create(IDD_DIALOG_ANTI_VIRUS_SCAN, 0);
	AntiVirusScan.ShowWindow(SW_SHOWNORMAL);
}

//全盘杀毒
void CAntiVirus::OnBnClickedButtonAllantivirus()
{
	// TODO: 在此添加控件通知处理程序代码
	CString FilePath = L"D:\\";
	m_scantype = 1;
	static	CAntiVirusScan AntiVirusScan(FilePath, m_scantype);
	AntiVirusScan.Create(IDD_DIALOG_ANTI_VIRUS_SCAN, 0);
	AntiVirusScan.ShowWindow(SW_SHOW);
}

//自定义文件杀毒
void CAntiVirus::OnBnClickedButtonCustomfile()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("所有文件(*.*)\0*.*\0Exe文件(*.exe)\0*.exe\0");//要选择的文件后缀   
	ofn.lpstrInitialDir = _T("D:\\Program Files");//默认的文件路径   
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT  
	BOOL bSel = GetOpenFileName(&ofn);

	//点了取消，或者无效文件夹返回NULL  
	if (bSel == NULL)
		return;


	CString FilePath = szBuffer;
	m_scantype = 2;
	static	CAntiVirusScan AntiVirusScan(FilePath, m_scantype);
	AntiVirusScan.Create(IDD_DIALOG_ANTI_VIRUS_SCAN, 0);
	AntiVirusScan.ShowWindow(SW_SHOW);
	
}


//自定义位置杀毒
void CAntiVirus::OnBnClickedButtonCustomantivirus()
{
	// TODO: 在此添加控件通知处理程序代码

	//文件夹路径缓存
	TCHAR szDir[MAX_PATH];

	//必须初始化
	BROWSEINFO bfi = {};		//浏览信息
	ITEMIDLIST *pidl;			//文件夹标识符列表
	bfi.hwndOwner = this->m_hWnd;
	bfi.pszDisplayName = szDir;
	bfi.lpszTitle = _T("请选择文件夹");
	bfi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	pidl = SHBrowseForFolder(&bfi); //返回一个PIDL，它指定所选文件夹相对于命名空间根目录的位置。如果用户在对话框中选择取消按钮，则返回值为NULL。

	//点了取消，或者无效文件夹返回NULL  
	if (pidl == NULL)
		return;

	//获取文件夹路径
	if (SHGetPathFromIDList(pidl, szDir))
	{
		CString FilePath = szDir;
		m_scantype = 3;
		static	CAntiVirusScan AntiVirusScan(FilePath, m_scantype);
		AntiVirusScan.Create(IDD_DIALOG_ANTI_VIRUS_SCAN, 0);
		AntiVirusScan.ShowWindow(SW_SHOW);
	}
}





void CAntiVirus::OnBnClickedButtonMd5()
{
	// TODO: 在此添加控件通知处理程序代码

}



void CAntiVirus::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(L"正在施工");
}


void CAntiVirus::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(L"正在施工");

}


void CAntiVirus::OnBnClickedButtonnote()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(L"正在施工");

}
