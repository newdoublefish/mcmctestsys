s��        �^   ' j~  �   �   ����                               MLApp                           Matlab Automation Server Type Library                                                                    � ��const char *     � ��LCID     	� 	��ERRORINFO  �  � ��HRESULT  � � ��SAFEARRAY *  � 	� 	��LPUNKNOWN     � ��VARIANT  � � ��VBOOL  �  � ��SCODE  � � ��CAObjHandle  � � ��DATE     � ��CURRENCY   '    Matlab Automation Server Type Library     '    Matlab Application Dispatch Interface     %    Matlab Application Server Interface    c    Use this function to create a new DIMLApp object, and obtain a handle to the object.

If the server application is already running, this function may or may not start another copy of the application.  This is determined by the server application.

You must call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED if you register any ActiveX event callbacks and want the callbacks to be called from the same thread in which they were registered.  If you do not call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED your callbacks will be called from a system thread.        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code.        The name or IP address of the machine on which you want to run the ActiveX server.  The name can be either a UNC name ("\\playdough")  or DNS name ("plato.natinst.com").

If you pass NULL for this parameter, and there is a RemoteServerName registry entry for this server, the server will be run on the machine specified by the RemoteServerName entry.

If you pass NULL for this parameter and there is no RemoteServerName registry entry for this server, the server will be run on the same machine as your program.    �    Pass 0 if you use the object only from the thread that calls this function.  Pass 1 if you use the object from multiple threads.

The CVI ActiveX library uses the COM Global Interface Table (GIT) to marshal interface pointers between threads.  There is overhead associated with using the GIT that should be avoided when possible.  If you do not pass the CAObjHandle between threads in your application, you do not need to use the GIT.    S    Pass the locale for the object.  This value tells the object how to interpret arguments to its methods.  Pass LOCALE_NEUTRAL to indicate the default language-neutral locale.  This value is not used by the server when you call it through a dual interface method.

The CVI ActiveX library passes this value to the IDispatch::Invoke method.     B    This parameter is reserved.  You must pass 0 for this parameter.     �    A handle to the requested ActiveX object.

Use this handle to call methods and get/set properties of this ActiveX object.

When it is no longer needed, you must discard this handle using CA_DiscardObjHandle.    �����  �    Status                            � -   �  �    Server                            � - �     �    Support Multithreading            	� -� �  �    Locale                            
� �      �    Reserved                          ' � � �  �    Object Handle                      	           NULL    1    LOCALE_NEUTRAL    0    	          s    Use this function to load an existing DIMLApp object from a file, and obtain a handle to the object.

If the server application is already running, this function may or may not start another copy of the application.  This is determined by the server application.

You must call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED if you register any ActiveX event callbacks and want the callbacks to be called from the same thread in which they were registered.  If you do not call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED your callbacks will be called from a system thread.        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code.     :    A file containing the data for an ActiveX server object.        The name or IP address of the machine on which you want to run the ActiveX server.  The name can be either a UNC name ("\\playdough")  or DNS name ("plato.natinst.com").

If you pass NULL for this parameter, and there is an ActivateAtStorage registry entry for this server, the server will be run on the machine on which the file specified by the filename parameter resides.

If you pass NULL for this parameter and there is no ActivateAtStorage registry entry for this server, the server will be run on the same machine as your program.    �    Pass 0 if you use the object only from the thread that calls this function.  Pass 1 if you use the object from multiple threads.

The CVI ActiveX library uses the COM Global Interface Table (GIT) to marshal interface pointers between threads.  There is overhead associated with using the GIT that should be avoided when possible.  If you do not pass the CAObjHandle between threads in your application, you do not need to use the GIT.    S    Pass the locale for the object.  This value tells the object how to interpret arguments to its methods.  Pass LOCALE_NEUTRAL to indicate the default language-neutral locale.  This value is not used by the server when you call it through a dual interface method.

The CVI ActiveX library passes this value to the IDispatch::Invoke method.     B    This parameter is reserved.  You must pass 0 for this parameter.     �    A handle to the requested ActiveX object.

Use this handle to call methods and get/set properties of this ActiveX object.

When it is no longer needed, you must discard this handle using CA_DiscardObjHandle.    �����  �    Status                             -   �  �    File Name                         E - � �  �    Server                            h -�     �    Support Multithreading            $ �  �  �    Locale                             � �     �    Reserved                          � �� �  �    Object Handle                      	               NULL    1    LOCALE_NEUTRAL    0    	          �    Use this function to get a handle to an active DIMLApp object.

