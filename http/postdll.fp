s��         \       P   �   D����                                                               postdll                                     CVIFUNC                                                      +  System_Net_CookieContainer *[]     *  System_Net_CookieContainer []     ) ��System_Net_CookieContainer **     ( ��System_Net_CookieContainer *     ' ��System_Net_CookieContainer     &  postdll_Class1 *[]     %  postdll_Class1 []     $ ��postdll_Class1 **     # ��postdll_Class1 *     " ��postdll_Class1     ! 	 
ssize_t *[]     
   	ssize_t []     
 
��ssize_t **     	 	��ssize_t *      ��ssize_t     
  	size_t *[]     	  size_t []     	 	��size_t **      ��size_t *      ��size_t       CDotNetAssemblyHandle *[]       CDotNetAssemblyHandle []      ��CDotNetAssemblyHandle **      ��CDotNetAssemblyHandle *      ��CDotNetAssemblyHandle       CDotNetHandle *[]       CDotNetHandle []      ��CDotNetHandle **      ��CDotNetHandle *      ��CDotNetHandle     	  void **[]       void *[]      ��void ***     
 ��void **     		  char **[]      ��char ***     
  	double *[]     	 	��double **      ��double *     	  float *[]      ��float **      ��float *       unsigned __int64 *[]        unsigned __int64 []     � ��unsigned __int64 **     � ��unsigned __int64 *     � ��unsigned __int64     � 	 
__int64 *[]     
�  	__int64 []     
� 
��__int64 **     	� 	��__int64 *     � ��__int64     �  unsigned char *[]     � ��unsigned char **     � ��unsigned char *     � ��char **     �  unsigned short *[]     � ��unsigned short **     � ��unsigned short *     	�  short *[]     � ��short **     � ��short *     �  unsigned int *[]     � ��unsigned int **     � ��unsigned int *     �  int *[]     � ��int **     � ��int *  �    LabWindows/CVI .NET controller instrument wrapper for postdll, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null.

The target assembly name is specified by the '__assemblyName' constant in the generated source file. If there are multiple versions of this assembly, and you want .NET to determine and load the appropriate one, then you can remove the version, culture, and public key information from the constant and just specify the name of the assembly.     t    Initializes this CVI .NET controller.

Call this function before calling any other functions in this controller.     �    A value indicating whether an error occurred. A negative error code indicates function failure.

Error codes are defined in cvi\include\cvidotnet.h.

You can use CDotNetGetErrorDescription to get the description of an error code.    	����    �    Library Status                     	            j    Closes this CVI .NET controller.

Call this function after your program is done using this controller.     �    A value indicating whether an error occurred. A negative error code indicates function failure.

Error codes are defined in cvi\include\cvidotnet.h.

You can use CDotNetGetErrorDescription to get the description of an error code.    ����    �    Library Status                     	            �    A value indicating whether an error occurred. A negative error code indicates function failure.

Error codes are defined in cvi\include\cvidotnet.h.

You can use CDotNetGetErrorDescription to get the description of an error code.     �    A handle to the requested .NET object of type: postdll.Class1

Use this handle to invoke members of this .NET object.

When it is no longer needed, you must discard this handle using CDotNetDiscardHandle.        A handle to the exception thrown by the target .NET assembly. If the output value is not NULL, then you must discard it using CDotNetDiscardHandle. Call CDotNetGetExceptionInfo to get information about the exception.

You may pass NULL for this parameter.    ����    �    Library Status                    � -   "  �    Instance Handle                   � - �   �    Exception Handle                   	            	            
        0    �    A value indicating whether an error occurred. A negative error code indicates function failure.

Error codes are defined in cvi\include\cvidotnet.h.

You can use CDotNetGetErrorDescription to get the description of an error code.     :    You must free the output string using CDotNetFreeMemory.        A handle to the exception thrown by the target .NET assembly. If the output value is not NULL, then you must discard it using CDotNetDiscardHandle. Call CDotNetGetExceptionInfo to get information about the exception.

You may pass NULL for this parameter.    p���    �    Library Status                  ���� -      �    url                             ���� - �    �    postData                          c -�    �    returnValue                       � �    �    Exception Handle                   	                    	            
        0    )    Gets the m_Cookie field of this object.     �    A value indicating whether an error occurred. A negative error code indicates function failure.

Error codes are defined in cvi\include\cvidotnet.h.

You can use CDotNetGetErrorDescription to get the description of an error code.     @    You must discard the output handle using CDotNetDiscardHandle.        A handle to the exception thrown by the target .NET assembly. If the output value is not NULL, then you must discard it using CDotNetDiscardHandle. Call CDotNetGetExceptionInfo to get information about the exception.

You may pass NULL for this parameter.    ���    �    Library Status                    
 -   '  �    returnValue                       R - �   �    Exception Handle                   	            	            
        0    )    Sets the m_Cookie field of this object.     �    A value indicating whether an error occurred. A negative error code indicates function failure.

Error codes are defined in cvi\include\cvidotnet.h.

You can use CDotNetGetErrorDescription to get the description of an error code.        A handle to the exception thrown by the target .NET assembly. If the output value is not NULL, then you must discard it using CDotNetDiscardHandle. Call CDotNetGetExceptionInfo to get information about the exception.

You may pass NULL for this parameter.    R���    �    Library Status                  ���� -   '  �    m_Cookie                          E - �   �    Exception Handle                   	                
        0 ����         	~  
�             K.        Initialize_postdll                                                                                                                      ����         .  �             K.        Close_postdll                                                                                                                           ����       ����  �             K.        postdll_Class1__Create                                                                                                                  ����       ����  �             K.        postdll_Class1_PostUrl                                                                                                                  ����         �  ]             K.        postdll_Class1__Get__m_Cookie                                                                                                           ����         !  P             K.        postdll_Class1__Set__m_Cookie                                                                                                                 �                                                                                     �Initialize                                                                           �Close                                                                             ����postdll_Class1                                                                       �Type Constructor                                                                     �postdll Class1 PostUrl                                                               �postdll Class1 Get m Cookie                                                          �postdll Class1 Set m Cookie                                                     