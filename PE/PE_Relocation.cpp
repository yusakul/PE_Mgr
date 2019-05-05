// PE_Relocation.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_Relocation.h"
#include "afxdialogex.h"
#include <winnt.h>


// CPE_Relocation 对话框

IMPLEMENT_DYNAMIC(CPE_Relocation, CDialogEx)

CPE_Relocation::CPE_Relocation(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RELOCATION, pParent)
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt32 = pNt32;
}

CPE_Relocation::CPE_Relocation(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RELOCATION, pParent)
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt64 = pNt64;
}

CPE_Relocation::~CPE_Relocation()
{
}

void CPE_Relocation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RELOCA_BLOCK, m_list_relocation_block);
	DDX_Control(pDX, IDC_LIST_RELOCA_SECTION, m_list_relocation_section);
}


BEGIN_MESSAGE_MAP(CPE_Relocation, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_RELOCA_SECTION, &CPE_Relocation::OnNMClickListRelocaSection)
END_MESSAGE_MAP()


// CPE_Relocation 消息处理程序


void CPE_Relocation::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL CPE_Relocation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list_relocation_section.AddColumn(4, L"索引",50, L"Section",100, L"RVA", 100,L"Items",100);
	m_list_relocation_block.AddColumn(6, L"索引",50, L"RVA",100, L"Offset",100, L"Type",150, L"Far Address",200, L"数据解释",150);

	if (m_pNt32)
	{
		GetRelocInfo32();
	}
	else
	{
		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}



