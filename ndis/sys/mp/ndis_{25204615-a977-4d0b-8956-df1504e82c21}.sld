<?xml version="1.0" encoding="UTF-16"?>
<!DOCTYPE DCARRIER SYSTEM "mantis.dtd" [
<!-- RegKey registry types (map to REG_SZ etc) -->
<!ENTITY RegTypeNone                        "0" >
<!ENTITY RegTypeSz                          "1" >
<!ENTITY RegTypeExpandSz                    "2" >
<!ENTITY RegTypeBinary                      "3" >
<!ENTITY RegTypeDword                       "4" >
<!ENTITY RegTypeDwordBigEndian              "5" >
<!ENTITY RegTypeLink                        "6" >
<!ENTITY RegTypeMultiSz                     "7" >
<!ENTITY RegTypeResourceList                "8" >
<!ENTITY RegTypeFullResourceDescriptor      "9" >
<!ENTITY RegTypeResourceRequirementsList    "10" >
<!ENTITY RegTypeQword                       "11" >

<!-- RegKey registry operations -->
<!ENTITY RegOpWrite               "1" >
<!ENTITY RegOpDelete              "2" >
<!ENTITY RegOpEdit                "3" >

<!-- RegKey registry conditionals -->
<!ENTITY RegCondAlways            "1" >
<!ENTITY RegCondIfExists          "2" >
<!ENTITY RegCondIfNotExists       "3" >

<!-- RawDep dependency types -->
<!ENTITY RawDepNone               "None" >
<!ENTITY RawDepCLSID              "CLSID" >
<!ENTITY RawDepFile               "File" >
<!ENTITY RawDepRegKey             "RegKey" >
<!ENTITY RawDepRegValue           "RegValue" >
<!ENTITY RawDepRegPath            "RegPath" >
]>
<DCARRIER CarrierRevision="1">
	<TOOLINFO ToolName="iCat"><![CDATA[<?xml version="1.0"?>
<!DOCTYPE TOOL SYSTEM "file://mess/icat/tool.dtd">
<TOOL>
	<CREATED><NAME>iCat</NAME><VSGUID>{f802f157-4623-426a-8b39-ac2eb4332c74}</VSGUID><VERSION>1.0.0.244</VERSION><BUILD>244</BUILD><DATE>4/17/2000</DATE></CREATED><LASTSAVED><NAME>iCat</NAME><VSGUID>{97b86ee0-259c-479f-bc46-6cea7ef4be4d}</VSGUID><VERSION>1.0.0.452</VERSION><BUILD>452</BUILD><DATE>3/12/2001</DATE></LASTSAVED></TOOL>
]]></TOOLINFO><COMPONENT ComponentVSGUID="{25204615-A977-4D0B-8956-DF1504E82C21}" ComponentVIGUID="{747A25F5-197E-4893-9BAA-0BC2285AD409}" PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}" RepositoryVSGUID="{8E0BE9ED-7649-47F3-810B-232D36C430B4}" Revision="4" Visibility="200" MultiInstance="0" Released="1" Editable="0" HTMLFinal="0" IsMacro="0" HTMLTitle="" PrototypeVIGUID=""><HELPCONTEXT src="H:\nt\net\ndis\sys\mp\ndis.htm">&lt;html xmlns:v="urn:schemas-microsoft-com:vml"
xmlns:o="urn:schemas-microsoft-com:office:office"
xmlns:w="urn:schemas-microsoft-com:office:word"
xmlns="http://www.w3.org/TR/REC-html40"&gt;

&lt;head&gt;
&lt;meta http-equiv=Content-Type content="text/html; charset=windows-1252"&gt;
&lt;meta name=ProgId content=Word.Document&gt;
&lt;meta name=Generator content="Microsoft Word 9"&gt;
&lt;meta name=Originator content="Microsoft Word 9"&gt;
&lt;link rel=File-List href="./ndis_files/filelist.xml"&gt;
&lt;link rel=Edit-Time-Data href="./ndis_files/editdata.mso"&gt;
&lt;title&gt;NDIS Component Help&lt;/title&gt;
&lt;!--[if gte mso 9]&gt;&lt;xml&gt;
 &lt;o:DocumentProperties&gt;
  &lt;o:Author&gt;SStonich&lt;/o:Author&gt;
  &lt;o:Description&gt;Contact name: arvindm&lt;/o:Description&gt;
  &lt;o:Template&gt;comptemp1&lt;/o:Template&gt;
  &lt;o:LastAuthor&gt;Arvind Murching&lt;/o:LastAuthor&gt;
  &lt;o:Revision&gt;2&lt;/o:Revision&gt;
  &lt;o:TotalTime&gt;2&lt;/o:TotalTime&gt;
  &lt;o:LastPrinted&gt;2000-10-23T19:48:00Z&lt;/o:LastPrinted&gt;
  &lt;o:Created&gt;2000-11-13T20:48:00Z&lt;/o:Created&gt;
  &lt;o:LastSaved&gt;2000-11-13T20:48:00Z&lt;/o:LastSaved&gt;
  &lt;o:Pages&gt;1&lt;/o:Pages&gt;
  &lt;o:Words&gt;90&lt;/o:Words&gt;
  &lt;o:Characters&gt;515&lt;/o:Characters&gt;
  &lt;o:Company&gt;MSFT&lt;/o:Company&gt;
  &lt;o:Lines&gt;4&lt;/o:Lines&gt;
  &lt;o:Paragraphs&gt;1&lt;/o:Paragraphs&gt;
  &lt;o:CharactersWithSpaces&gt;632&lt;/o:CharactersWithSpaces&gt;
  &lt;o:Version&gt;9.3821&lt;/o:Version&gt;
 &lt;/o:DocumentProperties&gt;
