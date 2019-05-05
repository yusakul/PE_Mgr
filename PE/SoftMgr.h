#pragma once
#include <vector>
#include "MyListCtrl.h"
#include "afxcmn.h"


// CSoftMgr 对话框
// CSOFTWARE 对话框
/**/
typedef struct _SOFTINFO1
{
	WCHAR szSoftName[50];			//软件名称
	WCHAR szSoftVer[50];				//软件版本号
	WCHAR szSoftDate[20];				//软件安装日期
	WCHAR szSoftSize[MAX_PATH];				//软件大小
	WCHAR strSoftInsPath[MAX_PATH];			//软件安装路径
	WCHAR strSoftUniPath[MAX_PATH];			//软包卸载路径
	WCHAR strsoftVenRel[50];				//软件发布厂商
	WCHAR strSoftIco[MAX_PATH];				//软件图标路径
	WCHAR szSoftPub[50];			//

}SOFTINFO1, *PSOFTINFO1;


class CSoftMgr : public CDialogEx
{
	DECLARE_DYNAMIC(CSoftMgr)

public:
	CSoftMgr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSoftMgr();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SOFTWARE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();

	HKEY m_hRootKey;					  //主键
	CString m_SubKey1;					  //子键1
	CString m_SubKey2;					  //子键2

	CString m_KeyName;					  //键名
	CString m_KeyNameVer;				  //版本
	CString m_KeyNameDate;				  //日期
	CString m_KeyNameSize;				  //大小
	CString m_KeyNamePub;				  //厂家
	CString m_KeyNameInsPath;			  //安装路径
	CString m_KeyNameUniPath;			  //卸载路径

	CString m_SoftName;					  //显示名称
	CString m_SoftPub;					  //作者
	CString m_SoftDate;					  //日期
	CString m_SoftSize;					  //尺寸
	CString m_SoftVer;					  //版本
	CString m_SoftInsPath;				  //安装路径
	CString m_SoftUniPath;				  //卸载路径

	std::vector<SOFTINFO1> m_vecSoftInfo;	//存储信息
	void GetSoftwareInfo(CString SubKey);	//获取软件信息
	void UpdateSoftInfo();					//刷新
	void OpenExploer();						//打开文件夹
	void UnistallSoft();					//卸载软件

	CMyListCtrl m_list_soft;
	afx_msg void OnNMRClickListSoftware(NMHDR *pNMHDR, LRESULT *pResult);	//软件列表右键菜单
};
