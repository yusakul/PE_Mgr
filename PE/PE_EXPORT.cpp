// PE_EXPORT.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_EXPORT.h"
#include "afxdialogex.h"


// CPE_EXPORT 对话框

IMPLEMENT_DYNAMIC(CPE_EXPORT, CDialogEx)

CPE_EXPORT::CPE_EXPORT(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXPORT, pParent)
	, m_AddressOfFunctions(_T(""))
	, m_AddressofNameOrdinals(_T(""))
	, m_AddressOfNames(_T(""))
	, m_Base(_T(""))
	, m_characteristrics(_T(""))
	, m_MajorVersion(_T(""))
	, m_MinorVersion(_T(""))
	, m_Name(_T(""))
	, m_NameAddr(_T(""))
	, m_NumberOfFunctions(_T(""))
	, m_NumberOfNames(_T(""))
	, m_TimeStamp(_T(""))
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt32 = pNt32;
	m_pDesc = m_pNt32->OptionalHeader.DataDirectory;
}

CPE_EXPORT::CPE_EXPORT(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXPORT, pParent)
	, m_AddressOfFunctions(_T(""))
	, m_AddressofNameOrdinals(_T(""))
	, m_AddressOfNames(_T(""))
	, m_Base(_T(""))
	, m_characteristrics(_T(""))
	, m_MajorVersion(_T(""))
	, m_MinorVersion(_T(""))
	, m_Name(_T(""))
	, m_NameAddr(_T(""))
	, m_NumberOfFunctions(_T(""))
	, m_NumberOfNames(_T(""))
	, m_TimeStamp(_T(""))
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt64 = pNt64;
	m_pDesc = m_pNt64->OptionalHeader.DataDirectory;
}

CPE_EXPORT::~CPE_EXPORT()
{
}

void CPE_EXPORT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EXPORT_ADDRESSOFFUNCTIONS, m_AddressOfFunctions);
	DDX_Text(pDX, IDC_EXPORT_ADDRESSOFNAMEOFDINALS, m_AddressofNameOrdinals);
	DDX_Text(pDX, IDC_EXPORT_ADDRESSOFNAMES, m_AddressOfNames);
	DDX_Text(pDX, IDC_EXPORT_BASE, m_Base);
	DDX_Text(pDX, IDC_EXPORT_CHARACTERISTICS, m_characteristrics);
	DDX_Text(pDX, IDC_EXPORT_MAJORVERSION, m_MajorVersion);
	DDX_Text(pDX, IDC_EXPORT_MINORVERSION, m_MinorVersion);
	DDX_Text(pDX, IDC_EXPORT_NAME, m_Name);
	DDX_Text(pDX, IDC_EXPORT_NAMECHR, m_NameAddr);
	DDX_Text(pDX, IDC_EXPORT_NUMBEROFFUNCTIONS, m_NumberOfFunctions);
	DDX_Text(pDX, IDC_EXPORT_NUMBEROFNAMES, m_NumberOfNames);
	DDX_Text(pDX, IDC_EXPORT_TIMEDATESTAMP, m_TimeStamp);
	DDX_Control(pDX, IDC_LIST_EXPORT, m_list_export);
}


BEGIN_MESSAGE_MAP(CPE_EXPORT, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPE_EXPORT 消息处理程序


BOOL CPE_EXPORT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_list_export.AddColumn(4, L"序号", 100, L"RVA", 140, L"偏移", 140, L"函数名称",140);

	if (m_pNt32)
	{
		SetExportDirectoryInfo32();

	}
	else
	{
		SetExportDirectoryInfo64();

	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPE_EXPORT::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

//设置导出表信息32位
void CPE_EXPORT::SetExportDirectoryInfo32()
{
	

	CPE_FOA_Calc objCalc(m_pNt32);

	//找到导出表
	DWORD dwExprotRVA = m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	PIMAGE_EXPORT_DIRECTORY pExprotRVA = (PIMAGE_EXPORT_DIRECTORY)(m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	//获取导出表在文件中的位置
	PIMAGE_EXPORT_DIRECTORY pExportFOA = (PIMAGE_EXPORT_DIRECTORY)(objCalc.RVAtoFOA(dwExprotRVA) + pFileImageBase);

	//地址表中的个数
	DWORD dwCountOfFuntions = pExportFOA->NumberOfFunctions;

	//名称表中的个数
	DWORD dwCountOfNames = pExportFOA->NumberOfNames;

	//地址表地址
	PDWORD pAddrOfFuntion = (PDWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfFunctions) + pFileImageBase);

	//名称表地址
	PDWORD pAddrOfName = (PDWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfNames) + pFileImageBase);

	//序号表地址
	PWORD pAddrOfOrdial = (PWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfNameOrdinals) + pFileImageBase);

	//base值
	DWORD dwBase = pExportFOA->Base;


	for (DWORD i = 0; i < dwCountOfFuntions; i++)
	{
		//地址表中可能存在无用的值（0）
		if (pAddrOfFuntion[i] == 0)
		{
			continue;
		}
		//根据序号表中是否有 和 地址表下标值相同 来判断是否是名称导出
		bool bRet = FALSE;
		for (unsigned j = 0; j < dwCountOfNames; j++)
		{
			if (i == pAddrOfOrdial[j])
			{
				//取出名称表中的地址名称RVA
				DWORD dwNameRVA = pAddrOfName[j];
				strName = (CString)(objCalc.RVAtoFOA(dwNameRVA) + pFileImageBase);
				bRet = true;
				break;
			}
		}
		if (!bRet)//序号表中没有，说明是以序号导出
		{
			//函数名
			strName.Format(L"N/A");
		}

		//RVA
		strRVA.Format(L"%08X", pAddrOfFuntion[i]);
		//FOA
		strOffset.Format(L"%08X", objCalc.RVAtoFOA(pAddrOfFuntion[i]));
		//序号
		strID.Format(L"%08X", i + dwBase);

		m_list_export.AddItem(0, 4, strID, strRVA, strOffset, strName);



	}


	//时间
	m_characteristrics.Format(L"00000000");
	m_TimeStamp.Format(L"%08X", pExportFOA->TimeDateStamp);
	m_MajorVersion.Format(L"%04X", pExportFOA->MajorVersion);
	m_MinorVersion.Format(L"%04X", pExportFOA->MinorVersion);
	m_Name = (CString)(objCalc.RVAtoFOA(pExportFOA->Name) + pFileImageBase);
	m_NameAddr.Format(L"%08X", pExportFOA->Name);
	m_Base.Format(L"%08X", pExportFOA->Base);
	m_NumberOfFunctions.Format(L"%08d", dwCountOfFuntions);
	m_NumberOfNames.Format(L"%08d", dwCountOfNames);
	m_AddressOfFunctions.Format(L"%08X", pExportFOA->AddressOfFunctions);
	m_AddressOfNames.Format(L"%08X", pExportFOA->AddressOfNames);
	m_AddressofNameOrdinals.Format(L"%08X", pExportFOA->AddressOfNameOrdinals);
	UpdateData(FALSE);
}


