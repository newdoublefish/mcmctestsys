#ifndef __BMSHELPER_H__
#define __BMSHELPER_H__

#include "cvidef.h" 

#ifdef __cplusplus
    extern "C" {
#endif

//#include "relayHelper.h"
#include "eutEntity.h" 
		
typedef struct{
	unsigned short address;
	short value;
}tBmsItem;
BOOL BmsSetItem(RSCONFIG config,tBmsItem item);
BOOL BmsGetItem(RSCONFIG config,tBmsItem *item);
#ifdef __cplusplus
    }
#endif


#endif /* ndef __APPINIT_H__ */  