You must call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED if you register any ActiveX event callbacks and want the callbacks to be called from the same thread in which they were registered.  If you do not call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED your callbacks will be called from a system thread.        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code.    �    The name or IP address of the machine on which to look for the active ActiveX server object.  The name can be either a UNC name ("\\playdough")  or DNS name ("plato.natinst.com").

If you pass NULL, the function will look for the active ActiveX server object on the same machine as your program.

NOTE: Windows 95 and Windows NT 4.0 do not support accessing active objects on remote machines.  Future versions of these operating systems may support this functionality.    �    Pass 0 if you use the object only from the thread that calls this function.  Pass 1 if you use the object from multiple threads.

The CVI ActiveX library uses the COM Global Interface Table (GIT) to marshal interface pointers between threads.  There is overhead associated with using the GIT that should be avoided when possible.  If you do not pass the CAObjHandle between threads in your application, you do not need to use the GIT.    S    Pass the locale for the object.  This value tells the object how to interpret arguments to its methods.  Pass LOCALE_NEUTRAL to indicate the default language-neutral locale.  This value is not used by the server when you call it through a dual interface method.

The CVI ActiveX library passes this value to the IDispatch::Invoke method.     B    This parameter is reserved.  You must pass 0 for this parameter.     �    A handle to the requested ActiveX object.

Use this handle to call methods and get/set properties of this ActiveX object.

When it is no longer needed, you must discard this handle using CA_DiscardObjHandle.    ����  �    Status                             -   �  �    Server                            � - �     �    Support Multithreading            � -� �  �    Locale                            ! �      �    Reserved                          !U � � �  �    Object Handle                      	           NULL    1    LOCALE_NEUTRAL    0    	              A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    m    Documentation for this function, if provided by the server, is located in the function help.

The Array Functions class in the CVI ActiveX Library contains functions to help convert a Safe Array into a C Style array. If you do not convert the Safe Array into a C Style array, you must free the Safe Array using the CVI ActiveX Library function CA_SafeArrayDestroy    m    Documentation for this function, if provided by the server, is located in the function help.

The Array Functions class in the CVI ActiveX Library contains functions to help convert a Safe Array into a C Style array. If you do not convert the Safe Array into a C Style array, you must free the Safe Array using the CVI ActiveX Library function CA_SafeArrayDestroy    #�����  �    Status                            %� -   �  �    Object Handle                     ' - � �  �    Error Info                        )A -� �  �    Name                              )� �  �  �    Workspace                         * � � �  �    Pr                                +� �� �  �    Pi                                 	                       NULL    ""    ""    	           	              A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    .�����  �    Status                            0� -   �  �    Object Handle                     2 - � �  �    Error Info                        4G -� �  �    Name                              4� �  �  �    Workspace                         5 � � �  �    Pr                                5y �� �  �    Pi                                 	                       NULL    ""    ""               A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.        The string returned by the function.

Documentation for this function, if provided by the server, is located in the function help.

When it is no longer needed, you must free the string returned in this parameter by calling the CVI ActiveX Library function CA_FreeMemory.    7�����  �    Status                            9� -   �  �    Object Handle                     :� - � �  �    Error Info                        = -� �  �    Name                              =� �     �    Return Value                       	                       NULL    ""    	               A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.    ?�����  �    Status                            A� -   �  �    Object Handle                     CF - � �  �    Error Info                         	                       NULL       A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.    F1����  �    Status                            HP -   �  �    Object Handle                     I� - � �  �    Error Info                         	                       NULL       A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.    L�����  �    Status                            N� -   �  �    Object Handle                     O� - � �  �    Error Info                         	                       NULL       A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.        The string returned by the function.

Documentation for this function, if provided by the server, is located in the function help.

When it is no longer needed, you must free the string returned in this parameter by calling the CVI ActiveX Library function CA_FreeMemory.    R�����  �    Status                            U  -   �  �    Object Handle                     VN - � �  �    Error Info                        Xz -� �  �    Name                              X� �  �  �    Workspace                         YF � �    �    Return Value                       	                       NULL    ""    ""    	               A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    [�����  �    Status                            ]� -   �  �    Object Handle                     _B - � �  �    Error Info                        an -� �  �    Name                              a� �  �  �    Workspace                         b: � � �  �    Char Array                         	                       NULL    ""    ""    ""        property Visible        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     �    The value returned by the ActiveX server function.

