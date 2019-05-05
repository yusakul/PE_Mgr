#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CBoot 对话框

class CBoot : public CDialogEx
{
	DECLARE_DYNAMIC(CBoot)

public:
	CBoot(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBoot();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BOOT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CMyListCtrl m_list_boot;


	CString	m_Name;		  //键名
	CString m_Key;		  //键值

	HKEY m_hRootKey1;	  //主键1
	HKEY m_hRootKey2;	  //主键2
	CString m_Root1;	  //主键1
	CString m_Root2;	  //主键2
	CString m_SubKey1;	  //子键1
	CString m_SubKey2;	  //子键2


	void DeleBoot();		  //删除启动项
	void UpDataBoot();	  //刷新启动项
	void AddBoot();		  //添加驱动型
	void GetBootInfo(HKEY hRootKey, CString Path);//获取启动项
	afx_msg void OnNMRClickListBoot(NMHDR *pNMHDR, LRESULT *pResult);
};
