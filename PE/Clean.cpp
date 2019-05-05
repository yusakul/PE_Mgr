// Clean.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "Clean.h"
#include "afxdialogex.h"
#include <strsafe.h>
#include <xutility>
#include <atlconv.h>


// CClean 对话框

IMPLEMENT_DYNAMIC(CClean, CDialogEx)

CClean::CClean(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CLEAN, pParent)
	, m_check_all(FALSE)
	, m_check_explore(FALSE)
	, m_check_recycle(FALSE)
	, m_check_report(FALSE)
	, m_check_sysTemp(FALSE)
	, m_check_userTemp(FALSE)
	, m_check_all2(TRUE)
	, m_check_ilk(TRUE)
	, m_check_log(TRUE)
	, m_check_obj(TRUE)
	, m_check_pch(TRUE)
	, m_check_pdb(TRUE)
	, m_check_tlog(TRUE)
	, m_check_ipch(TRUE)

	, m_check_test(FALSE)
{

}

CClean::~CClean()
{
}

void CClean::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLEAN, m_list_clean);
	DDX_Check(pDX, IDC_CHECK_ALL, m_check_all);
	DDX_Check(pDX, IDC_CHECK_Explore, m_check_explore);
	DDX_Check(pDX, IDC_CHECK_Recycle, m_check_recycle);
	DDX_Check(pDX, IDC_CHECK_Report, m_check_report);
	DDX_Check(pDX, IDC_CHECK_SysTemp, m_check_sysTemp);
	DDX_Check(pDX, IDC_CHECK_UserTemp, m_check_userTemp);
	DDX_Check(pDX, IDC_CHECK_ALL2, m_check_all2);
	DDX_Check(pDX, IDC_CHECK_ilk, m_check_ilk);
	DDX_Check(pDX, IDC_CHECK_log, m_check_log);
	DDX_Check(pDX, IDC_CHECK_obj, m_check_obj);
	DDX_Check(pDX, IDC_CHECK_pch, m_check_pch);
	DDX_Check(pDX, IDC_CHECK_pch, m_check_ipch);

	DDX_Check(pDX, IDC_CHECK_pdb, m_check_pdb);
	DDX_Check(pDX, IDC_CHECK_tlog, m_check_tlog);
	DDX_Check(pDX, IDC_CHECK_TEST, m_check_test);
}


BEGIN_MESSAGE_MAP(CClean, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_ALL, &CClean::OnBnClickedCheckAll)

	ON_BN_CLICKED(IDC_BUTTON_ONEKEYCLEAN, &CClean::OnBnClickedButtonOnekeyclean)
	ON_BN_CLICKED(IDC_CHECK_ALL2, &CClean::OnBnClickedCheckAll2)
	ON_BN_CLICKED(IDC_BUTTON_UserClean, &CClean::OnBnClickedButtonUserclean)
	ON_BN_CLICKED(IDC_BUTTON_SCANSys, &CClean::OnBnClickedButtonScansys)
	ON_WM_DROPFILES()
	ON_EN_CHANGE(IDC_MFCEDITBROWSE1, &CClean::OnEnChangeMfceditbrowse_sys)
	ON_EN_CHANGE(IDC_MFCEDITBROWSE2, &CClean::OnEnChangeMfceditbrowse2)
	ON_BN_CLICKED(IDC_BUTTON_SCANhouzhui, &CClean::OnBnClickedButtonScanhouzhui)
END_MESSAGE_MAP()


// CClean 消息处理程序


void CClean::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL CClean::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	m_list_clean.AddColumn(4, L"文件路径", 150, L"大小(KB)", 100,  L"创建时间",150, L"修改时间", 150);

	//定义路径
	m_Path_Sys = L"C:\\Windows\\Temp";
	m_Path_Test = L"C:\\Users\\Administrator\\Desktop\\test";

	m_Path_User = L"C:\\User\\Administrator\\AppData\\Local\\Temp";
	m_Path_Report = L"C:\\Users\\Administrator\\AppData\\Local\\Microsoft\\Windows\\WER\\ReportQueue";
	m_Path_IE = L"C:\\Users\\Administrator\\AppData\\Local\\Microsoft\\Windows\\INetCache";

	m_FilePath_Mfceditbrowse1 = L"";
	m_FilePath_Mfceditbrowse2 = L"";
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



//文件夹浏览控件——目录下所有文件
void CClean::OnEnChangeMfceditbrowse_sys()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	GetDlgItemText(IDC_MFCEDITBROWSE1, m_FilePath_Mfceditbrowse1);
	myScanDirectory(m_FilePath_Mfceditbrowse1);
}



