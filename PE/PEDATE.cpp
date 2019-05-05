#include "stdafx.h"
#include "PEDATE.h"


CPE::CPE()
{
	Init();
	 m_pNt32=0;
	 m_pNt64=0;
}


CPE::~CPE()
{
	Clear();
}

//��ȡ�ļ�
bool CPE::ReadFileToMem(CString m_szFilePath)
{
	//���ļ���ȡ�ļ����
	HANDLE hFile = CreateFile(m_szFilePath, GENERIC_READ, FALSE, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"�ļ���ʧ�ܣ�", L"����", MB_OK);
		return false;
	}
	//��ȡ�ļ���С
	dwFileSize = GetFileSize(hFile, NULL);
	m_pFileImageBase = new BYTE[dwFileSize]{};
	DWORD dwRead;

	//���ļ���ȡ���ڴ���
	bool bRet = ReadFile(hFile, m_pFileImageBase, dwFileSize, &dwRead, NULL);

	if (!bRet) {
		delete[] m_pFileImageBase;
	}

	//�رվ��
	CloseHandle(hFile);
	return true;
}


//�ж��Ƿ�PE�ļ�
bool CPE::IsPEFile()
{
	//ʹ��PIMAGE_DOS_HEADER(ռ64�ֽڣ�����ǰ64���ֽ�
	m_pDosHead = (PIMAGE_DOS_HEADER)m_pFileImageBase;

	//�ж�PE�ļ��ı�ʶ�Ƿ���ȷ����һ�����ԣ���ô���Ͳ���PE�ļ�
	if (m_pDosHead->e_magic != IMAGE_DOS_SIGNATURE)	//0x5A4D('MZ')
	{
		MessageBox(NULL, L"������Ч��PE�ļ�", L"����", MB_OK);
		return false;
	}

	//�ļ�ͷָ��
	m_pFileHead = (IMAGE_FILE_HEADER*)((byte*)m_pFileImageBase + m_pDosHead->e_lfanew + sizeof(DWORD));

	if (m_pFileHead->Machine == IMAGE_FILE_MACHINE_I386)		//32
	{
		m_pNt32 = (PIMAGE_NT_HEADERS32)((PBYTE)m_pFileImageBase + m_pDosHead->e_lfanew);
		m_pOptionalHeader32 = (IMAGE_OPTIONAL_HEADER32*)((byte*)m_pFileImageBase + m_pDosHead->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	}
	else if (m_pFileHead->Machine == IMAGE_FILE_MACHINE_IA64 || m_pFileHead->Machine == IMAGE_FILE_MACHINE_AMD64)	//64
	{
		m_pNt64 = (PIMAGE_NT_HEADERS64)((PBYTE)m_pFileImageBase + m_pDosHead->e_lfanew);
		m_pOptionalHeader64 = (IMAGE_OPTIONAL_HEADER64*)((byte*)m_pFileImageBase + m_pDosHead->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	}
	return true;
}


void CPE::Clear()
{
	delete m_pFileImageBase;
	m_pFileImageBase = NULL;
	m_pDosHead = NULL;
	m_pFileHead = NULL;
	m_pOptionalHeader32 = NULL;
	m_pOptionalHeader64 = NULL;
}

void CPE::Init()
{
	m_pFileImageBase = NULL;
	m_pDosHead = NULL;
	m_pFileHead = NULL;
	m_pOptionalHeader32 = NULL;
	m_pOptionalHeader64 = NULL;
	memset(m_szPath, 0, MAX_PATH);
}
