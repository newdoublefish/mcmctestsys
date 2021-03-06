s��         ��     �Q  �   �     �)                               RegExpr     Regular Expressions                              � ��const char *  *    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">Use the functions in this instrument driver to search text for patterns that match regular expressions.</p>

<p class="Body">You can program with regular expressions in two ways:</p>

<ul>
<li>At the high level, you can call <span class="Monospace">RegExpr_FindPatternInText</span>, which searches a text buffer for a match to a regular expression. This function parses the expression, iteratively tries to match the expression to subsequent positions
in the text buffer, and then destroys the regular expression parse tree.</li>

<li>At the low level, you can call separate functions for parsing a regular expression, matching the expression to the beginning of a text buffer, and destroying the parse tree. The Low-level class help contains an example of how to use the low-level functions.</li>
</ul>

<p class="Body">The following example demonstrates how to use the high-level function.</p>

<p class="code">char *text; </p>
<P class="code">int textLen, caseSens, matchPolicy;</p>    
<P class="code">int handle, matched, matchPosition, matchLen;</p>
<P class="code">/* set values for text, textLen, caseSens, matchPolicy */</p>    
<p class="code">if (RegExpr_FindPatternInText (regExpr, caseSens, text, textLen,</p>
<p class="code1Nospace">RegExpr_SearchForwards, matchPolicy, &amp;matched, &amp;matchPosition, &amp;matchLen) &lt; 0)</p>

<p class="code1">printf (&quot;Error parsing regular expression %s\n(%s)\n&quot;, regExpr, RegExpr_GetErrorElaboration());</p> 
<p class="code">else</p>
<p class="code1">if (matched)</p>             
<p class="code2">printf (&quot;Match found at index=%d, length=%d&quot;\n, matchPosition, matchLen);</p>
<p class="code1">else</p>             
<p class="code2">printf (&quot;No match found\n&quot;.);</p>

<p class="body">The functions in this instrument driver are multithread-safe.</p>

</BODY>
</HTML>    V    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_Lowlevel</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<p class="Body">This class contains functions for parsing a regular expression, matching it to the beginning of a text buffer, and destroying it.  For many cases, the high&ndash;level function <span class="Monospace">RegExpr_FindPatternInText</span> is sufficient.  In those cases where it is not, the functions in this class are useful.</p>

<p class="Body">The example below shows how to use the functions in this class to implement a simplified (and somewhat modified) version of <span class="Monospace">FindPatternInText</span>.</p>


<p class="Code">
int FindPatternInText (<br>
char *regExpr,  /* must be nul-terminated */<br>
int  caseSens,<br>
int  matchPolicy,<br>
char *text,     /* ASSUME:  nul-terminated */<br>
int  *matchCol,<br>
int  *matchLen)<br>
{</p>
    <p class="Code1">
    int handle, matched;<br><br>

    matched = FALSE;<br>
    *matchCol = -1;<br>
    *matchLen = -1;<br><br>

    handle = RegExpr_Parse (regExpr, caseSens, &amp;handle);<br>
    if (handle &lt; 0)<br>
    &nbsp;&nbsp;&nbsp;&nbsp;
          printf (&quot;Error parsing regular expression %s\n(%s)\n&quot;, regExpr,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
RegExpr_GetErrorElaboration());<br> 
    else </p>
        <p class="Code2">
        {<br>
        int textLen = strlen(text);<br>
        int done = FALSE;<br>
        int i;<br><br>

        for (i=0; i &lt; textLen &amp;&amp; !done; i++)</p> 
            <p class="Code3">
            {<br>
            switch (RegExpr_MatchText (handle, text+i, textLen-i,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
 matchPolicy, matchLen))<br>
                
   &nbsp;&nbsp;&nbsp;&nbsp;{<br>
   &nbsp;&nbsp;&nbsp;&nbsp;case RegExpr_Match:<br>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
matched = TRUE;<br>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
*matchCol = i;<br>
     &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
done = TRUE;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;break;<br>
   &nbsp;&nbsp;&nbsp;&nbsp;case RegExpr_NoMatch:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;break;<br>
   &nbsp;&nbsp;&nbsp;&nbsp;case RegExpr_NotInText:  /* optimization */<br>
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
done = TRUE;<br>    
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;break;<br>
   &nbsp;&nbsp;&nbsp;&nbsp;}<br>
       
            }</p>
        <p class="Code2">
        RegExpr_Destroy (handle);<br>
        }</p>
    <p class="Code1">
    return matched;</p>
<p class="Code">
}</p>

