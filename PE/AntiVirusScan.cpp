// AntiVirusScan.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "AntiVirusScan.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "datatype.h"


// CAntiVirusScan 对话框

IMPLEMENT_DYNAMIC(CAntiVirusScan, CDialogEx)

CAntiVirusScan::CAntiVirusScan(CString FilePath, DWORD scantype, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ANTI_VIRUS_SCAN, pParent)
	, m_IsPause(false)
	, m_IsCancle(false)
{
	m_scantype = scantype;		//扫描类型
	m_FilePath = FilePath;		//文件路径
	m_FolderPath = FilePath;	//文件夹路径
	
}
CAntiVirusScan::CAntiVirusScan(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ANTI_VIRUS_SCAN, pParent)
	, m_IsPause(false)
	, m_IsCancle(false)
{
}

CAntiVirusScan::~CAntiVirusScan()
{
}

void CAntiVirusScan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ANTI_VIRUS_SCAN, m_list_AntivirusScan);
	DDX_Control(pDX, IDC_STATIC_SCANINFO, m_TextScanInfo);
	DDX_Control(pDX, IDC_STATIC_LINKINFO, m_TextCloud);
}


BEGIN_MESSAGE_MAP(CAntiVirusScan, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CAntiVirusScan::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_CANCELSCAN, &CAntiVirusScan::OnBnClickedButtonCancelscan)
	ON_BN_CLICKED(IDC_BUTTON_DEL_VIRUS, &CAntiVirusScan::OnBnClickedButtonDelVirus)

	ON_NOTIFY(NM_RCLICK, IDC_LIST_ANTI_VIRUS_SCAN, &CAntiVirusScan::OnNMRClickListAntiVirusScan)
END_MESSAGE_MAP()


// CAntiVirusScan 消息处理程序


void CAntiVirusScan::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_IsCancle == false)
	{
		MessageBox(L"请先取消扫描...", 0, MB_OK);
	}
	else if(m_IsCancle == true)
	{
		DestroyWindow();
		CDialogEx::OnClose();
	}
	
}


BOOL CAntiVirusScan::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_list_AntivirusScan.AddColumn(3, L"路径", 350, L"MD5", 250, L"类型", 100);
	
	//创建线程从服务端获取数据
	LinkServer();

	//读取数据库
	ReadDataBase();


	//创建扫描线程
	hScanThread = AfxBeginThread(MyThreadScan, this);
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



//扫描方式
void CAntiVirusScan::AntiScan()
{
	m_IsCancle = false;
	//根据扫描类型分发
	switch (m_scantype)
	{
	case 0:
		//快速扫描
		FastScan();
		break;
	case 1:
		//全盘扫描
		ScanAll();
		break;
	case 2:
		//文件扫描
		ScanFile();
		break;
	case 3:
		//文件夹扫描
		ScanFloder(m_FolderPath);
	default:
		break;
	}
	m_TextScanInfo.SetWindowTextW(L"扫描完成");
	MessageBox(L"扫描结束！");
}


//扫描单个文件
void CAntiVirusScan::ScanFile()
{
	//转换文件名;
	CStringA FinalFileName = CW2A(m_FilePath.GetBuffer(), CP_THREAD_ACP);

	//获取MD5
	char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
	m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

	m_TextScanInfo.SetWindowTextW(L"扫描完成");
	//对比MD5
	if (DataBaseCMP(m_ScanMd5))
	{
		m_vecFilePath.push_back(m_FilePath);
		m_list_AntivirusScan.AddItem(0,3, m_FilePath, m_ScanMd5, L"危险文件");
		MessageBox(L"危险文件!\r\n请处理！");
	}
	else
	{
		m_list_AntivirusScan.AddItem(0, 3, m_FilePath, m_ScanMd5, L"安全文件");
		MessageBox(L"安全文件!");
	}
}


