s��   	     ��   4 `�  !   �   	 ��                                Ini                             Reading/Writing .ini-Style Files                                                                         � ��__int64  � � ��unsigned char *     �  const unsigned char[]     � ��const char *     �  const char []     � ��Ini_OutputFunc     � ��Ini_InputFunc     � ��Ini_SectionFilterFunc     � ��IniText  !    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
<script src="launchhelp.js"></script>
</head>

<body>
<p class="Body">This <a href="javascript:LaunchMergedHelp('cvi.chm','toolslib.chm', 'toolslibUsing_an_Instrument_Driver.htm')">instrument driver</a> contains functions for storing and accessing hierarchical configuration information using <span class="Monospace">.ini</span>-style files or Windows Registry Keys.</p>  

<p class=body>To increase speed, this instrument driver has not been implemented
with any multithread&ndash;safe locking.  National Instruments recommends that you do not share access to
the same <span class="Monospace">.ini</span> file among multiple threads.  You can have multiple <span class="Monospace">.ini</span> files coexisting with multiple threads, as long has no single <span class="Monospace">.ini</span> file is shared among multiple threads.</p>

<p class=body>Existing comments in the <span class="Monospace">.ini</span> files are indicated by a preceding semicolon. These comments persist when you read, write, or edit the file; however, if you delete a section of the file, you also delete any comments associated with that section.</p>

<p class=body><span class="Monospace">.ini</span>&ndash;style files have the following format:</p>
<ul><p class="code">[section 1]
<BR>tag 1 = &quot;string value 1&quot;
<BR>tag 2 = &quot;string value 2&quot;
<BR>tag 3 = 53
</ul>
</p>

<ul><p class="code">; This is a section 2 comment
<br>[section 2]
<BR>tag 1 = True
<BR>tag 2 = &ndash;12.3</p>
</ul>

<p class=body>where <span class="Monospace">section 1</span>, <span class="Monospace">section 2</span>, <span class="Monospace">tag 1</span>, <span class="Monospace">tag 2</span>, and <span class="Monospace">tag 3</span> are user&ndash;defined
names. The number of sections and tags is unlimited.</p>

<p class=body>The Windows Registry has the following format:

<p class=body>Root Key<br>
<p class=indent>SubKey 1<br>
<p class=indent2>Value Name 1&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Value Data<br>
<p class=indent>SubKey 2<br>
<p class=indent2>Value Name 2&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Value Data</p>


<p class=body>where &quot;SubKey 1&quot;, &quot;SubKey 2&quot;, &quot;Value Name 1&quot;, and &quot;Value Name 2&quot; are
user&ndash;defined names, but &quot;Root Key&quot; is one of a number of system&ndash;defined values.</p>

<p class=body>This instrument driver helps you access, store, and manipulate Sections of
configuration information, each containing tag/value pairs. For <span class="Monospace">.ini</span> files,
Sections correspond to bracketed entries in the file, where each Section usually
groups logically a number of tag=value entries (tag/value pairs). </p>

<p class=body>For Windows Registry branches, contain all configuration information for an
application in a branch starting at a user&ndash;defined Base Key that descends from one of the system&ndash;defined Root Keys.  Each SubKey under the Base Key
corresponds to a Section, where each value&ndash;name/value&ndash;data pair under that Key
is considered a tag/value pair.</p>

<p class=body>This instrument driver supports the following logical value types:</p>

<ul><p class=body>
<li>    strings</li>
<li>    integers</li>
<li>    unsigned integers</li>
<li>    doubles</li>
<li>    boolean</li>
</ul>

<p class=body>Strings can be of unlimited size. If strings in a <span class="Monospace">.ini</span>&ndash;style file are more than 80 characters in length, they are broken into
multiple lines. Each line has a tag suffix, which the following example demonstrates:</p>

<ul>
        [section 1]
<BR>    tag 1 line 0001 = &quot;This is the first line of a very long &quot;
<BR>    tag 1 line 0002 = &quot;paragraph.  This is the second line.&quot;
</ul>


<p class=body>Unprintable characters are written using hexadecimal escape codes.</p>
<p class=body>For Windows Registry storage, all data is actually string data.  Your
application is free to interpret the data in any supported format.</p>

<p class=body>For writing this type of configuration information to the system, the general
approach is to  build up the tag/value pairs in memory and then to write the
entire file or Registry branch at once.   For reading, the general approach is to
read the entire file or Key branch into memory at once and then to
interrogate individual tag/value pairs.</p>

<p class=body>To write a file with two sections, each containing two tags, the code without
error checking might look similar to the following code:</p>
<p class=body><span class="Monospace">
   IniText iniText;
<BR>    char pathName[MAX_PATHNAME_LEN];
<BR>    char dirName[MAX_PATHNAME_LEN];
<BR>
<BR>    /* set up the pathName for the .ini file */
<BR>    GetProjectDir (dirName);
<BR>    MakePathname (dirName, &quot;myconfig.ini&quot;, pathName);
<BR>
<BR>    /* create object for holding the value/tag pairs */
<BR>    iniText = Ini_New (TRUE);   /* TRUE for automatic sorting */
<BR>
<BR>    /* create the in&ndash;memory tag/value pairs */
<BR>    Ini_PutString (iniText, &quot;section 1&quot;, &quot;tag 1&quot;, &quot;string 1&quot;);
<BR>    Ini_PutString (iniText, &quot;section 1&quot;, &quot;tag 2&quot;, &quot;string 2&quot;);
<BR>    Ini_PutInt (iniText, &quot;section 2&quot;, &quot;tag 1&quot;, 53);
<BR>    Ini_PutBoolean (iniText, &quot;section 2&quot;, &quot;tag 2&quot;, TRUE);
<BR>
<BR>    /* write out the tag/value pairs */
<BR>    Ini_WriteToFile (iniText, pathName);
<BR>
<BR>    /* dispose of the in&ndash;memory tag/value pairs */
<BR>    Ini_Dispose (iniText);</span></p>

<p class=body>To read the same file, the code without error checking might look similar to
the following code:</p>

<p class=body><span class="Monospace">
    IniText iniText;
<BR>    char pathName[MAX_PATHNAME_LEN];
<BR>    char dirName[MAX_PATHNAME_LEN];
<BR>    char *str1, *str2;
<BR>    int intVal, boolVal;
<BR>
<BR>    /* set up the pathName for the .ini file */
<BR>    GetProjectDir (dirName);
<BR>    MakePathname (dirName, &quot;myconfig.ini&quot;, pathName);
<BR>
<BR>    /* create object for holding the value/tag pairs */
<BR>    iniText = Ini_New (TRUE);   /* TRUE for automatic sorting */
<BR>
<BR>    /* read in the tag/value pairs */
<BR>    Ini_ReadFromFile (iniText, pathName);
<BR>
<BR>    /* create the in&ndash;memory tag/value pairs */
<BR>    Ini_GetStringCopy (iniText, &quot;section 1&quot;, &quot;tag 1&quot;, &amp;str1);
<BR>    Ini_GetStringCopy (iniText, &quot;section 1&quot;, &quot;tag 2&quot;, &amp;str2);
<BR>    Ini_GetInt (iniText, &quot;section 2&quot;, &quot;tag 1&quot;, &amp;intVal);
<BR>    Ini_GetBoolean (iniText, &quot;section 2&quot;, &quot;tag 2&quot;, &amp;boolVal);
<BR>
<BR>    /* dispose of the in&ndash;memory tag/value pairs */
<BR>    Ini_Dispose (iniText);</span></p>


<p class=body>You gain the following advantages by storing information in this type of standard fashion.</p>

<UL>
<LI> Humans can read, and potentially edit, the files and Keys.</li>

<li> Adding new information to the file does not change its format. This makes backwards compatibility virtually automatic.</li>

<li> Multiple independent modules or programs can use the same file, as long
as each module or program uses unique section names.</li>

<li>Storing information in this way is platform&ndash;independent, which means there are no byte&ndash;packing, alignment, OS&ndash;dependence, or endianization issues.</li>
</ul>

<p class=body>The disadvantage of storing information in this fashion is that  binary format
has the maximum potential for size and speed optimization.</p>

<p class=body>While it may seem that the section/item organization of this type of storage
limits the data to two levels of hierarchy, it is possible to obtain multiple
levels of hierarchy by adding a prefix for each level of hierarchy to the section
names.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;This instrument driver makes extensive use of dynamic memory and therefore runs much slower if the project's debugging level is set to extended. Set the debugging level in the Build Options dialog box, which you can access by selecting <strong>Options&raquo;Build Options</strong>.</td></tr></table>

</BODY>
</HTML>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_Management</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions to:</p>

   <ul><li>create and dispose an object which can contain an in-memory 
     list of tag/value pairs.</li>

   <li>sort the in-memory list of tag/value pairs</li>
   
   <li>write the in-memory list of tag/value pairs to a file</li>

   <li>read a list of tag/value pairs from a file</li>

   <li>write the in-memory list of tag/value pairs to the Windows
     Registry</li>

   <li>Read a list of tag/value pairs from the Windows Registry </li></ul>


</body>
</html>    c    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PuttingTagValuePairs</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions to add tag/value pairs to the in-memory list. </p>

</body>
</html>
    {    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GettingValuesFromTags</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions to obtain values associated with tags in the in-memory tag/value list.</p>

</body>
</html>    j    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_Informational</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions which return information about the in-memory tag/value list.</p>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemovingSectionsAndItems</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class provides functions for deleting sections and items from a list of in&ndash;memory tag/value pairs.</p>

</body>
</html>    
	    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_New</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_New</h1>

<p class="syntax">IniText Ini_New (int automaticSorting);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function creates an object which can contain an in-memory list of tag/value pairs within sections.  You can:</p>

<ul><li>Read tag/value pairs into this object using the 
    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_ReadFromFile.htm')">Ini_ReadFromFile</a></span> or <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_ReadFromRegistry.htm')">Ini_ReadFromRegistry</a></span> functions, and
    then obtain the values using the <span class="Monospace">Ini_Get...</span> functions;  or</li>

<li>Insert tag/value pairs into this object using the 
    <span class="Monospace">Ini_Put...</span> functions, and then write the tag/value 
    pairs to the system using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_WriteToFile.htm')">Ini_WriteToFile</a></span> or 
    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_WriteToRegistry.htm')">Ini_WriteToRegistry</a></span> functions.</li></ul>

<p class="Body">Since this instrument driver has not been implemented with any multithread-safe locking, we recommend that you do not share the same .ini object among multiple threads.  It is all right to have multiple .ini objects among multiple threads as long has no single .ini object is shared among the threads.</p>

<p class="Body">This function allows you to choose whether the in-memory list is automatically sorted.  See the help text for the Automatic Sorting control for information regarding how automatic sorting affects performance.</p>

<p class="Body">By default, each time you add an object to the in-memory list using one of the Ini_Put functions, a check is made for items with the same name.  This can prevent unwanted duplicates, but it also can slow performance.  You can disable this feature using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.</p>

</body>
</html>    H    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_New</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>The handle to the object created in this function.<br><br>

0 is returned if there is not enough memory.<br><br>

Use this handle as the first parameter to all of the other functions in this instrument driver.</td>
</tr>
</table>


</body>
</html>    "    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_New</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">automaticSorting</td>
<td class="paramDataType">integer</td>

<td>If you pass a non-zero value, the in-memory list of tag/value pairs is maintained in sorted order.<br><br>

The sorting is based on section name, and tag name within section name.  The sorting is done on a case-insensitive basis.<br><br>

If you pass a zero value, the sections and tags are kept in the order created (or read from the file).<br><br>

You can dynamically change the sorting mode by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_Sort.htm')">Ini_Sort</a></span> or <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_DisableSorting.htm')">Ini_DisableSorting</a></span>.<br><br>


Effect of Sorting on Performance<br>
<hr>

If sorting is enabled, the following performance characteristics hold:<br><br>

  <ul><li> Getting values from the in-memory list is faster. </li> 

  <li> If checking for duplicates is enabled (the default),
    adding values to the in-memory list is faster than  
    it would be for a non-sorted list.</li>

  <li> If checking for duplicates is disabled, 
    adding values to the in-memory list is slower than
    it would be for a non-sorted list.  (See the 
    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.)</li></ul>
</td>
</tr>
</table>

</body>
</html>    5�����  x    Handle                          ���� � Q��                                           8 =           Automatic Sorting?                 	            �Note:  In CVI 4.0.1, checks for duplicate items were added in each Ini_Put function.
           This may slow performance.    You can disable this feature by using the 
           Ini_SetDuplicateChecking function.   Yes 1 No 0   �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_Dispose</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_Dispose</h1>

<p class="syntax">void Ini_Dispose (IniText handle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Disposes of the in-memory list of tag/value pairs identified by <strong>Handle</strong>.</p>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_Dispose</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>. It represents the list of in-memory tag/value pairs.<br><br>

Zero may be passed, and <span class="Monospace">Ini_Dispose</span> will return without doing anything.</td>
</tr>
</table>


</body>
</html>    B� = �  �  x    Handle                                b    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_Sort</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_Sort</h1>

<p class="syntax">void Ini_Sort (IniText handle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Sorts the tag/value pairs in the list identified by <strong>handle</strong>. The list automatically remains sorted until you call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_DisableSorting.htm')">Ini_DisableSorting</a></span>.</p>

<p class="Body">The sorting is based on section name, and tag name within section name.  The sorting is done on a case-insensitive basis.</p>

<p class="Body">If automatic sorting was specified in the call to <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span> when the tag/value list was created, then the list is already sorted and this function will do nothing.</p><br>


<p class="Body">Effect of Sorting on Performance</p>
<hr>

<p class="Body">If sorting is enabled, the following performance characteristics hold:</p>

  <ul><li>Getting values from the in-memory list is faster.</li>  

  <li>If checking for duplicates is enabled (the default),
    adding values to the in-memory list is faster than  
    it would be for a non-sorted list.</li>

  <li>If checking for duplicates is disabled, 
    adding values to the in-memory list is slower than
    it would be for a non-sorted list.  (See the 
    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.)</li></ul>






</body>
</html>    T    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_Sort</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>. It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    M4 = �  �  x    Handle                                �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_DisableSorting</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_DisableSorting</h1>

<p class="syntax">void Ini_DisableSorting (IniText handle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Disables automatic sorting of the in-memory list.  Does not affect the current order of the tag-value pairs in the list.</p>

<p class="Body">To reenable automatic sorting, call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_Sort.htm')">Ini_Sort</a></span>.</p><br>


<p class="Body">Effect of Sorting on Performance</p>
<hr>

<p class="Body">If sorting is enabled, the following performance characteristics hold:</p>

  <ul><li>Getting values from the in-memory list is faster.</li>  

  <li>If checking for duplicates is enabled (the default),
    adding values to the in-memory list is faster than  
    it would be for a non-sorted list.</li>

  <li>If checking for duplicates is disabled, 
    adding values to the in-memory list is slower than
    it would be for a non-sorted list.  (See the 
    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.)</li></ul>


</body>
</html>    ^    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_DisableSorting</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    U� = �  �  x    Handle                                
<    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SortInternally</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_SortInternally</h1>

<p class="syntax">int Ini_SortInternally (IniText handle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function gives the same performance benefits as <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_Sort.htm')">Ini_Sort</a></span>, but it does not alter the order of the tag/value pairs in the list identified by <strong>handle</strong>.</p> 

<p class="Body">When internal sorting is enabled by this function, an additional internal list of pointers to tag/value pairs is created, sorted, and used to speed up IniText operations.  The original list is maintained as well so that the order of the tag/value pairs in the list is preserved when the IniText is written to a file or traversed using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_NthItemName.htm')">Ini_NthItemName</a></span> and <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_NthSectionName.htm')">Ini_NthSectionName</a></span> functions. </p>  

<p class="Body">If the Ini_Text has enabled regular sorting by calling <span class="Monospace">Ini_Sort</span> or by passing TRUE to <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span> then enabling internal sorting with this function will not improve performance any further.</p>

<p class="Body">See the help for <span class="Monospace">Ini_Sort</span>.</p>


<p class="Body">Effect of Sorting on Performance</p>
<hr>

<p class="Body">If sorting is enabled, the following performance characteristics hold:</p>

  <ul>
 <li>Getting values from the in-memory list is faster.  </li>

 <li>If checking for duplicates is enabled (the default),
    adding values to the in-memory list is faster than  
    it would be for a non-sorted list.</li>

  <li>If checking for duplicates is disabled, 
    adding values to the in-memory list is slower than
    it would be for a non-sorted list.  (See the 
    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.)</li></ul>




</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SortInternally</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in userint.h.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

The only error you are likely to encounter is:<br><br>

<span class="nobreak">&ndash;12</span>&nbsp;&nbsp;&nbsp;Out of memory!</td>
</tr>
</table>

</body>
</html>
    _    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SortInternally</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    b~���    x    Status                            fj = �  �  x    Handle                             	               E    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_DisableInternalSorting</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_DisableInternalSorting</h1>

<p class="syntax">void Ini_DisableInternalSorting (IniText handle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Disables automatic internal sorting of the in-memory list. </p> 

<p class="Body">To reenable automatic internal sorting, call <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SortInternally.htm')">Ini_SortInternally</a></span>.</p>

<p class="Body">Refer to the help for <span class="Monospace">Ini_SortInternally</span> for information about internal sorting.</p><br>


<p class="Body">Effect of Sorting on Performance</p>
<hr>

<p class="Body">If sorting is enabled, the following performance characteristics hold:</p>

  <ul><li>Getting values from the in-memory list is faster.</li>  

  <li>If checking for duplicates is enabled (the default),
    adding values to the in-memory list is faster than  
    it would be for a non-sorted list.</li>

  <li>If checking for duplicates is disabled, 
    adding values to the in-memory list is slower than
    it would be for a non-sorted list.  (See the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.)</li></ul>




</body>
</html>    g    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_DisableInternalSorting</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    o� = �  �  x    Handle                                :    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteToFile</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_WriteToFile</h1>

<p class="syntax">int Ini_WriteToFile (IniText handle, const char pathname[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Writes the tag/value pairs in the list identified by <strong>handle</strong> to the file identified by <strong>pathname</strong>.</p>

<p class="Body">If you want to write the data to a destination other than a file, use the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_WriteGeneric.htm')">Ini_WriteGeneric</a></span> function.</p>


</body>
</html>        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteToFile</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

Among the more commonly encountered errors are:<br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
    <tr><td><span class="nobreak">&ndash;91</span></td>  <td>Too many files are open.</td></tr>
    <tr><td><span class="nobreak">&ndash;93</span></td>  <td>Input/Output error.</td></tr>
    <tr><td><span class="nobreak">&ndash;94</span></td>  <td>File not found.</td></tr>
    <tr><td><span class="nobreak">&ndash;95</span></td>  <td>File access permission denied.</td></tr>
    <tr><td><span class="nobreak">&ndash;97</span></td>  <td>Disk is full.</td></tr>
    <tr><td><span class="nobreak">&ndash;99</span></td>  <td>File already open.</td></tr>
   <tr><td><span class="nobreak">&ndash;100</span></td>  <td>Badly formed pathname.</td></tr>
  <tr><td><span class="nobreak">&ndash;5003</span></td>  <td>Could not generate an unused temporary file name in the
         same directory as the output file.</td></tr>
  <tr><td><span class="nobreak">&ndash;5004</span></td>  <td>Could not create a temporary file in the same 
         directory as the output file.</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>    \    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteToFile</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteToFile</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">pathname</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The pathname of the file to which to write the tag/value pairs.
If a file with that pathname already exists, it will be overwritten.<br><br>

If the pathname is not absolute, the file is created relative to the current working directory.<br><br>

Pass 0 to write the tag/value pairs to the standard output stream.</td>
</tr>
</table>

</body>
</html>    u����    x    Status                            }� = d  �  x    Handle                            � =! �  �    Pathname                           	                ""   Q    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadFromFile</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_ReadFromFile</h1>

<p class="syntax">int Ini_ReadFromFile (IniText handle, const char pathname[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Reads the tag/value pairs in the file identified by <strong>pathname</strong> into the list identified by <strong>handle</strong>.</p>

<p class="Body">If you want to read the data from a source other than a file or the Windows Registry, use the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_ReadGeneric.htm')">Ini_ReadGeneric</a></span> function.</p>



</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadFromFile</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

Among the more commonly encountered errors are:<br>
    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
    <tr><td><span class="nobreak">&ndash;91</span></td>  <td>Too many files are open.</td></tr>
    <tr><td><span class="nobreak">&ndash;92</span></td>  <td>Unexpectedly reached end of file.</td></tr>
    <tr><td><span class="nobreak">&ndash;93</span></td>  <td>Input/Output error.</td></tr>
    <tr><td><span class="nobreak">&ndash;94</span></td>  <td>File not found.</td></tr>
    <tr><td><span class="nobreak">&ndash;95</span></td>  <td>File access permission denied.</td></tr>
    <tr><td><span class="nobreak">&ndash;99</span></td>  <td>File already open.</td></tr>
   <tr><td><span class="nobreak">&ndash;100</span></td>  <td>Badly formed pathname.</td></tr>
  <tr><td><span class="nobreak">&ndash;5001</span></td>  <td>Could not open file for reading</td></tr>
  <tr><td><span class="nobreak">&ndash;5002</span></td>  <td>Error reading file</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>    ]    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadFromFile</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    :    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadFromFile</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">pathname</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The pathname of the file from which to read the tag/value pairs.<br><br>

If the pathname is not absolute, the file is located relative to the current working directory.</td>
</tr>
</table>

</body>
</html>    �����    x    Status                            �p = d  �  x    Handle                            �� =! �  �    Pathname                           	                ""   �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteToRegistry</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_WriteToRegistry</h1>

<p class="syntax">int Ini_WriteToRegistry (IniText handle, int rootKey, const char subkeyName[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Writes the tag/value pairs in the list identified by <strong>handle</strong> to the specified Windows Registry Key.  All values are written out as strings.</p>

<p class="Body">If you want to write the data to a destination other than a file or the Windows Registry, use the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_WriteGeneric.htm')">Ini_WriteGeneric</a></span> function.</p>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteToRegistry</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

Among the more commonly encountered errors are:<br><br>
    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
    <tr><td><span class="nobreak">&ndash;91</span></td>  <td>Too many files are open.</td></tr>
    <tr><td><span class="nobreak">&ndash;93</span></td>  <td>Input/Output error.</td></tr>
    <tr><td><span class="nobreak">&ndash;94</span></td>  <td>File not found.</td></tr>
    <tr><td><span class="nobreak">&ndash;95</span></td>  <td>File access permission denied.</td></tr>
    <tr><td><span class="nobreak">&ndash;99</span></td>  <td>File already open.</td></tr>
   <tr><td><span class="nobreak">&ndash;100</span></td>  <td>Badly formed pathname.</td></tr>
  <tr><td><span class="nobreak">&ndash;5003</span></td>  <td>Could not generate an unused temporary file name in the
         same directory as the output file.</td></tr>
  <tr><td><span class="nobreak">&ndash;5004</span></td>  <td>Could not create a temporary file in the same 
         directory as the output file.</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>


    _    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteToRegistry</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    8    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteToRegistry  </title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">rootKey</td>
<td class="paramDataType">integer</td>

<td>The Root Key in the Registry to write to.  The available Root Keys are:<br><br>

    <p class="Body"><span class="Monospace">
    HKEY_CLASSES_ROOT<br>
    HKEY_CURRENT_USER<br>
    HKEY_LOCAL_MACHINE<br>
    HKEY_USERS</span><br><br>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;Applications typically store global configuration data
       under:<br><br>

<span class="Monospace">HKEY_LOCAL_MACHINE\\Software\\CompanyName\\AppName\\Version\\</span><br><br>

And per-user configuration data under:<br><br>

<span class="Monospace">HKEY_CURRENT_USER\\Software\\CompanyName\\AppName\\Version\\</span></td></tr></table>
</td>
</tr>
</table>
</body>
</html>    P    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteToRegistry</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">subkeyName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>A NULL-terminated string containing the name of the Registry Subkey to which to write tag/value pairs.  The Subkey should be '\' separated and relative to the specified Root Key.<br><br>

The following is an example of a valid Subkey string for the Root Key <span class="Monospace">HKEY_CURRENT_USER</span>:<br><br>

<span class="Monospace">"Software\\National Instruments\\CVI\\5.0"</span><br><br>

CVI stores its configuration under this subkey.  Do not store your program's information under this key.  Use the following convention to name the subkey under which to store your program's information:<br><br>

<span class="Monospace">"Software\\Company Name\\Program Name\\Program Version"</span></td>
</tr>
</table>

</body>
</html>    �[���    x    Status                            � = d  �  x    Handle                            �� =!    �    Root Key                          �� � � �  �    Subkey Name                     ���� � ���                                            	                          JHKEY_CLASSES_ROOT 0 HKEY_CURRENT_USER 1 HKEY_LOCAL_MACHINE 2 HKEY_USERS 3        7This function is available only on Windows 9x/NT/2000.   O    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadFromRegistry</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_ReadFromRegistry</h1>

<p class="syntax">int Ini_ReadFromRegistry (IniText handle, int rootKey, const char subkeyName[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Reads tag/value pairs from the Windows Registry into the list identified by <strong>handle</strong>.</p>

<p class="Body">If you want to read the data from a source other than a file or the Windows Registry, use the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_ReadGeneric.htm')">Ini_ReadGeneric</a></span> function.</p>

</body>
</html>


    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadFromRegistry</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

Among the more commonly encountered errors are:<br>
    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
    <tr><td><span class="nobreak">&ndash;91</span></td>  <td>Too many files are open.</td></tr>
    <tr><td><span class="nobreak">&ndash;92</span></td>  <td>Unexpectedly reached end of file.</td></tr>
    <tr><td><span class="nobreak">&ndash;93</span></td>  <td>Input/Output error.</td></tr>
    <tr><td><span class="nobreak">&ndash;94</span></td>  <td>File not found.</td></tr>
    <tr><td><span class="nobreak">&ndash;95</span></td>  <td>File access permission denied.</td></tr>
    <tr><td><span class="nobreak">&ndash;99</span></td>  <td>File already open.</td></tr>
   <tr><td><span class="nobreak">&ndash;100</span></td>  <td>Badly formed pathname.</td></tr>
  <tr><td><span class="nobreak">&ndash;5001</span></td>  <td>Could not open file for reading</td></tr>
  <tr><td><span class="nobreak">&ndash;5002</span></td>  <td>Error reading file</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>

    b    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadFromRegistry</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>



</body>
</html>    :    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadFromRegistry</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">rootKey</td>
<td class="paramDataType">integer</td>

<td>The Root Key in the Registry from which to read.  The available Root Keys are:<br><br>

    <span class="Monospace">
    HKEY_CLASSES_ROOT<br>
    HKEY_CURRENT_USER<br>
    HKEY_LOCAL_MACHINE<br>
    HKEY_USERS</span><br>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;Applications typically store global configuration data
       under:<br><br>

  <span class="Monospace">HKEY_LOCAL_MACHINE\\Software\\CompanyName\\AppName\\Version\\</span><br><br>

       And per-user configuration data under:<br><br>

  <span class="Monospace">HKEY_CURRENT_USER\\Software\\CompanyName\\AppName\\Version\\</span></td></tr></table>

</td>
</tr>
</table>

</body>
</html>


        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadFromRegistry</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">subkeyName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>A NULL-terminated string containing the name of the Registry Subkey from which to read tag/value pairs.  The Subkey should be '\' separated and relative to the specified Root Key.<br><br>

The following is an example of a valid Subkey string for the Root Key <span class="Monospace">HKEY_CURRENT_USER:</span><br><br>

<span class="Monospace">"Software\\National Instruments\\CVI\\5.0"</span></td>
</tr>
</table>

</body>
</html>    �%���    x    Status                          ���� � ���                                           �� = d  �  x    Handle                            �1 =!    �    Root Key                          �s � � �  �    Subkey Name                        	            7This function is available only on Windows 9x/NT/2000.                  JHKEY_CLASSES_ROOT 0 HKEY_CURRENT_USER 1 HKEY_LOCAL_MACHINE 2 HKEY_USERS 3       $    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_WriteGeneric</h1>

<p class="syntax">int Ini_WriteGeneric (IniText handle, Ini_OutputFunc outputFunction, void *outputDestination);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Writes the tag/value pairs in the list identified by <strong>handle</strong> to the specified <strong>outputDestination</strong> by calling the specified <strong>outputFunction</strong>.</p>

<p class="Body">This function allows you to write the in-memory contents to any destination, such as a memory mapped file or a TCP/IP address.</p>

<p class="Body">If you want to write to a disk file, you can use the function <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_WriteToFile.htm')">Ini_WriteToFile</a></span>.</td>
</tr>
</table>

</body>
</html>
    -    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

If the specified output function returns a negative value, the value is returned by <span class="Monospace">Ini_WriteGeneric</span>.<br><br>

Among the more commonly encountered errors are:<br>
    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
    <tr><td><span class="nobreak">&ndash;91</span></td>  <td>Too many files are open.</td></tr>
    <tr><td><span class="nobreak">&ndash;93</span></td>  <td>Input/Output error.</td></tr>
    <tr><td><span class="nobreak">&ndash;94</span></td>  <td>File not found.</td></tr>
    <tr><td><span class="nobreak">&ndash;95</span></td>  <td>File access permission denied.</td></tr>
    <tr><td><span class="nobreak">&ndash;99</span></td>  <td>File already open.</td></tr>
   <tr><td><span class="nobreak">&ndash;100</span></td>  <td>Badly formed pathname.</td></tr>
  <tr><td><span class="nobreak">&ndash;5003</span></td>  <td>Could not generate an unused temporary file name in the same directory as the output file.</td></tr>
  <tr><td><span class="nobreak">&ndash;5004</span></td>  <td>Could not create a temporary file in the same directory as the output file.</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>    ]    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    7    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">outputFunction</td>
<td class="paramDataType">Ini_OutputFunc</td>

<td>A pointer to a callback function that is called to write out each line of the IniText.<br><br>

The prototype for the callback function is:<br><br>

   <p class="Code">int CVICALLBACK OutputFunc(void *outputDest, 
                              char *outputString);</p> 

The callback function should return 0 if it successfully writes the <strong>outputString</strong> parameter.  Otherwise, it should return a negative error code, which is then returned by <span class="Monospace">Ini_WriteGeneric</span>.<br><br>

Each call to the function represents one line.  It is the responsibility of the function to mark the end of the line in a manner appropriate to the destination type.</td>
</tr>
</table>

</body>
</html>    _    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_WriteGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">outputDestination</td>
<td class="paramDataType">void&nbsp;*</td>

<td>A value representing the output destination.  This value is passed to the specified output function.<br>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;   
  This value can be used pass a pointer to a structure which
  contains the information necessary for the output function to
  write to a particular destination, such as a memory mapped 
  file or a TCP/IP address.  </td></tr></table>

</td>
</tr>
</table>

</body>
</html>
    �u���    x    Status                            Ϊ = d  �  x    Handle                            � =! �  �    Output Function                   �N � �    �    Output Destination                 	                           <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_ReadGeneric</h1>

<p class="syntax">int Ini_ReadGeneric (IniText handle, Ini_InputFunc inputFunction, void *inputSource);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Reads the tag/value pairs into the list identified by <strong>handle</strong> from the specified <strong>inputSource</strong> by calling the specified <strong>inputFunction</strong>.</p>

<p class="Body">This function allows you to read into memory data from any source, such as a memory mapped file or a TCP/IP address.</p>

<p class="Body">If you want to read from a disk file, you can use the function <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_ReadFromFile.htm')">Ini_ReadFromFile</a></span>.</p>



</body>
</html>

    \    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    !    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

If the specified input function returns a negative value, the value is returned by <span class="Monospace">Ini_ReadGeneric</span>.<br><br>

Among the more commonly encountered errors are:<br>
    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
    <tr><td><span class="nobreak">&ndash;91</span></td>  <td>Too many files are open.</td></tr>
    <tr><td><span class="nobreak">&ndash;92</span></td>  <td>Unexpectedly reached end of file.</td></tr>
    <tr><td><span class="nobreak">&ndash;93</span></td>  <td>Input/Output error.</td></tr>
    <tr><td><span class="nobreak">&ndash;94</span></td>  <td>File not found.</td></tr>
    <tr><td><span class="nobreak">&ndash;95</span></td>  <td>File access permission denied.</td></tr>
    <tr><td><span class="nobreak">&ndash;99</span></td>  <td>File already open.</td></tr>
   <tr><td><span class="nobreak">&ndash;100</span></td>  <td>Badly formed pathname.</td></tr>
  <tr><td><span class="nobreak">&ndash;5001</span></td>  <td>Could not open file for reading</td></tr>
  <tr><td><span class="nobreak">&ndash;5002</span></td>  <td>Error reading file</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>

    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">inputFunction</td>
<td class="paramDataType">Ini_InputFunc</td>

<td>A pointer to a callback function that is called to read in each line of the IniText.<br><br>

The prototype for the callback function is:<br><br>

   <p class="code">
int CVICALLBACK InputFunc(void *inputSource, 
                          char *inputBuffer, int maxLineLength,
                          int *endOfSource, int *lineNumRead);</p><br>

The callback function should return 0 if it successfully reads a line of text. Otherwise, it should return a negative error code, which is then returned by <span class="Monospace">Ini_ReadGeneric</span>.<br><br>

The callback function should copy the line that it reads into the <strong>inputBuffer</strong> parameter as a NULL terminated string.  The <strong>maxLineLength</strong> parameter specifies the maximum number of characters (not including the NULL byte) that can be safely copied into the <strong>inputBuffer</strong>.  If your <strong>inputFunction</strong> encounters a line longer than this limit, it can return a negative error code or it can discard the extra characters.  The default maximum line length is 255, which is specified as a macro in <span class="Monospace">inifile.c</span>.<br><br>

The callback function should set the <strong>*endOfSource</strong> parameter to a non-zero value if there may be further lines to read.<br><br>

The callback function should set the <strong>*lineNumRead</strong> parameter to the 1&ndash;based index of the line that was read.  If this information is not needed, <strong>*lineNumRead</strong> can be set to zero.
</td>
</tr>
</table>

</body>
</html>
    N    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ReadGeneric</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">inputSource</td>
<td class="paramDataType">void&nbsp;*</td>

<td>A value representing the input source.  This value is passed to the specified input function.<br>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;   
  This value can be used to pass a pointer to a structure which
  contains the information necessary for the input function to
  read from a particular source, such as a memory mapped file
  or a TCP/IP address.  </td></tr></table>

</td>
</tr>
</table>

</body>
</html>
    ݪ = d  �  x    Handle                            ����    x    Status                            �7 =! �  �    Input Function                    �� � �    �    Input Source                           	                   &    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_CopySection</h1>

<p class="syntax">int Ini_CopySection (IniText source, const char sectionToCopy[], IniText destination, int overwriteDuplicateSections);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function copies a section of tag-value pairs from one in-memory list to another.</p>  

<p class="Body">If there already is a section of the same name in the Destination list, you have the option of overwriting the section or merging the two sections.  If you merge the two sections, items with the same tags are overwritten unless you have disabled duplicate checking using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.
</p>


</body>
</html>

    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">source</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.<br><br>

This list will be copied into the list specified by the <strong>destination</strong> parameter.</td>
</tr>
</table>

</body>
</html>
    9    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function is successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

The only error you are likely to encounter is:<br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
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
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">destination</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.<br><br>

The contents of the Source list will be copied into this list.</td>
</tr>
</table>

</body>
</html>
    {    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">overwriteDuplicateSections</td>
<td class="paramDataType">integer</td>

<td>This parameter specifies whether sections in the Source list completely overwrite previously existing sections with the same name in the Destination list.<br><br>

If there a section in the Destination list has the same name as a section being copied, you have the option of overwriting the section or merging the two sections. If you merge the two sections, items with the same tags are overwritten unless you have disabled duplicate checking using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.<br><br>

Pass 1 (TRUE) to completely overwrite previously existing sections in the Destination.<br><br>

Pass 0 (FALSE) to merge the items in sections with same name into the Destination list. Items with the same tag names are overwritten unless you have disabled duplicate checking using the <span class="Monospace">Ini_SetDuplicateChecking</span> function.</td>
</tr>
</table>

</body>
</html>
    T    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionToCopy</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>This parameter specifies the name of the Source list section  to merge into the Destination List.<br><br>

Pass 0 to merge all of the sections in the Source list into the Destination list.</td>
</tr>
</table>


</body>
</html>

    �0 = d  �  x    Source                            �����    x    Status                            �> � d �  x    Destination                      � �a          Overwrite Duplicate Sections?    r =! �  �    Section To Copy                        	               Yes 1 No 0    0   C    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySectionEx</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_CopySectionEx</h1>

<p class="syntax">int Ini_CopySectionEx (IniText source, const char sectionToCopy[], IniText destination, const char destinationSectionToCopyTo[] overwriteDuplicateSections);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function copies a whole section from one in-memory list to another.</p>  

<p class="Body">If there already is a section of the same name in the Destination list, you have the option of overwriting the section or merging the two sections.  If you merge the two sections, items with the same tags are overwritten unless you have disabled duplicate checking using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.
</p>


</body>
</html>

    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">source</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.<br><br>

This list will be copied into the list specified by the <strong>destination</strong> parameter.</td>
</tr>
</table>

</body>
</html>
    :    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

The only error you are likely to encounter is:<br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
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
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">destination</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.<br><br>

The contents of the Source list will be copied into this list.</td>
</tr>
</table>

</body>
</html>
    {    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">overwriteDuplicateSections</td>
<td class="paramDataType">integer</td>

<td>This parameter specifies whether sections in the Source list completely overwrite previously existing sections with the same name in the Destination list.<br><br>

If there a section in the Destination list has the same name as a section being copied, you have the option of overwriting the section or merging the two sections. If you merge the two sections, items with the same tags are overwritten unless you have disabled duplicate checking using the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span> function.<br><br>

Pass 1 (TRUE) to completely overwrite previously existing sections in the Destination.<br><br>

Pass 0 (FALSE) to merge the items in sections with same name into the Destination list. Items with the same tag names are overwritten unless you have disabled duplicate checking using the <span class="Monospace">Ini_SetDuplicateChecking</span> function.</td>
</tr>
</table>

</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionToCopy</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>This parameter specifies the name of the Source list section to merge into the Destination List.<br><br>
</td>
</tr>
</table>


</body>
</html>

        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_CopySection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">destinationSectionToCopyTo</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>This parameter specifies the name of the Destination list section into which to write.<br><br>
</td>
</tr>
</table>


</body>
</html>

   I = d  �  x    Source                           ���    x    Status                           X � d �  x    Destination                      	 � �          Overwrite Duplicate Sections?    � =! �  �    Section To Copy                   � ! �  �    Destination Section To Copy To         	               Yes 1 No 0           N    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetTokens</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_SetTokens</h1>

<p class="syntax">int Ini_SetTokens (IniText handle, char sectionNameStartCharacter, char sectionNameEndCharacter, const char valueSeparator[], const char commentMarker[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function specifies the tokens used as delimiters in the text representation of the sections headings and tag/value pairs.</p>

<p class="Body">If you do not call this function, the default tokens are:</p>

    <table class="Borderless">
    <tr><td>Section Name Start Character</td>    <td>&quot;&#91;&quot;</td></tr>
    <tr><td>Section Name End Character</td>      <td>&quot;&#93;&quot;</td></tr>
    <tr><td>Value Separator</td>                 <td>&quot;&#61;&quot;</td></tr>
    <tr><td>Comment Marker</td>                 <td>&quot;&#59;&quot;</td></tr>
    </table>




</body>
</html>



 

    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetTokens</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>This function always returns 0.</td>
</tr>
</table>

</body>
</html>
    Y    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetTokens</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetTokens</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionNameStartCharacter</td>
<td class="paramDataType">char</td>

<td>The character used to delimit the start of a section name.<br><br>

Pass 0 to leave the current delimiter unchanged.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetTokens</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionNameEndCharacter</td>
<td class="paramDataType">char</td>

<td>The character used to delimit the end of a section name.<br><br>

Pass 0 to leave the current delimiter unchanged.</td>
</tr>
</table>

</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetTokens</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">valueSeparator</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The string used to separate item tag names from values.<br><br>

Pass 0 to leave the current delimiter unchanged.</td>
</tr>
</table>

</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetTokens</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">commentMarker</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>Pass the string used to mark the beginning of a comment line.<br><br>

Pass 0 to leave the current delimiter unchanged.</td>
</tr>
</table>

</body>
</html>
   (_���    x    Status                           * = 9  �  x    Handle                           ,d = �    x    Section Name Start Character     .m =s    x    Section Name End Character       0r � 9 �  x    Value Separator                  2� � � �  x    Comment Marker                     	                '['    ']'    "="    ";"   o    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetSectionFilter</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_SetSectionFilter</h1>

<p class="syntax">int Ini_SetSectionFilter (IniText handle, Ini_SectionFilterFunc sectionFilterFunction, void *callbackData);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function installs a callback function that controls which sections are read into the IniText during a call to <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_ReadFromFile.htm')">Ini_ReadFromFile</a></span> or <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_ReadGeneric.htm')">Ini_ReadGeneric</a></span>.</p>

<p class="Body">This filtering callback function is useful when you want to read a subset of a large <span class="Monospace">.ini</span> file without the overhead of reading the entire file into memory.</p>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetSectionFilter</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>This function always returns 0.</td>
</tr>
</table>

</body>
</html>
    a    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetSectionFilter</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    }    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetSectionFilter</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionFilterFunction</td>
<td class="paramDataType">Ini_SectionFilterFunc</td>

<td>A pointer to a callback function that is called when each section name is read.  The value returned by this callback function determines whether the section is read into the IniText or discarded.<br><br>

The prototype for the callback function is:<br><br>

   <p class="code">int CVICALLBACK FilterFunction(IniText theIniText, void *callbackData, char *sectionName);</p><br>

The Filter Function should return 1 (TRUE) if the section is to be read into the IniText.  Otherwise, it should return 0 (FALSE).<br>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;You can pass 0 to remove a previously installed  
       Section Filter Function.
</td></tr></table>

</td>
</tr>
</table>

</body>
</html>
        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetSectionFilter</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">callbackData</td>
<td class="paramDataType">void&nbsp;*</td>

<td>A user defined value that is passed to the Section Filter Function in its <strong>callbackData</strong> parameter each time it is called.</td>
</tr>
</table>


</body>
</html>
   :t���    x    Status                           < = n  �  x    Handle                           >� = �  �    Section Filter Function          C � �    �    Callback Data                      	                    0   �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetDuplicateChecking</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_SetDuplicateChecking</h1>

<p class="syntax">int Ini_SetDuplicateChecking (IniText handle, int checkForDuplicates);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">By default, when you use one of the <span class="Monospace">Ini_Put</span> functions to add an item to the Ini_Text, a check is made for an existing item with a duplicate name.  If there is an existing item with a duplicate name, it is deleted.  This check can slow down the <span class="Monospace">Ini_Put</span> functions.  You can use this function to disable or reenable the duplicate checks.</p>

<p class="Body">When duplicate checking is on (the default setting), putting an item automatically replaces it if it already exists.</p>

<p class="Body">When duplicate checking is off, items are put without checking to see if they already exist.  This makes it possible for multiple items with the same name to be put into an IniText.  If more than one item has the same name, which value is returned by an <span class="Monospace">Ini_Get</span> function is undefined.</p>

<p class="Body">The only time this function should be used is when you want to speed up putting a large number of items by bypassing the check for duplicate items.  If you turn duplicate checking off, you are responsible for ensuring that no item is put into an IniText which already contains an item with the same name.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;The <span class="Monospace">Ini_Merge</span> function relies on the <span class="Monospace">Ini_Put</span> functions and is therefore subject the duplicate checking state of its Destination argument.</td></tr></table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetDuplicateChecking</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">previousSetting</td>
<td class="paramDataType">integer</td>

<td>Returns the previous duplicate checking setting for the IniText.</td>
</tr>
</table>

</body>
</html>
    d    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetDuplicateChecking</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SetDuplicateChecking</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">checkForDuplicates</td>
<td class="paramDataType">integer</td>

<td>This parameter specifies whether the <span class="Monospace">Ini_Put</span> functions check for and delete existing items with duplicate names before adding an item to the Ini_Text.<br><br>

Pass 1 to check for duplicates.<br><br>

Pass 0 to disable duplicate checking.</td>
</tr>
</table>

</body>
</html>
   M����    x    Previous Setting                 O� = �  �  x    Handle                           R0 =S          Check For Duplicates               	                Yes 1 No 0   	�    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_PutString</h1>

<p class="syntax">int Ini_PutString (IniText handle, const char sectionName[], const char tagName[], const char stringValue[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Adds a tag/string pair to the tag/value list identified by <strong>handle</strong>.  The string is associated with the tag identified by <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

<p class="Body">If the string is very long, the instrument driver will split it across multiple lines associated with the same tag name.  This is done by generating new tag names, such as in:</p>

   <table class="Borderless">
   <tr><td>&lt;Tag Name&gt; Line 0001</td> <td>&#61; &quot;first part of line ........&quot;</td></tr>
   <tr><td>&lt;Tag Name&gt; Line 0002</td> <td>&#61; &quot;second part of line .......&quot;</td></tr>
   <tr><td>&lt;Tag Name&gt; Line 0003</td> <td>&#61; &quot;third part of line&quot;</td></tr>
   </table>
<p class="Body">When you attempt to retrieve the string from the file by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetPointerToString.htm')">Ini_GetPointerToString</a></span>, <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringCopy.htm')">Ini_GetStringCopy</a></span>, or <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringIntoBuffer.htm')">Ini_GetStringIntoBuffer</a></span>, you only need to enter the same Tag Name that you passed into this function.  The instrument driver takes care of reconstructing the original single string from the  multiple lines.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;By default, the function checks for and deletes existing items with duplicate names.  You can disable the duplicate checking, and thereby increase performance, by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span>.</td></tr></table>


</body>
</html>
    7    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

The only error you are likely to encounter is:<br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>    Y    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to place the tag/value pair.</td>
</tr>
</table>


</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name to use in the tag/value pair.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">stringValue</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The string to associate with the tag name.</td>
</tr>
</table>

</body>
</html>
   _l���    x    Status                           c� =   �  x    Handle                           f = � �  �    Section Name                     g� =f �  �    Tag Name                         i� �  �  �    String Value                       	                ""    ""    ""   �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_PutRawString</h1>

<p class="syntax">int Ini_PutRawString (IniText handle, const char sectionName[], const char tagName[], const char stringValue[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Adds a tag/string pair to the tag/value list identified by <strong>handle</strong>.  The string is associated with the tag identified by <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>


<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;This function is exactly like <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_PutString.htm')">Ini_PutString</a></span> except that unprintable characters in the string are not translated into escape codes.  One possible use for this function is to write out a pathname in PC format.  Since PC pathnames contain backslashes and since the escape character used by <span class="Monospace">Ini_PutString</span> is also the backslash, you must use <span class="Monospace">Ini_PutRawString</span> if you do not want the back slashes in the pathname to be converted into double backslashes in the destination <span class="Monospace">.ini</span> file.  This is only an issue if you are using the inifile instrument to create entries that will not be read by one of the <span class="Monospace">Ini_GetString</span> functions in this instrument.  The non-raw versions of the <span class="Monospace">Ini_GetString</span> functions automatically recognize and remove the escape codes inserted by <span class="Monospace">Ini_PutString</span>.</td></tr></table>

<p class="Body">If the string is very long, the instrument driver will split it across multiple lines associated with the same tag name.  This is done by generating new tag names, such as in:</p>

   <table class="Borderless">
   <tr><td>&lt;Tag Name&gt; Line 0001</td> <td>&#61; &quot;first part of line ........&quot;</td></tr>
   <tr><td>&lt;Tag Name&gt; Line 0002</td> <td>&#61; &quot;second part of line .......&quot;</td></tr>
   <tr><td>&lt;Tag Name&gt; Line 0003</td> <td>&#61; &quot;third part of line&quot;</td></tr></table>
   
<p class="Body">When you attempt to retrieve the string from the file by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetPointerToString.htm')">Ini_GetPointerToString</a></span>, <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringCopy.htm')">Ini_GetStringCopy</a></span>, or <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringIntoBuffer.htm')">Ini_GetStringIntoBuffer</a></span>, you only need to enter the same Tag Name that you passed into this function.  The instrument driver takes care of reconstructing the original single string from the  multiple lines.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;By default, the function checks for and deletes existing items with duplicate names.  You can disable the duplicate checking, and thereby increase performance, by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span>.
</td></tr></table>




</body>
</html>

    Q    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br><br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>    \    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to place the tag/value pair.</td>
</tr>
</table>

</body>
</html>

    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name to use in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">stringValue</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The string to associate with the tag name.</td>
</tr>
</table>

</body>
</html>

   {E���    x    Status                           � =   �  x    Handle                           � = � �  �    Section Name                     �� =f �  �    Tag Name                         �� �  �  �    String Value                       	                ""    ""    ""   �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_PutInt</h1>

<p class="syntax">int Ini_PutInt (IniText handle, const char sectionName[], const char tagName[], int integerValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Adds a tag/integer pair to the tag/value list identified by <strong>handle</strong>.  The integer value is associated with the tag identified by <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;By default, the function checks for and deletes existing items with duplicate names.  You can disable the duplicate checking, and thereby increase performance, by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span>.
</td></tr></table>


</body>
</html>


    I    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>

    V    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to place the tag/value pair.</td>
</tr>
</table>

</body>
</html>

    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name to use in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">integerValue</td>
<td class="paramDataType">integer</td>

<td>The integer value to associate with the Tag Name.</td>
</tr>
</table>


</body>
</html>   �/���    x    Status                           �� =   �  x    Handle                           �� = � �  �    Section Name                     �� =f �  �    Tag Name                         �k �      x    Integer Value                      	                ""    ""       �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_PutUInt</h1>

<p class="syntax">int Ini_PutUInt (IniText handle, const char sectionName[], const char tagName[], unsigned int unsignedIntegerValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Adds a tag/unsigned integer pair to the tag/value list identified by <strong>handle</strong>.  The unsigned integer value is associated with the tag identified by <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;By default, the function checks for and deletes existing items with duplicate names.  You can disable the duplicate checking, and thereby increase performance, by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span>.</td></tr></table>



</body>
</html>




    X    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to place the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name to use in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    H    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">unsignedIntegerValue</td>
<td class="paramDataType">unsigned&nbsp;integer</td>

<td>The unsigned integer value to associate with the Tag Name.</td>
</tr>
</table>

</body>
</html>   � =   �  x    Handle                           �v = � �  �    Section Name                     �F =f �  �    Tag Name                         ����    x    Status                           �S �     x    Unsigned Integer Value                 ""    ""    	               �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_PutDouble</h1>

<p class="syntax">int Ini_PutDouble (IniText handle, const char sectionName[], const char tagName[], double doubleValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Adds a tag/double pair to the tag/value list identified by <strong>handle</strong>.  The double value is associated with the tag identified by <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;By default, the function checks for and deletes existing items with duplicate names.  You can disable the duplicate checking, and thereby increase performance, by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span>.</td></tr></table>

</body>
</html>


    Y    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to place the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name to use in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    J    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">doubleValue</td>
<td class="paramDataType">double</td>

<td>The double value to associate with the Tag Name.</td>
</tr>
</table>

</body>
</html>   �� =   �  x    Handle                           �] = � �  �    Section Name                     �/ =f �  �    Tag Name                         �����    x    Status                           �@ �     x    Double Value                           ""    ""    	               9    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_PutBoolean</h1>

<p class="syntax">int Ini_PutBoolean (IniText handle, const char sectionName[], const char tagName[], int booleanValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Adds a tag/boolean pair to the tag/value list identified by Handle.  The boolean value is associated with the tag identified by <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.  If the boolean value passed is non-zero, &quot;True&quot; is associated with the tag.  Otherwise, &quot;False&quot; is associated with the tag.</p>

</body>
</html>    [    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to place the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name to use in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    L    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>
    L    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">booleanValue</td>
<td class="paramDataType">integer</td>

<td>The boolean value to associate with the Tag Name.  
If the value is non-zero, &quot;True&quot; is associated with the Tag Name.  If the value is zero, &quot;False&quot; is associated with the Tag Name.</td>
</tr>
</table>

</body>
</html>   �b =   �  x    Handle                           �� = � �  �    Section Name                     Ř =f �  �    Tag Name                         �X���    x    Status                           ˬ � C          Boolean Value                          ""    ""    	           True 1 False 0   
    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_PutData</h1>

<p class="syntax">int Ini_PutData (IniText handle, const char sectionName[], const char tagName[], const unsigned char data[], long dataSize);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function stores the specified block of bytes in the tag/value list identified by <strong>handle</strong>.</p>

<p class="Body">The data block can be any length greater than zero bytes and can contain any byte values.</p>

<p class="Body">The data is associated with the tag identified by <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

<p class="Body">The bytes in the data block are converted to printable ASCII characters before they are added to the IniText.  <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetData.htm')">Ini_GetData</a></span> automatically converts the ASCII characters back to their original byte values.</p>

<p class="Body">If the data is very long, the instrument driver will split it across multiple lines associated with the same tag name.  This is done by generating new tag names, such as in:</p>

   <table class="Borderless">
   <tr><td>&lt;Tag Name&gt; Line 0001</td> <td>&#61; &quot;first part of data ........&quot;</td></tr>
   <tr><td>&lt;Tag Name&gt; Line 0002</td> <td>&#61; &quot;second part of data .......&quot;</td></tr>
   <tr><td>&lt;Tag Name&gt; Line 0003</td> <td>&#61; &quot;third part of data&quot;</td></tr></table>
   
<p class="Body">When you attempt to retrieve the data from the file by calling <span class="Monospace">Ini_GetData</span>, you only need to enter the same Tag Name that you passed into this function.  The instrument driver takes care of reconstructing the original single block of data from the  multiple lines.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;By default, the function checks for and deletes existing items with duplicate names.  You can disable the duplicate checking, and thereby increase performance, by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_SetDuplicateChecking.htm')">Ini_SetDuplicateChecking</a></span>.</td></tr></table>


</body>
</html>    Z    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to place the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name to use in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">data</td>
<td class="paramDataType">const&nbsp;unsigned&nbsp;char&nbsp;[]</td>

<td>A pointer to the block of bytes to store with the tag name.</td>
</tr>
</table>

</body>
</html>      H    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function was successful.<br><br>

A zero indicates success.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

    <table class="Borderless-Wide">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
</table>

</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_PutData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">dataSize</td>
<td class="paramDataType">long</td>

<td>The number of bytes to store with the tag name.</td>
</tr>
</table>

</body>
</html>   �F =   �  x    Handle                           ۨ = � �  �    Section Name                     �x =f �  �    Tag Name                         �5 �  �  x    Data                             ����    x    Status                           �` � �    x    Data Size                              ""    ""        	                   <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetPointerToString</h1>

