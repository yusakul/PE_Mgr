// MonitorDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "MonitorDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//ȫ�ֱ���
//    �������
#pragma data_seg("Share")
HWND g_hwnd = NULL;            //    �����ھ��������HOOKʱ����
HINSTANCE hInstance = NULL;    //    ��DLL��ʵ�����
HHOOK hhook = NULL;            //    ��깳�Ӿ��
DWORD g_dwProcessId;        //    ����id
HANDLE g_hProcess = NULL;    //    ���汾������Զ�����еľ��    
#pragma data_seg()
#pragma comment(linker,"/section:Share,rws")
							 //    ������������
HANDLE hProcess = NULL;                //    ��ǰ���̾��
bool bIsInjected = false;            //    ��ֻ֤ע��һ��
#define CODE_LENGTH    5                //    ���ָ���

									 // TerminateProcess
typedef BOOL(WINAPI *TypeTerminateProcess)(_In_ HANDLE hProcess, _In_ UINT uExitCode); //Kernel32.dll
TypeTerminateProcess oldTerminateProcess = NULL;
FARPROC pfOldTerminateProcess = NULL;
BOOL WINAPI MyTerminateProcess(_In_ HANDLE hProcess, _In_ UINT uExitCode);
BYTE oldCodeTermPro[CODE_LENGTH];    //    ԭAPI���
BYTE newCodeTermpro[CODE_LENGTH];    //    ��API���

									 //    OpenProcess
typedef HANDLE(WINAPI *TypeOpenProcess)(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ DWORD dwProcessId);
TypeOpenProcess oldOpenProcess = NULL;
FARPROC pfOldOpenProcess = NULL;
HANDLE WINAPI MyOpenProcess(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ DWORD dwProcessId);
BYTE oldCodeOpenPro[CODE_LENGTH];
BYTE newCodeOpenPro[CODE_LENGTH];

BOOL WINAPI HookLoad(HWND hwnd, DWORD dwProcessId);    // ����dll hook ����
VOID WINAPI HookUnload();
VOID Inject();
VOID HookOn();
VOID HookOff();

BOOL SetPrivilege(
	HANDLE hToken, // access token handle
	LPCTSTR lpszPrivilege, // name of privilege to enable/disable
	BOOL bEnablePrivilege // to enable or disable privilege
);

LRESULT CALLBACK MouseProc(        // ��깳���ӹ��̵���
	int nCode,    // hook code
	WPARAM wParam,// message identifier
	LPARAM lParam // mouse coordinates
);

BOOL WriteMemory(LPVOID lpAddress, BYTE* pcode, size_t length); //������Ϊ length �� pcode д���ַ lpAddress �Ľ����ڴ���




//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CMonitorDllApp

BEGIN_MESSAGE_MAP(CMonitorDllApp, CWinApp)
END_MESSAGE_MAP()


// CMonitorDllApp ����

CMonitorDllApp::CMonitorDllApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMonitorDllApp ����

CMonitorDllApp theApp;


// CMonitorDllApp ��ʼ��

BOOL CMonitorDllApp::InitInstance()
{
	CWinApp::InitInstance();
	hInstance = AfxGetInstanceHandle();		//��ȡ��dll���
											/*
											�����Ȩ�ޣ��ٻ�ȡ���̾����
											��Ϊֻ��Ȩ���㹻�����ܻ�ȡ����ǰ���̵ľ����
											*/
	HANDLE hToken;
	BOOL bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
	if (bRet == FALSE) {
		AfxMessageBox(_T("Ȩ������ʧ��"));
	}
	SetPrivilege(hToken, SE_DEBUG_NAME, TRUE);

	DWORD dwPid = ::GetCurrentProcessId();
	hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, 0, dwPid);
	if (hProcess == NULL) {
		CString str;
		str.Format(_T("OpenProcess fail!!, error code = [%d]"), GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}

	Inject();        //    ��ʼע��
	return TRUE;
}


//
//    ʵ���˳��������˳�ʱ��һ��Ҫ�ǵûָ�ԭ������ַ������
//
int CMonitorDllApp::ExitInstance()
{
	HookOff();    //Ҫ�ǵûָ�ԭ������ַ
	return CWinApp::ExitInstance();
}

/*
��깳���ӹ��̣�Ŀ���Ǽ��ر�dll��ʹ�����ĳ���.
��깳�ӵ����ã��������ĳ���򴰿���ʱ���ͻ�����������dll��
��ʹ��DLL������깳��ע�뵽Ŀ������С�
*/
LRESULT CALLBACK MouseProc(
	int nCode,        // hook code
	WPARAM wParam,    // message identifier
	LPARAM lParam    // mouse coordinates
)
{
	return CallNextHookEx(hhook, nCode, wParam, lParam);
}


/*
��װ���ӡ�
���������봰�ھ���ͽ���id��
*/
BOOL WINAPI HookLoad(HWND hwnd, DWORD dwProcessId) {
	BOOL ret = FALSE;
	g_hwnd = hwnd;
	g_dwProcessId = dwProcessId;
	hhook = ::SetWindowsHookEx(WH_MOUSE, MouseProc, hInstance, 0);

	if (hhook == NULL) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}

