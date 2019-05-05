#pragma once
#include "datatype.h"
#include <map>

class CClientSocket
{
public:
	CClientSocket();
	~CClientSocket();


	//��ʼ���ͻ���;
	bool ConnectServer(char *szIP, WORD port);
	char * Recv();
	bool Send(DATAPURPOSE purpose, char* bufSend, DWORD dwLen);
	bool Close();
private:

	//������Ϣ;
	void SendQuery(char * bufSend, DWORD dwLen);				//��ѯMD5	
	void SendUpdataDataBase(char * bufSend, DWORD dwLen);		//��ȡ���ݿ�
	void SendSubmit(char * bufSend, DWORD dwLen);				//�ύ�ļ���Ϣ
public:

	HANDLE m_hThread = 0;					//�߳̾��
	SOCKET m_sClient = NULL;				//�׽���

	std::map<CString, CDialogEx*> m_map;			//�����ֵ�

	char m_szMD5[BUFMD5] = {};				//MD5
	char m_szType[BUFMSG] = {};				//�ļ���Ϣ
											//������Ϣ;
	CHATSEND *m_pObjChatRecv;
private:
	char m_bufRecv[BUFMSG] = {};			//������Ϣ
};