<p class="syntax">int Ini_GetPointerToString (IniText handle, const char sectionName[], const char tagName[], char **pointerToString);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains a pointer to the string associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

<p class="Body">Do NOT modify the string or attempt to free the pointer.</p>

<p class="Body">The contents pointed to by the pointer are valid only until the next call to<br><br>

    <span class="Monospace">Ini_GetPointerToString</span>,<br> 
     <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringCopy.htm')">Ini_GetStringCopy</a></span>, or<br>
    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringIntoBuffer.htm')">Ini_GetStringIntoBuffer</a></span><br><br>

on the same in-memory list of tag/value pairs.</p>

</body>
</html>    c    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    L    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a tag/string pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/string pair was found.  If the string portion of the tag/string pair was empty, such as in<br><br>

    tag 1 &#61; &quot;&quot;<br><br>

the string is considered to have been found, and it is the empty string &#40;&quot;&quot;&#41;.  If the tag's value is empty, such as in <br><br>

    tag 1 &#61; <br><br>

no tag/value pair is found.<br><br>

A zero indicates that no tag/string pair was found.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

<table class="Borderless"><tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr></table>

</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">pointerToString</td>
<td class="paramDataType">char&nbsp;**</td>

<td>A pointer variable into which the address of the string associated with <strong>tagName</strong> is placed.  Do NOT modify the string or attempt to free the pointer.<br><br>

