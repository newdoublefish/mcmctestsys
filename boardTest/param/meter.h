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

#ifndef __METER_H__
#define __METER_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include"eutEntity.h"

BOOL getMeterDlt2007Voltage(RSCONFIG config,char *addressr,float *voltage);
BOOL getMeterDlt2007Current(RSCONFIG config,char *address,float *voltage);  
		
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __log_H__ */
