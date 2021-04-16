<?xml version="1.0" encoding="UTF-16"?>
<!DOCTYPE DCARRIER SYSTEM "Mantis.DTD">

  <DCARRIER
    CarrierRevision="1"
    DTDRevision="16"
  >
    <TASKS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </TASKS>

    <PLATFORMS
      Context="1"
    >    </PLATFORMS>

    <REPOSITORIES
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </REPOSITORIES>

    <GROUPS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </GROUPS>

    <COMPONENTS
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >
      <COMPONENT
        ComponentVSGUID="{4336AAA8-D473-4A57-BE96-3103FBEF060F}"
        ComponentVIGUID="{C79C5A6F-C6BB-499E-9D89-9CB2C8D741FB}"
        Revision="11"
        RepositoryVSGUID="{8E0BE9ED-7649-47F3-810B-232D36C430B4}"
        Visibility="1000"
        MultiInstance="False"
        Released="False"
        Editable="True"
        HTMLFinal="False"
        IsMacro="False"
        Opaque="False"
        Context="1"
        PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
      >
        <HELPCONTEXT
          src=".\AppleTalk_Protocol.htm"
        ><![CDATA[<html xmlns:v="urn:schemas-microsoft-com:vml"
xmlns:o="urn:schemas-microsoft-com:office:office"
xmlns:w="urn:schemas-microsoft-com:office:word"
xmlns="http://www.w3.org/TR/REC-html40">

<head>
<meta http-equiv=Content-Type content="text/html; charset=windows-1252">
<meta name=ProgId content=Word.Document>
<meta name=Generator content="Microsoft Word 10">
<meta name=Originator content="Microsoft Word 10">
<link rel=File-List href="AppleTalk_Protocol_files/filelist.xml">
<link rel=Edit-Time-Data href="AppleTalk_Protocol_files/editdata.mso">
<title>MSDN Authoring Template</title>
<!--[if gte mso 9]><xml>
 <o:DocumentProperties>
  <o:Author>SStonich</o:Author>
  <o:Description>Use for authoring only. Publication should be accomplished with CSS for HTML, or a related Word template for print or WinHelp.</o:Description>
  <o:Template>Normal</o:Template>
  <o:LastAuthor>Sachin Sheth</o:LastAuthor>
  <o:Revision>2</o:Revision>
  <o:TotalTime>5</o:TotalTime>
  <o:LastPrinted>2000-10-23T18:48:00Z</o:LastPrinted>
  <o:Created>2001-08-08T14:35:00Z</o:Created>
  <o:LastSaved>2001-08-08T14:35:00Z</o:LastSaved>
  <o:Pages>1</o:Pages>
  <o:Words>93</o:Words>
  <o:Characters>535</o:Characters>
  <o:Company>MSFT</o:Company>
  <o:Lines>4</o:Lines>
  <o:Paragraphs>1</o:Paragraphs>
  <o:CharactersWithSpaces>627</o:CharactersWithSpaces>
  <o:Version>10.2625</o:Version>
 </o:DocumentProperties>
</xml><![endif]--><!--[if gte mso 9]><xml>
 <w:WordDocument>
  <w:ActiveWritingStyle Lang="EN-US" VendorID="8" DLLVersion="513" NLCheck="0">1</w:ActiveWritingStyle>
  <w:SpellingState>Clean</w:SpellingState>
  <w:GrammarState>Clean</w:GrammarState>
  <w:UpdateStylesOnOpen/>
  <w:DisplayHorizontalDrawingGridEvery>0</w:DisplayHorizontalDrawingGridEvery>
  <w:DisplayVerticalDrawingGridEvery>0</w:DisplayVerticalDrawingGridEvery>
  <w:UseMarginsForDrawingGridOrigin/>
  <w:Compatibility>
   <w:FootnoteLayoutLikeWW8/>
   <w:ShapeLayoutLikeWW8/>
   <w:AlignTablesRowByRow/>
   <w:ForgetLastTabAlignment/>
   <w:LayoutRawTableWidth/>
   <w:LayoutTableRowsApart/>
   <w:UseWord97LineBreakingRules/>
  </w:Compatibility>
  <w:BrowserLevel>MicrosoftInternetExplorer4</w:BrowserLevel>
 </w:WordDocument>
</xml><![endif]-->
<style>
<!--
 /* Font Definitions */
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
	{mso-style-name:"Normal\,APPLY ANOTHER STYLE";
	mso-style-parent:"";
	margin:0in;
	margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	font-size:12.0pt;
	font-family:"Times New Roman";
	mso-fareast-font-family:"Times New Roman";}
h1
	{mso-style-name:"Heading 1\,h1\,Level 1 Topic Heading";
	mso-style-parent:"";
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
	{mso-style-name:"Heading 2\,h2\,Level 2 Topic Heading";
	mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
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
	{mso-style-name:"Heading 3\,h3\,Level 3 Topic Heading";
	mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
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
	{mso-style-name:"Heading 4\,h4\,First Subheading";
	mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
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
	{mso-style-name:"Heading 5\,h5\,Second Subheading";
	mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
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
	{mso-style-name:"Heading 6\,h6\,Third Subheading";
	mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
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
	{mso-style-name:"Index 1\,idx1";
	mso-style-noshow:yes;
	mso-style-parent:"Text\,t";
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
	{mso-style-name:"Index 2\,idx2";
	mso-style-noshow:yes;
	mso-style-parent:"Index 1\,idx1";
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
	{mso-style-name:"Index 3\,idx3";
	mso-style-noshow:yes;
	mso-style-parent:"Index 1\,idx1";
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
	{mso-style-name:"TOC 1\,toc1";
	mso-style-noshow:yes;
	mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
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
	{mso-style-name:"TOC 2\,toc2";
	mso-style-noshow:yes;
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
	color:maroon;}
p.MsoToc3, li.MsoToc3, div.MsoToc3
	{mso-style-name:"TOC 3\,toc3";
	mso-style-noshow:yes;
	mso-style-parent:"TOC 2\,toc2";
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
	{mso-style-name:"TOC 4\,toc4";
	mso-style-noshow:yes;
	mso-style-parent:"TOC 2\,toc2";
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
	{mso-style-name:"Footnote Text\,ft\,Used by Word for text of Help footnotes";
	mso-style-noshow:yes;
	mso-style-parent:"Text\,t";
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
	{mso-style-name:"Comment Text\,ct\,Used by Word for text of author queries";
	mso-style-noshow:yes;
	mso-style-parent:"Text\,t";
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
	{mso-style-name:"Header\,h";
	mso-style-parent:"Footer\,f";
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
	{mso-style-name:"Footer\,f";
	mso-style-parent:"";
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
	{mso-style-name:"Index Heading\,ih";
	mso-style-noshow:yes;
	mso-style-parent:"Heading 1\,h1\,Level 1 Topic Heading";
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
	{mso-style-name:"Footnote Reference\,fr\,Used by Word for Help footnote symbols";
	mso-style-noshow:yes;
	color:red;
	vertical-align:super;}
span.MsoPageNumber
	{mso-style-name:"Page Number\,pn";
	mso-ansi-font-size:8.0pt;
	font-family:Verdana;
	mso-ascii-font-family:Verdana;
	mso-hansi-font-family:Verdana;
	color:maroon;
	font-weight:bold;
	mso-bidi-font-weight:normal;}
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
	color:black;
	mso-no-proof:yes;}
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
	mso-list:l42 level1 lfo63;
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
	color:black;
	mso-no-proof:yes;}
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
	font-family:"Courier New";
	mso-ascii-font-family:"Courier New";
	mso-hansi-font-family:"Courier New";}
span.LabelEmbedded
	{mso-style-name:"Label Embedded\,le";
	mso-ansi-font-size:10.0pt;
	font-family:Verdana;
	mso-ascii-font-family:Verdana;
	mso-hansi-font-family:Verdana;
	font-weight:bold;
	mso-bidi-font-weight:normal;
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
	color:black;
	mso-no-proof:yes;}
span.ConditionalMarker
	{mso-style-name:"Conditional Marker\,cm";
	mso-ansi-font-size:10.0pt;
	font-family:"Courier New";
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
	color:black;
	mso-no-proof:yes;}
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
	font-family:"Courier New";
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
	mso-list:l27 level1 lfo56;
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
	mso-list:l45 level1 lfo61;
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
	mso-list:l46 level1 lfo58;
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
	font-family:"Courier New";
	mso-ascii-font-family:"Courier New";
	mso-hansi-font-family:"Courier New";
	font-weight:bold;
	mso-bidi-font-weight:normal;}
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
	color:maroon;
	mso-no-proof:yes;}
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
	font-family:Verdana;
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
	font-weight:bold;
	mso-bidi-font-weight:normal;}
span.BoldItalic
	{mso-style-name:"Bold Italic\,bi";
	font-weight:bold;
	mso-bidi-font-weight:normal;
	font-style:italic;
	mso-bidi-font-style:normal;}
span.Italic
	{mso-style-name:"Italic\,i";
	font-style:italic;
	mso-bidi-font-style:normal;}
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
span.SpellE
	{mso-style-name:"";
	mso-spl-e:yes;}
@page Section1
	{size:8.5in 11.0in;
	margin:1.0in 70.5pt 1.0in 91.5pt;
	mso-header-margin:.75in;
	mso-footer-margin:.75in;
	mso-even-footer:url("AppleTalk_Protocol_files/header.htm") ef1;
	mso-footer:url("AppleTalk_Protocol_files/header.htm") f1;
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
	{mso-list-id:681705840;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l22:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l23
	{mso-list-id:769737302;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l23:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l24
	{mso-list-id:1054350067;
	mso-list-type:simple;
	mso-list-template-ids:-1624840694;}
@list l24:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l25
	{mso-list-id:1079012351;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l25:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l26
	{mso-list-id:1157041145;
	mso-list-type:simple;
	mso-list-template-ids:-13829058;}
@list l26:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l27
	{mso-list-id:1161654485;
	mso-list-type:simple;
	mso-list-template-ids:520143332;}
@list l27:level1
	{mso-level-number-format:bullet;
	mso-level-style-link:Copyright;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l28
	{mso-list-id:1242174498;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l28:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l29
	{mso-list-id:1247305601;
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
	{mso-list-id:1352609911;
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
	{mso-list-id:1387410818;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l31:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l32
	{mso-list-id:1434664814;
	mso-list-type:simple;
	mso-list-template-ids:-1000035972;}
@list l32:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l33
	{mso-list-id:1435589206;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l33:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l34
	{mso-list-id:1440906259;
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
	{mso-list-id:1484086278;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l35:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l36
	{mso-list-id:1602645195;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l36:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l37
	{mso-list-id:1610774877;
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
	{mso-list-id:1615479013;
	mso-list-type:simple;
	mso-list-template-ids:70557352;}
@list l38:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l39
	{mso-list-id:1656881520;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l39:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l40
	{mso-list-id:1686596083;
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
	{mso-list-id:1780173060;
	mso-list-type:simple;
	mso-list-template-ids:67698703;}
@list l41:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l42
	{mso-list-id:1809861688;
	mso-list-type:simple;
	mso-list-template-ids:1656119020;}
@list l42:level1
	{mso-level-style-link:"Numbered List 2";
	mso-level-tab-stop:.5in;
	mso-level-number-position:left;
	text-indent:-.25in;}
@list l43
	{mso-list-id:1835412302;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l43:level1
	{mso-level-number-format:bullet;
	mso-level-text:\F0B7;
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;
	font-family:Symbol;}
@list l44
	{mso-list-id:1839688267;
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
	{mso-list-id:1892156636;
	mso-list-type:simple;
	mso-list-template-ids:2143072124;}
@list l45:level1
	{mso-level-number-format:bullet;
	mso-level-style-link:"Index 1";
	mso-level-text:\F0B7;
	mso-level-tab-stop:.5in;
	mso-level-number-position:left;
	text-indent:-.25in;
	font-family:Symbol;}
@list l46
	{mso-list-id:1908110580;
	mso-list-type:simple;
	mso-list-template-ids:-1994478282;}
@list l46:level1
	{mso-level-style-link:"Numbered List 1";
	mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l47
	{mso-list-id:1925800502;
	mso-list-type:simple;
	mso-list-template-ids:-387552862;}
@list l47:level1
	{mso-level-tab-stop:.25in;
	mso-level-number-position:left;
	margin-left:.25in;
	text-indent:-.25in;}
@list l48
	{mso-list-id:2128041332;
	mso-list-type:simple;
	mso-list-template-ids:67698689;}
@list l48:level1
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
-->
</style>
<!--[if gte mso 10]>
<style>
 /* Style Definitions */
 table.MsoNormalTable
	{mso-style-name:"Table Normal";
	mso-tstyle-rowband-size:0;
	mso-tstyle-colband-size:0;
	mso-style-noshow:yes;
	mso-style-parent:"";
	mso-padding-alt:0in 5.4pt 0in 5.4pt;
	mso-para-margin:0in;
	mso-para-margin-bottom:.0001pt;
	mso-pagination:widow-orphan;
	font-size:10.0pt;
	font-family:"Times New Roman";}
</style>
<![endif]--><!--[if gte mso 9]><xml>
 <o:shapedefaults v:ext="edit" spidmax="2050"/>
</xml><![endif]--><!--[if gte mso 9]><xml>
 <o:shapelayout v:ext="edit">
  <o:idmap v:ext="edit" data="1"/>
 </o:shapelayout></xml><![endif]-->
</head>

<body lang=EN-US style='tab-interval:5.0in'>

<div class=Section1>

<h1>Component Description</h1>

<p class=Text>AppleTalk protocol is the layer of AppleTalk Phase2 protocols
that delivers data to its destination on the network. The computer running
Windows with AppleTalk protocol can also provide routing and seed routing
support. An AppleTalk router broadcasts routing information, such as network
addresses, and keeps track of and directs data packets on AppleTalk networks.
Seed routers perform these functions and also seed the physical networks on
which they reside </p>

<h2><span style='color:black'>Component Configuration<o:p></o:p></span></h2>

<p class=Text>No configuration is required to use this component.</p>

<p class=Text><o:p>&nbsp;</o:p></p>

<h3><span style='color:black'>For More Information<o:p></o:p></span></h3>

<p class=Text><o:p>&nbsp;</o:p></p>

<h3><span style='color:black'>Additional Information<o:p></o:p></span></h3>

<p class=Text>Contact name: <span class=SpellE>sachins</span></p>

<p class=Text><o:p>&nbsp;</o:p></p>

</div>

</body>

</html>
]]></HELPCONTEXT>

        <PROPERTIES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >        </PROPERTIES>

        <RESOURCES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >
          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk&quot;,&quot;Type&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Type</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk&quot;,&quot;Start&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Start</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk&quot;,&quot;ErrorControl&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >ErrorControl</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk&quot;,&quot;Tag&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Tag</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >7</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk&quot;,&quot;ImagePath&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >ImagePath</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >System32\DRIVERS\sfmatalk.sys</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk&quot;,&quot;DisplayName&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >DisplayName</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >AppleTalk Protocol</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk&quot;,&quot;Group&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Group</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >PNP_TDI</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk&quot;,&quot;Description&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Description</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >AppleTalk Protocol</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Linkage&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Linkage</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Linkage&quot;,&quot;Bind&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Linkage</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Bind</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Multi"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >5C004400650076006900630065005C007B00380039003400370036003700330034002D0042004400450033002D0034004200440041002D0038003300430042002D003900430044004200320039003200450041003300420043007D000A005C004400650076006900630065005C004E00640069007300570061006E004100740061006C006B000000</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >7</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Linkage&quot;,&quot;Route&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Linkage</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Route</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Multi"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >5C004400650076006900630065005C007B00380039003400370036003700330034002D0042004400450033002D0034004200440041002D0038003300430042002D003900430044004200320039003200450041003300420043007D000A005C004400650076006900630065005C004E00640069007300570061006E004100740061006C006B0022007B00380039003400370036003700330034002D0042004400450033002D0034004200440041002D0038003300430042002D003900430044004200320039003200450041003300420043007D0022000A0022004E00640069007300570061006E004100740061006C006B0022000000</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >7</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Linkage&quot;,&quot;Export&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Linkage</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Export</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Multi"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >5C004400650076006900630065005C007B00380039003400370036003700330034002D0042004400450033002D0034004200440041002D0038003300430042002D003900430044004200320039003200450041003300420043007D000A005C004400650076006900630065005C004E00640069007300570061006E004100740061006C006B0022007B00380039003400370036003700330034002D0042004400450033002D0034004200440041002D0038003300430042002D003900430044004200320039003200450041003300420043007D0022000A0022004E00640069007300570061006E004100740061006C006B0022005C004400650076006900630065005C004100700070006C006500540061006C006B005F007B00380039003400370036003700330034002D0042004400450033002D0034004200440041002D0038003300430042002D003900430044004200320039003200450041003300420043007D000A005C004400650076006900630065005C004100700070006C006500540061006C006B005F004E00640069007300570061006E004100740061006C006B000000</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >7</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters&quot;,&quot;DefaultPort&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >DefaultPort</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >\Device\{89476734-BDE3-4BDA-83CB-9CDB292EA3BC}</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters&quot;,&quot;DesiredZone&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >DesiredZone</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Corp-40/2243</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters&quot;,&quot;EnableRouter&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >EnableRouter</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >0</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Adapters&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Adapters</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock&quot;,&quot;HelperDllName&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HelperDllName</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%SystemRoot%\System32\sfmwshat.dll</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >2</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock&quot;,&quot;MaxSockAddrLength&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >MaxSockAddrLength</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >8</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock&quot;,&quot;MinSockAddrLength&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >MinSockAddrLength</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >6</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock&quot;,&quot;Mapping&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Parameters\Winsock</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Mapping</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Binary"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >02010000030000001000000001000000803F00001000000004000000803F00001000000004000000833F00001000000002000000813E00001000000002000000823E00001000000002000000833E00001000000002000000843E00001000000002000000853E00001000000002000000863E00001000000002000000873E00001000000002000000883E00001000000002000000893E000010000000020000008A3E000010000000020000008B3E000010000000020000008C3E000010000000020000008D3E000010000000020000008E3E000010000000020000008F3E00001000000002000000903E00001000000002000000913E00001000000002000000923E00001000000002000000933E00001000000002000000943E00001000000002000000953E00001000000002000000963E00001000000002000000973E00001000000002000000983E00001000000002000000993E000010000000020000009A3E000010000000020000009B3E000010000000020000009C3E000010000000020000009D3E000010000000020000009E3E000010000000020000009F3E00001000000002000000A03E00001000000002000000A13E00001000000002000000A23E00001000000002000000A33E00001000000002000000A43E00001000000002000000A53E00001000000002000000A63E00001000000002000000A73E00001000000002000000A83E00001000000002000000A93E00001000000002000000AA3E00001000000002000000AB3E00001000000002000000AC3E00001000000002000000AD3E00001000000002000000AE3E00001000000002000000AF3E00001000000002000000B03E00001000000002000000B13E00001000000002000000B23E00001000000002000000B33E00001000000002000000B43E00001000000002000000B53E00001000000002000000B63E00001000000002000000B73E00001000000002000000B83E00001000000002000000B93E00001000000002000000BA3E00001000000002000000BB3E00001000000002000000BC3E00001000000002000000BD3E00001000000002000000BE3E00001000000002000000BF3E00001000000002000000C03E00001000000002000000C13E00001000000002000000C23E00001000000002000000C33E00001000000002000000C43E00001000000002000000C53E00001000000002000000C63E00001000000002000000C73E00001000000002000000C83E00001000000002000000C93E00001000000002000000CA3E00001000000002000000CB3E00001000000002000000CC3E00001000000002000000CD3E00001000000002000000CE3E00001000000002000000CF3E00001000000002000000D03E00001000000002000000D13E00001000000002000000D23E00001000000002000000D33E00001000000002000000D43E00001000000002000000D53E00001000000002000000D63E00001000000002000000D73E00001000000002000000D83E00001000000002000000D93E00001000000002000000DA3E00001000000002000000DB3E00001000000002000000DC3E00001000000002000000DD3E00001000000002000000DE3E00001000000002000000DF3E00001000000002000000E03E00001000000002000000E13E00001000000002000000E23E00001000000002000000E33E00001000000002000000E43E00001000000002000000E53E00001000000002000000E63E00001000000002000000E73E00001000000002000000E83E00001000000002000000E93E00001000000002000000EA3E00001000000002000000EB3E00001000000002000000EC3E00001000000002000000ED3E00001000000002000000EE3E00001000000002000000EF3E00001000000002000000F03E00001000000002000000F13E00001000000002000000F23E00001000000002000000F33E00001000000002000000F43E00001000000002000000F53E00001000000002000000F63E00001000000002000000F73E00001000000002000000F83E00001000000002000000F93E00001000000002000000FA3E00001000000002000000FB3E00001000000002000000FC3E00001000000002000000FD3E00001000000002000000FE3E00001000000002000000FF3E00001000000002000000003F00001000000002000000013F00001000000002000000023F00001000000002000000033F00001000000002000000043F00001000000002000000053F00001000000002000000063F00001000000002000000073F00001000000002000000083F00001000000002000000093F000010000000020000000A3F000010000000020000000B3F000010000000020000000C3F000010000000020000000D3F000010000000020000000E3F000010000000020000000F3F00001000000002000000103F00001000000002000000113F00001000000002000000123F00001000000002000000133F00001000000002000000143F00001000000002000000153F00001000000002000000163F00001000000002000000173F00001000000002000000183F00001000000002000000193F000010000000020000001A3F000010000000020000001B3F000010000000020000001C3F000010000000020000001D3F000010000000020000001E3F000010000000020000001F3F00001000000002000000203F00001000000002000000213F00001000000002000000223F00001000000002000000233F00001000000002000000243F00001000000002000000253F00001000000002000000263F00001000000002000000273F00001000000002000000283F00001000000002000000293F000010000000020000002A3F000010000000020000002B3F000010000000020000002C3F000010000000020000002D3F000010000000020000002E3F000010000000020000002F3F00001000000002000000303F00001000000002000000313F00001000000002000000323F00001000000002000000333F00001000000002000000343F00001000000002000000353F00001000000002000000363F00001000000002000000373F00001000000002000000383F00001000000002000000393F000010000000020000003A3F000010000000020000003B3F000010000000020000003C3F000010000000020000003D3F000010000000020000003E3F000010000000020000003F3F00001000000002000000403F00001000000002000000413F00001000000002000000423F00001000000002000000433F00001000000002000000443F00001000000002000000453F00001000000002000000463F00001000000002000000473F00001000000002000000483F00001000000002000000493F000010000000020000004A3F000010000000020000004B3F000010000000020000004C3F000010000000020000004D3F000010000000020000004E3F000010000000020000004F3F00001000000002000000503F00001000000002000000513F00001000000002000000523F00001000000002000000533F00001000000002000000543F00001000000002000000553F00001000000002000000563F00001000000002000000573F00001000000002000000583F00001000000002000000593F000010000000020000005A3F000010000000020000005B3F000010000000020000005C3F000010000000020000005D3F000010000000020000005E3F000010000000020000005F3F00001000000002000000603F00001000000002000000613F00001000000002000000623F00001000000002000000633F00001000000002000000643F00001000000002000000653F00001000000002000000663F00001000000002000000673F00001000000002000000683F00001000000002000000693F000010000000020000006A3F000010000000020000006B3F000010000000020000006C3F000010000000020000006D3F000010000000020000006E3F000010000000020000006F3F00001000000002000000703F00001000000002000000713F00001000000002000000723F00001000000002000000733F00001000000002000000743F00001000000002000000753F00001000000002000000763F00001000000002000000773F00001000000002000000783F00001000000002000000793F000010000000020000007A3F000010000000020000007B3F000010000000020000007C3F000010000000020000007D3F000010000000020000007E3F000010000000020000007F3F0000</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance&quot;,&quot;Library&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Library</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >atkctrs.dll</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance&quot;,&quot;Open&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Open</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >OpenAtkPerformanceData</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance&quot;,&quot;Close&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Close</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >CloseAtkPerformanceData</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance&quot;,&quot;Collect&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Collect</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >CollectAtkPerformanceData</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance&quot;,&quot;WbemAdapFileTime&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >WbemAdapFileTime</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Binary"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >C004E516CF3BBF01</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance&quot;,&quot;WbemAdapFileSize&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >WbemAdapFileSize</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >14096</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance&quot;,&quot;WbemAdapStatus&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Performance</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >WbemAdapStatus</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >0</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Security&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Security</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Security&quot;,&quot;Security&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Security</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Security</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Binary"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >01001480A0000000AC000000140000003000000002001C000100000002801400FF010F00010100000000000100000000020070000400000000001800FD01020001010000000000051200000074006C0000001C00FF010F00010200000000000520000000200200000000B337000018008D01020001010000000000050B0000002002000000001C00FD010200010200000000000520000000230200000000B337010100000000000512000000010100000000000512000000</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >3</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Enum&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Enum</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Enum&quot;,&quot;0&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Enum</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >0</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Root\LEGACY_APPLETALK\0000</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Enum&quot;,&quot;Count&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Enum</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >Count</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RegKey(819):&quot;HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Enum&quot;,&quot;NextInstance&quot;"
            ResTypeVSGUID="{2C10DB69-39AB-48A4-A83F-9AB3ACBA7C45}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="KeyPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\AppleTalk\Enum</PROPERTY>

              <PROPERTY
                Name="ValueName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >NextInstance</PROPERTY>

              <PROPERTY
                Name="RegValue"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegType"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >4</PROPERTY>

              <PROPERTY
                Name="RegOp"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>

              <PROPERTY
                Name="RegCond"
                Format="Integer"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >1</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%11%&quot;,&quot;sfmatmsg.dll&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%11%</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >sfmatmsg.dll</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%11%&quot;,&quot;sfmmon.dll&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%11%</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >sfmmon.dll</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%11%&quot;,&quot;sfmwshat.dll&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%11%</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >sfmwshat.dll</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%12%&quot;,&quot;sfmatalk.sys&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%12%</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >sfmatalk.sys</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%17%&quot;,&quot;netatlk.inf&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%17%</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >netatlk.inf</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;MSVCRT.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >MSVCRT.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;KERNEL32.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >KERNEL32.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;ADVAPI32.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >ADVAPI32.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;USER32.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >USER32.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;WSOCK32.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >WSOCK32.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;SPOOLSS.DLL&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >SPOOLSS.DLL</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;NETUI0.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >NETUI0.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;NETUI2.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >NETUI2.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;ntdll.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >ntdll.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%55%&quot;,&quot;sfmpsprt.dll&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%55%</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >sfmpsprt.dll</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="File(819):&quot;%11%&quot;,&quot;atkctrs.dll&quot;"
            ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="DstPath"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >%11%</PROPERTY>

              <PROPERTY
                Name="DstName"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >atkctrs.dll</PROPERTY>

              <PROPERTY
                Name="NoExpand"
                Format="Boolean"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >False</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;ntoskrnl.exe&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >ntoskrnl.exe</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;HAL.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >HAL.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;NDIS.SYS&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >NDIS.SYS</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;TDI.SYS&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >TDI.SYS</PROPERTY>
            </PROPERTIES>
          </RESOURCE>

          <RESOURCE
            Name="RawDep(819):&quot;File&quot;,&quot;netcfgx.dll&quot;"
            ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}"
            BuildTypeMask="819"
            BuildOrder="1000"
            Localize="False"
            Disabled="False"
            Context="1"
            PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
          >
            <PROPERTIES
              Context="1"
              PlatformGUID="{00000000-0000-0000-0000-000000000000}"
            >
              <PROPERTY
                Name="RawType"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >File</PROPERTY>

              <PROPERTY
                Name="Value"
                Format="String"
                Context="1"
                PlatformGUID="{00000000-0000-0000-0000-000000000000}"
              >netcfgx.dll</PROPERTY>
            </PROPERTIES>
          </RESOURCE>
        </RESOURCES>

        <GROUPMEMBERS
        >
          <GROUPMEMBER
            GroupVSGUID="{E01B4103-3883-4FE8-992F-10566E7B796C}"
          ></GROUPMEMBER>

          <GROUPMEMBER
            GroupVSGUID="{388249D2-1897-44FF-86F2-E159A27AA037}"
          ></GROUPMEMBER>
        </GROUPMEMBERS>

        <DEPENDENCIES
          Context="1"
          PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}"
        >        </DEPENDENCIES>

        <DISPLAYNAME>AppleTalk Protocol</DISPLAYNAME>

        <VERSION>1.0</VERSION>

        <DESCRIPTION>AppleTalk Protocol</DESCRIPTION>

        <COPYRIGHT>2000 Microsoft Corp.</COPYRIGHT>

        <VENDOR>Microsoft Corp.</VENDOR>

        <OWNERS>sachins</OWNERS>

        <AUTHORS>sachins</AUTHORS>

        <DATECREATED>9/24/2000</DATECREATED>

        <DATEREVISED>8/8/2001 2:36:22 PM</DATEREVISED>
      </COMPONENT>
    </COMPONENTS>

    <RESTYPES
      Context="1"
      PlatformGUID="{00000000-0000-0000-0000-000000000000}"
    >    </RESTYPES>
  </DCARRIER>