The address of the string will be valid until the next call to <span class="Monospace">Ini_GetPointerToString</span>, <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringCopy.htm')">Ini_GetStringCopy</a></span>, <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringIntoBuffer.htm')">Ini_GetStringIntoBuffer</a></span>, <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetPointerToRawString.htm')">Ini_GetPointerToRawString</a></span>, <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetRawStringCopy.htm')">Ini_GetRawStringCopy</a></span>, or <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetRawStringIntoBuffer.htm')">Ini_GetRawStringIntoBuffer</a></span>.<br><br>

If an error occurs or the Tag Name cannot be found in the section, the pointer is set to NULL.</td>
</tr>
</table>

</body>
</html>   � =   �  x    Handle                           �� = � �  �    Section Name                     �� =f �  �    Tag Name                         ����    x    Status                           �� �f    x    Pointer to String                      ""    ""    	            	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetStringCopy</h1>

<p class="syntax">int Ini_GetStringCopy (IniText handle, const char sectionName[], const char tagName[], char **copyOfString);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains a copy of the string associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>  

<p class="Body">You are responsible for freeing the string.</p>

</body>
</html>
    ^    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>
    H    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a tag/string pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/string pair was found.  If the string portion of the tag/string pair was empty, such as in<br><br>

    tag 1 &#61; &quot;&quot;<br><br>

