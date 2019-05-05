#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"

// CBoot �Ի���

class CBoot : public CDialogEx
{
	DECLARE_DYNAMIC(CBoot)

public:
	CBoot(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBoot();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BOOT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CMyListCtrl m_list_boot;


	CString	m_Name;		  //����
	CString m_Key;		  //��ֵ

	HKEY m_hRootKey1;	  //����1
	HKEY m_hRootKey2;	  //����2
	CString m_Root1;	  //����1
	CString m_Root2;	  //����2
	CString m_SubKey1;	  //�Ӽ�1
	CString m_SubKey2;	  //�Ӽ�2


	void DeleBoot();		  //ɾ��������
	void UpDataBoot();	  //ˢ��������
	void AddBoot();		  //���������
	void GetBootInfo(HKEY hRootKey, CString Path);//��ȡ������
	afx_msg void OnNMRClickListBoot(NMHDR *pNMHDR, LRESULT *pResult);
};
