// PE_FOA_Calc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "PE_FOA_Calc.h"
#include "afxdialogex.h"
#include <winnt.h>


// CPE_FOA_Calc �Ի���

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


// CPE_FOA_Calc ��Ϣ�������


BOOL CPE_FOA_Calc::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	//Ĭ��ѡ�е�2��radio
	m_radioRVA.SetCheck(TRUE);

	flag_radio = 2;	//radioѡ���ʶĬ��Ϊ�ڶ���2_RVA



	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CPE_FOA_Calc::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


//ѡ������VA
void CPE_FOA_Calc::OnBnClickedRadioVa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�޸Ķ�Ӧedit�ؼ��Ƿ��д
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

//ѡ������RVA
void CPE_FOA_Calc::OnBnClickedRadioRva()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�޸Ķ�Ӧedit�ؼ��Ƿ��д
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

//ѡ������FOA
void CPE_FOA_Calc::OnBnClickedRadioFoa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�޸Ķ�Ӧedit�ؼ��Ƿ��д
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


//������㰴ť
void CPE_FOA_Calc::OnBnClickedButtonCalc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (flag_radio == 1)			//VA	
	{

	}
	else if (flag_radio == 2)		//RVA
	{
		UpdateData(TRUE);
		DWORD dwRVA = 0;
		DWORD dwFOA = 0;
		DWORD dwVA = 0;

		//����������ֵ���ʮ������ת��10���ƣ���Ϊ���溯�������Ĭ��Ϊ10���ƣ�
		//FOA
		dwRVA = _tcstol(m_RVA, NULL, 16);

		dwFOA = RVAtoFOA(dwRVA);

		m_FOA.Format(_T("%08X"), dwFOA);	//����FOA��ת��ʮ�����ƣ�

		//VA
		dwVA = _tcstol(m_RVA, NULL, 16);

		//�ж�64λ 32λ=====================================
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

		//����������ֵ���ʮ������ת��10���ƣ���Ϊ���溯�������Ĭ��Ϊ10���ƣ�
		//RVA
		dwFOA = _tcstol(m_FOA, NULL, 16);
		dwRVA = FOAtoRVA(dwFOA);
		m_RVA.Format(_T("%08X"), dwRVA);		//����RVA��ת��ʮ�����ƣ�

		//VA
		dwVA = _tcstol(m_RVA, NULL, 16);
		//�ж�64λ 32λ=====================================

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
	//��RVA�����ĸ�������
	//�ҵ��������κ󣬼�ȥ�������ε���ʼλ�ã��������ļ��е���ʼλ��
	int nCountOfSection;
	PIMAGE_SECTION_HEADER pSec;
	DWORD dwSecAligment;

	//�ж�64λ 32λ
	if (m_pNt32)//32λ======================================================
	{
		 nCountOfSection = m_pNt32->FileHeader.NumberOfSections;
		 pSec = IMAGE_FIRST_SECTION(m_pNt32);

		//�������
		dwSecAligment = m_pNt32->OptionalHeader.SectionAlignment;
	}
	else//64λ======================================================
	{
		 nCountOfSection = m_pNt64->FileHeader.NumberOfSections;
		 pSec = IMAGE_FIRST_SECTION(m_pNt64);

		//�������
		dwSecAligment = m_pNt64->OptionalHeader.SectionAlignment;
	}

	for (int i = 0; i < nCountOfSection; i++)
	{
		//�����ڴ��е���ʵ��С
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

			//FOA = RVA - �ڴ������ε���ʼλ�� + ���ļ������ε���ʼλ��
			return dwRVA - pSec->VirtualAddress + pSec->PointerToRawData;
		}
		//��һ�����ε�ַ
		pSec++;
	}
	return 0;
}



DWORD  CPE_FOA_Calc::FOAtoRVA(DWORD dwFOA) 
{
	int nCountOfSection;
	PIMAGE_SECTION_HEADER pSec;

	//�ж�64λ 32λ
	if (m_pNt32)
	{
		 nCountOfSection = m_pNt32->FileHeader.NumberOfSections;
		 pSec = IMAGE_FIRST_SECTION(m_pNt32);

		 if (dwFOA < pSec->PointerToRawData)
		 {
			 return dwFOA + m_pNt32->OptionalHeader.ImageBase;
		 }

		 //�����ж�FOA���ĸ�������
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

				 //������FOAתRVA��ʽ
				 //��ǰ��������ַ = ��ǰ�ļ�ƫ�� - ��ǰ���ε��ļ�ƫ�� + ��ǰ����������������ַ�� + ���̵���ʼ��ַ
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

		 //�����ж�FOA���ĸ�������
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

				 //������FOAתRVA��ʽ
				 //��ǰ��������ַ = ��ǰ�ļ�ƫ�� - ��ǰ���ε��ļ�ƫ�� + ��ǰ����������������ַ�� + ���̵���ʼ��ַ
				 return dwFOA - pSec->PointerToRawData + pSec->VirtualAddress;
			 }
		 }
	}

	//������RVA��С�ڵ�һ��������ļ�ƫ��PointerToRawData����ô�����ļ��е�ƫ�ƺ����ڴ澵���е�ƫ����һ����, ����Ӧ��С��SizeOfHeaders
	

	return 0;
}