Documentation for this function, if provided by the server, is located in the function help.    d*����  �    Status                            fI -   �  �    Object Handle                     g� - � �  �    Error Info                        i� -�    �    Return Value                       	                       NULL    	                property Visible        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.    kw����  �    Status                            m� -   �  �    Object Handle                     n� - � �  �    Error Info                        q -�    �    New Value                          	                       NULL            method GetWorkspaceData        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    @    A Variant that is filled in by the function.

Documentation for this function, if provided by the server, is located in the function help.

The Variant Related Functions class in the CVI ActiveX Library contains functions to help you query the type of value stored in a Variant and to retrieve values from a Variant.

    r�����  �    Status                            t� -   �  �    Object Handle                     u� - � �  �    Error Info                        x( -� �  �    Name                              x� �  �  �    Workspace                         x� � � �  �    Pdata                              	                       NULL    ""    ""    	                method PutWorkspaceData        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    {�����  �    Status                            }� -   �  �    Object Handle                     @ - � �  �    Error Info                        �l -� �  �    Name                              �� �  �  �    Workspace                         �8 � � �  �    Data                               	                       NULL    ""    ""            method Feval        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    @    A Variant that is filled in by the function.

Documentation for this function, if provided by the server, is located in the function help.

The Variant Related Functions class in the CVI ActiveX Library contains functions to help you query the type of value stored in a Variant and to retrieve values from a Variant.

     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    �"f����  `    Status                            �A    �  `    Object Handle                     ��  h �  `    Error Info                        ��  � �  `    Bstr Name                         �! 0    `    Nargout                           �� � �  `    Pvar Arg Out                      �� T  �  `    Arg1                              �5 T h �  `    Arg2                              �� T � �  `    Arg3                              � T0 �  `    Arg4                              �g T� 	�  `    Arg5                              �� �  
�  `    Arg6                              �3 � h �  `    Arg7                              �� � � �  `    Arg8                              �� �0 �  `    Arg9                              �e �� �  `    Arg10                             �� �  �  `    Arg11                             �1 � h �  `    Arg12                             �� � � �  `    Arg13                             �� �0 �  `    Arg14                             �c �� �  `    Arg15                             �� �  �  `    Arg16                             �/ � h �  `    Arg17                             �� � � �  `    Arg18                             �� �0 �  `    Arg19                             �a �� �  `    Arg20                             ��  �  `    Arg21                             �- h �  `    Arg22                             �� � �  `    Arg23                             ��0 �  `    Arg24                             �_� �  `    Arg25                             ��5  �  `    Arg26                             �+5 h �  `    Arg27                             ��5 �  �  `    Arg28                             ��50 !�  `    Arg29                             �]5� "�  `    Arg30                             ��b  #�  `    Arg31                             �)b h $�  `    Arg32                              	                       NULL    ""        	            CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL        method GetVariable        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    9    The Variant returned by the function.

Documentation for this function, if provided by the server, is located in the function help.

The Variant Related Functions class in the CVI ActiveX Library contains functions to help you query the type of value stored in a Variant and to retrieve values from a Variant.

    ������  �    Status                            � -   �  �    Object Handle                     �l - � �  �    Error Info                        �� -� �  �    Name                              �� �  �  �    Workspace                         �d � � �  �    Return Value                       	                       NULL    ""    ""    	                Internal use only        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    F    An ActiveX object handle obtained from NewDIMLApp, OpenDIMLApp, ActiveDIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    @    A Variant that is filled in by the function.

Documentation for this function, if provided by the server, is located in the function help.

The Variant Related Functions class in the CVI ActiveX Library contains functions to help you query the type of value stored in a Variant and to retrieve values from a Variant.

     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    �6����  �    Status                            �U -   �  �    Object Handle                     �� - � �  �    Error Info                        �� -� �  �    Bstr Name                         �5 �     �    Nargout                           �� � � �  �    Pvar Arg Out                      �� ��    �    Nargin                            �I �  �  �    Var Arg In                         	                       NULL    ""        	                   b    Use this function to create a new IMLApp object, and obtain a handle to the object.

If the server application is already running, this function may or may not start another copy of the application.  This is determined by the server application.

