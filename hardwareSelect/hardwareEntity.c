#include "inifile.h"
#include "hardwareEntity.h"


char *hardwareIniFilePath="\\config\\hardware.ini";

HRESULT saveHardware(HARDWARE gHardware)
{
	HRESULT error=0;
	IniText g_myInifile = 0; 
	char inifileName[MAX_PATHNAME_LEN]; 
	
	
	GetProjectDir (inifileName);
	strcat(inifileName,hardwareIniFilePath);
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
	 
	 Ini_PutInt(g_myInifile,"Hardware","arinc429Card",gHardware.arinc429card);
	 Ini_WriteToFile (g_myInifile, inifileName);  
                                
Error:        
      if (g_myInifile);
         Ini_Dispose (g_myInifile);

	
	return error;
	 
}


HARDWARE getHardware(void)
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
	HARDWARE gHardware;
	
	memset(&gHardware,0,sizeof(HARDWARE));
	
	
	GetProjectDir (inifileName);
	strcat(inifileName,hardwareIniFilePath);
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
                     if(strcmp(itemName,"arinc429Card")==0)
					 {
                        if (Ini_GetInt (g_myInifile, sectionName, itemName,
                                            &integerValue) > 0)
						{
							 gHardware.arinc429card=integerValue;
						}	
					 }	 
				 }	 
		  
		    }
		  }	
	 
	 }	 
	 
Error:        
      if (g_myInifile);
         Ini_Dispose (g_myInifile);

	return gHardware;
}

