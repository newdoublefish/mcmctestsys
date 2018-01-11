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
#include"settingEntity.h"
#include "inifile.h"


static char *iniFilePath="\\config\\setting.ini";

HRESULT saveSetting(SETTING gSetting)
{
	HRESULT error=0;
	IniText g_myInifile = 0; 
	char inifileName[MAX_PATHNAME_LEN]; 
	
	
	GetProjectDir (inifileName);
	strcat(inifileName,iniFilePath);
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
	 
	 Ini_PutBoolean (g_myInifile,"Setting","ShowTipsWhenError",gSetting.showTipsWhenError);
	 Ini_PutBoolean (g_myInifile,"Setting","Voice",gSetting.voice);
	 Ini_PutBoolean (g_myInifile,"Setting","SaveToDb",gSetting.saveToDb); 
	 Ini_PutBoolean (g_myInifile,"Setting","LogSave",gSetting.saveLog); 
	 Ini_PutBoolean (g_myInifile,"Setting","SimuTest",gSetting.simuTest); 
     Ini_PutRawString (g_myInifile,"Setting","FileSaveDir",gSetting.saveDir);
	 Ini_PutInt (g_myInifile,"Setting","MaxComunPerRowInAutoTestPanel",gSetting.maxComunPerRowInAutoTestPanel);
	 //Ini_PutInt (g_myInifile,"Setting","MethodToGetdirFile ",gSetting.methodToGetdirFile); 
	 Ini_PutBoolean (g_myInifile,"Setting","CollectTestMention ",gSetting.collectTestMention);
	 Ini_PutBoolean (g_myInifile,"Setting","AutoSave ",gSetting.autoSave); 
	 Ini_PutBoolean (g_myInifile,"Setting","RelayEnable ",gSetting.relayEnable);    
	 Ini_PutInt (g_myInifile,"Setting","MentionAutoCloseTime",gSetting.mentionAutoCloseTime); 
	 Ini_PutInt (g_myInifile,"Setting","FrontSize",gSetting.frontSize);
	 Ini_PutInt (g_myInifile,"Setting","ReTestCnt",gSetting.reTestCnt);
	 Ini_PutBoolean (g_myInifile,"Setting","ReportInfoCustom",gSetting.reportInfoCustom);
	 Ini_WriteToFile (g_myInifile, inifileName);  
	  
                                
Error:        
      if (g_myInifile);
         Ini_Dispose (g_myInifile);

	
	return error;
	 
}


SETTING getSetting(void)
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
	SETTING gSetting;
	
	memset(&gSetting,0,sizeof(SETTING));
	
	
	GetProjectDir (inifileName);
	strcat(inifileName,iniFilePath);
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
		    for(int item=1;item<=items;item++)
		    {
		         Ini_NthItemName (g_myInifile, sectionName, item,&itemName);
				 if(itemName)
				 {
				     if(strcmp(itemName,"FileSaveDir")==0)
					 {
					    memset(gSetting.saveDir,strlen(gSetting.saveDir),0);
                        if (Ini_GetPointerToRawString (g_myInifile,
                                 sectionName, itemName, &stringValue)>0)
						{
						
							if(strcmp(stringValue,"Default")==0)
							{	
								 char temp[MAX_PATHNAME_LEN]={0};
								 
								 GetProjectDir(temp); //如果是默认值,防止在程序目录下
								 //sprintf(temp,"%s\\%s",temp,"Report");      
								 sprintf(gSetting.saveDir,"%s\\%s",temp,"Report");
								 
							}else
							{	
								
						       sprintf(gSetting.saveDir,"%s",stringValue);
							  /* int ret=MakeDir(gSetting.saveDir);
							   if(ret==0 || ret==-9)
							   {

								  
							   }else
							   {
							      char temp[MAX_PATHNAME_LEN]={0};     
							      GetProjectDir(temp);
								  memset(gSetting.saveDir,0,MAX_DIRNAME_LEN);
								  sprintf(gSetting.saveDir,"%s",temp); 
							   }*/
							   
							   int fileSize;
							   if(FileExists(stringValue,&fileSize)<=0)
							   {
							      char temp[MAX_PATHNAME_LEN]={0};     
							      GetProjectDir(temp);
								  memset(gSetting.saveDir,0,MAX_DIRNAME_LEN);
                                  sprintf(gSetting.saveDir,"%s\\%s",temp,"Report");   						   
							   }	   
	   
							} 
						}		 
							
							
						
					 }else if(strcmp(itemName,"ShowTipsWhenError")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							gSetting.showTipsWhenError=booleanValue;
						}	
					 
						 
					 }else if(strcmp(itemName,"Voice")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.voice=booleanValue;
						}	
					 
					 }else if(strcmp(itemName,"SaveToDb")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.saveToDb=booleanValue;
						}	
					 
					 }else if(strcmp(itemName,"LogSave")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.saveLog=booleanValue;
						}
					 }else if(strcmp(itemName,"SimuTest")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.simuTest=booleanValue;
						}						
					 }else if(strcmp(itemName,"MaxComunPerRowInAutoTestPanel")==0)
					 {
						int tempValue=0;
					    if(Ini_GetInt(g_myInifile,sectionName,itemName,&tempValue)>0)
						{
						     gSetting.maxComunPerRowInAutoTestPanel=tempValue;
						}else
						{
						     gSetting.maxComunPerRowInAutoTestPanel=2;
						}	
					     
					 }/*else if(strcmp(itemName,"MethodToGetdirFile")==0)
					 {
						int tempValue=0;
					    if(Ini_GetInt(g_myInifile,sectionName,itemName,&tempValue)>0)
						{
						     gSetting.methodToGetdirFile=tempValue;
						}else
						{
						     gSetting.methodToGetdirFile=0;
						}	
					     
					 }*/else if(strcmp(itemName,"CollectTestMention")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.collectTestMention=booleanValue;
						}else{
						     gSetting.collectTestMention=0;
						}
							
					 }else if(strcmp(itemName,"AutoSave")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.autoSave=booleanValue;
						}else{
						     gSetting.autoSave=0;
						}
							
					 }else if(strcmp(itemName,"RelayEnable")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.relayEnable=booleanValue;
						}else{
						     gSetting.relayEnable=0;
						}
							
					 }else if(strcmp(itemName,"MentionAutoCloseTime")==0)
					 {
                        if (Ini_GetInt(g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.mentionAutoCloseTime=booleanValue;
						}else{
						     gSetting.mentionAutoCloseTime=0;
						}
							
					 }else if(strcmp(itemName,"FrontSize")==0)
					 {
                        if (Ini_GetInt(g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.frontSize=booleanValue;
						}else{
						     gSetting.frontSize=0;
						}
							
					 }else if(strcmp(itemName,"ReTestCnt")==0)
					 {
                        if (Ini_GetInt(g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.reTestCnt=booleanValue;
						}else{
						     gSetting.reTestCnt=0;
						}
							
					 }else if(strcmp(itemName,"ReportInfoCustom")==0)
					 {
                        if (Ini_GetBoolean (g_myInifile, sectionName, itemName,
                                            &booleanValue) > 0)
						{
							 gSetting.reportInfoCustom=booleanValue;
						}else{
						     gSetting.reportInfoCustom=0;
						}
							
					 }
				 
				 }	 
		  
		    }
		  }	
	 
	 }	 
	 
Error:        
      if (g_myInifile);
         Ini_Dispose (g_myInifile);

	
	return gSetting;
}


