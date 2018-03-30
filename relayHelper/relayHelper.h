#ifndef __RELAYHELPER_H__
#define __RELAYHELPER_H__

#include "cvidef.h" 

#ifdef __cplusplus
    extern "C" {
#endif
#include "eutEntity.h"

//#define RELAY_1 (1<<0)
//#define RELAY_2 (1<<0) 
#define RELAY(x) (1<<(x-1)) 
		
#define MASK32 0xFFFFFFFF
#define MASK16 0xFFFF
		
typedef struct
{
    int len;
	unsigned char data[1024];
}RelayBuf;

BOOL OpenDo(RSCONFIG config,int doMask);
BOOL CloseDo(RSCONFIG config,int doMask);

//BOOL OperatDoSet(RSCONFIG config,unsigned int mask);
BOOL OperatDoSet(RSCONFIG config,unsigned int doMask,unsigned int mask);
BOOL CheckDoCheckOptoCoupler(RSCONFIG config,unsigned int *value,unsigned int mask);


#ifdef __cplusplus
    }
#endif


#endif /* ndef __APPINIT_H__ */  