You must call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED if you register any ActiveX event callbacks and want the callbacks to be called from the same thread in which they were registered.  If you do not call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED your callbacks will be called from a system thread.        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code.        The name or IP address of the machine on which you want to run the ActiveX server.  The name can be either a UNC name ("\\playdough")  or DNS name ("plato.natinst.com").

If you pass NULL for this parameter, and there is a RemoteServerName registry entry for this server, the server will be run on the machine specified by the RemoteServerName entry.

If you pass NULL for this parameter and there is no RemoteServerName registry entry for this server, the server will be run on the same machine as your program.    �    Pass 0 if you use the object only from the thread that calls this function.  Pass 1 if you use the object from multiple threads.

The CVI ActiveX library uses the COM Global Interface Table (GIT) to marshal interface pointers between threads.  There is overhead associated with using the GIT that should be avoided when possible.  If you do not pass the CAObjHandle between threads in your application, you do not need to use the GIT.    S    Pass the locale for the object.  This value tells the object how to interpret arguments to its methods.  Pass LOCALE_NEUTRAL to indicate the default language-neutral locale.  This value is not used by the server when you call it through a dual interface method.

The CVI ActiveX library passes this value to the IDispatch::Invoke method.     B    This parameter is reserved.  You must pass 0 for this parameter.     �    A handle to the requested ActiveX object.

Use this handle to call methods and get/set properties of this ActiveX object.

When it is no longer needed, you must discard this handle using CA_DiscardObjHandle.    ������  �    Status                            � -   �  �    Server                            � - �     �    Support Multithreading            �� -� �  �    Locale                            �. �      �    Reserved                          �x � � �  �    Object Handle                      	           NULL    1    LOCALE_NEUTRAL    0    	          r    Use this function to load an existing IMLApp object from a file, and obtain a handle to the object.

If the server application is already running, this function may or may not start another copy of the application.  This is determined by the server application.

You must call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED if you register any ActiveX event callbacks and want the callbacks to be called from the same thread in which they were registered.  If you do not call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED your callbacks will be called from a system thread.        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code.     :    A file containing the data for an ActiveX server object.        The name or IP address of the machine on which you want to run the ActiveX server.  The name can be either a UNC name ("\\playdough")  or DNS name ("plato.natinst.com").

If you pass NULL for this parameter, and there is an ActivateAtStorage registry entry for this server, the server will be run on the machine on which the file specified by the filename parameter resides.

If you pass NULL for this parameter and there is no ActivateAtStorage registry entry for this server, the server will be run on the same machine as your program.    �    Pass 0 if you use the object only from the thread that calls this function.  Pass 1 if you use the object from multiple threads.

The CVI ActiveX library uses the COM Global Interface Table (GIT) to marshal interface pointers between threads.  There is overhead associated with using the GIT that should be avoided when possible.  If you do not pass the CAObjHandle between threads in your application, you do not need to use the GIT.    S    Pass the locale for the object.  This value tells the object how to interpret arguments to its methods.  Pass LOCALE_NEUTRAL to indicate the default language-neutral locale.  This value is not used by the server when you call it through a dual interface method.

The CVI ActiveX library passes this value to the IDispatch::Invoke method.     B    This parameter is reserved.  You must pass 0 for this parameter.     �    A handle to the requested ActiveX object.

Use this handle to call methods and get/set properties of this ActiveX object.

When it is no longer needed, you must discard this handle using CA_DiscardObjHandle.    �E����  �    Status                            �S -   �  �    File Name                         ŕ - � �  �    Server                            Ǹ -�     �    Support Multithreading            �t �  �  �    Locale                            �� � �     �    Reserved                          � �� �  �    Object Handle                      	               NULL    1    LOCALE_NEUTRAL    0    	          �    Use this function to get a handle to an active IMLApp object.

You must call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED if you register any ActiveX event callbacks and want the callbacks to be called from the same thread in which they were registered.  If you do not call CA_InitActiveXThreadStyleForCurrentThread with COINIT_APARTMENTTHREADED your callbacks will be called from a system thread.        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code.    �    The name or IP address of the machine on which to look for the active ActiveX server object.  The name can be either a UNC name ("\\playdough")  or DNS name ("plato.natinst.com").

If you pass NULL, the function will look for the active ActiveX server object on the same machine as your program.