//清理回收站
void CClean::CleanRecycle()
{
	//初始化SHQUERYRBINFO结构体
	SHQUERYRBINFO RecycleBinInfo = {};
	RecycleBinInfo.cbSize = sizeof(RecycleBinInfo);

	//查询回收站信息
	SHQueryRecycleBin(NULL, &RecycleBinInfo);

	//清空回收站
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}


//==========================================================

//文件夹浏览控件——目录下指定文件
void CClean::OnEnChangeMfceditbrowse2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_list_clean.DeleteAllItems();

	PCLEAMTYPE pCleanType = new CLEANTYPE;

	pCleanType->ilk = m_check_ilk;
	pCleanType->log = m_check_log;
	pCleanType->tlog = m_check_tlog;
	pCleanType->obj = m_check_obj;
	pCleanType->pdb = m_check_pdb;
	pCleanType->pch = m_check_pch;
	pCleanType->pch = m_check_ipch;


	GetDlgItemText(IDC_MFCEDITBROWSE2, m_FilePath_Mfceditbrowse2);	//获取路径
	EnumFoldAndFile(m_FilePath_Mfceditbrowse2, pCleanType);			//遍历并输出到列表
}


//自定义垃圾选择check事件
void CClean::OnBnClickedCheckAll2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_check_all2)
	{
		m_check_tlog = TRUE;
		m_check_obj = TRUE;
		m_check_log = TRUE;
		m_check_pch	= TRUE;
		m_check_ilk = TRUE;
		m_check_pdb = TRUE;
		m_check_ipch = TRUE;

	}
	else
	{
		m_check_tlog = FALSE;
		m_check_obj = FALSE;
		m_check_log = FALSE;
		m_check_pch = FALSE;
		m_check_ilk = FALSE;
		m_check_pdb = FALSE;
		m_check_ipch = FALSE;

	}

	UpdateData(FALSE);
}

//按钮——扫描指定后缀文件
void CClean::OnBnClickedButtonScanhouzhui()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!(lstrcmp(m_FilePath_Mfceditbrowse2, L"")))
	{
		MessageBox(L"没有选定文件夹");
		return;
	}
	UpdateData(TRUE);
	m_list_clean.DeleteAllItems();

	PCLEAMTYPE pCleanType = new CLEANTYPE;

	pCleanType->ilk = m_check_ilk;
	pCleanType->log = m_check_log;
	pCleanType->tlog = m_check_tlog;
	pCleanType->obj = m_check_obj;
	pCleanType->pdb = m_check_pdb;
	pCleanType->pch = m_check_pch;
	pCleanType->pch = m_check_ipch;


	EnumFoldAndFile(m_FilePath_Mfceditbrowse2, pCleanType);
}

//按钮——清理自定义后缀文件垃圾
void CClean::OnBnClickedButtonUserclean()
{
	// TODO: 在此添加控件通知处理程序代码

	if (MessageBox(L"确认删除文件？", 0, MB_OKCANCEL) == 0)
	{
		return;
	}

	UpdateData(TRUE);
	m_list_clean.DeleteAllItems();

	PCLEAMTYPE pCleanType = new CLEANTYPE;

	pCleanType->ilk = m_check_ilk;
	pCleanType->log = m_check_log;
	pCleanType->tlog = m_check_tlog;
	pCleanType->obj = m_check_obj;
	pCleanType->pdb = m_check_pdb;
	pCleanType->pch = m_check_pch;
	pCleanType->pch = m_check_ipch;


	EnumFoldAndDeleteFile(m_FilePath_Mfceditbrowse2, pCleanType);
	m_list_clean.DeleteAllItems();
	MessageBox(L"清理完成！");
}

