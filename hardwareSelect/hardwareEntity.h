/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ��ݺ��º��տƼ��ɷ����޹�˾��Ȩ���У�δ����Ȩ����ֹ����
 // �����п�����ʹ�á�
 //
 // �ļ�����hardwareEntity.h
 // �ļ���������: Ӳ����Դ���ݴ洢����
 //
 // 
 // ������ʶ�������� 20150721
 //
 // �޸ı�ʶ��
 // �޸�������
 //-------------------------------------------------------------------------*/
#ifndef __HARDWAREENTITY_H__
#define __HARDWAREENTITY_H__

#include <stdlib.h>
#include "toolbox.h"
#include <utility.h>

#ifdef __cplusplus
   extern "C" {
#endif


typedef struct
{
    int arinc429card;
}HARDWARE;

HRESULT saveHardware(HARDWARE hardware);

HARDWARE getHardware(void);

#ifdef __cplusplus
   }
#endif

#endif