NOTE: Windows 95 and Windows NT 4.0 do not support accessing active objects on remote machines.  Future versions of these operating systems may support this functionality.    �    Pass 0 if you use the object only from the thread that calls this function.  Pass 1 if you use the object from multiple threads.

The CVI ActiveX library uses the COM Global Interface Table (GIT) to marshal interface pointers between threads.  There is overhead associated with using the GIT that should be avoided when possible.  If you do not pass the CAObjHandle between threads in your application, you do not need to use the GIT.    S    Pass the locale for the object.  This value tells the object how to interpret arguments to its methods.  Pass LOCALE_NEUTRAL to indicate the default language-neutral locale.  This value is not used by the server when you call it through a dual interface method.

The CVI ActiveX library passes this value to the IDispatch::Invoke method.     B    This parameter is reserved.  You must pass 0 for this parameter.     �    A handle to the requested ActiveX object.

Use this handle to call methods and get/set properties of this ActiveX object.

When it is no longer needed, you must discard this handle using CA_DiscardObjHandle.    �W����  �    Status                            �e -   �  �    Server                            �C - �     �    Support Multithreading            �� -� �  �    Locale                            �Z �      �    Reserved                          դ � � �  �    Object Handle                      	           NULL    1    LOCALE_NEUTRAL    0    	              A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    m    Documentation for this function, if provided by the server, is located in the function help.

The Array Functions class in the CVI ActiveX Library contains functions to help convert a Safe Array into a C Style array. If you do not convert the Safe Array into a C Style array, you must free the Safe Array using the CVI ActiveX Library function CA_SafeArrayDestroy    m    Documentation for this function, if provided by the server, is located in the function help.

The Array Functions class in the CVI ActiveX Library contains functions to help convert a Safe Array into a C Style array. If you do not convert the Safe Array into a C Style array, you must free the Safe Array using the CVI ActiveX Library function CA_SafeArrayDestroy    ������  �    Status                            � -   �  �    Object Handle                     �a - � �  �    Error Info                        ݍ -� �  �    Name                              �� �  �  �    Workspace                         �Y � � �  �    Pr                                �� �� �  �    Pi                                 	                       NULL    ""    ""    	           	              A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    ������  �    Status                            � -   �  �    Object Handle                     �d - � �  �    Error Info                        � -� �  �    Name                              �� �  �  �    Workspace                         �\ � � �  �    Pr                                �� �� �  �    Pi                                 	                       NULL    ""    ""               A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.    ������  �    Status                            �� -   �  �    Object Handle                     �9 - � �  �    Error Info                        �e -� �  �    Name                               	                       NULL    ""       A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.    ������  �    Status                            �� -   �  �    Object Handle                     �/ - � �  �    Error Info                         	                       NULL       A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.    �����  �    Status                            �9 -   �  �    Object Handle                     �� - � �  �    Error Info                         	                       NULL       A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.    �o����  �    Status                           � -   �  �    Object Handle                    � - � �  �    Error Info                         	                       NULL       A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    6    You can pass NULL for this parameter if you do not need this information.

Documentation for this function, if provided by the server, is located in the function help.

When it is no longer needed, you must free the string returned in this parameter by calling the CVI ActiveX Library function CA_FreeMemory.   �����  �    Status                           � -   �  �    Object Handle                    	. - � �  �    Error Info                       Z -� �  �    Name                             � �  �  �    Workspace                        & � �    �    Ml String                          	                       NULL    ""    ""    	               A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.   �����  �    Status                           � -   �  �    Object Handle                    D - � �  �    Error Info                       p -� �  �    Name                             � �  �  �    Workspace                        < � � �  �    Char Array                         	                       NULL    ""    ""    ""        property Visible        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     �    You can pass NULL for this parameter if you do not need this information.

Documentation for this function, if provided by the server, is located in the function help.   ,����  �    Status                           K -   �  �    Object Handle                    � - � �  �    Error Info                       � -�    �    P Val                              	                       NULL    	                property Visible        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.   �����  �    Status                            � -   �  �    Object Handle                    !� - � �  �    Error Info                       $# -�    �    P Val                              	                       NULL            method GetWorkspaceData        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    �    A Variant that is filled in by the function. You can pass NULL for this parameter if you do not need this information.

Documentation for this function, if provided by the server, is located in the function help.

