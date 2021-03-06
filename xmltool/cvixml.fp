s��        �   . ϴ  
\   �    �@                                           CVIXML                                           � ��MSXMLObj_IXMLDOMAttribute     � ��MSXMLObj_IXMLDOMElement     � ��MSXMLObj_IXMLDOMDocument     � ��CVIXMLFormatType  �  � ��ListType  � � ��CVIXMLAttribute  � � ��CAObjHandle  �  � ��CVIXMLStatus  � � ��CVIXMLElement  � � ��CVIXMLDocument     	� 	��ERRORINFO  ]    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXML</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This instrument driver contains functions for creating and modifying XML documents.</p>

<p class="body">An XML document is a hierarchical representation of data, similar to a tree with parent/child relationships. The data appears as elements, which consist of a tag name and a value. Elements also can contain attributes.</p>

<p class="body"><span class="Monospace">cvixml.fp</span> includes the following classes:</p>

<ul>
<li><strong>Document Functions</strong>&mdash;Open XML documents for modification, create new XML documents, and save and format modified XML documents.</li>
<li><strong>Element Functions</strong>&mdash;Create new elements, modify existing elements, copy elements, search for elements by their tags, and delete elements.</li>   
<li><strong>Child Element Functions</strong>&mdash;Get the number of children of an element, get the children of an element, and get the parent of an element.</li>   
<li><strong>Attribute Functions</strong>&mdash;Get the number of attributes of an element; get the attributes of an element; and create, modify, and delete attributes.</li>   
<li><strong>Error Processing Functions</strong>&mdash;Process errors.</li>   
<li><strong>Low Level Functions</strong>&mdash;Get the ActiveX handle of a document, get the ActiveX handle of an element, and get the ActiveX handle of an attribute.</li>
</ul>

</body>
</html>



</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXML</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions to:</p>
<ul>
<li>open XML documents for modification</li>
<li>create new XML documents</li>
<li>save and format modified XML documents</li>

</ul>
  




</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLElementFunctions</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions to:</p>
<ul><li>create new elements</li>
<li>modify existing elements</li>
<li>copy elements</li>
<li>search for elements by their tags</li>
<li>delete elements</li></ul>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLChildElementFunctions</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions to:</p>
<ul><li>get the number of children an element has</li>
<li>get the children of an element</li>
<li>get the parent of an element</li></ul>





</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLAttributeFunctions</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions to:</p>
<ul><li>get the number of attributes an element has</li>
<li>get the attributes of an element</li>
<li>create new attributes</li>
<li>modify existing attributes</li>
<li>delete attributes</li></ul>





</body>
</html>
    G    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLErrorProcessing</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains error processing functions.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLLowLevelFunctions</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions to:</p>
<ul><li>get the ActiveX handle of a document</li>
<li>get the ActiveX handle of an element</li>
<li>get the ActiveX handle of an attribute</li>
</ul>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLLoadDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLLoadDocument</h1>

<p class="syntax">CVIXMLStatus CVIXMLLoadDocument (char *path, CVIXMLDocument *document);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Loads an existing XML document.</p>
  




</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLLoadDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLLoadDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">path</td>
<td class="paramDataType">char&nbsp;*</td>

<td>The pathname of the XML document to load.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLLoadDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">document</td>
<td class="paramDataType">CVIXMLDocument&nbsp;*</td>

<td>On output, contains the handle to the document object created in this function. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardDocument.htm')">CVIXMLDiscardDocument</a></span> function.</td>
</tr>
</table>

</body>
</html>    �����  �    Status                            | $      �    Path                              0 $� �  �    Document                           	                	           S    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLNewDocument</h1>

<p class="syntax">CVIXMLStatus CVIXMLNewDocument (char *rootElementTag, CVIXMLDocument *document);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Creates a new XML document. <span class="Monospace">CVIXMLNewDocument</span> creates a root element for the document with the given tag.</p>
  




</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">rootElementTag</td>
<td class="paramDataType">char&nbsp;*</td>

<td>The tag of the new root element of the XML document.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">document</td>
<td class="paramDataType">CVIXMLDocument&nbsp;*</td>

<td>On output, contains the handle to the document object created in this function. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardDocument.htm')">CVIXMLDiscardDocument</a></span> function.</td>
</tr>
</table>


</body>
</html>     ����  �    Status                            #� $      �    Root Element Tag                  %� $� �  �    Document                           	                	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSaveDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLSaveDocument</h1>

<p class="syntax">CVIXMLStatus CVIXMLSaveDocument (CVIXMLDocument document, int format, char *path);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Saves an XML document.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSaveDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSaveDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">document</td>
<td class="paramDataType">CVIXMLDocument</td>

