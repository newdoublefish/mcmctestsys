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
#ifndef __DEBUG_H__
#define   __DEBUG_H__

#include<stdio.h> 

#ifdef __cplusplus
    extern "C" {
#endif


#define DEBUG
#ifdef DEBUG
#define PRINT_LEVEL(level,format,...)  \
        do   \
		{	  \
		   fprintf(stdout,"[%s]func:%s,(FILE:%s,LINE:%d):"format"\n",level,__FUNCTION__,__FILE__,__LINE__,##__VA_ARGS__); \
		}while(0)
		
#define PRINT(format,...)  \
        do   \
		{	  \
		   fprintf(stdout,"func:%s,(FILE:%s,LINE:%d):"format"\n",__FUNCTION__,__FILE__,__LINE__,##__VA_ARGS__); \
		}while(0)	
		
#else
#define LOG_LEVEL(level,format,...)		
#define LOG(format,...) 
#endif
		
		
#ifdef __cplusplus
	}
#endif
		

#endif