&lt;/xml&gt;&lt;![endif]--&gt;&lt;!--[if gte mso 9]&gt;&lt;xml&gt;
 &lt;w:WordDocument&gt;
  &lt;w:ActiveWritingStyle Lang="EN-US" VendorID="8" DLLVersion="513" NLCheck="0"&gt;1&lt;/w:ActiveWritingStyle&gt;
  &lt;w:UpdateStylesOnOpen/&gt;
  &lt;w:TrackRevisions/&gt;
  &lt;w:DisplayHorizontalDrawingGridEvery&gt;0&lt;/w:DisplayHorizontalDrawingGridEvery&gt;
  &lt;w:DisplayVerticalDrawingGridEvery&gt;0&lt;/w:DisplayVerticalDrawingGridEvery&gt;
  &lt;w:UseMarginsForDrawingGridOrigin/&gt;
  &lt;w:Compatibility&gt;
   &lt;w:FootnoteLayoutLikeWW8/&gt;
   &lt;w:ShapeLayoutLikeWW8/&gt;
   &lt;w:AlignTablesRowByRow/&gt;
   &lt;w:ForgetLastTabAlignment/&gt;
   &lt;w:LayoutRawTableWidth/&gt;
   &lt;w:LayoutTableRowsApart/&gt;
  &lt;/w:Compatibility&gt;
 &lt;/w:WordDocument&gt;
&lt;/xml&gt;&lt;![endif]--&gt;&lt;![if !supportAnnotations]&gt;
&lt;style id="dynCom" type="text/css"&gt;&lt;!-- --&gt;&lt;/style&gt;
&lt;script language="JavaScript"&gt;&lt;!--
function msoCommentShow(anchor_id, com_id)
{
	if(msoBrowserCheck()) 
		{
		c = document.all(com_id);
		if (null != c)
			{
			a = document.all(anchor_id);
			var cw = c.offsetWidth;
			var ch = c.offsetHeight;
			var aw = a.offsetWidth;
			var ah = a.offsetHeight;
			var x  = a.offsetLeft;
			var y  = a.offsetTop;
			var el = a;
			while (el.tagName != "BODY") 
				{
				el = el.offsetParent;
				x = x + el.offsetLeft;
				y = y + el.offsetTop;
				}
			var bw = document.body.clientWidth;
			var bh = document.body.clientHeight;
			var bsl = document.body.scrollLeft;
			var bst = document.body.scrollTop;
			if (x + cw + ah / 2 &gt; bw + bsl &amp;&amp; x + aw - ah / 2 - cw &gt;= bsl ) 
				{ c.style.left = x + aw - ah / 2 - cw; }
			else 
				{ c.style.left = x + ah / 2; }
			if (y + ch + ah / 2 &gt; bh + bst &amp;&amp; y + ah / 2 - ch &gt;= bst ) 
				{ c.style.top = y + ah / 2 - ch; }
			else 
				{ c.style.top = y + ah / 2; }
			c.style.visibility = "visible";
}	}	}
function msoCommentHide(com_id) 
{
	if(msoBrowserCheck())
		{
		c = document.all(com_id);
		if (null != c)
		{
		c.style.visibility = "hidden";
		c.style.left = -1000;
		c.style.top = -1000;
		} } 
}
function msoBrowserCheck()
{
	ms = navigator.appVersion.indexOf("MSIE");
	vers = navigator.appVersion.substring(ms + 5, ms + 6);
	ie4 = (ms &gt; 0) &amp;&amp; (parseInt(vers) &gt;= 4);
	return ie4;
}
if (msoBrowserCheck())
{
	document.styleSheets.dynCom.addRule(".msocomanchor","background: infobackground");
	document.styleSheets.dynCom.addRule(".msocomoff","display: none");
	document.styleSheets.dynCom.addRule(".msocomtxt","visibility: hidden");
	document.styleSheets.dynCom.addRule(".msocomtxt","position: absolute");
	document.styleSheets.dynCom.addRule(".msocomtxt","top: -1000");
	document.styleSheets.dynCom.addRule(".msocomtxt","left: -1000");
	document.styleSheets.dynCom.addRule(".msocomtxt","width: 33%");
	document.styleSheets.dynCom.addRule(".msocomtxt","background: infobackground");
	document.styleSheets.dynCom.addRule(".msocomtxt","color: infotext");
	document.styleSheets.dynCom.addRule(".msocomtxt","border-top: 1pt solid threedlightshadow");
	document.styleSheets.dynCom.addRule(".msocomtxt","border-right: 2pt solid threedshadow");
	document.styleSheets.dynCom.addRule(".msocomtxt","border-bottom: 2pt solid threedshadow");
	document.styleSheets.dynCom.addRule(".msocomtxt","border-left: 1pt solid threedlightshadow");
	document.styleSheets.dynCom.addRule(".msocomtxt","padding: 3pt 3pt 3pt 3pt");
}
// --&gt;&lt;/script&gt;
&lt;![endif]&gt;
&lt;style&gt;
&lt;!--
 /* Font Definitions */
@font-face
	{font-family:Wingdings;
	panose-1:5 0 0 0 0 0 0 0 0 0;
	mso-font-charset:2;
	mso-generic-font-family:auto;
	mso-font-pitch:variable;
	mso-font-signature:0 268435456 0 0 -2147483648 0;}
