// PE_OptionalHeader.cpp : 实现文件
//

#include "stdafx.h"
#include "PE.h"
#include "PE_OptionalHeader.h"
#include "afxdialogex.h"


// CPE_OptionalHeader 对话框

IMPLEMENT_DYNAMIC(CPE_OptionalHeader, CDialogEx)

CPE_OptionalHeader::CPE_OptionalHeader(PIMAGE_NT_HEADERS32 pNt32, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_OPTIONALHEADER, pParent)
	, m_BaseOfCode(_T(""))
	, m_BaseOfData(_T(""))
	, m_Checksum(_T(""))
	, m_DllCharacter(_T(""))
	, m_EntryPoint(_T(""))
	, m_FileAlignment(_T(""))
	, m_ImageBase(_T(""))
	, m_Magic(_T(""))
	, m_LoaderFlags(_T(""))
	, m_majorImageVersion(_T(""))
	, m_MajorLinkerVersion(_T(""))
	, m_MajorOsVersion(_T(""))
	, m_MajorSubSystemVersion(_T(""))
	, m_MinorImageVersion(_T(""))
	, m_MinorLinkerVersion(_T(""))
	, m_MinorOSVersion(_T(""))
	, m_MinorSubSystemVersion(_T(""))
	, m_NumberOfRvaAndSize(_T(""))
	, m_SectionAlignmemt(_T(""))
	, m_SizeOfCode(_T(""))
	, SizeOfHeaders(_T(""))
	, m_SizeOfHeapCommit(_T(""))
	, m_SizeOfHeapReserve(_T(""))
	, m_SizeOfImage(_T(""))
	, m_SizeOfInitData(_T(""))
	, m_SizeOfStackCommit(_T(""))
	, m_SizeOfStackReserve(_T(""))
	, m_SizeOfUnInitData(_T(""))
	, m_Subsystem(_T(""))
	, m_Win32Version(_T(""))
{
	m_pNt32 = pNt32;
}
CPE_OptionalHeader::CPE_OptionalHeader(PIMAGE_NT_HEADERS64 pNt64, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PE_OPTIONALHEADER, pParent)
	, m_BaseOfCode(_T(""))
	, m_BaseOfData(_T(""))
	, m_Checksum(_T(""))
	, m_DllCharacter(_T(""))
	, m_EntryPoint(_T(""))
	, m_FileAlignment(_T(""))
	, m_ImageBase(_T(""))
	, m_Magic(_T(""))
	, m_LoaderFlags(_T(""))
	, m_majorImageVersion(_T(""))
	, m_MajorLinkerVersion(_T(""))
	, m_MajorOsVersion(_T(""))
	, m_MajorSubSystemVersion(_T(""))
	, m_MinorImageVersion(_T(""))
	, m_MinorLinkerVersion(_T(""))
	, m_MinorOSVersion(_T(""))
	, m_MinorSubSystemVersion(_T(""))
	, m_NumberOfRvaAndSize(_T(""))
	, m_SectionAlignmemt(_T(""))
	, m_SizeOfCode(_T(""))
	, SizeOfHeaders(_T(""))
	, m_SizeOfHeapCommit(_T(""))
	, m_SizeOfHeapReserve(_T(""))
	, m_SizeOfImage(_T(""))
	, m_SizeOfInitData(_T(""))
	, m_SizeOfStackCommit(_T(""))
	, m_SizeOfStackReserve(_T(""))
	, m_SizeOfUnInitData(_T(""))
	, m_Subsystem(_T(""))
	, m_Win32Version(_T(""))
{
	m_pNt64 = pNt64;
}

CPE_OptionalHeader::~CPE_OptionalHeader()
{
}

