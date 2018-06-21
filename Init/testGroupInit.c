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
#include "excel2000.h"    
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "testGroupInit.h"
#include "convertTool.h"
#include "common.h"
#include "SutPanel.h"
#include "excelHelper.h"
#include "configPath.h"
#include "log.h"
#include "debug.h"


static TestGroup group;
static TestItem item;

static ListType typeList=0;   
static ListType tiaoliGroup=0;	   //条例组
static ListType tiaoliList=0;	   //条例

static int groupIdCount=0;

static int itemCount=0;

static int slidePanelHandle;
static int testCaseCount=0;   


#define TEST_CASE_RANGE "A2:I2"


static HRESULT ParseStandardValue(char *expression,VALUE_STANDARD *standard)
{
   HRESULT error = 0; 
   int start=-1,middle1=-1,middle2=-1,end=-1;	
   for(int i=0;i<strlen(expression);i++)
   {
       if(expression[i]=='(')
	   {	   
		   start=i;
	   }else if(expression[i]==-95)
	   {
	       middle1=i;
		   
	   }else if(expression[i]==-64)
	   {
	       middle2=i;
	   }	   
	   else if(expression[i]==')')
	   {
	       end=i;
	   }
	   //printf("expression[i]=%c,%d\n",expression[i],expression[i]);
   }
   
   if(start==-1 && middle1==-1 && middle2 ==-1 && end==-1)//最大值，最小值一致 AAAA
   {
       standard->maxValue=standard->minValue=(double)String2UnsignedLong(expression);
	   return error;
   }else if(start!=-1 && middle1!=-1 && middle2!=-1 && end!=-1) //最大值最小值不一致，例如(0±0.2)V
   {	   
      char *temp;
      char data[10]={0};
      char tolerance[10]={0};
   
      memcpy(data,expression+start+1,middle1-start-1);   
      //printf("hello data=%s",data);
      memcpy(tolerance,expression+middle2+1,end-middle2-1);
      //printf("hello tolerance=%s\n",tolerance);
      standard->maxValue=String2Double(data)+String2Double(tolerance);
      standard->minValue=String2Double(data)-String2Double(tolerance);
      //printf("max=%f,min=%f\n",standard->maxValue,standard->minValue);
      return error;
   }
   
   return -1;
}



void printList(ListType list)
{
     TestGroup group;
	 TestItem item;
	 
	 for(int i=1;i<=ListNumItems(list);i++)
	 {
	     ListGetItem(list,&group,i);
		 //printf("groupIndex=%d\n",group.index);
		 for(int j=1;j<=ListNumItems(group.subItems);j++)
		 {
		    ListGetItem(group.subItems,&item,j);
			
			printf("-----------------------\n参数名称：%s\n",item.itemName_);  
			printf("序号:%d\n",item.itemId);
			printf("参数类型:%s\n",item.itemType_);
			printf("输入值：%f\n",item.input_Value); 
			printf("最大值：%f，最小值：%f\n",item.standard_value.maxValue,item.standard_value.minValue);  
			printf("组别:%d\n",item.testLevel); 
		 }	 
	 }	 
}

static void ItemListCopy(ListType srcList,ListType desList)
{
	 TestItem group;
	 int index=ListNumItems(desList)+1;
	 for(int i=1;i<=ListNumItems(srcList);i++)
     {
	      ListGetItem(srcList,&group,i);
		  group.itemId=index++;
		  ListInsertItem(desList,&group,END_OF_LIST);
	 
	 }		 

}

static HRESULT showProgress(char *itemName,int error)
{
	
	int perc;
	
	//itemCount++; 
    perc=itemCount*100/testCaseCount;
	
	
	if(error>0)
	{	
		char description[100]={0};
		Fmt(description,"%s     %s\n",itemName,"error");
	    slideProgressShowWithTextBox(slidePanelHandle,perc,description,0.01);
	}else{
		char description[100]={0};
		Fmt(description,"%s     %s\n",itemName,"ok");		
		slideProgressShowWithTextBox(slidePanelHandle,perc,description,0.01);
	}
	return 0;
}

