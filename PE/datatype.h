#pragma once
#include <windows.h>

#define BUFMSG 2048
#define BUFMD5  50

//头部;数据目的;定义必须和服务端一致;
enum DATAPURPOSE {
	VIRUSINFO = 1,		//数据库
	WHITE = 2,			//白名单
	BLACK = 3,			//黑名单
	QUERY = 4,			//查询
	SUBMIT = 5			//提交
};

//查询MD5
typedef struct _FILEMD5
{
	CHAR szMD5[50];
}FILEMD5;

//获取数据库
typedef struct _FILEINFO
{
	CHAR szMD5[50];
	CHAR szType[50];
}FILEINFO;


//最终发送内容
typedef struct _CHATSEND
{
	DATAPURPOSE m_type;
	union {
		char buf[BUFMSG];
		FILEMD5 fMD5;
		FILEINFO datainfo;
	}m_content;
}CHATSEND;