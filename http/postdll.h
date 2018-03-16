// CVI wrapper header file for .NET assembly: postdll, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null
//
// This is specified by the '__assemblyName' constant in the generated source file.
// If there are multiple versions of this assembly, and you want .NET to determine 
// the appropriate one to load, then you can remove the version, culture, and public 
// key token information from the constant and just specify the name of the assembly.
//
// Other assemblies referenced by the target assembly:
// Name:+ System, Location: C:\WINDOWS\Microsoft.Net\assembly\GAC_MSIL\System\v4.0_4.0.0.0__b77a5c561934e089\System.dll
//
// If any assembly, including the target assembly, is not installed in the
// Global Assembly Cache (GAC) or in the application directory, and the wrapper
// code needs to load the assembly to convert types like arrays and enums, then
// you must register the path of the assembly with the CVI .NET library by
// calling CDotNetRegisterAssemblyPath before calling the wrapper code.
//
// Types exposed by the target assembly but defined in other assemblies:
// CVI name: System_Net_CookieContainer, .NET name: System.Net.CookieContainer, Assembly: System, Module: Global Assembly Cache

#include <cvidotnet.h>

#ifdef __cplusplus
extern "C" {
#endif

// Type definitions
typedef struct __postdll_Class1 * postdll_Class1;
typedef struct __System_Net_CookieContainer * System_Net_CookieContainer;




// Global Functions
int CVIFUNC Initialize_postdll(void);
int CVIFUNC Close_postdll(void);

// Type: postdll.Class1
int CVIFUNC postdll_Class1__Create(
	postdll_Class1 * __instance,
	CDotNetHandle * __exception);
int CVIFUNC postdll_Class1_PostUrl(
	char * url,
	char * postData,
	char ** __returnValue,
	CDotNetHandle * __exception);
int CVIFUNC postdll_Class1__Get__m_Cookie(
	System_Net_CookieContainer * __returnValue,
	CDotNetHandle * __exception);
int CVIFUNC postdll_Class1__Set__m_Cookie(
	System_Net_CookieContainer m_Cookie,
	CDotNetHandle * __exception);


#ifdef __cplusplus
}
#endif
