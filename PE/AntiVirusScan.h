#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include <vector>
#include "MD5.h"
#include "afxwin.h"
#include "ClientSocket.h"


// CAntiVirusScan �Ի���

class CAntiVirusScan : public CDialogEx
{
	DECLARE_DYNAMIC(CAntiVirusScan)

public:
	CAntiVirusScan(CString FilePath, DWORD scantype, CWnd* pParent = NULL);   // ��׼���캯��
	CAntiVirusScan( CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAntiVirusScan();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANTI_VIRUS_SCAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CMyListCtrl m_list_AntivirusScan;	//��Ϣ�б�
	DWORD m_scantype;				//ɨ������
	//CString m_FilePath;				//ɨ��·��
  
	
	DWORD 	m_dwScanType;		  //ɨ������
 	bool	m_IsPause;			  //��ͣ
 	bool	m_IsCancle;			  //ȡ��
	CString m_FolderPath;		  //�ļ���·��
	CString m_FilePath;			  //�ļ�·��

	CString m_ScanFileFath;		  //ɨ��·��
	CString m_ScanMd5;			  //ɨ��MD5
	CString m_ScanType;			  //ɨ������

	std::vector<CString>  m_vecDisk;		   //�������̷�
	std::vector<CString>  m_vecFilePath;	   //�ļ�·��

	std::vector<CString>  m_vecVirusDatabase;  //������
	std::vector<CString>  m_vecBlack;		   //������
	std::vector<CString>  m_vecWhite;		   //������

	const TCHAR* szFileName[3] = {			   //���ز������ļ�
		L"H:\\VirusDataBase.txt",
		L"H:\\WhiteList.txt",
		L"H:\\BlackList.txt" };

	void AntiScan();							//ɨ��ַ�
	
	
	void ScanFile();							//ɨ���ļ�
	void ScanFloder(CString FloderPath);		//ɨ���ļ���								
	void ScanAll();								//ȫ��ɨ��
	void ScanProcess();							//ɨ�����		
	void FastScan();							//����ɨ��
									
	void ReadDataBase();						//��ȡ������												
	bool DataBaseCMP(CString strMD5);			//�Ա�MD5

	CClientSocket m_Socket;			//Socket�ͻ���
	CString		  m_MD5;			//MD5
	CString		  m_DatabaseTime;	//��������ʱ��
	std::vector<CString>  m_vecMD5;	//��ȡ���ݿ�MD5����
	CStatic m_TextCloud;	//���ӷ�������ʾ

public:

	CStatic m_TextScanInfo;			//ɨ����ʾ

	afx_msg void OnBnClickedButtonPause();			//��ͣɨ��
	afx_msg void OnBnClickedButtonCancelscan();		//�˳�ɨ��
	afx_msg void OnBnClickedButtonDelVirus();		//ɾ������


	HANDLE hScanThread;
	static UINT  MyThreadScan(LPVOID lpParam);	//ȫ��ɨ��ص�����;
	HANDLE hUpdataThread;					//��ȡ����������߳��߳̾��
	
	void LinkServer();	//��ȡ����������߳�



	bool UpdataDataBase(DWORD dwType);		//�ӷ���˸��±������ݿ�							

	afx_msg void OnNMRClickListAntiVirusScan(NMHDR *pNMHDR, LRESULT *pResult);	//�Ҽ��б�˵�
};