@font-face
	{font-family:Verdana;
	panose-1:2 11 6 4 3 5 4 4 2 4;
	mso-font-charset:0;
	mso-generic-font-family:swiss;
	mso-font-pitch:variable;
	mso-font-signature:536871559 0 0 0 415 0;}
@font-face
	{font-family:"Microsoft Logo 95";
	panose-1:0 0 0 0 0 0 0 0 0 0;
	mso-font-alt:Symbol;
	mso-font-charset:2;
	mso-generic-font-family:auto;
	mso-font-format:other;
	mso-font-pitch:variable;
	mso-font-signature:0 0 0 0 0 0;}
 /* Style Definitions */
p.MsoNormal, li.MsoNormal, div.MsoNormal
	{mso-style-parent:"";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:yellow;}
h1
	{mso-style-parent:"";
	mso-style-next:"Text\,t";
	margin-top:9.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:18.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:1;
	font-size:16.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	color:black;
	mso-font-kerning:12.0pt;
	mso-bidi-font-weight:normal;}
h2
	{mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
	mso-style-next:"Text\,t";
	margin-top:9.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:18.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:2;
	font-size:16.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	color:gray;
	mso-font-kerning:12.0pt;
	mso-bidi-font-weight:normal;}
h3
	{mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
	mso-style-next:"Text\,t";
	margin-top:9.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:18.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:3;
	font-size:16.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	color:silver;
	mso-font-kerning:12.0pt;
	mso-bidi-font-weight:normal;}
h4
	{mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
	mso-style-next:"Text\,t";
	margin-top:9.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:16.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:4;
	font-size:14.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	color:black;
	mso-font-kerning:12.0pt;
	mso-bidi-font-weight:normal;}
h5
	{mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
	mso-style-next:"Text\,t";
	margin-top:9.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:15.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:5;
	font-size:13.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	color:black;
	mso-font-kerning:12.0pt;
	mso-bidi-font-weight:normal;}
h6
	{mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
	mso-style-next:"Text\,t";
	margin-top:9.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:12.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:6;
	font-size:10.0pt;
	font-family:Verdana;
	color:black;
	mso-font-kerning:12.0pt;
	mso-bidi-font-weight:normal;}
