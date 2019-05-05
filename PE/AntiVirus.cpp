// AntiVirus.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "AntiVirus.h"
#include "afxdialogex.h"


// CAntiVirus �Ի���

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


// CAntiVirus ��Ϣ�������

BOOL CAntiVirus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



//����ɱ��
void CAntiVirus::OnBnClickedButtonFastantivirus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePath = L"C:\\Windows";
	m_scantype = 0;

	static	CAntiVirusScan AntiVirusScan(FilePath, m_scantype);
	AntiVirusScan.Create(IDD_DIALOG_ANTI_VIRUS_SCAN, 0);
	AntiVirusScan.ShowWindow(SW_SHOWNORMAL);
}

//ȫ��ɱ��
void CAntiVirus::OnBnClickedButtonAllantivirus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString FilePath = L"D:\\";
	m_scantype = 1;
	static	CAntiVirusScan AntiVirusScan(FilePath, m_scantype);
	AntiVirusScan.Create(IDD_DIALOG_ANTI_VIRUS_SCAN, 0);
	AntiVirusScan.ShowWindow(SW_SHOW);
}

//�Զ����ļ�ɱ��
void CAntiVirus::OnBnClickedButtonCustomfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("�����ļ�(*.*)\0*.*\0Exe�ļ�(*.exe)\0*.exe\0");//Ҫѡ����ļ���׺   
	ofn.lpstrInitialDir = _T("D:\\Program Files");//Ĭ�ϵ��ļ�·��   
	ofn.lpstrFile = szBuffer;//����ļ��Ļ�����   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT  
	BOOL bSel = GetOpenFileName(&ofn);

	//����ȡ����������Ч�ļ��з���NULL  
	if (bSel == NULL)
		return;


	CString FilePath = szBuffer;
	m_scantype = 2;
	static	CAntiVirusScan AntiVirusScan(FilePath, m_scantype);
	AntiVirusScan.Create(IDD_DIALOG_ANTI_VIRUS_SCAN, 0);
	AntiVirusScan.ShowWindow(SW_SHOW);
	
}


//�Զ���λ��ɱ��
void CAntiVirus::OnBnClickedButtonCustomantivirus()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�ļ���·������
	TCHAR szDir[MAX_PATH];

	//�����ʼ��
	BROWSEINFO bfi = {};		//�����Ϣ
	ITEMIDLIST *pidl;			//�ļ��б�ʶ���б�
	bfi.hwndOwner = this->m_hWnd;
	bfi.pszDisplayName = szDir;
	bfi.lpszTitle = _T("��ѡ���ļ���");
	bfi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	pidl = SHBrowseForFolder(&bfi); //����һ��PIDL����ָ����ѡ�ļ�������������ռ��Ŀ¼��λ�á�����û��ڶԻ�����ѡ��ȡ����ť���򷵻�ֵΪNULL��

	//����ȡ����������Ч�ļ��з���NULL  
	if (pidl == NULL)
		return;

	//��ȡ�ļ���·��
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}



void CAntiVirus::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(L"����ʩ��");
}


void CAntiVirus::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(L"����ʩ��");

}


void CAntiVirus::OnBnClickedButtonnote()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(L"����ʩ��");

}
