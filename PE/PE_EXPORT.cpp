// PE_EXPORT.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "PE_EXPORT.h"
#include "afxdialogex.h"


// CPE_EXPORT �Ի���

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


// CPE_EXPORT ��Ϣ�������


BOOL CPE_EXPORT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_list_export.AddColumn(4, L"���", 100, L"RVA", 140, L"ƫ��", 140, L"��������",140);

	if (m_pNt32)
	{
		SetExportDirectoryInfo32();

	}
	else
	{
		SetExportDirectoryInfo64();

	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CPE_EXPORT::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}

//���õ�������Ϣ32λ
void CPE_EXPORT::SetExportDirectoryInfo32()
{
	

	CPE_FOA_Calc objCalc(m_pNt32);

	//�ҵ�������
	DWORD dwExprotRVA = m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	PIMAGE_EXPORT_DIRECTORY pExprotRVA = (PIMAGE_EXPORT_DIRECTORY)(m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	//��ȡ���������ļ��е�λ��
	PIMAGE_EXPORT_DIRECTORY pExportFOA = (PIMAGE_EXPORT_DIRECTORY)(objCalc.RVAtoFOA(dwExprotRVA) + pFileImageBase);

	//��ַ���еĸ���
	DWORD dwCountOfFuntions = pExportFOA->NumberOfFunctions;

	//���Ʊ��еĸ���
	DWORD dwCountOfNames = pExportFOA->NumberOfNames;

	//��ַ���ַ
	PDWORD pAddrOfFuntion = (PDWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfFunctions) + pFileImageBase);

	//���Ʊ��ַ
	PDWORD pAddrOfName = (PDWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfNames) + pFileImageBase);

	//��ű��ַ
	PWORD pAddrOfOrdial = (PWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfNameOrdinals) + pFileImageBase);

	//baseֵ
	DWORD dwBase = pExportFOA->Base;


	for (DWORD i = 0; i < dwCountOfFuntions; i++)
	{
		//��ַ���п��ܴ������õ�ֵ��0��
		if (pAddrOfFuntion[i] == 0)
		{
			continue;
		}
		//������ű����Ƿ��� �� ��ַ���±�ֵ��ͬ ���ж��Ƿ������Ƶ���
		bool bRet = FALSE;
		for (unsigned j = 0; j < dwCountOfNames; j++)
		{
			if (i == pAddrOfOrdial[j])
			{
				//ȡ�����Ʊ��еĵ�ַ����RVA
				DWORD dwNameRVA = pAddrOfName[j];
				strName = (CString)(objCalc.RVAtoFOA(dwNameRVA) + pFileImageBase);
				bRet = true;
				break;
			}
		}
		if (!bRet)//��ű���û�У�˵��������ŵ���
		{
			//������
			strName.Format(L"N/A");
		}

		//RVA
		strRVA.Format(L"%08X", pAddrOfFuntion[i]);
		//FOA
		strOffset.Format(L"%08X", objCalc.RVAtoFOA(pAddrOfFuntion[i]));
		//���
		strID.Format(L"%08X", i + dwBase);

		m_list_export.AddItem(0, 4, strID, strRVA, strOffset, strName);



	}


	//ʱ��
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


//���õ�������Ϣ32λ
void CPE_EXPORT::SetExportDirectoryInfo64()
{


	CPE_FOA_Calc objCalc(m_pNt64);

	//�ҵ�������
	DWORD dwExprotRVA = m_pNt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;

	//��ȡ���������ļ��е�λ��
	PIMAGE_EXPORT_DIRECTORY pExportFOA = (PIMAGE_EXPORT_DIRECTORY)(objCalc.RVAtoFOA(dwExprotRVA) + pFileImageBase);

	//��ַ���еĸ���
	DWORD dwCountOfFuntions = pExportFOA->NumberOfFunctions;

	//���Ʊ��еĸ���
	DWORD dwCountOfNames = pExportFOA->NumberOfNames;

	//��ַ���ַ
	PDWORD pAddrOfFuntion = (PDWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfFunctions) + pFileImageBase);

	//���Ʊ��ַ
	PDWORD pAddrOfName = (PDWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfNames) + pFileImageBase);

	//��ű��ַ
	PWORD pAddrOfOrdial = (PWORD)(objCalc.RVAtoFOA(pExportFOA->AddressOfNameOrdinals) + pFileImageBase);

	//baseֵ
	DWORD dwBase = pExportFOA->Base;


	for (DWORD i = 0; i < dwCountOfFuntions; i++)
	{
		//��ַ���п��ܴ������õ�ֵ��0��
		if (pAddrOfFuntion[i] == 0)
		{
			continue;
		}
		//������ű����Ƿ��� �� ��ַ���±�ֵ��ͬ ���ж��Ƿ������Ƶ���
		bool bRet = FALSE;
		for (unsigned j = 0; j < dwCountOfNames; j++)
		{
			if (i == pAddrOfOrdial[j])
			{
				//ȡ�����Ʊ��еĵ�ַ����RVA
				DWORD dwNameRVA = pAddrOfName[j];
				strName = (CString)(objCalc.RVAtoFOA(dwNameRVA) + pFileImageBase);
				bRet = true;
				break;
			}
		}
		if (!bRet)//��ű���û�У�˵��������ŵ���
		{
			//������
			strName.Format(L"N/A");
		}

		//RVA
		strRVA.Format(L"%08X", pAddrOfFuntion[i]);
		//FOA
		strOffset.Format(L"%08X", objCalc.RVAtoFOA(pAddrOfFuntion[i]));
		//���
		strID.Format(L"%08X", i + dwBase);

		m_list_export.AddItem(0, 4, strID, strRVA, strOffset, strName);



	}


	//ʱ��
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




