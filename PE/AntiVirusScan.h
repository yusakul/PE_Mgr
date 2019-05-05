#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include <vector>
#include "MD5.h"
#include "afxwin.h"
#include "ClientSocket.h"


// CAntiVirusScan 对话框

class CAntiVirusScan : public CDialogEx
{
	DECLARE_DYNAMIC(CAntiVirusScan)

public:
	CAntiVirusScan(CString FilePath, DWORD scantype, CWnd* pParent = NULL);   // 标准构造函数
	CAntiVirusScan( CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAntiVirusScan();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ANTI_VIRUS_SCAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CMyListCtrl m_list_AntivirusScan;	//信息列表
	DWORD m_scantype;				//扫描类型
	//CString m_FilePath;				//扫描路径
  
	
	DWORD 	m_dwScanType;		  //扫描类型
 	bool	m_IsPause;			  //暂停
 	bool	m_IsCancle;			  //取消
	CString m_FolderPath;		  //文件夹路径
	CString m_FilePath;			  //文件路径

	CString m_ScanFileFath;		  //扫描路径
	CString m_ScanMd5;			  //扫描MD5
	CString m_ScanType;			  //扫描类型

	std::vector<CString>  m_vecDisk;		   //驱动器盘符
	std::vector<CString>  m_vecFilePath;	   //文件路径

	std::vector<CString>  m_vecVirusDatabase;  //病毒库
	std::vector<CString>  m_vecBlack;		   //黑名单
	std::vector<CString>  m_vecWhite;		   //白名单

	const TCHAR* szFileName[3] = {			   //本地病毒库文件
		L"H:\\VirusDataBase.txt",
		L"H:\\WhiteList.txt",
		L"H:\\BlackList.txt" };

	void AntiScan();							//扫描分发
	
	
	void ScanFile();							//扫描文件
	void ScanFloder(CString FloderPath);		//扫描文件夹								
	void ScanAll();								//全盘扫描
	void ScanProcess();							//扫描进程		
	void FastScan();							//快速扫描
									
	void ReadDataBase();						//读取病毒库												
	bool DataBaseCMP(CString strMD5);			//对比MD5

	CClientSocket m_Socket;			//Socket客户端
	CString		  m_MD5;			//MD5
	CString		  m_DatabaseTime;	//更新数据时间
	std::vector<CString>  m_vecMD5;	//读取数据库MD5缓存
	CStatic m_TextCloud;	//连接服务器提示

public:

	CStatic m_TextScanInfo;			//扫描提示

	afx_msg void OnBnClickedButtonPause();			//暂停扫描
	afx_msg void OnBnClickedButtonCancelscan();		//退出扫描
	afx_msg void OnBnClickedButtonDelVirus();		//删除病毒


	HANDLE hScanThread;
	static UINT  MyThreadScan(LPVOID lpParam);	//全盘扫描回调函数;
	HANDLE hUpdataThread;					//获取服务端数据线程线程句柄
	
	void LinkServer();	//获取服务端数据线程



	bool UpdataDataBase(DWORD dwType);		//从服务端更新本地数据库							

	afx_msg void OnNMRClickListAntiVirusScan(NMHDR *pNMHDR, LRESULT *pResult);	//右键列表菜单
};
