#pragma once



class CPE
{
public:
	CPE();
	~CPE();
	
	//��ȡ�ļ����ڴ�
	bool ReadFileToMem(CString m_szFilePath);

	//�ж��ļ��Ƿ�ΪPE�ļ�
	bool IsPEFile();

	void Clear();
	void Init();

public:
	PBYTE m_pFileImageBase;			//�ļ�Ĭ�ϼ��ػ�ַ
	PIMAGE_DOS_HEADER m_pDosHead;//DOSͷ
	PIMAGE_FILE_HEADER m_pFileHead;//��׼peͷ

	PIMAGE_OPTIONAL_HEADER32 m_pOptionalHeader32;//32λ�Ŀ�ѡPEͷ
	PIMAGE_OPTIONAL_HEADER64 m_pOptionalHeader64;//64λ�Ŀ�ѡPEͷ

	PIMAGE_NT_HEADERS32 m_pNt32;
	PIMAGE_NT_HEADERS64 m_pNt64;

	char m_szPath[MAX_PATH];//�ļ�·��

	DWORD dwFileSize;//�ļ���С
};

