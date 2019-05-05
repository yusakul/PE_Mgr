// PE_Delay.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_Delay.h"
#include "afxdialogex.h"


// CPE_Delay 对话框

IMPLEMENT_DYNAMIC(CPE_Delay, CDialogEx)

CPE_Delay::CPE_Delay(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_DELAY, pParent)
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt32 = pNt32;
	m_pDesc = m_pNt32->OptionalHeader.DataDirectory;
}

CPE_Delay::CPE_Delay(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_DELAY, pParent)
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt64 = pNt64;
	m_pDesc = m_pNt64->OptionalHeader.DataDirectory;
}

CPE_Delay::~CPE_Delay()
{
}

void CPE_Delay::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DELAY_API, m_list_API);
	DDX_Control(pDX, IDC_LIST_DELAY_DLL, m_list_DLL);
}


BEGIN_MESSAGE_MAP(CPE_Delay, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_DELAY_DLL, &CPE_Delay::OnNMClickListDelayDll)
END_MESSAGE_MAP()


// CPE_Delay 消息处理程序


void CPE_Delay::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}



BOOL CPE_Delay::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_list_DLL.AddColumn(3, L"DllName", 150, L"TNT(FO)", 200, L"IAT(FO)", 300);

	if (m_pNt32)
	{
		GetDelayDllInfo32();
	}
	else
	{
		GetDelayDllInfo64();
	}

	m_list_API.AddColumn(2, L"FunOrdinal", 80, L"FunName", 170);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


// 获取延迟导入表信息
void CPE_Delay::GetDelayDllInfo32()
{
	CPE_FOA_Calc objcalc(m_pNt32);
	if (m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress == 0)
	{
		MessageBox(L"没有延迟载入表...");
		EndDialog(0);
		return;
	}
	QWORD dwFO = (QWORD)objcalc.RVAtoFOA((QWORD)m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress)+(QWORD)pFileImageBase;
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = (PIMAGE_DELAYLOAD_DESCRIPTOR)(dwFO);

	//循环查找
	while (pDelay->DllNameRVA)
	{
		m_DelayDllName = (CHAR*)(pFileImageBase + objcalc.RVAtoFOA(pDelay->DllNameRVA));
		m_DelayINT.Format(L"%p", objcalc.RVAtoFOA(pDelay->ImportAddressTableRVA));
		m_DelayIAT.Format(L"%p", objcalc.RVAtoFOA(pDelay->ImportNameTableRVA));

		m_list_DLL.AddItem(0, 3, m_DelayDllName, m_DelayINT, m_DelayIAT);
		pDelay++;
	}
}

// 获取延迟导入表信息
void CPE_Delay::GetDelayDllInfo64()
{
	CPE_FOA_Calc objcalc(m_pNt64);
	if (m_pNt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress == 0)
	{
		MessageBox(L"没有延迟载入表...");
		EndDialog(0);
		return;
	}
	QWORD dwFO = (QWORD)objcalc.RVAtoFOA((QWORD)m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress) + (QWORD)pFileImageBase;
	PIMAGE_DELAYLOAD_DESCRIPTOR pDelay = (PIMAGE_DELAYLOAD_DESCRIPTOR)(dwFO);

	//循环查找
	while (pDelay->DllNameRVA)
	{
		m_DelayDllName = (CHAR*)(pFileImageBase + objcalc.RVAtoFOA(pDelay->DllNameRVA));
		m_DelayINT.Format(L"%p", objcalc.RVAtoFOA(pDelay->ImportAddressTableRVA));
		m_DelayIAT.Format(L"%p", objcalc.RVAtoFOA(pDelay->ImportNameTableRVA));

		m_list_DLL.AddItem(0, 3, m_DelayDllName, m_DelayINT, m_DelayIAT);
		pDelay++;
	}
}



//获取延迟导入函数信息
void CPE_Delay::GetDelayFunInfo32()
{
	m_list_API.DeleteAllItems();
	CPE_FOA_Calc objcalc(m_pNt32);

	//循环查找
	while (m_pDelayThunk32->u1.AddressOfData)
	{
		//序号命名
		if (IMAGE_SNAP_BY_ORDINAL32(m_pDelayThunk32->u1.Ordinal))
		{
			m_DelayFunOrdinal.Format(L"%X", IMAGE_ORDINAL32(m_pDelayThunk32->u1.Ordinal));
			m_DelayFunName = L"--";
		}
		//名称命名
		else
		{
			QWORD dwName = (QWORD)(pFileImageBase +
				objcalc.RVAtoFOA(m_pDelayThunk32->u1.AddressOfData));
			PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(dwName);

			m_DelayFunOrdinal.Format(L"%X", pName->Hint);
			m_DelayFunName = (CHAR*)pName->Name;
		}
		m_list_API.AddItem(0, 2, m_DelayFunOrdinal, m_DelayFunName);
		m_pDelayThunk32++;
	}
}

//获取延迟导入函数信息
void CPE_Delay::GetDelayFunInfo64()
{
	m_list_API.DeleteAllItems();
	CPE_FOA_Calc objcalc(m_pNt64);

	//循环查找
	while (m_pDelayThunk64->u1.AddressOfData)
	{
		//序号命名
		if (IMAGE_SNAP_BY_ORDINAL32(m_pDelayThunk64->u1.Ordinal))
		{
			m_DelayFunOrdinal.Format(L"%X", IMAGE_ORDINAL32(m_pDelayThunk64->u1.Ordinal));
			m_DelayFunName = L"--";
		}
		//名称命名
		else
		{
			QWORD dwName = (QWORD)(pFileImageBase +
				objcalc.RVAtoFOA(m_pDelayThunk64->u1.AddressOfData));
			PIMAGE_IMPORT_BY_NAME pName = (PIMAGE_IMPORT_BY_NAME)(dwName);

			m_DelayFunOrdinal.Format(L"%X", pName->Hint);
			m_DelayFunName = (CHAR*)pName->Name;
		}
		m_list_API.AddItem(0, 2, m_DelayFunOrdinal, m_DelayFunName);
		m_pDelayThunk64++;
	}
}



//点击DLL列表 显示对应的API
void CPE_Delay::OnNMClickListDelayDll(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	//获取选中数据
	int nSel = m_list_DLL.GetSelectionMark();
	if (nSel < 0)
	{
		MessageBox(L"请选中数据");
		return;
	}

	//获取序号
	int  dwINTF0 = wcstoll(m_list_DLL.GetItemText(nSel, 2), NULL, 16);

	if (dwINTF0 == -1)
	{
		dwINTF0 = wcstoll(m_list_DLL.GetItemText(nSel, 1), NULL, 16);
	}

	if (dwINTF0 == -1)
	{
		return;
	}

	

	//获取函数信息
	if (m_pNt32)
	{
		//转换
		m_pDelayThunk32 = (PIMAGE_THUNK_DATA32)((PBYTE)pFileImageBase + dwINTF0);
		GetDelayFunInfo32();
	}
	else
	{
		//转换
		m_pDelayThunk64 = (PIMAGE_THUNK_DATA64)((PBYTE)pFileImageBase + dwINTF0);
		GetDelayFunInfo64();
	}
}
