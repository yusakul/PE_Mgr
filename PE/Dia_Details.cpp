// Dia_Details.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Details.h"
#include "afxdialogex.h"
#include <string>



// CDia_Details �Ի���

IMPLEMENT_DYNAMIC(CDia_Details, CDialogEx)

CDia_Details::CDia_Details(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DETAILS, pParent)
{

}

CDia_Details::~CDia_Details()
{
}

void CDia_Details::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Details, m_ListCtrl_De);

}


BEGIN_MESSAGE_MAP(CDia_Details, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_WinProc, &CDia_Details::OnLvnItemchangedListWinproc)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Details, &CDia_Details::OnDblclkListDetails)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_Details, &CDia_Details::OnRclickListDetails)
END_MESSAGE_MAP()


// CDia_Details ��Ϣ�������


void CDia_Details::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


//�����б�
void CDia_Details::UpdataList()
{
	m_ListCtrl_De.DeleteAllItems();

	typedef long(_stdcall*FUN)
		(
			UINT SystemInformationClass,
			PVOID SystemInformation,
			ULONG SystemInformationLength,
			PULONG ReturnLength
			);
	FUN fun;
	HMODULE  hNtDll = LoadLibrary(L"Ntdll.dll");
	if (!hNtDll)
	{
		win32pError();
		return;
	}
	fun = (FUN)GetProcAddress(hNtDll, "NtQuerySystemInformation");
	ULONG dwNeedSize = 0;
	int status = fun(5, NULL, 0, &dwNeedSize);
	BYTE* buf = new BYTE[dwNeedSize];

	PSYSTEM_PROCESS_INFORMATION pSpi = (PSYSTEM_PROCESS_INFORMATION)buf;
	status = fun(5, (PVOID)buf, dwNeedSize, NULL);

	for (int i = 0; pSpi->NextEntryDelta != 0; i++)
	{
		if (pSpi->ProcessName.Buffer == NULL)
		{
			i--;
			pSpi = (PSYSTEM_PROCESS_INFORMATION)
				(((PUCHAR)pSpi) + pSpi->NextEntryDelta);
			continue;
		}

		HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pSpi->ProcessId);

		BOOL Wow64Process = FALSE;
		IsWow64Process(handle, &Wow64Process);//Wow64Process [out] ���������WOW64�����У���ָ������ΪTRUE��ֵ��ָ�롣���������32λWindows�����У����ֵ����ΪFALSE������ý�������64λWindows�����е�64λӦ�ó������ֵҲ����ΪFALSE��

		TCHAR temp[MAX_PATH] = {};
		//��ʾ������
		wcscpy_s(temp, pSpi->ProcessName.Length, pSpi->ProcessName.Buffer);
		//��ʾ32λ
		if (Wow64Process == TRUE)
		{
			wcscat_s(temp, MAX_PATH, L"��32λ��");
		}
		m_ListCtrl_De.InsertItem(i, temp);

		//��ʾ����PID
		_itow_s(pSpi->ProcessId, temp, 10);//value�����ƣ������ڷ�Χ 2-36��
		m_ListCtrl_De.SetItemText(i, 1, temp);

		//�߳���
		_itow_s(pSpi->ThreadCount, temp, 10);
		m_ListCtrl_De.SetItemText(i, 2, temp);

		//����ռ���ڴ�
		_itow_s(pSpi->VmCounters.WorkingSetSize / 1024, temp, 10);
		wcscat_s(temp, MAX_PATH, L"K");
		m_ListCtrl_De.SetItemText(i, 3, temp);

		pSpi = (PSYSTEM_PROCESS_INFORMATION)
			(((PUCHAR)pSpi) + pSpi->NextEntryDelta);
	}
	delete buf;
}

//��ʼ��
BOOL CDia_Details::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//���ö�ʱʱ��
	SetTimer(IDC_LIST_Details, 20000, NULL);

	m_ListCtrl_De.SetExtendedStyle(m_ListCtrl_De.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_DOUBLEBUFFER);
	m_ListCtrl_De.InsertColumn(0, _T("����"), 0, 200);
	m_ListCtrl_De.InsertColumn(1, _T("PID"), 0, 200);
	m_ListCtrl_De.InsertColumn(2, _T("���߳���"), 0, 200);
	m_ListCtrl_De.InsertColumn(3, _T("ռ���ڴ�"), 0, 200);
	UpdataList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//��ʱ
