<?xml version="1.0" encoding="UTF-16"?>
<!DOCTYPE DCARRIER SYSTEM "mantis.dtd">
<DCARRIER CarrierRevision="1">
	<TOOLINFO ToolName="iCat"><![CDATA[<?xml version="1.0" encoding="UTF-16"?>
<!DOCTYPE TOOL SYSTEM "tool.dtd">
<TOOL>
	<CREATED><NAME>iCat</NAME><VSGUID>{97b86ee0-259c-479f-bc46-6cea7ef4be4d}</VSGUID><VERSION>1.0.0.452</VERSION><BUILD>452</BUILD><DATE>4/23/2001</DATE></CREATED><LASTSAVED><NAME>iCat</NAME><VSGUID>{97b86ee0-259c-479f-bc46-6cea7ef4be4d}</VSGUID><VERSION>1.0.0.452</VERSION><BUILD>452</BUILD><DATE>5/16/2001</DATE></LASTSAVED></TOOL>
]]></TOOLINFO><COMPONENT Revision="4" Visibility="1000" MultiInstance="0" Released="1" Editable="1" HTMLFinal="0" ComponentVSGUID="{1BAC88A7-C976-498A-9755-EB7604FAAA5A}" ComponentVIGUID="{B6CAA39F-C298-42BF-9B2B-A378B9C92086}" PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}" RepositoryVSGUID="{8E0BE9ED-7649-47F3-810B-232D36C430B4}"><HELPCONTEXT src="E:\ntmain\net\tcpip\driver\sys\nt\TCPIPServices.htm">&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN"&gt;
&lt;HTML DIR="LTR"&gt;&lt;HEAD&gt;
&lt;META HTTP-EQUIV="Content-Type" Content="text/html; charset=Windows-1252"&gt;
&lt;style type="text/css"&gt;@import url(td.css);&lt;/style&gt;
&lt;TITLE&gt;TCP/IP Services Component Description&lt;/TITLE&gt;
&lt;/HEAD&gt;
&lt;body topmargin=0 id="bodyID" class = "dtBODY"&gt;
&lt;H1 class="dtH1"&gt;&lt;A NAME="_tcp!ip_services_component_description"&gt;&lt;/A&gt;&lt;SUP&gt;&lt;/SUP&gt;TCP/IP Services Component Description&lt;/H1&gt;

&lt;P&gt;This component supports the following TCP/IP services:

&lt;UL&gt;
	&lt;LI&gt;FTP client&lt;/li&gt;

	&lt;LI&gt;SIMPTCP.DLL &amp;#0150; echo/chargen&lt;/li&gt;

	&lt;LI&gt;Tcpsvcs.exe - TCP/IP services&lt;/li&gt;

	&lt;LI&gt;Tftp.exe &amp;#0150; Trivial client&lt;/li&gt;
&lt;/UL&gt;

&lt;H1 class="dtH1"&gt;Component Configuration&lt;/H1&gt;

&lt;P&gt;There are no configuration requirements for this component.&lt;/P&gt;

&lt;!--Footer--&gt;
&lt;div class="footer"&gt;
&lt;br&gt;
&lt;center&gt;&lt;font color=228B22 size="1"&gt;Built on Tuesday, May 15, 2001&lt;/font&gt;&lt;/center&gt;
&lt;/div&gt;
&lt;/BODY&gt;
&lt;/HTML&gt;
</HELPCONTEXT><DISPLAYNAME>TCPIPServices</DISPLAYNAME><VERSION>1.0</VERSION><DESCRIPTION>TCPIP services (ftp,simptcp and tcpsvcs)</DESCRIPTION><COPYRIGHT>2000 Microsoft Corp.</COPYRIGHT><VENDOR>Microsoft Corp.</VENDOR><OWNERS>nksrin</OWNERS><AUTHORS>nksrin</AUTHORS><DATECREATED>4/23/2001</DATECREATED><DATEREVISED>5/16/2001</DATEREVISED><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%11%&quot;,&quot;ftp.exe&quot;"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">ftp.exe</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>FTP</DISPLAYNAME><DESCRIPTION>ftp client</DESCRIPTION></RESOURCE><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%11%&quot;,&quot;tcpsvcs.exe&quot;"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">tcpsvcs.exe</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>TCPSVCS</DISPLAYNAME><DESCRIPTION>TCP services</DESCRIPTION></RESOURCE><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%11%&quot;,&quot;tftp.exe&quot;"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">tftp.exe</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>TFTP</DISPLAYNAME><DESCRIPTION>tftp client</DESCRIPTION></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;MSVCRT.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">MSVCRT.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;KERNEL32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">KERNEL32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;WS2_32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">WS2_32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;USER32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">USER32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;MSWSOCK.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">MSWSOCK.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;ADVAPI32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">ADVAPI32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;ntdll.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">ntdll.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;RPCRT4.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">RPCRT4.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%11%&quot;,&quot;simptcp.dll&quot;"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">simptcp.dll</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>simptcp</DISPLAYNAME><DESCRIPTION>Simple tcp services(echo and chargen)</DESCRIPTION></RESOURCE><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%12%\etc&quot;,&quot;quotes&quot;"><PROPERTY Name="DstPath" Format="String">%12%\etc</PROPERTY><PROPERTY Name="DstName" Format="String">quotes</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY></RESOURCE><GROUPMEMBER GroupVSGUID="{E01B4103-3883-4FE8-992F-10566E7B796C}"/><GROUPMEMBER GroupVSGUID="{388249D2-1897-44FF-86F2-E159A27AA037}"/></COMPONENT></DCARRIER>
