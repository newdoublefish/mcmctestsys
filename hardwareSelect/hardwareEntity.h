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