//扫描一个文件夹下的指定后缀文件  
void CClean::EnumFoldAndFile(CString directory_path, PCLEAMTYPE pCleanType)
{
	CString path;
	CFileFind finder;

	path.Format(L"%s\\*", directory_path);

	BOOL bWorking = finder.FindFile(path);


	//遍历
	while (bWorking)
	{


		bWorking = finder.FindNextFile();

		if (finder.IsDirectory() && !finder.IsDots())
		{	
			//处理文件夹  
		
			EnumFoldAndFile(finder.GetFilePath(),pCleanType); //递归文件夹  
		}
		if (finder.IsDots())	//如果是. ..则跳过
		{
			continue;
		}
		//获取文件的后缀
		CString FileSuffix = (CString)(PathFindExtension(finder.GetFilePath()));
		if (!lstrcmp(FileSuffix, L".tlog") && pCleanType->tlog ||			//满足其中一个类型就可以删除
			!lstrcmp(FileSuffix, L".obj") && pCleanType->obj ||
			!lstrcmp(FileSuffix, L".log") && pCleanType->log ||
			!lstrcmp(FileSuffix, L".pch") && pCleanType->pch ||
			!lstrcmp(FileSuffix, L".pch") && pCleanType->ipch ||

			!lstrcmp(FileSuffix, L".ilk") && pCleanType->ilk ||
			!lstrcmp(FileSuffix, L".pdb") && pCleanType->pdb)
		{


			//文件名===================================
			CString szName = finder.GetFileName();

			//获取后缀名;===============================
			CString szExName = PathFindExtension(szName);

			//大小=====================================
			CString szSize;
			szSize.Format(L"%u", finder.GetLength() / 1024);


			//创建时间=================================
			//      创建时间	修改时间   	修改时间  创建时间
			CString  cFileTime, mFileTime, strmTime, strcTime;

			CString path_temp = finder.GetFilePath();

			HANDLE hFile = CreateFile(path_temp,
				GENERIC_READ,  //必须有GENERIC_READ属性才能得到时间     GENERIC_WRITE | 
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				SYSTEMTIME sysTime;
				GetSystemTime(&sysTime);//这里得到的时间是标准系统时间，也就是0时区的时间。
				GetLocalTime(&sysTime);//这里得到的是本地时间，也就是标准时间+时区时间

				FILETIME fCreateTime, fAccessTime, fWriteTime;

				GetFileTime(&hFile, &fCreateTime, &fAccessTime, &fWriteTime);//获取文件时间


				 //创建时间====================================================================
				FileTimeToSystemTime(&fCreateTime, &sysTime);//将文件时间转换为标准系统时间
				SystemTimeToTzSpecificLocalTime(0, &sysTime, &sysTime);//将标准系统时间转换为本地系统时间

				strcTime.Format(_T("%4d年%2d月%2d日,%2d:%2d:%2d"),
					sysTime.wYear,
					sysTime.wMonth,
					sysTime.wDay,
					sysTime.wHour,
					sysTime.wMinute,
					sysTime.wSecond
				);

				//修改时间====================================================================
				FileTimeToSystemTime(&fWriteTime, &sysTime);//将文件时间转换为标准系统时间
				SystemTimeToTzSpecificLocalTime(0, &sysTime, &sysTime);//将标准系统时间转换为本地系统时间

				strmTime.Format(_T("%4d年%2d月%2d日,%2d:%2d:%2d"),
					sysTime.wYear,
					sysTime.wMonth,
					sysTime.wDay,
					sysTime.wHour,
					sysTime.wMinute,
					sysTime.wSecond
				);


				cFileTime += strcTime;
				mFileTime += strmTime;

			}
			CloseHandle(hFile);

			if (szName.Find(szExName) >= 0)
			{
				m_list_clean.AddItem(0, 4, szName, szSize, cFileTime, mFileTime);
			}
		}
	}
}

//删除一个文件夹下的指定后缀文件  
void CClean::EnumFoldAndDeleteFile(CString directory_path, PCLEAMTYPE pCleanType)
{
	CString path;
	CFileFind finder;


	//path.Format(L"%s/*.*", directory_path);
	//WCHAR szFoldPath[MAX_PATH] = L"H:\\*";		//在路径字符串追加通配符L"\\*"

	path.Format(L"%s\\*", directory_path);

	BOOL bWorking = finder.FindFile(path);
	DWORD dwIndex = 1;

	//处理

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots()) {
			//处理文件夹  
			myDeleteDirectory(finder.GetFilePath()); //递归删除文件夹  
													 //RemoveDirectory(finder.GetFilePath());
		}
		if (finder.IsDots())	//如果是. ..则跳过
		{
			continue;
		}
		//获取文件的后缀
		CString FileSuffix = (CString)(PathFindExtension(finder.GetFilePath()));
		if (!lstrcmp(FileSuffix, L".tlog") && pCleanType->tlog ||			//满足其中一个类型就可以删除
			!lstrcmp(FileSuffix, L".obj") && pCleanType->obj ||
			!lstrcmp(FileSuffix, L".log") && pCleanType->log ||
			!lstrcmp(FileSuffix, L".pch") && pCleanType->pch ||
			!lstrcmp(FileSuffix, L".ilk") && pCleanType->ilk ||
			!lstrcmp(FileSuffix, L".pdb") && pCleanType->pdb)
		{
			DeleteFile(finder.GetFilePath());
		}
	}


	/*CString path;
	CFileFind finder;

	path.Format(L"%s\\*", directory_path);

	BOOL bWorking = finder.FindFile(path);


	//遍历
	while (bWorking)
	{

		bWorking = finder.FindNextFile();
		//对比后缀名;

		if (finder.IsDirectory() && !finder.IsDots())
		{	//处理文件夹  
			EnumFoldAndDeleteFile(finder.GetFilePath(),pCleanType); //递归文件夹  
		}
		if (finder.IsDots())	//如果是. ..则跳过
		{
			continue;
		}
		//获取文件的后缀
		LPWSTR FileSuffix = PathFindExtension(finder.GetFileName());
		if (!lstrcmp(FileSuffix, L".tlog") && pCleanType->tlog ||			//满足其中一个类型就可以删除
			!lstrcmp(FileSuffix, L".obj") && pCleanType->obj ||
			!lstrcmp(FileSuffix, L".log") && pCleanType->log ||
			!lstrcmp(FileSuffix, L".pch") && pCleanType->pch ||
			!lstrcmp(FileSuffix, L".pch") && pCleanType->ipch ||
			!lstrcmp(FileSuffix, L".ilk") && pCleanType->ilk ||
			!lstrcmp(FileSuffix, L".pdb") && pCleanType->pdb)
		{

			DeleteFile(finder.GetFilePath());
		}
	}*/
}




