#include <utility.h>
#include <ansi_c.h>
#include "inifile.h"
#include <userint.h>
#include "application.h"



typedef int (*APPLICATION_LOAD_FUNC)(IniText iniText,char *sectionName,char *itemName,void *data);
typedef int (*APPLICATION_STORE_FUNC)(IniText iniText,char *sectionName,char *itemName,void *data); 

typedef struct{
	char name[50];
	APPLICATION_LOAD_FUNC loadFunc;
	APPLICATION_STORE_FUNC storeFunc;
}tAppItem;

int onBasicLoad(IniText iniText,char *sectionName,char *itemName,void *data);
//int onBasicStore(IniText iniText,char *sectionName,char *itemName,void *data);

static tAppItem appItems[]=
{
	{"Basic",onBasicLoad,NULL},
};

tApplication gApplication;

char *applicationIniFilePath="\\config\\setting.ini";   


tApplication getApplication()
{
	return gApplication;
}

int ApplicationInit()
{
	HRESULT error=0;
	int sections;
	int items;
    int    booleanValue;
    int    integerValue;
    char   *sectionName = NULL;
    char   *itemName = NULL;
    char   *stringValue;
    double doubleValue;
	IniText g_myInifile = 0; 
	char inifileName[MAX_PATHNAME_LEN]; 
	//tLoginConfig config={0};
	GetProjectDir (inifileName);
	strcat(inifileName,applicationIniFilePath);
    if (g_myInifile){
        Ini_Dispose (g_myInifile);
         g_myInifile = 0;
     }    
    if (!(g_myInifile = Ini_New (0)))
    {
        MessagePopup("Inifile","Error allocating memory for Inifile");
		error=-1;
        goto Error;
     }   
     if (Ini_ReadFromFile (g_myInifile, inifileName))
     {
         MessagePopup("Inifile","Error reading Inifile");
		 error=-1; 
         goto Error;
     } 
	 
	 sections = Ini_NumberOfSections (g_myInifile);
	 
	 for(int section=1;section<=sections;section++)
	 {
	      Ini_NthSectionName (g_myInifile, section, &sectionName);
		  items  = Ini_NumberOfItems (g_myInifile, sectionName);
		  if(sectionName)
		  {	
			for(int i=0;i<sizeof(appItems)/sizeof(tAppItem);i++)
			{
				if(strcmp(sectionName,appItems[i].name)==0)
				{
					//TODO:load ini
					for(int item=1;item<=items;item++)
					{
						Ini_NthItemName (g_myInifile, sectionName, item,&itemName);
						appItems[i].loadFunc(g_myInifile,sectionName,itemName,&gApplication);
					}
				}
			}
		  }	
	 
	 }	 
	 
Error:        
      if (g_myInifile);
         Ini_Dispose (g_myInifile);

	return 1;	
}


int onBasicLoad(IniText iniText,char *sectionName,char *itemName,void *data)
{
	tApplication *app = (tApplication *)data;
	char   *stringValue;
	if(strcmp(itemName,"ApplicationName")==0)
	{
        if (Ini_GetPointerToRawString (iniText,sectionName, itemName, &stringValue)>0)
		{
			app->basic.applicationName = (char *)malloc(strlen(stringValue)+1);
			sprintf(app->basic.applicationName,"%s",stringValue);
		}		
	}else if(strcmp(itemName,"Company")==0)
	{
        if (Ini_GetPointerToRawString (iniText,sectionName, itemName, &stringValue)>0)
		{
			app->basic.company = (char *)malloc(strlen(stringValue)+1);
			sprintf(app->basic.company,"%s",stringValue);
		}		
	}else if(strcmp(itemName,"Version")==0)
	{
        if (Ini_GetPointerToRawString (iniText,sectionName, itemName, &stringValue)>0)
		{
			app->basic.version = (char *)malloc(strlen(stringValue)+1);
			sprintf(app->basic.version,"%s",stringValue);
		}		
	}else if(strcmp(itemName,"CopyRight")==0)
	{
        if (Ini_GetPointerToRawString (iniText,sectionName, itemName, &stringValue)>0)
		{
			app->basic.copyRight = (char *)malloc(strlen(stringValue)+1);
			sprintf(app->basic.copyRight,"%s",stringValue);
		}		
	}else if(strcmp(itemName,"LOGO")==0)
	{
        if (Ini_GetPointerToRawString (iniText,sectionName, itemName, &stringValue)>0)
		{
			app->basic.logo = (char *)malloc(strlen(stringValue)+1);
			sprintf(app->basic.logo,"%s",stringValue);
		}		
	}
	return 1;
}
