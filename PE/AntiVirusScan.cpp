// AntiVirusScan.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "AntiVirusScan.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "datatype.h"


// CAntiVirusScan �Ի���

IMPLEMENT_DYNAMIC(CAntiVirusScan, CDialogEx)

CAntiVirusScan::CAntiVirusScan(CString FilePath, DWORD scantype, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ANTI_VIRUS_SCAN, pParent)
	, m_IsPause(false)
	, m_IsCancle(false)
{
	m_scantype = scantype;		//ɨ������
	m_FilePath = FilePath;		//�ļ�·��
	m_FolderPath = FilePath;	//�ļ���·��
	
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


// CAntiVirusScan ��Ϣ�������


void CAntiVirusScan::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_IsCancle == false)
	{
		MessageBox(L"����ȡ��ɨ��...", 0, MB_OK);
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_list_AntivirusScan.AddColumn(3, L"·��", 350, L"MD5", 250, L"����", 100);
	
	//�����̴߳ӷ���˻�ȡ����
	LinkServer();

	//��ȡ���ݿ�
	ReadDataBase();


	//����ɨ���߳�
	hScanThread = AfxBeginThread(MyThreadScan, this);
	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



//ɨ�跽ʽ
void CAntiVirusScan::AntiScan()
{
	m_IsCancle = false;
	//����ɨ�����ͷַ�
	switch (m_scantype)
	{
	case 0:
		//����ɨ��
		FastScan();
		break;
	case 1:
		//ȫ��ɨ��
		ScanAll();
		break;
	case 2:
		//�ļ�ɨ��
		ScanFile();
		break;
	case 3:
		//�ļ���ɨ��
		ScanFloder(m_FolderPath);
	default:
		break;
	}
	m_TextScanInfo.SetWindowTextW(L"ɨ�����");
	MessageBox(L"ɨ�������");
}


//ɨ�赥���ļ�
void CAntiVirusScan::ScanFile()
{
	//ת���ļ���;
	CStringA FinalFileName = CW2A(m_FilePath.GetBuffer(), CP_THREAD_ACP);

	//��ȡMD5
	char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
	m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

	m_TextScanInfo.SetWindowTextW(L"ɨ�����");
	//�Ա�MD5
	if (DataBaseCMP(m_ScanMd5))
	{
		m_vecFilePath.push_back(m_FilePath);
		m_list_AntivirusScan.AddItem(0,3, m_FilePath, m_ScanMd5, L"Σ���ļ�");
		MessageBox(L"Σ���ļ�!\r\n�봦��");
	}
	else
	{
		m_list_AntivirusScan.AddItem(0, 3, m_FilePath, m_ScanMd5, L"��ȫ�ļ�");
		MessageBox(L"��ȫ�ļ�!");
	}
}


//ɨ���ļ���
void CAntiVirusScan::ScanFloder(CString FloderPath)
{
	//ȡ������
	if (m_IsCancle)
	{
		//��������

		return;
	}

	//��ͣ����
	while (m_IsPause)
	{
	}

	//��·���˳�;
	if (FloderPath.IsEmpty())
	{
		m_IsCancle = true;
		return;
	}

	//�򿪼�;
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
		//���˱������ϼ�;
		if (!wcscmp(wsd.cFileName, L".")
			|| !wcscmp(wsd.cFileName, L".."))
		{
			continue;
		}
		//�ݹ�����ļ���;
		else if (wsd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString NextDir = FloderPath.Left(FloderPath.GetLength() - 1);
			NextDir += wsd.cFileName;
			ScanFloder(NextDir);
		}
	
		//ƴ���ļ���;
		CString m_ScanFileFath = FloderPath.Left(FloderPath.GetLength() - 1);
		m_ScanFileFath += wsd.cFileName;
		CStringA FinalFileName = CW2A(m_ScanFileFath.GetBuffer(), CP_THREAD_ACP);

		//��ȡMD5
		if (wsd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)	//�ų��ļ���
		{
			continue;
		}
		char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
		m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

		//����ɨ��״̬
		CString strScanInfo = L"����ɨ��...";
		strScanInfo += m_ScanFileFath;
		m_TextScanInfo.SetWindowTextW(strScanInfo);

		//MD5ֵ �Ա� �������ݿ⡢��������������
		if (DataBaseCMP(m_ScanMd5))
		{
			m_vecFilePath.push_back(m_ScanFileFath);
			m_list_AntivirusScan.AddItem(0 ,3, m_ScanFileFath, m_ScanMd5, L"Σ���ļ�");
		}


	} while (FindNextFile(hFind, &wsd));

	FindClose(hFind);

	//����ɨ��״̬
	m_TextScanInfo.SetWindowTextW(L"ɨ�����");
	m_IsCancle = true;
	return;
}

