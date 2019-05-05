
// PEDlg.h : 头文件
//

#pragma once

#include "PEINFO.h"
#include "PEDATE.h"
#include "TaskMgr.h"
#include "afxcmn.h"
#include "MyTab.h"
#include "AntiVirus.h"
#include "Clean.h"
#include "Home.h"
#include "Speed.h"
#include "Tools.h"


// CPEDlg 对话框
class CPEDlg : public CDialogEx
{
// 构造
public:
	CPEDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnClose();//    关闭程序的时候卸载DLL !!!!!



	CString m_szPath;
	

	afx_msg void OnEnChangeMfceditbrowse1();

	//任务管理器===================================================================
	CTaskMgr task;								//任务管理器类对象
	afx_msg void OnBnClickedButtonOpentask();	//按钮——打开任务管理器

	//首页菜单=====================================================================
	afx_msg void OnCancel();
	afx_msg void OnLogout();		//注销
	afx_msg void OnShutdown();		//关机
	afx_msg void OnReboot();		//重启
	afx_msg void OnHotkey();		//热键

	//提权函数
	BOOL AdjustPrivilege();


	//首页Tab页面======================================================================
	CMyTab m_Tab_Home;

	//管理员权限运行按钮==============================================================
	afx_msg void OnBnClickedButtonAdmin();

	//是否是管理员
	BOOL IsAdmin(HANDLE hProcess);

	
	//进程保护=======================================================================Start
	HINSTANCE m_hinstHookDll;    //    MonitorDll的实例句柄
	void HookLoad();            //    加载HOOK            
	void HookUnload();            //    卸载HOOK
	//进程保护=======================================================================End

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonProctect();

	//插入地址——注入目标
	bool StartInsert(char * title, char * winclass, char * dllpath);

	//重写注入
	void StartInject();
	
	//获取目标进程PID
	HANDLE GetThePidOfTargetProcess(HWND hwnd);
	
	//注入
	BOOL DoInjection(char * DllPath, HANDLE hProcess);
};