<td>The document handle must be a valid CVIXMLDocument.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSaveDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">format</td>
<td class="paramDataType">integer</td>

<td>Choose whether to format the document or not. If you choose the formatting option, line breaks are inserted between elements.<br><br>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;Saving XML documents with the format option may cause problems with certain kinds of XML layout and in certain XML applications. National Instruments recommends that you save the XML document without formatting, unless you are sure that formatting causes no problems in your applications.</td>
</tr>
</table>

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSaveDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">path</td>
<td class="paramDataType">char&nbsp;*</td>

<td>The pathname of the XML document to save. If it is NULL, the pathname used with <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLLoadDocument.htm')">CVIXMLLoadDocument</a></span> is used. If there is not an existing pathname, an error is returned.</td>
</tr>
</table>

</body>
</html>
    +����  �    Status                            .� $   �  �    Document                          0� $ �          Format                            4w $l    �    Path                               	                Yes 1 No 0       �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLDiscardDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLDiscardDocument</h1>

<p class="syntax">void CVIXMLDiscardDocument (CVIXMLDocument document);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Discards an XML document and frees its memory.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLDiscardDocument</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">document</td>
<td class="paramDataType">CVIXMLDocument</td>

<td>The document handle must be a valid CVIXMLDocument.</td>
</tr>
</table>


</body>
</html>
    : $ �  �  �    Document                              k    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetRootElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetRootElement</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetRootElement (CVIXMLDocument document, CVIXMLElement *element);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the root element of an XML document. The root element of a document is the main element of the document, in which all other elements are contained.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetRootElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetRootElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">document</td>
<td class="paramDataType">CVIXMLDocument</td>

<td>The document handle must be a valid CVIXMLDocument.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetRootElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement&nbsp;*</td>

<td>On output, contains the handle to the root element object obtained in this function.  This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardElement.htm')">CVIXMLDiscardElement</a></span> function.</td>
</tr>
</table>

</body>
</html>
    >�����  �    Status                            Bq $   �  �    Document                          D9 $� �  �    Element                            	                	           $    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLNewElement</h1>

<p class="syntax">CVIXMLStatus CVIXMLNewElement (CVIXMLElement parentElement, int index, char *tag, CVIXMLElement *element);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Creates a new element at the specified position with the given tag.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">parentElement</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
    z    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">index</td>
<td class="paramDataType">integer</td>

<td>The zero-based index at which the element will be inserted under the parent element. Pass <span class="nobreak">&ndash;1</span> to append the element to the end. The index refers to the position of XML elements and does not include other XML entities.</td>
</tr>
</table>

</body>
</html>
    4    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tag</td>
<td class="paramDataType">char&nbsp;*</td>

<td>Tag of the new element. If it is NULL, an error is returned.<br><br>

Certain tags are invalid XML. These include those with white space, non-alphanumeric characters, or numbers.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLNewElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement&nbsp;*</td>

<td>On output, contains the handle to the element object created in this function. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardElement.htm')">CVIXMLDiscardElement</a></span> function.</td>
</tr>
</table>


</body>
</html>
    I�����  �    Status                            M� $   �  �    Parent Element                    O� $ �     �    Index                             R $�    �    Tag                               TG b  �  �    Element                            	                        	           E    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSetElementValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLSetElementValue</h1>

<p class="syntax">CVIXMLStatus CVIXMLSetElementValue (CVIXMLElement element, char *value);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Sets the value of an element. An element can have multiple values, but this function replaces the first value that it finds.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSetElementValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSetElementValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
    e    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSetElementValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">value</td>
<td class="paramDataType">char&nbsp;*</td>

<td>The new value of the element. If the empty string (&quot;&quot;) is passed in, then the element's value is removed. If it is NULL, an error is returned.<br>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;Some XML elements may have multiple value fields. This function sets the first value of the element. You can set other value fields using the Microsoft XML DOM ActiveX functions.</td>
</tr>
</table>

</td>
</tr>
</table>


