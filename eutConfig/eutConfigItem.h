#ifndef __EUT_CONFIG_ITEM_H__
#define __EUT_CONFIG_ITEM_H__
#include <stdlib.h> 
#include "toolbox.h"
#include "eutEntity.h"
#ifdef __cplusplus
   extern "C" {
#endif
#define MAX_EUT_CONGI_ITEM_LEN 50
typedef void (*EUT_CONOFIG_CREATE_PANEL)(int *);
typedef void (*EUT_CONFIG_SAVE_TO_XML)();
typdef  void (*EUT_CONFIG_READ_FROM_XML)(char *name,char *value);

typedef struct{
	char name[MAX_EUT_CONGI_ITEM_LEN];
	int  configHandle;
	int  panelHandle;
	EUT_CONOFIG_CREATE_PANEL onCreatePanel;
	EUT_CONFIG_SAVE_TO_XML onSave;
	EUT_CONFIG_READ_FROM_XML onRead;
	
	
	
}EUT_CONFIG_ITEM;
#ifdef __cplusplus
   }
#endif

#endif
