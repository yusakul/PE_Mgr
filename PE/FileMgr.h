#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "afxshelltreectrl.h"
#include "afxshelllistctrl.h"
#include "MyShellListCtrl.h"
#include "afxwin.h"


// CFileMgr 对话框

class CFileMgr : public CDialogEx
{
	DECLARE_DYNAMIC(CFileMgr)

public:
	CFileMgr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFileMgr();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILEMGR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CMyListCtrl m_list_filemgr;
	CMFCShellTreeCtrl m_shell_tree;
	CMFCShellListCtrl m_shell_list;

	//CMyShellListCtrl m_shell_list;
	

	afx_msg void OnNMClickMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult);

	CString m_FileMd5;			  //文件MD5
	CString m_sCurrName;		//当前
	CStringA m_sFilePath;

	CEdit m_FileNameEdit;
	CEdit m_FileTypeEdit;
};
