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
#include "toolbox.h"
#include "resultInfo.h"
#include "settingEntity.h" 

BOOL loadResultInfo(HashTableType hashTable){

	return TRUE;
} 


#if 0
void saveEutList()
{
	char fileName[MAX_PATHNAME_LEN];
	SETTING set=getSetting();
	GetProjectDir (fileName);
	//strcat(fileName,deviceFile);
	
    CVIXMLDocument      document = 0;
    CVIXMLElement       currElem = 0 ,currChildElem = 0;  
    int                 len,numChildElems;
    char                *elemName = NULL, *elemValue = NULL;
    /* Load document and process it */
    CVIXMLLoadDocument (fileName, &document);
    CVIXMLGetRootElement (document, &currElem);
    CVIXMLGetElementTagLength (currElem, &len);
    elemName = malloc (len + 1);
    CVIXMLGetElementTag (currElem, elemName);
    CVIXMLGetElementValueLength (currElem, &len);
    elemValue = malloc (len + 1);
	
    CVIXMLGetElementValue (currElem, elemValue);

	CVIXMLGetNumChildElements (currElem, &numChildElems); 

	//removeEutElement(currElem);
	/*for(int i=1;i<=ListNumItems(eutList);i++)
	{
		CVIXMLNewElement(currElem,i-1,"Results", &currChildElem); 
		EEUT eut;
		ListGetItem(eutList,&eut,i);
		saveEut2Xml(eut,currChildElem);
		CVIXMLDiscardElement (currChildElem);
	}  */
	
    free (elemName);
    free (elemValue);
	CVIXMLSaveDocument (document, 0, NULL);
    CVIXMLDiscardElement (currElem);
    CVIXMLDiscardDocument (document);	
}
#endif
BOOL saveResultInfo(char *fileName,HashTableType hashTable){
	return TRUE;
}
