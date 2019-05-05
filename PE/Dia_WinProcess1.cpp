// Dia_WinProcess1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "Dia_WinProcess1.h"
#include "afxdialogex.h"



// CDia_WinProcess1 �Ի���

IMPLEMENT_DYNAMIC(CDia_WinProcess1, CDialogEx)

CDia_WinProcess1::CDia_WinProcess1(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_WINPROCESS, pParent)
{

}

CDia_WinProcess1::~CDia_WinProcess1()
{
}

void CDia_WinProcess1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WinProc, m_ListCtrl_winproc);
}


BEGIN_MESSAGE_MAP(CDia_WinProcess1, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_WinProc, &CDia_WinProcess1::OnNMRClickListWinproc)
END_MESSAGE_MAP()


// CDia_WinProcess1 ��Ϣ�������


void CDia_WinProcess1::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


BOOL CDia_WinProcess1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetTimer(IDC_LIST_WinProc, 30000, NULL);
	pImageList->Create(16, 16, ILC_COLOR32, 0, 0);

	m_ListCtrl_winproc.DeleteAllItems();
	m_ListCtrl_winproc.SetExtendedStyle(m_ListCtrl_winproc.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);
	m_ListCtrl_winproc.InsertColumn(0, _T("Ӧ�ó���"), 0, 400);
	m_ListCtrl_winproc.InsertColumn(1, _T("���"), 0, 200);
	m_ListCtrl_winproc.InsertColumn(2, _T("PID"), 0, 200);

	UpdataList();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDia_WinProcess1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnTimer(nIDEvent);
}


//�������ڽ���
BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	LRESULT WndClassex;//WNDCLASSEX����ָ���ṹ��С��cbSize��Ա�Ͱ����봰���������Сͼ��ľ����hIconSm��Ա��
	CDia_WinProcess1* m_winproc = (CDia_WinProcess1*)lParam;
	MyWinProcStruct Temp_list;
	DWORD temp_PID;
	if (!GetWindow(hWnd, GW_OWNER) && IsWindowVisible(hWnd))//���ָ�����ڣ��丸���ڣ��丸���ڵȾ���WS_VISIBLE��ʽ���򷵻�ֵΪ����ֵ�����򣬷���ֵΪ�㡣���ڷ���ֵָ�������Ƿ����WS_VISIBLE��ʽ����˼�ʹ������ȫ�����������ڵ����ô���Ҳ���ܲ�Ϊ�㡣
	{
		GetWindowText(hWnd, Temp_list.lpWinTitle, MAX_PATH - 1);//��������ɹ����򷵻�ֵ�Ǹ����ַ����ĳ��ȣ����ַ�Ϊ��λ������������ֹ���ַ����������û�б��������ı������������Ϊ�գ����ߴ��ڻ�ؼ������Ч���򷵻�ֵΪ�㡣
		if (wcscmp(Temp_list.lpWinTitle, L"") == 0)//�����������Ϊ��
		{
			return true;
		}
		else
		{
			GetWindowThreadProcessId(hWnd, &temp_PID);
			WndClassex = GetClassLong(hWnd, NULL); //GCL_HICONSM��������������Сͼ��ľ����
												   //WndClassex = GetClassLong(hWnd,); //
			if (WndClassex == 0)
			{
				WndClassex = SendMessage(hWnd, WM_GETICON, 0, 0);
			}
			Temp_list.hIcon = (HICON)WndClassex;//��ȡͼ��
			Temp_list.hPID = temp_PID;//��ȡPID
			Temp_list.hWnd = hWnd;//��ȡ���
			m_winproc->m_WinProcVec.push_back(Temp_list);
		}

	}
	return true;
}


void CDia_WinProcess1::UpdataList()
{
	m_ListCtrl_winproc.DeleteAllItems();

	m_ListCtrl_winproc.SetImageList(pImageList, LVSIL_SMALL);//����Сͼ��
	TCHAR* temp_buff = new TCHAR[MAX_PATH];

	m_WinProcVec.clear();
	EnumWindows(EnumWindowsProc, (LPARAM)this);
	for (UINT i = 0; i < m_WinProcVec.size(); i++)
	{
		//���ͼ��
		m_ListCtrl_winproc.InsertItem(i, m_WinProcVec[i].lpWinTitle, i);
		pImageList->Add(m_WinProcVec[i].hIcon);
		//���pid
		wsprintf(temp_buff, L"%d", m_WinProcVec[i].hPID);
		m_ListCtrl_winproc.SetItemText(i, 2, temp_buff);
		//��Ӿ��
		wsprintf(temp_buff, L"%d", m_WinProcVec[i].hWnd);
		m_ListCtrl_winproc.SetItemText(i, 1, temp_buff);
		m_WinProcVec[i].ListID = i;
	}
	delete temp_buff;
}


//�Ҽ�list�б����Menu��������
void CDia_WinProcess1::OnNMRClickListWinproc(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMenu menu, *pmenu;
	//MessageBox(0, 0, 0);
	menu.LoadMenuW(IDR_MENU1);
	pmenu = menu.GetSubMenu(0);//ָ�������˵��еĵ����˵���λ�á� λ��ֵ��0��ʼ��һ���˵�� 
	ClientToScreen(&pNMItemActivate->ptAction);//�ڿͻ���������ָʾ�����¼���λ��
	UINT Select = pmenu->TrackPopupMenu(TPM_RETURNCMD,
		pNMItemActivate->ptAction.x,
		pNMItemActivate->ptAction.y,
		this,
		NULL);

	//��ȡPID

	DWORD dwPID = 0;

	WCHAR wcProcsessID[MAX_PATH] = {};
	if (pNMItemActivate->iItem != -1)//���GetItemText��Ŀ����δ����֪ͨ����˳�Ա������-1��
	{
		m_ListCtrl_winproc.GetItemText(pNMItemActivate->iItem, 2, wcProcsessID, MAX_PATH);//GetItemText:1.������ 2.�������� 3�� �����ı���ָ�룬 4�� ���������ȣ����ؼ����ַ����ĳ���(int)
	}
	dwPID = _wtoi(wcProcsessID);

	//�˵����֧
	switch (Select)
	{
	case ID_KILLPROCESS_32781:	//��������
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);
		TerminateProcess(hProcess, 0);//��������
	}
	break;
	case ID_KILLPROCESS_32782:	//ˢ��
	{
		UpdataList();
	}
	break;
	default:
		break;
	}


	*pResult = 0;
}