the string is considered to have been found, and it is the empty string &#40;&quot;&quot;&#41;.  If the tag's value is empty, such as in <br><br>

    tag 1 &#61; <br><br>

no tag/value pair is found.<br><br>

A zero indicates that no tag/string pair was found.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

<table class="Borderless"><tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr></table>

</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">copyOfString</td>
<td class="paramDataType">char&nbsp;**</td>

<td>The pointer variable into which the address of the dynamically allocated copy of the string associated with <strong>tagName</strong> is placed.  
You are responsible for freeing the string by calling the <span class="Monospace">free</span> function.<br><br>

If an error occurs or the Tag Name cannot be found in the section, the pointer is set to NULL.</td>
</tr>
</table>

</body>
</html>
   U =   �  x    Handle                           � = � �  �    Section Name                     � =f �  �    Tag Name                         
N���    x    Status                           � �f    x    Copy of String                         ""    ""    	            	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetStringIntoBuffer</h1>

<p class="syntax">int Ini_GetStringIntoBuffer (IniText handle, const char sectionName[], const char tagName[], char buffer[], int bufferSize);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Copies the string associated with <strong>tagName</strong> into <strong>buffer</strong> in the section identified by <strong>sectionName</strong>.</p>  

<p class="Body"><strong>buffer</strong> must be able to contain at least as many bytes as indicated by <strong>bufferSize</strong>.</p>