</body>
</html>    8    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">RegExpr_FindPatternInText</h1>

<p class="syntax">int RegExpr_FindPatternInText (const char *regularExpressionText, int caseSensitive, const char *textToSearch, int textLength, int direction, int matchPolicy, int *matched, int *matchPosition, int *matchLength);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function searches a text buffer for a match to a regular expression.</p>

<p class="Body">See the Instrument help for example source code showing how to use this function.</p>
                                           
    
</body>
</html>    v    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">textToSearch</td>
<td class="paramDataType">const&nbsp;char&nbsp;*</td>

<td>The text to match to the regular expression.<br><br>

The regular expression must match from the beginning of the text.  If you want to search for a regular expression match anywhere in the text, you must call this function iteratively, each time passing the address of the next character in the text.
See the Instrument Help for example source code.  The function <span class="Monospace">RegExpr_FindPatternInText</span> performs this for you.</td>
</tr>
</table>                         
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">textLength</td>
<td class="paramDataType">integer</td>

<td>The number of bytes in the text you want to be included to the search for a match to the regular expression.<br><br>

If you pass a value less than zero, the text is assumed to be nul&ndash;terminated, and the full length is used.</td>
</tr>
</table>                                   
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">matchLength</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>This parameter is set to the number of characters in the text that actually matched to the regular expression.<br><br>

If there was no match, then the parameter is set to <span class="nobreak">&ndash;1</span>.</td>
</tr>
</table>
                                           
    
</body>
</html>    
�    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">parseStatus</td>
<td class="paramDataType">integer</td>

<td>Indicates if the regular expression was parsed successfully.<br><br>

If the string is not a valid regular expression, a negative error number is returned.  You can pass this error number to the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_GetErrorString.htm')">RegExpr_GetErrorString</a></span> function.  However, in some cases there is more error information than can be encoded in the error number.  You can get more detailed information about the result of the last call to this function by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_GetErrorElaboration.htm')">RegExpr_GetErrorElaboration</a></span>.<br><br>

The error numbers are:<br>

<table class="Borderless">
       <tr><td>0</td> <td>success</td></tr>
    <tr><td><span class="nobreak">&ndash;12</span></td> <td>out of memory</td></tr>
   <tr><td><span class="nobreak">&ndash;7900</span></td> <td>unmatched character</td></tr>
   <tr><td><span class="nobreak">&ndash;7901</span></td> <td>invalid character in range</td></tr> 
   <tr><td><span class="nobreak">&ndash;7902</span></td> <td>regular expression ends with backslash</td></tr>
  <tr><td><span class="nobreak">&ndash;7903</span></td> <td>invalid hex character &#40;after \x&#41;</td></tr>
   <tr><td><span class="nobreak">&ndash;7904</span></td> <td>operator applied to an empty pattern</td></tr>
   <tr><td><span class="nobreak">&ndash;7905</span></td> <td>empty left side of &#39;&#124;&#39;</td></tr>
   <tr><td><span class="nobreak">&ndash;7906</span></td> <td>empty right side of &#39;&#124;&#39;</td></tr>
   <tr><td><span class="nobreak">&ndash;7907</span></td> <td>empty group</td></tr>
   <tr><td><span class="nobreak">&ndash;7908</span></td> <td>invalid range</td></tr>
   <tr><td><span class="nobreak">&ndash;7909</span></td> <td>empty set &#40;&#91;&#93;&#41;</td></tr>
   <tr><td><span class="nobreak">&ndash;7910</span></td> <td>empty input string</td></tr>
   <tr><td><span class="nobreak">&ndash;7911</span></td> <td>NULL input string</td></tr>
   <tr><td><span class="nobreak">&ndash;7912</span></td> <td>multibyte characters not allowed in range</td></tr></table>
    