//获取重定位区域信息
void CPE_Relocation::GetRelocInfo32()
{
	//重定位表由一个或一个以上的重定位块结构体组成
	//重定位表是一个由重定位块结构体的集合（也就是一个数组），数组是以一个全0的元素最为
	//结束的标志.

	CPE_FOA_Calc objCalc(m_pNt32);

	//重定位表RVA
	QWORD dwRelocRVA = m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;

	//是否为空
	if (!dwRelocRVA)
	{
		MessageBox(L"没有重定位表...\n", 0, 0);
		return;
	}

	//重定位表在文件中地址
	QWORD dwRelocFOA = (QWORD)(objCalc.RVAtoFOA(dwRelocRVA) + pFileImageBase);
	PIMAGE_BASE_RELOCATION pRelocFOA = (PIMAGE_BASE_RELOCATION)(dwRelocFOA);

	for (int i = 0; pRelocFOA->VirtualAddress ; i++)	//需要重定位的位置，这是一个相对虚拟地址（RVA）
	{
		RELOCAREAINFO StructRelocAreaInfo;

		//获取重定位项的个数
		DWORD dwCount = (pRelocFOA->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

		//序号
		m_Sec_Index.Format(L"%d", i + 1);
		StructRelocAreaInfo.index = m_Sec_Index;

		//段名
		m_Sec_Section = GetSectionName(pRelocFOA->VirtualAddress);
		StructRelocAreaInfo.Section = m_Sec_Section;

		//需要重定位的位置RVA
		m_Sec_RVA.Format(L"%08X", pRelocFOA->VirtualAddress);
		StructRelocAreaInfo.RVA = m_Sec_RVA;

		//项目
		m_Sec_Items.Format(L"%0Xh / %dd", dwCount, dwCount);
		StructRelocAreaInfo.Items = m_Sec_Items;
		
		m_vecRelocAreaInfo.push_back(StructRelocAreaInfo);

		m_vecPReloc.push_back(pRelocFOA);

		
		// 找到下一个重定位块
		pRelocFOA = (IMAGE_BASE_RELOCATION*)((QWORD)pRelocFOA + pRelocFOA->SizeOfBlock);
	}

	for (int i = m_vecRelocAreaInfo.size()-1;i>=0;  i--)
	{
		m_list_relocation_section.AddItem(0, 4, m_vecRelocAreaInfo[i].index, m_vecRelocAreaInfo[i].Section, m_vecRelocAreaInfo[i].RVA, m_vecRelocAreaInfo[i].Items);
	}
}

//获取重定位区域信息
void CPE_Relocation::GetRelocInfo64()
{
	//重定位表由一个或一个以上的重定位块结构体组成
	//重定位表是一个由重定位块结构体的集合（也就是一个数组），数组是以一个全0的元素最为
	//结束的标志.

	CPE_FOA_Calc objCalc(m_pNt64);

	//重定位表RVA
	QWORD dwRelocRVA = m_pNt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;

	//是否为空
	if (!dwRelocRVA)
	{
		MessageBox(L"没有重定位表...\n", 0, 0);
		return;
	}

	//重定位表在文件中地址
	QWORD dwRelocFOA = (QWORD)(objCalc.RVAtoFOA(dwRelocRVA) + pFileImageBase);
	PIMAGE_BASE_RELOCATION pRelocFOA = (PIMAGE_BASE_RELOCATION)(dwRelocFOA);

	for (int i = 0; pRelocFOA->VirtualAddress; i++)	//需要重定位的位置，这是一个相对虚拟地址（RVA）
	{
		RELOCAREAINFO StructRelocAreaInfo;

		//获取重定位项的个数
		DWORD dwCount = (pRelocFOA->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

		//序号
		m_Sec_Index.Format(L"%d", i + 1);
		StructRelocAreaInfo.index = m_Sec_Index;

		//段名
		m_Sec_Section = GetSectionName(pRelocFOA->VirtualAddress);
		StructRelocAreaInfo.Section = m_Sec_Section;

		//需要重定位的位置RVA
		m_Sec_RVA.Format(L"%08X", pRelocFOA->VirtualAddress);
		StructRelocAreaInfo.RVA = m_Sec_RVA;

		//项目
		m_Sec_Items.Format(L"%0Xh / %dd", dwCount, dwCount);
		StructRelocAreaInfo.Items = m_Sec_Items;

		m_vecRelocAreaInfo.push_back(StructRelocAreaInfo);

		m_vecPReloc.push_back(pRelocFOA);


		// 找到下一个重定位块
		pRelocFOA = (IMAGE_BASE_RELOCATION*)((QWORD)pRelocFOA + pRelocFOA->SizeOfBlock);
	}

	for (int i = m_vecRelocAreaInfo.size() - 1; i >= 0; i--)
	{
		m_list_relocation_section.AddItem(0, 4, m_vecRelocAreaInfo[i].index, m_vecRelocAreaInfo[i].Section, m_vecRelocAreaInfo[i].RVA, m_vecRelocAreaInfo[i].Items);
	}
}


//获取区域名
CString CPE_Relocation::GetSectionName(DWORD dwRVA)
{
	PIMAGE_SECTION_HEADER pSec;
	DWORD NumberOfSec;
	if (m_pNt32)
	{
		pSec = IMAGE_FIRST_SECTION(m_pNt32);
		NumberOfSec = m_pNt32->FileHeader.NumberOfSections;
	}
	else
	{
		pSec = IMAGE_FIRST_SECTION(m_pNt64);
		NumberOfSec = m_pNt64->FileHeader.NumberOfSections;
	}
	//循环查找
	for (int i = 0; i < NumberOfSec; i++)
	{
		//比对地址
		if (dwRVA >= pSec[i].VirtualAddress&&
			dwRVA <= pSec[i].VirtualAddress + pSec[i].SizeOfRawData)
		{
			//区段名	RVA	VS	FO	FS
			CHAR pName[9] = {};
			memcpy_s(pName, 9, pSec[i].Name, 8);
			return  (CString)pName;
			
		}
	}

	return L"--";
}


//获取区块详细信息
void CPE_Relocation::GetRelocDataInfo32(PIMAGE_BASE_RELOCATION pReloc)
{
	m_list_relocation_block.DeleteAllItems();
	std::vector<RELOCINFO> vecRelocInfo;

	//获取重定位项的开始地址
	POFFSET_TYPE pTypeOffset = (POFFSET_TYPE)(pReloc + 1);

	//获取重定位项的个数
	DWORD dwCount = (pReloc->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

	CPE_FOA_Calc objCalc(m_pNt32);

	for (unsigned i = 0; i < dwCount; i++)
	{
		RELOCINFO structRelocInfo;

		if (pTypeOffset[i].type == 3)	//这个结构体 的高四位被用来说明此重定位项的类型 3代表需要修正
		{
			//获取需要修复的RVA
			DWORD dwRelocDataRVA = pReloc->VirtualAddress + pTypeOffset[i].offset;
			QWORD dwRelocDataFOA = (QWORD)(objCalc.RVAtoFOA(dwRelocDataRVA) + pFileImageBase);

			//需要修复的重定位数据地址是个VA
			DWORD dwRealDataVA = *(DWORD*)dwRelocDataFOA;

			//转成RVA，得到FOA
			DWORD dwRealDataRVA = dwRealDataVA - m_pNt32->OptionalHeader.ImageBase;
			QWORD dwRealDataFOA = (QWORD)(objCalc.RVAtoFOA(dwRealDataRVA) + pFileImageBase);

			//序号
			m_Block_Index.Format(L"%d", i + 1);
			structRelocInfo.index = m_Block_Index;

			//需要重定位的位置RVA
			m_Block_RVA.Format(L"%08X", dwRelocDataRVA);
			structRelocInfo.dwRelocRVa = m_Block_RVA;

			//偏移
			m_Block_Offset.Format(L"%08X", dwRelocDataFOA);
			structRelocInfo.dwOffset = m_Block_Offset;

			//类型
			m_Block_Type.Format(L"HIGHLOW （3）");
			structRelocInfo.bType = m_Block_Type;

			//FAR地址
			m_Block_FarAddress.Format(L"%08X", dwRealDataVA);
			structRelocInfo.dwRelocValue = m_Block_FarAddress;

			//数据解释
			//需要重定位的具体数据（字节数不确定）
			byte* dwData = (byte*)dwRealDataFOA;

			if ((*dwData >= 'A'&& *dwData <= 'z'))	//判断数据类型
			{
				/*m_Block_DataInterpretaion.Format(L"%s", pData);*/
				m_Block_DataInterpretaion = dwData;
			}
			else
			{
				//输出十六进制字节
				m_Block_DataInterpretaion.Format(L"%02X  %02X  %02X  %02X  %02X  %02X  ",
					*(dwData),
					*(dwData + 1),
					*(dwData + 2),
					*(dwData + 3),
					*(dwData + 4),
					*(dwData + 5));
			}
			structRelocInfo.bData = m_Block_DataInterpretaion;

			vecRelocInfo.push_back(structRelocInfo);
		}
	}

	for (int i = vecRelocInfo.size() - 1; i >= 0; i--)
	{
		m_list_relocation_block.AddItem(0, 6, vecRelocInfo[i].index, vecRelocInfo[i].dwRelocRVa, vecRelocInfo[i].dwOffset, vecRelocInfo[i].bType, vecRelocInfo[i].dwRelocValue, vecRelocInfo[i].bData);
	}
}

//获取区块详细信息
void CPE_Relocation::GetRelocDataInfo64(PIMAGE_BASE_RELOCATION pReloc)
{
	m_list_relocation_block.DeleteAllItems();
	std::vector<RELOCINFO> vecRelocInfo;

	//获取重定位项的开始地址
	POFFSET_TYPE pTypeOffset = (POFFSET_TYPE)(pReloc + 1);

	//获取重定位项的个数
	DWORD dwCount = (pReloc->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

	CPE_FOA_Calc objCalc(m_pNt64);

	for (unsigned i = 0; i < dwCount; i++)
	{
		RELOCINFO structRelocInfo;

		if (pTypeOffset[i].type == 3)	//这个结构体 的高四位被用来说明此重定位项的类型 3代表需要修正
		{
			//获取需要修复的RVA
			DWORD dwRelocDataRVA = pReloc->VirtualAddress + pTypeOffset[i].offset;
			QWORD dwRelocDataFOA = (QWORD)(objCalc.RVAtoFOA(dwRelocDataRVA) + pFileImageBase);

			//需要修复的重定位数据地址是个VA
			DWORD dwRealDataVA = *(DWORD*)dwRelocDataFOA;

			//转成RVA，得到FOA
			DWORD dwRealDataRVA = dwRealDataVA - m_pNt64->OptionalHeader.ImageBase;
			QWORD dwRealDataFOA = (QWORD)(objCalc.RVAtoFOA(dwRealDataRVA) + pFileImageBase);

			//序号
			m_Block_Index.Format(L"%d", i + 1);
			structRelocInfo.index = m_Block_Index;

			//需要重定位的位置RVA
			m_Block_RVA.Format(L"%08X", dwRelocDataRVA);
			structRelocInfo.dwRelocRVa = m_Block_RVA;

			//偏移
			m_Block_Offset.Format(L"%08X", dwRelocDataFOA);
			structRelocInfo.dwOffset = m_Block_Offset;

			//类型
			m_Block_Type.Format(L"HIGHLOW （3）");
			structRelocInfo.bType = m_Block_Type;

			//FAR地址
			m_Block_FarAddress.Format(L"%08X", dwRealDataVA);
			structRelocInfo.dwRelocValue = m_Block_FarAddress;

			//数据解释
			//需要重定位的具体数据（字节数不确定）
			byte* dwData = (byte*)dwRealDataFOA;

			if ((*dwData >= 'A'&& *dwData <= 'z'))	//判断数据类型
			{
				/*m_Block_DataInterpretaion.Format(L"%s", pData);*/
				m_Block_DataInterpretaion = dwData;
			}
			else
			{
				//输出十六进制字节
				m_Block_DataInterpretaion.Format(L"%02X  %02X  %02X  %02X  %02X  %02X  ",
					*(dwData),
					*(dwData + 1),
					*(dwData + 2),
					*(dwData + 3),
					*(dwData + 4),
					*(dwData + 5));
			}
			structRelocInfo.bData = m_Block_DataInterpretaion;

			vecRelocInfo.push_back(structRelocInfo);
		}
	}

	for (int i = vecRelocInfo.size() - 1; i >= 0; i--)
	{
		m_list_relocation_block.AddItem(0, 6, vecRelocInfo[i].index, vecRelocInfo[i].dwRelocRVa, vecRelocInfo[i].dwOffset, vecRelocInfo[i].bType, vecRelocInfo[i].dwRelocValue, vecRelocInfo[i].bData);
	}
}


void CPE_Relocation::OnNMClickListRelocaSection(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	

	CString stIndex;

	if (pNMItemActivate->iItem != -1)//如果GetItemText项目索引未用于通知，则此成员将包含-1。
	{
		stIndex = m_list_relocation_section.GetItemText(pNMItemActivate->iItem, 0);
		int index = _tcstol(stIndex, NULL,10)-1;

		//获取信息
		GetRelocDataInfo32(m_vecPReloc[index]);
	}
}