//ȫ��ɨ��
void CAntiVirusScan::ScanAll()
{
	//��ȡ�̷�;
	TCHAR szBuf[1024] = {};
	GetLogicalDriveStrings(1024, szBuf);
	TCHAR *pstr = szBuf;

	while (*pstr)
	{
		m_vecDisk.push_back(pstr);
		//��ȡ��һ���̷�;
		pstr += _tcslen(pstr) + 1;
	}

	//ɨ�����	
	void ScanProcess();

	//ѭ�������̷�
	for (DWORD i = 0; i < m_vecDisk.size(); i++)
	{
		ScanFloder(m_vecDisk[i]);
	}
}

//ɨ�����	
void CAntiVirusScan::ScanProcess()
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnapShot == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"�������̿���ʧ�ܣ�");
		return;
	}

	//��ʼ��������Ϣ�ṹ��;
	PROCESSENTRY32 pi = { sizeof(PROCESSENTRY32) };
	if (!Process32First(hSnapShot, &pi))
	{
		MessageBox(L"��ȡ������Ϣʧ��");
		return;
	}
	do
	{
		TCHAR *szBuf[5] = { new TCHAR[MAX_PATH], new TCHAR[6],  new TCHAR[6],  new TCHAR[6], new TCHAR[MAX_PATH], };
		szBuf[0] = pi.szExeFile;

		//��Ҫ��ȡ·���Ľ���
		HANDLE hPro = OpenProcess(PROCESS_QUERY_INFORMATION
			| PROCESS_QUERY_LIMITED_INFORMATION,
			FALSE, pi.th32ProcessID);
		//��ȡ��������·��;
		if (hPro)
		{
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hPro, NULL, szBuf[4], &dwSize);

		}
		else
		{
			szBuf[4] = L"��Ȩ����";
		}

		CString  FileFathW = szBuf[4];
		CStringA FinalFileName = CW2A(szBuf[4], CP_THREAD_ACP);
		//��ȡMD5
		char* szMD5 = md5FileValue(FinalFileName.GetBuffer());
		m_ScanMd5 = CA2W(szMD5, CP_THREAD_ACP);

		CString strScanInfo = L"����ɨ����̣�";
		strScanInfo += FileFathW;
		m_TextScanInfo.SetWindowTextW(strScanInfo);

		//�Ա�MD5
		if (DataBaseCMP(m_ScanMd5))
		{

			if (TerminateProcess(hPro, 0))
			{
				MessageBox(L"���ֿ��ɽ���!\r\n�ѽ�������\r\n");
			}
			else
			{
				MessageBox(L"���ֿ��ɽ���!\r\n����ʧ��!\r\n���ֶ�����");
			}
			m_vecFilePath.push_back(FileFathW);
			m_list_AntivirusScan.AddItem(0, 3, FileFathW, m_ScanMd5, L"Σ�ս���");
		}
		CloseHandle(hPro);

	} while (Process32Next(hSnapShot, &pi));
}

//����ɨ��
void CAntiVirusScan::FastScan()
{
	//ɨ�����
	ScanProcess();
	//ɨ��ϵͳ�ļ���
	ScanFloder(m_FolderPath);

	m_TextScanInfo.SetWindowTextW(L"ɨ�����");
}




