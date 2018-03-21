/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ㶫������綯����Ӫ�ɷ����޹�˾��Ȩ���У�δ����Ȩ��
 // ��ֹ���������п�����ʹ�á�
 //
 // �ļ�����
 // �ļ���������: 
 //
 // 
 // ������ʶ�������� 
 //
 // �޸ı�ʶ��
 // �޸�������
 //-------------------------------------------------------------------------*/
#ifndef __POSTDATA_H__
#define __POSTDATA_H__ 

#ifdef __cplusplus
    extern "C" {
#endif

#define POST_DATA_VALUE 256
#define POST_DATA_BASE 50

typedef struct{
	char name[POST_DATA_BASE];
	char type[POST_DATA_BASE];
	char value[POST_DATA_VALUE];
}tPostParam;

typedef struct{
	char name[30];
	char url[POST_DATA_VALUE];
	char request[POST_DATA_BASE];
	ListType postParamList;
}tPostData;
typedef BOOL (*ON_FILL_DATA_PARAM)(tPostParam *);

BOOL buildPostDataStr(tPostData data,char *buffer,char *callbackFunc);
BOOL importPostProtocol(void);
BOOL getPostData(tPostData *postDataPtr,char *name);

#ifdef __cplusplus
    }
#endif

#endif
