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
#include "meter.h"
#include "UartUtil.h"

static unsigned char bcd_decimal(unsigned char bcd)
{
	return bcd-(bcd >> 4)*6;
}

static unsigned char decimal_bcd(unsigned char decimal)
{
	return (unsigned char) (decimal+(decimal / 10)*6);
}

unsigned char CalcMeterCheckSum(unsigned char *buffer,int len)
{
	unsigned char sum = 0;
	for(int i=0;i<len;i++)
	{
		sum += buffer[i];
	}
	return sum;
}

void buildMeterBuffer(tUartRx *meterPtr,char *address,unsigned char *data,int dataLen)
{
	meterPtr->rxBuffer[meterPtr->rxLen++]=0x68;
	if(strcmp(address,"AAAAAAAAAAAA")==0)
	{
		for(int i=0;i<6;i++)
		{
			meterPtr->rxBuffer[meterPtr->rxLen++] = 0xAA;				
		}		
	}else{
		for(int i=0;i<6;i++)
		{
			meterPtr->rxBuffer[meterPtr->rxLen++] = decimal_bcd(10*(address[12-i*2-2]-'0')+address[12-i*2-1]-'0');				
		}
	}
	meterPtr->rxBuffer[meterPtr->rxLen++] = 0x68;
	meterPtr->rxBuffer[meterPtr->rxLen++] = 0x11;
	meterPtr->rxBuffer[meterPtr->rxLen++] = 0x04;
	//memcpy(&(meterPtr->rxBuffer[meterPtr->rxLen]),data,dataLen);
	//meterPtr->rxLen +=4;
	for(int i=0;i<dataLen;i++)
	{
		meterPtr->rxBuffer[meterPtr->rxLen++] = data[i];
	}
	unsigned char sum = CalcMeterCheckSum(meterPtr->rxBuffer,meterPtr->rxLen);
	meterPtr->rxBuffer[meterPtr->rxLen++] = sum;
	meterPtr->rxBuffer[meterPtr->rxLen++] = 0x16; 
}



BOOL getMeterDlt2007Voltage(RSCONFIG config,char *address,float *voltage)
{
	unsigned char volData[4]={0x33,0x34,0x34,0x35};	
	
	tUartRx *t=createUartRx(config,100,1);
	if(t!=NULL)
	{
		buildMeterBuffer(t,address,volData,4);
		if(doUartRxProcess(t)==TRUE)
		{
			if(memcmp(volData,&t->rxBuffer[10],4)==0)
			{
				unsigned char low = t->rxBuffer[14] - 0x33;
				unsigned char high = t->rxBuffer[15] - 0x33;
				if(voltage!=0)
				{
					*voltage = (float)(bcd_decimal(high)*100 + bcd_decimal(low));
				}
			}else{
				goto ERROR;
			}
			releaseUartRx(t);
			return TRUE;			
		}
		
	}
ERROR:	
	if(t!=NULL)
		releaseUartRx(t); 
	return FALSE;
}
BOOL getMeterDlt2007Current(RSCONFIG config,char *address,float *voltage)
{
	unsigned char volData[4]={0x33,0x34,0x35,0x35};	
	
	tUartRx *t=createUartRx(config,100,1);
	if(t!=NULL)
	{
		buildMeterBuffer(t,address,volData,4);
		if(doUartRxProcess(t)==TRUE)
		{
			if(memcmp(volData,&t->rxBuffer[10],4)==0)
			{
				unsigned char low = t->rxBuffer[14] - 0x33;
				unsigned char high = t->rxBuffer[15] - 0x33;
				if(voltage!=0)
				{
					*voltage = (float)(bcd_decimal(high)*100 + bcd_decimal(low));
				}
			}else{
				goto ERROR;
			}
			releaseUartRx(t);
			return TRUE;			
		}
		
	}
ERROR:	
	if(t!=NULL)
		releaseUartRx(t); 
	return FALSE;
}