//扫描文件夹
void CAntiVirusScan::ScanFloder(CString FloderPath)
{
	//取消任务
	if (m_IsCancle)
	{
		//结束进程

		return;
	}

	//暂停任务
	while (m_IsPause)
	{
	}

	//空路径退出;
	if (FloderPath.IsEmpty())
	{
		m_IsCancle = true;
		return;
	}

	//打开夹;
	WIN32_FIND_DATA wsd = {};
	HANDLE hFind = FindFirstFile(FloderPath, &wsd);
	FloderPath += L"\\*";
	hFind = FindFirstFile(FloderPath, &wsd);

	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		//过滤本级和上级;
		if (!wcscmp(wsd.cFileName, L".")
			|| !wcscmp(wsd.cFileName, L".."))
		{
			continue;
		}
		//递归遍历文件夹;
		else if (wsd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString NextDir = FloderPath.Left(FloderPath.GetLength() - 1);
			NextDir += wsd.cFileName;
			ScanFloder(NextDir);
		}
	
		//拼接文件名;
		CString m_ScanFileFath = FloderPath.Left(FloderPath.GetLength() - 1);
		m_ScanFileFath += wsd.cFileName;
		CStringA FinalFileName = CW2A(m_ScanFileFath.GetBuffer(), CP_THREAD_ACP);

		//获取MD5
		if (wsd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	//排除文件夹
		{
			continue;
		}
		char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
		m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

		//更新扫描状态
		CString strScanInfo = L"正在扫描...";
		strScanInfo += m_ScanFileFath;
		m_TextScanInfo.SetWindowTextW(strScanInfo);

		//MD5值 对比 本地数据库、白名单、黑名单
		if (DataBaseCMP(m_ScanMd5))
		{
			m_vecFilePath.push_back(m_ScanFileFath);
			m_list_AntivirusScan.AddItem(0 ,3, m_ScanFileFath, m_ScanMd5, L"危险文件");
		}


	} while (FindNextFile(hFind, &wsd));

	FindClose(hFind);

	//更新扫描状态
	m_TextScanInfo.SetWindowTextW(L"扫描完成");
	m_IsCancle = true;
	return;
}

//全盘扫描
void CAntiVirusScan::ScanAll()
{
	//获取盘符;
	TCHAR szBuf[1024] = {};
	GetLogicalDriveStrings(1024, szBuf);
	TCHAR *pstr = szBuf;

	while (*pstr)
	{
		m_vecDisk.push_back(pstr);
		//获取下一个盘符;
		pstr += _tcslen(pstr) + 1;
	}

	//扫描进程	
	void ScanProcess();

	//循环遍历盘符
	for (DWORD i = 0; i < m_vecDisk.size(); i++)
	{
		ScanFloder(m_vecDisk[i]);
	}
}

//扫描进程	
void CAntiVirusScan::ScanProcess()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"创建进程快照失败！");
		return;
	}

	//初始化快照信息结构体;
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	if (!Process32First(hSnapShot, &pi))
	{
		MessageBox(L"获取进程信息失败");
		return;
	}
	do
	{
		TCHAR *szBuf[5] = { new TCHAR[MAX_PATH], new TCHAR[6],  new TCHAR[6],  new TCHAR[6], new TCHAR[MAX_PATH], };
		szBuf[0] = pi.szExeFile;

		//打开要获取路径的进程
		HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
			| PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pi.th32ProcessID);
		//获取进程完整路径;
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);

		}
		else
		{
			szBuf[4] = L"无权访问";
		}

		CString  FileFathW = szBuf[4];
		CStringA FinalFileName = CW2A(szBuf[4], CP_THREAD_ACP);
		//获取MD5
		char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
		m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

		CString strScanInfo = L"正在扫描进程：";
		strScanInfo += FileFathW;
		m_TextScanInfo.SetWindowTextW(strScanInfo);

		//对比MD5
		if (DataBaseCMP(m_ScanMd5))
		{

			if (TerminateProcess(hPro, 0))
			{
				MessageBox(L"发现可疑进程!\r\n已结束进程\r\n");
			}
			else
			{
				MessageBox(L"发现可疑进程!\r\n处理失败!\r\n请手动处理！");
			}
			m_vecFilePath.push_back(FileFathW);
			m_list_AntivirusScan.AddItem(0, 3, FileFathW, m_ScanMd5, L"危险进程");
		}
		CloseHandle(hPro);

	} while (Process32Next(hSnapShot, &pi));
}