p.MsoIndex1, li.MsoIndex1, div.MsoIndex1
	{mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:9.0pt;
	text-indent:-9.0pt;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
p.MsoIndex2, li.MsoIndex2, div.MsoIndex2
	{mso-style-parent:"Index 1\,idx1";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:27.0pt;
	text-indent:-9.0pt;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
p.MsoIndex3, li.MsoIndex3, div.MsoIndex3
	{mso-style-parent:"Index 1\,idx1";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:45.0pt;
	text-indent:-9.0pt;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
p.MsoToc1, li.MsoToc1, div.MsoToc1
	{mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:12.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	tab-stops:.25in;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;
	mso-font-kerning:12.0pt;
	font-weight:bold;
	mso-bidi-font-weight:normal;}
p.MsoToc2, li.MsoToc2, div.MsoToc2
	{mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
p.MsoToc3, li.MsoToc3, div.MsoToc3
	{mso-style-parent:"TOC 2\,toc2";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.5in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
p.MsoToc4, li.MsoToc4, div.MsoToc4
	{mso-style-parent:"TOC 2\,toc2";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.75in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
p.MsoFootnoteText, li.MsoFootnoteText, div.MsoFootnoteText
	{mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:red;}
p.MsoCommentText, li.MsoCommentText, div.MsoCommentText
	{mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.MsoHeader, li.MsoHeader, div.MsoHeader
	{mso-style-parent:"Footer\,f";
	margin:0in;
	margin-bottom:.0001pt;
	text-align:right;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
p.MsoFooter, li.MsoFooter, div.MsoFooter
	{mso-style-parent:"";
	margin:0in;
	margin-bottom:.0001pt;
	text-align:right;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
p.MsoIndexHeading, li.MsoIndexHeading, div.MsoIndexHeading
	{mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
	mso-style-next:"Index 1\,idx1";
	margin-top:9.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:15.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	page-break-after:avoid;
	mso-outline-level:5;
	font-size:13.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;
	mso-font-kerning:12.0pt;
	font-weight:bold;
	mso-bidi-font-weight:normal;}
span.MsoFootnoteReference
	{color:red;
	vertical-align:super;}
span.MsoPageNumber
	{mso-ansi-font-size:8.0pt;
	mso-ascii-font-family:Verdana;
	mso-hansi-font-family:Verdana;
	color:maroon;
	font-weight:bold;}
p.Text, li.Text, div.Text
	{mso-style-name:"Text\,t";
	mso-style-parent:"";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.Figure, li.Figure, div.Figure
	{mso-style-name:"Figure\,fig";
	mso-style-parent:"Text\,t";
	mso-style-next:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:9.0pt;
	margin-left:0in;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.Code, li.Code, div.Code
	{mso-style-name:"Code\,c";
	mso-style-parent:"";
	margin-top:0in;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:15.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:"Courier New";
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.LabelinList2, li.LabelinList2, div.LabelinList2
	{mso-style-name:"Label in List 2\,l2";
	mso-style-parent:"Text in List 2\,t2";
	mso-style-next:"Text in List 2\,t2";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.5in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;
	font-weight:bold;
	mso-bidi-font-weight:normal;}
p.TextinList2, li.TextinList2, div.TextinList2
	{mso-style-name:"Text in List 2\,t2";
	mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.5in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.Label, li.Label, div.Label
	{mso-style-name:"Label\,l";
	mso-style-parent:"Text\,t";
	mso-style-next:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;
	font-weight:bold;
	mso-bidi-font-weight:normal;}
p.NumberedList2, li.NumberedList2, div.NumberedList2
	{mso-style-name:"Numbered List 2\,nl2";
	mso-style-parent:"Text in List 2\,t2";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.5in;
	text-indent:-.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	mso-list:l43 level1 lfo63;
	tab-stops:list .5in;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.Syntax, li.Syntax, div.Syntax
	{mso-style-name:"Syntax\,s";
	mso-style-parent:"Code\,c";
	margin-top:0in;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:15.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:"Courier New";
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.TableFootnote, li.TableFootnote, div.TableFootnote
	{mso-style-name:"Table Footnote\,tf";
	mso-style-parent:"Text\,t";
	mso-style-next:"Text\,t";
	margin-top:2.0pt;
	margin-right:0in;
	margin-bottom:4.0pt;
	margin-left:0in;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	border:none;
	mso-border-top-alt:solid windowtext .5pt;
	padding:0in;
	mso-padding-alt:1.0pt 0in 0in 0in;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
span.CodeEmbedded
	{mso-style-name:"Code Embedded\,ce";
	mso-ansi-font-size:10.0pt;
	mso-ascii-font-family:"Courier New";
	mso-hansi-font-family:"Courier New";}
span.LabelEmbedded
	{mso-style-name:"Label Embedded\,le";
	mso-ansi-font-size:10.0pt;
	mso-ascii-font-family:Verdana;
	mso-hansi-font-family:Verdana;
	font-weight:bold;
	text-decoration:none;
	text-underline:none;}
span.LinkText
	{mso-style-name:"Link Text\,lt";
	color:green;
	text-decoration:underline;
	text-underline:double;}
span.LinkTextPopup
	{mso-style-name:"Link Text Popup\,ltp";
	color:green;
	text-decoration:underline;
	text-underline:single;}
span.LinkID
	{mso-style-name:"Link ID\,lid";
	color:red;
	display:none;
	mso-hide:all;}
p.TableSpacingAfter, li.TableSpacingAfter, div.TableSpacingAfter
	{mso-style-name:"Table Spacing After\,tsa";
	mso-style-parent:"Text\,t";
	mso-style-next:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:0in;
	margin-left:0in;
	margin-bottom:.0001pt;
	line-height:6.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:6.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:yellow;}
p.CodeinList2, li.CodeinList2, div.CodeinList2
	{mso-style-name:"Code in List 2\,c2";
	mso-style-parent:"Code\,c";
	margin-top:0in;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.5in;
	line-height:15.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:"Courier New";
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
span.ConditionalMarker
	{mso-style-name:"Conditional Marker\,cm";
	mso-ansi-font-size:10.0pt;
	mso-ascii-font-family:"Courier New";
	mso-hansi-font-family:"Courier New";
	color:black;
	display:none;
	mso-hide:all;
	border:none;
	background:#FFFFBF;
	mso-shading:windowtext;
	mso-pattern:gray-375 yellow;}
p.FigureinList2, li.FigureinList2, div.FigureinList2
	{mso-style-name:"Figure in List 2\,fig2";
	mso-style-parent:"Figure\,fig";
	mso-style-next:"Text in List 2\,t2";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:9.0pt;
	margin-left:.5in;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.FigureEmbedded, li.FigureEmbedded, div.FigureEmbedded
	{mso-style-name:"Figure Embedded\,fige";
	mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:9.0pt;
	margin-left:0in;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.TableFootnoteinList2, li.TableFootnoteinList2, div.TableFootnoteinList2
	{mso-style-name:"Table Footnote in List 2\,tf2";
	mso-style-parent:"Text in List 2\,t2";
	mso-style-next:"Text in List 2\,t2";
	margin-top:2.0pt;
	margin-right:0in;
	margin-bottom:4.0pt;
	margin-left:.5in;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	border:none;
	mso-border-top-alt:solid windowtext .5pt;
	padding:0in;
	mso-padding-alt:1.0pt 0in 0in 0in;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.LabelinList1, li.LabelinList1, div.LabelinList1
	{mso-style-name:"Label in List 1\,l1";
	mso-style-parent:"Text in List 1\,t1";
	mso-style-next:"Text in List 1\,t1";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;
	font-weight:bold;
	mso-bidi-font-weight:normal;}
p.TextinList1, li.TextinList1, div.TextinList1
	{mso-style-name:"Text in List 1\,t1";
	mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.CodeinList1, li.CodeinList1, div.CodeinList1
	{mso-style-name:"Code in List 1\,c1";
	mso-style-parent:"Code\,c";
	margin-top:0in;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.25in;
	line-height:15.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:"Courier New";
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.FigureinList1, li.FigureinList1, div.FigureinList1
	{mso-style-name:"Figure in List 1\,fig1";
	mso-style-parent:"Figure\,fig";
	mso-style-next:"Text in List 1\,t1";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:9.0pt;
	margin-left:.25in;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.TableFootnoteinList1, li.TableFootnoteinList1, div.TableFootnoteinList1
	{mso-style-name:"Table Footnote in List 1\,tf1";
	mso-style-parent:"Text in List 1\,t1";
	mso-style-next:"Text in List 1\,t1";
	margin-top:2.0pt;
	margin-right:0in;
	margin-bottom:4.0pt;
	margin-left:.25in;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	border:none;
	mso-border-top-alt:solid windowtext .5pt;
	padding:0in;
	mso-padding-alt:1.0pt 0in 0in 0in;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
span.HTML
	{mso-style-name:HTML;
	mso-ansi-font-size:10.0pt;
	mso-ascii-font-family:"Courier New";
	mso-hansi-font-family:"Courier New";
	color:black;
	display:none;
	mso-hide:all;
	border:none;
	background:#DDFFDD;
	mso-shading:windowtext;
	mso-pattern:gray-25 lime;}
p.AlertText, li.AlertText, div.AlertText
	{mso-style-name:"Alert Text\,at";
	mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.AlertTextinList1, li.AlertTextinList1, div.AlertTextinList1
	{mso-style-name:"Alert Text in List 1\,at1";
	mso-style-parent:"Text in List 1\,t1";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.5in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.AlertTextinList2, li.AlertTextinList2, div.AlertTextinList2
	{mso-style-name:"Alert Text in List 2\,at2";
	mso-style-parent:"Text in List 2\,t2";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.75in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.RevisionHistory, li.RevisionHistory, div.RevisionHistory
	{mso-style-name:"Revision History\,rh";
	mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:1.0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:purple;
	display:none;
	mso-hide:all;
	font-style:italic;
	mso-bidi-font-style:normal;}
p.BulletedList1, li.BulletedList1, div.BulletedList1
	{mso-style-name:"Bulleted List 1\,bl1";
	mso-style-parent:"Text in List 1\,t1";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.25in;
	text-indent:-.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	mso-list:l28 level1 lfo56;
	tab-stops:.25in;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.TextIndented, li.TextIndented, div.TextIndented
	{mso-style-name:"Text Indented\,ti";
	mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:.25in;
	margin-bottom:3.0pt;
	margin-left:.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.BulletedList2, li.BulletedList2, div.BulletedList2
	{mso-style-name:"Bulleted List 2\,bl2";
	mso-style-parent:"Text in List 2\,t2";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.5in;
	text-indent:-.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	mso-list:l46 level1 lfo61;
	tab-stops:.5in;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.DefinedTerm, li.DefinedTerm, div.DefinedTerm
	{mso-style-name:"Defined Term\,dt";
	mso-style-parent:"Text\,t";
	mso-style-next:"Definition\,d";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:0in;
	margin-left:0in;
	margin-bottom:.0001pt;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.Definition, li.Definition, div.Definition
	{mso-style-name:"Definition\,d";
	mso-style-parent:"Text\,t";
	mso-style-next:"Defined Term\,dt";
	margin-top:0in;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.NumberedList1, li.NumberedList1, div.NumberedList1
	{mso-style-name:"Numbered List 1\,nl1";
	mso-style-parent:"Text in List 1\,t1";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:.25in;
	text-indent:-.25in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	mso-list:l47 level1 lfo58;
	tab-stops:.25in;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.GlueLinkText, li.GlueLinkText, div.GlueLinkText
	{mso-style-name:"Glue Link Text\,glt";
	mso-style-parent:"Text\,t";
	mso-style-next:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.IndexTag, li.IndexTag, div.IndexTag
	{mso-style-name:"Index Tag\,it";
	mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:1.0in;
	margin-bottom:0in;
	margin-left:0in;
	margin-bottom:.0001pt;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:olive;
	display:none;
	mso-hide:all;}
span.CodeFeaturedElement
	{mso-style-name:"Code Featured Element\,cfe";
	mso-ansi-font-size:10.0pt;
	mso-ascii-font-family:"Courier New";
	mso-hansi-font-family:"Courier New";
	font-weight:bold;}
p.Copyright, li.Copyright, div.Copyright
	{mso-style-name:"Copyright\,copy";
	mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;}
p.PrintDivisionTitle, li.PrintDivisionTitle, div.PrintDivisionTitle
	{mso-style-name:"Print Division Title\,pdt";
	mso-style-parent:"";
	margin-top:9.0pt;
	margin-right:0in;
	margin-bottom:9.0pt;
	margin-left:0in;
	text-align:right;
	line-height:20.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:18.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;
	font-weight:bold;
	mso-bidi-font-weight:normal;}
p.PrintMSCorp, li.PrintMSCorp, div.PrintMSCorp
	{mso-style-name:"Print MS Corp\,pms";
	mso-style-parent:"";
	mso-style-next:"Text\,t";
	margin-top:9.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	text-align:right;
	line-height:15.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:13.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:"Microsoft Logo 95";
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
p.Slugline, li.Slugline, div.Slugline
	{mso-style-name:"Slugline\,slug";
	mso-style-parent:"Footer\,f";
	margin:0in;
	margin-bottom:.0001pt;
	line-height:9.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	mso-element:frame;
	mso-element-frame-height:40.0pt;
	mso-element-frame-hspace:9.0pt;
	mso-element-frame-vspace:9.0pt;
	mso-element-wrap:around;
	mso-element-anchor-vertical:page;
	mso-element-anchor-horizontal:margin;
	mso-element-top:730.05pt;
	mso-height-rule:exactly;
	font-size:6.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;}
span.MultilanguageMarkerAuto
	{mso-style-name:"Multilanguage Marker Auto\,mma";
	mso-ansi-font-size:8.0pt;
	mso-ascii-font-family:Verdana;
	mso-hansi-font-family:Verdana;
	color:fuchsia;}
p.MultilanguageMarkerExplicitBegin, li.MultilanguageMarkerExplicitBegin, div.MultilanguageMarkerExplicitBegin
	{mso-style-name:"Multilanguage Marker Explicit Begin\,mmeb";
	mso-style-parent:"Text\,t";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:fuchsia;}
p.MultilanguageMarkerExplicitEnd, li.MultilanguageMarkerExplicitEnd, div.MultilanguageMarkerExplicitEnd
	{mso-style-name:"Multilanguage Marker Explicit End\,mmee";
	mso-style-parent:"Multilanguage Marker Explicit Begin\,mmeb";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:11.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:8.0pt;
	mso-bidi-font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:purple;}
span.Bold
	{mso-style-name:"Bold\,b";
	font-weight:bold;}
span.BoldItalic
	{mso-style-name:"Bold Italic\,bi";
	font-weight:bold;
	font-style:italic;}
span.Italic
	{mso-style-name:"Italic\,i";
	font-style:italic;}
p.LabelSpecial, li.LabelSpecial, div.LabelSpecial
	{mso-style-name:"Label Special\,ls";
	mso-style-parent:"Label\,l";
	margin-top:3.0pt;
	margin-right:0in;
	margin-bottom:3.0pt;
	margin-left:0in;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:black;
	font-weight:bold;
	mso-bidi-font-weight:normal;}
p.PrintDivisionNumber, li.PrintDivisionNumber, div.PrintDivisionNumber
	{mso-style-name:"Print Division Number\,pdn";
	mso-style-parent:"Print Division Title\,pdt";
	margin-top:9.0pt;
	margin-right:-6.0pt;
	margin-bottom:0in;
	margin-left:0in;
	margin-bottom:.0001pt;
	text-align:right;
	line-height:13.0pt;
	mso-line-height-rule:exactly;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:Verdana;
	mso-fareast-font-family:"Times New Roman";
	mso-bidi-font-family:"Times New Roman";
	color:maroon;
	text-transform:uppercase;
	letter-spacing:6.0pt;}
span.Strikethrough
	{mso-style-name:"Strikethrough\,strike";
	text-decoration:line-through;}
span.Subscript
	{mso-style-name:"Subscript\,sub";
	vertical-align:sub;}
span.Superscript
	{mso-style-name:"Superscript\,sup";
	vertical-align:super;}
span.msoIns
	{mso-style-type:export-only;
	mso-style-name:"";
	text-decoration:underline;
	text-underline:single;
	color:teal;}
span.msoDel
	{mso-style-type:export-only;
	mso-style-name:"";
	text-decoration:line-through;
	color:red;}
@page Section1
	{size:8.5in 11.0in;
	margin:1.0in 70.5pt 1.0in 91.5pt;
	mso-header-margin:.75in;
	mso-footer-margin:.75in;
	mso-even-footer:url("./ndis_files/header.htm") ef1;
	mso-footer:url("./ndis_files/header.htm") f1;
	mso-paper-source:0;}
div.Section1
	{page:Section1;}
 /* List Definitions */
@list l0
	{mso-list-id:-132;
	mso-list-type:simple;
	mso-list-template-ids:-1025456648;}
@list l0:level1
	{mso-level-tab-stop:1.25in;
	mso-level-number-position:left;
	margin-left:1.25in;
	text-indent:-.25in;}
@list l1
	{mso-list-id:-131;
	mso-list-type:simple;
	mso-list-template-ids:1704917546;}
@list l1:level1
	{mso-level-tab-stop:1.0in;
	mso-level-number-position:left;
	margin-left:1.0in;
	text-indent:-.25in;}
@list l2
	{mso-list-id:-130;
	mso-list-type:simple;
	mso-list-template-ids:-1166372666;}
@list l2:level1
	{mso-level-tab-stop:.75in;
	mso-level-number-position:left;
	margin-left:.75in;
	text-indent:-.25in;}
@list l3
	{mso-list-id:-129;
	mso-list-type:simple;
	mso-list-template-ids:-1283709830;}
@list l3:level1
	{mso-level-tab-stop:.5in;
	mso-level-number-position:left;
	text-indent:-.25in;}
@list l4
	{mso-list-id:-128;
	mso-list-type:simple;
	mso-list-template-ids:-1261818240;}
@list l4:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:1.25in;
	mso-level-number-position:left;
	margin-left:1.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l5
	{mso-list-id:-127;
	mso-list-type:simple;
	mso-list-template-ids:-2137080934;}
@list l5:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:1.0in;
	mso-level-number-position:left;
	margin-left:1.0in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l6
	{mso-list-id:-126;
	mso-list-type:simple;
	mso-list-template-ids:480825406;}
@list l6:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.75in;
	mso-level-number-position:left;
	margin-left:.75in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l7
	{mso-list-id:-125;
	mso-list-type:simple;
	mso-list-template-ids:-629221776;}
@list l7:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.5in;
	mso-level-number-position:left;
	text-indent:-.25in;
	font-family:Symbol;}
@list l8
	{mso-list-id:-120;
	mso-list-type:simple;
	mso-list-template-ids:1309295726;}
@list l8:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l9
	{mso-list-id:-119;
	mso-list-type:simple;
	mso-list-template-ids:1565455404;}
@list l9:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l10
	{mso-list-id:68771112;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l10:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l11
	{mso-list-id:98985430;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l11:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l12
	{mso-list-id:152069974;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l12:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l13
	{mso-list-id:213931746;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l13:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l14
	{mso-list-id:309751557;
	mso-list-type:simple;
	mso-list-template-ids:1104607418;}
@list l14:level1
	{mso-level-tab-stop:.75in;
	mso-level-number-position:left;
	text-indent:-.25in;}
@list l15
	{mso-list-id:318533751;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l15:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l16
	{mso-list-id:414674143;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l16:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l17
	{mso-list-id:475953915;
	mso-list-type:simple;
	mso-list-template-ids:1980517918;}
@list l17:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l18
	{mso-list-id:521362406;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l18:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l19
	{mso-list-id:555043825;
	mso-list-type:simple;
	mso-list-template-ids:309995936;}
@list l19:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l20
	{mso-list-id:624120574;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l20:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l21
	{mso-list-id:653800487;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l21:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l22
	{mso-list-id:657225592;
	mso-list-type:hybrid;
	mso-list-template-ids:101086550 67698689 67698691 67698693 67698689 67698691 67698693 67698689 67698691 67698693;}
@list l22:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.5in;
	mso-level-number-position:left;
	text-indent:-.25in;
	font-family:Symbol;}
@list l23
	{mso-list-id:681705840;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l23:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l24
	{mso-list-id:769737302;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l24:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l25
	{mso-list-id:1054350067;
	mso-list-type:simple;
	mso-list-template-ids:-1624840694;}
@list l25:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l26
	{mso-list-id:1079012351;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l26:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l27
	{mso-list-id:1157041145;
	mso-list-type:simple;
	mso-list-template-ids:-13829058;}
@list l27:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l28
	{mso-list-id:1161654485;
	mso-list-type:simple;
	mso-list-template-ids:520143332;}
@list l28:level1
	{mso-level-number-format:bullet;
	mso-level-style-link:Copyright;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l29
	{mso-list-id:1242174498;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l29:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l30
	{mso-list-id:1247305601;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l30:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l31
	{mso-list-id:1352609911;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l31:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l32
	{mso-list-id:1387410818;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l32:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l33
	{mso-list-id:1434664814;
	mso-list-type:simple;
	mso-list-template-ids:-1000035972;}
@list l33:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l34
	{mso-list-id:1435589206;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l34:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l35
	{mso-list-id:1440906259;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l35:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l36
	{mso-list-id:1484086278;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l36:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l37
	{mso-list-id:1602645195;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l37:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l38
	{mso-list-id:1610774877;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l38:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l39
	{mso-list-id:1615479013;
	mso-list-type:simple;
	mso-list-template-ids:70557352;}
@list l39:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l40
	{mso-list-id:1656881520;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l40:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l41
	{mso-list-id:1686596083;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l41:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l42
	{mso-list-id:1780173060;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l42:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l43
	{mso-list-id:1809861688;
	mso-list-type:simple;
	mso-list-template-ids:1656119020;}
@list l43:level1
	{mso-level-style-link:"Numbered List 2";
	mso-level-tab-stop:.5in;
	mso-level-number-position:left;
	text-indent:-.25in;}
@list l44
	{mso-list-id:1835412302;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l44:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l45
	{mso-list-id:1839688267;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l45:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l46
	{mso-list-id:1892156636;
	mso-list-type:simple;
	mso-list-template-ids:2143072124;}
@list l46:level1
	{mso-level-number-format:bullet;
	mso-level-style-link:"Index 1";
	mso-level-text:\F0B7;
	mso-level-tab-stop:.5in;
	mso-level-number-position:left;
	text-indent:-.25in;
	font-family:Symbol;}
@list l47
	{mso-list-id:1908110580;
	mso-list-type:simple;
	mso-list-template-ids:-1994478282;}
@list l47:level1
	{mso-level-style-link:"Numbered List 1";
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l48
	{mso-list-id:1925800502;
	mso-list-type:simple;
	mso-list-template-ids:-387552862;}
@list l48:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l49
	{mso-list-id:2128041332;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l49:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
ol
	{margin-bottom:0in;}
ul
	{margin-bottom:0in;}
--&gt;
&lt;/style&gt;
&lt;!--[if gte mso 9]&gt;&lt;xml&gt;
 &lt;o:shapedefaults v:ext="edit" spidmax="2050"/&gt;
&lt;/xml&gt;&lt;![endif]--&gt;&lt;!--[if gte mso 9]&gt;&lt;xml&gt;
 &lt;o:shapelayout v:ext="edit"&gt;
  &lt;o:idmap v:ext="edit" data="1"/&gt;
 &lt;/o:shapelayout&gt;&lt;/xml&gt;&lt;![endif]--&gt;
&lt;/head&gt;

&lt;body lang=EN-US style='tab-interval:5.0in'&gt;

&lt;div class=Section1&gt;

&lt;h1&gt;NDIS Component &lt;/h1&gt;

&lt;p class=Text&gt;The Network Driver Interface Specification (NDIS) component
provides the run time environment (APIs, execution context) used by all network
device (NIC) drivers, as well as by network protocols (e.g. TCP/IP). NIC
drivers and network drivers use NDIS APIs to interact with each other. NDIS
also supports a mechanism for administering network protocols by using
NetConfig. &lt;/p&gt;

&lt;h2&gt;&lt;span style='color:black'&gt;Configuring the Component&lt;o:p&gt;&lt;/o:p&gt;&lt;/span&gt;&lt;/h2&gt;

&lt;p class=Text&gt;There are no configuration requirements for this component.&lt;/p&gt;

&lt;h3&gt;&lt;span style='color:black'&gt;Design Issues&lt;o:p&gt;&lt;/o:p&gt;&lt;/span&gt;&lt;/h3&gt;

&lt;p class=Text style='margin-left:.5in;text-indent:-.5in;mso-text-indent-alt:
-.25in;mso-list:l22 level1 lfo64;mso-list-change:\F0B7 "Arvind Murching" 20001113T1247;
tab-stops:list .5in'&gt;&lt;![if !supportLists]&gt;&lt;span style='font-family:Symbol'&gt;·&lt;span
style='font:7.0pt "Times New Roman"'&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;
&lt;/span&gt;&lt;/span&gt;&lt;![endif]&gt;NDIS is a base component required for any type of
network functionality.&lt;/p&gt;

&lt;p class=Text style='margin-left:.5in;text-indent:-.5in;mso-text-indent-alt:
-.25in;mso-list:l22 level1 lfo64;mso-list-change:\F0B7 "Arvind Murching" 20001113T1247;
tab-stops:list .5in'&gt;&lt;![if !supportLists]&gt;&lt;span style='font-family:Symbol'&gt;·&lt;span
style='font:7.0pt "Times New Roman"'&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;&amp;nbsp;
&lt;/span&gt;&lt;/span&gt;&lt;![endif]&gt;NDIS must be started before any other network driver
starts.&lt;/p&gt;

&lt;p class=Text&gt;&lt;![if !supportEmptyParas]&gt;&amp;nbsp;&lt;![endif]&gt;&lt;o:p&gt;&lt;/o:p&gt;&lt;/p&gt;

&lt;p class=Text&gt;&lt;![if !supportEmptyParas]&gt;&amp;nbsp;&lt;![endif]&gt;&lt;o:p&gt;&lt;/o:p&gt;&lt;/p&gt;

&lt;/div&gt;

&lt;div style='mso-element:comment-list'&gt;&lt;![if !supportAnnotations]&gt;

&lt;hr class=msocomoff align=left size=1 width="33%"&gt;

&lt;![endif]&gt;&lt;/div&gt;

&lt;/body&gt;

&lt;/html&gt;
</HELPCONTEXT><DISPLAYNAME>NDIS</DISPLAYNAME><VERSION>1.0</VERSION><DESCRIPTION>NDIS Runtime environment for network protocols and device drivers.</DESCRIPTION><COPYRIGHT>2000 Microsoft Corp.</COPYRIGHT><VENDOR>Microsoft Corp.</VENDOR><OWNERS>arvindm;alid</OWNERS><AUTHORS>arvindm</AUTHORS><DATECREATED>4/17/2000</DATECREATED><DATEREVISED>3/12/2001</DATEREVISED><RESOURCE Name="File:&quot;%12%&quot;,&quot;ndis.sys&quot;" ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819"><PROPERTY Name="DstPath" Format="String">%12%</PROPERTY><PROPERTY Name="DstName" Format="String">ndis.sys</PROPERTY><PROPERTY Name="SrcFileSize" Format="Integer">0</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>NDIS Driver</DISPLAYNAME><DESCRIPTION>Runtime environment for network protocols and device drivers.</DESCRIPTION></RESOURCE><RESOURCE Name="RegKey:HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS\Start" ResTypeVSGUID="{2C10DB69-39AB-48a4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS</PROPERTY><PROPERTY Name="ValueName" Format="String">Start</PROPERTY><PROPERTY Name="RegValue" Format="Integer">0</PROPERTY><PROPERTY Name="RegType" Format="Integer">4</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><RESOURCE Name="RegKey:HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS\Type" ResTypeVSGUID="{2C10DB69-39AB-48a4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS</PROPERTY><PROPERTY Name="ValueName" Format="String">Type</PROPERTY><PROPERTY Name="RegValue" Format="Integer">1</PROPERTY><PROPERTY Name="RegType" Format="Integer">4</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS&quot;" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS</PROPERTY><PROPERTY Name="RegType" Format="Integer">1</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS\MediaTypes&quot;" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS\MediaTypes</PROPERTY><PROPERTY Name="RegType" Format="Integer">1</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS\Parameters&quot;" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS\Parameters</PROPERTY><PROPERTY Name="RegType" Format="Integer">1</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS&quot;,&quot;DisplayName&quot;" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS</PROPERTY><PROPERTY Name="ValueName" Format="String">DisplayName</PROPERTY><PROPERTY Name="RegValue" Format="String">NDIS System Driver</PROPERTY><PROPERTY Name="RegType" Format="Integer">1</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS&quot;,&quot;ErrorControl&quot;" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS</PROPERTY><PROPERTY Name="ValueName" Format="String">ErrorControl</PROPERTY><PROPERTY Name="RegValue" Format="Integer">1</PROPERTY><PROPERTY Name="RegType" Format="Integer">4</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS&quot;,&quot;Group&quot;" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS</PROPERTY><PROPERTY Name="ValueName" Format="String">Group</PROPERTY><PROPERTY Name="RegValue" Format="String">NDIS Wrapper</PROPERTY><PROPERTY Name="RegType" Format="Integer">1</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS\Parameters&quot;,&quot;ProcessorAffinityMask&quot;" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\NDIS\Parameters</PROPERTY><PROPERTY Name="ValueName" Format="String">ProcessorAffinityMask</PROPERTY><PROPERTY Name="RegValue" Format="Integer">-1</PROPERTY><PROPERTY Name="RegType" Format="Integer">4</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><GROUPMEMBER GroupVSGUID="{388249D2-1897-44ff-86F2-E159A27AA037}"/><GROUPMEMBER GroupVSGUID="{4E7B2F7A-A581-11D4-8E94-00B0D03D27C6}"/></COMPONENT></DCARRIER>
