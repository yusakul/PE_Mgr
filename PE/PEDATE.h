#pragma once



class CPE
{
public:
	CPE();
	~CPE();
	
	//读取文件到内存
	bool ReadFileToMem(CString m_szFilePath);

	//判断文件是否为PE文件
	bool IsPEFile();

	void Clear();
	void Init();

public:
	PBYTE m_pFileImageBase;			//文件默认加载基址
	PIMAGE_DOS_HEADER m_pDosHead;//DOS头
	PIMAGE_FILE_HEADER m_pFileHead;//标准pe头

	PIMAGE_OPTIONAL_HEADER32 m_pOptionalHeader32;//32位的可选PE头
	PIMAGE_OPTIONAL_HEADER64 m_pOptionalHeader64;//64位的可选PE头

	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;

	char m_szPath[MAX_PATH];//文件路径

	DWORD dwFileSize;//文件大小
};

