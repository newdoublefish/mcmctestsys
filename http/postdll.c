// CVI wrapper source file for .NET assembly: postdll, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null

#include "postdll.h"

// Macros
#ifndef __errChk
#define __errChk(f) if (__error = (f), __error < 0) goto __Error; else
#endif
#ifndef __nullChk
#define __nullChk(p) if (!(p)) { __error = CDotNetOutOfMemoryError; goto __Error; } else
#endif

// Constants
static const char * __assemblyName = "postdll, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null";

// Static Variables
static CDotNetAssemblyHandle __assemblyHandle = 0;

// Forward declarations
static void _CDotNetGenDisposeScalar(void * scalar, unsigned int typeId);

// Global Functions
int CVIFUNC Initialize_postdll(void)
{
	int __error = 0;


	if (__assemblyHandle == 0)
		__errChk(CDotNetLoadAssembly(
			__assemblyName, 
			&__assemblyHandle));



__Error:
	return __error;
}

int CVIFUNC Close_postdll(void)
{
	int __error = 0;


	if (__assemblyHandle) {
		__errChk(CDotNetDiscardAssemblyHandle(__assemblyHandle));
		__assemblyHandle = 0;
	}



__Error:
	return __error;
}


// Type: postdll.Class1
int CVIFUNC postdll_Class1__Create(
	postdll_Class1 * __instance,
	CDotNetHandle * __exception)
{
	int __error = 0;

	if (__exception)
		*__exception = 0;


	*__instance = 0;

	// Call constructor
	__errChk(CDotNetCreateGenericInstance(
		__assemblyHandle, 
		"postdll.Class1", 
		0, 
		0, 
		__instance, 
		0, 
		0, 
		0, 
		0, 
		__exception));


__Error:
	return __error;
}

int CVIFUNC postdll_Class1_PostUrl(
	char * url,
	char * postData,
	char ** __returnValue,
	CDotNetHandle * __exception)
{
	int __error = 0;
	char * __parameterTypeNames[2] = {0};
	unsigned int __parameterTypes[2];
	void * __parameters[2];
	unsigned int __returnValueTypeId;

	if (__exception)
		*__exception = 0;
	if (__returnValue)
		*__returnValue = 0;


	// Pre-process parameter: url
	__parameterTypeNames[0] = "System.String";
	__parameterTypes[0] = (CDOTNET_STRING);
	__parameters[0] = &url;

	// Pre-process parameter: postData
	__parameterTypeNames[1] = "System.String";
	__parameterTypes[1] = (CDOTNET_STRING);
	__parameters[1] = &postData;

	// Pre-process return value
	__returnValueTypeId = CDOTNET_STRING;

	// Call static member
	__errChk(CDotNetInvokeGenericStaticMember(
		__assemblyHandle, 
		"postdll.Class1", 
		0, 
		0, 
		CDOTNET_CALL_METHOD, 
		"PostUrl", 
		0, 
		0, 
		2, 
		(const char **)__parameterTypeNames, 
		__parameterTypes, 
		__parameters, 
		&__returnValueTypeId, 
		__returnValue, 
		__exception));


__Error:
	if (__error < 0) {
		_CDotNetGenDisposeScalar(
			__returnValue, 
			CDOTNET_STRING);
	}
	return __error;
}

int CVIFUNC postdll_Class1__Get__m_Cookie(
	System_Net_CookieContainer * __returnValue,
	CDotNetHandle * __exception)
{
	int __error = 0;
	unsigned int __returnValueTypeId;

	if (__exception)
		*__exception = 0;
	if (__returnValue)
		*__returnValue = 0;


	// Pre-process return value
	__returnValueTypeId = CDOTNET_OBJECT;

	// Call static member
	__errChk(CDotNetInvokeGenericStaticMember(
		__assemblyHandle, 
		"postdll.Class1", 
		0, 
		0, 
		CDOTNET_GET_FIELD, 
		"m_Cookie", 
		0, 
		0, 
		0, 
		0, 
		0, 
		0, 
		&__returnValueTypeId, 
		__returnValue, 
		__exception));


__Error:
	if (__error < 0) {
		_CDotNetGenDisposeScalar(
			__returnValue, 
			CDOTNET_OBJECT);
	}
	return __error;
}

int CVIFUNC postdll_Class1__Set__m_Cookie(
	System_Net_CookieContainer m_Cookie,
	CDotNetHandle * __exception)
{
	int __error = 0;
	char * __parameterTypeNames[1] = {0};
	unsigned int __parameterTypes[1];
	void * __parameters[1];

	if (__exception)
		*__exception = 0;


	// Pre-process parameter: m_Cookie
	__parameterTypeNames[0] = "System.Net.CookieContainer,System, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089";
	__parameterTypes[0] = (CDOTNET_OBJECT);
	__parameters[0] = &m_Cookie;

	// Call static member
	__errChk(CDotNetInvokeGenericStaticMember(
		__assemblyHandle, 
		"postdll.Class1", 
		0, 
		0, 
		CDOTNET_SET_FIELD, 
		"m_Cookie", 
		0, 
		0, 
		1, 
		(const char **)__parameterTypeNames, 
		__parameterTypes, 
		__parameters, 
		0, 
		0, 
		__exception));


__Error:
	return __error;
}



// Internal functions
static void _CDotNetGenDisposeScalar(void * scalar, unsigned int typeId)
{
	if (!*(void **)scalar)
		return;

	typeId &= CDOTNET_BASIC_TYPE_MASK;
	if (typeId == CDOTNET_STRING) {
		CDotNetFreeMemory(*(char **)scalar);
		*(char **)scalar = 0;
	}
	else if (typeId == CDOTNET_OBJECT || typeId == CDOTNET_STRUCT) {
		CDotNetDiscardHandle(*(CDotNetHandle *)scalar);
		*(CDotNetHandle *)scalar = 0;
	}
}

