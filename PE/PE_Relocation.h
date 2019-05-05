#pragma once
#include "afxcmn.h"
#include "MyListCtrl.h"
#include "PE_FOA_Calc.h"
#include <vector>


typedef struct _OFFSET_TYPE
{
	WORD offset : 12;	//��ҳ��ƫ����
	WORD type : 4;		//�ض�λ���ͣ�3��
}OFFSET_TYPE, *POFFSET_TYPE;


typedef struct _RELOCINFO
{
	CString index;
	CString dwRelocRVa;		//��Ҫ�ض�λ����������ַ��RVA��
	CString dwOffset;			//������������ַ������ļ�ƫ��
	CString bType;				//�ض�λ��ʽ
	CString dwRelocValue;		//��������ļ�ƫ��ȡ�������ݣ�������ݾ��������ַ��VA��
	CString bData;			//������õ���VA��ȥ��ַ�õ���������ַ��RVA��������һ���ļ�ƫ�ƣ������е�����ȡ���ŵ��������

}RELOCINFO, *PRELOCINFO;	//����ṹ������һ���ض�λ��Ϣ

typedef struct _RELOCAREAINFO
{
	CString index;
	CString Section;		//�������ڵĽ���
	CString RVA;			//����Ļ�ַ������һ����������ַ��
	CString Items;		//���������ض�λ���� 
	std::vector<RELOCINFO> vecRelocInfo;	//��������ض�λ�ľ�����Ϣ
}RELOCAREAINFO, *PRELOCAREAINFO;			//����ṹ������һ��������ض�λ��Ϣ

// CPE_Relocation �Ի���

class CPE_Relocation : public CDialogEx
{
	DECLARE_DYNAMIC(CPE_Relocation)

public:
	CPE_Relocation(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent = NULL);   // ��׼���캯��
	CPE_Relocation(PIMAGE_DOS_HEADER pDos, PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~CPE_Relocation();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RELOCATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	CMyListCtrl m_list_relocation_block;	//�����б�
	CString m_Sec_Index;
	CString m_Sec_Section;
	CString m_Sec_RVA;
	CString m_Sec_Items;

	CMyListCtrl m_list_relocation_section;		//����Ŀ�б�
	CString m_Block_Index;		//
	CString m_Block_RVA;		//
	CString m_Block_Offset;
	CString m_Block_Type;
	CString m_Block_FarAddress;
	CString m_Block_DataInterpretaion;


	PIMAGE_NT_HEADERS32 m_pNt32;			//NTͷָ��
	PIMAGE_NT_HEADERS64 m_pNt64;			//NTͷָ��

	PIMAGE_DATA_DIRECTORY m_pDesc;		//����Ŀ¼��
	PIMAGE_DOS_HEADER m_pDos;
	byte* pFileImageBase;				//�ļ���ʼ


public:



	std::vector<RELOCAREAINFO> m_vecRelocAreaInfo;
	std::vector<RELOCINFO> m_vecRelocInfo;
	std::vector <PIMAGE_BASE_RELOCATION> m_vecPReloc;

	//��ȡ�ض�λ��������Ϣ
	void GetRelocInfo32();
	void GetRelocInfo64();

	CString GetSectionName(DWORD dwRVA);

	//��ȡ������ϸ��Ϣ
	void GetRelocDataInfo32(PIMAGE_BASE_RELOCATION pRelocFOA);
	//��ȡ������ϸ��Ϣ
	void GetRelocDataInfo64(PIMAGE_BASE_RELOCATION pReloc);


	afx_msg void OnNMClickListRelocaSection(NMHDR *pNMHDR, LRESULT *pResult);
};