</td>
</tr>
</table>                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">matchPosition</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>The zero&ndash;based index of the position within the text where the match to the regular expression begins.<br><br>

If there was no match, then the parameter is set to <span class="nobreak">&ndash;1</span>.</td>
</tr>
</table>
                                           
    
</body>
</html>    c    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">direction</td>
<td class="paramDataType">integer</td>

<td>Specifies whether the text is searched starting at the beginning and working forwards or starting at the end and working backwards.<br><br>

Values:<br>

    <table class="Borderless">
    <tr><td>RegExpr_SearchForwards</td>  <td>1:  Start at beginning
                                (any non&ndash;zero value can be used)</td></tr>
   
    <tr><td>RegExpr_SearchBackwards</td> <td>0:  Start end end</td></tr>
    </table>
</td>
</tr>
</table>
                                           
    
</body>
</html>    u    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">regularExpressionText</td>
<td class="paramDataType">const&nbsp;char&nbsp;*</td>

<td>A nul&ndash;terminated string containing a regular expression.<br><br>

A regular expression consists of the following tokens:<br>

<table class="Borderless">
<tr><td>. (period)</td>
<td>match 1 character </td>
<td>a.t matches act and apt but not abort or at</td>
</tr>
<tr><td>&#42; (asterisk)</td>
<td>match 0 or more occurrences of the preceding character or &#123;expression&#125; </td>      
<td>0&#42;1 matches 1, 01, 001, etc. a.&#42; matches act, apt, abort, and at</td>
</tr>
<tr><td>&#43; (plus sign)</td>
<td>match 1 or more occurrences of the preceding character or &#123;expression&#125;</td>       
<td>0&#43;1 matches 01, 001, 0001, etc. &#123;ab&#125;&#43;c matches abc, ababc, but not c</td>                 
</tr>
<tr>                   
<td>&#63; (question mark)</td>
<td>match 0 or 1 occurrences of the preceding character or &#123;expression&#125;</td>
<td>0&#63;1 matches 1, 01, but not 001</td>
</tr>
<tr><td>&#124; (pipe)</td>        
<td>matches either the preceding or following character or &#123;expression&#125;</td>
<td>a3&#124;4b matches a3b or a4b</td>
</tr>
<tr><td>&#94; (caret)</td>
<td>matches the beginning of a line</td>
<td>&#94;int matches any line that begins with int</td>   
</tr>
<tr><td>&#36; (dollar sign)</td>
<td>matches the end of a line</td>
<td>done&#36; matches any line that ends with done</td>
</tr>
<tr><td>&#123;&#125; (curly braces)</td>          
<td>groups characters or expressions</td>        
<td>&#123;a3&#125;&#124;&#123;4b&#125; matches a3 or 4b</td>                    
</tr>
<tr><td>&#91;&#93; (brackets)</td>   
<td>matches any one character or range listed within the brackets</td>
<td>&#91;a&ndash;z&#93; matches every occurrence of lowercase letters &#91;abc&#93; matches every occurrence of a, b or c</td>
</tr>
<tr><td>&#126;  (tilde)</td>
<td>when appearing immediately after the left bracket, negates the contents of the set </td>       
<td>&#91;&#126;a&ndash;z&#93; matches anything except lowercase letters &#91;a&ndash;z&#126;A&ndash;Z&#93; matches all letters and the &#39;&#126;&#39; character</td>
</tr>
<tr><td>&#92;t&nbsp;(backslash&nbsp;t)</td>  
<td>matches any tab character</td>
<td>&#92;t3 matches every occurrence of a tab followed by a 3</td>
</tr>
<tr><td>&#92;x&nbsp;(backslash&nbsp;x)</td>  
<td>matches any character specified in hex</td>       
<td>&#92;x2a matches every occurrence of the &#39;*&#39; specified in hexcharacter</td>
</tr>
<tr><td>&#92;&nbsp;(backslash)</td>   
<td>used if any of the above characters themselves are to be included in the search</td>
<td>&#92;&ndash;&#92;&#63;&#92;&#92; matches every occurrence of &#39;&ndash;&#39; followed by &#39;&#63;&#39; and &#39;&#92;&#39;</td>
</tr>    
</table>

