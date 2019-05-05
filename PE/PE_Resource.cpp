// PE_Resource.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "PE_Resource.h"
#include "afxdialogex.h"


// CPE_Resource �Ի���

IMPLEMENT_DYNAMIC(CPE_Resource, CDialogEx)

CPE_Resource::CPE_Resource(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RESOURCE, pParent)
	, m_Resource_ID1(_T(""))
	, m_Resource_ID2(_T(""))
	, m_Resource_Name1(_T(""))
	, m_Resource_Name2(_T(""))
	, m_Resource_Offset(_T(""))
	, m_Resource_Rva(_T(""))
	, m_Resource_Size(_T(""))
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt32 = pNt32;
	m_pDesc = m_pNt32->OptionalHeader.DataDirectory;
}

CPE_Resource::CPE_Resource(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RESOURCE, pParent)
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt64 = pNt64;
	m_pDesc = m_pNt32->OptionalHeader.DataDirectory;
}

CPE_Resource::~CPE_Resource()
{
}

void CPE_Resource::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RESOURCE_ID1, m_Resource_ID1);
	DDX_Text(pDX, IDC_EDIT_RESOURCE_ID2, m_Resource_ID2);
	DDX_Text(pDX, IDC_EDIT_RESOURCE_NAME1, m_Resource_Name1);
	DDX_Text(pDX, IDC_EDIT_RESOURCE_NAME2, m_Resource_Name2);
	DDX_Text(pDX, IDC_EDIT_RESOURCE_OFFSET, m_Resource_Offset);
	DDX_Text(pDX, IDC_EDIT_RESOURCE_RVA, m_Resource_Rva);
	DDX_Text(pDX, IDC_EDIT_RESOURCE_SIZE, m_Resource_Size);
	DDX_Control(pDX, IDC_TREE_RESOURCE, m_Tree_Resource);
}