</body>
</html>
    Z�����  �    Status                            ^_ $   �  �    Element                           `$ $�    �    Value                              	                   J    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementTagLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetElementTagLength</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetElementTagLength (CVIXMLElement element, int *length);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the length of the tag of an element.  This can be used to allocate memory before getting the tag of the element.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementTagLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.</p>

<p class="Body">You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:</p>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementTagLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementTagLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">length</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>On output, contains the length of the element's tag in bytes. The length does not include the terminating NUL-byte.</td>
</tr>
</table>

</body>
</html>
    f�����  �    Status                            j $   �  �    Element                           lH $�     �    Length                             	                	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementTag</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetElementTag</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetElementTag (CVIXMLElement element, char tag[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the tag of an element.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementTag</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementTag</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementTag</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tag</td>
<td class="paramDataType">char&nbsp;[]</td>

<td>On output, contains the element's tag. Pass in a buffer large enough to hold the tag.</td>
</tr>
</table>


</body>
</html>
    p�����  �    Status                            t� $   �  �    Element                           v� $�    �    Tag                                	                	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementValueLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetElementValueLength</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetElementValueLength (CVIXMLElement element, int *length);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the length of the value of an element. This can be used to allocate memory before getting the value of the element. An element can have multiple values, but this function gets the length of the first value that it finds.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementValueLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementValueLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementValueLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">length</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>On output, contains the length of the element's value in bytes. The length does not include the terminating NUL-byte.</td>
</tr>
</table>


</body>
</html>
    {�����  �    Status                            � $   �  �    Element                           �� $�     �    Length                             	                	           B    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetElementValue</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetElementValue (CVIXMLElement element, char value[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the value of an element. An element can have multiple values, but this function gets the first value that it finds.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table>
 
</td>
</tr>
</table>



</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">value</td>
<td class="paramDataType">char&nbsp;[]</td>

<td>On output, contains the element's value. Pass in a buffer large enough to hold the value.</td>
</tr>
</table>


</body>
</html>
    ������  �    Status                            �� $   �  �    Element                           �[ $�    �    Value                              	                	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLCopyElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLCopyElement</h1>

<p class="syntax">CVIXMLStatus CVIXMLCopyElement (CVIXMLElement element, int copyChildren, CVIXMLElement parentElement, int index, CVIXMLElement *copiedElement);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Creates a copy of an element at the specified position. The <strong>copyChildren</strong> parameter specifies whether or not to copy the children of the element.</p>



</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLCopyElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLCopyElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLCopyElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">copyChildren</td>
<td class="paramDataType">integer</td>

<td>Specifies whether or not to copy the element's children. Pass 0 not to copy the child elements, and pass a nonzero value to copy the child elements.</td>
</tr>
</table>

</body>
</html>

       �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLCopyElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">parentElement</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
    {    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLCopyElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">index</td>
<td class="paramDataType">integer</td>

<td>The zero-based index at which the element will be inserted under the parent element. Pass <span class="nobreak">&ndash;1</span> to append the element to the end. The index refers to the position of XML elements and does not include other XML entities.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLCopyElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">copiedElement</td>
<td class="paramDataType">CVIXMLElement&nbsp;*</td>

<td>On output, contains the handle to the element object created in this function. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardElement.htm')">CVIXMLDiscardElement</a></span> function.</td>
</tr>
</table>


</body>
</html>
    ������  �    Status                            �� $   �  �    Element                           �G $      �    Copy Children                     �n $� �  �    Parent Element                    �5 b      �    Index                             �� b � �  �    Copied Element                     	               Yes 1 No 0            	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLDiscardElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLDiscardElement</h1>

<p class="syntax">void CVIXMLDiscardElement (CVIXMLElement element);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Discards an element and frees its memory.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLDiscardElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
    �� $ �  �  �    Element                               \    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLFindElements</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLFindElements</h1>

<p class="syntax">CVIXMLStatus CVIXMLFindElements (CVIXMLElement parentElement, char *tag, ListType *elementList);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Finds all elements with the given tag that are descendants of the element passed in the <strong>parentElement</strong> parameter.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLFindElements</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">parentElement</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLFindElements</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLFindElements</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tag</td>
<td class="paramDataType">char&nbsp;*</td>

<td>Tag of the elements to search for. If you pass NULL for this parameter, an error is returned.<br><br>

Certain tags are invalid XML. These include those with white space, non-alphanumeric characters, or numbers.<br><br> 

The special string &quot;*&quot; matches all descendant elements, but other regular expressions and wild cards are not supported.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLFindElements</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">elementList</td>
<td class="paramDataType">ListType&nbsp;*</td>

<td>On output, contains the list of matching elements. The <span class="Monospace">ListType</span> data type resides in the Programmer's Toolbox. You can find more information about the <span class="Monospace">ListType</span> data type in <span class="Monospace">toolbox.fp</span>. After you finish using this list, you should discard each element in the list and dispose the list.<br><br>

<p class="code">/* Example code - error checking omitted */</p><br>

<p class="code">ListType elementList = 0;</p>
<p class="code">CVIXMLElement element = 0;</p>
<p class="code">int i, numElements;</p><br>

<p class="code">CVIXMLFindElements (parent, &quot;search_tag&quot;, &amp;elementList);</p><br>

<p class="code">/* Traversing the list */</p>
<p class="code">numElements = ListNumItems (elementList);</p>
<p class="code">for (i = 0; i < numElements; ++i)</p>
<p class="code">{</p>
<p class="code1">/* Get the ith element */</p>
<p class="code1">ListGetItem (elementList, &amp;element, i+1);</p><br>

<p class="code1">/* Do something with the element... */</p>
<p class="code">}</p><br>

<p class="code">/* Other code... */</p><br>

<p class="code">/* When done, discard the elements and dispose the list */</p>
<p class="code">for (i = 0; i < numElements; ++i)</p>
<p class="code">{</p>
<p class="code1">/* Remove each element and discard it */</p>
<p class="code1">ListRemoveItem (elementList, &amp;element, FRONT_OF_LIST);</p>
<p class="code1">CVIXMLDiscardElement (element);</p>
<p class="code">}</p>
<p class="code">ListDispose (elementList);</p>
</td>
</tr>
</table>



</body>
</html>
    �3 $   �  �    Parent Element                    ������  �    Status                            �� $ �    �    Tag                               �� $� �  �    Element List                           	                	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLRemoveElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLRemoveElement</h1>

<p class="syntax">CVIXMLStatus CVIXMLRemoveElement (CVIXMLElement element);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Removes an element from an XML document.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;You still need to discard the element handle when you finish using it.</td></tr></table>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLRemoveElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLRemoveElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement. If the input element is the root element in the XML document, an error is returned.</td>
</tr>
</table>

</body>
</html>
    �	����  �    Status                            �� $ �  �  �    Element                            	               �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNumChildElements</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetNumChildElements</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetNumChildElements (CVIXMLElement element, int *numberChildren);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the number of children an element has. <span class="Monospace">CVIXMLGetNumChildElements</span> counts only elements&mdash;all other XML data is not counted.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNumChildElements</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNumChildElements</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNumChildElements</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">numberChildren</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>On output, contains the element's number of children.</td>
</tr>
</table>



</body>
</html>
    ������  �    Status                            �� $   �  �    Element                           ʪ $�     �    Number Children                    	                	           n    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetChildElementByIndex</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetChildElementByIndex (CVIXMLElement element, int index, CVIXMLElement *childElement);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets an element's child at the specified position. If the element does not exist at the index, an error is returned.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>


</body>
</html>
    v    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">index</td>
<td class="paramDataType">integer</td>

<td>The zero-based index where the element is located under the parent element. Pass <span class="nobreak">&ndash;1</span> to get the last element. The index refers to the position of XML elements and does not include other XML entities.</td>
</tr>
</table>



</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">childElement</td>
<td class="paramDataType">CVIXMLElement&nbsp;*</td>

<td>On output, contains the handle to the element object obtained in this function. If the element does not exist at the index, an error is returned. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardElement.htm')">CVIXMLDiscardElement</a></span> function.</td>
</tr>
</table>



</body>
</html>
    ϲ����  �    Status                            Ӓ $   �  �    Element                           �_ $ �     �    Index                             �� $� �  �    Child Element                      	                    	           V    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByTag</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetChildElementByTag</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetChildElementByTag (CVIXMLElement element, char *tag, CVIXMLElement *childElement);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets an element's child by its tag. This function outputs the first element it finds with the tag.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByTag</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByTag</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>

</body>
</html>
    P    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByTag</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tag</td>
<td class="paramDataType">char&nbsp;*</td>

<td>Tag to search for the element by. If it is NULL, the function returns an error.<br><br>

Certain tags are invalid XML. These include those with white space, non-alphanumeric characters, or numbers.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetChildElementByTag</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">childElement</td>
<td class="paramDataType">CVIXMLElement&nbsp;*</td>

<td>On output, contains the handle to the element object obtained in this function. If the element does not exist, the output value is 0. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardElement.htm')">CVIXMLDiscardElement</a></span> function.</td>
</tr>
</table>

</body>
</html>
    �E����  �    Status                            �( $   �  �    Element                           �� $ �    �    Tag                               �J $� �  �    Child Element                      	                    	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNextElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetNextElement</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetNextElement (CVIXMLElement element, CVIXMLElement *nextElement);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the next element under this element's parent.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNextElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNextElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNextElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">nextElement</td>
<td class="paramDataType">CVIXMLElement&nbsp;*</td>

<td>On output, contains the handle to the element object obtained in this function. If the element does not exist, the output value is 0. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardElement.htm')">CVIXMLDiscardElement</a></span> function.</td>
</tr>
</table>


</body>
</html>
    �T����  �    Status                            �1 $   �  �    Element                           �� $� �  �    Next Element                       	                	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetPreviousElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetPreviousElement</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetPreviousElement (CVIXMLElement element, CVIXMLElement *previousElement);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the previous element under this element's parent.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetPreviousElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetPreviousElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>


</body>
</html>
         <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetPreviousElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">previousElement</td>
<td class="paramDataType">CVIXMLElement&nbsp;*</td>

<td>On output, contains the handle to the element object obtained in this function. If the element does not exist, the output value is 0. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardElement.htm')">CVIXMLDiscardElement</a></span> function.</td>
</tr>
</table>


</body>
</html>
    ������  �    Status                            �� $   �  �    Element                           � $� �  �    Previous Element                   	                	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetParentElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetParentElement</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetParentElement (CVIXMLElement element, CVIXMLElement *parentElement);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the parent of this element.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetParentElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetParentElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>



</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetParentElement</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">parentElement</td>
<td class="paramDataType">CVIXMLElement&nbsp;*</td>

<td>On output, contains the handle to the element object obtained in this function. If the input element is the root element in the XML document, the output value is 0. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardElement.htm')">CVIXMLDiscardElement</a></span> function.</td>
</tr>
</table>

</body>
</html>
   G����  �    Status                           & $   �  �    Element                          � $� �  �    Parent Element                     	                	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNumAttributes</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetNumAttributes</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetNumAttributes (CVIXMLElement element, int *numberAttributes);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the number of attributes an element has.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNumAttributes</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNumAttributes</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNumAttributes</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">numberAttributes</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>On output, contains the element's number of attributes.</td>
</tr>
</table>


</body>
</html>
   �����  �    Status                           � $   �  �    Element                          } $�     �    Number Attributes                  	                	           s    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetAttributeByIndex</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetAttributeByIndex (CVIXMLElement element, int index, CVIXMLAttribute *attribute);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets an element's attribute at the specified position. If the attribute does not exist at the index, the function returns an error.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body><table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>



</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">index</td>
<td class="paramDataType">integer</td>

<td>The zero-based index where the attribute is located under the parent element. Pass <span class="nobreak">&ndash;1</span> to get the last attribute.</td>
</tr>
</table>


</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByIndex</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute&nbsp;*</td>

<td>On output, contains the handle to the attribute object obtained in this function. If the attribute does not exist at the index, this function returns an error. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardAttribute.htm')">CVIXMLDiscardAttribute</a></span> function.</td>
</tr>
</table>

</body>
</html>
   �����  �    Status                           l $   �  �    Element                          6 $ �     �    Index                            !Z $� �  �    Attribute                          	                    	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetAttributeByName</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetAttributeByName (CVIXMLElement element, char *name, CVIXMLAttribute *attribute);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets an element's attribute by its name.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>



</body>
</html>
    W    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">name</td>
<td class="paramDataType">char&nbsp;*</td>

<td>Attribute name to get. If you pass in NULL, this function returns an error.<br><br>

Certain attribute names are invalid XML. These include those with white space, non-alphanumeric characters, or numbers.</td>
</tr>
</table>


</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeByName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute&nbsp;*</td>

<td>On output, contains the handle to the attribute object obtained in this function. If the attribute does not exist, the output value is 0. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardAttribute.htm')">CVIXMLDiscardAttribute</a></span> function.</td>
</tr>
</table>


</body>
</html>
   '�����  �    Status                           +o $   �  �    Element                          -9 $ �    �    Name                             /� $� �  �    Attribute                          	                    	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNextAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetNextAttribute</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetNextAttribute (CVIXMLAttribute attribute, CVIXMLAttribute *nextAttribute);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the next attribute under this attribute's parent.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNextAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNextAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute&nbsp;*</td>

<td>The attribute handle must be a valid CVIXMLAttribute.</td>
</tr>
</table>



</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetNextAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">nextAttribute</td>
<td class="paramDataType">CVIXMLAttribute&nbsp;*</td>

<td>On output, contains the handle to the attribute object obtained in this function. If the attribute does not exist, the output value is 0. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardAttribute.htm')">CVIXMLDiscardAttribute</a></span> function.</td>
</tr>
</table>



</body>
</html>
   5�����  �    Status                           9� $   �  �    Attribute                        ;p $� �  �    Next Attribute                     	                	           .    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetPreviousAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetPreviousAttribute</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetPreviousAttribute (CVIXMLAttribute attribute, CVIXMLAttribute *previousAttribute);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the previous attribute under this attribute's parent.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetPreviousAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetPreviousAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute</td>

<td>The attribute handle must be a valid CVIXMLAttribute.</td>
</tr>
</table>



</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetPreviousAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">previousAttribute</td>
<td class="paramDataType">CVIXMLAttribute&nbsp;*</td>

<td>On output, contains the handle to the attribute object obtained in this function. If the attribute does not exist, the output value is 0. This handle must be discarded using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLDiscardAttribute.htm')">CVIXMLDiscardAttribute</a></span> function.</td>
</tr>
</table>


</body>
</html>
   Ap����  �    Status                           ES $   �  �    Attribute                        G' $� �  �    Previous Attribute                 	                	           G    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLAddAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLAddAttribute</h1>

<p class="syntax">CVIXMLStatus CVIXMLAddAttribute (CVIXMLElement element, char *name, char *value);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Creates a new attribute with the given name and value. The new attribute is appended to the element's existing attributes.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLAddAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLAddAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>The element handle must be a valid CVIXMLElement.</td>
</tr>
</table>


</body>
</html>
    O    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLAddAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">name</td>
<td class="paramDataType">char&nbsp;*</td>

<td>Name of the new attribute. If it is NULL, this function returns an error.<br><br>

Certain attribute names are invalid XML. These include those with white space, non-alphanumeric characters, or numbers.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLAddAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">value</td>
<td class="paramDataType">char&nbsp;*</td>

<td>Value of the new attribute. If it is NULL, this function returns an error.</td>
</tr>
</table>


</body>
</html>
   MG����  �    Status                           Q" $   �  �    Element                          R� $ �    �    Name                             U< $�    �    Value                              	                       �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSetAttributeValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLSetAttributeValue</h1>

<p class="syntax">CVIXMLStatus CVIXMLSetAttributeValue (CVIXMLAttribute attribute, char *value);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Sets the value of an attribute.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSetAttributeValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSetAttributeValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute</td>

<td>The attribute handle must be a valid CVIXMLAttribute.</td>
</tr>
</table>


</body>
</html>
    1    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLSetAttributeValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">value</td>
<td class="paramDataType">char&nbsp;*</td>

<td>The new value of the attribute. If the empty string (&quot;&quot;) is passed in, then the attribute's value is removed. If it is NULL, this function returns an error.</td>
</tr>
</table>


</body>
</html>
   Y�����  �    Status                           ]� $   �  �    Attribute                        _� $�    �    Value                              	                   g    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeNameLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetAttributeNameLength</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetAttributeNameLength (CVIXMLAttribute attribute, int *length);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the length of the name of an attribute.  You can use this function to allocate memory before getting the name of the attribute.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeNameLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeNameLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute</td>

<td>The attribute handle must be a valid CVIXMLAttribute.</td>
</tr>
</table>


</body>
</html>
    
    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeNameLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">length</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>On output, contains the length of the attribute's name in bytes. The length does not include the terminating NUL-byte.</td>
</tr>
</table>


</body>
</html>
   e����  �    Status                           h� $   �  �    Attribute                        j� $�     �    Length                             	                	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetAttributeName</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetAttributeName (CVIXMLAttribute attribute, char name[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the name of an attribute.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute</td>

<td>The attribute handle must be a valid CVIXMLAttribute.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">name</td>
<td class="paramDataType">char&nbsp;[]</td>

<td>On output, contains the attribute's name. Pass in a buffer large enough to hold the name.</td>
</tr>
</table>



</body>
</html>
   o�����  �    Status                           sc $   �  �    Attribute                        u2 $�    �    Name                               	                	           c    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeValueLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetAttributeValueLength</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetAttributeValueLength (CVIXMLAttribute attribute, int *length);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the length of the value of an attribute.  This can be used to allocate memory before getting the value of the attribute.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeValueLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeValueLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute</td>

<td>The attribute handle must be a valid CVIXMLAttribute.</td>
</tr>
</table>



</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeValueLength</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">length</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>On output, contains the length of the attribute's value in bytes. The length does not include the terminating NUL-byte.</td>
</tr>
</table>


</body>
</html>
   zD����  �    Status                           ~* $   �  �    Attribute                        � $�     �    Length                             	                	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetAttributeValue</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetAttributeValue (CVIXMLAttribute attribute, char value[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the value of an attribute.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute</td>

<td>The attribute handle must be a valid CVIXMLAttribute.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeValue</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">value</td>
<td class="paramDataType">char&nbsp;[]</td>

<td>On output, contains the attribute's value. Pass in a buffer large enough to hold the value.</td>
</tr>
</table>


</body>
</html>
   ������  �    Status                           �� $   �  �    Attribute                        �| $�    �    Value                              	                	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLRemoveAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLRemoveAttribute</h1>

<p class="syntax">CVIXMLStatus CVIXMLRemoveAttribute (CVIXMLAttribute attribute);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Removes an attribute from an element.</p> <table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;You still need to discard the attribute handle when you finish using it.</td></tr></table>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLRemoveAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLRemoveAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute</td>

<td>The attribute handle must be a valid CVIXMLAttribute.</td>
</tr>
</table>


</body>
</html>
   ������  �    Status                           �� $ �  �  �    Attribute                          	               �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLDiscardAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLDiscardAttribute</h1>

<p class="syntax">void CVIXMLDiscardAttribute (CVIXMLAttribute attribute);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Discards an attribute and frees its memory.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLDiscardAttribute</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute</td>

<td>The attribute handle must be a valid CVIXMLAttribute.</td>
</tr>
</table>


</body>
</html>
   �� $ �  �  �    Attribute                                 <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetErrorString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetErrorString</h1>

<p class="syntax">void CVIXMLGetErrorString (CVIXMLStatus error, char buffer[], int bufferLength);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the error string corresponding to the error code.</p>





</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetErrorString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">error</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Error code returned by CVIXML function.</td>
</tr>
</table>



</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetErrorString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">buffer</td>
<td class="paramDataType">char&nbsp;[]</td>

<td>Buffer to return the error string in. Pass an array of size <em>bufferLength</em> bytes. If <strong>buffer</strong> cannot hold the entire error string, this function copies (<em>bufferLength</em> - 1) bytes into <strong>buffer</strong>, appended by the ASCII NUL byte.</td>
</tr>
</table>



</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetErrorString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">bufferLength</td>
<td class="paramDataType">integer</td>

<td>Pass the length of the buffer, in bytes, used to hold the error string.</td>
</tr>
</table>



</body>
</html>
   � $   �  �    Error                            �� $ �    �    Buffer                           �i $�     �    Buffer Length                          	                   <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetLastErrorInfo</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetLastErrorInfo</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetLastErrorInfo (ERRORINFO *errorInformation);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets the extended error information last set in the current thread.</p>
</body>
</html>
    \    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetLastErrorInfo</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A negative value indicates failure. On success, zero indicates that error information is available, and one indicates that error information is not available.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    ?    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetLastErrorInfo</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">errorInformation</td>
<td class="paramDataType">ERRORINFO&nbsp;*</td>

<td>Pass the address of an ERRORINFO structure. You can pass NULL if you do not want to get the error information but want to check whether error information is available.</td>
</tr>
</table>
</body>
</html>
   �����  �    Status                           �g $ �  �  �    Error Information                  	            	          #    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetDocumentActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetDocumentActiveXHandle</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetDocumentActiveXHandle (CVIXMLDocument document, MSXMLObj_IXMLDOMDocument *activeXHandle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets a document's ActiveX handle.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetDocumentActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    ~    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetDocumentActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">document</td>
<td class="paramDataType">CVIXMLDocument</td>

<td>The document handle that <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLLoadDocument.htm')">CVIXMLLoadDocument</a></span> outputs. It represents the XML document.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetDocumentActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">activeXHandle</td>
<td class="paramDataType">MSXMLObj_IXMLDOMDocument&nbsp;*</td>

<td>On output, contains the Microsoft XML DOM ActiveX document handle. The handle can be used with Microsoft XML DOM ActiveX functions. You must discard this handle by calling <span class="Monospace"><a href="javascript:LaunchHelp('cvi.chm::/LibRef/cviCA_DiscardObjHandle.htm')">CA_DiscardObjHandle</a></span> when you are done using it.</td>
</tr>
</table>

</body>
</html>
   �[����  �    Status                           �B $   �  �    Document                         �� $� �  �    ActiveX Handle                     	                	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetElementActiveXHandle</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetElementActiveXHandle (CVIXMLElement element, MSXMLObj_IXMLDOMElement *activeXHandle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets an element's ActiveX handle.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">element</td>
<td class="paramDataType">CVIXMLElement</td>

<td>Element whose ActiveX handle to get.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">activeXHandle</td>
<td class="paramDataType">MSXMLObj_IXMLDOMElement&nbsp;*</td>

<td>On output, contains the Microsoft XML DOM ActiveX element handle. The handle can be used with Microsoft XML DOM ActiveX functions. You must discard this handle by calling <span class="Monospace"><a href="javascript:LaunchHelp('cvi.chm::/LibRef/cviCA_DiscardObjHandle.htm')">CA_DiscardObjHandle</a></span> when you are done using it.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetElementActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
   �� $   �  �    Element                          �k $� �  �    ActiveX Handle                   �k����  �    Status                                 	            	           ,    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">CVIXMLGetAttributeActiveXHandle</h1>

<p class="syntax">CVIXMLStatus CVIXMLGetAttributeActiveXHandle (CVIXMLAttribute attribute, MSXMLObj_IXMLDOMAttribute *activeX_Handle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Gets an attribute's ActiveX handle.</p>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">CVIXMLStatus</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

You can call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviCVIXMLGetErrorString.htm')">CVIXMLGetErrorString</a></span> to obtain textual descriptions of error codes. Among the more commonly encountered errors are:<br><br>

<table>
<tr><td><span class="Monospace">E_OUTOFMEMORY</span></td>
<td>Out of memory.</td></tr>
<tr><td><span class="Monospace">E_INVALIDARG</span></td>
<td>Invalid argument to a function.</td></tr>
</table> 

</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">attribute</td>
<td class="paramDataType">CVIXMLAttribute</td>

<td>Attribute whose ActiveX handle to obtain.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>CVIXMLGetAttributeActiveXHandle</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">activeXHandle</td>
<td class="paramDataType">MSXMLObj_IXMLDOMAttribute&nbsp;*</td>

<td>On output, contains the Microsoft XML DOM ActiveX attribute handle. The handle can be used with Microsoft XML DOM ActiveX functions. You must discard this handle by calling <span class="Monospace"><a href="javascript:LaunchHelp('cvi.chm::/LibRef/cviCA_DiscardObjHandle.htm')">CA_DiscardObjHandle</a></span> when you are done using it.</td>
</tr>
</table>

</body>
</html>
   �@����  �    Status                           �( $   �  �    Attribute                        �� $� �  �    ActiveX Handle                     	                	         ����         �  �     K.    CVIXMLLoadDocument              ����         �  (t     K.    CVIXMLNewDocument               ����         )/  7=     K.    CVIXMLSaveDocument              ����         83  ;�     K.    CVIXMLDiscardDocument           ����         <"  G     K.    CVIXMLGetRootElement            ����         G�  W     K.    CVIXMLNewElement                ����         X5  c�     K.    CVIXMLSetElementValue           ����         dD  nS     K.    CVIXMLGetElementTagLength       ����         o  xt     K.    CVIXMLGetElementTag             ����         y/  ��     K.    CVIXMLGetElementValueLength     ����         �m  �F     K.    CVIXMLGetElementValue           ����         �  ��     K.    CVIXMLCopyElement               ����         ��  ��     K.    CVIXMLDiscardElement            ����         ��  �^     K.    CVIXMLFindElements              ����         �R  ��     K.    CVIXMLRemoveElement             ����         �v  ́     K.    CVIXMLGetNumChildElements       ����         �<  ��     K.    CVIXMLGetChildElementByIndex    ����         ��  �P     K.    CVIXMLGetChildElementByTag      ����         �D  ��     K.    CVIXMLGetNextElement            ����         ��  �     K.    CVIXMLGetPreviousElement        ����        B      K.    CVIXMLGetParentElement          ����        � T     K.    CVIXMLGetNumAttributes          ����         $|     K.    CVIXMLGetAttributeByIndex       ����        %p 2�     K.    CVIXMLGetAttributeByName        ����        3� >     K.    CVIXMLGetNextAttribute          ����        ?: J=     K.    CVIXMLGetPreviousAttribute      ����        J� W     K.    CVIXMLAddAttribute              ����        X  a�     K.    CVIXMLSetAttributeValue         ����        b� l�     K.    CVIXMLGetAttributeNameLength    ����        m� w     K.    CVIXMLGetAttributeName          ����        w� �     K.    CVIXMLGetAttributeValueLength   ����        �� �k     K.    CVIXMLGetAttributeValue         ����        �& ��     K.    CVIXMLRemoveAttribute           ����        � ��     K.    CVIXMLDiscardAttribute          ����        � �D     K.    CVIXMLGetErrorString            ����        �� ��     K.    CVIXMLGetLastErrorInfo          ����        �0 ��     K.    CVIXMLGetDocumentActiveXHandle  ����        �� �Q     K.    CVIXMLGetElementActiveXHandle   ����        � ��     K.    CVIXMLGetAttributeActiveXHandle       �                                    Document Functions                   DLoad Document                        DNew Document                         DSave Document                        DDiscard Document                     DGet Root Element                    	�Element Functions                    DNew Element                          DSet Element Value                    DGet Element Tag Length               DGet Element Tag                      DGet Element Value Length             DGet Element Value                    DCopy Element                         DDiscard Element                      DFind Elements                        DRemove Element                      �Child Element Functions              DGet Number Child Elements            DGet Child Element By Index           DGet Child Element By Tag             DGet Next Element                     DGet Previous Element                 DGet Parent Element                  �Attribute Functions                  DGet Number Attributes                DGet Attribute By Index               DGet Attribute By Name                DGet Next Attribute                   DGet Previous Attribute               DAdd Attribute                        DSet Attribute Value                  DGet Attribute Name Length            DGet Attribute Name                   DGet Attribute Value Length           DGet Attribute Value                  DRemove Attribute                     DDiscard Attribute                   �Error Processing                     DGet Error String                     DGet Last Error Info                 �Low Level Functions                  DGet Document ActiveX Handle          DGet Element ActiveX Handle           DGet Attribute ActiveX Handle          msxmldom.fp    toolbox.fp 