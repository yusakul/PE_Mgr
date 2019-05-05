// PE_Relocation.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "PE_Relocation.h"
#include "afxdialogex.h"
#include <winnt.h>


// CPE_Relocation �Ի���

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


// CPE_Relocation ��Ϣ�������


void CPE_Relocation::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


BOOL CPE_Relocation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_list_relocation_section.AddColumn(4, L"����",50, L"Section",100, L"RVA", 100,L"Items",100);
	m_list_relocation_block.AddColumn(6, L"����",50, L"RVA",100, L"Offset",100, L"Type",150, L"Far Address",200, L"���ݽ���",150);

	if (m_pNt32)
	{
		GetRelocInfo32();
	}
	else
	{
		
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



//��ȡ�ض�λ������Ϣ
void CPE_Relocation::GetRelocInfo32()
{
	//�ض�λ����һ����һ�����ϵ��ض�λ��ṹ�����
	//�ض�λ����һ�����ض�λ��ṹ��ļ��ϣ�Ҳ����һ�����飩����������һ��ȫ0��Ԫ����Ϊ
	//�����ı�־.

	CPE_FOA_Calc objCalc(m_pNt32);

	//�ض�λ��RVA
	QWORD dwRelocRVA = m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;

	//�Ƿ�Ϊ��
	if (!dwRelocRVA)
	{
		MessageBox(L"û���ض�λ��...\n", 0, 0);
		return;
	}

	//�ض�λ�����ļ��е�ַ
	QWORD dwRelocFOA = (QWORD)(objCalc.RVAtoFOA(dwRelocRVA) + pFileImageBase);
	PIMAGE_BASE_RELOCATION pRelocFOA = (PIMAGE_BASE_RELOCATION)(dwRelocFOA);

	for (int i = 0; pRelocFOA->VirtualAddress ; i++)	//��Ҫ�ض�λ��λ�ã�����һ����������ַ��RVA��
	{
		RELOCAREAINFO StructRelocAreaInfo;

		//��ȡ�ض�λ��ĸ���
		DWORD dwCount = (pRelocFOA->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

		//���
		m_Sec_Index.Format(L"%d", i + 1);
		StructRelocAreaInfo.index = m_Sec_Index;

		//����
		m_Sec_Section = GetSectionName(pRelocFOA->VirtualAddress);
		StructRelocAreaInfo.Section = m_Sec_Section;

		//��Ҫ�ض�λ��λ��RVA
		m_Sec_RVA.Format(L"%08X", pRelocFOA->VirtualAddress);
		StructRelocAreaInfo.RVA = m_Sec_RVA;

		//��Ŀ
		m_Sec_Items.Format(L"%0Xh / %dd", dwCount, dwCount);
		StructRelocAreaInfo.Items = m_Sec_Items;
		
		m_vecRelocAreaInfo.push_back(StructRelocAreaInfo);

		m_vecPReloc.push_back(pRelocFOA);

		
		// �ҵ���һ���ض�λ��
		pRelocFOA = (IMAGE_BASE_RELOCATION*)((QWORD)pRelocFOA + pRelocFOA->SizeOfBlock);
	}

	for (int i = m_vecRelocAreaInfo.size()-1;i>=0;  i--)
	{
		m_list_relocation_section.AddItem(0, 4, m_vecRelocAreaInfo[i].index, m_vecRelocAreaInfo[i].Section, m_vecRelocAreaInfo[i].RVA, m_vecRelocAreaInfo[i].Items);
	}
}

//��ȡ�ض�λ������Ϣ
void CPE_Relocation::GetRelocInfo64()
{
	//�ض�λ����һ����һ�����ϵ��ض�λ��ṹ�����
	//�ض�λ����һ�����ض�λ��ṹ��ļ��ϣ�Ҳ����һ�����飩����������һ��ȫ0��Ԫ����Ϊ
	//�����ı�־.

	CPE_FOA_Calc objCalc(m_pNt64);

	//�ض�λ��RVA
	QWORD dwRelocRVA = m_pNt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;

	//�Ƿ�Ϊ��
	if (!dwRelocRVA)
	{
		MessageBox(L"û���ض�λ��...\n", 0, 0);
		return;
	}

	//�ض�λ�����ļ��е�ַ
	QWORD dwRelocFOA = (QWORD)(objCalc.RVAtoFOA(dwRelocRVA) + pFileImageBase);
	PIMAGE_BASE_RELOCATION pRelocFOA = (PIMAGE_BASE_RELOCATION)(dwRelocFOA);

	for (int i = 0; pRelocFOA->VirtualAddress; i++)	//��Ҫ�ض�λ��λ�ã�����һ����������ַ��RVA��
	{
		RELOCAREAINFO StructRelocAreaInfo;

		//��ȡ�ض�λ��ĸ���
		DWORD dwCount = (pRelocFOA->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

		//���
		m_Sec_Index.Format(L"%d", i + 1);
		StructRelocAreaInfo.index = m_Sec_Index;

		//����
		m_Sec_Section = GetSectionName(pRelocFOA->VirtualAddress);
		StructRelocAreaInfo.Section = m_Sec_Section;

		//��Ҫ�ض�λ��λ��RVA
		m_Sec_RVA.Format(L"%08X", pRelocFOA->VirtualAddress);
		StructRelocAreaInfo.RVA = m_Sec_RVA;

		//��Ŀ
		m_Sec_Items.Format(L"%0Xh / %dd", dwCount, dwCount);
		StructRelocAreaInfo.Items = m_Sec_Items;

		m_vecRelocAreaInfo.push_back(StructRelocAreaInfo);

		m_vecPReloc.push_back(pRelocFOA);


		// �ҵ���һ���ض�λ��
		pRelocFOA = (IMAGE_BASE_RELOCATION*)((QWORD)pRelocFOA + pRelocFOA->SizeOfBlock);
	}

	for (int i = m_vecRelocAreaInfo.size() - 1; i >= 0; i--)
	{
		m_list_relocation_section.AddItem(0, 4, m_vecRelocAreaInfo[i].index, m_vecRelocAreaInfo[i].Section, m_vecRelocAreaInfo[i].RVA, m_vecRelocAreaInfo[i].Items);
	}
}


//��ȡ������
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
	//ѭ������
	for (int i = 0; i < NumberOfSec; i++)
	{
		//�ȶԵ�ַ
		if (dwRVA >= pSec[i].VirtualAddress&&
			dwRVA <= pSec[i].VirtualAddress + pSec[i].SizeOfRawData)
		{
			//������	RVA	VS	FO	FS
			CHAR pName[9] = {};
			memcpy_s(pName, 9, pSec[i].Name, 8);
			return  (CString)pName;
			
		}
	}

	return L"--";
}


//��ȡ������ϸ��Ϣ
void CPE_Relocation::GetRelocDataInfo32(PIMAGE_BASE_RELOCATION pReloc)
{
	m_list_relocation_block.DeleteAllItems();
	std::vector<RELOCINFO> vecRelocInfo;

	//��ȡ�ض�λ��Ŀ�ʼ��ַ
	POFFSET_TYPE pTypeOffset = (POFFSET_TYPE)(pReloc + 1);

	//��ȡ�ض�λ��ĸ���
	DWORD dwCount = (pReloc->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

	CPE_FOA_Calc objCalc(m_pNt32);

	for (unsigned i = 0; i < dwCount; i++)
	{
		RELOCINFO structRelocInfo;

		if (pTypeOffset[i].type == 3)	//����ṹ�� �ĸ���λ������˵�����ض�λ������� 3������Ҫ����
		{
			//��ȡ��Ҫ�޸���RVA
			DWORD dwRelocDataRVA = pReloc->VirtualAddress + pTypeOffset[i].offset;
			QWORD dwRelocDataFOA = (QWORD)(objCalc.RVAtoFOA(dwRelocDataRVA) + pFileImageBase);

			//��Ҫ�޸����ض�λ���ݵ�ַ�Ǹ�VA
			DWORD dwRealDataVA = *(DWORD*)dwRelocDataFOA;

			//ת��RVA���õ�FOA
			DWORD dwRealDataRVA = dwRealDataVA - m_pNt32->OptionalHeader.ImageBase;
			QWORD dwRealDataFOA = (QWORD)(objCalc.RVAtoFOA(dwRealDataRVA) + pFileImageBase);

			//���
			m_Block_Index.Format(L"%d", i + 1);
			structRelocInfo.index = m_Block_Index;

			//��Ҫ�ض�λ��λ��RVA
			m_Block_RVA.Format(L"%08X", dwRelocDataRVA);
			structRelocInfo.dwRelocRVa = m_Block_RVA;

			//ƫ��
			m_Block_Offset.Format(L"%08X", dwRelocDataFOA);
			structRelocInfo.dwOffset = m_Block_Offset;

			//����
			m_Block_Type.Format(L"HIGHLOW ��3��");
			structRelocInfo.bType = m_Block_Type;

			//FAR��ַ
			m_Block_FarAddress.Format(L"%08X", dwRealDataVA);
			structRelocInfo.dwRelocValue = m_Block_FarAddress;

			//���ݽ���
			//��Ҫ�ض�λ�ľ������ݣ��ֽ�����ȷ����
			byte* dwData = (byte*)dwRealDataFOA;

			if ((*dwData >= 'A'&& *dwData <= 'z'))	//�ж���������
			{
				/*m_Block_DataInterpretaion.Format(L"%s", pData);*/
				m_Block_DataInterpretaion = dwData;
			}
			else
			{
				//���ʮ�������ֽ�
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

//��ȡ������ϸ��Ϣ
void CPE_Relocation::GetRelocDataInfo64(PIMAGE_BASE_RELOCATION pReloc)
{
	m_list_relocation_block.DeleteAllItems();
	std::vector<RELOCINFO> vecRelocInfo;

	//��ȡ�ض�λ��Ŀ�ʼ��ַ
	POFFSET_TYPE pTypeOffset = (POFFSET_TYPE)(pReloc + 1);

	//��ȡ�ض�λ��ĸ���
	DWORD dwCount = (pReloc->SizeOfBlock - (sizeof(IMAGE_BASE_RELOCATION))) / sizeof(WORD);

	CPE_FOA_Calc objCalc(m_pNt64);

	for (unsigned i = 0; i < dwCount; i++)
	{
		RELOCINFO structRelocInfo;

		if (pTypeOffset[i].type == 3)	//����ṹ�� �ĸ���λ������˵�����ض�λ������� 3������Ҫ����
		{
			//��ȡ��Ҫ�޸���RVA
			DWORD dwRelocDataRVA = pReloc->VirtualAddress + pTypeOffset[i].offset;
			QWORD dwRelocDataFOA = (QWORD)(objCalc.RVAtoFOA(dwRelocDataRVA) + pFileImageBase);

			//��Ҫ�޸����ض�λ���ݵ�ַ�Ǹ�VA
			DWORD dwRealDataVA = *(DWORD*)dwRelocDataFOA;

			//ת��RVA���õ�FOA
			DWORD dwRealDataRVA = dwRealDataVA - m_pNt64->OptionalHeader.ImageBase;
			QWORD dwRealDataFOA = (QWORD)(objCalc.RVAtoFOA(dwRealDataRVA) + pFileImageBase);

			//���
			m_Block_Index.Format(L"%d", i + 1);
			structRelocInfo.index = m_Block_Index;

			//��Ҫ�ض�λ��λ��RVA
			m_Block_RVA.Format(L"%08X", dwRelocDataRVA);
			structRelocInfo.dwRelocRVa = m_Block_RVA;

			//ƫ��
			m_Block_Offset.Format(L"%08X", dwRelocDataFOA);
			structRelocInfo.dwOffset = m_Block_Offset;

			//����
			m_Block_Type.Format(L"HIGHLOW ��3��");
			structRelocInfo.bType = m_Block_Type;

			//FAR��ַ
			m_Block_FarAddress.Format(L"%08X", dwRealDataVA);
			structRelocInfo.dwRelocValue = m_Block_FarAddress;

			//���ݽ���
			//��Ҫ�ض�λ�ľ������ݣ��ֽ�����ȷ����
			byte* dwData = (byte*)dwRealDataFOA;

			if ((*dwData >= 'A'&& *dwData <= 'z'))	//�ж���������
			{
				/*m_Block_DataInterpretaion.Format(L"%s", pData);*/
				m_Block_DataInterpretaion = dwData;
			}
			else
			{
				//���ʮ�������ֽ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;


	

	CString stIndex;

	if (pNMItemActivate->iItem != -1)//���GetItemText��Ŀ����δ����֪ͨ����˳�Ա������-1��
	{
		stIndex = m_list_relocation_section.GetItemText(pNMItemActivate->iItem, 0);
		int index = _tcstol(stIndex, NULL,10)-1;

		//��ȡ��Ϣ
		GetRelocDataInfo32(m_vecPReloc[index]);
	}
}