BEGIN_MESSAGE_MAP(CPE_Resource, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPE_Resource ��Ϣ�������


BOOL CPE_Resource::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


	if (m_pNt32)
	{
		ShowResource32();

	}
	else
	{
		ShowResource64();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CPE_Resource::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}






//��������ƫ�Ƴ�Ա��Ϊ�������Դ���ʼ��λ�ã�PIMAGE_RESOURCE_DIRECTORY��ƫ�� �õ������һ���ṹ���г�Աoffsettodate��һ��RVA
void CPE_Resource::ShowResource32()
{
	CPE_FOA_Calc objCalc(m_pNt32);

	//��Դ��RVA
	QWORD dwResourceRVA = m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
	PIMAGE_RESOURCE_DIRECTORY pResourceRVA = (PIMAGE_RESOURCE_DIRECTORY)(dwResourceRVA);

	//��Դ�����ļ���λ��
	QWORD dwResourceFOA = (QWORD)(objCalc.RVAtoFOA(dwResourceRVA) + pFileImageBase);
	PIMAGE_RESOURCE_DIRECTORY pResourceFOA = (PIMAGE_RESOURCE_DIRECTORY)(dwResourceFOA);


	//��һ��(����)
	//�������
	QWORD dwCountOfResType = pResourceFOA->NumberOfNamedEntries + pResourceFOA->NumberOfIdEntries;

	//��ȡĿ¼���׵�ַ
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourceEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResourceFOA + 1);

	for (unsigned i = 0; i < dwCountOfResType; i++)
	{


		//�ж���Դ���ַ�������ID����һ�����������λΪ0,��NameIsString=0����ΪID������
		if (pResourceEntry->NameIsString)	//�ַ�����Ϊ��Դ��ʶ
		{
			//������ַ�����NameOffset����ľ����ַ�����RVA

			//��ȡ�����ַ�����FOA
			QWORD dwNameFOA = (QWORD)(objCalc.RVAtoFOA(pResourceEntry->NameOffset) + pResourceFOA);

			//NameOffset��ָ��Ľṹ��_IMAGE_RESOUCE_DIR_STRING_U,�������ַ����ĳ��Ⱥ���ʼλ��
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;

			//������ַ���������0��β������'\0'
			WCHAR* pResName = new WCHAR[pName->Length + 1]{};
			memcpy(pResName, pName, (pName->Length) * sizeof(WCHAR));

			// ���ڵ� ����ɹ���������Ŀ�ľ�� ���� NULL
			hParent = m_Tree_Resource.InsertItem(
				pResName,	/*������Ŀ�ı����ַ����ĵ�ַ*/
				NULL,			/*��Ŀ��ͼ�������������ͼ�ؼ�ͼ�ε��б�*/
				NULL,			/*��Ŀ�е�ѡ��ͼ�������������ͼ�ؼ�ͼ�ε��б�*/
				NULL,			/*������ĸ����ľ��*/
				TVI_LAST);		/*Ҫ�������Ŀ��Ȼ�������Ŀ�ľ��*/

			delete[] pResName;
		}
		else  //ID 
		{
			char* arryResType[] = {
				"",
				"���ָ�루Cursor)",			//1
				"λͼ��Bitmap)",				//2
				"ͼ�꣨Icon��",					//3
				"�˵���Menu��",					//4
				"�Ի���Dialog��",				//5
				"�ַ����б�String Table��",	//6
				"����Ŀ¼��Font Directory��",	//7
				"���壨Font��",					//8
				"��ݼ���Accelerators��",		//9
				"�Ǹ�ʽ����Դ��Unformatted��",	//A
				"��Ϣ�б�Message Table��",	//B
				"���ָ���飨Croup Cursor��",	//C
				"",
				"ͼ���飨Group Icon��",			//E
				"",
				"�汾��Ϣ��Version Information��" ,//10
			};


			if (pResourceEntry->Id <= 16)
			{
				hParent = m_Tree_Resource.InsertItem((CString)(arryResType[pResourceEntry->Id]), NULL, NULL, NULL, TVI_LAST);
			}
			else
			{
				CString strTemp;
				strTemp.Format(L"%d", pResourceEntry->Id);
				hParent = m_Tree_Resource.InsertItem(strTemp, NULL, NULL, NULL, TVI_LAST);
			}

			//�ж��Ƿ��еڶ���
			if (pResourceEntry->DataIsDirectory)
			{
				QWORD dwResSecond = (QWORD)pResourceFOA + pResourceEntry->OffsetToDirectory;
				PIMAGE_RESOURCE_DIRECTORY pResSecond = (PIMAGE_RESOURCE_DIRECTORY)dwResSecond;


				//�ڶ������
				QWORD dwCountOfSecond = pResSecond->NumberOfIdEntries + pResSecond->NumberOfNamedEntries;

				//�ڶ����ṹ��
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pResSecondEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResSecond + 1);
				//����ÿһ����Դ
				for (unsigned iSecond = 0; iSecond < dwCountOfSecond; iSecond++)
				{
					//�ж�������Դ���ַ�������ID
					if (pResourceEntry->NameIsString)
					{
						//������ַ�����NameOffset����ľ�������ַ�����RVA
						//�õ������ַ�����FOA
						QWORD dwNameFOA = (QWORD)(objCalc.RVAtoFOA(pResourceEntry->NameOffset) + pResourceFOA);
						//NameOffset��ָ��Ľṹ����IMAGE_RESOURCE_DIR_STRING_U����
						//���ﱣ�����ַ����ĳ��Ⱥ���ʼλ��
						PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;
						//������ַ���������0��β�ģ�������Ҫ�����������ϡ�\0����β���ٴ�ӡ
						WCHAR *pResName = new WCHAR[pName->Length + 1]{};
						memcpy(pResName, pName, (pName->Length) * sizeof(WCHAR));

						hInsertAfter = m_Tree_Resource.InsertItem(pResName, NULL, NULL, hParent, TVI_LAST);

						delete[] pResName;
					}
					else //id
					{
						CString strID;
						strID.Format(L"%d", pResSecondEntry->Id);
						hInsertAfter = m_Tree_Resource.InsertItem(strID, NULL, NULL, hParent, TVI_LAST);
					}

					//�ж���û�е�����
					if (pResourceEntry->DataIsDirectory)
					{
						//���������ʼλ��
						QWORD dwResThrid =
							(QWORD)pResourceFOA + pResSecondEntry->OffsetToDirectory;
						PIMAGE_RESOURCE_DIRECTORY pResThrid =
							(PIMAGE_RESOURCE_DIRECTORY)dwResThrid;

						PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThridEntry =
							(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResThrid + 1);

						//�����㣬ʹ��PIMAGE_RESOURCE_DIRECTORY_ENTRY�ĳ�Ա
						//OffsetToData��Ա���õ�PIMAGE_RESOURCE_DATA_ENTRY��λ��
						PIMAGE_RESOURCE_DATA_ENTRY pResData =
							(PIMAGE_RESOURCE_DATA_ENTRY)(pResThridEntry->OffsetToData + (QWORD)pResourceFOA);

						//��Դ��rva��size
						QWORD dwResDataRVA = pResData->OffsetToData;
						QWORD dwresDataSize = pResData->Size;

						//PIMAGE_RESOURCE_DATA_ENTRY�е�offsettodata��RVA 
						QWORD dwResDataFOA = (QWORD)(objCalc.RVAtoFOA(dwResDataRVA));

						CString strTemp;
						strTemp.Format(L"RVA:%08X", pResData->OffsetToData);
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

						strTemp.Format(L"ƫ��:%08X", objCalc.RVAtoFOA(pResData->OffsetToData));
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

						strTemp.Format(L"Size:%08X", pResData->Size);
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

					}
					//��һ����Դ
					pResSecondEntry++;
				}
			}
		}
		//��һ����Դ
		pResourceEntry++;
	}
}




