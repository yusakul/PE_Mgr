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

//读取文件
bool CPE::ReadFileToMem(CString m_szFilePath)
{
	//打开文件获取文件句柄
	HANDLE hFile = CreateFile(m_szFilePath, GENERIC_READ, FALSE, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, L"文件打开失败！", L"错误", MB_OK);
		return false;
	}
	//获取文件大小
	dwFileSize = GetFileSize(hFile, NULL);
	m_pFileImageBase = new BYTE[dwFileSize]{};
	DWORD dwRead;

	//将文件读取到内存中
	bool bRet = ReadFile(hFile, m_pFileImageBase, dwFileSize, &dwRead, NULL);

	if (!bRet) {
		delete[] m_pFileImageBase;
	}

	//关闭句柄
	CloseHandle(hFile);
	return true;
}


//判断是否PE文件
bool CPE::IsPEFile()
{
	//使用PIMAGE_DOS_HEADER(占64字节）解释前64个字节
	m_pDosHead = (PIMAGE_DOS_HEADER)m_pFileImageBase;

	//判断PE文件的标识是否正确，有一个不对，那么他就不是PE文件
	if (m_pDosHead->e_magic != IMAGE_DOS_SIGNATURE)	//0x5A4D('MZ')
	{
		MessageBox(NULL, L"不是有效的PE文件", L"错误", MB_OK);
		return false;
	}

	//文件头指针
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
