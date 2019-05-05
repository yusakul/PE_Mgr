// PE_FOA_Calc.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_FOA_Calc.h"
#include "afxdialogex.h"
#include <winnt.h>


// CPE_FOA_Calc 对话框

IMPLEMENT_DYNAMIC(CPE_FOA_Calc, CDialogEx)

CPE_FOA_Calc::CPE_FOA_Calc(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FOA_CALC, pParent)
	, m_FOA(_T(""))
	, m_Position(_T(""))
	, m_RVA(_T(""))
	, m_VA(_T(""))
{
	m_pNt32 = pNt32;
}

CPE_FOA_Calc::CPE_FOA_Calc(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FOA_CALC, pParent)
	, m_FOA(_T(""))
	, m_Position(_T(""))
	, m_RVA(_T(""))
	, m_VA(_T(""))
{
	m_pNt64 = pNt64;
}

CPE_FOA_Calc::~CPE_FOA_Calc()
{
}

void CPE_FOA_Calc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FOA, m_FOA);
	DDX_Text(pDX, IDC_EDIT_POSITION, m_Position);
	DDX_Text(pDX, IDC_EDIT_RVA, m_RVA);
	DDX_Text(pDX, IDC_EDIT_VA, m_VA);
	DDX_Control(pDX, IDC_RADIO_FOA, m_radioFOA);
	DDX_Control(pDX, IDC_RADIO_RVA, m_radioRVA);
	DDX_Control(pDX, IDC_RADIO_VA, m_radioVA);
	DDX_Control(pDX, IDC_EDIT_FOA, m_editFoa);
	DDX_Control(pDX, IDC_EDIT_RVA, m_editRva);
	DDX_Control(pDX, IDC_EDIT_VA, m_editVa);
	DDX_Control(pDX, IDC_EDIT_POSITION, m_editPosition);
}


BEGIN_MESSAGE_MAP(CPE_FOA_Calc, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CPE_FOA_Calc::OnBnClickedButtonCalc)
	ON_BN_CLICKED(IDC_RADIO_VA, &CPE_FOA_Calc::OnBnClickedRadioVa)
	ON_BN_CLICKED(IDC_RADIO_RVA, &CPE_FOA_Calc::OnBnClickedRadioRva)
	ON_BN_CLICKED(IDC_RADIO_FOA, &CPE_FOA_Calc::OnBnClickedRadioFoa)
END_MESSAGE_MAP()


// CPE_FOA_Calc 消息处理程序


BOOL CPE_FOA_Calc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//默认选中第2项radio
	m_radioRVA.SetCheck(TRUE);

	flag_radio = 2;	//radio选择标识默认为第二项2_RVA



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPE_FOA_Calc::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


//选择输入VA
void CPE_FOA_Calc::OnBnClickedRadioVa()
{
	// TODO: 在此添加控件通知处理程序代码

	//修改对应edit控件是否可写
	m_editVa.SetReadOnly(FALSE);
	m_editRva.SetReadOnly(TRUE);
	m_editFoa.SetReadOnly(TRUE);
	flag_radio = 1;


	UpdateData(TRUE);
	m_VA = "";
	m_RVA = "";
	m_FOA = "";
	m_Position = "";
	UpdateData(FALSE);
}

//选择输入RVA
void CPE_FOA_Calc::OnBnClickedRadioRva()
{
	// TODO: 在此添加控件通知处理程序代码

	//修改对应edit控件是否可写
	m_editVa.SetReadOnly(TRUE);
	m_editRva.SetReadOnly(FALSE);
	m_editFoa.SetReadOnly(TRUE);
	flag_radio = 2;

	UpdateData(TRUE);
	m_VA = "";
	m_RVA = "";
	m_FOA = "";
	m_Position = "";
	UpdateData(FALSE);
}

//选择输入FOA
void CPE_FOA_Calc::OnBnClickedRadioFoa()
{
	// TODO: 在此添加控件通知处理程序代码

	//修改对应edit控件是否可写
	m_editVa.SetReadOnly(TRUE);
	m_editRva.SetReadOnly(TRUE);
	m_editFoa.SetReadOnly(FALSE);
	flag_radio = 3;

	UpdateData(TRUE);
	m_VA = "";
	m_RVA = "";
	m_FOA = "";
	m_Position = "";
	UpdateData(FALSE);
}


//点击计算按钮
void CPE_FOA_Calc::OnBnClickedButtonCalc()
{
	// TODO: 在此添加控件通知处理程序代码

	if (flag_radio == 1)			//VA	
	{

	}
	else if (flag_radio == 2)		//RVA
	{
		UpdateData(TRUE);
		DWORD dwRVA = 0;
		DWORD dwFOA = 0;
		DWORD dwVA = 0;

		//将输入的数字当做十六进制转成10进制（因为下面函数里参数默认为10进制）
		//FOA
		dwRVA = _tcstol(m_RVA, NULL, 16);

		dwFOA = RVAtoFOA(dwRVA);

		m_FOA.Format(_T("%08X"), dwFOA);	//更新FOA（转回十六进制）

		//VA
		dwVA = _tcstol(m_RVA, NULL, 16);

		//判断64位 32位=====================================
		if (m_pNt32)
		{
			dwVA += m_pNt32->OptionalHeader.ImageBase;

		}
		else
		{
			dwVA += m_pNt64->OptionalHeader.ImageBase;

		}
		m_VA.Format(_T("%08X"), dwVA);

		//RVA
		dwRVA = _tcstol(m_RVA, NULL, 16);
		m_RVA.Format(_T("%08X"), dwRVA);

		UpdateData(FALSE);
	}
	else if (flag_radio == 3)		//FOA
	{
		UpdateData(TRUE);
		DWORD dwRVA = 0;
		DWORD dwFOA = 0;
		DWORD dwVA = 0;

		//将输入的数字当做十六进制转成10进制（因为下面函数里参数默认为10进制）
		//RVA
		dwFOA = _tcstol(m_FOA, NULL, 16);
		dwRVA = FOAtoRVA(dwFOA);
		m_RVA.Format(_T("%08X"), dwRVA);		//更新RVA（转回十六进制）

		//VA
		dwVA = _tcstol(m_RVA, NULL, 16);
		//判断64位 32位=====================================

		if (m_pNt32)
		{
			dwVA += m_pNt32->OptionalHeader.ImageBase;
		}
		else
		{
			dwVA += m_pNt64->OptionalHeader.ImageBase;
		}
		m_VA.Format(_T("%08X"), dwVA);


		//FOA
		dwFOA = _tcstol(m_FOA, NULL, 16);
		m_FOA.Format(_T("%08X"), dwFOA);



		UpdateData(FALSE);
	}
}