</body>
</html>
    c    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>
    S    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a tag/string pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/string pair was found.  If the string portion of the tag/string pair was empty, such as in <br><br>

    tag 1 &#61; &quot;&quot;<br><br>

the string is considered to have been found, and it is the empty string &#40;&quot;&quot;&#41;.  If the tag's value is empty, such as in <br><br>

    tag 1 &#61;<br><br> 

no tag/value pair is found.<br><br>

A zero indicates that no tag/string pair was found.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

    <table class="Borderless"><tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr></table>

</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">buffer</td>
<td class="paramDataType">char&nbsp;[]</td>

<td>The buffer into which to copy the string associated with <strong>tagName</strong>.  The buffer must be large enough to contain at least as many bytes as specified by <strong>bufferSize</strong>.<br><br>

If an error occurs or the Tag Name cannot be found in the section, the first byte of <strong>buffer</strong> is set to ASCII NUL.</td>
</tr>
</table>

</body>
</html>
    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">bufferSize</td>
<td class="paramDataType">integer</td>

<td>The maximum number of bytes (including the ASCII NUL byte) to be written into <strong>buffer</strong>.</td>
</tr>
</table>

</body>
</html>
   � =   �  x    Handle                           R = � �  �    Section Name                     . =f �  �    Tag Name                         ����    x    Status                           $L � �    x    Buffer                           '- �f     x    Buffer Size                            ""    ""    	            	               �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetPointerToRawString</h1>

