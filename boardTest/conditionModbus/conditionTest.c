/*---------------------------------------------------------------------------
 // 版权声明：本文件由广东万城万充电动车运营股份有限公司版权所有，未经授权，
 // 禁止第三方进行拷贝和使用。
 //
 // 文件名：
 // 文件功能描述: 
 //
 // 
 // 创建标识：曾衍仁 
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
#include "tpsHelper.h"
#include "resultSave.h"
#include "common.h"
#include "conditionModbus.h"
#include "EutHelper.h"
#include "ParamSetGet.h"  


METHODRET ConditionTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;

	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"空调",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}
	
	if(FALSE==ParamSetDepend(eut,"1枪调试启动充电","1"))
	{
		APPEND_INFO(msgPanel,"发送启动充电命令失败"); 
		WarnShow1(0,"发送启动充电命令失败！");
		APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	
		return ret;
	}else{
		APPEND_INFO(msgPanel,"已成功发送启动充电命令");
	}	
	
	if(FALSE==AlertDialogWithRet(0,"waring","已启动充电流程，并且电压已经稳定","否","是"))
	{
			//getStubNetService(ip,port);
		APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	
		return ret;
	}		
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		/*if(AlertDialogWithRet(0,"请选择","请确认结果是否正确","错误","正确")==TRUE)
		{
			itemResult.pass=1;	
		}else
			itemResult.pass=0;*/
		tConditonItem conditionItem = {0};
		conditionItem.address = HexStrToUnsignedInt(item.inputValue_);
		conditionItem.value = 0x01;
		//printf("0x%x",conditionItem.address);
		//conditionItem.address = 0x0100;
		/*if(strstr(item.itemName_,"远程自测")!=NULL)
		{
			if(TRUE==ConditionSetItem(resconfig,&conditionItem))
			{
				APPEND_INFO_FORMAT(msgPanel,"%s 值为 %d",item.itemName_,conditionItem.value);
						
		}else{
				APPEND_INFO_FORMAT(msgPanel,"%s 值获取失败",item.itemName_); 
				goto DONE;
			}					
		}*/
		
		
		if(TRUE==ConditionGetItem(resconfig,&conditionItem))
		{
			APPEND_INFO_FORMAT(msgPanel,"%s 值为 %d",item.itemName_,conditionItem.value);
						
		}else{
			APPEND_INFO_FORMAT(msgPanel,"%s 值获取失败",item.itemName_); 
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
	if(FALSE==ParamSetDepend(eut,"1枪调试停止充电","1"))
	{
		APPEND_INFO(msgPanel,"发送停止充电命令失败"); 
		WarnShow1(0,"发送停止充电命令失败！");
		return ret;
	}else{
		APPEND_INFO(msgPanel,"已成功发送停止充电命令");
	}		
	WarnShow1(0,"请等待并确保充电流程已经停止"); 
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);		
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