</td>
</tr>
</table>
                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">caseSensitive</td>
<td class="paramDataType">integer</td>

<td>Specifies whether the matching of characters is to be done on a case&ndash;senstive or case&ndash;insensitive basis.<br><br>

A non&ndash;zero value specifies that characters are to be matched on a case&ndash;sensitive basis.  For example. &quot;chr&quot; would match only to &quot;chr&quot; and not to &quot;CHR&quot;.<br><br>

A zero value specifies that characters are to be matched on a case&ndash;insensitive basis.  For example. &quot;chr&quot; would match to &quot;chr&quot;, &quot;CHR&quot;, and &quot;Chr&quot;.<br><br>

This parameter does apply to ranges.  For example, if this parameter is non&ndash;zero, then &quot;&#91;a&ndash;z&#93;&quot; in the regular expression string would match to any lowercase letter.  If this parameter is zero, then &quot;&#91;a&ndash;z&#93;&quot; would match to any letter.</td>
</tr>
</table>                                
    
</body>
</html>        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">matched</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>If there was a match, this parameter is set to 1.
Otherwise, it is set to 0.</td>
</tr>
</table>
                                           
    
</body>
</html>    w    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_FindPatternInText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">matchPolicy</td>
<td class="paramDataType">integer</td>

<td>Specifies whether the maximum or minimum number of characters are matched to the pattern.<br><br>

Examples:<br><br>

    If the pattern is &quot;a&#43;&quot; and the text to search is &quot;aaaaab&quot;, 
    the match could be of length 1 or 5.  If you specify
    <span class="Monospace">RegExpr_MatchLargestNumChars</span>, the match length will be 5.
    Otherwise, it will be 1.<br><br>
   
    If the pattern is &quot;a&#43;b&quot; and the text to search is &quot;ababab&quot;, 
    the match could be of length 2 or 6.  If you specify
    <span class="Monospace">RegExpr_MatchLargestNumChars</span>, the match length will be 6.
    Otherwise, it will be 2.<br><br>

Values:<br>

    <table class="Borderless">
    <tr><td><span class="Monospace">RegExpr_MatchLargestNumChars</span>  1</td>   
    <td>(Any non&ndash;zero value can be used)</td></tr>

    <tr><td><span class="Monospace">RegExpr_MatchSmallestNumChars</span> 0</td></tr>  </table>

</td>
</tr>
</table>                                           
    
</body>
</html>    h k P �  �    Text to Search                    � k2     `    Text Length                       � �;     `    Match Length                       %���    `    Parse Status                      *� � �     `    Match Position                    -^ � q          Direction                         0� % O  �  �    Regular Expression Text           =F %T          Case Sensitive?                   B1 � !     `    Matched?                          DE � �          Match Policy                       ""        	            	            	          B Forwards RegExpr_SearchForwards Backwards RegExpr_SearchBackwards    ""   Yes 1 No 0    	          z  Match Largest Number of  Chars RegExpr_MatchLargestNumChars Match Smallest Number of  Chars RegExpr_MatchSmallestNumChars   >    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_GetErrorElaboration</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">RegExpr_GetErrorElaboration</h1>

<p class="syntax">char *RegExpr_GetErrorElaboration ();</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function returns a string describing the result of the last call to <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_Parse.htm')">RegExpr_Parse</a></span>.</p>

<p class="Body">The string is held in a static buffer and will be overwritten by the next call to this function.</p>
    
                                           
    
</body>
</html>        <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_GetErrorElaboration</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">errorElaborationString</td>
<td class="paramDataType">char&nbsp;*</td>

<td>A string describing the result of the last call to <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_Parse.htm')">RegExpr_Parse</a></span>.<br><br>

The string is held in a static buffer and will be overwritten by the next call to this function.</td>
</tr>
</table>
                                           
    
</body>
</html>    P, e F��  �    Error Elaboration String           	           X    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_GetErrorString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">RegExpr_GetErrorString</h1>

<p class="syntax">char *RegExpr_GetErrorString (int errorNumber);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function takes an error number returned by the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_Parse.htm')">RegExpr_Parse</a></span> function and returns a text description of the error.</p>   

<p class="Body">For some errors, there may be more error information than can be encoded in the error number.  The <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_GetErrorElaboration.htm')">RegExpr_GetErrorElaboration</a></span> function returns a more complete error message in that case.</p>
    
                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_GetErrorString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">errorNumber</td>
<td class="paramDataType">integer</td>

<td>An error number returned by the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_Parse.htm')">RegExpr_Parse</a></span> function.</td>
</tr>
</table>
    
                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_GetErrorString</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">errorString</td>
<td class="paramDataType">char&nbsp;*</td>

<td>The error string corresponding to the error number.</td>
</tr>
</table>                                
    
</body>
</html>    W� B d      `    Error Number                      Zh � d��  �    Error String                       0    	           �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_Parse</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">RegExpr_Parse</h1>