//设置导出表信息32位
void CPE_EXPORT::SetExportDirectoryInfo64()
{


	CPE_FOA_Calc objCalc(m_pNt64);

	//找到导出表
	DWORD dwExprotRVA = m_pNt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	//获取导出表在文件中的位置
	PIMAGE_EXPORT_DIRECTORY pExportFOA = (PIMAGE_EXPORT_DIRECTORY)(objCalc.RVAtoFOA(dwExprotRVA) + pFileImageBase);

	//地址表中的个数
	DWORD dwCountOfFuntions = pExportFOA->NumberOfFunctions;

	//名称表中的个数
	DWORD dwCountOfNames = pExportFOA->NumberOfNames;

	//地址表地址
	PDWORD pAddrOfFuntion = (PDWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfFunctions) + pFileImageBase);

	//名称表地址
	PDWORD pAddrOfName = (PDWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfNames) + pFileImageBase);

	//序号表地址
	PWORD pAddrOfOrdial = (PWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfNameOrdinals) + pFileImageBase);

	//base值
	DWORD dwBase = pExportFOA->Base;


	for (DWORD i = 0; i < dwCountOfFuntions; i++)
	{
		//地址表中可能存在无用的值（0）
		if (pAddrOfFuntion[i] == 0)
		{
			continue;
		}
		//根据序号表中是否有 和 地址表下标值相同 来判断是否是名称导出
		bool bRet = FALSE;
		for (unsigned j = 0; j < dwCountOfNames; j++)
		{
			if (i == pAddrOfOrdial[j])
			{
				//取出名称表中的地址名称RVA
				DWORD dwNameRVA = pAddrOfName[j];
				strName = (CString)(objCalc.RVAtoFOA(dwNameRVA) + pFileImageBase);
				bRet = true;
				break;
			}
		}
		if (!bRet)//序号表中没有，说明是以序号导出
		{
			//函数名
			strName.Format(L"N/A");
		}

		//RVA
		strRVA.Format(L"%08X", pAddrOfFuntion[i]);
		//FOA
		strOffset.Format(L"%08X", objCalc.RVAtoFOA(pAddrOfFuntion[i]));
		//序号
		strID.Format(L"%08X", i + dwBase);

		m_list_export.AddItem(0, 4, strID, strRVA, strOffset, strName);



	}


	//时间
	m_characteristrics.Format(L"00000000");
	m_TimeStamp.Format(L"%08X", pExportFOA->TimeDateStamp);
	m_MajorVersion.Format(L"%04X", pExportFOA->MajorVersion);
	m_MinorVersion.Format(L"%04X", pExportFOA->MinorVersion);
	m_Name = (CString)(objCalc.RVAtoFOA(dwExprotRVA) + pFileImageBase);
	m_NumberOfFunctions.Format(L"%08d", dwCountOfFuntions);
	m_NumberOfNames.Format(L"%08d", dwCountOfNames);
	m_AddressOfFunctions.Format(L"%08X", pExportFOA->AddressOfFunctions);
	m_AddressOfNames.Format(L"%08X", pExportFOA->AddressOfNames);
	m_AddressofNameOrdinals.Format(L"%08X", pExportFOA->AddressOfNameOrdinals);
	UpdateData(FALSE);
}