//========================================================.

//全选check控件点击事件 系统垃圾
void CClean::OnBnClickedCheckAll()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_check_all)
	{
		m_check_sysTemp = TRUE;
		m_check_userTemp = TRUE;
		m_check_report = TRUE;
		m_check_recycle = TRUE;
		m_check_explore = TRUE;
		m_check_test = TRUE;

	}
	else
	{
		m_check_sysTemp = FALSE;
		m_check_userTemp = FALSE;
		m_check_report = FALSE;
		m_check_recycle = FALSE;
		m_check_explore = FALSE;
		m_check_test = FALSE;
	}

	UpdateData(FALSE);
}

//删除一个文件夹下的所有内容  
void CClean::myDeleteDirectory(CString directory_path)
{
	CString path;
	CFileFind finder;


	//path.Format(L"%s/*.*", directory_path);
	//WCHAR szFoldPath[MAX_PATH] = L"H:\\*";		//在路径字符串追加通配符L"\\*"

	path.Format(L"%s\\*", directory_path);

	BOOL bWorking = finder.FindFile(path);
	DWORD dwIndex = 1;

	//处理

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots()) {
			//处理文件夹  
			myDeleteDirectory(finder.GetFilePath()); //递归删除文件夹  
			//RemoveDirectory(finder.GetFilePath());
		}
		if (finder.IsDots())	//如果是. ..则跳过
		{
			continue;
		}
		else
		{	//处理文件  

			DeleteFile(finder.GetFilePath());
		}
	}


}

//扫描一个文件夹下的所有内容  
void CClean::myScanDirectory(CString directory_path)
{
	CString path;
	CFileFind finder;

	path.Format(L"%s\\*", directory_path);

	BOOL bWorking = finder.FindFile(path);


	//遍历
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots())
		{
			myScanDirectory(finder.GetFilePath()); //递归文件夹  
		}
		if (finder.IsDots())	//如果是. ..则跳过
		{
			continue;
		}
		else 
		{	//处理文件  


			CFileStatus rStatus;

			//文件名===================================
			CString szName = finder.GetFileName();

			//获取后缀名;===============================
			CString szExName = PathFindExtension(szName);

			//大小=====================================
			CString szSize;
			szSize.Format(L"%u", finder.GetLength() / 1024);


			// 			CFile::GetStatus(szName, rStatus);
			// 			CString szSize;
			// 			szSize.Format(L"%u", rStatus.m_size / 1024);

			//创建时间=================================

			//      创建时间	修改时间   	修改时间  创建时间
			CString  cFileTime, mFileTime, strmTime, strcTime;

			CString path_temp = finder.GetFilePath();

			HANDLE hFile = CreateFile(path_temp,
				 GENERIC_READ,  //必须有GENERIC_READ属性才能得到时间     
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				SYSTEMTIME sysTime;
				GetSystemTime(&sysTime);//这里得到的时间是标准系统时间，也就是0时区的时间。
				GetLocalTime(&sysTime);//这里得到的是本地时间，也就是标准时间+时区时间

				FILETIME fCreateTime, fAccessTime, fWriteTime;

				GetFileTime(&hFile, &fCreateTime, &fAccessTime, &fWriteTime);//获取文件时间

																			 //将文件时间转换为本地系统时间的两种方式：
																			 //(1)
																			 // 				FileTimeToLocalFileTime(&fCreateTime, &localTime);//将文件时间转换为本地文件时间
																			 // 				FileTimeToSystemTime(&localTime, &sysTime);//将文件时间转换为本地系统时间

																			 //(2)
																			 //修改时间====================================================================

				FileTimeToSystemTime(&fCreateTime, &sysTime);//将文件时间转换为标准系统时间
				SystemTimeToTzSpecificLocalTime(0, &sysTime, &sysTime);//将标准系统时间转换为本地系统时间

				strcTime.Format(_T("%4d年%2d月%2d日,%2d:%2d:%2d"),
					sysTime.wYear,
					sysTime.wMonth,
					sysTime.wDay,
					sysTime.wHour,
					sysTime.wMinute,
					sysTime.wSecond
				);

				//修改时间====================================================================
				FileTimeToSystemTime(&fWriteTime, &sysTime);//将文件时间转换为标准系统时间
				SystemTimeToTzSpecificLocalTime(0, &sysTime, &sysTime);//将标准系统时间转换为本地系统时间

				strmTime.Format(_T("%4d年%2d月%2d日,%2d:%2d:%2d"),
					sysTime.wYear,
					sysTime.wMonth,
					sysTime.wDay,
					sysTime.wHour,
					sysTime.wMinute,
					sysTime.wSecond
				);


				cFileTime += strcTime;
				mFileTime += strmTime;

			}

			CloseHandle(hFile);
			if (szName.Find(szExName) >= 0)
			{
				m_list_clean.AddItem(0, 4, szName, szSize, cFileTime, mFileTime);
			}
		}
	}
}