void CDia_Details::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	UpdataList();
	CDialogEx::OnTimer(nIDEvent);
}


void CDia_Details::OnLvnItemchangedListWinproc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


//�б�˫����ģ���б���
void CDia_Details::OnDblclkListDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	std::wstring strPID;

	if (-1 != pNMItemActivate->iItem)
	{
		strPID = m_ListCtrl_De.GetItemText(pNMItemActivate->iItem, 1);
	}
	DWORD dwPID = _wtoi(strPID.c_str());
	if (m_mod.Create(IDD_DIALOG_MODULE, this))
	{
		m_mod.ShowWindow(SW_SHOW);
		m_mod.ListProcessModules(dwPID);
	}

}

//�б��Ҽ���ȡ�˵�
void CDia_Details::OnRclickListDetails(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU_RClickList);
	pmenu = menu.GetSubMenu(0);//ָ�������˵��еĵ����˵���λ�á� λ��ֵ��0��ʼ��һ���˵�� 
	ClientToScreen(&pNMItemActivate->ptAction);//�ڿͻ���������ָʾ�����¼���λ��
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);

	//��ȡPID
	int nRanNum = rand() / 2;
	DWORD dwPID = 0;
	if (nRanNum == 0)
	{
		WCHAR wcProcsessID[MAX_PATH] = {};
		if (pNMItemActivate->iItem != -1)//���GetItemText��Ŀ����δ����֪ͨ����˳�Ա������-1��
		{
			m_ListCtrl_De.GetItemText(pNMItemActivate->iItem, 1, wcProcsessID, MAX_PATH);//GetItemText:1.������ 2.�������� 3�� �����ı���ָ�룬 4�� ���������ȣ����ؼ����ַ����ĳ���(int)
		}
		dwPID = _wtoi(wcProcsessID);
	}
	else
	{
		std::wstring strPID;
		if (pNMItemActivate->iItem != -1)
		{
			strPID = m_ListCtrl_De.GetItemText(pNMItemActivate->iItem, 1);//GetItemText:1.������ 2.�������󡣷���һ��CString���ı�
		}
		dwPID = _wtoi(strPID.c_str());
	}
	//�˵����֧
	switch (Select)
	{
	case ID_32778:	//�����߳�
	{
		m_Thread.Create(IDD_DIALOG_THREAD, this);
		m_Thread.ShowWindow(SW_SHOW);
		m_Thread.ListProcessThreads(dwPID);
	}
	break;
	case ID_32779:	//����ģ��
	{
		m_mod.Create(IDD_DIALOG_MODULE, this);
		m_mod.ShowWindow(SW_SHOW);
		m_mod.ListProcessModules(dwPID);
	}
	break;
	case ID_32780:	//������
	{
		m_Heap.Create(IDD_DIALOG_HEAP, this);
		m_Heap.ShowWindow(SW_SHOW);
		m_Heap.ListProcessHeap(dwPID);
	}
	break;
	case ID_IDR_32775:	//��������
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
		TerminateProcess(hProcess, 0);//��������
	}
	break;
	case ID_IDR_32776:	//���ļ�����λ��
	{

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
		WCHAR wcFileName[MAX_PATH] = {};
		GetModuleFileNameEx(hProcess, NULL, wcFileName, MAX_PATH);

		PVOID pTemp = wcsrchr(wcFileName, '\\');//��λ��·��XXX\\XXX\\xxx.exe��exeǰ\\�ĵ�ַ
		memset(pTemp, 0, 1);//��pTemp��ָ���ĳһ���ڴ��еĺ�2���ֽڵ�����ȫ������Ϊ0��ASCIIֵ,������ɾ��exeǰ��\\,ֻ��Ҫ��ȡ���ļ���·��Ϊֹ
		ShellExecute(NULL, L"explore", wcFileName, NULL, NULL, SW_SHOW);
		CloseHandle(hProcess);
	}
	break;
	case ID_IDR_32777:	//ˢ��
	{
		UpdataList();
	}
	break;
	default:
		break;
	}
	*pResult = 0;
}
