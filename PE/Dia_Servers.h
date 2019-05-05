#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CDia_Servers 对话框

class CDia_Servers : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Servers)

public:
	CDia_Servers(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDia_Servers();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERVERS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CMyListCtrl m_list_servers;

	CString m_ServersName;	//服务名称
	CString m_ServersDesc;	//描述
	CString m_ServersStatus;	//状态
	CString m_ServersType;		//类型
	CString m_ServersPath;		//路径

	void UpdataServers();	//刷新服务
	void GetServers();		//获取服务
	void ServersKey(bool Keyflag);		//服务开关

	const WCHAR* ServerStatus[8] =		//服务状态
	{
		L" ",
		L"已停止",
		L"正在停止",
		L"正在运行",
		L"即将继续",
		L"正在暂停",
		L"已暂停"
	};

	const WCHAR* ServerStartType[6] =	//服务启动类型
	{
		L" ",
		L" ",
		L"自动",
		L"手动",
		L"禁用",
		L" "
	};


	afx_msg void OnNMRClickListServers(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnMenuServers(UINT uID);
};
