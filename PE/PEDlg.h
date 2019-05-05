
// PEDlg.h : ͷ�ļ�
//

#pragma once

#include "PEINFO.h"
#include "PEDATE.h"
#include "TaskMgr.h"
#include "afxcmn.h"
#include "MyTab.h"
#include "AntiVirus.h"
#include "Clean.h"
#include "Home.h"
#include "Speed.h"
#include "Tools.h"


// CPEDlg �Ի���
class CPEDlg : public CDialogEx
{
// ����
public:
	CPEDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnClose();//    �رճ����ʱ��ж��DLL !!!!!



	CString m_szPath;
	

	afx_msg void OnEnChangeMfceditbrowse1();

	//���������===================================================================
	CTaskMgr task;								//��������������
	afx_msg void OnBnClickedButtonOpentask();	//��ť���������������

	//��ҳ�˵�=====================================================================
	afx_msg void OnCancel();
	afx_msg void OnLogout();		//ע��
	afx_msg void OnShutdown();		//�ػ�
	afx_msg void OnReboot();		//����
	afx_msg void OnHotkey();		//�ȼ�

	//��Ȩ����
	BOOL AdjustPrivilege();


	//��ҳTabҳ��======================================================================
	CMyTab m_Tab_Home;

	//����ԱȨ�����а�ť==============================================================
	afx_msg void OnBnClickedButtonAdmin();

	//�Ƿ��ǹ���Ա
	BOOL IsAdmin(HANDLE hProcess);

	
	//���̱���=======================================================================Start
	HINSTANCE m_hinstHookDll;    //    MonitorDll��ʵ�����
	void HookLoad();            //    ����HOOK            
	void HookUnload();            //    ж��HOOK
	//���̱���=======================================================================End

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonProctect();

	//�����ַ����ע��Ŀ��
	bool StartInsert(char * title, char * winclass, char * dllpath);

	//��дע��
	void StartInject();
	
	//��ȡĿ�����PID
	HANDLE GetThePidOfTargetProcess(HWND hwnd);
	
	//ע��
	BOOL DoInjection(char * DllPath, HANDLE hProcess);
};
