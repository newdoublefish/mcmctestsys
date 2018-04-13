#ifndef __CONDITION_MODBUS_H__
#define __CONDITION_MODBUS_H__

#include "cvidef.h" 

#ifdef __cplusplus
    extern "C" {
#endif

//#include "relayHelper.h"
#include "eutEntity.h" 
		
typedef struct{
	unsigned short address;
	short value;
}tConditonItem;
BOOL ConditionGetItem(RSCONFIG config,tConditonItem *item);
BOOL ConditionSetItem(RSCONFIG config,tConditonItem *itemPtr);
#ifdef __cplusplus
    }
#endif


#endif /* ndef __APPINIT_H__ */  
