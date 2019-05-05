// PE_Import.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_Import.h"
#include "afxdialogex.h"


// CPE_Import 对话框

IMPLEMENT_DYNAMIC(CPE_Import, CDialogEx)

CPE_Import::CPE_Import(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMPORT, pParent)
{

}

CPE_Import::CPE_Import(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /* = NULL */)
	: CDialogEx(IDD_IMPORT, pParent)
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt32 = pNt32;

	m_pDesc = m_pNt32->OptionalHeader.DataDirectory;
}

CPE_Import::CPE_Import(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /* = NULL */)
	: CDialogEx(IDD_IMPORT, pParent)
{
	m_pDos = pDos;
	pFileImageBase = (byte*)m_pDos;
	m_pNt64 = pNt64;

	m_pDesc = m_pNt64->OptionalHeader.DataDirectory;
}

CPE_Import::~CPE_Import()
{
}

void CPE_Import::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_API_IMPROT, m_list_API);
	DDX_Control(pDX, IDC_LIST_DLL_IMPORT, m_list_DLL);
}


BEGIN_MESSAGE_MAP(CPE_Import, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_DLL_IMPORT, &CPE_Import::OnNMClickListDllImport)
END_MESSAGE_MAP()


// CPE_Import 消息处理程序


BOOL CPE_Import::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list_DLL.AddColumn(6, L"DLL 名称", 100, L"原始 First Thunk", 130, L"时间/日期标志", 130, L"Forwarder Chain", 130, L"NameRVA", 100, L"First Thunk", 100);
	m_list_API.AddColumn(5, L"API 名称", 150, L"Thunk RVA", 150, L"Thunk 偏移", 150, L"Thunk 值", 150, L"Hint", 100);

	if (m_pNt32)
	{
		GetDLLInfo32();
	}
	else
	{
		GetDLLInfo64();
	}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPE_Import::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

//获取DLL信息
void CPE_Import::GetDLLInfo32()
{
	//找到导入表
	DWORD dwImportRVA = m_pDesc[1].VirtualAddress;

	
	//找到文件中的位置
	CPE_FOA_Calc objCalc(m_pNt32);

	QWORD dwImportInFile = (QWORD)(objCalc.RVAtoFOA(dwImportRVA) + pFileImageBase);
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)dwImportInFile;


	//遍历每一个导入表
	while (pImport->Name)
	{
		//函数名称地址
		PIMAGE_THUNK_DATA pFirstThunk = (PIMAGE_THUNK_DATA)(objCalc.RVAtoFOA(pImport->FirstThunk) + pFileImageBase);

		//模块名
		m_DLL_Name = (char*)(objCalc.RVAtoFOA(pImport->Name) + pFileImageBase);
		//INT输入名称表
		m_DLL_OriginalFirstThunk.Format(L"%08X", pImport->OriginalFirstThunk);
		//IAT输入地址表
		m_DLL_FirstThunk.Format(L"%08X", pImport->FirstThunk);
		
		//时间戳
		m_DLL_TimeDateStamp.Format(L"%08X", pImport->TimeDateStamp);

		//转发
		m_DLL_ForwarderChain.Format(L"%08X", pImport->ForwarderChain);

		//导入的PE文件的名字的相对虚拟地址RVA
		m_DLL_NameRVA.Format(L"%08X",pImport->Name);

		//m_list_DLL.AddColumn(6, L"DLL 名称", 100, L"原始 First Thunk", 150, L"时间/日期标志", 100, L"Forwarder Chain", 150, L"名称", 100, L"First Thunk", 100);
		m_list_DLL.AddItem(0, 6, m_DLL_Name, m_DLL_OriginalFirstThunk, m_DLL_TimeDateStamp, m_DLL_ForwarderChain, m_DLL_NameRVA, m_DLL_FirstThunk);

		pImport++;
	}
}