The Variant Related Functions class in the CVI ActiveX Library contains functions to help you query the type of value stored in a Variant and to retrieve values from a Variant.

   %�����  �    Status                           '� -   �  �    Object Handle                    ) - � �  �    Error Info                       +8 -� �  �    Name                             +� �  �  �    Workspace                        , � � �  �    Pdata                              	                       NULL    ""    ""    	                method PutWorkspaceData        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.   /-����  �    Status                           1L -   �  �    Object Handle                    2� - � �  �    Error Info                       4� -� �  �    Name                             5) �  �  �    Workspace                        5� � � �  �    Data                               	                       NULL    ""    ""            method Feval        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    �    A Variant that is filled in by the function. You can pass NULL for this parameter if you do not need this information.

Documentation for this function, if provided by the server, is located in the function help.

The Variant Related Functions class in the CVI ActiveX Library contains functions to help you query the type of value stored in a Variant and to retrieve values from a Variant.

     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.   7yf����  `    Status                           9�    �  `    Object Handle                    :�  h �  `    Error Info                       =  � �  `    Bstr Name                        =u 0    `    Nargout                          =� � �  `    Pvar Arg Out                     ?m T  �  `    Arg1                             ?� T h �  `    Arg2                             @9 T � �  `    Arg3                             @� T0 �  `    Arg4                             A T� 	�  `    Arg5                             Ak �  
�  `    Arg6                             A� � h �  `    Arg7                             B7 � � �  `    Arg8                             B� �0 �  `    Arg9                             C �� �  `    Arg10                            Ci �  �  `    Arg11                            C� � h �  `    Arg12                            D5 � � �  `    Arg13                            D� �0 �  `    Arg14                            E �� �  `    Arg15                            Eg �  �  `    Arg16                            E� � h �  `    Arg17                            F3 � � �  `    Arg18                            F� �0 �  `    Arg19                            F� �� �  `    Arg20                            Ge  �  `    Arg21                            G� h �  `    Arg22                            H1 � �  `    Arg23                            H�0 �  `    Arg24                            H�� �  `    Arg25                            Ic5  �  `    Arg26                            I�5 h �  `    Arg27                            J/5 �  �  `    Arg28                            J�50 !�  `    Arg29                            J�5� "�  `    Arg30                            Kab  #�  `    Arg31                            K�b h $�  `    Arg32                              	                       NULL    ""        	            CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL    CA_DEFAULT_VAL        method GetVariable        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    �    A Variant that is filled in by the function. You can pass NULL for this parameter if you do not need this information.

Documentation for this function, if provided by the server, is located in the function help.

The Variant Related Functions class in the CVI ActiveX Library contains functions to help you query the type of value stored in a Variant and to retrieve values from a Variant.

   V�����  �    Status                           X� -   �  �    Object Handle                    Z - � �  �    Error Info                       \3 -� �  �    Name                             \� �  �  �    Workspace                        \� � � �  �    Pdata                              	                       NULL    ""    ""    	                Internal use only        A value indicating whether an error occurred.  A negative error code indicates function failure.

Error codes are defined in cvi\include\cviauto.h and cvi\sdk\include\winerror.h.

You can use CA_GetAutomationErrorString to get the description of an error code or CA_DisplayErrorInfo to display the description of the error code.

If the error code is DISP_E_EXCEPTION  (0x80020009 or -2147352567), then the Error Info parameter contains additional error information.  You can use CA_DisplayErrorInfo to display the error information.    C    An ActiveX object handle obtained from NewIMLApp, OpenIMLApp, ActiveIMLApp, or an ActiveX method or property.

All of the methods that can be applied to a particular object are grouped under a single class in the function tree.  The name of the class corresponds to the type of the object to which this handle must refer.    $    When an ActiveX server function fails with the error code DISP_E_EXCEPTION, descriptive information about the error code is stored in this parameter.  The descriptive information may include the error's code, source, and description.  It may also include a help file and help file context.

When an ActiveX server function fails with the error codes DISP_E_PARAMNOTFOUND, DISP_E_TYPEMISMATCH, or E_INVALIDARG, the parameter position of the invalid argument may be stored in the errorParamPos member of this parameter.

This parameter may be NULL.     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.    @    A Variant that is filled in by the function.

Documentation for this function, if provided by the server, is located in the function help.

