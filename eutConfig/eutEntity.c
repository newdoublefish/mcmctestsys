/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ㶫������綯����Ӫ�ɷ����޹�˾��Ȩ���У�δ����Ȩ��
 // ��ֹ���������п�����ʹ�á�
 //
 // �ļ�����
 // �ļ���������: 
 //
 // 
 // ������ʶ�������� 
 //
 // �޸ı�ʶ��
 // �޸�������
 //-------------------------------------------------------------------------*/
#include "cvixml.h"
#include <cvirte.h>		
#include <userint.h>
#include <stdlib.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "eutEntity.h"
#include "resultUtil.h"

static char *deviceFile="\\config\\Device.xml";   

static EUT tempDevice;

#ifndef xmlChk
#define xmlChk(f) if (xmlErr = (f), FAILED (xmlErr)) goto Error; else
#endif

/** @brief ����XML�е�UUT��Ϣ
 *  @param[in]  ��
 *  @param[out] ��
 *  @return     int
 *  @note       �޶���¼  :
*/
static EUT xmlToEut(CVIXMLElement currElem)
{
	//Device device;
	int len,numChildElems,index;
	CVIXMLElement       currChildElem = 0;
    char  *elemName = NULL, *elemValue = NULL;	
	CVIXMLGetNumChildElements (currElem, &numChildElems);     
	//memset(&device,0,sizeof(device));
	for(index=0;index<numChildElems;index++)
	{
	   CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
	   CVIXMLGetElementTagLength (currChildElem, &len);
	   elemName = malloc (len + 1);
	   CVIXMLGetElementTag (currChildElem, elemName);
	   CVIXMLGetElementValueLength (currChildElem, &len);
	   elemValue = malloc (len + 1);
	   CVIXMLGetElementValue (currChildElem, elemValue);
	   if(strcmp(elemName,"DeviceId")==0)
	   {
	      tempDevice.index=atoi(elemValue);
	   }else if(strcmp(elemName,"DeviceName")==0)
	   {
		   //tempDevice.eutName=(char*)malloc(len+1); 
		   memset(tempDevice.eutName,0,MAX_EUTNAME_LEN);
	       strcpy(tempDevice.eutName,elemValue);
	   }else if(strcmp(elemName,"Enable")==0)
	   {
	   
		  tempDevice.enable=atoi(elemValue);  
	   }else if(strcmp(elemName,"PortNum")==0)
	   {
	       tempDevice.matainConfig.portNum=atoi(elemValue);
	   }else if(strcmp(elemName,"BandRate")==0)
	   {
	        tempDevice.matainConfig.baudRate=atoi(elemValue);  
	   }else if(strcmp(elemName,"Parity")==0)
	   {
	       tempDevice.matainConfig.parity=atoi(elemValue);
	   }else if(strcmp(elemName,"DataBit")==0)
	   {
	       tempDevice.matainConfig.dataBit=atoi(elemValue);
	   }else if(strcmp(elemName,"StopBit")==0)
	   {
	       tempDevice.matainConfig.stopBit=atoi(elemValue);
	   }else if(strcmp(elemName,"RSR422ComPortNum")==0)
	   {
	       tempDevice.RS422Config.portNum=atoi(elemValue);
	   }else if(strcmp(elemName,"RS422ComBandRate")==0)
	   {
	        tempDevice.RS422Config.baudRate=atoi(elemValue);  
	   }else if(strcmp(elemName,"RS422ComParity")==0)
	   {
	       tempDevice.RS422Config.parity=atoi(elemValue);
	   }else if(strcmp(elemName,"RS422ComDataBit")==0)
	   {
	       tempDevice.RS422Config.dataBit=atoi(elemValue);
	   }else if(strcmp(elemName,"RS422ComtopBit")==0)
	   {
	       tempDevice.RS422Config.stopBit=atoi(elemValue);
	   }else if(strcmp(elemName,"ARINC429Port")==0) {
	       tempDevice.arinc429Config.ch=atoi(elemValue);
       }else if(strcmp(elemName,"ARINC429Card")==0) {
	       tempDevice.arinc429Config.card=atoi(elemValue);
	   }
	   
        free (elemName);
        free (elemValue);
        elemName = elemValue = NULL;
        /* NOTE: Recursion! Make sure stack size can handle input data */
        xmlToEut (currChildElem);
        CVIXMLDiscardElement (currChildElem);
	}
	return tempDevice;
}

/** @brief ��xml�ļ��ж�ȡ��Ϣ�����ұ����ΪUUT����
 *  @param[in]  ��
 *  @param[out] ��
 *  @return     int
 *  @note       �޶���¼  :
*/

