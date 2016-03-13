#include <formatio.h>
#include <ansi_c.h>
#include <utility.h>
#include <userint.h>
#include "ARINC429DRV.h"

#define MAX_429_CARD 4

static WDC_DEVICE_HANDLE hDevX[MAX_429_CARD]={0};  

static char *OBT_PCI_LINK3_LIB="\\driver\\obt\\PCI_LINK3.lib"; 
static char *OBT_WDAPI901_LIB="\\driver\\obt\\wdapi901.lib";

int hasLoad=0;

int Load429DRV(int hdl)
{   int MudID1,MudID2;
	int Status;
	char libName[MAX_PATHNAME_LEN]={0}; 
	if(hasLoad!=0)
	{
	   return 1;
	}	
	hasLoad=1;//以防重复加载
	GetProjectDir (libName); 
	strcat(libName,OBT_PCI_LINK3_LIB); 
 	//MudID1 = LoadExternalModuleEx (libName,__CVIUserHInst);
	
	
	memset(libName,0,MAX_PATHNAME_LEN);
	GetProjectDir (libName); 
	strcat(libName,OBT_WDAPI901_LIB);	
	//MudID2 = LoadExternalModuleEx (libName,__CVIUserHInst);	
 	MudID1 = LoadExternalModuleEx ("PCI_LINK3.lib",__CVIUserHInst);
	MudID2 = LoadExternalModuleEx ("wdapi901.lib",__CVIUserHInst);		
	
	if((MudID1<0)||(MudID2<0))
	{
		MessagePopup("error","装载驱动文件出错:0");
		hasLoad=0;
		return 0;
	}
	pDeviceFindAndOpen = GetExternalModuleAddr(MudID1,"DeviceFindAndOpen",&Status);
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:1");
		return 0;
	}
	pOBT429TOPCI_LibInit=GetExternalModuleAddr(MudID1,"OBT429TOPCI_LibInit",&Status);
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:2");
		return 0;
	}
	pRX_DATA =GetExternalModuleAddr(MudID1,"RX_DATA",&Status);		 //数据接收
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:3");
		return 0;
	}
	pTX1_SEND =GetExternalModuleAddr(MudID1,"TX1_SEND",&Status);    //发送1
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:4");
		return 0;
	}
	pTX2_SEND =GetExternalModuleAddr(MudID1,"TX2_SEND",&Status);    //发送2
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:5");
		return 0;
	}
	pTX1_INIT =GetExternalModuleAddr(MudID1,"TX1_INIT",&Status);      //发送1
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:6");
		return 0;
	}
	pTX2_INIT =GetExternalModuleAddr(MudID1,"TX2_INIT",&Status);	 // 发送2
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:7");
		return 0;
	}
	pRX_ENABLE =GetExternalModuleAddr(MudID1,"RX_ENABLE",&Status); 	   //接收允许
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:8");
		return 0;
	}
	pTX_ENABLE =GetExternalModuleAddr(MudID1,"TX_ENABLE",&Status);		//发送允许
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:9");
		return 0;
	}
	pDeviceClose =GetExternalModuleAddr(MudID1,"DeviceClose",&Status);    //关闭设备
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:10");
		return 0;
	}
	pOBT429_RESET =GetExternalModuleAddr(MudID1,"OBT429_RESET",&Status);  //设备复位
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:11");
		return 0;
	}
	pRX1_INIT =GetExternalModuleAddr(MudID1,"RX1_INIT",&Status);		 //接收初始化1
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:12");
		return 0;
	}
	pRX2_INIT =GetExternalModuleAddr(MudID1,"RX2_INIT",&Status);		  //接收初始化2
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:13");
		return 0;
	}
	pRX3_INIT =GetExternalModuleAddr(MudID1,"RX3_INIT",&Status); 		 //接收初始化3
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:14");
		return 0;
	}
	pRX4_INIT =GetExternalModuleAddr(MudID1,"RX4_INIT",&Status);    	//接收初始化4
	if(Status<0)
	{
		MessagePopup("error","装载板卡驱动时出错:15");
		return 0;
	}	
    return 1;
}


