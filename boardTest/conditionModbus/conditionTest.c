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
#include "tpsHelper.h"
#include "resultSave.h"
#include "common.h"
#include "conditionModbus.h"
#include "EutHelper.h"
#include "ParamSetGet.h"  


METHODRET ConditionTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"��ʼ����:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;

	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"�յ�",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}
	
	if(FALSE==ParamSetDepend(eut,"1ǹ�����������","1"))
	{
		APPEND_INFO(msgPanel,"���������������ʧ��"); 
		WarnShow1(0,"���������������ʧ�ܣ�");
		APPEND_INFO_FORMAT(msgPanel,"%s�������",group.groupName);	
		return ret;
	}else{
		APPEND_INFO(msgPanel,"�ѳɹ����������������");
	}	
	
	if(FALSE==AlertDialogWithRet(0,"waring","������������̣����ҵ�ѹ�Ѿ��ȶ�","��","��"))
	{
			//getStubNetService(ip,port);
		APPEND_INFO_FORMAT(msgPanel,"%s�������",group.groupName);	
		return ret;
	}		
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		/*if(AlertDialogWithRet(0,"��ѡ��","��ȷ�Ͻ���Ƿ���ȷ","����","��ȷ")==TRUE)
		{
			itemResult.pass=1;	
		}else
			itemResult.pass=0;*/
		tConditonItem conditionItem = {0};
		conditionItem.address = HexStrToUnsignedInt(item.inputValue_);
		conditionItem.value = 0x01;
		//printf("0x%x",conditionItem.address);
		//conditionItem.address = 0x0100;
		/*if(strstr(item.itemName_,"Զ���Բ�")!=NULL)
		{
			if(TRUE==ConditionSetItem(resconfig,&conditionItem))
			{
				APPEND_INFO_FORMAT(msgPanel,"%s ֵΪ %d",item.itemName_,conditionItem.value);
						
		}else{
				APPEND_INFO_FORMAT(msgPanel,"%s ֵ��ȡʧ��",item.itemName_); 
				goto DONE;
			}					
		}*/
		
		
		if(TRUE==ConditionGetItem(resconfig,&conditionItem))
		{
			APPEND_INFO_FORMAT(msgPanel,"%s ֵΪ %d",item.itemName_,conditionItem.value);
						
		}else{
			APPEND_INFO_FORMAT(msgPanel,"%s ֵ��ȡʧ��",item.itemName_); 
			goto DONE;
		}
		
		if(strstr(item.standard_,"NA")!=NULL)
		{
		
		}else{
			itemResult.pass=1;	
		}
		
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%d",conditionItem.value);
		saveResult(hashTable,&itemResult);
		
	}
DONE:
	if(FALSE==ParamSetDepend(eut,"1ǹ����ֹͣ���","1"))
	{
		APPEND_INFO(msgPanel,"����ֹͣ�������ʧ��"); 
		WarnShow1(0,"����ֹͣ�������ʧ�ܣ�");
		return ret;
	}else{
		APPEND_INFO(msgPanel,"�ѳɹ�����ֹͣ�������");
	}		
	WarnShow1(0,"��ȴ���ȷ����������Ѿ�ֹͣ"); 
	APPEND_INFO_FORMAT(msgPanel,"%s�������",group.groupName);		
	return ret;
}



TPS registerConditionTestTPS(void)
{
	TPS tps=newTps("condition");
	//tps.autoTestFunction=DemoTest;
	tps.testFunction=ConditionTest;
	//tps.createTpsPanel=NULL;
	//tps.manualTestFunction=DemoTest;
	return tps;
}

