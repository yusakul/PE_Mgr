#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "PE_FOA_Calc.h"
#include <vector>


typedef struct _OFFSET_TYPE
{
	WORD offset : 12;	//本页的偏移量
	WORD type : 4;		//重定位类型（3）
}OFFSET_TYPE, *POFFSET_TYPE;


typedef struct _RELOCINFO
{
	CString index;
	CString dwRelocRVa;		//需要重定位的相对虚拟地址（RVA）
	CString dwOffset;			//根据相对虚拟地址算出的文件偏移
	CString bType;				//重定位方式
	CString dwRelocValue;		//从算出的文件偏移取出的数据，这个数据就是虚拟地址（VA）
	CString bData;			//用上面得到的VA减去基址得到相对虚拟地址（RVA），再算一下文件偏移，将其中的数据取出放到这个数组

}RELOCINFO, *PRELOCINFO;	//这个结构体描述一个重定位信息

typedef struct _RELOCAREAINFO
{
	CString index;
	CString Section;		//区域所在的节名
	CString RVA;			//区域的基址（它是一个相对虚拟地址）
	CString Items;		//这个区域的重定位个数 
	std::vector<RELOCINFO> vecRelocInfo;	//这个区域重定位的具体信息
}RELOCAREAINFO, *PRELOCAREAINFO;			//这个结构体描述一个区域的重定位信息

// CPE_Relocation 对话框

class CPE_Relocation : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_Relocation)

public:
	CPE_Relocation(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // 标准构造函数
	CPE_Relocation(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CPE_Relocation();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RELOCATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CMyListCtrl m_list_relocation_block;	//区段列表
	CString m_Sec_Index;
	CString m_Sec_Section;
	CString m_Sec_RVA;
	CString m_Sec_Items;

	CMyListCtrl m_list_relocation_section;		//快项目列表
	CString m_Block_Index;		//
	CString m_Block_RVA;		//
	CString m_Block_Offset;
	CString m_Block_Type;
	CString m_Block_FarAddress;
	CString m_Block_DataInterpretaion;


	PIMAGE_NT_HEADERS32 m_pNt32;			//NT头指针
	PIMAGE_NT_HEADERS64 m_pNt64;			//NT头指针

	PIMAGE_DATA_DIRECTORY m_pDesc;		//数据目录表
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;				//文件起始


public:



	std::vector<RELOCAREAINFO> m_vecRelocAreaInfo;
	std::vector<RELOCINFO> m_vecRelocInfo;
	std::vector <PIMAGE_BASE_RELOCATION> m_vecPReloc;

	//获取重定位表区域信息
	void GetRelocInfo32();
	void GetRelocInfo64();

	CString GetSectionName(DWORD dwRVA);

	//获取区块详细信息
	void GetRelocDataInfo32(PIMAGE_BASE_RELOCATION pRelocFOA);
	//获取区块详细信息
	void GetRelocDataInfo64(PIMAGE_BASE_RELOCATION pReloc);


	afx_msg void OnNMClickListRelocaSection(NMHDR *pNMHDR, LRESULT *pResult);
};
