#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "MyListCtrl.h"


typedef struct _CLEANTYPE
{
	BOOL ALL;
	BOOL ilk;
	BOOL log;
	BOOL obj;
	BOOL pch;
	BOOL pdb;
	BOOL tlog;
	BOOL ipch;
}CLEANTYPE, *PCLEAMTYPE;

// CClean �Ի���

class CClean : public CDialogEx
{
	DECLARE_DYNAMIC(CClean)

public:
	CClean(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClean();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLEAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	BOOL m_check_all;
	BOOL m_check_explore;
	BOOL m_check_recycle;
	BOOL m_check_report;
	BOOL m_check_sysTemp;
	BOOL m_check_userTemp;
	BOOL m_check_test;
	CMyListCtrl m_list_clean;

	CString m_FilePath;	//�ļ�·��
	CString m_FileType;
	CString m_FileName;	//�ļ���
	CString m_index;		//���

	CString m_Result;	//�ļ�˵��

	CString m_Path_Report;	//���󱨸�
	CString m_Path_Sys;		//ϵͳ��ʱ�ļ�
	CString m_Path_IE;		//���������
	CString m_Path_User;	//�û���ʱ�ļ�
	CString m_Path_Test;	//��������ļ���



	CString m_FilePath_Mfceditbrowse1;
	CString m_FilePath_Mfceditbrowse2;


	//�������վ
	void CleanRecycle();


	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedButtonOnekeyclean();

	//ɨ��һ���ļ����µ��������� 
	void myScanDirectory(CString directory_path);

	//ɾ��һ���ļ����µ���������  
	void myDeleteDirectory(CString directory_path);

	//ɨ��һ���ļ����µ���������  
	void EnumFoldAndFile(CString directory_path, PCLEAMTYPE pCleanType);			//ֻɨ��ָ���ļ�����
	void EnumFoldAndDeleteFile(CString directory_path, PCLEAMTYPE pCleanType);	   //ɾ��ָ���ļ�����



	afx_msg void OnBnClickedCheckAll2();
	BOOL m_check_all2;
	BOOL m_check_ilk;
	BOOL m_check_log;
	BOOL m_check_obj;
	BOOL m_check_pch;
	BOOL m_check_pdb;
	BOOL m_check_tlog;
	BOOL m_check_ipch;


	afx_msg void OnBnClickedButtonUserclean();	//��ť���������Զ����ļ������ļ�
	afx_msg void OnBnClickedButtonScansys();	//��ť����ɨ���ļ����������ļ�
	afx_msg void OnEnChangeMfceditbrowse_sys();	//����Ŀ¼�������ļ������ļ����

	afx_msg void OnDropFiles(HDROP hDropInfo);	//��ק�ļ���


	afx_msg void OnEnChangeMfceditbrowse2();//����Ŀ¼��ָ����׺�ļ������ļ����
	afx_msg void OnBnClickedButtonScanhouzhui();//��ť����ɨ���ļ�����ָ����׺�ļ�
};



