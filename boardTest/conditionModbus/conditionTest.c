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


METHODRET ConditionTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"��ʼ����:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;

	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"�յ�",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}	
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;
		/*if(AlertDialogWithRet(0,"��ѡ��","��ȷ�Ͻ���Ƿ���ȷ","����","��ȷ")==TRUE)
		{
			itemResult.pass=1;	
		}else
			itemResult.pass=0;*/
		tConditonItem conditionItem = {0};
		conditionItem.address = HexStrToUnsignedInt(item.inputValue_);
		printf("0x%x",conditionItem.address);
		//conditionItem.address = 0x0100;
		if(TRUE==ConditionGetItem(resconfig,&conditionItem))
		{
						
		}
		
		//APPEND_INFO_FORMAT(msgPanel,"��������:%s�Ĳ��Խ��Ϊ��%s",item.itemName_,(itemResult.pass==1)?"�ϸ�":"���ϸ�");
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"0x%x",conditionItem.value);
		saveResult(hashTable,&itemResult);
		
	}
	APPEND_INFO_FORMAT(msgPanel,"%s�������",group.groupName);		
	return ret;
}



TPS registerConditionTestTPS(void)
{
	TPS tps=newTps("condition");
	//tps.autoTestFunction=DemoTest;
	tps.testFunction=ConditionTest;
	tps.createTpsPanel=NULL;
	//tps.manualTestFunction=DemoTest;
	return tps;
}

