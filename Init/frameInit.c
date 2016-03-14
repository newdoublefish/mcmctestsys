/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：frameInit.c
 // 文件功能描述:
 //
 // 
 // 创建标识：曾衍仁 20141114
 //
 // 修改标识：曾衍仁  20150814
 // 修改描述：
 //-------------------------------------------------------------------------*/
#include "collectParamInit.h"
#include "frameInit.h"
#include "common.h"
#include "log.h"

#define MAX_SLOT 512
#define MAX_PARAM_PER_SLOT 16

static ListType subFrameList;
static int frameLen;

void initFrameLen()
{
	frameLen=0;
    for(int i=1;i<=ListNumItems(subFrameList);i++)
	{
	      SubFrame subFrame;
		  ListGetItem(subFrameList,&subFrame,i);
		  int postionCnt=ListNumItems(subFrame.positionList);
		  for(int j=1;j<=postionCnt;j++)
		  {
			  Position p;
		      ListGetItem(subFrame.positionList,&p,j);
			  frameLen+=p.len;
		  }
	}
	//printf("frameLne:%d",frameLen);
	
}

ListType initSubFrameList()
{
	ListType subFrameList=ListCreate(sizeof(SubFrame));
	for(int i=0;i<512;i++)
	{
	     SubFrame frame;
		 frame.frameIndex=0;
		 frame.positionList=ListCreate(sizeof(Position));
		 frame.subFrameLen=0;
		 ListInsertItem(subFrameList,&frame,END_OF_LIST);
	}
	int cnt=ListNumItems(subFrameList);
	return subFrameList;
}

void addToFrame(int z,CollectParam param,int step)
{
	int subFrameIndex=z;
	SubFrame subFrame;
	int subFrameWithleastParam=0; 
	ListGetItem(subFrameList,&subFrame,z);
	subFrameWithleastParam=ListNumItems(subFrame.positionList); 
	
	for(int i=z;i<z+step;i++)
	{
	     ListGetItem(subFrameList,&subFrame,i); 
		 int currentParamCnt = ListNumItems(subFrame.positionList);
		 if(currentParamCnt<subFrameWithleastParam && currentParamCnt<=MAX_PARAM_PER_SLOT)
		 {
		       subFrameWithleastParam=currentParamCnt;
			   subFrameIndex = i;
		 }
	}
	
	ListGetItem(subFrameList,&subFrame,subFrameIndex);
	
	Position position;
	position.paramId = param.id;
	position.len = param.storeLen;
	position.dataType = param.dataType;
	
	ListInsertItem(subFrame.positionList,&position,END_OF_LIST);
}

void addToFrame2(int z,CollectParam param,int step,int *subFrameCnt)
{
	int subFrameIndex=z;
	SubFrame subFrame;
	int subFrameWithleastParam=0; 
	//ListGetItem(subFrameList,&subFrame,z);
	//subFrameWithleastParam=ListNumItems(subFrame.positionList); 
	subFrameWithleastParam=subFrameCnt[z-1];
	
	for(int i=z;i<z+step;i++)
	{
	     //ListGetItem(subFrameList,&subFrame,i); 
		 //int currentParamCnt = ListNumItems(subFrame.positionList);
		int currentParamCnt = subFrameCnt[i-1];
		 if(currentParamCnt<subFrameWithleastParam && currentParamCnt<=MAX_PARAM_PER_SLOT)
		 {
		       subFrameWithleastParam=currentParamCnt;
			   subFrameIndex = i;
		 }
	}
	
	ListGetItem(subFrameList,&subFrame,subFrameIndex);
	
	Position position;
	position.paramId = param.id;
	position.len = param.storeLen;
	position.dataType = param.dataType;
	
	subFrameCnt[subFrameIndex-1]+=1;
	
	ListInsertItem(subFrame.positionList,&position,END_OF_LIST);
}


