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
#include "testGroupInit.h"
#include "convertTool.h"
#include "common.h"
#include "SutPanel.h"
#include "excelHelper.h"
#include "configPath.h"
#include "log.h"


static TestGroup group;
static TestItem item;

static ListType typeList=0;   
static ListType tiaoliGroup=0;	   //������
static ListType tiaoliList=0;	   //����

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
   
   if(start==-1 && middle1==-1 && middle2 ==-1 && end==-1)//���ֵ����Сֵһ�� AAAA
   {
       standard->maxValue=standard->minValue=(double)String2UnsignedLong(expression);
	   return error;
   }else if(start!=-1 && middle1!=-1 && middle2!=-1 && end!=-1) //���ֵ��Сֵ��һ�£�����(0��0.2)V
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
			
			printf("-----------------------\n�������ƣ�%s\n",item.itemName_);  
			printf("���:%d\n",item.itemId);
			printf("��������:%s\n",item.itemType_);
			printf("����ֵ��%f\n",item.input_Value); 
			printf("���ֵ��%f����Сֵ��%f\n",item.standard_value.maxValue,item.standard_value.minValue);  
			printf("���:%d\n",item.testLevel); 
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
 	if(column==1) //������
	{
		if(CA_VariantHasCString(MyVariant))
		{					
			if(group.subItems!=0)//�����Ϊ���������ʱ��Ŵ�
			{
				
				ListInsertItem(tiaoliGroup,&group,END_OF_LIST);
				
				groupIdCount++;
			}
			group.groupId=groupIdCount+1; //���������  
			group.subItems=ListCreate(sizeof(TestItem));  //��������������
			CA_VariantGetCString(MyVariant, &temp);
			memset(group.groupName,0,MAX_GROUP_NAME_LEN);
			memcpy(group.groupName,temp,strlen(temp)+1); //������ֵ
			sprintf(group.nickName,"������%d_%s",group.groupId,group.groupName);
			//sprintf(group.groupName,"������%d_%s",group.groupId,group.groupName);
			//sprintf(group.groupName,"group%d_%s",group.groupId,group.groupName);  
			CA_FreeMemory(temp);
        }
				
	}else if(column==2) //������
	{
		if(CA_VariantHasCString(MyVariant))
		{					
			CA_VariantGetCString(MyVariant, &temp);
			memset(group.type,0,MAX_TESTITEM_TYPE_LEN);  
			memcpy(group.type,temp,strlen(temp)+1); //���͸�ֵ 
			CA_FreeMemory(temp);  
        }
		
		if(CA_VariantHasDouble(MyVariant))
	    {
			double tt;
		    CA_VariantGetDouble(MyVariant,&tt);
			memset(group.type,0,MAX_TESTITEM_TYPE_LEN);
			sprintf(group.type,"%d",(int)tt);//���͸�ֵ
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
	}else if(column==4) //�������� 
	{
		if(CA_VariantIsEmpty(MyVariant))
		{
			quitFlag=1;
			return EXCEL_TASK_QUIT;  //���û�в����ˣ�����Ϊ�����˳�
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
	}else if(column==5) //����ֵ
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
	}else if(column==6)   //����Ҫ��
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
			  //TODO:�������ֵ��Сֵ
			  if(ParseStandardValue(temp,&item.standard_value)==-1)
			  {
						char message[100]={0};
						sprintf(message,"��%d�и�ʽ����\n",itemCount+3);
						WarnShow(message);
						CA_FreeMemory(temp);  
						return EXCEL_TASK_ERROR;   //�����д�����  
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
		 showProgress(item.itemName_,0);//��ʾ����  
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
			
			printf("-----------------------\n�������ƣ�%s\n",item.itemName_);  
			printf("���:%d\n",item.itemId);
			printf("��������:%s\n",item.itemType_);
			printf("����ֵ��%f\n",item.input_Value); 
			printf("���ֵ��%f����Сֵ��%f\n",item.standard_value.maxValue,item.standard_value.minValue);  
			printf("���:%d\n",item.testLevel); 
		 }	 
	 }*/
	 TestGroup group;
	 TestItem item;
	 for(int i=1;i<=ListNumItems(tiaoliGroup);i++)
	 {
	     ListGetItem(tiaoliGroup,&group,i);
		 //printf("<<<<<<<<<<<<<<<<<����ID��%d,�������ƣ�%s,��������:%s\n",group.groupId,group.groupName,group.type);   //LOG_FORMAT
		 LOG_FORMAT("<<<<<<<<<<<<<<<<<����ID��%d,�������ƣ�%s,��������:%s\n",group.groupId,group.groupName,group.type);
		 for(int j=1;j<=ListNumItems(group.subItems);j++)
		 {
		     ListGetItem(group.subItems,&item,j);
			 //printf("-----------------------\n�������ƣ�%s\n",item.itemName_);  
			 //printf("���:%d\n",item.itemId);
			 //printf("����ֵ��%f\n",item.input_Value); 
			 //printf("���ֵ��%f����Сֵ��%f\n",item.standard_value.maxValue,item.standard_value.minValue);  
			 LOG_FORMAT("-----------------------\n�������ƣ�%s\n",item.itemName_);  
			 LOG_FORMAT("���:%d\n",item.itemId);
			 LOG_FORMAT("����ֵ��%f\n",item.input_Value); 
			 LOG_FORMAT("���ֵ��%f����Сֵ��%f\n",item.standard_value.maxValue,item.standard_value.minValue);  
		 }	 
	 }	 
}

static HRESULT onFinishListenerTestCase()
{
	  ItemListCopy(typeList,tiaoliGroup);
	  //ListDispose(typeList);
	  ListInsertItem(tiaoliGroup,&group,END_OF_LIST);
	  slideProgressShowWithTextBox(slidePanelHandle,-1,"�����������������\n",1);
	  disposeSlideProgress(slidePanelHandle); 
	  
	  testCaseCount=0;
	  //printGroupList();
	  return 0;
}
static HRESULT callbackFortestCaseCount(VARIANT *MyVariant,int row,int column)
{
	HRESULT ret=0;
	if(column==3) //��������
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
	
	slidePanelHandle=displaySlideProgressWithTextBox("���ڷ�������������"); 
	slideProgressShowWithTextBox(slidePanelHandle,-1,"��ʼ��������������\n",1);
	
	EXCELTask task1=createExcelTask(filePath,"��������",TEST_CASE_RANGE,8); 
	task1.onCellListener=(void *)callbackFortestCaseCount;
	if(runExcelTask(task1)<0)  //��ȡ����������������
	{	
		return -1;
	}	
	

	EXCELTask task2=createExcelTask(filePath,"��������",TEST_CASE_RANGE,8); 
	task2.onExcelTaskStartListener=(void *)onExcelTaskStartListenerTestCase;
	task2.onCellListener=(void *)onCellListenerTestCase;
	//task2.onRowListener=(void *)onRowListenerTestCase;//������ʾ
	task2.onExcelTaskFinishListener=(void *)onFinishListenerTestCase; 
	if(runExcelTask(task2)<0)  //��ȡ����������������	
	{
	    return -1;
	}	
	//printf("testCaseCount:%d\n",ListNumItems(tiaoliList));
	//printf("testGroupCount:%d\n",ListNumItems(tiaoliGroup)); 
    return error;
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