<p class="syntax">int Ini_GetPointerToRawString (IniText handle, const char sectionName[], const char tagName[], char **pointerToString);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains a pointer to the string associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

<p class="Body">Do NOT modify the string or attempt to free the pointer.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;This function is exactly like <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetPointerToString.htm')">Ini_GetPointerToString</a></span> except that backslash characters in the string are not interpreted as denoting escape codes for unprintable characters.  One possible use for this function is to read a pathname in PC format.  Since PC pathnames contain backslashes and since the escape character used by <span class="Monospace">Ini_GetPointerToString</span> is also the backslash, you must use <span class="Monospace">Ini_GetPointerToRawString</span> if you do not want the back slashes in the pathname to be interpreted as escape codes.  This is only an issue if you are using the inifile instrument to read entries that were not created by the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_PutString.htm')">Ini_PutString</a></span> function.  The non&ndash;raw versions of the <span class="Monospace">Ini_GetString</span> functions automatically recognize and remove the escape codes inserted by <span class="Monospace">Ini_PutString</span>.</td></tr></table>

</body>
</html>    e    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    V    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a tag/string pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/string pair was found.  If the string portion of the tag/string pair was empty, such as in <br><br>

    tag 1 &#61; &quot;&quot;<br><br>

the string is considered to have been found, and it is the empty string &#40;&quot;&quot;&#41;.  If the tag's value is empty, such as in <br><br>

    tag 1 &#61;<br><br> 

no tag/value pair is found.<br><br>

A zero indicates that no tag/string pair was found.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

    <table class="Borderless"><tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr></table>

</td>
</tr>
</table>

</body>
</html>

    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetPointerToRawString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">pointerToString</td>
<td class="paramDataType">char&nbsp;**</td>

<td>A pointer variable into which the address of the string associated with <strong>tagName</strong> is placed.  Do NOT modify the string or attempt to free the pointer.<br><br>

The address of the string will be valid until the next call to <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetPointerToString.htm')">Ini_GetPointerToString</a></span>, <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringCopy.htm')">Ini_GetStringCopy</a></span>, <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringIntoBuffer.htm')">Ini_GetStringIntoBuffer</a></span>, <span class="Monospace">Ini_GetPointerToRawString</span>, <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetRawStringCopy.htm')">Ini_GetRawStringCopy</a></span>, or <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetRawStringIntoBuffer.htm')">Ini_GetRawStringIntoBuffer</a></span>.<br><br>

If an error occurs or the Tag Name cannot be found in the section, the pointer is set to NULL.</td>
</tr>
</table>

</body>
</html>   2� =   �  x    Handle                           4� = � �  �    Section Name                     6� =f �  �    Tag Name                         8����    x    Status                           >� �f    x    Pointer to String                      ""    ""    	            	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetRawStringCopy</h1>

<p class="syntax">int Ini_GetRawStringCopy (IniText handle, const char sectionName[], const char tagName[], char **copyOfString);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains a copy of the string associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>  

<p class="Body">You are responsible for freeing the string.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;This function is exactly like <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringCopy.htm')">Ini_GetStringCopy</a></span> except that backslash characters in the string are not interpreted as denoting escape codes for unprintable characters.  One possible use for this function is to read a pathname in PC format.  Since PC pathnames contain backslashes and since the escape character used by <span class="Monospace">Ini_GetStringCopy</span> is also the backslash, you must use <span class="Monospace">Ini_GetRawStringCopy</span> if you do not want the back slashes in the pathname to be interpreted as escape codes.  This is only an issue if you are using the inifile instrument to read entries that were not created by the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_PutString.htm')">Ini_PutString</a></span> function.  The non-raw versions of the <span class="Monospace">Ini_GetString</span> functions automatically recognize and remove the escape codes inserted by <span class="Monospace">Ini_PutString</span>.</td></tr></table>

