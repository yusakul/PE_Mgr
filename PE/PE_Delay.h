#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "PE_FOA_Calc.h"

// CPE_Delay 对话框

class CPE_Delay : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_Delay)

public:
	CPE_Delay(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_Delay(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CPE_Delay();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_DELAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();


	PIMAGE_NT_HEADERS32 m_pNt32;			//NT头指针
	PIMAGE_NT_HEADERS64 m_pNt64;			//NT头指针

	PIMAGE_DATA_DIRECTORY m_pDesc;		//数据目录表
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;				//文件起始
	PIMAGE_THUNK_DATA32  m_pDelayThunk32;		//INT表
	PIMAGE_THUNK_DATA64  m_pDelayThunk64;		//INT表


	//list===================================================
	CMyListCtrl m_list_API;
	CMyListCtrl m_list_DLL;

	CString m_DelayDllName;					//DLL名
	CString m_DelayNameRVA;					//RVA
	CString m_DelayINT;						//INT
	CString m_DelayIAT;						//IAT

	CString m_DelayFunOrdinal;				//函数序号
	CString m_DelayFunName;					//函数名称

	void GetDelayDllInfo32();					//获取DLL信息
	void GetDelayFunInfo32();					//获取函数信息
	void GetDelayDllInfo64();					//获取DLL信息
	void GetDelayFunInfo64();					//获取函数信息
	afx_msg void OnNMClickListDelayDll(NMHDR *pNMHDR, LRESULT *pResult);
};