The Variant Related Functions class in the CVI ActiveX Library contains functions to help you query the type of value stored in a Variant and to retrieve values from a Variant.

     ^    Documentation for this function, if provided by the server, is located in the function help.     ^    Documentation for this function, if provided by the server, is located in the function help.   `"����  �    Status                           bA -   �  �    Object Handle                    c� - � �  �    Error Info                       e� -� �  �    Bstr Name                        f �     �    Nargout                          f� � � �  �    Pvar Arg Out                     g� ��    �    Nargin                           h2 �  �  �    Var Arg In                         	                       NULL    ""        	                 ����         C                K.        NewDIMLApp                                                                                                                              ����         z  �             K.        OpenDIMLApp                                                                                                                             ����         U  ".             K.        ActiveDIMLApp                                                                                                                           ����       ����  ,�             K.        DIMLAppGetFullMatrix                                                                                                                    ����       ����  5�             K.        DIMLAppPutFullMatrix                                                                                                                    ����       ����  >�             K.        DIMLAppExecute                                                                                                                          ����       ����  Er             K.        DIMLAppMinimizeCommandWindow                                                                                                            ����       ����  K�             K.        DIMLAppMaximizeCommandWindow                                                                                                            ����       ����  R"             K.        DIMLAppQuit                                                                                                                             ����       ����  Z_             K.        DIMLAppGetCharArray                                                                                                                     ����       ����  b�             K.        DIMLAppPutCharArray                                                                                                                     ����         d  j]             K.        DIMLAppGetVisible                                                                                                                       ����         k]  qv             K.        DIMLAppSetVisible                                                                                                                       ����         rn  z<             K.        DIMLAppGetWorkspaceData                                                                                                                 ����         {�  ��             K.        DIMLAppPutWorkspaceData                                                                                                                 ����         �  ��         &    �.        DIMLAppFeval                                                                                                                            ����         ��  ��             K.        DIMLAppGetVariable                                                                                                                      ����         �  ��             K.        DIMLAppXLEval                                                                                                                           ����         ��  �Q             K.        NewIMLApp                                                                                                                               ����         ��  ��             K.        OpenIMLApp                                                                                                                              ����         ͥ  �}             K.        ActiveIMLApp                                                                                                                            ����       ����  �C             K.        IMLAppGetFullMatrix                                                                                                                     ����       ����  �(             K.        IMLAppPutFullMatrix                                                                                                                     ����       ����  ��             K.        IMLAppExecute                                                                                                                           ����       ����  �[             K.        IMLAppMinimizeCommandWindow                                                                                                             ����       ����  ��             K.        IMLAppMaximizeCommandWindow                                                                                                             ����       ����              K.        IMLAppQuit                                                                                                                              ����       ���� d             K.        IMLAppGetCharArray                                                                                                                      ����       ���� �             K.        IMLAppPutCharArray                                                                                                                      ����         s             K.        IMLAppGetVisible                                                                                                                        ����        s $�             K.        IMLAppSetVisible                                                                                                                        ����        %� -�             K.        IMLAppGetWorkspaceData                                                                                                                  ����        / 5�             K.        IMLAppPutWorkspaceData                                                                                                                  ����        7c L-         &    �.        IMLAppFeval                                                                                                                             ����        V� ^�             K.        IMLAppGetVariable                                                                                                                       ����        ` h�             K.        IMLAppXLEval                                                                                                                                  �                                                                                    �DIMLApp                                                                              �New DIMLApp                                                                          �Open DIMLApp                                                                         �Active DIMLApp                                                                       �Get Full Matrix                                                                      �Put Full Matrix                                                                      �Execute                                                                              �Minimize Command Window                                                              �Maximize Command Window                                                              �Quit                                                                                 �Get Char Array                                                                       �Put Char Array                                                                       �Get Visible                                                                          �Set Visible                                                                          �Get Workspace Data                                                                   �Put Workspace Data                                                                   �Feval                                                                                �Get Variable                                                                         �XL Eval                                                                             IMLApp                                                                               �New IMLApp                                                                           �Open IMLApp                                                                          �Active IMLApp                                                                        �Get Full Matrix                                                                      �Put Full Matrix                                                                      �Execute                                                                              �Minimize Command Window                                                              �Maximize Command Window                                                              �Quit                                                                                 �Get Char Array                                                                       �Put Char Array                                                                       �Get Visible                                                                          �Set Visible                                                                          �Get Workspace Data                                                                   �Put Workspace Data                                                                   �Feval                                                                                �Get Variable                                                                         �XL Eval                                                                         