</body>
</html>


    `    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    H    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a tag/string pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/string pair was found.  If the string portion of the tag/string pair was empty, such as in <br><br>

tag 1 &#61; &quot;&quot;<br><br>

the string is considered to have been found, and it is the empty string &#40;&quot;&quot;&#41;.  If the tag's value is empty, such as in <br><br>

   tag 1 &#61;<br><br>

no tag/value pair is found.<br><br>

A zero indicates that no tag/string pair was found.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

    <table class="Borderless"><tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr></table>
</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringCopy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">copyOfString</td>
<td class="paramDataType">char&nbsp;**</td>

<td>The pointer variable into which the address of the dynamically allocated copy of the string associated with <strong>tagName</strong> is placed.  
You are responsible for freeing the string by calling the <span class="Monospace">free</span> function.<br><br>

If an error occurs or the Tag Name cannot be found in the section, the pointer is set to NULL.</td>
</tr>
</table>

</body>
</html>   N� =   �  x    Handle                           P� = � �  �    Section Name                     R� =f �  �    Tag Name                         T����    x    Status                           Z� �f    x    Copy of String                         ""    ""    	            	           R    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetRawStringIntoBuffer</h1>

<p class="syntax">int Ini_GetRawStringIntoBuffer (IniText handle, const char sectionName[], const char tagName[], char buffer[], int bufferSize);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Copies the string associated with <strong>tagName</strong> into <strong>buffer</strong> in the section identified by <strong>sectionName</strong>.</p>  

<p class="Body"><strong>Buffer</strong> must be able to contain at least as many bytes as indicated by <strong>bufferSize</strong>.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;This function is exactly like <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_GetStringIntoBuffer.htm')">Ini_GetStringIntoBuffer</a></span> except that backslash characters in the string are not interpreted as denoting escape codes for unprintable characters.  One possible use for this function is to read a pathname in PC format.  Since PC pathnames contain backslashes and since the escape character used by <span class="Monospace">Ini_GetStringIntoBuffer</span> is also the backslash, you must use <span class="Monospace">Ini_GetRawStringIntoBuffer</span> if you do not want the back slashes in the pathname to be interpreted as escape codes.  This is only an issue if you are using the inifile instrument to read entries that were not created by the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_PutString.htm')">Ini_PutString</a></span> function.  The non&ndash;raw versions of the <span class="Monospace">Ini_GetString</span> functions automatically recognize and remove the escape codes inserted by <span class="Monospace">Ini_PutString</span>.</td></tr></table>

</body>
</html>


    g    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>
    V    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a tag/string pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/string pair was found.  If the string portion of the tag/string pair was empty, such as in <br><br>

    tag 1 &#61; &quot;&quot;<br><br>

the string is considered to have been found, and it is the empty string &#40;&quot;&quot;&#41;.  If the tag's value is empty, such as in <br><br>

    tag 1 &#61;<br><br>

no tag/value pair is found.<br><br>

A zero indicates that no tag/string pair was found.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

    <table class="Borderless"><tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr></table>
</td>
</tr>
</table>

</body>
</html>


    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">buffer</td>
<td class="paramDataType">char&nbsp;[]</td>

<td>The buffer into which to copy the string associated with <strong>tagName</strong>.  The buffer must be large enough to contain at least as many bytes as specified by <strong>bufferSize</strong>.<br><br>

If an error occurs or the Tag Name cannot be found in the section, the first byte of <strong>buffer</strong> is set to ASCII NUL.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetRawStringIntoBuffer</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">bufferSize</td>
<td class="paramDataType">integer</td>

<td>The maximum number of bytes (including the ASCII NUL byte) to be written into <strong>buffer</strong>.</td>
</tr>
</table>

</body>
</html>   gX =   �  x    Handle                           i� = � �  �    Section Name                     k� =f �  �    Tag Name                         mk���    x    Status                           s� � �    x    Buffer                           v� �f     x    Buffer Size                            ""    ""    	            	               J    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetInt</h1>

<p class="syntax">int Ini_GetInt (IniText handle, const char sectionName[], const char tagName[], int *integerValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains the integer value associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

</body>
</html>


    X    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>



</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a valid tag/integer pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/integer pair was found, and that the integer portion of the pair was a valid ASCII decimal representation of an integer.<br><br>

A zero indicates that no tag/integer pair was found, or that the integer portion was empty.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.
<br><br>

For this function, the only errors you might encounter are:<br><br>

<table class="Borderless">
 <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
  <tr><td><span class="nobreak">&ndash;5021</span></td>  <td>Invalid integer</td></tr></table>

</td>
</tr>
</table>

</body>
</html>


    l    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">integerValue</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>The variable into which the integer value associated with <strong>tagName</strong> is placed.<br><br>

If <strong>tagName</strong> cannot be found, or the integer value is empty, or an error occurs, the variable is left untouched.</td>
</tr>
</table>

</body>
</html>   |c =   �  x    Handle                           ~� = � �  �    Section Name                     �� =f �  �    Tag Name                         �F���    x    Status                           �> �f     x    Integer Value                          ""    ""    	            	           h    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetUInt</h1>

<p class="syntax">int Ini_GetUInt (IniText handle, const char sectionName[], const char tagName[], unsigned int *unsignedIntegerValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains the unsigned integer value associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>


</body>
</html>


    X    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>


</body>
</html>    3    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a valid tag/unsigned integer pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/unsigned integer pair was found, and that the unsigned integer portion of the pair was a valid ASCII decimal representation of an unsigned integer.<br><br>

A zero indicates that no tag/unsigned integer pair was found, or that the unsigned integer portion was empty.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only errors you might encounter are:<br>

    <table class="Borderless">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
  <tr><td><span class="nobreak">&ndash;5022</span></td>  <td>Invalid unsigned integer</td></tr></table>

</td>
</tr>
</table>


</body>
</html>


    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">unsignedIntegerValue</td>
<td class="paramDataType">unsigned&nbsp;integer&nbsp;*</td>

<td>The variable into which the unsigned integer value associated with <strong>tagName</strong> is placed.<br><br>

If <strong>tagName</strong> cannot be found, or the unsigned integer value is empty, or an error occurs, the variable is left untouched.</td>
</tr>
</table>


</body>
</html>   �S =   �  x    Handle                           �� = � �  �    Section Name                     �� =f �  �    Tag Name                         �9���    x    Status                           �t �f    x    Unsigned Integer Value                 ""    ""    	            	           T    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetDouble</h1>

<p class="syntax">int Ini_GetDouble (IniText handle, const char sectionName[], const char tagName[], double *doubleValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains the double value associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

</body>
</html>


    Z    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a valid tag/double pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/double pair was found, and that the double portion of the pair was a valid ASCII representation of an double&ndash;precision real number.<br><br>

A zero indicates that no tag/double pair was found, or that the double portion was empty.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only errors you might encounter are:<br>

<table class="Borderless">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
  <tr><td><span class="nobreak">&ndash;5023</span></td>  <td>Invalid double value</td></tr></table>

</td>
</tr>
</table>

</body>
</html>    k    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetDouble</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">doubleValue</td>
<td class="paramDataType">double&nbsp;*</td>

<td>The variable into which the double value associated with <strong>tagName</strong> is placed.<br><br>

If <strong>tagName</strong> cannot be found, or the double value is empty, or an error occurs, the variable is left untouched.</td>
</tr>
</table>

</body>
</html>   �� =   �  x    Handle                           � = � �  �    Section Name                     �� =f �  �    Tag Name                         �����    x    Status                           �� �f    x    Double Value                           ""    ""    	            	          V    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetBoolean</h1>

<p class="syntax">int Ini_GetBoolean (IniText handle, const char sectionName[], const char tagName[], int *booleanValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains the boolean value associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

</body>
</html>


    [    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a valid tag/boolean pair was found with the specified <strong>Tag Name</strong> in the section specified by <strong>Section Name</strong>.<br><br>

A positive return value indicates that a tag/boolean pair was found, and that the boolean portion of the pair was one of the valid boolean representations.<br><br>

A zero indicates that no tag/boolean pair was found, or that the boolean portion was empty.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only errors you might encounter are:<br>

 <table class="Borderless">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
  <tr><td><span class="nobreak">&ndash;5024</span></td>  <td>Invalid boolean</td></tr></table>

</td>
</tr>
</table>  
  
</body>
</html>


        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetBoolean</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">booleanValue</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>The variable into which the boolean value associated with <strong>tagName</strong> is placed.  If the boolean value is &quot;True&quot;, a 1 is placed in the variable.  If the boolean value is &quot;False&quot;, a 0 is placed in the variable.<br><br>

If <strong>tagName</strong> cannot be found, or the value is not a valid boolean representation, or an error occurs, the variable is left untouched.</td>
</tr>
</table>

</body>
</html>   �� =   �  x    Handle                           �� = � �  �    Section Name                     �� =f �  �    Tag Name                         �����    x    Status                           �z �f     x    Boolean Value                          ""    ""    	            	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetData</h1>

<p class="syntax">int Ini_GetData (IniText handle, const char sectionName[], const char tagName[], unsigned char **data, long *dataSize);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains a copy of the data block associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>  

<p class="Body">You are responsible for freeing the data.</p>

  
</body>
</html>


    W    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

  
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

  
</body>
</html>    v    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether a tag/data pair was found with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/data pair was found.<br><br>

If the tag's value is empty, such as in <br><br>

    tag 1 &#61; <br><br>

no tag/value pair is found.<br><br>

A zero indicates that no tag/data pair was found.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, the only error you are likely to encounter is:<br>

 <table class="Borderless"><tr>   <td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr></table>
</td>
</tr>
</table>

  
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">data</td>
<td class="paramDataType">unsigned&nbsp;char&nbsp;**</td>

<td>The pointer variable into which the address of a dynamically allocated copy of the data associated with <strong>tagName</strong> is placed.  
You are responsible for freeing the data by calling the <span class="Monospace">free</span> function.<br><br>

If an error occurs or the Tag Name cannot be found in the section, the pointer is set to NULL.</td>
</tr>
</table>

  
</body>
</html>


         <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetData</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">dataSize</td>
<td class="paramDataType">long&nbsp;*</td>

<td>Returns the size of the data in bytes.<br><br>

You may pass 0 (NULL) for this parameter if you do not need this information.</td>
</tr>
</table>

  
</body>
</html>   ŀ =   �  x    Handle                           �� = � �  �    Section Name                     ɱ =f �  �    Tag Name                         �j���    x    Status                           �� � � �  x    Data                             �� �f    x    Data Size                              ""    ""    	            	           	           ]    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetInt64</h1>

<p class="syntax">int Ini_GetInt64 (IniText handle, const char sectionName[], const char tagName[], long long *integerValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains the 64-bit integer value associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>

</body>
</html>


    b    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>. This handle represents the list of tag/value pairs in memory.</td>
</tr>
</table>



</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function found a valid tag/integer pair with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/integer pair was found and that the integer portion of the pair was a valid ASCII decimal representation of an integer.<br><br>

A zero indicates that no tag/integer pair was found, or that the integer portion was empty.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.
<br><br>

For this function, you might encounter the following errors:<br><br>

<table class="Borderless">
 <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
  <tr><td><span class="nobreak">&ndash;5021</span></td>  <td>Invalid integer</td></tr></table>

</td>
</tr>
</table>

</body>
</html>


    n    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">integerValue</td>
<td class="paramDataType">long long&nbsp;*</td>

<td>Returns the 64-bit integer value associated with <strong>tagName</strong>.<br><br>

If the function cannot find the <strong>tagName</strong>, or the integer value is empty, or an error occurs, the function ignores this variable.</td>
</tr>
</table>

</body>
</html>   ٻ 3   �  x    Handle                           �% 3 � �  �    Section Name                     �� 3T �  �    Tag Name                         ߬���    x    Status                           � �T   x    Integer Value                          ""    ""    	            	           t    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_GetUInt64</h1>

<p class="syntax">int Ini_GetUInt64 (IniText handle, const char sectionName[], const char tagName[], unsigned long long *unsignedIntegerValue);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains the unsigned integer value associated with <strong>tagName</strong> in the section identified by <strong>sectionName</strong>.</p>


</body>
</html>


    b    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>. This handle represents the list of tag/value pairs in memory.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>


</body>
</html>    @    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">status</td>
<td class="paramDataType">integer</td>

<td>Indicates whether the function found a valid tag/unsigned integer pair with the specified <strong>tagName</strong> in the section specified by <strong>sectionName</strong>.<br><br>

A positive return value indicates that a tag/unsigned integer pair was found, and that the unsigned integer portion of the pair was a valid ASCII decimal representation of an unsigned integer.<br><br>

A zero indicates that no tag/unsigned integer pair was found or that the unsigned integer portion was empty.<br><br>

A negative value indicates an error.  If the value is from
<span class="nobreak">&ndash;1</span> to <span class="nobreak">&ndash;999</span>, it is one of the error values defined in <span class="Monospace">userint.h</span>.  If the value is from <span class="nobreak">&ndash;5000</span> to <span class="nobreak">&ndash;5999</span>, it is one of the error values defined in <span class="Monospace">toolbox.h</span>.<br><br>

For this function, you might encounter the following errors:<br><br>

    <table class="Borderless">
    <tr><td><span class="nobreak">&ndash;12</span></td>  <td>Out of memory!</td></tr>
  <tr><td><span class="nobreak">&ndash;5022</span></td>  <td>Invalid unsigned integer</td></tr></table>

</td>
</tr>
</table>


</body>
</html>


    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_GetUInt64</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">unsignedIntegerValue</td>
<td class="paramDataType">unsigned&nbsp;long long&nbsp;*</td>

<td>Returns the unsigned 64-bit integer value associated with <strong>tagName</strong>.<br><br>

If the function cannot find <strong>tagName</strong>, or the unsigned integer value is empty, or an error occurs, the function ignores this variable.</td>
</tr>
</table>


</body>
</html>   �� 3   �  x    Handle                           �< 3 � �  �    Section Name                     � 3T �  �    Tag Name                         �����    x    Status                           � �T   x    Unsigned Integer Value                 ""    ""    	            	           F    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SectionExists</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_SectionExists</h1>

<p class="syntax">int Ini_SectionExists (IniText handle, const char sectionName[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Determines if the section specified by <strong>sectionName</strong> exists in the list of tag/value pairs specified by <strong>handle</strong>.</p>

</body>
</html>
    ]    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SectionExists</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SectionExists</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The name of the section, the existence of which is determined.</td>
</tr>
</table>

</body>
</html>        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_SectionExists</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionExists</td>
<td class="paramDataType">integer</td>

<td>Indicates if the section specified by <strong>sectionName</strong> exists in the list of tag/value pairs specified by <strong>handle</strong>.<br><br>

If the section exists, a 1 is returned.<br><br>

If the section does not exist, a 0 is returned.</td>
</tr>
</table>

</body>
</html>   �) =   �  x    Handle                            � = � �  �    Section Name                     i���    x    Section Exists                         ""    	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NumberOfSections</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_NumberOfSections</h1>

<p class="syntax">int Ini_NumberOfSections (IniText handle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Returns the number of sections in the tag/value pair list identified by <strong>handle</strong>.</p>

</body>
</html>    `    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NumberOfSections</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NumberOfSections</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">numberOfSections</td>
<td class="paramDataType">integer</td>

<td>The number of sections in the tag/value pair list identified by <strong>handle</strong>.</td>
</tr>
</table>

</body>
</html>   � =   �  x    Handle                           
���    x    Number of Sections                     	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthSectionName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_NthSectionName</h1>

<p class="syntax">int Ini_NthSectionName (IniText handle, int sectionIndex, char **sectionNamePointer);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains a pointer to name of the section specified by the 1&ndash;based index in <strong>sectionIndex</strong>.</p>  

<p class="Body">Do NOT modify the section name or attempt to free the pointer.</p>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthSectionName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionFound</td>
<td class="paramDataType">integer</td>

<td>If there is a section corresponding to the specified index, 
1 is returned.<br><br>

Otherwise, 0 is returned.</td>
</tr>
</table>


</body>
</html>    ^    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthSectionName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthSectionName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionIndex</td>
<td class="paramDataType">integer</td>

<td>A 1&ndash;based index into the list sections in the tag/value list specified by <strong>handle</strong>.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthSectionName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionNamePointer</td>
<td class="paramDataType">char&nbsp;**</td>

<td>A pointer variable into which the address of the section name is placed.  Do NOT modify the section name or attempt to free the pointer.<br><br>

The pointer is valid until the next time the list of in&ndash;memory tag/value pairs is modified.<br><br>

If there is no section corresponding to the specified index, the pointer variable is left untouched.</td>
</tr>
</table>


</body>
</html>   ���    x    Section Found?                    = =  �  x    Handle                            = �     x    Section Index                    v =y    x    Section Name Pointer               	                1    	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ItemExists</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_ItemExists</h1>