//一键清理目录下所有文件
void CClean::OnBnClickedButtonOnekeyclean()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (MessageBox(L"确认删除文件？", 0, MB_OKCANCEL) == 1)
	{
		m_list_clean.DeleteAllItems();
		if (m_check_sysTemp)
		{
			m_Result = L"系统临时文件";
			myDeleteDirectory(m_Path_Sys);
		}

		if (m_check_explore)
		{
			m_Result = L"浏览器临时文件";
			myDeleteDirectory(m_Path_IE);
		}

		if (m_check_recycle)
		{
			m_Result = L"回收站";
			CleanRecycle();
		}

		if (m_check_userTemp)
		{
			m_Result = L"用户临时文件";
			myDeleteDirectory(m_Path_User);
		}

		if (m_check_report)
		{
			m_Result = L"错误报告";
			myDeleteDirectory(m_Path_Report);
		}

		if (m_check_test)
		{
			m_Result = L"自定义目录";
			myDeleteDirectory(m_Path_Test);
		}

		if (m_FilePath_Mfceditbrowse1 != L" ")
		{
			m_Result = L"自定义目录";
			myDeleteDirectory(m_FilePath_Mfceditbrowse1);
			m_FilePath_Mfceditbrowse1 = L"";
		}
		MessageBox(L"清理完成！");
	}
}

//一键扫描指定目录下全部文件
void CClean::OnBnClickedButtonScansys()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_list_clean.DeleteAllItems();
	if (m_check_sysTemp)
	{
		m_Result = L"系统临时文件";
		myScanDirectory(m_Path_Sys);
	}

	if (m_check_explore)
	{
		m_Result = L"浏览器临时文件";
		myScanDirectory(m_Path_IE);
	}

/*
	if (m_check_recycle)
	{
		m_Result = L"回收站";
		CleanRecycle();
	}*/

	if (m_check_userTemp)
	{
		m_Result = L"用户临时文件";
		myScanDirectory(m_Path_User);
	}

	if (m_check_report)
	{
		m_Result = L"错误报告";
		myScanDirectory(m_Path_Report);
	}
	if (m_check_test)
	{
		m_Result = L"自定义文件夹";
		myScanDirectory(m_Path_Test);
	}
	if (lstrcmp(m_FilePath_Mfceditbrowse1, L""))
	{
		m_Result = L"自定义文件夹";
		myScanDirectory(m_FilePath_Mfceditbrowse1);
	}

}




//拖拽文件
void CClean::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

/*

	TCHAR szPatch[MAX_PATH] = { 0 };

	UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT idx = 0; idx < nCount; idx++)
	{
		DragQueryFile(hDropInfo, idx, szPatch, MAX_PATH);
		SetDlgItemText(IDC_MFCEDITBROWSE1, szPatch);
		SetDlgItemText(IDC_MFCEDITBROWSE2, szPatch);

	}*/

	CDialogEx::OnDropFiles(hDropInfo);
}