//快速扫描
void CAntiVirusScan::FastScan()
{
	//扫描进程
	ScanProcess();
	//扫描系统文件夹
	ScanFloder(m_FolderPath);

	m_TextScanInfo.SetWindowTextW(L"扫描完成");
}




//读取本地数据库
void CAntiVirusScan::ReadDataBase()
{
	//读取病毒库
	CString strDatabase = szFileName[0];
	CStdioFile sfDatabase;
	if (sfDatabase.Open(strDatabase, CFile::modeRead))
	{
		CString temp;
		//循环读取
		while (sfDatabase.ReadString(temp))
		{
			m_vecVirusDatabase.push_back(temp);
		}
		sfDatabase.Close();
	}
	else
	{
		MessageBox(L"读取本地病毒库失败！");
		m_dwScanType = 4;
		return;
	}

	//读取白名单
	CString strWhite = szFileName[1];
	CStdioFile sfWhite;
	if (sfWhite.Open(strWhite, CFile::modeRead))
	{
		CString temp;
		//循环读取
		while (sfWhite.ReadString(temp))
		{
			m_vecWhite.push_back(temp);
		}
		sfWhite.Close();
	}

	//读取黑名单
	CString strBlack = szFileName[2];
	CStdioFile sfBlack;
	if (sfBlack.Open(strBlack, CFile::modeRead))
	{
		CString temp;
		//循环读取
		while (sfBlack.ReadString(temp))
		{
			m_vecBlack.push_back(temp);
		}
		sfBlack.Close();
	}
}


//对比本地数据库
bool CAntiVirusScan::DataBaseCMP(CString strMD5)
{
	//对比白名单
	for (DWORD i = 0; i < m_vecWhite.size(); i++)
	{
		if (strMD5 == m_vecWhite[i])
		{
			return false;
		}
	}

	//对比黑名单
	for (DWORD i = 0; i < m_vecBlack.size(); i++)
	{
		if (strMD5 == m_vecBlack[i])
		{
			return true;
		}
	}

	//对比病毒库
	for (DWORD i = 0; i < m_vecVirusDatabase.size(); i++)
	{
		if (strMD5 == m_vecVirusDatabase[i])
		{
			return true;
		}
	}
	return false;
}





//暂停扫描
void CAntiVirusScan::OnBnClickedButtonPause()
{
	// TODO: 在此添加控件通知处理程序代码
	//任务未开始不能暂停


	m_IsPause = !m_IsPause;

	//更改按钮状态
	if (m_IsPause)
	{
		GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowTextW(L"继续扫描");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowTextW(L"暂停扫描");
	}
}

//取消扫描
void CAntiVirusScan::OnBnClickedButtonCancelscan()
{
	// TODO: 在此添加控件通知处理程序代码
	m_IsCancle = true;
	GetDlgItem(IDC_BUTTON_CANCELSCAN)->SetWindowTextW(L"扫描已取消");
}

//删除病毒
void CAntiVirusScan::OnBnClickedButtonDelVirus()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_vecFilePath.size() == 0)
	{
		MessageBox(L"暂无可处理项目！");
		return;
	}

	//删除病毒
	for (DWORD i = 0; i < m_vecFilePath.size(); i++)
	{
		DeleteFile(m_vecFilePath[i]);
	}
	m_list_AntivirusScan.DeleteAllItems();
	MessageBox(L"处理完成！");
}




//扫描线程回调函数
UINT CAntiVirusScan::MyThreadScan(LPVOID lpParam)
{
	CAntiVirusScan* pAntiScan = (CAntiVirusScan*)lpParam;

	pAntiScan->AntiScan();

	pAntiScan->hScanThread = NULL;

	return 0;
}

//服务端===================================================================================

