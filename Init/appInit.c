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
#include "excel2000.h"    
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "AppInitSplash.h" 
#include "excelHelper.h"   
#include "log.h"
#include "reportDb.h"
#include "common.h"


#define DEBUG

HRESULT EnterAppInit()
{
	int panelLoadHandle;
	if ((panelLoadHandle = LoadPanel (0, "AppInitSplash.uir", LOAD_P)) < 0)
		return -1;
	DisplayPanel(panelLoadHandle);
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_V,"�汾:v0.1");
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG,"���ڳ�ʼ��Excel�齨...");
	
	
	//ע������������д��룬���򲻻��жϣ�����ʱ���
#ifndef DEBUG	
	DisableBreakOnLibraryErrors();
#endif	
	
	//Delay(2);
	InitExcel();
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG,"���ڳ�ʼ��Excel�齨...OK"); 
	
	/*if(initDb()<0)
	{
		WarnShow1(0,"���ݿ�����ʧ��");
		//DeInitExcel();
	}*/
	
	initLogModule();
	
	//Delay(2); 
	
	
	DiscardPanel(panelLoadHandle);
	
	return 0;
}

HRESULT AppDeInit()
{
	quitLogModule();//�Ƿ�����������
	DeInitExcel();
	return 0;
}

  