static HRESULT onCellListenerTestCase(VARIANT *MyVariant,int row,int column)   
{
	static quitFlag=0;
	char *temp;
 	if(column==1) //组名称
	{
		if(CA_VariantHasCString(MyVariant))
		{					
			if(group.subItems!=0)//这个作为条例组的临时存放处
			{
				
				ListInsertItem(tiaoliGroup,&group,END_OF_LIST);
				
				groupIdCount++;
			}
			group.groupId=groupIdCount+1; //组序号增加  
			group.subItems=ListCreate(sizeof(TestItem));  //创建组内条例组
			CA_VariantGetCString(MyVariant, &temp);
			memset(group.groupName,0,MAX_GROUP_NAME_LEN);
			memcpy(group.groupName,temp,strlen(temp)+1); //组名赋值
			sprintf(group.nickName,"条例组%d_%s",group.groupId,group.groupName);
			//sprintf(group.groupName,"条例组%d_%s",group.groupId,group.groupName);
			//sprintf(group.groupName,"group%d_%s",group.groupId,group.groupName);  
			CA_FreeMemory(temp);
        }
				
	}else if(column==2) //组类型
	{
		if(CA_VariantHasCString(MyVariant))
		{					
			CA_VariantGetCString(MyVariant, &temp);
			memset(group.type,0,MAX_TESTITEM_TYPE_LEN);  
			memcpy(group.type,temp,strlen(temp)+1); //类型赋值 
			CA_FreeMemory(temp);  
        }
		
		if(CA_VariantHasDouble(MyVariant))
	    {
			double tt;
		    CA_VariantGetDouble(MyVariant,&tt);
			memset(group.type,0,MAX_TESTITEM_TYPE_LEN);
			sprintf(group.type,"%d",(int)tt);//类型赋值
		}
	}else if(column==3)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			double testCaseIndex=0;
			CA_VariantGetDouble(MyVariant,&testCaseIndex);
			//item.inputValue=(char *)malloc(30);
			//printf("%f\n",testCaseIndex);
			item.itemId=(int)testCaseIndex;
			//itemCount++;
		}	
	}else if(column==4) //条例名称 
	{
		if(CA_VariantIsEmpty(MyVariant))
		{
			quitFlag=1;
			return EXCEL_TASK_QUIT;  //如果没有参数了，则认为可以退出
		}	
		if(CA_VariantHasCString(MyVariant))
		{
             CA_VariantGetCString(MyVariant, &temp);  
			 //item.itemId=itemCount+1;
			 //printf("%d\n",item.itemId);
			 //item.itemId=itemCount+1;
			 //item.itemName=(char *)malloc(strlen(temp)+1); 
			 memset(item.itemName_,0,MAX_TESTITEM_NAME_LEN); 
			 memcpy(item.itemName_,temp,strlen(temp)+1); 
				CA_FreeMemory(temp);
		}	    
	}else if(column==5) //输入值
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(item.input_Value));
			//item.inputValue=(char *)malloc(30);
			
			memset(item.inputValue_,0,MAX_TESTITEM_INPUTVALUE_LEN);
			sprintf(item.inputValue_,"%f",item.input_Value);
		}
		if(CA_VariantHasCString(MyVariant))
		{
			CA_VariantGetCString(MyVariant, &temp);
			item.input_Value=(double)String2UnsignedLong(temp);
			//item.inputValue=(char *)malloc(strlen(temp)+1);
			memset(item.inputValue_,0,MAX_TESTITEM_INPUTVALUE_LEN);
			memset(item.inputValue_,0,strlen(temp)+1);
			memcpy(item.inputValue_,temp,strlen(temp)+1);
			CA_FreeMemory(temp); 
		}
	}else if(column==6)   //技术要求
	{
		if(CA_VariantHasDouble(MyVariant))
		{
			 CA_VariantGetDouble(MyVariant,&(item.standard_value.maxValue));
			 item.standard_value.minValue=item.standard_value.maxValue;
			 //item.standard=(char *)malloc(30);
			 memset(item.standard_,0,MAX_TESTITEM_STANDARD_LEN);
			 sprintf(item.standard_,"%f",item.standard_value.minValue);
         }
		 if(CA_VariantHasCString(MyVariant))
		 {
			  CA_VariantGetCString(MyVariant, &temp);
			  //TODO:解码最大值最小值
			  //PRINT("%s\n",temp);
			  if(ParseStandardValue(temp,&item.standard_value)==-1)
			  {
						char message[100]={0};
						sprintf(message,"第%d行格式有误\n",itemCount+3);
						WarnShow1(0,message);
						CA_FreeMemory(temp);  
						return EXCEL_TASK_ERROR;   //参数有错，返回  
			  }	
					//item.standard=(char *)malloc(strlen(temp)+1);
			  memset(item.standard_,0,MAX_TESTITEM_STANDARD_LEN); 
					//memset(item.standard,0,strlen(temp)+1);
			  memcpy(item.standard_,temp,strlen(temp)+1);
					
			  CA_FreeMemory(temp);
		  }
		 ListInsertItem(group.subItems,&item,END_OF_LIST);
		 ListInsertItem(tiaoliList,&item,END_OF_LIST);
		 itemCount++;
		 showProgress(item.itemName_,0);//显示进度  
	}	
	return EXCEL_TASK_OK; 
}


