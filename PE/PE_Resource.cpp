// PE_Resource.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_Resource.h"
#include "afxdialogex.h"


// CPE_Resource 对话框

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


// CPE_Resource 消息处理程序


BOOL CPE_Resource::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	if (m_pNt32)
	{
		ShowResource32();

	}
	else
	{
		ShowResource64();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPE_Resource::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}






//联合体中偏移成员均为相对于资源表最开始的位置（PIMAGE_RESOURCE_DIRECTORY）偏移 得到的最后一个结构体中成员offsettodate是一个RVA
void CPE_Resource::ShowResource32()
{
	CPE_FOA_Calc objCalc(m_pNt32);

	//资源表RVA
	QWORD dwResourceRVA = m_pNt32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
	PIMAGE_RESOURCE_DIRECTORY pResourceRVA = (PIMAGE_RESOURCE_DIRECTORY)(dwResourceRVA);

	//资源表在文件中位置
	QWORD dwResourceFOA = (QWORD)(objCalc.RVAtoFOA(dwResourceRVA) + pFileImageBase);
	PIMAGE_RESOURCE_DIRECTORY pResourceFOA = (PIMAGE_RESOURCE_DIRECTORY)(dwResourceFOA);


	//第一层(种类)
	//种类个数
	QWORD dwCountOfResType = pResourceFOA->NumberOfNamedEntries + pResourceFOA->NumberOfIdEntries;

	//获取目录项首地址
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourceEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResourceFOA + 1);

	for (unsigned i = 0; i < dwCountOfResType; i++)
	{


		//判断资源是字符串还是ID：第一个联合体最高位为0,即NameIsString=0，则为ID起作用
		if (pResourceEntry->NameIsString)	//字符串作为资源标识
		{
			//如果是字符串，NameOffset保存的就是字符串的RVA

			//获取名字字符串的FOA
			QWORD dwNameFOA = (QWORD)(objCalc.RVAtoFOA(pResourceEntry->NameOffset) + pResourceFOA);

			//NameOffset所指向的结构体_IMAGE_RESOUCE_DIR_STRING_U,保存了字符串的长度和起始位置
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;

			//这里的字符串不是以0结尾，加上'\0'
			WCHAR* pResName = new WCHAR[pName->Length + 1]{};
			memcpy(pResName, pName, (pName->Length) * sizeof(WCHAR));

			// 根节点 如果成功返回新项目的句柄 否则 NULL
			hParent = m_Tree_Resource.InsertItem(
				pResName,	/*包含项目文本的字符串的地址*/
				NULL,			/*项目的图像的索引在树视图控件图形的列表*/
				NULL,			/*项目中的选定图像的索引在树视图控件图形的列表*/
				NULL,			/*插入项的父级的句柄*/
				TVI_LAST);		/*要插入该项目，然后后新项目的句柄*/

			delete[] pResName;
		}
		else  //ID 
		{
			char* arryResType[] = {
				"",
				"鼠标指针（Cursor)",			//1
				"位图（Bitmap)",				//2
				"图标（Icon）",					//3
				"菜单（Menu）",					//4
				"对话框（Dialog）",				//5
				"字符串列表（String Table）",	//6
				"字体目录（Font Directory）",	//7
				"字体（Font）",					//8
				"快捷键（Accelerators）",		//9
				"非格式化资源（Unformatted）",	//A
				"消息列表（Message Table）",	//B
				"鼠标指针组（Croup Cursor）",	//C
				"",
				"图标组（Group Icon）",			//E
				"",
				"版本信息（Version Information）" ,//10
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

			//判断是否有第二层
			if (pResourceEntry->DataIsDirectory)
			{
				QWORD dwResSecond = (QWORD)pResourceFOA + pResourceEntry->OffsetToDirectory;
				PIMAGE_RESOURCE_DIRECTORY pResSecond = (PIMAGE_RESOURCE_DIRECTORY)dwResSecond;


				//第二层个数
				QWORD dwCountOfSecond = pResSecond->NumberOfIdEntries + pResSecond->NumberOfNamedEntries;

				//第二个结构体
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pResSecondEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResSecond + 1);
				//遍历每一个资源
				for (unsigned iSecond = 0; iSecond < dwCountOfSecond; iSecond++)
				{
					//判断这种资源是字符串还是ID
					if (pResourceEntry->NameIsString)
					{
						//如果是字符串，NameOffset保存的就是这个字符串的RVA
						//得到名字字符串的FOA
						QWORD dwNameFOA = (QWORD)(objCalc.RVAtoFOA(pResourceEntry->NameOffset) + pResourceFOA);
						//NameOffset所指向的结构体是IMAGE_RESOURCE_DIR_STRING_U类型
						//这里保存了字符串的长度和起始位置
						PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;
						//这里的字符串不是以0结尾的，所以需要拷贝出来加上‘\0’结尾后再打印
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

					//判断有没有第三层
					if (pResourceEntry->DataIsDirectory)
					{
						//第三层的起始位置
						QWORD dwResThrid =
							(QWORD)pResourceFOA + pResSecondEntry->OffsetToDirectory;
						PIMAGE_RESOURCE_DIRECTORY pResThrid =
							(PIMAGE_RESOURCE_DIRECTORY)dwResThrid;

						PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThridEntry =
							(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResThrid + 1);

						//第三层，使用PIMAGE_RESOURCE_DIRECTORY_ENTRY的成员
						//OffsetToData成员，得到PIMAGE_RESOURCE_DATA_ENTRY的位置
						PIMAGE_RESOURCE_DATA_ENTRY pResData =
							(PIMAGE_RESOURCE_DATA_ENTRY)(pResThridEntry->OffsetToData + (QWORD)pResourceFOA);

						//资源的rva和size
						QWORD dwResDataRVA = pResData->OffsetToData;
						QWORD dwresDataSize = pResData->Size;

						//PIMAGE_RESOURCE_DATA_ENTRY中的offsettodata是RVA 
						QWORD dwResDataFOA = (QWORD)(objCalc.RVAtoFOA(dwResDataRVA));

						CString strTemp;
						strTemp.Format(L"RVA:%08X", pResData->OffsetToData);
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

						strTemp.Format(L"偏移:%08X", objCalc.RVAtoFOA(pResData->OffsetToData));
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

						strTemp.Format(L"Size:%08X", pResData->Size);
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

					}
					//下一个资源
					pResSecondEntry++;
				}
			}
		}
		//下一种资源
		pResourceEntry++;
	}
}