DWORD CPE_FOA_Calc::RVAtoFOA(DWORD dwRVA)
{
	//此RVA落在哪个区段中
	//找到所在区段后，减去所在区段的起始位置，加上在文件中的起始位置
	int nCountOfSection;
	PIMAGE_SECTION_HEADER pSec;
	DWORD dwSecAligment;

	//判断64位 32位
	if (m_pNt32)//32位======================================================
	{
		 nCountOfSection = m_pNt32->FileHeader.NumberOfSections;
		 pSec = IMAGE_FIRST_SECTION(m_pNt32);

		//块对齐数
		dwSecAligment = m_pNt32->OptionalHeader.SectionAlignment;
	}
	else//64位======================================================
	{
		 nCountOfSection = m_pNt64->FileHeader.NumberOfSections;
		 pSec = IMAGE_FIRST_SECTION(m_pNt64);

		//块对齐数
		dwSecAligment = m_pNt64->OptionalHeader.SectionAlignment;
	}

	for (int i = 0; i < nCountOfSection; i++)
	{
		//求在内存中的真实大小
		bool flag = pSec->Misc.VirtualSize % dwSecAligment;
		DWORD dwRealVirSize = 0;
		if (flag)
		{
			dwRealVirSize = pSec->Misc.VirtualSize / dwSecAligment;
			dwRealVirSize *= dwSecAligment;
			dwRealVirSize += dwSecAligment;

		}
		else
		{
			dwRealVirSize = pSec->VirtualAddress;
		}

		if (dwRVA >= pSec->VirtualAddress && dwRVA < pSec->VirtualAddress + dwRealVirSize)
		{
			if (m_editPosition)
			{
				m_editPosition.SetWindowText((CString)pSec->Name);
				m_Position = (CString)pSec->Name;
			}

			//FOA = RVA - 内存中区段的起始位置 + 在文件中区段的起始位置
			return dwRVA - pSec->VirtualAddress + pSec->PointerToRawData;
		}
		//下一个区段地址
		pSec++;
	}
	return 0;
}



DWORD  CPE_FOA_Calc::FOAtoRVA(DWORD dwFOA) 
{
	int nCountOfSection;
	PIMAGE_SECTION_HEADER pSec;

	//判断64位 32位
	if (m_pNt32)
	{
		 nCountOfSection = m_pNt32->FileHeader.NumberOfSections;
		 pSec = IMAGE_FIRST_SECTION(m_pNt32);

		 if (dwFOA < pSec->PointerToRawData)
		 {
			 return dwFOA + m_pNt32->OptionalHeader.ImageBase;
		 }

		 //否则判断FOA在哪个区段里
		 for (int i = 0; i < m_pNt32->FileHeader.NumberOfSections; i++)
		 {
			 if ((dwFOA >= pSec->PointerToRawData) &&
				 (dwFOA < (pSec->PointerToRawData + pSec->SizeOfRawData)))
			 {
				 if (m_editPosition)
				 {
					 m_editPosition.SetWindowText((CString)pSec->Name);
					 m_Position = (CString)pSec->Name;
				 }

				 //区段里FOA转RVA公式
				 //当前相对虚拟地址 = 当前文件偏移 - 当前区段的文件偏移 + 当前所在区段相对虚拟地址） + 进程的起始地址
				 return dwFOA - pSec->PointerToRawData + pSec->VirtualAddress;
			 }
		 }
	}
	else
	{
		 nCountOfSection = m_pNt64->FileHeader.NumberOfSections;
		 pSec = IMAGE_FIRST_SECTION(m_pNt64);

		 if (dwFOA < pSec->PointerToRawData)
		 {
			 return dwFOA + m_pNt64->OptionalHeader.ImageBase;
		 }

		 //否则判断FOA在哪个区段里
		 for (int i = 0; i < m_pNt64->FileHeader.NumberOfSections; i++)
		 {
			 if ((dwFOA >= pSec->PointerToRawData) &&
				 (dwFOA < (pSec->PointerToRawData + pSec->SizeOfRawData)))
			 {
				 if (m_editPosition)
				 {
					 m_editPosition.SetWindowText((CString)pSec->Name);
					 m_Position = (CString)pSec->Name;
				 }

				 //区段里FOA转RVA公式
				 //当前相对虚拟地址 = 当前文件偏移 - 当前区段的文件偏移 + 当前所在区段相对虚拟地址） + 进程的起始地址
				 return dwFOA - pSec->PointerToRawData + pSec->VirtualAddress;
			 }
		 }
	}

	//如果如果RVA是小于第一个区块的文件偏移PointerToRawData，那么它在文件中的偏移和在内存镜像中的偏移是一样的, 都是应该小于SizeOfHeaders
	

	return 0;
}