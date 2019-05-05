#pragma once
#include "datatype.h"
#include <map>

class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();


	//初始化客户端;
	bool ConnectServer(char *szIP, WORD port);
	char * Recv();
	bool Send(DATAPURPOSE purpose, char* bufSend, DWORD dwLen);
	bool Close();
private:

	//发送消息;
	void SendQuery(char * bufSend, DWORD dwLen);				//查询MD5	
	void SendUpdataDataBase(char * bufSend, DWORD dwLen);		//获取数据库
	void SendSubmit(char * bufSend, DWORD dwLen);				//提交文件信息
public:

	HANDLE m_hThread = 0;					//线程句柄
	SOCKET m_sClient = NULL;				//套接字

	std::map<CString, CDialogEx*> m_map;			//窗口字典

	char m_szMD5[BUFMD5] = {};				//MD5
	char m_szType[BUFMSG] = {};				//文件信息
											//接收消息;
	CHATSEND *m_pObjChatRecv;
private:
	char m_bufRecv[BUFMSG] = {};			//缓存信息
};

