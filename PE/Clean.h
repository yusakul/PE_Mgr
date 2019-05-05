#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyListCtrl.h"


typedef struct _CLEANTYPE
{
	BOOL ALL;
	BOOL ilk;
	BOOL log;
	BOOL obj;
	BOOL pch;
	BOOL pdb;
	BOOL tlog;
	BOOL ipch;
}CLEANTYPE, *PCLEAMTYPE;

// CClean 对话框

class CClean : public CDialogEx
{
	DECLARE_DYNAMIC(CClean)

public:
	CClean(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClean();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLEAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	BOOL m_check_all;
	BOOL m_check_explore;
	BOOL m_check_recycle;
	BOOL m_check_report;
	BOOL m_check_sysTemp;
	BOOL m_check_userTemp;
	BOOL m_check_test;
	CMyListCtrl m_list_clean;

	CString m_FilePath;	//文件路径
	CString m_FileType;
	CString m_FileName;	//文件名
	CString m_index;		//序号

	CString m_Result;	//文件说明

	CString m_Path_Report;	//错误报告
	CString m_Path_Sys;		//系统临时文件
	CString m_Path_IE;		//浏览器垃圾
	CString m_Path_User;	//用户临时文件
	CString m_Path_Test;	//桌面测试文件夹



	CString m_FilePath_Mfceditbrowse1;
	CString m_FilePath_Mfceditbrowse2;


	//清理回收站
	void CleanRecycle();


	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonOnekeyclean();

	//扫描一个文件夹下的所有内容 
	void myScanDirectory(CString directory_path);

	//删除一个文件夹下的所有内容  
	void myDeleteDirectory(CString directory_path);

	//扫描一个文件夹下的所有内容  
	void EnumFoldAndFile(CString directory_path, PCLEAMTYPE pCleanType);			//只扫描指定文件类型
	void EnumFoldAndDeleteFile(CString directory_path, PCLEAMTYPE pCleanType);	   //删除指定文件类型



	afx_msg void OnBnClickedCheckAll2();
	BOOL m_check_all2;
	BOOL m_check_ilk;
	BOOL m_check_log;
	BOOL m_check_obj;
	BOOL m_check_pch;
	BOOL m_check_pdb;
	BOOL m_check_tlog;
	BOOL m_check_ipch;


	afx_msg void OnBnClickedButtonUserclean();	//按钮——清理自定义文件类型文件
	afx_msg void OnBnClickedButtonScansys();	//按钮——扫描文件夹下所有文件
	afx_msg void OnEnChangeMfceditbrowse_sys();	//清理目录下所有文件——文件浏览

	afx_msg void OnDropFiles(HDROP hDropInfo);	//拖拽文件夹


	afx_msg void OnEnChangeMfceditbrowse2();//清理目录下指定后缀文件——文件浏览
	afx_msg void OnBnClickedButtonScanhouzhui();//按钮——扫描文件夹下指定后缀文件
};



