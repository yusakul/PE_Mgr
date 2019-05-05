// Clean.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Clean.h"
#include "afxdialogex.h"
#include <strsafe.h>
#include <xutility>
#include <atlconv.h>


// CClean �Ի���

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


// CClean ��Ϣ�������


void CClean::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


BOOL CClean::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


	m_list_clean.AddColumn(4, L"�ļ�·��", 150, L"��С(KB)", 100,  L"����ʱ��",150, L"�޸�ʱ��", 150);

	//����·��
	m_Path_Sys = L"C:\\Windows\\Temp";
	m_Path_Test = L"C:\\Users\\Administrator\\Desktop\\test";

	m_Path_User = L"C:\\User\\Administrator\\AppData\\Local\\Temp";
	m_Path_Report = L"C:\\Users\\Administrator\\AppData\\Local\\Microsoft\\Windows\\WER\\ReportQueue";
	m_Path_IE = L"C:\\Users\\Administrator\\AppData\\Local\\Microsoft\\Windows\\INetCache";

	m_FilePath_Mfceditbrowse1 = L"";
	m_FilePath_Mfceditbrowse2 = L"";
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



//�ļ�������ؼ�����Ŀ¼�������ļ�
void CClean::OnEnChangeMfceditbrowse_sys()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	GetDlgItemText(IDC_MFCEDITBROWSE1, m_FilePath_Mfceditbrowse1);
	myScanDirectory(m_FilePath_Mfceditbrowse1);
}



//�������վ
void CClean::CleanRecycle()
{
	//��ʼ��SHQUERYRBINFO�ṹ��
	SHQUERYRBINFO RecycleBinInfo = {};
	RecycleBinInfo.cbSize = sizeof(RecycleBinInfo);

	//��ѯ����վ��Ϣ
	SHQueryRecycleBin(NULL, &RecycleBinInfo);

	//��ջ���վ
	SHEmptyRecycleBin(NULL, NULL, SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND);
}


//==========================================================

//�ļ�������ؼ�����Ŀ¼��ָ���ļ�
void CClean::OnEnChangeMfceditbrowse2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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


	GetDlgItemText(IDC_MFCEDITBROWSE2, m_FilePath_Mfceditbrowse2);	//��ȡ·��
	EnumFoldAndFile(m_FilePath_Mfceditbrowse2, pCleanType);			//������������б�
}


//�Զ�������ѡ��check�¼�
void CClean::OnBnClickedCheckAll2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//��ť����ɨ��ָ����׺�ļ�
void CClean::OnBnClickedButtonScanhouzhui()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (!(lstrcmp(m_FilePath_Mfceditbrowse2, L"")))
	{
		MessageBox(L"û��ѡ���ļ���");
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

//��ť���������Զ����׺�ļ�����
void CClean::OnBnClickedButtonUserclean()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (MessageBox(L"ȷ��ɾ���ļ���", 0, MB_OKCANCEL) == 0)
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
	MessageBox(L"������ɣ�");
}