<p class="syntax">int RegExpr_Parse (const char *regularExpressionText, int caseSensitive, int *regularExpressionHandle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function parses a regular expression string.</p>

<p class="Body">If the string is a valid regular expression, a value is placed into the &#39;handle&#39; parameter, which can then be passed to the following functions:<br><br>

    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_MatchText.htm')">RegExpr_MatchText</a></span><br>
     <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_Destroy.htm')">RegExpr_Destroy</a></span><br><br>

If the string is not a valid regular expression, a negative error number is returned.  You can pass this error number to the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_GetErrorString.htm')">RegExpr_GetErrorString</a></span> function.  However, in some cases there is more error information than can be encoded in the error number.  You can get more detailed information about the result of the last call to this function by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_GetErrorElaboration.htm')">RegExpr_GetErrorElaboration</a></span>.</p>
    
                                           
    
</body>
</html>    h    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_Parse</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">regularExpressionText</td>
<td class="paramDataType">const&nbsp;char&nbsp;*</td>

<td>A nul&ndash;terminated string containing a regular expression.<br><br>

A regular expression consists of the following tokens:<br>

<table class="Borderless">
<tr><td>. (period)</td>
<td>match 1 character </td>
<td>a.t matches act and apt but not abort or at</td>
</tr>
<tr><td>&#42; (asterisk)</td>
<td>match 0 or more occurrences of the preceding character or &#123;expression&#125; </td>      
<td>0&#42;1 matches 1, 01, 001, etc. a.&#42; matches act, apt, abort, and at</td>
</tr>
<tr><td>&#43; (plus sign)</td>
<td>match 1 or more occurrences of the preceding character or &#123;expression&#125;</td>       
<td>0&#43;1 matches 01, 001, 0001, etc. &#123;ab&#125;&#43;c matches abc, ababc, but not c</td>                 
</tr>
<tr>                   
<td>&#63; (question mark)</td>
<td>match 0 or 1 occurrences of the preceding character or &#123;expression&#125;</td>
<td>0&#63;1 matches 1, 01, but not 001</td>
</tr>
<tr><td>&#124; (pipe)</td>        
<td>matches either the preceding or following character or &#123;expression&#125;</td>
<td>a3&#124;4b matches a3b or a4b</td>
</tr>
<tr><td>&#94; (caret)</td>
<td>matches the beginning of a line</td>
<td>&#94;int matches any line that begins with int</td>   
</tr>
<tr><td>&#36; (dollar sign)</td>
<td>matches the end of a line</td>
<td>done&#36; matches any line that ends with done</td>
</tr>
<tr><td>&#123;&#125; (curly braces)</td>          
<td>groups characters or expressions</td>        
<td>&#123;a3&#125;&#124;&#123;4b&#125; matches a3 or 4b</td>                    
</tr>
<tr><td>&#91;&#93; (brackets)</td>   
<td>matches any one character or range listed within the brackets</td>
<td>&#91;a&ndash;z&#93; matches every occurrence of lowercase letters &#91;abc&#93; matches every occurrence of a, b or c</td>
</tr>
<tr><td>&#126;  (tilde)</td>
<td>when appearing immediately after the left bracket, negates the contents of the set </td>       
<td>&#91;&#126;a&ndash;z&#93; matches anything except lowercase letters &#91;a&ndash;z&#126;A&ndash;Z&#93; matches all letters and the &#39;&#126;&#39; character</td>
</tr>
<tr><td>&#92;t&nbsp;(backslash&nbsp;t)</td>  
<td>matches any tab character</td>
<td>&#92;t3 matches every occurrence of a tab followed by a 3</td>
</tr>
<tr><td>&#92;x&nbsp;(backslash&nbsp;x)</td>  
<td>matches any character specified in hex</td>       
<td>&#92;x2a matches every occurrence of the &#39;*&#39; specified in hexcharacter</td>
</tr>
<tr><td>&#92;&nbsp;(backslash)</td>   
<td>used if any of the above characters themselves are to be included in the search</td>
<td>&#92;&ndash;&#92;&#63;&#92;&#92; matches every occurrence of &#39;&ndash;&#39; followed by &#39;&#63;&#39; and &#39;&#92;&#39;</td>
</tr>    
</table>

