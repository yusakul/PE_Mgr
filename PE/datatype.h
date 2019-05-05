#pragma once
#include <windows.h>

#define BUFMSG 2048
#define BUFMD5  50

//ͷ��;����Ŀ��;�������ͷ����һ��;
enum DATAPURPOSE {
	VIRUSINFO = 1,		//���ݿ�
	WHITE = 2,			//������
	BLACK = 3,			//������
	QUERY = 4,			//��ѯ
	SUBMIT = 5			//�ύ
};

//��ѯMD5
typedef struct _FILEMD5
{
	CHAR szMD5[50];
}FILEMD5;

//��ȡ���ݿ�
typedef struct _FILEINFO
{
	CHAR szMD5[50];
	CHAR szType[50];
}FILEINFO;


//���շ�������
typedef struct _CHATSEND
{
	DATAPURPOSE m_type;
	union {
		char buf[BUFMSG];
		FILEMD5 fMD5;
		FILEINFO datainfo;
	}m_content;
}CHATSEND;