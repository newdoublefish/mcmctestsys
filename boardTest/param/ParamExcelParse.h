#ifndef __EXCEL_PARSE_H__
#define __EXCEL_PARSE_H__

#ifdef __cplusplus
    extern "C" {
#endif

BOOL ParamProtocolInit(char *sheetName); 

ListType getParamList(void);
		
#ifdef __cplusplus
    }
#endif
	
#endif 