void OnRXInit( WDC_DEVICE_HANDLE hDevX ) 
{
	// TODO: Add your control notification handler code here
			  unsigned int enable=1;		    //RX1使能(0, 1)
			  unsigned int word_length=0;		//RX1字长 (0, 1)
			  unsigned int parity_select=0; 	//奇偶校验(0,1,2)
			  unsigned int label_enable=0;	    //标号校验(0,1)
			  unsigned int sdi=0; 		        //SDI校验(0,1,2,3,4)
              unsigned int scaler=2;			//波特率设置(0,1,2)
			  unsigned int label1=0;
			  unsigned int label2=0;
			  unsigned int label3=0;
			  unsigned int label4=0;
			  unsigned int label5=0;
			  unsigned int label6=0;
			  unsigned int label7=0; 

              if(hDevX!=0)
			  {
	 	          
			  	  pRX1_INIT(hDevX,enable,word_length,parity_select,label_enable,sdi,scaler,
				  label1,label2,label3,label4,label5,label6,label7 );//RX1通道应用设置
			  	  pRX2_INIT(hDevX,enable,word_length,parity_select,label_enable,sdi,scaler,
				  label1,label2,label3,label4,label5,label6,label7 );//RX2通道应用设置		
				  //printf("1--%x\n",hDevX);  
			  }
              else
			  {
                  MessagePopup("error","请先初始化板卡!");
			  }
 
}

int openObtDevice(int val)
{
    unsigned int enable=1;         //TX1使能(0, 1)
    unsigned int word_length=0;    //TX1字长 (0, 1)
    unsigned int parity=1;         //奇偶校验(0,1,2)
    unsigned int scaler=2;         //(0,1,2)波特率设置
    unsigned int tgap=40;           //字间间距(0~255)
    unsigned int mode=1;           //发送模式(0,1);
    long  status;
	
	status=pOBT429TOPCI_LibInit();
	hDevX[val] = pDeviceFindAndOpen(val); 
	
	if(hDevX[val]!=0)
	{
	   pOBT429_RESET(hDevX[val]); 
	   pTX1_INIT(hDevX[val],enable,word_length,parity,scaler,tgap,mode); //TX1通道应用设置
	   pTX2_INIT(hDevX[val],enable,word_length,parity,scaler,tgap,mode); //TX1通道应用设置
	   OnRXInit(hDevX[val]);
	   return 1;
	}else
	{
	   return 0;
	}	
}

void sendObdArinc429Data(int card,int ch,int*sendBuffer,int sendLen)
{
	if(hDevX[card]==0)
	{
	   openObtDevice(card);
	}
	
    if(ch==1)
		 pTX1_SEND(hDevX[card],sendBuffer,sendLen);
	else
		 pTX2_SEND(hDevX[card],sendBuffer,sendLen); 
}

void closeObtDevice(void)
{
	for(int i=0;i<MAX_429_CARD;i++)
	{
	 if(hDevX[i]!=0)
	 {
        pDeviceClose(hDevX[i]);
		hDevX[i]=0;
	 }

	}
}


void recvObtArinc429Enable(int card ,int ch,unsigned int enable)
{
#if 1	
	if(hDevX[card]==0)
	{
	   openObtDevice(card);
	}
	//printf("2---%x\n",hDevX[card]);  
	pRX_ENABLE(hDevX[card],ch,enable);
#endif
}


void  recvObdArinc429Data(int card,int ch,unsigned int *A429databuf,int *recvLen)
{
    int len=0;
	if(hDevX[card]==0)
	{
	   openObtDevice(card);
	}   
	//printf("3---%x\n",hDevX[card]);
   len=pRX_DATA(hDevX[card],ch,A429databuf,256);
   /*for(int i=0;i<len;i++)
   {
       printf("%x\n",A429databuf[i]);
   }*/
   *recvLen=len;
}
