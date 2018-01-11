#ifndef __RELAYHELPER_H__
#define __RELAYHELPER_H__

#include "cvidef.h" 

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct
{
    int portNum;
    int baudRate;
    int parity;
    int dataBit;
    int stopBit;
}RSCONFIG;

typedef struct
{
    int len;
	unsigned char data[1024];
}RelayBuf;

BOOL OpenDo(RSCONFIG config,int doMask);
BOOL CloseDo(RSCONFIG config,int doMask);

BOOL OperatDoSet(RSCONFIG config,unsigned int mask);


#ifdef __cplusplus
    }
#endif


#endif /* ndef __APPINIT_H__ */  