void showFrame()
{
	 SubFrame subFrame;
	 int subFrameCnt=ListNumItems(subFrameList);
     for(int i=1;i<=subFrameCnt;i++)
	 {
	      ListGetItem(subFrameList,&subFrame,i);
		  int postionCnt=ListNumItems(subFrame.positionList);
		  //printf("slot:%d:",i);
		  LOG_FORMAT("\n-----------slot:%d,postionCnt:%d\n:",i,postionCnt);
		  for(int j=1;j<=postionCnt;j++)
		  {
		       Position p;
			   ListGetItem(subFrame.positionList,&p,j);
			   //printf("%d,",p.paramId);
			   LOG_FORMAT("%d,",p.paramId); 
		  
		  }
		  //printf("\n");
	 }
  
}

void initFrame()
{
	
	int slidePanelFrameInitHandle=displaySlideProgressWithTextBox("正在进行飞参帧初始化"); 
	slideProgressShowWithTextBox(slidePanelFrameInitHandle,-1,"开始飞参帧初始化\n",1);	
    ListType list = getCollectParamList();
	subFrameList=initSubFrameList();
	
	//分配偶数采样率
	int paramCount=ListNumItems(list);
	
	for(int i=MAX_SLOT;i>0;i--)
	{
	     for(int j=1;j<=paramCount;j++)
		 {
		 	  CollectParam cParam;
			  ListGetItem(list,&cParam,j);
			  if((cParam.rate==i) && (cParam.rate %2 ==0))
			  {
				  //printf("rate:%d\n",cParam.rate);
			      int step = MAX_SLOT / i;
				  int maxRate = cParam.rate;
				  for(int z=1;z<=MAX_SLOT;z+=step)
				  {
				       if(maxRate -- >0)
					   {
					        addToFrame(z,cParam,step);
					   }
				  }
			  }
		  }
	  }

	slideProgressShowWithTextBox(slidePanelFrameInitHandle,30,"偶数采样点分配完毕\n",0.01);
#if 1	
	  //分配采样率为1
      for(int i=1;i<=paramCount;i++)
	  {
		   CollectParam cParam;
		   ListGetItem(list,&cParam,i);
		   if(cParam.rate == 1)
		   {
			    int step = MAX_SLOT /1;
	            for(int z=1;z<=MAX_SLOT;z+=step)
		        {
		       
		     		  addToFrame(z,cParam,step);
		        }		   
		   
		   }

	     
	 }
	  
	  slideProgressShowWithTextBox(slidePanelFrameInitHandle,60,"采样率1分配完毕\n",0.01);
	  
	int *subFrameCnt;
	
	subFrameCnt = (int *)malloc(MAX_SLOT*sizeof(int));
	memset(subFrameCnt,0,MAX_SLOT*sizeof(int));
	
	
	  
	for(int i=MAX_SLOT;i>0;i--)
	{
	     for(int j=1;j<=paramCount;j++)
		 {
		 	  CollectParam cParam;
			  ListGetItem(list,&cParam,j);
			  if((cParam.rate==i) && (cParam.rate %2 !=0)&&(cParam.rate!=1))
			  {
			      int step = MAX_SLOT / i;
				  int maxRate = cParam.rate;
				  for(int z=1;z<=MAX_SLOT;z+=step)
				  {
				       if(maxRate -- >0)
					   {
					        addToFrame2(z,cParam,step,subFrameCnt);
					   }
				  }
			  }
		  }
	  }	
	
	free(subFrameCnt);
	
	initFrameLen();


	
	slideProgressShowWithTextBox(slidePanelFrameInitHandle,100,"奇数分配完毕\n",0.01);
	slideProgressShowWithTextBox(slidePanelFrameInitHandle,-1,"分析完成\n",1);

	char temp[100]={0};
	sprintf(temp,"数据包长度为%d\n",frameLen+18);
	slideProgressShowWithTextBox(slidePanelFrameInitHandle,-1,temp,1);
	Delay(1);
	
	disposeSlideProgress(slidePanelFrameInitHandle);
#endif	
	//showFrame();
} 


ListType getSubFrameList()
{
	return subFrameList;
}

int getFrameLen()
{
    return frameLen;
}