//获取DLL信息
void CPE_Import::GetDLLInfo64()
{
	//找到导入表
	DWORD dwImportRVA = m_pDesc[1].VirtualAddress;


	//找到文件中的位置
	CPE_FOA_Calc objCalc(m_pNt64);
	DWORD temp = (DWORD)objCalc.RVAtoFOA(dwImportRVA);
	QWORD dwImportInFile = (QWORD)(objCalc.RVAtoFOA(dwImportRVA) + pFileImageBase);
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)dwImportInFile;


	//遍历每一个导入表
	while (pImport->Name)
	{
		//函数名称地址
		PIMAGE_THUNK_DATA pFirstThunk = (PIMAGE_THUNK_DATA)(objCalc.RVAtoFOA(pImport->FirstThunk) + pFileImageBase);

		//模块名
		m_DLL_Name = (char*)(objCalc.RVAtoFOA(pImport->Name) + pFileImageBase);
		//INT输入名称表
		m_DLL_OriginalFirstThunk.Format(L"%08X", pImport->OriginalFirstThunk);
		//IAT输入地址表
		m_DLL_FirstThunk.Format(L"%08X", pImport->FirstThunk);

		//时间戳
		m_DLL_TimeDateStamp.Format(L"%08X", pImport->TimeDateStamp);

		//转发
		m_DLL_ForwarderChain.Format(L"%08X", pImport->ForwarderChain);

		//导入的PE文件的名字的相对虚拟地址RVA
		m_DLL_NameRVA.Format(L"%08X", pImport->Name);

		//m_list_DLL.AddColumn(6, L"DLL 名称", 100, L"原始 First Thunk", 150, L"时间/日期标志", 100, L"Forwarder Chain", 150, L"名称", 100, L"First Thunk", 100);
		m_list_DLL.AddItem(0, 6, m_DLL_Name, m_DLL_OriginalFirstThunk, m_DLL_TimeDateStamp, m_DLL_ForwarderChain, m_DLL_NameRVA, m_DLL_FirstThunk);

		pImport++;
	}
}




//获取选中的DLL中API信息
void CPE_Import::GetAPIInof32(PIMAGE_THUNK_DATA32 pFirstThunk)
{
	m_list_API.DeleteAllItems();
	CPE_FOA_Calc objCalc(m_pNt32);


	DWORD rva = _tcstol(m_API_ThunkRVA, NULL, 16);


	for (;pFirstThunk->u1.AddressOfData; rva += 4)
	{
		//判断导入方式
		if (IMAGE_SNAP_BY_ORDINAL32(pFirstThunk->u1.AddressOfData))//判断最高位是否为1,参数就是这个结构体。
		{
			//=1 说明是序号导入（低16位是其序号）
			m_API_Name = L"--";
			m_API_Hint.Format(L"%08X", pFirstThunk->u1.Ordinal);

			
		}
		else   //名称导入
		{
			PIMAGE_IMPORT_BY_NAME pImportName =(PIMAGE_IMPORT_BY_NAME)(objCalc.RVAtoFOA(pFirstThunk->u1.AddressOfData) + pFileImageBase);

			m_API_Hint.Format(L"%04X", pImportName->Hint);
			m_API_Name = (CString)pImportName->Name;
	
		}

		m_API_ThunkRVA.Format(L"%08X", rva);
		m_API_ThunkFOA.Format(L"%08X", objCalc.RVAtoFOA(rva));
		m_API_ThunkValue.Format(L"%08X", pFirstThunk->u1);


		pFirstThunk++;

		//m_list_API.AddColumn(5, L"API 名称", 150, L"Thunk RVA", 150, L"Thunk 偏移", 150, L"Thunk 值", 150, L"Hint", 100);
		m_list_API.AddItem(0, 5, m_API_Name, m_API_ThunkRVA, m_API_ThunkFOA, m_API_ThunkValue, m_API_Hint);
	}

	
}