/*
ж�ع��ӡ�
ע��ж�ع���֮ǰ��һ��Ҫ�ǵûָ�ԭ������ַ������
*/
VOID WINAPI HookUnload() {
	HookOff();    // �ָ�ԭ������ַ
	if (hhook != NULL) {
		UnhookWindowsHookEx(hhook);
	}
	if (hInstance != NULL) {
		FreeLibrary(hInstance);
	}
}

/*
ע�뺯����
��Ҫ���ԭ������ַ�ı��棬���浽 oldCode_[]�У�
����ڵ�ַ�ļ��㣬���浽newCode_[]�У��� jmp xxxx ָ�
����ڵ�ַ = �º�����ַ - ԭ������ַ - ָ���
���һ��Ҫ�ǵ�HookOn����
*/

VOID Inject() {
	if (bIsInjected == TRUE) {
		return;
	}
	bIsInjected = TRUE;// ��ֻ֤ע��һ��

					   //    TerminateProcess
	HMODULE hmodleKernel32;
	hmodleKernel32 = ::LoadLibrary(_T("Kernel32.dll"));
	if (NULL == hmodleKernel32) {
		AfxMessageBox(_T("����Kernel32.dllʧ��"));
		return;
	}

	//    ��ȡԭ������ַ
	oldTerminateProcess = (TypeTerminateProcess)GetProcAddress(hmodleKernel32, "TerminateProcess");
	if (NULL == oldTerminateProcess) {
		AfxMessageBox(_T("��ȡTerminateProcess����ʧ��"));
		return;
	}

	pfOldTerminateProcess = (FARPROC)oldTerminateProcess;
	//    ����ԭ�������
// 	_asm
// 	{
// 		lea edi, oldCodeTermPro
// 		mov esi, pfOldTerminateProcess
// 		cld
// 		mov ecx, CODE_LENGTH
// 		rep movsb
// 	}
	SIZE_T addr_t = (SIZE_T)GetProcAddress(hmodleKernel32, "OpenProcess") - 8;
	BYTE oldCode_t[6] = { 0 };
	memcpy_s(oldCode_t, 5, (void*)addr_t, 5);
	size_t* funaddr_t = (size_t*)(addr_t + 5);




	//    �滻�º������
	newCodeTermpro[0] = 0xe9;
// 	_asm
// 	{
// 		lea eax, MyTerminateProcess
// 		mov ebx, pfOldTerminateProcess
// 		sub eax, ebx
// 		sub eax, CODE_LENGTH
// 		mov dword ptr[newCodeTermpro + 1], eax
// 	}
	DWORD oldProtect_t;
	VirtualProtect((void*)funaddr_t, 10, PAGE_EXECUTE_READWRITE, &oldProtect_t);
	SIZE_T myfun_t = (size_t)MyTerminateProcess;
	memcpy_s(funaddr_t, 5, &myfun_t, 5);
	
	
	

	// OpenProcess
	oldOpenProcess = (TypeOpenProcess)GetProcAddress(hmodleKernel32, "OpenProcess");
	if (NULL == oldOpenProcess) {
		AfxMessageBox(_T("��ȡOpenProcess��ַʧ��"));
		return;
	}
	pfOldOpenProcess = (FARPROC)oldOpenProcess;

// 	_asm
// 	{
// 		lea edi, oldCodeOpenPro
// 		mov esi, pfOldOpenProcess
// 		cld
// 		mov ecx, CODE_LENGTH
// 		rep movsb
// 	}
	SIZE_T addr_o = (SIZE_T)GetProcAddress(hmodleKernel32, "OpenProcess") - 8;
	BYTE oldCode_o[6] = { 0 };
	memcpy_s(oldCode_o, 5, (void*)addr_o, 5);
	size_t* funaddr_o = (size_t*)(addr_o + 5);


	newCodeOpenPro[0] = 0xe9;
// 	_asm
// 	{
// 		lea eax, MyOpenProcess
// 		mov ebx, pfOldOpenProcess
// 		sub eax, ebx
// 		sub eax, CODE_LENGTH
// 		mov dword ptr[newCodeOpenPro + 1], eax
// 
// 	}
	DWORD oldProtect_o;
	VirtualProtect((void*)funaddr_o, 10, PAGE_EXECUTE_READWRITE, &oldProtect_o);
	SIZE_T myfun_o = (size_t)MyOpenProcess;
	memcpy_s(funaddr_o, 5, &myfun_o, 5);

	HookOn();    //�����ϣ���ʼHOOK
}