//��ȡ�������ݿ�
void CAntiVirusScan::ReadDataBase()
{
	//��ȡ������
	CString strDatabase = szFileName[0];
	CStdioFile sfDatabase;
	if (sfDatabase.Open(strDatabase, CFile::modeRead))
	{
		CString temp;
		//ѭ����ȡ
		while (sfDatabase.ReadString(temp))
		{
			m_vecVirusDatabase.push_back(temp);
		}
		sfDatabase.Close();
	}
	else
	{
		MessageBox(L"��ȡ���ز�����ʧ�ܣ�");
		m_dwScanType = 4;
		return;
	}

	//��ȡ������
	CString strWhite = szFileName[1];
	CStdioFile sfWhite;
	if (sfWhite.Open(strWhite, CFile::modeRead))
	{
		CString temp;
		//ѭ����ȡ
		while (sfWhite.ReadString(temp))
		{
			m_vecWhite.push_back(temp);
		}
		sfWhite.Close();
	}

	//��ȡ������
	CString strBlack = szFileName[2];
	CStdioFile sfBlack;
	if (sfBlack.Open(strBlack, CFile::modeRead))
	{
		CString temp;
		//ѭ����ȡ
		while (sfBlack.ReadString(temp))
		{
			m_vecBlack.push_back(temp);
		}
		sfBlack.Close();
	}
}


//�Աȱ������ݿ�
bool CAntiVirusScan::DataBaseCMP(CString strMD5)
{
	//�ԱȰ�����
	for (DWORD i = 0; i < m_vecWhite.size(); i++)
	{
		if (strMD5 == m_vecWhite[i])
		{
			return false;
		}
	}

	//�ԱȺ�����
	for (DWORD i = 0; i < m_vecBlack.size(); i++)
	{
		if (strMD5 == m_vecBlack[i])
		{
			return true;
		}
	}

	//�ԱȲ�����
	for (DWORD i = 0; i < m_vecVirusDatabase.size(); i++)
	{
		if (strMD5 == m_vecVirusDatabase[i])
		{
			return true;
		}
	}
	return false;
}





//��ͣɨ��
void CAntiVirusScan::OnBnClickedButtonPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����δ��ʼ������ͣ


	m_IsPause = !m_IsPause;

	//���İ�ť״̬
	if (m_IsPause)
	{
		GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowTextW(L"����ɨ��");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_PAUSE)->SetWindowTextW(L"��ͣɨ��");
	}
}

//ȡ��ɨ��
void CAntiVirusScan::OnBnClickedButtonCancelscan()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_IsCancle = true;
	GetDlgItem(IDC_BUTTON_CANCELSCAN)->SetWindowTextW(L"ɨ����ȡ��");
}

//ɾ������
void CAntiVirusScan::OnBnClickedButtonDelVirus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_vecFilePath.size() == 0)
	{
		MessageBox(L"���޿ɴ�����Ŀ��");
		return;
	}

	//ɾ������
	for (DWORD i = 0; i < m_vecFilePath.size(); i++)
	{
		DeleteFile(m_vecFilePath[i]);
	}
	m_list_AntivirusScan.DeleteAllItems();
	MessageBox(L"������ɣ�");
}




//ɨ���̻߳ص�����
UINT CAntiVirusScan::MyThreadScan(LPVOID lpParam)
{
	CAntiVirusScan* pAntiScan = (CAntiVirusScan*)lpParam;

	pAntiScan->AntiScan();

	pAntiScan->hScanThread = NULL;

	return 0;
}

//�����===================================================================================

