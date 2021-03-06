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
	<CREATED><NAME>iCat</NAME><VSGUID>{f802f157-4623-426a-8b39-ac2eb4332c74}</VSGUID><VERSION>1.0.0.244</VERSION><BUILD>244</BUILD><DATE>7/18/2000</DATE></CREATED><LASTSAVED><NAME>iCat</NAME><VSGUID>{97b86ee0-259c-479f-bc46-6cea7ef4be4d}</VSGUID><VERSION>1.0.0.452</VERSION><BUILD>452</BUILD><DATE>4/12/2001</DATE></LASTSAVED></TOOL>
]]></TOOLINFO><COMPONENT ComponentVSGUID="{431B17CE-9C70-4F75-83AE-F1A50AAFD403}" ComponentVIGUID="{566D2530-95A3-4070-8CF0-B89FE0ECF565}" PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}" RepositoryVSGUID="{8E0BE9ED-7649-47F3-810B-232D36C430B4}" Revision="5" Visibility="1000" MultiInstance="False" Released="1" Editable="True"><HELPCONTEXT src="\\mantisqa\ovrdaily\mantis\hlp\arvindm\atm_component_description.htm">&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN"&gt;
&lt;HTML DIR="LTR"&gt;&lt;HEAD&gt;
&lt;META HTTP-EQUIV="Content-Type" Content="text/html; charset=Windows-1252"&gt;
&lt;TITLE&gt;ATM Component Description&lt;/TITLE&gt;
&lt;style type="text/css"&gt;@import url(td.css);&lt;/style&gt;&lt;/HEAD&gt;
&lt;BODY TOPMARGIN="0"&gt;
&lt;H1&gt;&lt;A NAME="atm_component_description"&gt;&lt;/A&gt;&lt;SUP&gt;&lt;/SUP&gt;ATM Component Description&lt;/H1&gt;

&lt;P&gt;The asynchronous transfer mode (ATM) component contains networking support using ATM devices. There are four ATM communication modes:

&lt;UL type="disc"&gt;
	&lt;LI&gt;Using local area network (LAN) emulation, known as ATMLANE. This is the mode typically used in corporate LANs.&lt;/li&gt;

	&lt;LI&gt;Using Ethernet-permanent virtual circuit (PVC), known as ATMEPVC. This is used in certain ATM-DSL deployments.&lt;/li&gt;

	&lt;LI&gt;Using Classical Internet Protocols (IP) over ATM, known as ATMARPC and ATMARPS. This is an alternative LAN technique suitable for IP-only networks.&lt;/li&gt;

	&lt;LI&gt;Using Winsock over raw ATM connections. This is used for special purpose applications .&lt;/li&gt;
&lt;/UL&gt;

&lt;H1&gt;Component Configuration&lt;/H1&gt;

&lt;P&gt;ATMLANE, ATMARPC, ATMARPS and Winsock do not require configuration when used over ATM.&lt;/P&gt;

&lt;P&gt;The following table shows the settings for Ethernet-PVC configurations.&lt;/P&gt;

&lt;P class="fineprint"&gt;&lt;/P&gt;

&lt;TABLE&gt;

&lt;TR VALIGN="top"&gt;
&lt;TH width=23%&gt;Property/setting name&lt;/TH&gt;
&lt;TH width=13%&gt;Default setting&lt;/TH&gt;
&lt;TH width=34%&gt;Description&lt;/TH&gt;
&lt;TH width=15%&gt;Property example &lt;/TH&gt;
&lt;TH width=15%&gt;Setting example &lt;/TH&gt;
&lt;/TR&gt;

&lt;TR VALIGN="top"&gt;
&lt;TD width=23%&gt;VPI&lt;/TD&gt;
&lt;TD width=13%&gt;0&lt;/TD&gt;
&lt;TD width=34%&gt;The virtual path identifier for the PVC on which the driver operates.&lt;/TD&gt;
&lt;TD width=15%&gt;VPI&lt;/TD&gt;
&lt;TD width=15%&gt;1&lt;/TD&gt;
&lt;/TR&gt;

&lt;TR VALIGN="top"&gt;
&lt;TD width=23%&gt;VCI&lt;/TD&gt;
&lt;TD width=13%&gt;0&lt;/TD&gt;
&lt;TD width=34%&gt;The virtual circuit identifier for the PVC.&lt;/TD&gt;
&lt;TD width=15%&gt;VCI&lt;/TD&gt;
&lt;TD width=15%&gt;100&lt;/TD&gt;
&lt;/TR&gt;

&lt;TR VALIGN="top"&gt;
&lt;TD width=23%&gt;Encap&lt;/TD&gt;
&lt;TD width=13%&gt;2&lt;/TD&gt;
&lt;TD width=34%&gt;Defines the encapsulation used for data transfer. Possible values are:
&lt;P&gt;0 &amp;#0150; IPv4&lt;/P&gt;

&lt;P&gt;1 &amp;#0150; IPv4 LLC/SNAP&lt;/P&gt;

&lt;P&gt;2 &amp;#0150; Ethernet&lt;/P&gt;

&lt;P&gt;3 &amp;#0150; Ethernet LLC/SNAP&lt;/P&gt;
&lt;/TD&gt;
&lt;TD width=15%&gt;Encap&lt;/TD&gt;
&lt;TD width=15%&gt;1&lt;/TD&gt;
&lt;/TR&gt;
&lt;/TABLE&gt;&lt;BR&gt;

