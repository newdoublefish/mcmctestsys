#include <ansi_c.h>
#include "ParamRuleParse.h"
#include "StubNetService.h"
#include "regexpr.h"   
//#define DEBUG


int ConfigParameter(tNET_SERVICE *servicePtr,PARAMETER para)
{
	char cmd[50]={0};
	sprintf(cmd,"g %d\r\n",para.group);
	startCommand(servicePtr,cmd);
	memset(cmd,0,50);
	sprintf(cmd,"i %d\r\n",para.item);
	startCommand(servicePtr,cmd);
	memset(cmd,0,50);
	sprintf(cmd,"v %d %d %d %s\r\n",para.gunIndex,para.array,para.element,para.value);
	startCommand(servicePtr,cmd);
	return 0;
}

//[0-9] [0-9]* [0-9]*.*
int SearchAndParse(char *str,PARAMETER *para){
	int matched,position,matchedLen; 
	char cmd[50]={0};
	sprintf(cmd,"%d %d %d.*",para->gunIndex,para->array,para->element);
	RegExpr_FindPatternInText(cmd,0,str,strlen(str),1,1,&matched,&position,&matchedLen);
	if(matched)
	{	
		char temp[128]={0};
	 	memcpy(temp,str+position,matchedLen);
#ifdef DEBUG		
		printf("patern:%s\n,matched:%s\n",temp);
#endif		
		char delims[] = " ";
  		char *result = NULL;
   		result = strtok( temp, delims );
		int index=0;
   		while( result != NULL ) {
      	 	
			if(3==index) 
			{
				//printf( "result is \"%s\"\n", result );
				sprintf(para->key,"%s",result);
			}else if(4 == index){
				sprintf(para->value,"%s",result);
			}else if(5==index)
			{
				sprintf(para->value,"%s %s",para->value,result);
			}
       	 	result = strtok( NULL, delims );
			index++;
			
			
 		}
		return 0;  

	}
	return -1;
	

}

int GetParameter(tNET_SERVICE *servicePtr,PARAMETER *para)
{
	char cmd[50]={0};
	sprintf(cmd,"g %d\r\n",para->group);
	if(startCommand(servicePtr,cmd)<0)
		return -1;
	memset(cmd,0,50);
	sprintf(cmd,"i %d\r\n",para->item);
	if(startCommand(servicePtr,cmd)<0)
		return -1;
	if(startCommand(servicePtr,"v\r\n")<0)
		return -1;
	//ÕÒµ½Õâ¶ù×Ö·û´®
	if(SearchAndParse(servicePtr->packet,para)<0)
		return -1;
	return 0;	
}

void printfParam(PARAMETER para){

	printf("%s,g:%d,i:%d:%d,%d,%d,key:%s,value:%s\n",para.paramName,para.group,para.item,para.gunIndex,para.array,para.element,para.key,para.value);
}
