// Dia_Thread.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_Thread.h"
#include "afxdialogex.h"
#include <TlHelp32.h>


// Dia_Thread �Ի���

IMPLEMENT_DYNAMIC(Dia_Thread, CDialogEx)

Dia_Thread::Dia_Thread(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_THREAD, pParent)
{

}

Dia_Thread::~Dia_Thread()
{
}

void Dia_Thread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Thread, m_List_Thread);
}


BEGIN_MESSAGE_MAP(Dia_Thread, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_Thread, &Dia_Thread::OnRclickListThread)
END_MESSAGE_MAP()


// Dia_Thread ��Ϣ�������


BOOL Dia_Thread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	UpdataList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void Dia_Thread::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DestroyWindow();
	CDialogEx::OnClose();
}

//�����б�
void Dia_Thread::UpdataList()
{
	m_List_Thread.DeleteAllItems();
	m_List_Thread.SetExtendedStyle(m_List_Thread.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	m_List_Thread.InsertColumn(0, _T("�߳�ID"), 0, 200);
	m_List_Thread.InsertColumn(1, _T("���ȼ�"), 0, 100);
	m_List_Thread.InsertColumn(2, _T("ӵ����(�߳�)ID"), 0, 200);
	m_List_Thread.InsertColumn(3, _T("ռ���ڴ�"), 0, 100);
}


BOOL Dia_Thread::ListProcessThreads(DWORD dwOwnerPID)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return(FALSE);

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(THREADENTRY32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		win32pError();  // Show cause of failure
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return(FALSE);
	}

	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	do
	{
		int i = 0;
		TCHAR *Temp = new TCHAR[MAX_PATH];

		if (te32.th32OwnerProcessID == dwOwnerPID)
		{

			//�߳�ID
			wsprintf(Temp, L"%d", te32.th32ThreadID);
			m_List_Thread.InsertItem(i, Temp);

			//�߳����ȼ�
			wsprintf(Temp, L"%d", te32.tpBasePri);
			m_List_Thread.SetItemText(i, 1, Temp);

			//ӵ�����߳�ID
			wsprintf(Temp, L"%d", te32.th32OwnerProcessID);
			m_List_Thread.SetItemText(i, 2, Temp);

			//ռ���ڴ�
			// 			wsprintf(Temp, L"%d", te32.dwSize);
			// 			m_List_Thread.SetItemText(i, 3, Temp);

			i++;
		}

	} while (Thread32Next(hThreadSnap, &te32));

	CloseHandle(hThreadSnap);
	return(TRUE);
}


void Dia_Thread::OnRclickListThread(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU_ThreadRCList);
	pmenu = menu.GetSubMenu(0);//ָ�������˵��еĵ����˵���λ�á� λ��ֵ��0��ʼ��һ���˵�� 
	ClientToScreen(&pNMItemActivate->ptAction);//�ڿͻ���������ָʾ�����¼���λ��
	UINT Select = pmenu->TrackPopupMenu(
		TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);

	//��ȡ����ID
	DWORD dwPID = 0;
	WCHAR wcProcsessID[MAX_PATH] = {};
	if (pNMItemActivate->iItem != -1)//���GetItemText��Ŀ����δ����֪ͨ����˳�Ա������-1��
	{
		m_List_Thread.GetItemText(pNMItemActivate->iItem, 2, wcProcsessID, MAX_PATH);//GetItemText:1.�������������У��� 2.�������������ǵڶ��У��� 3�� �����ı���ָ�룬 4�� ���������ȣ����ؼ����ַ����ĳ���(int)
	}
	dwPID = _wtoi(wcProcsessID);
	//��ȡ���̾��
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	//��ȡThreadID
	DWORD dwTID = 0;
	WCHAR wcThreadID[MAX_PATH] = {};
	if (pNMItemActivate->iItem != -1)//���GetItemText��Ŀ����δ����֪ͨ����˳�Ա������-1��
	{
		m_List_Thread.GetItemText(pNMItemActivate->iItem, 0, wcThreadID, MAX_PATH);//GetItemText:1.������ 2.�������� 3�� �����ı���ָ�룬 4�� ���������ȣ����ؼ����ַ����ĳ���(int)
	}
	dwTID = _wtoi(wcThreadID);
	//��ȡ�߳̾��
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwTID);

	//	//��ȡThread���
	//	HANDLE  hThread;
	//	DuplicateHandle(GetCurrentProcess(), //��ǰ���̾��
	//		GetCurrentThread(),				 //��ǰ�߳̾�����õ�����α�����
	//		GetCurrentProcess(),			 //Ŀ����̣��������Լ���
	//		&hThread,						//Ҫ�浽��Ŀ���߳̾����ַ
	//		0,								//���̵Ĳ���Ȩ�ޣ����Ҫ�õ���Ҫ��PROCESS_ALL_ACCESS��PROCESS_DUP_HANDLE
	//		FALSE,							//�Ƿ�̳�
	//		DUPLICATE_SAME_ACCESS);			//DUPLICATE_SAME_ACCESS �¾��ӵ����ԭʼ�����ͬ�İ�ȫ��������;DUPLICATE_CLOSE_SOURCE ԭʼ����Ѿ��رա���ʹ����������ҲҪ�ر�

	switch (Select)
	{
	case ID_MOUSERCLICK_32771:	//�����߳�
	{
		SuspendThread(hThread);
		//CloseHandle(hThread); 
		UpdataList();
		ListProcessThreads(dwPID);
	}
	break;
	case ID_MOUSERCLICK_32773:	//�ָ��߳�
	{
		ResumeThread(hThread);
		UpdataList();
		ListProcessThreads(dwPID);
	}
	break;
	case ID_MOUSERCLICK_32772:	//�����߳�
	{
		TerminateThread(hThread, 0);
		//CloseHandle(hThread);
		UpdataList();
		ListProcessThreads(dwPID);
	}
	break;
	default:
		break;
	}
}