</td>
</tr>
</table>                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_Parse</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">caseSensitive</td>
<td class="paramDataType">integer</td>

<td>Specifies whether the matching of characters is to be done on a case&ndash;senstive or case&ndash;insensitive basis.<br><br>

A non&ndash;zero value specifies that characters are to be matched on a case&ndash;sensitive basis.  For example. &quot;chr&quot; would match only to &quot;chr&quot; and not to &quot;CHR&quot;.<br><br>

A zero value specifies that characters are to be matched on a case&ndash;insensitive basis.  For example. &quot;chr&quot; would match to &quot;chr&quot;, &quot;CHR&quot;, and &quot;Chr&quot;.<br><br>

This parameter does apply to ranges.  For example, if this parameter is non&ndash;zero, then &quot;&#91;a&ndash;z&#93;&quot; in the regular expression string would match to any lowercase letter.  If this parameter is zero, then &quot;&#91;a&ndash;z&#93;&quot; would match to any letter.</td>
</tr>
</table>
                                           
    
</body>
</html>    u    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_Parse</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">regularExpressionHandle</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>A handle that represents the parse regular expression.<br><br>

It can be passed to the following functions:<br><br>

    <span class="Monospace">RegExpr_GetFirstCharVec</span><br>
    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_MatchText.htm')">RegExpr_MatchText</a></span><br>
    <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_Destroy.htm')">RegExpr_Destroy</a></span><br><br>

When you are done with the regular expression, you should call <span class="Monospace">RegExpr_Destroy</span> on the handle.  Otherwise, you will lose some memory.</td>
</tr>
</table>
                                           
    
</body>
</html>    
�    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_Parse</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">parseStatus</td>
<td class="paramDataType">integer</td>

<td>Indicates if the regular expression was parsed successfully.<br><br>

If the string is not a valid regular expression, a negative error number is returned.  You can pass this error number to the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_GetErrorString.htm')">RegExpr_GetErrorString</a></span> function.  However, in some cases there is more error information than can be encoded in the error number.  You can get more detailed information about the result of the last call to this function by calling <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_GetErrorElaboration.htm')">RegExpr_GetErrorElaboration</a></span>.<br><br>

The error numbers are:<br><br>

