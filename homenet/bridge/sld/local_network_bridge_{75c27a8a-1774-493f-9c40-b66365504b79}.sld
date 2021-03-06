<?xml version="1.0" encoding="UTF-16"?>
<!DOCTYPE DCARRIER SYSTEM "mantis.dtd">
<DCARRIER CarrierRevision="1">
	<TOOLINFO ToolName="iCat"><![CDATA[<?xml version="1.0"?>
<!DOCTYPE TOOL SYSTEM "tool.dtd">
<TOOL>
	<CREATED><NAME>iCat</NAME><VSGUID>{97b86ee0-259c-479f-bc46-6cea7ef4be4d}</VSGUID><VERSION>1.0.0.452</VERSION><BUILD>452</BUILD><DATE>3/5/2001</DATE></CREATED><LASTSAVED><NAME>iCat</NAME><VSGUID>{97b86ee0-259c-479f-bc46-6cea7ef4be4d}</VSGUID><VERSION>1.0.0.452</VERSION><BUILD>452</BUILD><DATE>7/6/2001</DATE></LASTSAVED></TOOL>
]]></TOOLINFO><COMPONENT Revision="3" Visibility="1000" MultiInstance="0" Released="1" Editable="1" HTMLFinal="0" ComponentVSGUID="{75C27A8A-1774-493F-9C40-B66365504B79}" ComponentVIGUID="{A1AD7546-8184-4EE4-ABDB-08A1EF3009F3}" PlatformGUID="{B784E719-C196-4DDB-B358-D9254426C38D}" RepositoryVSGUID="{8E0BE9ED-7649-47F3-810B-232D36C430B4}"><HELPCONTEXT src="D:\nt\net\homenet\bridge\sld\_local_network_bridge_component_description.htm">&lt;!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 3.2 Final//EN"&gt;
&lt;HTML DIR="LTR"&gt;&lt;HEAD&gt;
&lt;META HTTP-EQUIV="Content-Type" Content="text/html; charset=Windows-1252"&gt;
&lt;TITLE&gt;Local Network Bridge Component Description&lt;/TITLE&gt;
&lt;style type="text/css"&gt;@import url(td.css);&lt;/style&gt;&lt;/HEAD&gt;
&lt;BODY TOPMARGIN="0"&gt;
&lt;H1&gt;&lt;A NAME="_local_network_bridge_component_description"&gt;&lt;/A&gt;&lt;SUP&gt;&lt;/SUP&gt;Local Network Bridge Component Description&lt;/H1&gt;

&lt;P&gt;The network bridge is a driver that transparently connects two or more network segments so that hosts on those segments identify the resulting internetwork as one network segment. The Network Bridge component sets up small multisegment networks where ease of configuration is more important than scalability. When using Internet Protocol (IP) on a bridged internetwork, the internetwork is treated as a single IP subnet, eliminating the need to configure IP routing. &lt;/P&gt;

&lt;P&gt;The network bridge supports any network interface that is an Ethernet media type and uses standard Ethernet framing. In addition, the network bridge supports IP over 1394 segments with the Microsoft IP1394 driver.&lt;/P&gt;

&lt;P&gt;The network bridge implements the spanning tree algorithm as described in IEEE 802.1D-1990. When hardware permits, the spanning tree algorithm functions as a Media Access Control (MAC) bridge as described in the specification. When the network bridge operates over hardware that does not support promiscuous send and receive operations, such as many wireless adapters, it only supports IP and uses Address Resolution Protocol (ARP) to transparently connect network segments. &lt;/P&gt;

&lt;H1&gt;Component Configuration&lt;/H1&gt;

&lt;P&gt;The bridge is implemented as a Network Driver Interface Specification (NDIS) intermediate network driver. As such, it functions as both a network protocol and a network miniport, also known as a virtual adapter. To operate correctly, the bridge must be bound to two or more network adapters. All components other than the network bridge should be unbound from bridged adapters. Protocols and services that the user wishes to operate over the bridged segments should be bound to the virtual adapter exposed by the network bridge. The tools that allow network protocols, such as TCP/IP, to be bound or unbound from particular network adapters also enable binding operations with the bridge.&lt;/P&gt;

&lt;P&gt;Once bound, the network bridge requires no additional configuration.&lt;/P&gt;

&lt;H1&gt;Special Notes&lt;/H1&gt;

&lt;P&gt;The HNetMon component can be used to force the network bridge to operate in layer 3, for IP-only, mode. This is necessary for certain network adapters that incorrectly report that they support promiscuous send and receive. &lt;/P&gt;

&lt;/BODY&gt;
&lt;/HTML&gt;
</HELPCONTEXT><DISPLAYNAME>Local Network Bridge</DISPLAYNAME><VERSION>1.0</VERSION><DESCRIPTION>Layer Two Ethernet, and IP Bridge</DESCRIPTION><COPYRIGHT>2000 Microsoft Corp.</COPYRIGHT><VENDOR>Microsoft Corp.</VENDOR><OWNERS>maiken</OWNERS><AUTHORS>billi;rgatta; paulclap</AUTHORS><DATECREATED>3/5/2001</DATECREATED><DATEREVISED>7/6/2001</DATEREVISED><RESOURCE ResTypeVSGUID="{E66B49F6-4A35-4246-87E8-5C1A468315B5}" BuildTypeMask="819" Name="File(819):&quot;%12%&quot;,&quot;bridge.sys&quot;"><PROPERTY Name="DstPath" Format="String">%12%</PROPERTY><PROPERTY Name="DstName" Format="String">bridge.sys</PROPERTY><PROPERTY Name="NoExpand" Format="Boolean">0</PROPERTY><DISPLAYNAME>Layer Two Bridge Intermediate Driver</DISPLAYNAME><DESCRIPTION>Layer Two Ethernet, and IP Bridge</DESCRIPTION></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;ndis.sys&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">ndis.sys</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;netbrdgm.inf&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">netbrdgm.inf</PROPERTY></RESOURCE><RESOURCE ResTypeVSGUID="{90D8E195-E710-4AF6-B667-B1805FFC9B8F}" BuildTypeMask="819" Name="RawDep(819):&quot;File&quot;,&quot;netbrdgs.inf&quot;"><PROPERTY Name="RawType" Format="String">File</PROPERTY><PROPERTY Name="Value" Format="String">netbrdgs.inf</PROPERTY></RESOURCE><GROUPMEMBER GroupVSGUID="{E01B4103-3883-4FE8-992F-10566E7B796C}"/><GROUPMEMBER GroupVSGUID="{388249D2-1897-44FF-86F2-E159A27AA037}"/></COMPONENT></DCARRIER>