//��������ƫ�Ƴ�Ա��Ϊ�������Դ���ʼ��λ�ã�PIMAGE_RESOURCE_DIRECTORY��ƫ�� �õ������һ���ṹ���г�Աoffsettodate��һ��RVA
void CPE_Resource::ShowResource64()
{
	CPE_FOA_Calc objCalc(m_pNt64);

	//��Դ��RVA
	QWORD dwResourceRVA = m_pNt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
	PIMAGE_RESOURCE_DIRECTORY pResourceRVA = (PIMAGE_RESOURCE_DIRECTORY)(dwResourceRVA);

	//��Դ�����ļ���λ��
	QWORD dwResourceFOA = (QWORD)(objCalc.RVAtoFOA(dwResourceRVA) + pFileImageBase);
	PIMAGE_RESOURCE_DIRECTORY pResourceFOA = (PIMAGE_RESOURCE_DIRECTORY)(dwResourceFOA);


	//��һ��(����)
	//�������
	QWORD dwCountOfResType = pResourceFOA->NumberOfNamedEntries + pResourceFOA->NumberOfIdEntries;

	//��ȡĿ¼���׵�ַ
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourceEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResourceFOA + 1);

	for (unsigned i = 0; i < dwCountOfResType; i++)
	{
		

		//�ж���Դ���ַ�������ID����һ�����������λΪ0,��NameIsString=0����ΪID������
		if (pResourceEntry->NameIsString)	//�ַ�����Ϊ��Դ��ʶ
		{
			//������ַ�����NameOffset����ľ����ַ�����RVA

			//��ȡ�����ַ�����FOA
			QWORD dwNameFOA = (QWORD)(objCalc.RVAtoFOA(pResourceEntry->NameOffset) + pResourceFOA);

			//NameOffset��ָ��Ľṹ��_IMAGE_RESOUCE_DIR_STRING_U,�������ַ����ĳ��Ⱥ���ʼλ��
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;

			//������ַ���������0��β������'\0'
			WCHAR* pResName = new WCHAR[pName->Length + 1]{};
			memcpy(pResName, pName, (pName->Length) * sizeof(WCHAR));

			// ���ڵ� ����ɹ���������Ŀ�ľ�� ���� NULL
			hParent = m_Tree_Resource.InsertItem(
				pResName,	/*������Ŀ�ı����ַ����ĵ�ַ*/
				NULL,			/*��Ŀ��ͼ�������������ͼ�ؼ�ͼ�ε��б�*/
				NULL,			/*��Ŀ�е�ѡ��ͼ�������������ͼ�ؼ�ͼ�ε��б�*/
				NULL,			/*������ĸ����ľ��*/
				TVI_LAST);		/*Ҫ�������Ŀ��Ȼ�������Ŀ�ľ��*/

			delete[] pResName;
		}
		else  //ID 
		{
			char* arryResType[] = {
				"",
				"���ָ�루Cursor)",			//1
				"λͼ��Bitmap)",				//2
				"ͼ�꣨Icon��",					//3
				"�˵���Menu��",					//4
				"�Ի���Dialog��",				//5
				"�ַ����б�String Table��",	//6
				"����Ŀ¼��Font Directory��",	//7
				"���壨Font��",					//8
				"��ݼ���Accelerators��",		//9
				"�Ǹ�ʽ����Դ��Unformatted��",	//A
				"��Ϣ�б�Message Table��",	//B
				"���ָ���飨Croup Cursor��",	//C
				"",
				"ͼ���飨Group Icon��",			//E
				"",
				"�汾��Ϣ��Version Information��" ,//10
			};


			if (pResourceEntry->Id <= 16)
			{
				hParent = m_Tree_Resource.InsertItem((CString)(arryResType[pResourceEntry->Id]), NULL, NULL, NULL, TVI_LAST);
			}
			else
			{
				CString strTemp;
				strTemp.Format(L"%d", pResourceEntry->Id);
				hParent = m_Tree_Resource.InsertItem(strTemp, NULL, NULL, NULL, TVI_LAST);
			}

			//�ж��Ƿ��еڶ���
			if (pResourceEntry->DataIsDirectory)
			{
				QWORD dwResSecond = (QWORD)pResourceFOA + pResourceEntry->OffsetToDirectory;
				PIMAGE_RESOURCE_DIRECTORY pResSecond = (PIMAGE_RESOURCE_DIRECTORY)dwResSecond;


				//�ڶ������
				QWORD dwCountOfSecond =pResSecond->NumberOfIdEntries + pResSecond->NumberOfNamedEntries;

				//�ڶ����ṹ��
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pResSecondEntry =(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResSecond + 1);
				//����ÿһ����Դ
				for (unsigned iSecond = 0; iSecond < dwCountOfSecond; iSecond++)
				{
					//�ж�������Դ���ַ�������ID
					if (pResourceEntry->NameIsString)
					{
						//������ַ�����NameOffset����ľ�������ַ�����RVA
						//�õ������ַ�����FOA
						QWORD dwNameFOA = (QWORD)(objCalc.RVAtoFOA(pResourceEntry->NameOffset) + pResourceFOA);
						//NameOffset��ָ��Ľṹ����IMAGE_RESOURCE_DIR_STRING_U����
						//���ﱣ�����ַ����ĳ��Ⱥ���ʼλ��
						PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;
						//������ַ���������0��β�ģ�������Ҫ�����������ϡ�\0����β���ٴ�ӡ
						WCHAR *pResName = new WCHAR[pName->Length + 1]{};
						memcpy(pResName, pName, (pName->Length) * sizeof(WCHAR));

						hInsertAfter = m_Tree_Resource.InsertItem(pResName, NULL, NULL, hParent, TVI_LAST);

						delete[] pResName;
					}
					else //id
					{
						CString strID;
						strID.Format(L"%d", pResSecondEntry->Id);
						hInsertAfter = m_Tree_Resource.InsertItem(strID, NULL, NULL, hParent, TVI_LAST);
					}

					//�ж���û�е�����
					if (pResourceEntry->DataIsDirectory)
					{
						//���������ʼλ��
						QWORD dwResThrid =
							(QWORD)pResourceFOA + pResSecondEntry->OffsetToDirectory;
						PIMAGE_RESOURCE_DIRECTORY pResThrid =
							(PIMAGE_RESOURCE_DIRECTORY)dwResThrid;

						PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThridEntry =
							(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResThrid + 1);

						//�����㣬ʹ��PIMAGE_RESOURCE_DIRECTORY_ENTRY�ĳ�Ա
						//OffsetToData��Ա���õ�PIMAGE_RESOURCE_DATA_ENTRY��λ��
						PIMAGE_RESOURCE_DATA_ENTRY pResData =
							(PIMAGE_RESOURCE_DATA_ENTRY)(pResThridEntry->OffsetToData + (QWORD)pResourceFOA);

						//��Դ��rva��size
						QWORD dwResDataRVA = pResData->OffsetToData;
						QWORD dwresDataSize = pResData->Size;

						//PIMAGE_RESOURCE_DATA_ENTRY�е�offsettodata��RVA 
						QWORD dwResDataFOA = (QWORD)(objCalc.RVAtoFOA(dwResDataRVA));

						CString strTemp;
						strTemp.Format(L"RVA:%08X", pResData->OffsetToData);
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

						strTemp.Format(L"ƫ��:%08X", objCalc.RVAtoFOA(pResData->OffsetToData));
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

						strTemp.Format(L"Size:%08X", pResData->Size);
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

					}
					//��һ����Դ
					pResSecondEntry++;
				}
			}
		}
 		//��һ����Դ
		pResourceEntry++;
	}
}


	