/*
������Ϊ length �� pcode д���ַ lpAddress �Ľ����ڴ���
*/
BOOL WriteMemory(LPVOID lpAddress, BYTE* pcode, size_t length) {
	ASSERT(hProcess != NULL);
	DWORD dwtemp, dwOldProtect, dwRet, dwWrited;
	dwRet = VirtualProtectEx(hProcess, lpAddress, length, PAGE_READWRITE, &dwOldProtect);
	CString logInfo;

	if (0 == dwRet) {
		logInfo.Format(_T("WriteMemory :: Call VirtualProtectEx fail, eror code = [%d]\n\n"), GetLastError());
		AfxMessageBox(logInfo);
		return FALSE;
	}

	dwRet = WriteProcessMemory(hProcess, lpAddress, pcode, length, (SIZE_T*)&dwWrited);
	if (0 == dwRet || 0 == dwWrited) {
		logInfo.Format(_T("WriteMemory :: Call WriteProcessMomory fail, error code = [%d]\n\n"), GetLastError());
		AfxMessageBox(logInfo);
		return FALSE;
	}

	dwRet = VirtualProtectEx(hProcess, lpAddress, length, dwOldProtect, &dwtemp);
	if (0 == dwRet) {
		logInfo.Format(_T("WriteMemory :: Recover Protect fail, error code = [%d]\n\n"), GetLastError());
		AfxMessageBox(logInfo);
		return FALSE;
	}
	return TRUE;
}

/*
��ʼHOOK��
������Inject ��ʼ���õ���ڵ�ַ����д������ڴ��С�
������º������ newCode_[]��д���ڴ��С�
����һ������ԭ���������õ�ʱ�򣬾ͻ���ת�������º�����λ�á�

ע: ���ﴦ��ĺ������ǵ�ǰ��Ҫ�滻�����к���������ֻ��Inject()�����е��ã�
�����г�ʼ����ʱ���õ��ú�����
*/
VOID HookOn() {
	BOOL ret;
	ret = WriteMemory(pfOldTerminateProcess, newCodeTermpro, CODE_LENGTH);
	if (FALSE == ret) {
		AfxMessageBox(_T("HookOn :: Fail to write pfOldTerminateProcess"));
	}

	ret = WriteMemory(pfOldOpenProcess, newCodeOpenPro, CODE_LENGTH);
	if (FALSE == ret) {
		AfxMessageBox(_T("HookOn :: Fail to write pfOldOpenProcess"));
	}
}


/*
ֹͣHOOK��
�ָ�ԭ������ַ��
ע�����ﴦ����������滻�ĺ���������һ�������ֻ����ж��HOOK�����е���
*/
VOID HookOff() {
	ASSERT(hProcess != NULL);
	BOOL ret;
	ret = WriteMemory(pfOldTerminateProcess, oldCodeTermPro, CODE_LENGTH);
	if (FALSE == ret) {
		AfxMessageBox(_T("HookOff :: fail to recover pfOldTerminateProcess \n\n"));
	}
	ret = WriteMemory(pfOldOpenProcess, oldCodeOpenPro, CODE_LENGTH);
	if (FALSE == ret) {
		AfxMessageBox(_T("HookOff :: fail to recover pfOldOpenProcess"));
	}
}

/*
��������Ȩ�ޡ�
*/
BOOL SetPrivilege(
	HANDLE hToken, // access token handle
	LPCTSTR lpszPrivilege, // name of privilege to enable/disable
	BOOL bEnablePrivilege // to enable or disable privilege
) {

	TOKEN_PRIVILEGES tp;
	LUID luid;
	CString info;
	if (!LookupPrivilegeValue(
		NULL, // lookup privilege on local system
		lpszPrivilege, // privilege to lookup
		&luid)) // receives LUID of privilege
	{
		info.Format(_T("LookupPrivilegeValue error: %u\n"), GetLastError());
		AfxMessageBox(info);
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.
	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL)) {
		info.Format(_T("AdjustTokenPrivileges error: %u\n"), GetLastError());
		AfxMessageBox(info);
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
		info.Format(_T("The token does not have the specified privilege. \n"));
		AfxMessageBox(info);
		return FALSE;
	}
	return TRUE;
}

//
//    �Լ����¶���Ľ�����ֹ������
//    ��鵱ǰҪ��ֹ�Ľ����Ƿ����ܱ������̣��������ֹ�رա�
//
BOOL WINAPI MyTerminateProcess(_In_ HANDLE hProcess, _In_ UINT uExitCode) {
	BOOL ret;
	if (g_hProcess == hProcess) {
		AfxMessageBox(_T("���ܹر��ܱ�������Ŷ����"));
		ret = TRUE;
	}
	else {
		WriteMemory(pfOldTerminateProcess, oldCodeTermPro, CODE_LENGTH);
		ret = oldTerminateProcess(hProcess, uExitCode);
		WriteMemory(pfOldTerminateProcess, newCodeTermpro, CODE_LENGTH);
	}
	return ret;
}

//
//    �Լ�����Ĵ򿪽��̺�����
//    ����ǰ�򿪽���Ϊ�ܱ������̣����¼�¸�Զ�̵��þ����
//
HANDLE WINAPI MyOpenProcess(_In_ DWORD dwDesiredAccess, _In_ BOOL bInheritHandle, _In_ DWORD dwProcessId) {

	HANDLE hProcess = NULL;
	WriteMemory(pfOldOpenProcess, oldCodeOpenPro, CODE_LENGTH);
	hProcess = oldOpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
	if (dwProcessId == g_dwProcessId) {
		g_hProcess = hProcess;
	}

	WriteMemory(pfOldOpenProcess, newCodeOpenPro, CODE_LENGTH);
	return hProcess;
}