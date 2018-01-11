#ifndef __EXCEL_PARSE_H__
#define __EXCEL_PARSE_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "ParamRuleParse.h"

BOOL ParamProtocolInit(char *sheetName); 

//ListType getParamList(void);

BOOL getParameter(char *paramName,PARAMETER *param);
		
#ifdef __cplusplus
    }
#endif
	
#endif 