//ɨ��һ���ļ����µ�ָ����׺�ļ�  
void CClean::EnumFoldAndFile(CString directory_path, PCLEAMTYPE pCleanType)
{
	CString path;
	CFileFind finder;

	path.Format(L"%s\\*", directory_path);

	BOOL bWorking = finder.FindFile(path);


	//����
	while (bWorking)
	{


		bWorking = finder.FindNextFile();

		if (finder.IsDirectory() && !finder.IsDots())
		{	
			//�����ļ���  
		
			EnumFoldAndFile(finder.GetFilePath(),pCleanType); //�ݹ��ļ���  
		}
		if (finder.IsDots())	//�����. ..������
		{
			continue;
		}
		//��ȡ�ļ��ĺ�׺
		CString FileSuffix = (CString)(PathFindExtension(finder.GetFilePath()));
		if (!lstrcmp(FileSuffix, L".tlog") && pCleanType->tlog ||			//��������һ�����;Ϳ���ɾ��
			!lstrcmp(FileSuffix, L".obj") && pCleanType->obj ||
			!lstrcmp(FileSuffix, L".log") && pCleanType->log ||
			!lstrcmp(FileSuffix, L".pch") && pCleanType->pch ||
			!lstrcmp(FileSuffix, L".pch") && pCleanType->ipch ||

			!lstrcmp(FileSuffix, L".ilk") && pCleanType->ilk ||
			!lstrcmp(FileSuffix, L".pdb") && pCleanType->pdb)
		{


			//�ļ���===================================
			CString szName = finder.GetFileName();

			//��ȡ��׺��;===============================
			CString szExName = PathFindExtension(szName);

			//��С=====================================
			CString szSize;
			szSize.Format(L"%u", finder.GetLength() / 1024);


			//����ʱ��=================================
			//      ����ʱ��	�޸�ʱ��   	�޸�ʱ��  ����ʱ��
			CString  cFileTime, mFileTime, strmTime, strcTime;

			CString path_temp = finder.GetFilePath();

			HANDLE hFile = CreateFile(path_temp,
				GENERIC_READ,  //������GENERIC_READ���Բ��ܵõ�ʱ��     GENERIC_WRITE | 
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				SYSTEMTIME sysTime;
				GetSystemTime(&sysTime);//����õ���ʱ���Ǳ�׼ϵͳʱ�䣬Ҳ����0ʱ����ʱ�䡣
				GetLocalTime(&sysTime);//����õ����Ǳ���ʱ�䣬Ҳ���Ǳ�׼ʱ��+ʱ��ʱ��

				FILETIME fCreateTime, fAccessTime, fWriteTime;

				GetFileTime(&hFile, &fCreateTime, &fAccessTime, &fWriteTime);//��ȡ�ļ�ʱ��


				 //����ʱ��====================================================================
				FileTimeToSystemTime(&fCreateTime, &sysTime);//���ļ�ʱ��ת��Ϊ��׼ϵͳʱ��
				SystemTimeToTzSpecificLocalTime(0, &sysTime, &sysTime);//����׼ϵͳʱ��ת��Ϊ����ϵͳʱ��

				strcTime.Format(_T("%4d��%2d��%2d��,%2d:%2d:%2d"),
					sysTime.wYear,
					sysTime.wMonth,
					sysTime.wDay,
					sysTime.wHour,
					sysTime.wMinute,
					sysTime.wSecond
				);

				//�޸�ʱ��====================================================================
				FileTimeToSystemTime(&fWriteTime, &sysTime);//���ļ�ʱ��ת��Ϊ��׼ϵͳʱ��
				SystemTimeToTzSpecificLocalTime(0, &sysTime, &sysTime);//����׼ϵͳʱ��ת��Ϊ����ϵͳʱ��

				strmTime.Format(_T("%4d��%2d��%2d��,%2d:%2d:%2d"),
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

//ɾ��һ���ļ����µ�ָ����׺�ļ�  
void CClean::EnumFoldAndDeleteFile(CString directory_path, PCLEAMTYPE pCleanType)
{
	CString path;
	CFileFind finder;


	//path.Format(L"%s/*.*", directory_path);
	//WCHAR szFoldPath[MAX_PATH] = L"H:\\*";		//��·���ַ���׷��ͨ���L"\\*"

	path.Format(L"%s\\*", directory_path);

	BOOL bWorking = finder.FindFile(path);
	DWORD dwIndex = 1;

	//����

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots()) {
			//�����ļ���  
			myDeleteDirectory(finder.GetFilePath()); //�ݹ�ɾ���ļ���  
													 //RemoveDirectory(finder.GetFilePath());
		}
		if (finder.IsDots())	//�����. ..������
		{
			continue;
		}
		//��ȡ�ļ��ĺ�׺
		CString FileSuffix = (CString)(PathFindExtension(finder.GetFilePath()));
		if (!lstrcmp(FileSuffix, L".tlog") && pCleanType->tlog ||			//��������һ�����;Ϳ���ɾ��
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


	//����
	while (bWorking)
	{

		bWorking = finder.FindNextFile();
		//�ԱȺ�׺��;

		if (finder.IsDirectory() && !finder.IsDots())
		{	//�����ļ���  
			EnumFoldAndDeleteFile(finder.GetFilePath(),pCleanType); //�ݹ��ļ���  
		}
		if (finder.IsDots())	//�����. ..������
		{
			continue;
		}
		//��ȡ�ļ��ĺ�׺
		LPWSTR FileSuffix = PathFindExtension(finder.GetFileName());
		if (!lstrcmp(FileSuffix, L".tlog") && pCleanType->tlog ||			//��������һ�����;Ϳ���ɾ��
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

//ȫѡcheck�ؼ�����¼� ϵͳ����
void CClean::OnBnClickedCheckAll()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//ɾ��һ���ļ����µ���������  
void CClean::myDeleteDirectory(CString directory_path)
{
	CString path;
	CFileFind finder;


	//path.Format(L"%s/*.*", directory_path);
	//WCHAR szFoldPath[MAX_PATH] = L"H:\\*";		//��·���ַ���׷��ͨ���L"\\*"

	path.Format(L"%s\\*", directory_path);

	BOOL bWorking = finder.FindFile(path);
	DWORD dwIndex = 1;

	//����

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots()) {
			//�����ļ���  
			myDeleteDirectory(finder.GetFilePath()); //�ݹ�ɾ���ļ���  
			//RemoveDirectory(finder.GetFilePath());
		}
		if (finder.IsDots())	//�����. ..������
		{
			continue;
		}
		else
		{	//�����ļ�  

			DeleteFile(finder.GetFilePath());
		}
	}


}

//ɨ��һ���ļ����µ���������  
void CClean::myScanDirectory(CString directory_path)
{
	CString path;
	CFileFind finder;

	path.Format(L"%s\\*", directory_path);

	BOOL bWorking = finder.FindFile(path);


	//����
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (finder.IsDirectory() && !finder.IsDots())
		{
			myScanDirectory(finder.GetFilePath()); //�ݹ��ļ���  
		}
		if (finder.IsDots())	//�����. ..������
		{
			continue;
		}
		else 
		{	//�����ļ�  


			CFileStatus rStatus;

			//�ļ���===================================
			CString szName = finder.GetFileName();

			//��ȡ��׺��;===============================
			CString szExName = PathFindExtension(szName);

			//��С=====================================
			CString szSize;
			szSize.Format(L"%u", finder.GetLength() / 1024);


			// 			CFile::GetStatus(szName, rStatus);
			// 			CString szSize;
			// 			szSize.Format(L"%u", rStatus.m_size / 1024);

			//����ʱ��=================================

			//      ����ʱ��	�޸�ʱ��   	�޸�ʱ��  ����ʱ��
			CString  cFileTime, mFileTime, strmTime, strcTime;

			CString path_temp = finder.GetFilePath();

			HANDLE hFile = CreateFile(path_temp,
				 GENERIC_READ,  //������GENERIC_READ���Բ��ܵõ�ʱ��     
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				SYSTEMTIME sysTime;
				GetSystemTime(&sysTime);//����õ���ʱ���Ǳ�׼ϵͳʱ�䣬Ҳ����0ʱ����ʱ�䡣
				GetLocalTime(&sysTime);//����õ����Ǳ���ʱ�䣬Ҳ���Ǳ�׼ʱ��+ʱ��ʱ��

				FILETIME fCreateTime, fAccessTime, fWriteTime;

				GetFileTime(&hFile, &fCreateTime, &fAccessTime, &fWriteTime);//��ȡ�ļ�ʱ��

																			 //���ļ�ʱ��ת��Ϊ����ϵͳʱ������ַ�ʽ��
																			 //(1)
																			 // 				FileTimeToLocalFileTime(&fCreateTime, &localTime);//���ļ�ʱ��ת��Ϊ�����ļ�ʱ��
																			 // 				FileTimeToSystemTime(&localTime, &sysTime);//���ļ�ʱ��ת��Ϊ����ϵͳʱ��

																			 //(2)
																			 //�޸�ʱ��====================================================================

				FileTimeToSystemTime(&fCreateTime, &sysTime);//���ļ�ʱ��ת��Ϊ��׼ϵͳʱ��
				SystemTimeToTzSpecificLocalTime(0, &sysTime, &sysTime);//����׼ϵͳʱ��ת��Ϊ����ϵͳʱ��

				strcTime.Format(_T("%4d��%2d��%2d��,%2d:%2d:%2d"),
					sysTime.wYear,
					sysTime.wMonth,
					sysTime.wDay,
					sysTime.wHour,
					sysTime.wMinute,
					sysTime.wSecond
				);

				//�޸�ʱ��====================================================================
				FileTimeToSystemTime(&fWriteTime, &sysTime);//���ļ�ʱ��ת��Ϊ��׼ϵͳʱ��
				SystemTimeToTzSpecificLocalTime(0, &sysTime, &sysTime);//����׼ϵͳʱ��ת��Ϊ����ϵͳʱ��

				strmTime.Format(_T("%4d��%2d��%2d��,%2d:%2d:%2d"),
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




//һ������Ŀ¼�������ļ�
void CClean::OnBnClickedButtonOnekeyclean()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if (MessageBox(L"ȷ��ɾ���ļ���", 0, MB_OKCANCEL) == 1)
	{
		m_list_clean.DeleteAllItems();
		if (m_check_sysTemp)
		{
			m_Result = L"ϵͳ��ʱ�ļ�";
			myDeleteDirectory(m_Path_Sys);
		}

		if (m_check_explore)
		{
			m_Result = L"�������ʱ�ļ�";
			myDeleteDirectory(m_Path_IE);
		}

		if (m_check_recycle)
		{
			m_Result = L"����վ";
			CleanRecycle();
		}

		if (m_check_userTemp)
		{
			m_Result = L"�û���ʱ�ļ�";
			myDeleteDirectory(m_Path_User);
		}

		if (m_check_report)
		{
			m_Result = L"���󱨸�";
			myDeleteDirectory(m_Path_Report);
		}

		if (m_check_test)
		{
			m_Result = L"�Զ���Ŀ¼";
			myDeleteDirectory(m_Path_Test);
		}

		if (m_FilePath_Mfceditbrowse1 != L" ")
		{
			m_Result = L"�Զ���Ŀ¼";
			myDeleteDirectory(m_FilePath_Mfceditbrowse1);
			m_FilePath_Mfceditbrowse1 = L"";
		}
		MessageBox(L"������ɣ�");
	}
}

//һ��ɨ��ָ��Ŀ¼��ȫ���ļ�
void CClean::OnBnClickedButtonScansys()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	m_list_clean.DeleteAllItems();
	if (m_check_sysTemp)
	{
		m_Result = L"ϵͳ��ʱ�ļ�";
		myScanDirectory(m_Path_Sys);
	}

	if (m_check_explore)
	{
		m_Result = L"�������ʱ�ļ�";
		myScanDirectory(m_Path_IE);
	}

/*
	if (m_check_recycle)
	{
		m_Result = L"����վ";
		CleanRecycle();
	}*/

	if (m_check_userTemp)
	{
		m_Result = L"�û���ʱ�ļ�";
		myScanDirectory(m_Path_User);
	}

	if (m_check_report)
	{
		m_Result = L"���󱨸�";
		myScanDirectory(m_Path_Report);
	}
	if (m_check_test)
	{
		m_Result = L"�Զ����ļ���";
		myScanDirectory(m_Path_Test);
	}
	if (lstrcmp(m_FilePath_Mfceditbrowse1, L""))
	{
		m_Result = L"�Զ����ļ���";
		myScanDirectory(m_FilePath_Mfceditbrowse1);
	}

}




//��ק�ļ�
void CClean::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

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