//联合体中偏移成员均为相对于资源表最开始的位置（PIMAGE_RESOURCE_DIRECTORY）偏移 得到的最后一个结构体中成员offsettodate是一个RVA
void CPE_Resource::ShowResource64()
{
	CPE_FOA_Calc objCalc(m_pNt64);

	//资源表RVA
	QWORD dwResourceRVA = m_pNt64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
	PIMAGE_RESOURCE_DIRECTORY pResourceRVA = (PIMAGE_RESOURCE_DIRECTORY)(dwResourceRVA);

	//资源表在文件中位置
	QWORD dwResourceFOA = (QWORD)(objCalc.RVAtoFOA(dwResourceRVA) + pFileImageBase);
	PIMAGE_RESOURCE_DIRECTORY pResourceFOA = (PIMAGE_RESOURCE_DIRECTORY)(dwResourceFOA);


	//第一层(种类)
	//种类个数
	QWORD dwCountOfResType = pResourceFOA->NumberOfNamedEntries + pResourceFOA->NumberOfIdEntries;

	//获取目录项首地址
	PIMAGE_RESOURCE_DIRECTORY_ENTRY pResourceEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResourceFOA + 1);

	for (unsigned i = 0; i < dwCountOfResType; i++)
	{
		

		//判断资源是字符串还是ID：第一个联合体最高位为0,即NameIsString=0，则为ID起作用
		if (pResourceEntry->NameIsString)	//字符串作为资源标识
		{
			//如果是字符串，NameOffset保存的就是字符串的RVA

			//获取名字字符串的FOA
			QWORD dwNameFOA = (QWORD)(objCalc.RVAtoFOA(pResourceEntry->NameOffset) + pResourceFOA);

			//NameOffset所指向的结构体_IMAGE_RESOUCE_DIR_STRING_U,保存了字符串的长度和起始位置
			PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;

			//这里的字符串不是以0结尾，加上'\0'
			WCHAR* pResName = new WCHAR[pName->Length + 1]{};
			memcpy(pResName, pName, (pName->Length) * sizeof(WCHAR));

			// 根节点 如果成功返回新项目的句柄 否则 NULL
			hParent = m_Tree_Resource.InsertItem(
				pResName,	/*包含项目文本的字符串的地址*/
				NULL,			/*项目的图像的索引在树视图控件图形的列表*/
				NULL,			/*项目中的选定图像的索引在树视图控件图形的列表*/
				NULL,			/*插入项的父级的句柄*/
				TVI_LAST);		/*要插入该项目，然后后新项目的句柄*/

			delete[] pResName;
		}
		else  //ID 
		{
			char* arryResType[] = {
				"",
				"鼠标指针（Cursor)",			//1
				"位图（Bitmap)",				//2
				"图标（Icon）",					//3
				"菜单（Menu）",					//4
				"对话框（Dialog）",				//5
				"字符串列表（String Table）",	//6
				"字体目录（Font Directory）",	//7
				"字体（Font）",					//8
				"快捷键（Accelerators）",		//9
				"非格式化资源（Unformatted）",	//A
				"消息列表（Message Table）",	//B
				"鼠标指针组（Croup Cursor）",	//C
				"",
				"图标组（Group Icon）",			//E
				"",
				"版本信息（Version Information）" ,//10
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

			//判断是否有第二层
			if (pResourceEntry->DataIsDirectory)
			{
				QWORD dwResSecond = (QWORD)pResourceFOA + pResourceEntry->OffsetToDirectory;
				PIMAGE_RESOURCE_DIRECTORY pResSecond = (PIMAGE_RESOURCE_DIRECTORY)dwResSecond;


				//第二层个数
				QWORD dwCountOfSecond =pResSecond->NumberOfIdEntries + pResSecond->NumberOfNamedEntries;

				//第二个结构体
				PIMAGE_RESOURCE_DIRECTORY_ENTRY pResSecondEntry =(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResSecond + 1);
				//遍历每一个资源
				for (unsigned iSecond = 0; iSecond < dwCountOfSecond; iSecond++)
				{
					//判断这种资源是字符串还是ID
					if (pResourceEntry->NameIsString)
					{
						//如果是字符串，NameOffset保存的就是这个字符串的RVA
						//得到名字字符串的FOA
						QWORD dwNameFOA = (QWORD)(objCalc.RVAtoFOA(pResourceEntry->NameOffset) + pResourceFOA);
						//NameOffset所指向的结构体是IMAGE_RESOURCE_DIR_STRING_U类型
						//这里保存了字符串的长度和起始位置
						PIMAGE_RESOURCE_DIR_STRING_U pName = (PIMAGE_RESOURCE_DIR_STRING_U)dwNameFOA;
						//这里的字符串不是以0结尾的，所以需要拷贝出来加上‘\0’结尾后再打印
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

					//判断有没有第三层
					if (pResourceEntry->DataIsDirectory)
					{
						//第三层的起始位置
						QWORD dwResThrid =
							(QWORD)pResourceFOA + pResSecondEntry->OffsetToDirectory;
						PIMAGE_RESOURCE_DIRECTORY pResThrid =
							(PIMAGE_RESOURCE_DIRECTORY)dwResThrid;

						PIMAGE_RESOURCE_DIRECTORY_ENTRY pResThridEntry =
							(PIMAGE_RESOURCE_DIRECTORY_ENTRY)(pResThrid + 1);

						//第三层，使用PIMAGE_RESOURCE_DIRECTORY_ENTRY的成员
						//OffsetToData成员，得到PIMAGE_RESOURCE_DATA_ENTRY的位置
						PIMAGE_RESOURCE_DATA_ENTRY pResData =
							(PIMAGE_RESOURCE_DATA_ENTRY)(pResThridEntry->OffsetToData + (QWORD)pResourceFOA);

						//资源的rva和size
						QWORD dwResDataRVA = pResData->OffsetToData;
						QWORD dwresDataSize = pResData->Size;

						//PIMAGE_RESOURCE_DATA_ENTRY中的offsettodata是RVA 
						QWORD dwResDataFOA = (QWORD)(objCalc.RVAtoFOA(dwResDataRVA));

						CString strTemp;
						strTemp.Format(L"RVA:%08X", pResData->OffsetToData);
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

						strTemp.Format(L"偏移:%08X", objCalc.RVAtoFOA(pResData->OffsetToData));
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

						strTemp.Format(L"Size:%08X", pResData->Size);
						m_Tree_Resource.InsertItem(strTemp, 0, 0, hInsertAfter, TVI_LAST);

					}
					//下一个资源
					pResSecondEntry++;
				}
			}
		}
 		//下一种资源
		pResourceEntry++;
	}
}


	