//获取选中的DLL中API信息
void CPE_Import::GetAPIInof64(PIMAGE_THUNK_DATA64 pFirstThunk)
{
	m_list_API.DeleteAllItems();
	CPE_FOA_Calc objCalc(m_pNt64);


	DWORD rva = _tcstol(m_API_ThunkRVA, NULL, 16);


	while (pFirstThunk->u1.AddressOfData )
	{
		//判断导入方式
		if (IMAGE_SNAP_BY_ORDINAL64(pFirstThunk->u1.AddressOfData))//判断最高位是否为1,参数就是这个结构体。
		{
			//=1 说明是序号导入（低16位是其序号）
			m_API_Name = L"--";
			m_API_Hint.Format(L"%08X", pFirstThunk->u1.Ordinal);


		}
		else   //名称导入
		{
			PIMAGE_IMPORT_BY_NAME pImportName = (PIMAGE_IMPORT_BY_NAME)(objCalc.RVAtoFOA(pFirstThunk->u1.AddressOfData) + pFileImageBase);

			m_API_Hint.Format(L"%04X", pImportName->Hint);
			m_API_Name = (CString)pImportName->Name;

		}

		m_API_ThunkRVA.Format(L"%08X", rva);
		m_API_ThunkFOA.Format(L"%08X", objCalc.RVAtoFOA(rva));
		m_API_ThunkValue.Format(L"%08X", pFirstThunk->u1);


		pFirstThunk++;
		rva += 4;
		//pFirstThunk = (PIMAGE_THUNK_DATA32)(objCalc.RVAtoFOA(rva) + pFileImageBase);

		//m_list_API.AddColumn(5, L"API 名称", 150, L"Thunk RVA", 150, L"Thunk 偏移", 150, L"Thunk 值", 150, L"Hint", 100);
		m_list_API.AddItem(0, 5, m_API_Name, m_API_ThunkRVA, m_API_ThunkFOA, m_API_ThunkValue, m_API_Hint);
	}


}



//DLL列表点击事件 刷新API列表
void CPE_Import::OnNMClickListDllImport(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if (m_pNt32)
	{
		CPE_FOA_Calc objCalc(m_pNt32);

		DWORD temp_FirstThunk;

		if (pNMItemActivate->iItem != -1)//如果GetItemText项目索引未用于通知，则此成员将包含-1。
		{


			//m_list_API.AddColumn(5, L"API 名称", 150, L"Thunk RVA", 150, L"Thunk 偏移", 150, L"Thunk 值", 150, L"Hint", 100);

			m_API_ThunkRVA = m_list_DLL.GetItemText(pNMItemActivate->iItem, 1);


			//获取第6列 初始fistThunk
			CString bbb = m_list_DLL.GetItemText(pNMItemActivate->iItem, 5);
			temp_FirstThunk = _tcstol(bbb, NULL, 16);
			PIMAGE_THUNK_DATA32 pFirstThunk32 = (PIMAGE_THUNK_DATA32)(objCalc.RVAtoFOA(temp_FirstThunk) + pFileImageBase);



			//获取函数信息
			GetAPIInof32(pFirstThunk32);
		}
	}
	else
	{
		CPE_FOA_Calc objCalc(m_pNt64);

		DWORD temp_FirstThunk;

		if (pNMItemActivate->iItem != -1)//如果GetItemText项目索引未用于通知，则此成员将包含-1。
		{


			//m_list_API.AddColumn(5, L"API 名称", 150, L"Thunk RVA", 150, L"Thunk 偏移", 150, L"Thunk 值", 150, L"Hint", 100);

			m_API_ThunkRVA = m_list_DLL.GetItemText(pNMItemActivate->iItem, 1);


			//获取第6列 初始fistThunk
			CString bbb = m_list_DLL.GetItemText(pNMItemActivate->iItem, 5);
			temp_FirstThunk = _tcstol(bbb, NULL, 16);
			PIMAGE_THUNK_DATA64 pFirstThunk = (PIMAGE_THUNK_DATA64)(objCalc.RVAtoFOA(temp_FirstThunk) + pFileImageBase);



			//获取函数信息
			GetAPIInof64(pFirstThunk);
		}
	}
	

	

	*pResult = 0;
}
