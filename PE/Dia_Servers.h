#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CDia_Servers �Ի���

class CDia_Servers : public CDialogEx
{
	DECLARE_DYNAMIC(CDia_Servers)

public:
	CDia_Servers(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDia_Servers();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SERVERS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CMyListCtrl m_list_servers;

	CString m_ServersName;	//��������
	CString m_ServersDesc;	//����
	CString m_ServersStatus;	//״̬
	CString m_ServersType;		//����
	CString m_ServersPath;		//·��

	void UpdataServers();	//ˢ�·���
	void GetServers();		//��ȡ����
	void ServersKey(bool Keyflag);		//���񿪹�

	const WCHAR* ServerStatus[8] =		//����״̬
	{
		L" ",
		L"��ֹͣ",
		L"����ֹͣ",
		L"��������",
		L"��������",
		L"������ͣ",
		L"����ͣ"
	};

	const WCHAR* ServerStartType[6] =	//������������
	{
		L" ",
		L" ",
		L"�Զ�",
		L"�ֶ�",
		L"����",
		L" "
	};


	afx_msg void OnNMRClickListServers(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnMenuServers(UINT uID);
};