void CPE_OptionalHeader::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_O_BaseOfCode, m_BaseOfCode);
	DDX_Text(pDX, IDC_O_BaseOfData, m_BaseOfData);
	DDX_Text(pDX, IDC_O_Checksum, m_Checksum);
	DDX_Text(pDX, IDC_O_DLLcharacter, m_DllCharacter);
	DDX_Text(pDX, IDC_O_EntryPoint, m_EntryPoint);
	DDX_Text(pDX, IDC_O_FileAlignment, m_FileAlignment);
	DDX_Text(pDX, IDC_O_ImageBase, m_ImageBase);
	DDX_Text(pDX, IDC_O_Magic, m_Magic);
	DDX_Text(pDX, IDC_O_LoaderFlags, m_LoaderFlags);
	DDX_Text(pDX, IDC_O_MajorImageVersion, m_majorImageVersion);
	DDX_Text(pDX, IDC_O_MajorLinkerVersion, m_MajorLinkerVersion);
	DDX_Text(pDX, IDC_O_MajorOSVersion, m_MajorOsVersion);
	DDX_Text(pDX, IDC_O_MajorSubSystemVersion, m_MajorSubSystemVersion);
	DDX_Text(pDX, IDC_O_MinorImageVersion, m_MinorImageVersion);
	DDX_Text(pDX, IDC_O_MinorLinkerVersion, m_MinorLinkerVersion);
	DDX_Text(pDX, IDC_O_MinorOSVersion, m_MinorOSVersion);
	DDX_Text(pDX, IDC_O_MinorSubSystemVersion, m_MinorSubSystemVersion);
	DDX_Text(pDX, IDC_O_NumberOfRvaandSizes, m_NumberOfRvaAndSize);
	DDX_Text(pDX, IDC_O_SecionAlignment, m_SectionAlignmemt);
	DDX_Text(pDX, IDC_O_SizeOfCode, m_SizeOfCode);
	DDX_Text(pDX, IDC_O_SizeOfHeaders, SizeOfHeaders);
	DDX_Text(pDX, IDC_O_SizeOfHeapCommit, m_SizeOfHeapCommit);
	DDX_Text(pDX, IDC_O_SizeOfHeapReserve, m_SizeOfHeapReserve);
	DDX_Text(pDX, IDC_O_SizeOfImage, m_SizeOfImage);
	DDX_Text(pDX, IDC_O_SizeOfInitData, m_SizeOfInitData);
	DDX_Text(pDX, IDC_O_SizeOfStackCommit, m_SizeOfStackCommit);
	DDX_Text(pDX, IDC_O_SizeOfStackReserve, m_SizeOfStackReserve);
	DDX_Text(pDX, IDC_O_SizeOfUnInitData, m_SizeOfUnInitData);
	DDX_Text(pDX, IDC_O_Subsystem, m_Subsystem);
	DDX_Text(pDX, IDC_O_Win32VersionValue, m_Win32Version);
}


