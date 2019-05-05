#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "PE_FOA_Calc.h"
#include <vector>

using namespace std;


typedef struct EXPORTDIRECTORY
{
	DWORD nID;			//函数序号
	DWORD dwRVA;		//函数RVA
	DWORD dwOffset;
	CString strName;	//函数名称
}EXPORTDIRECTORY, *PEXPORTDIRECTORY;

// CPE_EXPORT 对话框
class CPE_EXPORT : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_EXPORT)

public:
	CPE_EXPORT(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_EXPORT(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CPE_EXPORT();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();


	CString strName;
		//RVA
	CString strRVA;
		//FOA
	CString strOffset;
		//序号
	CString strID;

	//数据
	CString m_AddressOfFunctions;
	CString m_AddressofNameOrdinals;
	CString m_AddressOfNames;
	CString m_Base;
	CString m_characteristrics;
	CString m_MajorVersion;
	CString m_MinorVersion;
	CString m_Name;
	CString m_NameAddr;
	CString m_NumberOfFunctions;
	CString m_NumberOfNames;
	CString m_TimeStamp;
	CMyListCtrl m_list_export;



	//设置导出表信息32位
	void SetExportDirectoryInfo32();
	//设置导出表信息64位
	void SetExportDirectoryInfo64();



	PIMAGE_NT_HEADERS32 m_pNt32;			//NT头指针
	PIMAGE_NT_HEADERS64 m_pNt64;			//NT头指针

	PIMAGE_DATA_DIRECTORY m_pDesc;		//数据目录表
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;				//文件起始


	vector<EXPORTDIRECTORY> m_exportDirectory;

};
