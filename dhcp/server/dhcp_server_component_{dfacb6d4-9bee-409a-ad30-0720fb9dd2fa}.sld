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
	<CREATED><NAME>iCat</NAME><VSGUID>{f802f157-4623-426a-8b39-ac2eb4332c74}</VSGUID><VERSION>1.0.0.244</VERSION><BUILD>244</BUILD><DATE>4/17/2000</DATE></CREATED><LASTSAVED><NAME>iCat</NAME><VSGUID>{97b86ee0-259c-479f-bc46-6cea7ef4be4d}</VSGUID><VERSION>1.0.0.452</VERSION><BUILD>452</BUILD><DATE>4/13/2001</DATE></LASTSAVED></TOOL>
]]></TOOLINFO><COMPONENT ComponentVSGUID="{DFACB6D4-9BEE-409A-AD30-0720FB9DD2FA}" ComponentVIGUID="{6DDD0C83-57F3-449C-AFBA-447756C767A3}" PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}" RepositoryVSGUID="{8E0BE9ED-7649-47F3-810B-232D36C430B4}" Revision="3" Visibility="1000" MultiInstance="False" Released="1" Editable="True"><HELPCONTEXT src="D:\nt\net\dhcp\server\dhcp_server_component_description.htm">&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN"&gt;
&lt;HTML DIR="LTR"&gt;&lt;HEAD&gt;
&lt;META HTTP-EQUIV="Content-Type" Content="text/html; charset=Windows-1252"&gt;
&lt;TITLE&gt;DHCP Server Component Description&lt;/TITLE&gt;
&lt;style type="text/css"&gt;@import url(td.css);&lt;/style&gt;&lt;/HEAD&gt;
&lt;BODY TOPMARGIN="0"&gt;
&lt;H1&gt;&lt;A NAME="dhcp_server_component_description"&gt;&lt;/A&gt;&lt;SUP&gt;&lt;/SUP&gt;DHCP Server Component Description&lt;/H1&gt;

&lt;P&gt;The Dynamic Host Configuration Protocol (DHCP) Server component contains the DHCP server.&lt;/P&gt;

&lt;H1&gt;Component Configuration&lt;/H1&gt;

&lt;P&gt;No configuration is required to use this component.&lt;/P&gt;

&lt;/BODY&gt;
&lt;/HTML&gt;
</HELPCONTEXT><DISPLAYNAME>DHCP Server component</DISPLAYNAME><VERSION>1.0</VERSION><DESCRIPTION>DHCP Server service component</DESCRIPTION><COPYRIGHT>2000 Microsoft Corp.</COPYRIGHT><VENDOR>Microsoft Corp.</VENDOR><OWNERS>rterala</OWNERS><AUTHORS>rameshv;rterala</AUTHORS><DATECREATED>4/17/2000</DATECREATED><DATEREVISED>4/13/2001</DATEREVISED><RESOURCE Name="File:&quot;%11%&quot;,&quot;dhcpssvc.dll&quot;" ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">dhcpssvc.dll</PROPERTY><PROPERTY Name="SrcFileSize" Format="Integer">0</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>DHCPSSVC.DLL</DISPLAYNAME><DESCRIPTION>DHCP Server service implementation DLL</DESCRIPTION></RESOURCE><RESOURCE Name="File:&quot;%11%&quot;,&quot;DHCPSAPI.DLL&quot;" ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">DHCPSAPI.DLL</PROPERTY><PROPERTY Name="SrcFileSize" Format="Integer">0</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>DHCPSAPI.DLL</DISPLAYNAME><DESCRIPTION>DHCP Server service API DLL</DESCRIPTION></RESOURCE><RESOURCE Name="File:&quot;%11%&quot;,&quot;DSAUTH.DLL&quot;" ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819"><PROPERTY Name="DstPath" Format="String">%11%</PROPERTY><PROPERTY Name="DstName" Format="String">DSAUTH.DLL</PROPERTY><PROPERTY Name="SrcFileSize" Format="Integer">0</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>DSAUTH.DLL</DISPLAYNAME><DESCRIPTION>DHCP DS implementation DLL</DESCRIPTION></RESOURCE><RESOURCE Name="RegKey:HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\DHCPServer\Parameters\DatabasePath" ResTypeVSGUID="{2C10DB69-39AB-48a4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\DHCPServer\Parameters</PROPERTY><PROPERTY Name="ValueName" Format="String">DatabasePath</PROPERTY><PROPERTY Name="RegValue" Format="String">%SystemRoot%\System32\dhcp</PROPERTY><PROPERTY Name="RegType" Format="Integer">2</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY><DESCRIPTION>Location of DHCP service database</DESCRIPTION></RESOURCE><RESOURCE Name="RegKey:HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\DHCPServer\Parameters\BackupDatabasePath" ResTypeVSGUID="{2C10DB69-39AB-48a4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\DHCPServer\Parameters</PROPERTY><PROPERTY Name="ValueName" Format="String">BackupDatabasePath</PROPERTY><PROPERTY Name="RegValue" Format="String">%SystemRoot%\System32\dhcp\backup</PROPERTY><PROPERTY Name="RegType" Format="Integer">2</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY><DESCRIPTION>Location of DHCP service database backup files</DESCRIPTION></RESOURCE><RESOURCE Name="RegKey:HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\DHCPServer\Parameters\DatabaseName" ResTypeVSGUID="{2C10DB69-39AB-48a4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\DHCPServer\Parameters</PROPERTY><PROPERTY Name="ValueName" Format="String">DatabaseName</PROPERTY><PROPERTY Name="RegValue" Format="String">dhcp.mdb</PROPERTY><PROPERTY Name="RegType" Format="Integer">1</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY><DESCRIPTION>DHCP database file name</DESCRIPTION></RESOURCE><RESOURCE Name="RegKey:HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\DhcpServer\Parameters\ApiProtocolSupport" ResTypeVSGUID="{2C10DB69-39AB-48a4-A83F-9AB3ACBA7C45}" BuildTypeMask="819" Localize="0"><PROPERTY Name="KeyPath" Format="String">HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\DhcpServer\Parameters</PROPERTY><PROPERTY Name="ValueName" Format="String">ApiProtocolSupport</PROPERTY><PROPERTY Name="RegValue" Format="Integer">5</PROPERTY><PROPERTY Name="RegType" Format="Integer">4</PROPERTY><PROPERTY Name="RegOp" Format="Integer">1</PROPERTY><PROPERTY Name="RegCond" Format="Integer">1</PROPERTY></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;netman.dll&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">netman.dll</PROPERTY></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;esent.dll&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">esent.dll</PROPERTY></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;iphlpapi.dll&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">iphlpapi.dll</PROPERTY></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;dnsapi.dll&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">dnsapi.dll</PROPERTY></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;netapi32.dll&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">netapi32.dll</PROPERTY></RESOURCE><RESOURCE Name="RawDep:&quot;RawFile&quot;,&quot;ws2_32.dll&quot;" ResTypeVSGUID="{90D8E195-E710-4af6-B667-B1805FFC9B8F}" BuildTypeMask="819"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">ws2_32.dll</PROPERTY></RESOURCE><GROUPMEMBER GroupVSGUID="{388249D2-1897-44ff-86F2-E159A27AA037}"/></COMPONENT></DCARRIER>