BEGIN_MESSAGE_MAP(CPE_OptionalHeader, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPE_OptionalHeader 消息处理程序


BOOL CPE_OptionalHeader::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (m_pNt32)	//32
	{
		GetOptionalHeaderInfo32();
	}
	else
	{
		GetOptionalHeaderInfo64();
	}
		

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CPE_OptionalHeader::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

//获取扩展头信息
void CPE_OptionalHeader::GetOptionalHeaderInfo32()
{
	UpdateData(TRUE);

	m_Magic.Format(L"%04X", m_pNt32->OptionalHeader.Magic);
	m_MajorLinkerVersion.Format(L"%02X", m_pNt32->OptionalHeader.MajorLinkerVersion);
	m_MinorLinkerVersion.Format(L"%02X", m_pNt32->OptionalHeader.MinorLinkerVersion);
	m_SizeOfCode.Format(L"%08X", m_pNt32->OptionalHeader.SizeOfCode);
	m_SizeOfInitData.Format(L"%08X", m_pNt32->OptionalHeader.SizeOfInitializedData);
	m_SizeOfUnInitData.Format(L"%08X", m_pNt32->OptionalHeader.SizeOfUninitializedData);	
	m_EntryPoint.Format(L"%08X", m_pNt32->OptionalHeader.AddressOfEntryPoint);
	m_BaseOfCode.Format(L"%08X", m_pNt32->OptionalHeader.BaseOfCode);
	m_BaseOfData.Format(L"%08X", m_pNt32->OptionalHeader.BaseOfData);
	m_ImageBase.Format(L"%08X", m_pNt32->OptionalHeader.ImageBase);
	m_SectionAlignmemt.Format(L"%08X", m_pNt32->OptionalHeader.SectionAlignment);
	m_FileAlignment.Format(L"%08X", m_pNt32->OptionalHeader.FileAlignment);
	m_MajorOsVersion.Format(L"%04X", m_pNt32->OptionalHeader.MajorOperatingSystemVersion);
	m_MinorOSVersion.Format(L"%04X", m_pNt32->OptionalHeader.MinorOperatingSystemVersion);
	m_majorImageVersion.Format(L"%04X", m_pNt32->OptionalHeader.MajorImageVersion);
	m_MinorImageVersion.Format(L"%04X", m_pNt32->OptionalHeader.MinorImageVersion);
	m_MajorSubSystemVersion.Format(L"%04X", m_pNt32->OptionalHeader.MajorSubsystemVersion);
	m_MinorSubSystemVersion.Format(L"%04X", m_pNt32->OptionalHeader.MinorSubsystemVersion);
	m_Win32Version.Format(L"%08X", m_pNt32->OptionalHeader.Win32VersionValue);
	m_SizeOfImage.Format(L"%08X", m_pNt32->OptionalHeader.SizeOfImage);
	SizeOfHeaders.Format(L"%08X", m_pNt32->OptionalHeader.SizeOfHeaders);
	m_Checksum.Format(L"%08X", m_pNt32->OptionalHeader.CheckSum);
	m_Subsystem.Format(L"%04X", m_pNt32->OptionalHeader.Subsystem);
	m_DllCharacter.Format(L"%04X", m_pNt32->OptionalHeader.DllCharacteristics);
	m_SizeOfStackReserve.Format(L"%08X", m_pNt32->OptionalHeader.SizeOfStackReserve);
	m_SizeOfStackCommit.Format(L"%08X", m_pNt32->OptionalHeader.SizeOfStackCommit);
	m_SizeOfHeapReserve.Format(L"%08X", m_pNt32->OptionalHeader.SizeOfHeapReserve);
	m_SizeOfHeapCommit.Format(L"%08X", m_pNt32->OptionalHeader.SizeOfHeapCommit);
	m_LoaderFlags.Format(L"%08X", m_pNt32->OptionalHeader.LoaderFlags);
	m_NumberOfRvaAndSize.Format(L"%08X", m_pNt32->OptionalHeader.NumberOfRvaAndSizes);


	UpdateData(FALSE);
}

void CPE_OptionalHeader::GetOptionalHeaderInfo64()
{
	m_Magic.Format(L"%04X", m_pNt64->OptionalHeader.Magic);
	m_MajorLinkerVersion.Format(L"%02X", m_pNt64->OptionalHeader.MajorLinkerVersion);
	m_MinorLinkerVersion.Format(L"%02X", m_pNt64->OptionalHeader.MinorLinkerVersion);
	m_SizeOfCode.Format(L"%08X", m_pNt64->OptionalHeader.SizeOfCode);
	m_SizeOfInitData.Format(L"%08X", m_pNt64->OptionalHeader.SizeOfInitializedData);
	m_SizeOfUnInitData.Format(L"%08X", m_pNt64->OptionalHeader.SizeOfUninitializedData);
	m_EntryPoint.Format(L"%08X", m_pNt64->OptionalHeader.AddressOfEntryPoint);
	m_BaseOfCode.Format(L"%08X", m_pNt64->OptionalHeader.BaseOfCode);
	//m_BaseOfData.Format(L"%08X", m_pNt64->OptionalHeader.BaseOfData);				//64位没有这个成员
	m_ImageBase.Format(L"%08X", m_pNt64->OptionalHeader.ImageBase);
	m_SectionAlignmemt.Format(L"%08X", m_pNt64->OptionalHeader.SectionAlignment);
	m_FileAlignment.Format(L"%08X", m_pNt64->OptionalHeader.FileAlignment);
	m_MajorOsVersion.Format(L"%04X", m_pNt64->OptionalHeader.MajorOperatingSystemVersion);
	m_MinorOSVersion.Format(L"%04X", m_pNt64->OptionalHeader.MinorOperatingSystemVersion);
	m_majorImageVersion.Format(L"%04X", m_pNt64->OptionalHeader.MajorImageVersion);
	m_MinorImageVersion.Format(L"%04X", m_pNt64->OptionalHeader.MinorImageVersion);
	m_MajorSubSystemVersion.Format(L"%04X", m_pNt64->OptionalHeader.MajorSubsystemVersion);
	m_MinorSubSystemVersion.Format(L"%04X", m_pNt64->OptionalHeader.MinorSubsystemVersion);
	m_Win32Version.Format(L"%08X", m_pNt64->OptionalHeader.Win32VersionValue);
	m_SizeOfImage.Format(L"%08X", m_pNt64->OptionalHeader.SizeOfImage);
	SizeOfHeaders.Format(L"%08X", m_pNt64->OptionalHeader.SizeOfHeaders);
	m_Checksum.Format(L"%08X", m_pNt64->OptionalHeader.CheckSum);
	m_Subsystem.Format(L"%04X", m_pNt64->OptionalHeader.Subsystem);
	m_DllCharacter.Format(L"%04X", m_pNt64->OptionalHeader.DllCharacteristics);
	m_SizeOfStackReserve.Format(L"%08X", m_pNt64->OptionalHeader.SizeOfStackReserve);
	m_SizeOfStackCommit.Format(L"%08X", m_pNt64->OptionalHeader.SizeOfStackCommit);
	m_SizeOfHeapReserve.Format(L"%08X", m_pNt64->OptionalHeader.SizeOfHeapReserve);
	m_SizeOfHeapCommit.Format(L"%08X", m_pNt64->OptionalHeader.SizeOfHeapCommit);
	m_LoaderFlags.Format(L"%08X", m_pNt64->OptionalHeader.LoaderFlags);
	m_NumberOfRvaAndSize.Format(L"%08X", m_pNt64->OptionalHeader.NumberOfRvaAndSizes);


	UpdateData(FALSE);
}
