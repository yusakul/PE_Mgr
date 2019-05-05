#include "stdafx.h"
#include "ClientSocket.h"
#include <WS2tcpip.h>


CClientSocket::CClientSocket()
{
}


CClientSocket::~CClientSocket()
{
}




//初始化套接字
bool CClientSocket::ConnectServer(char *szIP, WORD port)
{
	//初始化套接字;
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		return false;
	}

	//检查库版本
	if (LOBYTE(wsd.wVersion) != 2 || HIBYTE(wsd.wHighVersion) != 2)
	{
		WSACleanup();
		return false;
	}

	//创建套接字;
	m_sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_sClient == INVALID_SOCKET)
	{
		WSACleanup();
		return false;
	}

	//设置服务器地址;
	SOCKADDR_IN serverAddr = {};
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, szIP, &(serverAddr.sin_addr.S_un.S_addr));
	serverAddr.sin_port = htons((short)port);

	//连接服务端
	int nRetValue = connect(m_sClient, (sockaddr*)&serverAddr, sizeof(SOCKADDR_IN));
	if (nRetValue == SOCKET_ERROR)
	{
		closesocket(m_sClient);
		m_sClient = NULL;
		WSACleanup();
		return false;
	}

	return true;
}


//消息接收
char * CClientSocket::Recv()
{
	//接收消息;
	CHATSEND ct = {};
	if (SOCKET_ERROR == recv(m_sClient, (char*)&ct, sizeof(CHATSEND), NULL))
	{
		printf("recv error!\n");
		return nullptr;
	}
	//转存数据
	m_pObjChatRecv = &ct;

	//分发消息
	switch (ct.m_type)
	{
	case VIRUSINFO:
		break;
	case WHITE:
		break;
	case BLACK:
		break;
	case QUERY:
		break;
	case SUBMIT:
		break;
	default:
		break;
	}

	return nullptr;
}


//发送消息
bool CClientSocket::Send(DATAPURPOSE purpose, char* bufSend, DWORD dwLen)
{
	//分发消息
	switch (purpose)
	{
	case VIRUSINFO:
	case WHITE:
	case BLACK:
		SendUpdataDataBase(bufSend, dwLen);
		break;
	case QUERY:
		SendQuery(bufSend, dwLen);
		break;
	case SUBMIT:
		break;
	default:
		break;
	}

	return true;
}

//关闭套接字
bool CClientSocket::Close()
{
	closesocket(m_sClient);
	WSACleanup();
	return true;
}



//查询MD5
void CClientSocket::SendQuery(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { QUERY };
	char * pwd = nullptr;
	//处理数据
	memcpy_s(ct.m_content.fMD5.szMD5, strlen(bufSend) + 1, bufSend, strlen(bufSend) + 1);
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}


//获取数据库
void CClientSocket::SendUpdataDataBase(char * bufSend, DWORD dwLen)
{
	CHATSEND ct = { (DATAPURPOSE)dwLen };
	memcpy_s(ct.m_content.buf, strlen(bufSend), bufSend, strlen(bufSend));
	send(m_sClient, (char *)&ct, sizeof(ct), NULL);
}