ListType getEutListFromXmlFile(void)
{
	char fileName[MAX_PATHNAME_LEN];
	ListType deviceList=0;
	EUT temp;
	int len,numChildElems,index ;
	CVIXMLDocument      document = 0;
    CVIXMLElement       currElem = 0;
    CVIXMLAttribute     currAttr = 0;
    CVIXMLElement       currChildElem = 0;
    char  *elemName = NULL, *elemValue = NULL;	
	deviceList=ListCreate (sizeof(EUT));
	
	GetProjectDir (fileName);
	strcat(fileName,deviceFile);
    CVIXMLLoadDocument (fileName, &document);
    CVIXMLGetRootElement (document, &currElem);
	CVIXMLGetNumChildElements (currElem, &numChildElems);
		
	for(index=0;index<numChildElems;index++)
	{
	   CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
	   CVIXMLGetElementTagLength (currChildElem, &len);
	   elemName = malloc (len + 1);
	   CVIXMLGetElementTag (currChildElem, elemName);
	   CVIXMLGetElementValueLength (currChildElem, &len);
	   elemValue = malloc (len + 1);
	   CVIXMLGetElementValue (currChildElem, elemValue);
	   temp=xmlToEut(currChildElem);
	   ListInsertItem(deviceList,&temp,END_OF_LIST);
	   free (elemName);
       free (elemValue);
	}
	
    CVIXMLDiscardElement (currElem);
    CVIXMLDiscardDocument (document);
	
	return deviceList;

}


/** @brief XMLɾ��UUT
 *  @param[in]  ��
 *  @param[out] ��
 *  @return     int
 *  @note       �޶���¼  :
*/

static int removeEutElement(CVIXMLElement element)
{
    int                 error = 0;
    CVIXMLStatus        xmlErr = S_OK;
    int                 index, numAttrs, numChildElems, childItemIndex, len;
    CVIXMLAttribute     currAttr = 0;
    CVIXMLElement       currChildElem = 0;
    char                *elemName = NULL, *elemValue = NULL;
	char count;

    /* Process child elements */
   CVIXMLGetNumChildElements (element, &numChildElems);
   for (index = 0; index < numChildElems; ++index) {
        xmlChk (CVIXMLGetChildElementByIndex (element, index, &currChildElem));
        xmlChk (CVIXMLGetElementTagLength (currChildElem, &len));
        nullChk (elemName = malloc (len + 1));
        xmlChk (CVIXMLGetElementTag (currChildElem, elemName));
        xmlChk (CVIXMLGetElementValueLength (currChildElem, &len));
        nullChk (elemValue = malloc (len + 1));
        xmlChk (CVIXMLGetElementValue (currChildElem, elemValue));
        RemoveSurroundingWhiteSpace (elemValue);

		if(strcmp(elemName,"Device")==0)
		{
			while(numChildElems>0)
			{
			  xmlChk (CVIXMLGetChildElementByIndex (element, index, &currChildElem));	
		      CVIXMLRemoveElement(currChildElem); 
			  index=0;
			  xmlChk (CVIXMLGetNumChildElements (element, &numChildElems));
			}
			
			break;
			
		}else{

            removeEutElement (currChildElem);
		}
		free (elemName);
        free (elemValue);
        elemName = elemValue = NULL;
		
        CVIXMLDiscardElement (currChildElem);
        currChildElem = 0;
		
    }
	


Error:
    free (elemName);
    free (elemValue);
    CVIXMLDiscardAttribute (currAttr);
    CVIXMLDiscardElement (currChildElem);
    if (FAILED (xmlErr)) {
        char    errBuf[512];
        CVIXMLGetErrorString (xmlErr, errBuf, sizeof (errBuf));
        MessagePopup ("XML Error", errBuf);
    }
		   
	return error;
}

/** @brief ��Eut��Ϣת��xml��Ϣ
 *  @param[in]  ��
 *  @param[out] ��
 *  @return     int
 *  @note       �޶���¼  :
*/

