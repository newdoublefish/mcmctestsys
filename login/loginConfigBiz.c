/*---------------------------------------------------------------------------
 // 版权声明：本文件由广东万城万充电动车运营股份有限公司版权所有，未经授权，
 // 禁止第三方进行拷贝和使用。
 //
 // 文件名：
 // 文件功能描述: 
 //
 // 
 // 创建标识：曾衍仁 
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
#include "inifile.h"
#include "loginConfigBiz.h"


char *loginConfigIniFilePath="\\config\\setting.ini";

HRESULT saveLoginConfig(tLoginConfig s)
{
	HRESULT error=0;
	IniText g_myInifile = 0; 
	char inifileName[MAX_PATHNAME_LEN]; 
	
	
	GetProjectDir (inifileName);
	strcat(inifileName,loginConfigIniFilePath);
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
	 
	 //Ini_PutInt(g_myInifile,"Hardware","arinc429Card",gHardware.arinc429card);
	 Ini_PutBoolean (g_myInifile,"LoginConfig","remember",s.remember);  
	 Ini_PutRawString (g_myInifile,"LoginConfig","username",s.userName); 
	 Ini_PutRawString (g_myInifile,"LoginConfig","password",s.password); 

	 Ini_WriteToFile (g_myInifile, inifileName);  
                                
Error:        
      if (g_myInifile);
         Ini_Dispose (g_myInifile);

	
	return error;
	 
}


tLoginConfig getLoginConfig()
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
	tLoginConfig config={0};
	GetProjectDir (inifileName);
	strcat(inifileName,loginConfigIniFilePath);
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
			if(strcmp(sectionName,"LoginConfig")!=0)
				continue;			  
		    for(int item=1;item<=items;item++)
		    {
		         Ini_NthItemName (g_myInifile, sectionName, item,&itemName);
				 if(itemName)
				 {
                     if(strcmp(itemName,"remember")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &integerValue) > 0)
						{
							 config.remember=integerValue;
						}	
					 }else if(strcmp(itemName,"username")==0){
					 
                        if (Ini_GetPointerToRawString (g_myInifile,
                                 sectionName, itemName, &stringValue)>0)
						{
							sprintf(config.userName,"%s",stringValue);							
						}					 
					 }else if(strcmp(itemName,"password")==0){
                        if (Ini_GetPointerToRawString (g_myInifile,
                                 sectionName, itemName, &stringValue)>0)
						{
							sprintf(config.password,"%s",stringValue);							
						}						 
					 }
				 }	 
		    }
		  }	
	 
	 }	 
	 
Error:        
      if (g_myInifile);
         Ini_Dispose (g_myInifile);

	return config;
}

