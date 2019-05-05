// FileMgr.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PE.h"
#include "FileMgr.h"
#include "afxdialogex.h"
#include "MD5.h"

// CFileMgr �Ի���

IMPLEMENT_DYNAMIC(CFileMgr, CDialogEx)

CFileMgr::CFileMgr(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FILEMGR, pParent)
{

}

CFileMgr::~CFileMgr()
{
}

void CFileMgr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILEMGR, m_list_filemgr);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_shell_tree);
	DDX_Control(pDX, IDC_MFCSHELLLIST1, m_shell_list);
	//DDX_Control(pDX, IDC_EDIT_filename, m_FileNameEdit);
	//DDX_Control(pDX, IDC_EDIT_filetype, m_FileTypeEdit);
}


BEGIN_MESSAGE_MAP(CFileMgr, CDialogEx)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_CLICK, IDC_MFCSHELLLIST1, &CFileMgr::OnNMClickMfcshelllist1)
END_MESSAGE_MAP()


// CFileMgr ��Ϣ�������


void CFileMgr::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}




BOOL CFileMgr::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_shell_list.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);//�ؼ�����ΪReport

	m_list_filemgr.AddColumn(2, L"����", 250, L"MD5", 250);


	//��Tree��List
	m_shell_tree.SetRelatedList(&m_shell_list);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CFileMgr::OnNMClickMfcshelllist1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	bool flag = false;
	CString temp;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1)
	{
		//�ļ���
		m_sCurrName = m_shell_list.GetItemText(pNMListView->iItem, pNMListView->iSubItem);
		//�ļ�·��
		flag = m_shell_list.GetItemPath(temp, pNMListView->iItem);
		m_sFilePath = temp;

	}

	if (!flag)
	{
		return;
	}
	

//��ȡMD5
	char* szMD5 = md5FileValue(m_sFilePath.GetBuffer());
	m_FileMd5 = CA2W(szMD5, CP_THREAD_ACP);m_list_filemgr.DeleteAllItems();m_list_filemgr.AddItem(0, 2, m_sCurrName, m_FileMd5);



}