static void eutToXml(ListType eutList,CVIXMLElement element)
{
	CVIXMLElement currElem;
	CVIXMLElement currChildElem; 
	CVIXMLElement  currSuperChildElem=0;  
	char temp[100]={0};
	EUT device;
	for(int deviceIndex=1;deviceIndex<=ListNumItems(eutList);deviceIndex++)
	{
	     ListGetItem(eutList,&device,deviceIndex);
		 
	     CVIXMLNewElement(element,deviceIndex-1,"Device", &currElem);
		 //дID
	     CVIXMLNewElement(currElem,0,"DeviceId",&currChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.index); 
	     CVIXMLSetElementValue (currChildElem, temp);		 
	     //д Name
	     CVIXMLNewElement(currElem,0,"DeviceName",&currChildElem);
	     CVIXMLSetElementValue (currChildElem, device.eutName);
		 //д Enable
	     CVIXMLNewElement(currElem,0,"Enable",&currChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.enable); 
	     CVIXMLSetElementValue (currChildElem, temp);		 
	     //д  MatainCom
	     CVIXMLNewElement(currElem,1,"MatainCom",&currChildElem);
         //���ں�
		 CVIXMLNewElement(currChildElem,0,"PortNum",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.matainConfig.portNum); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);
		 //������
		 CVIXMLNewElement(currChildElem,1,"BandRate",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.matainConfig.baudRate); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);
		 //У����  
		 CVIXMLNewElement(currChildElem,2,"Parity",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.matainConfig.parity); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);
		 
		 //����λ 
		 CVIXMLNewElement(currChildElem,3,"DataBit",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.matainConfig.dataBit); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);
		 
		 //ֹͣλ
		 CVIXMLNewElement(currChildElem,4,"StopBit",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.matainConfig.stopBit); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);
		 
		 //д RS422PORT
		 
	     CVIXMLNewElement(currElem,1,"RS422Com",&currChildElem);
		 //memset(temp,100,0);
		 //sprintf(temp,"%d",device.matainCom);  
	     //CVIXMLSetElementValue (currChildElem, temp);
		 //ά����
		 //CVIXMLNewElement(currElem,1,"MatainCom",&currChildElem);
		 //���ں�
		 CVIXMLNewElement(currChildElem,0,"RSR422ComPortNum",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.RS422Config.portNum); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);
		 //������
		 CVIXMLNewElement(currChildElem,1,"RS422ComBandRate",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.RS422Config.baudRate); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);
		//У����  
		 CVIXMLNewElement(currChildElem,2,"RS422ComParity",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.RS422Config.parity); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);
		 
		 //����λ 
		 CVIXMLNewElement(currChildElem,3,"RS422ComDataBit",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.RS422Config.dataBit); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);
		 //ֹͣλ
		 CVIXMLNewElement(currChildElem,4,"RS422ComtopBit",&currSuperChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.RS422Config.stopBit); 
	     CVIXMLSetElementValue (currSuperChildElem, temp);

	    //д  disable
	     /*CVIXMLNewElement(currElem,2,"Enable",&currChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.enable); 
	     CVIXMLSetElementValue (currChildElem, temp);*/
	    //д 429Card
	     CVIXMLNewElement(currElem,2,"ARINC429Card",&currChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.arinc429Config.card); 
	     CVIXMLSetElementValue (currChildElem, temp);		 
		 
	    //д 429Port
	     CVIXMLNewElement(currElem,2,"ARINC429Port",&currChildElem);
		 memset(temp,100,0); 
		 sprintf(temp,"%d",device.arinc429Config.ch); 
	     CVIXMLSetElementValue (currChildElem, temp);
	}
}


/** @brief��UUT�����浽���ݿ��У�xml  
 *  @param[in]  ��
 *  @param[out] ��
 *  @return     int
 *  @note       �޶���¼  :
*/
void saveEutListToXmlFile(ListType eutList)
{
    int                 error = 0;
    CVIXMLStatus        xmlErr = S_OK;
    CVIXMLDocument      document = 0;
    CVIXMLElement       currElem = 0;
    int                 numCols, itemIndex, len;
    char                *elemName = NULL, *elemValue = NULL;
	CVIXMLElement       currChildElem = 0; 
	int numChildElems=0,index=0;
    char fileName[MAX_PATHNAME_LEN]; 	


	GetProjectDir (fileName);
	strcat(fileName,deviceFile);

    /* Load document and process it */
    CVIXMLLoadDocument (fileName, &document);
    CVIXMLGetRootElement (document, &currElem);

    CVIXMLGetElementTagLength (currElem, &len);
    elemName = malloc (len + 1);
    CVIXMLGetElementTag (currElem, elemName);
    CVIXMLGetElementValueLength (currElem, &len);
    elemValue = malloc (len + 1);
    CVIXMLGetElementValue (currElem, elemValue);
    RemoveSurroundingWhiteSpace (elemValue);
    removeEutElement(currElem);	   //step1 ɾ��xml�ļ������е�eut��Ϣ
	eutToXml(eutList,currElem);   //step2 ��eut���浽xml�ļ���
Error:
    SetWaitCursor (0);
    free (elemName);
    free (elemValue);
	CVIXMLSaveDocument (document, 0, NULL);
    CVIXMLDiscardElement (currElem);
    CVIXMLDiscardDocument (document);
    if (FAILED (xmlErr)) {
        char    errBuf[512];
        CVIXMLGetErrorString (xmlErr, errBuf, sizeof (errBuf));
        MessagePopup ("XML Error", errBuf);
    }
}