static HRESULT onRowListenerTestCase(void)
{
	
	int perc;
	char temp[100]={0};
	//itemCount++; 
	if(itemCount<=testCaseCount)
    	perc=itemCount*100/testCaseCount;
	else
		perc=100;
	Fmt(temp,"%d percent",perc);
	slideProgressShow(slidePanelHandle,perc,temp,0.01);
	return 0;
}

void printGroupList()
{
/*     TestItem group;
	 TestItem item;
	 
	 for(int i=1;i<=ListNumItems(list);i++)
	 {
	     ListGetItem(list,&group,i);
		 for(int j=1;j<=ListNumItems(group.subItems);j++)
		 {
		    ListGetItem(group.subItems,&item,j);
			
			printf("-----------------------\n参数名称：%s\n",item.itemName_);  
			printf("序号:%d\n",item.itemId);
			printf("参数类型:%s\n",item.itemType_);
			printf("输入值：%f\n",item.input_Value); 
			printf("最大值：%f，最小值：%f\n",item.standard_value.maxValue,item.standard_value.minValue);  
			printf("组别:%d\n",item.testLevel); 
		 }	 
	 }*/
	 TestGroup group;
	 TestItem item;
	 for(int i=1;i<=ListNumItems(tiaoliGroup);i++)
	 {
	     ListGetItem(tiaoliGroup,&group,i);
		 //printf("<<<<<<<<<<<<<<<<<集合ID：%d,集合名称：%s,测试类型:%s\n",group.groupId,group.groupName,group.type);   //LOG_FORMAT
		 LOG_FORMAT("<<<<<<<<<<<<<<<<<集合ID：%d,集合名称：%s,测试类型:%s\n",group.groupId,group.groupName,group.type);
		 for(int j=1;j<=ListNumItems(group.subItems);j++)
		 {
		     ListGetItem(group.subItems,&item,j);
			 //printf("-----------------------\n参数名称：%s\n",item.itemName_);  
			 //printf("序号:%d\n",item.itemId);
			 //printf("输入值：%f\n",item.input_Value); 
			 //printf("最大值：%f，最小值：%f\n",item.standard_value.maxValue,item.standard_value.minValue);  
			 LOG_FORMAT("-----------------------\n参数名称：%s\n",item.itemName_);  
			 LOG_FORMAT("序号:%d\n",item.itemId);
			 LOG_FORMAT("输入值：%f\n",item.input_Value);
			 LOG_FORMAT("技术要求：%s\n",item.standard_); 
			 LOG_FORMAT("最大值：%f，最小值：%f\n",item.standard_value.maxValue,item.standard_value.minValue);  
		 }	 
	 }	 
}

static HRESULT onFinishListenerTestCase()
{
	  ItemListCopy(typeList,tiaoliGroup);
	  //ListDispose(typeList);
	  ListInsertItem(tiaoliGroup,&group,END_OF_LIST);
	  slideProgressShowWithTextBox(slidePanelHandle,-1,"分析测试条例表完成\n",1);
	  disposeSlideProgress(slidePanelHandle); 
	  
	  testCaseCount=0;
	  //printGroupList();
	  return 0;
}
static HRESULT callbackFortestCaseCount(VARIANT *MyVariant,int row,int column)
{
	HRESULT ret=0;
	if(column==3) //参数名称
	{
				
		if(CA_VariantIsEmpty(MyVariant))
		{
			return -1;
		}else
		{
		    testCaseCount++;
		}	
	}
	return ret;
}

