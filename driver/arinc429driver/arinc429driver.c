#include "obtArinc429Drv.h"
#include "arinc429driver.h"
#include "hardwareConfig.h"
HRESULT load429driver(void)
{
	 HRESULT ret=FALSE; 
	 HARDWARE h=GetHardware();
     if(h.arinc429card==0)  //obt
	 {
	    if(Load429DRV(0)==0)	 //注意重复加载
		{
		   return ret;
		}	
		/*if(openObtDevice(0)==0)
		{
		   return ret;
		}*/
		ret=TRUE;
		//send429Data=sendObdArinc429Data;
	 }else   //神州飞航
	 {
	 
	 }
	 return ret;
}

HRESULT unLoad429driver(void)
{
	 HRESULT ret=FALSE;  
	 HARDWARE h=GetHardware();
     if(h.arinc429card==0)  //obt
	 {
		closeObtDevice();
	 }else   //神州飞航
	 {
	 
	 }
	 return ret;
}