//��ȡ����������߳�
void CAntiVirusScan::LinkServer()
{
	CAntiVirusScan Antivirus;

	if (!(Antivirus.m_Socket.ConnectServer("127.0.0.1", 1234)))
	{
		m_TextCloud.SetWindowText(L"δ���ӷ����");
		Antivirus.m_Socket.Close();
		return;
	}

	if (!Antivirus.UpdataDataBase(3))	//���º�����
	{
		Antivirus.m_Socket.Close();
		m_TextCloud.SetWindowTextW(L"���º�����ʧ��");
		return;
	}
	Sleep(100);
	if (!Antivirus.UpdataDataBase(2))		//���°�����
	{
		Antivirus.m_Socket.Close();
		m_TextCloud.SetWindowTextW(L"���°�����ʧ��");
		return;
	}
	Sleep(100);
	if (!Antivirus.UpdataDataBase(1))		//���±��ز�����
	{
		Antivirus.m_Socket.Close();
		m_TextCloud.SetWindowTextW(L"���²�����ʧ��");
		return;
	}
/*
	if (!Antivirus.UpdataDataBase(5))		//�ύmd5��������
	{
		Antivirus.m_Socket.Close();
		m_TextCloud.SetWindowTextW(L"�ϴ�������ʧ��");
		return;
	}*/
	Antivirus.m_Socket.Close();
	m_TextCloud.SetWindowTextW(L"�����Ʒ���˳ɹ���");
	return;
}


//�������ݿ�
bool CAntiVirusScan::UpdataDataBase(DWORD dwType)	//�ļ�����dwType    1�����²�����  2�����°����� 3�����º����� 4����ѯmd5  5���ύ����md5
{

	//��������dwType���͹̶����������д 
	m_Socket.Send((DATAPURPOSE)dwType, "cloud", dwType);

	//������Ϣ;
	CHATSEND ct = {};
	//ѭ������
	while (true)
	{
		if (SOCKET_ERROR == recv(m_Socket.m_sClient, (char*)&ct, sizeof(CHATSEND), NULL))
		{
			return false;
		}
		//���ս�����־
		if (!strcmp(ct.m_content.buf, "~~~end~~~"))
		{
			break;
		}
		m_MD5 = ct.m_content.buf;
		m_vecMD5.push_back(m_MD5);
	}

	//д���ļ�
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


//�Ҽ������˵� �����������������
void CAntiVirusScan::OnNMRClickListAntiVirusScan(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;



	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU2);
	pmenu = menu.GetSubMenu(3);//ָ�������˵��еĵ����˵���λ�á� λ��ֵ��0��ʼ��һ���˵�� 
	ClientToScreen(&pNMItemActivate->ptAction);//�ڿͻ���������ָʾ�����¼���λ��
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);

	//��ȡpath��md5
	CString sPath;
	CString sMd5;
	if (pNMItemActivate->iItem != -1)
	{
		sPath = m_list_AntivirusScan.GetItemText(pNMItemActivate->iItem, 0);//GetItemText:1.������ 2.�������󡣷���һ��CString���ı�
		sMd5 = m_list_AntivirusScan.GetItemText(pNMItemActivate->iItem, 1);//GetItemText:1.������ 2.�������󡣷���һ��CString���ı�
	}

	//�˵����֧
	switch (Select)
	{
	case ID_32808:	//����������б�
	{
		CString sFile = szFileName[1];
		CStdioFile sfFile;
		if (sfFile.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
		{
			sfFile.WriteString(sMd5); //д��һ���ı�

			for (DWORD i = 0; i < m_vecFilePath.size(); i++)		//��Ҫɾ����Ŀ¼��ȥ��
			{
				if (m_vecFilePath[i] == sPath)
				{
					m_vecFilePath.erase(m_vecFilePath.begin() + i);	//ɾ����i��
				}
			}
			sfFile.Close();
		}
	}
	break;
	case ID_32809:		//���������
	{
		CString sFile = szFileName[2];
		CStdioFile sfFile;
		if (sfFile.Open(sFile, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
		{
			sfFile.WriteString(sMd5); //д��һ���ı�
			sfFile.Close();
		}
	}
	break;
	default:
		break;
	}
}