static HRESULT  onExcelTaskStartListenerTestCase(void)
{
	
	groupIdCount=0;
	itemCount=0;
	//testCaseCount=0;
	
	if(tiaoliGroup!=0)
		ListDispose(tiaoliGroup);
	tiaoliGroup = ListCreate (sizeof(TestGroup));
	
	memset(&group,0,sizeof(TestGroup));
	
	if(tiaoliList!=0)
		ListDispose(tiaoliList);
	tiaoliList = ListCreate (sizeof(TestItem));
	
	memset(&item,0,sizeof(TestItem));
	
	
	
	return 0;
}


HRESULT testGroupDeinit(void)
{
	if(tiaoliGroup!=0)
		ListDispose(tiaoliGroup);
	tiaoliGroup=0;
	
	if(tiaoliList!=0)
		ListDispose(tiaoliList);
	tiaoliList=0;
	
	return TRUE;
}

HRESULT testGroupInit(char *filePath)
{
	HRESULT error = 0;
	if(strstr(filePath,".xlsx")!=NULL)
	{
		slidePanelHandle=displaySlideProgressWithTextBox("正在分析测试条例表"); 
		slideProgressShowWithTextBox(slidePanelHandle,-1,"开始分析测试条例表\n",1);
	
		EXCELTask task1=createExcelTask(filePath,"测试条例",TEST_CASE_RANGE,8); 
		task1.onCellListener=(void *)callbackFortestCaseCount;
		if(runExcelTask(task1)<0)  //获取到测试条例的数量
		{	
			return -1;
		}	
	

		EXCELTask task2=createExcelTask(filePath,"测试条例",TEST_CASE_RANGE,8); 
		task2.onExcelTaskStartListener=(void *)onExcelTaskStartListenerTestCase;
		task2.onCellListener=(void *)onCellListenerTestCase;
		//task2.onRowListener=(void *)onRowListenerTestCase;//进度显示
		task2.onExcelTaskFinishListener=(void *)onFinishListenerTestCase; 
		if(runExcelTask(task2)<0)  //获取到测试条例的数量	
		{
	    	return -1;
		}	
		//printf("testCaseCount:%d\n",ListNumItems(tiaoliList));
		//printf("testGroupCount:%d\n",ListNumItems(tiaoliGroup)); 
    	return error;
	}else if(strstr(filePath,".xml")!=NULL)
	{
		
	}
	
	return -1;
}


void getTestGroup(TestGroup *groupPtr,char *groupName)
{
	  int num=ListNumItems(tiaoliGroup);
      for(int i=1;i<=num;i++)
	  {
	  	  ListGetItem(tiaoliGroup,groupPtr,i);
		  if(strcmp(groupPtr->groupName,groupName)==0)
		  {
			  //printf("group.groupName:%s,sheetName:%s\n",groupPtr->groupName,groupName);
		      break;
		  }	  
	  }	  
}


void getGroupItemByName(TestGroup group,TestItem *item,char *itemName)
{
	  int itemCount=ListNumItems(group.subItems);
	  for(int i=1;i<=itemCount;i++)
	  {
	      ListGetItem(group.subItems,item,i);
		  if(strcmp(item->itemName_,itemName)==0)
		  {
		     break;
		  }	  
	  }	  
}

int isTypeGroupExsit(char *typeName)
{
	  int itemCount=ListNumItems(tiaoliGroup);
	  TestGroup group;
	  for(int i=1;i<=itemCount;i++)
	  {
	      ListGetItem(tiaoliGroup,&group,i);
		  if(strcmp(group.type,typeName)==0)
		  {
		     return 1;
		  }	  
	  }	  
	  return 0;
}

ListType getTestCaseList(void)
{
   return tiaoliList;
}
ListType getItemList(void)
{
    return tiaoliGroup;
}

int getTestCaseId(char *itemName,double intputValue,char *valueString)
{
	int count=ListNumItems(tiaoliList);
	int retId=-1;
	//printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<,count:%d\n",count);
	for(int i=1;i<=count;i++)
	{
	   TestItem item;
	   ListGetItem(tiaoliList,&item,i);
	  // printf("---count---%d",i);
	  // printf("itemName:%s,valueString:%s\n",itemName,valueString);
	  // printf("item.itemName:%s,item.valueString:%s\n",item.itemName_,item.inputValue_); 
	   if((strcmp(itemName,item.itemName_)==0) && (strcmp(item.standard_,valueString)==0))
	   {
	   	   retId=i;
	       break;
	   }	   
	}
	return retId;
}



