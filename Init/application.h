#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#ifdef __cplusplus
    extern "C" {
#endif

typedef struct{
	char *applicationName;
	char *company;
	char *version;
	char *copyRight;
	char *logo;
	int configEnable;
}tAppBasic;		
		
typedef struct{
	tAppBasic basic;
}tApplication;

int ApplicationInit();
tApplication getApplication();

#ifdef __cplusplus
    }
#endif


#endif /* ndef __APPINIT_H__ */  
