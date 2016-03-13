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
	hasLoad=1;//�Է��ظ�����
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
		MessagePopup("error","װ�������ļ�����:0");
		hasLoad=0;
		return 0;
	}
	pDeviceFindAndOpen = GetExternalModuleAddr(MudID1,"DeviceFindAndOpen",&Status);
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:1");
		return 0;
	}
	pOBT429TOPCI_LibInit=GetExternalModuleAddr(MudID1,"OBT429TOPCI_LibInit",&Status);
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:2");
		return 0;
	}
	pRX_DATA =GetExternalModuleAddr(MudID1,"RX_DATA",&Status);		 //���ݽ���
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:3");
		return 0;
	}
	pTX1_SEND =GetExternalModuleAddr(MudID1,"TX1_SEND",&Status);    //����1
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:4");
		return 0;
	}
	pTX2_SEND =GetExternalModuleAddr(MudID1,"TX2_SEND",&Status);    //����2
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:5");
		return 0;
	}
	pTX1_INIT =GetExternalModuleAddr(MudID1,"TX1_INIT",&Status);      //����1
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:6");
		return 0;
	}
	pTX2_INIT =GetExternalModuleAddr(MudID1,"TX2_INIT",&Status);	 // ����2
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:7");
		return 0;
	}
	pRX_ENABLE =GetExternalModuleAddr(MudID1,"RX_ENABLE",&Status); 	   //��������
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:8");
		return 0;
	}
	pTX_ENABLE =GetExternalModuleAddr(MudID1,"TX_ENABLE",&Status);		//��������
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:9");
		return 0;
	}
	pDeviceClose =GetExternalModuleAddr(MudID1,"DeviceClose",&Status);    //�ر��豸
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:10");
		return 0;
	}
	pOBT429_RESET =GetExternalModuleAddr(MudID1,"OBT429_RESET",&Status);  //�豸��λ
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:11");
		return 0;
	}
	pRX1_INIT =GetExternalModuleAddr(MudID1,"RX1_INIT",&Status);		 //���ճ�ʼ��1
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:12");
		return 0;
	}
	pRX2_INIT =GetExternalModuleAddr(MudID1,"RX2_INIT",&Status);		  //���ճ�ʼ��2
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:13");
		return 0;
	}
	pRX3_INIT =GetExternalModuleAddr(MudID1,"RX3_INIT",&Status); 		 //���ճ�ʼ��3
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:14");
		return 0;
	}
	pRX4_INIT =GetExternalModuleAddr(MudID1,"RX4_INIT",&Status);    	//���ճ�ʼ��4
	if(Status<0)
	{
		MessagePopup("error","װ�ذ忨����ʱ����:15");
		return 0;
	}	
    return 1;
}


void OnRXInit( WDC_DEVICE_HANDLE hDevX ) 
{
	// TODO: Add your control notification handler code here
			  unsigned int enable=1;		    //RX1ʹ��(0, 1)
			  unsigned int word_length=0;		//RX1�ֳ� (0, 1)
			  unsigned int parity_select=0; 	//��żУ��(0,1,2)
			  unsigned int label_enable=0;	    //���У��(0,1)
			  unsigned int sdi=0; 		        //SDIУ��(0,1,2,3,4)
              unsigned int scaler=2;			//����������(0,1,2)
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
				  label1,label2,label3,label4,label5,label6,label7 );//RX1ͨ��Ӧ������
			  	  pRX2_INIT(hDevX,enable,word_length,parity_select,label_enable,sdi,scaler,
				  label1,label2,label3,label4,label5,label6,label7 );//RX2ͨ��Ӧ������		
				  //printf("1--%x\n",hDevX);  
			  }
              else
			  {
                  MessagePopup("error","���ȳ�ʼ���忨!");
			  }
 
}

int openObtDevice(int val)
{
    unsigned int enable=1;         //TX1ʹ��(0, 1)
    unsigned int word_length=0;    //TX1�ֳ� (0, 1)
    unsigned int parity=1;         //��żУ��(0,1,2)
    unsigned int scaler=2;         //(0,1,2)����������
    unsigned int tgap=40;           //�ּ���(0~255)
    unsigned int mode=1;           //����ģʽ(0,1);
    long  status;
	
	status=pOBT429TOPCI_LibInit();
	hDevX[val] = pDeviceFindAndOpen(val); 
	
	if(hDevX[val]!=0)
	{
	   pOBT429_RESET(hDevX[val]); 
	   pTX1_INIT(hDevX[val],enable,word_length,parity,scaler,tgap,mode); //TX1ͨ��Ӧ������
	   pTX2_INIT(hDevX[val],enable,word_length,parity,scaler,tgap,mode); //TX1ͨ��Ӧ������
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