<table class="Borderless">
       <tr><td>0</td> <td>success</td></tr>
    <tr><td><span class="nobreak">&ndash;12</span></td> <td>out of memory</td></tr>
   <tr><td><span class="nobreak">&ndash;7900</span></td> <td>unmatched character</td></tr>
   <tr><td><span class="nobreak">&ndash;7901</span></td> <td>invalid character in range</td></tr> 
   <tr><td><span class="nobreak">&ndash;7902</span></td> <td>regular expression ends with backslash</td></tr>
  <tr><td><span class="nobreak">&ndash;7903</span></td> <td>invalid hex character &#40;after \x&#41;</td></tr>
   <tr><td><span class="nobreak">&ndash;7904</span></td> <td>operator applied to an empty pattern</td></tr>
   <tr><td><span class="nobreak">&ndash;7905</span></td> <td>empty left side of &#39;&#124;&#39;</td></tr>
   <tr><td><span class="nobreak">&ndash;7906</span></td> <td>empty right side of &#39;&#124;&#39;</td></tr>
   <tr><td><span class="nobreak">&ndash;7907</span></td> <td>empty group</td></tr>
   <tr><td><span class="nobreak">&ndash;7908</span></td> <td>invalid range</td></tr>
   <tr><td><span class="nobreak">&ndash;7909</span></td> <td>empty set &#40;&#91;&#93;&#41;</td></tr>
   <tr><td><span class="nobreak">&ndash;7910</span></td> <td>empty input string</td></tr>
   <tr><td><span class="nobreak">&ndash;7911</span></td> <td>NULL input string</td></tr>
   <tr><td><span class="nobreak">&ndash;7912</span></td> <td>multibyte characters not allowed in range</td></tr></table>
    
 </td>
</tr>
</table>                                          
    
</body>
</html>    c� 0   �  �    Regular Expression Text           pG 0 �          Case Sensitive?                   u2 0~     �    Regular Expression Handle         y� ����    `    Parse Status                       ""   Yes 1 No 0    	           	               <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_MatchText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">RegExpr_MatchText</h1>

<p class="syntax">int RegExpr_MatchText (int regularExpressionHandle, const char *textToMatch, int textLength, int matchPolicy, int *matchLength);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">Determines if the specified text begins with a pattern that matches to the regular expression.</p>

<p class="Body">The regular expression must match from the beginning of the text.  If you want to search for a regular expression match anywhere in the text, you must call this function iteratively, each time passing the address of the next character in the text.</p>

<p class="Body">See the Low&ndash;level class help for example source code showing how to use this function.</p>

    
                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_MatchText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">regularExpressionHandle</td>
<td class="paramDataType">integer</td>

<td>A handle returned from the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_Parse.htm')">RegExpr_Parse</a></span> function.</td>
</tr>
</table>
    
                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_MatchText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">textToMatch</td>
<td class="paramDataType">const&nbsp;char&nbsp;*</td>

<td>The text to match to the regular expression.<br><br>

The regular expression must match from the beginning of the text.  If you want to search for a regular expression match anywhere in the text, you must call this function iteratively, each time passing the address of the next character in the text.
See the Instrument Help for example source code.  The function <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_FindPatternInText.htm')">RegExpr_FindPatternInText</a></span> performs this for you.</td>
</tr>
</table>
                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_MatchText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">textLength</td>
<td class="paramDataType">integer</td>

<td>The number of bytes in the text you want to be included to the attempt to match the regular expression.<br><br>

If you pass a value less than zero, the text is assumed to be nul&ndash;terminated, and the full length is used.</td>
</tr>
</table>
                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_MatchText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">matchLength</td>
<td class="paramDataType">integer&nbsp;*</td>

<td>This parameter is set to the number of characters in the text that actually matched to the regular expression.<br><br>

If there was no match, then the parameter is set to <span class="nobreak">&ndash;1</span>.</td>
</tr>
</table>
                                           
    
</body>
</html>    �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_MatchText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">matchStatus</td>
<td class="paramDataType">integer</td>

<td>Returns an indication of whether the regular expression matched the text.<br><br>

Values:<br>  

       <table class="Borderless">
       <tr><td>1</td>     <td>matched</td></tr>
     
       <tr><td>0</td>     <td>did not match</td></tr>

      <tr><td><span class="nobreak">&ndash;1</span></td>     <td>did not match, and it was also determined that all attempts to find a match starting at subsequent bytes in the text will also fail.</td></tr></table><br>

The <span class="nobreak">&ndash;1</span> return value is provided as an optimization.  In some cases, the function can determine that it is useless to try to find a match anywhere in the text on or after the text address that was passed to the function.  In that case, the function returns <span class="nobreak">&ndash;1</span>.  If you are calling this function iteratively until you find a match, you can break out of your loop if the function returns <span class="nobreak">&ndash;1</span>.  If you do not need to optimize, then you can treat 0 as the same.<br><br>

<table class="Borderless-Wide">
<tr>
<td class="Icon"><img src="note.gif"></td>
<td>
<strong>Note</strong>&nbsp;&nbsp; A return value of <span class="nobreak">0</span> does NOT necessarily indicate that there is a match later in the text.  
</td></tr></table>
      
</td>
</tr>
</table>  
                                           
    
</body>
</html>    <    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_MatchText</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">matchPolicy</td>
<td class="paramDataType">integer</td>

<td>Specifies whether the maximum or minimum number of characters are matched to the pattern.<br><br>

Examples:<br><br>

If the pattern is &quot;a&#43;&quot; and the text to search is &quot;aaaaab&quot;, 
    the match could be of length 1 or 5.  If you specify
    <span class="Monospace">RegExpr_MatchLargestNumChars</span>, the match length will be 5.
    Otherwise, it will be 1.<br><br>
   
If the pattern is &quot;a&#43;b&quot; and the text to search is &quot;ababab&quot;, 
    the match could be of length 2 or 6.  If you specify
    <span class="Monospace">RegExpr_MatchLargestNumChars</span>, the match length will be 6.
    Otherwise, it will be 2.<br><br>

Values:<br>

    <table class="Borderless">
    <tr><td><span class="Monospace">RegExpr_MatchLargestNumChars</span>  1</td>   
    <td>(Any non&ndash;zero value can be used)</td></tr>

    <tr><td><span class="Monospace">RegExpr_MatchSmallestNumChars</span> 0</td></tr>  </table>
  
</td>
</tr>
</table>
  
</body>
</html>    �R 8 3      �    Regular Expression Handle         �� 8 � �  �    Text to Match                     �� 8�     `    Text Length                       �� ~�     `    Match Length                      � ����    `    Match Status                      �� � Y          Match Policy                           ""        	            	          z  Match Largest Number of  Chars RegExpr_MatchLargestNumChars Match Smallest Number of  Chars RegExpr_MatchSmallestNumChars   �    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_Destroy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<h1 class="function">RegExpr_Destroy</h1>