&lt;P class="fineprint"&gt;&lt;/P&gt;

&lt;H1&gt;Special Notes&lt;/H1&gt;

&lt;P&gt;ATM networks commonly use only one of the four possible operation modes listed above.&lt;/P&gt;

&lt;H1&gt;Links for Further Information&lt;/H1&gt;

&lt;P&gt;For more information on ATM technology, see this &lt;A HREF="http://www.microsoft.com/hwdev"&gt;Microsoft Web site&lt;/A&gt;.&lt;/P&gt;

&lt;/BODY&gt;
&lt;/HTML&gt;
</HELPCONTEXT><DISPLAYNAME>ATM</DISPLAYNAME><VERSION>1.0</VERSION><DESCRIPTION>Asynchronous Transfer Mode support</DESCRIPTION><COPYRIGHT>2000 Microsoft Corp.</COPYRIGHT><VENDOR>Microsoft Corp.</VENDOR><OWNERS>arvindm</OWNERS><AUTHORS>arvindm</AUTHORS><DATECREATED>7/18/2000</DATECREATED><DATEREVISED>4/12/2001</DATEREVISED><RESOURCE Name="File:&quot;%12%&quot;,&quot;atmuni.sys&quot;" ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819"><PROPERTY Name="DstPath" Format="String">%12%</PROPERTY><PROPERTY Name="DstName" Format="String">atmuni.sys</PROPERTY><PROPERTY Name="SrcFileSize" Format="Integer">0</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>ATM UNI</DISPLAYNAME><DESCRIPTION>ATM UNI 3.1 Call Manager</DESCRIPTION></RESOURCE><RESOURCE Name="File:&quot;%12%&quot;,&quot;atmlane.sys&quot;" ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819"><PROPERTY Name="DstPath" Format="String">%12%</PROPERTY><PROPERTY Name="DstName" Format="String">atmlane.sys</PROPERTY><PROPERTY Name="SrcFileSize" Format="Integer">0</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>ATM LANE</DISPLAYNAME><DESCRIPTION>ATM LAN Emulation Protocol Driver</DESCRIPTION></RESOURCE><RESOURCE Name="File:&quot;%12%&quot;,&quot;atmarpc.sys&quot;" ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819"><PROPERTY Name="DstPath" Format="String">%12%</PROPERTY><PROPERTY Name="DstName" Format="String">atmarpc.sys</PROPERTY><PROPERTY Name="SrcFileSize" Format="Integer">0</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>ATM ARP Client</DISPLAYNAME><DESCRIPTION>ATM ARP Client Protocol Driver (Classical IP over ATM)</DESCRIPTION></RESOURCE><RESOURCE Name="File:&quot;%11%&quot;,&quot;atmadm.exe&quot;" ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">atmadm.exe</PROPERTY><PROPERTY Name="SrcFileSize" Format="Integer">0</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>ATM Admin</DISPLAYNAME><DESCRIPTION>ATM Administration Utility Program</DESCRIPTION></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;netauni.inf&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">netauni.inf</PROPERTY></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;netlanem.inf&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">netlanem.inf</PROPERTY></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;netlanep.inf&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">netlanep.inf</PROPERTY></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;nettcpip.inf&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">nettcpip.inf</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%11%&quot;,&quot;wshatm.dll&quot;"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">wshatm.dll</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>Winsock/ATM</DISPLAYNAME><DESCRIPTION>Winsock services over ATM helper DLL</DESCRIPTION></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;MSVCRT.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">MSVCRT.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;ntdll.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">ntdll.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;KERNEL32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">KERNEL32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;WS2_32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">WS2_32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%12%&quot;,&quot;atmepvc.sys&quot;"><PROPERTY Name="DstPath" Format="String">%12%</PROPERTY><PROPERTY Name="DstName" Format="String">atmepvc.sys</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>Ethernet PVC</DISPLAYNAME><DESCRIPTION>Ethernet over ATM Permanent Virtual Circuit (RFC 2684)</DESCRIPTION></RESOURCE><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%11%&quot;,&quot;atmpvcno.dll&quot;"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">atmpvcno.dll</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>Ethernet PVC Notify Object</DISPLAYNAME><DESCRIPTION>Network configuration notify object for Ethernet PVC (atmepvc)</DESCRIPTION></RESOURCE><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%12%&quot;,&quot;rawwan.sys&quot;"><PROPERTY Name="DstPath" Format="String">%12%</PROPERTY><PROPERTY Name="DstName" Format="String">rawwan.sys</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>Winsock/ATM driver</DISPLAYNAME><DESCRIPTION>Winsock application support over ATM networks</DESCRIPTION></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;ADVAPI32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">ADVAPI32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;ole32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">ole32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;OLEAUT32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">OLEAUT32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;USER32.dll&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">USER32.dll</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;netepvcm.inf&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">netepvcm.inf</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;netepvcp.inf&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">netepvcp.inf</PROPERTY></RESOURCE><GROUPMEMBER GroupVSGUID="{388249D2-1897-44ff-86F2-E159A27AA037}"/></COMPONENT></DCARRIER>
