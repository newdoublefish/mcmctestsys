 /*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ��ݺ��º��տƼ��ɷ����޹�˾��Ȩ���У�δ����Ȩ����ֹ����
 // �����п�����ʹ�á�
 //
 // �ļ�����FileDownLoadHelper.h
 // �ļ������������ļ�����
 //
 // 
 // ������ʶ�������� 20150930
 //
 // �޸ı�ʶ��
 // �޸�������
 //-------------------------------------------------------------------------*/
#ifndef __FILEDOWNLOADHELPER_H__
#define __FILEDOWNLOADHELPER_H__
#include "userint.h"
#include "eutConfig.h"
#ifdef __cplusplus
   extern "C" {
#endif
typedef enum
{
    FD_COMMON_CANNOT_OPEN=0,//���ܴ򿪴���
	FD_SUCCESS=1,//�����ļ��ɹ�
	FD_ERROR=2 //�����ļ�ʧ��
}FD_RET;  //�����ļ���������

FD_RET downloadNewestFile(char *srcDir,EUT eut,char *localFile);

#ifdef __cplusplus
   }
#endif

#endif