<p class="syntax">int Ini_ItemExists (IniText handle, const char sectionName[], const char tagName[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Determines if the item specified by <strong>tagName</strong> exists in the section identified by <strong>sectionName</strong>, or if there are items having tag names of the form<br><br>

     &lt;Tag Name&gt; Line 0001<br>
     &lt;Tag Name&gt; Line 0002<br>
     &lt;Tag Name&gt; Line 0003<br><br>

in the section identified by <strong>sectionName</strong>.  The latter case indicates that the item specified by <strong>tagName</strong> was split across several lines.</p>


<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;This function should be used instead of the obsoleted function <span class="Monospace">Ini_ItemOrMultiLineItemExists</span>.
</td></tr></table>

</body>
</html>    Z    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ItemExists</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ItemExists</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ItemExists</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name in the tag/value pair.</td>
</tr>
</table>

</body>
</html>    ^    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_ItemExists</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">itemExists</td>
<td class="paramDataType">integer</td>

<td>Indicates if the item specified by <strong>tagName</strong> exists in the section identified by <strong>sectionName</strong>.<br><br>

If the item exists, 1 is returned.<br><br>

If the item does not exist, 0 is returned.</td>
</tr>
</table>

</body>
</html>    3 !  �  x    Handle                            q 3 � �  �    Section Name                     "C 3` �  �    Tag Name                         #����    x    Item Exists                            ""    ""    	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NumberOfItems</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_NumberOfItems</h1>

<p class="syntax">int Ini_NumberOfItems (IniText handle, const char sectionName[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains the number of items in the section identified by <strong>SectionName</strong>.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;If an item was split across several lines, it has several tag names entries in the <span class="Monospace">.ini</span> file, as in the following.<br><br>

     &lt;Tag Name&gt; Line 0001<br>
     &lt;Tag Name&gt; Line 0002<br>
     &lt;Tag Name&gt; Line 0003<br><br>

For the purposes of this function, the group of tag names counts as only one item.</td></tr></table>
</body>
</html>


    ]    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NumberOfItems</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NumberOfItems</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The name of the section under which the item count is returned.</td>
</tr>
</table>

</body>
</html>        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NumberOfItems</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">numberOfItems</td>
<td class="paramDataType">integer</td>

<td>The number of items in the section identified by <strong>sectionName</strong>.<br><br>

If the section does not exist, 0 is returned.</td>
</tr>
</table>

</body>
</html>   +P 9 %  �  x    Handle                           -� : � �  �    Section Name                     /����    x    Number of Items                        ""    	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthItemName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_NthItemName</h1>

<p class="syntax">int Ini_NthItemName (IniText handle, const char sectionName[], int itemIndex, char **itemTagNamePointer);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains a pointer to the tag name of the tag/value pair specified by the 1&ndash;based index in <strong>itemIndex</strong>, within the section identified by <strong>sectionName</strong>.</p>

<p class="Body">Do NOT modify the tag name or attempt to free the pointer.</p>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp;If the item was split across several lines, it has several tag names entries in the <span class="Monospace">.ini</span> file, as in the following.<br><br>

     &lt;Tag Name&gt; Line 0001<br>
     &lt;Tag Name&gt; Line 0002<br>
     &lt;Tag Name&gt; Line 0003<br><br>

For the purposes of this function, the group of tag names counts as only one item with the name of &lt;Tag Name&gt;.</td></tr></table>

</body>
</html>

    [    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthItemName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthItemName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The section name under which to find the tag/value pair.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthItemName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">itemTagNamePointer</td>
<td class="paramDataType">char&nbsp;**</td>

<td>A pointer variable into which the address of the tag name is placed.  Do NOT modify the tag name or attempt to free the pointer.<br><br>

The pointer is valid until the next time the list of in&ndash;memory tag/value pairs is modified.<br><br>

If there is no tag/value pair corresponding to the specified index in the section, the pointer variable is left untouched.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthItemName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">itemFound</td>
<td class="paramDataType">integer</td>

<td>If there is a section corresponding to the specified index, 
1 is returned.<br><br>

Otherwise, 0 is returned.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_NthItemName</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">itemIndex</td>
<td class="paramDataType">integer</td>

<td>A 1&ndash;based index into the list tag/value pairs in the section identified by <strong>sectionName</strong>.</td>
</tr>
</table>

</body>
</html>   7S 2 2  �  x    Handle                           9� 2 � �  �    Section Name                     ;� ��    x    Item Tag Name Pointer            >����    x    Item Found?                      @� 4}     x    Item Index                             ""    	            	            1   �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_LineOfLastAccess</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_LineOfLastAccess</h1>

<p class="syntax">int Ini_LineOfLastAccess (IniText handle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Obtains the 1&ndash;based index of the line of the file the was the source of the last tag/value pair accessed by one of the <span class="Monospace">Ini_Get</span>... functions.</p>

<p class="Body">This information can be useful when reporting an error while processing an <span class="Monospace">.ini</span> style file.</p>

</body>
</html>    `    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_LineOfLastAccess</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    0    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_LineOfLastAccess</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">lineNumber</td>
<td class="paramDataType">integer</td>

<td>The 1&ndash;based index of the line of the file the was the source of the last tag/value pair accessed by one of the <span class="Monospace">Ini_Get</span>... functions.</td>
</tr>
</table>

</body>
</html>   F� B �  �  x    Handle                           I���    x    Line Number                            	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemoveSection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_RemoveSection</h1>

<p class="syntax">int Ini_RemoveSection (IniText handle, const char sectionName[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function removes a section and all of its items from the list of in&ndash;memory tag/value pairs.</p>

</body>
</html>    ]    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemoveSection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemoveSection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The name of the section to remove.</td>
</tr>
</table>

</body>
</html>    {    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemoveSection</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">result</td>
<td class="paramDataType">integer</td>

<td>Indicates if the section specified by <strong>sectionName</strong> was removed from the tag/value pairs specified by <strong>handle</strong>.<br><br>

If the section was removed, a 1 is returned.<br><br>

If the section was not found, a 0 is returned.</td>
</tr>
</table>

</body>
</html>   M� 3 (  �  x    Handle                           P> 3 � �  �    Section Name                     Q����    x    Result                                 ""    	           r    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemoveItem</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">Ini_RemoveItem</h1>

<p class="syntax">int Ini_RemoveItem (IniText handle, const char sectionName[], const char tagName[]);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function removes an item from the list of in&ndash;memory tag/value pairs. If the item is a string that was split into multiple lines, then all of the lines are removed.</p>


</body>
</html>    Z    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemoveItem</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">handle</td>
<td class="paramDataType">IniText</td>

<td>A handle returned from <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviIni_New.htm')">Ini_New</a></span>.  It represents the list of in-memory tag/value pairs.</td>
</tr>
</table>

</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemoveItem</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">sectionName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The name of the section containing the item to remove.</td>
</tr>
</table>

</body>
</html>    P    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemoveItem</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">result</td>
<td class="paramDataType">integer</td>

<td>Indicates if the specified item was removed from the tag/value pairs specified by <strong>handle</strong>.<br><br>

If the item was removed, a 1 is returned.<br><br>

If the item was not found, a 0 is returned.</td>
</tr>
</table>


</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>Ini_RemoveItem</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">tagName</td>
<td class="paramDataType">const&nbsp;char&nbsp;[]</td>

<td>The tag name of the tag/value pair to remove.</td>
</tr>
</table>


</body>
</html>   W� 3   �  x    Handle                           Z 3 � �  �    Section Name                     [����    x    Result                           ^9 3x �  �    Tag Name                               ""    	            "" ����         +�  ?C             K.        New                                                                                                                                     ����         @�  E�             K.        Dispose                                                                                                                                 ����         E�  O�             K.        Sort                                                                                                                                    ����         O�  X             K.        DisableSorting                                                                                                                          ����         X:  h�             K.        SortInternally                                                                                                                          ����         iK  r             K.        DisableInternalSorting                                                                                                                  ����         r@  ��             K.        WriteToFile                                                                                                                             ����         �{  �             K.        ReadFromFile                                                                                                                            ����         ��  �             K.        WriteToRegistry                                                                                                                         ����         ��  ��             K.        ReadFromRegistry                                                                                                                        ����         �I  ص             K.        WriteGeneric                                                                                                                            ����         ١  �             K.        ReadGeneric                                                                                                                             ����         � 	�             K.        CopySection                                                                                                                             ����        
� "�             K.        CopySectionEx                                                                                                                           ����        $	 4�             K.        SetTokens                                                                                                                               ����        5� E-             K.        SetSectionFilter                                                                                                                        ����        F T�             K.        SetDuplicateChecking                                                                                                                    ����        U� kd             K.        PutString                                                                                                                               ����        l� �c             K.        PutRawString                                                                                                                            ����        �� �$             K.        PutInt                                                                                                                                  ����        �M �+             K.        PutUInt                                                                                                                                 ����        �T ��             K.        PutDouble                                                                                                                               ����        �! �              K.        PutBoolean                                                                                                                              ����        �7 �             K.        PutData                                                                                                                                 ����        �r  w             K.        GetPointerToString                                                                                                                      ����        � �             K.        GetStringCopy                                                                                                                           ����        � )&             K.        GetStringIntoBuffer                                                                                                                     ����        *� E�             K.        GetPointerToRawString                                                                                                                   ����        F� ]�             K.        GetRawStringCopy                                                                                                                        ����        ^� x�             K.        GetRawStringIntoBuffer                                                                                                                  ����        z ��             K.        GetInt                                                                                                                                  ����        �� �             K.        GetUInt                                                                                                                                 ����        �C �             K.        GetDouble                                                                                                                               ����        �7 ��             K.        GetBoolean                                                                                                                              ����        �� ��             K.        GetData                                                                                                                                 ����        �V �%             K.        GetInt64                                                                                                                                ����        �V ��             K.        GetUInt64                                                                                                                               ����        �� �             K.        SectionExists                                                                                                                           ����        �              K.        NumberOfSections                                                                                                                        ����        � r             K.        NthSectionName                                                                                                                          ����        g &b             K.        ItemExists                                                                                                                              ����        'R 1�             K.        NumberOfItems                                                                                                                           ����        2Z B}             K.        NthItemName                                                                                                                             ����        C� K;             K.        LineOfLastAccess                                                                                                                        ����        K� T�             K.        RemoveSection                                                                                                                           ����        U5 _�             K.        RemoveItem                                                                                                                                    �                                                                                    "�Management                                                                           �New In-Memory Object                                                                 �Dispose In-Memory Object                                                             �Sort                                                                                 �Disable Sorting                                                                      �Sort (Internally)                                                                    �Disable Sorting (Internally)                                                         �Write To File                                                                        �Read From File                                                                       �Write To Registry                                                                    �Read From Registry                                                                   �Write To Generic Destination                                                         �Read From Generic Source                                                             �Copy Section                                                                         �Copy Section Ex                                                                      �Set Tokens                                                                           �Set Section Filter                                                                   �Set Duplicate Checking                                                              %�Putting Tag/Value Pairs                                                              �Put String                                                                           �Put Raw String                                                                       �Put Integer                                                                          �Put Unsigned Integer                                                                 �Put Double                                                                           �Put Boolean                                                                          �Put Data                                                                            '4Getting Values from Tags                                                             �Get Pointer To String                                                                �Get Copy Of String                                                                   �Get String Into Buffer                                                               �Get Pointer To Raw String                                                            �Get Copy Of Raw String                                                               �Get Raw String Into Buffer                                                           �Get Integer                                                                          �Get Unsigned Integer                                                                 �Get Double                                                                           �Get Boolean                                                                          �Get Data                                                                             �Get 64 bit Integer                                                                   �Get Unsigned 64 bit Integer                                                         (�Informational                                                                        �Section Exists?                                                                      �Number of Sections                                                                   �Nth Section Name                                                                     �Item Exists?                                                                         �Number of Items                                                                      �Nth Item Name                                                                        �Line Of Last Access                                                                 *)Removing Sections and Items                                                          �Remove Section                                                                       �Remove Item                                                                           toolbox.fp 