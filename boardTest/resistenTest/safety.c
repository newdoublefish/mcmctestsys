#include <rs232.h>
#include "toolbox.h"
#include "safety.h"
#include "log.h"
BOOL buildSafetyBuffer(tSafetyBuffer *tBuffer,char *type,double volatage,double upper,double lower,double rtime,double ttime,double ftime,double arc,double freq)
{
	//头部
	//if(strcmp(type,"IR")==0)
	tBuffer->buffer[tBuffer->len++]=0x11;
	tBuffer->buffer[tBuffer->len++]=0x08;
	//长度
	tBuffer->buffer[tBuffer->len++]=0x15;
	//基本设置
	tBuffer->buffer[tBuffer->len++]=0x02;
	//命令设置
	//w_ma 设置
	unsigned char w_ma=0;
	if(strcmp(type,"IR")==0)
	{
		w_ma |=(1<<6);				
	}else if(strcmp(type,"DCW")==0){
		
		w_ma |=(1<<5);	
	}else if(strcmp(type,"ACW")==0){ 
	
	}else{
		return FALSE;
	}
	
	if(freq==50)
	{
	    
	}else if(freq==60){
	   w_ma |=(1<<4);
	}else{
		if(strcmp(type,"ACW")==0) 
			return FALSE;
	}
	
	
	w_ma |=((int)(arc)&0x0f);
	tBuffer->buffer[tBuffer->len++]=w_ma; 	
	//设置电压
	tBuffer->buffer[tBuffer->len++]=(int)(volatage*100) & 0xff;
	
	tBuffer->buffer[tBuffer->len++]=((int)(volatage*100)>>8)&0xff;
	tBuffer->buffer[tBuffer->len++]=(int)(volatage*100) & 0xff; 

	//设置电流上限
	tBuffer->buffer[tBuffer->len++]=((int)(upper*100)>>8)&0xff; 
	tBuffer->buffer[tBuffer->len++]=(int)(upper*100) & 0xff;
	
	//设置电流下限                      	
	tBuffer->buffer[tBuffer->len++]=((int)(lower*100)>>8)&0xff;		
	tBuffer->buffer[tBuffer->len++]=(int)(lower*100) & 0xff;  
	
	//设置缓升时间                      	
	tBuffer->buffer[tBuffer->len++]=((int)(rtime*10)>>8)&0xff;		
	tBuffer->buffer[tBuffer->len++]=(int)(rtime*10) & 0xff;  
	
	//设置测试时间                      	
	tBuffer->buffer[tBuffer->len++]=((int)(ttime*10)>>8)&0xff;	
	tBuffer->buffer[tBuffer->len++]=(int)ttime*10 & 0xff;  
	
	//设置绝缘电阻上限                      	
	tBuffer->buffer[tBuffer->len++]=((int)upper>>8)&0xff;	
	tBuffer->buffer[tBuffer->len++]=(int)upper & 0xff;  
	                                    
	//设置绝缘电阻下限                      
	tBuffer->buffer[tBuffer->len++]=((int)lower>>8)&0xff;
	tBuffer->buffer[tBuffer->len++]=(int)lower & 0xff;  
	
	//延时判断时间                      
	tBuffer->buffer[tBuffer->len++]=((int)(ttime*10)>>8)&0xff;
	tBuffer->buffer[tBuffer->len++]=(int)(ttime*10) & 0xff;  
	
	//缓降时间                     
	tBuffer->buffer[tBuffer->len++]=((int)(ftime*10)>>8)&0xff;
	tBuffer->buffer[tBuffer->len++]=(int)(ftime*10) & 0xff;  
	return TRUE;
}

BOOL buildSafetyBufferWithScpiCmd(tSafetyBuffer *tBuffer,tSCPICMD cmd){
	return buildSafetyBuffer(tBuffer,cmd.type,cmd.volt,cmd.upper,cmd.lower,cmd.rtim,cmd.ttim,cmd.ftim,cmd.arc,cmd.freq);	
}

//0x11,0x08,0x02,0x00,0x52
BOOL sendToJinKo(int port,tSafetyBuffer cmd)
{
	char logBuffer[100]={0};
	for(int i=0;i<cmd.len;i++)
	{
		//printf("%02x ",cmd.buffer[i]);
		sprintf(logBuffer,"%s %02x",logBuffer,cmd.buffer[i]);
	}
	LOG_EVENT_FORMAT(LOG_INFO,"send %s\n",logBuffer);
	//printf("\n");
	int recvLen = ComWrt (port,cmd.buffer,cmd.len);
	if(recvLen<0 | recvLen!=cmd.len)
		return FALSE;
	return TRUE;	
}

BOOL JinkoReset(int comPort)
{
	tSafetyBuffer cmd={0};
	cmd.buffer[cmd.len++]=0x11;
	cmd.buffer[cmd.len++]=0x08;
	cmd.buffer[cmd.len++]=0x02;
	cmd.buffer[cmd.len++]=0x00;
	cmd.buffer[cmd.len++]=0x52;
	return sendToJinKo(comPort,cmd);
}
//0x11,0x08,0x02,0x01,0x02
BOOL SetJinkoChannel(int comPort,int channel)
{
	tSafetyBuffer cmd={0};
	cmd.buffer[cmd.len++]=0x11;
	cmd.buffer[cmd.len++]=0x08;
	cmd.buffer[cmd.len++]=0x02;
	cmd.buffer[cmd.len++]=0x01;
	cmd.buffer[cmd.len++]=channel&0xff;
	return sendToJinKo(comPort,cmd);   
}
BOOL SetJinkoParam(int comPort,tSCPICMD cmd){
	tSafetyBuffer buffer={0};
	buildSafetyBufferWithScpiCmd(&buffer,cmd);
	return sendToJinKo(comPort,buffer);   	
}
//0x11,0x08,0x02,0x00,0x54
BOOL StartJinkoTest(int comPort){
	tSafetyBuffer cmd={0};
	cmd.buffer[cmd.len++]=0x11;
	cmd.buffer[cmd.len++]=0x08;
	cmd.buffer[cmd.len++]=0x02;
	cmd.buffer[cmd.len++]=0x00;
	cmd.buffer[cmd.len++]=0x54;
	return sendToJinKo(comPort,cmd);   	
}

BOOL parseJinkoResult(char *resultBuffer,char *result)
{

	BOOL ret=FALSE;
	if(resultBuffer[2]==0x00 || resultBuffer[2]==0x06)
	{
		ret= TRUE;
	}
	
	int tempResult = resultBuffer[6]<<24 | resultBuffer[7]<<16 | resultBuffer[8]<<8 | resultBuffer[9]&0xff; 
	//printf("%x\n",resultBuffer[3]);
	if((resultBuffer[3] & 0xC0) ==0){
		sprintf(result,"%d mA",tempResult*100);
	}else if((resultBuffer[3] & 0xC0) >0){
		sprintf(result,"%d MΩ",tempResult);	  	
	}
		
	return ret;	
}