//获取服务端数据线程
void CAntiVirusScan::LinkServer()
{
	CAntiVirusScan Antivirus;

	if (!(Antivirus.m_Socket.ConnectServer("127.0.0.1", 1234)))
	{
		m_TextCloud.SetWindowText(L"未连接服务端");
		Antivirus.m_Socket.Close();
		return;
	}

	if (!Antivirus.UpdataDataBase(3))	//更新黑名单
	{
		Antivirus.m_Socket.Close();
		m_TextCloud.SetWindowTextW(L"更新黑名单失败");
		return;
	}
	Sleep(100);
	if (!Antivirus.UpdataDataBase(2))		//更新白名单
	{
		Antivirus.m_Socket.Close();
		m_TextCloud.SetWindowTextW(L"更新白名单失败");
		return;
	}
	Sleep(100);
	if (!Antivirus.UpdataDataBase(1))		//更新本地病毒库
	{
		Antivirus.m_Socket.Close();
		m_TextCloud.SetWindowTextW(L"更新病毒库失败");
		return;
	}
/*
	if (!Antivirus.UpdataDataBase(5))		//提交md5至服务器
	{
		Antivirus.m_Socket.Close();
		m_TextCloud.SetWindowTextW(L"上传病毒库失败");
		return;
	}*/
	Antivirus.m_Socket.Close();
	m_TextCloud.SetWindowTextW(L"连接云服务端成功！");
	return;
}


//更新数据库
bool CAntiVirusScan::UpdataDataBase(DWORD dwType)	//文件类型dwType    1：更新病毒库  2：更新白名单 3：更新黑名单 4：查询md5  5：提交病毒md5
{

	//发送请求；dwType类型固定，内容随便写 
	m_Socket.Send((DATAPURPOSE)dwType, "cloud", dwType);

	//接收消息;
	CHATSEND ct = {};
	//循环接收
	while (true)
	{
		if (SOCKET_ERROR == recv(m_Socket.m_sClient, (char*)&ct, sizeof(CHATSEND), NULL))
		{
			return false;
		}
		//接收结束标志
		if (!strcmp(ct.m_content.buf, "~~~end~~~"))
		{
			break;
		}
		m_MD5 = ct.m_content.buf;
		m_vecMD5.push_back(m_MD5);
	}

	//写入文件
	CString sFile = szFileName[dwType];
	CStdioFile sfFile;
	if (sfFile.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
	{
		for (DWORD i = 0; i < m_vecMD5.size(); i++)
		{
			m_vecMD5[i] += L"\n\r";
			sfFile.WriteString(m_vecMD5[i]);
		}
		sfFile.Close();
	}

	return true;
}


//右键弹出菜单 加入白名单、黑名单
void CAntiVirusScan::OnNMRClickListAntiVirusScan(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;



	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU2);
	pmenu = menu.GetSubMenu(3);//指定包含菜单中的弹出菜单的位置。 位置值从0开始第一个菜单项。 
	ClientToScreen(&pNMItemActivate->ptAction);//在客户端坐标中指示发生事件的位置
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);

	//获取path和md5
	CString sPath;
	CString sMd5;
	if (pNMItemActivate->iItem != -1)
	{
		sPath = m_list_AntivirusScan.GetItemText(pNMItemActivate->iItem, 0);//GetItemText:1.索引， 2.检索对象。返回一个CString的文本
		sMd5 = m_list_AntivirusScan.GetItemText(pNMItemActivate->iItem, 1);//GetItemText:1.索引， 2.检索对象。返回一个CString的文本
	}

	//菜单项分支
	switch (Select)
	{
	case ID_32808:	//加入白名单列表
	{
		CString sFile = szFileName[1];
		CStdioFile sfFile;
		if (sfFile.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
		{
			sfFile.WriteString(sMd5); //写入一行文本

			for (DWORD i = 0; i < m_vecFilePath.size(); i++)		//从要删除的目录中去除
			{
				if (m_vecFilePath[i] == sPath)
				{
					m_vecFilePath.erase(m_vecFilePath.begin() + i);	//删除第i个
				}
			}
			sfFile.Close();
		}
	}
	break;
	case ID_32809:		//加入黑名单
	{
		CString sFile = szFileName[2];
		CStdioFile sfFile;
		if (sfFile.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
		{
			sfFile.WriteString(sMd5); //写入一行文本
			sfFile.Close();
		}
	}
	break;
	default:
		break;
	}
}
