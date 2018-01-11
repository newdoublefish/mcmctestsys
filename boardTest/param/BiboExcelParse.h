#ifndef __BIBO_EXCEL_PARSE_H__
#define __BIBO_EXCEL_PARSE_H__

#ifdef __cplusplus
    extern "C" {
#endif
typedef struct{
	char paramName[50];
	unsigned int maskBi;
	unsigned int maskBo;
}tBIBO;

BOOL BiboProtocolInit(char *sheetName); 

BOOL getBibo(char *paramName,tBIBO *bibo); 
		
#ifdef __cplusplus
    }
#endif
	
#endif 