<p class="syntax">void RegExpr_Destroy (int regularExpressionHandle);</p>

<h2 class="purpose">Purpose</h2>
<p class="Body">This function retrieves the memory that was allocated by <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_Parse.htm')">RegExpr_Parse</a></span> to store the parsed regular expression.</p>
      
</body>
</html>    U    <html>
<head>
<script src="launchhelp.js"></script>
<META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
<title>RegExpr_Destroy</title>
<LINK REL="StyleSheet" HREF="styles.css" type="text/css">
</head>

<body>
<table class=borderless><TR>

     <td class="paramName">regularExpressionHandle</td>
<td class="paramDataType">integer</td>

<td>A handle returned from the <span class="Monospace"><a href="javascript:LaunchMergedHelp('cvi.chm', 'toolslib.chm', 'FunctionReference/cviRegExpr_Parse.htm')">RegExpr_Parse</a></span> function.</td>
</tr>
</table>
      
</body>
</html>    �� : �      �    Regular Expression Handle           ����         (  I� 
    K.    FindPatternInText               ����         L�  S;     K.    GetErrorElaboration             ����         S|  \U     K.    GetErrorString                  ����         \�  �B     K.    Parse                           ����         �B  �,     K.    MatchText                       ����         �  �     K.    Destroy                                �                                     DFind Pattern In Text                 DGet Error Elaboration                DGet Error String                    �Low-level                            DParse Regular Expression             DMatch Text                           DDestroy Regular Expression            toolbox.fp 