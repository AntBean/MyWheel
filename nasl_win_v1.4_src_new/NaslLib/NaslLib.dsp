# Microsoft Developer Studio Project File - Name="NaslLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=NaslLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NaslLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NaslLib.mak" CFG="NaslLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NaslLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "NaslLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NaslLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NASLLIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NASLLIB_EXPORTS" /D "WINDOWSNT" /D "NESSUSNT" /D "_XSCAN_PLUG" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../bin/NaslLib.dll"

!ELSEIF  "$(CFG)" == "NaslLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NASLLIB_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "NASLLIB_EXPORTS" /D "WINDOWSNT" /D "NESSUSNT" /D "_XSCAN_PLUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../bin/NaslLib.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "NaslLib - Win32 Release"
# Name "NaslLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "nasl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\arglists.c
# End Source File
# Begin Source File

SOURCE=..\src\attack.c
# End Source File
# Begin Source File

SOURCE=..\src\attack.h
# End Source File
# Begin Source File

SOURCE=..\src\bpf_share.c
# End Source File
# Begin Source File

SOURCE=..\src\capture_packet.c
# End Source File
# Begin Source File

SOURCE=..\src\capture_packet.h
# End Source File
# Begin Source File

SOURCE=..\src\comm.h
# End Source File
# Begin Source File

SOURCE=..\src\diff.c
# End Source File
# Begin Source File

SOURCE=..\src\diff.h
# End Source File
# Begin Source File

SOURCE=..\src\exec.c
# End Source File
# Begin Source File

SOURCE=..\src\exec.h
# End Source File
# Begin Source File

SOURCE=..\src\find_service.c
# End Source File
# Begin Source File

SOURCE=..\src\ftp_funcs.c
# End Source File
# Begin Source File

SOURCE=..\src\ftp_funcs.h
# End Source File
# Begin Source File

SOURCE=..\src\getopt.c
# End Source File
# Begin Source File

SOURCE=..\src\getopt.h
# End Source File
# Begin Source File

SOURCE=..\src\getopt1.c
# End Source File
# Begin Source File

SOURCE=..\src\harglists.c
# End Source File
# Begin Source File

SOURCE=..\src\harglists.h
# End Source File
# Begin Source File

SOURCE=..\src\hlst.c
# End Source File
# Begin Source File

SOURCE=..\src\hlst.h
# End Source File
# Begin Source File

SOURCE=..\src\hmacmd5.c
# End Source File
# Begin Source File

SOURCE=..\src\hmacmd5.h
# End Source File
# Begin Source File

SOURCE=..\src\iflist.c
# End Source File
# Begin Source File

SOURCE=..\src\includes.h
# End Source File
# Begin Source File

SOURCE=..\src\kb.c
# End Source File
# Begin Source File

SOURCE=..\src\LIBNESSUS.H
# End Source File
# Begin Source File

SOURCE=..\src\lint.c
# End Source File
# Begin Source File

SOURCE=..\src\list.h
# End Source File
# Begin Source File

SOURCE=..\src\md5.c
# End Source File
# Begin Source File

SOURCE=..\src\md5.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_cmd_exec.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_cmd_exec.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_crypto.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_crypto.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_crypto2.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_crypto2.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_debug.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_debug.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_func.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_func.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_global_ctxt.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_grammar_tab.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_grammar_tab.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_host.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_host.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_http.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_http.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_icmp.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_init.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_init.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_ip.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_lex_ctxt.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_lex_ctxt.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_misc_funcs.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_misc_funcs.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_nessusd_glue.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_nessusd_glue.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_packet_forgery.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_packet_forgery.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_plugins.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_raw.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_regex.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_socket.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_socket.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_tcp.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_text_utils.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_text_utils.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_tree.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_tree.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_udp.h
# End Source File
# Begin Source File

SOURCE=..\src\nasl_var.c
# End Source File
# Begin Source File

SOURCE=..\src\nasl_var.h
# End Source File
# Begin Source File

SOURCE=..\src\network.c
# End Source File
# Begin Source File

SOURCE=..\src\network.h
# End Source File
# Begin Source File

SOURCE=..\src\pluginscheduler.c
# End Source File
# Begin Source File

SOURCE=..\src\pluginscheduler.h
# End Source File
# Begin Source File

SOURCE=..\src\plugs_req.c
# End Source File
# Begin Source File

SOURCE=..\src\plugs_req.h
# End Source File
# Begin Source File

SOURCE=..\src\plugutils.c
# End Source File
# Begin Source File

SOURCE=..\src\plugutils.h
# End Source File
# Begin Source File

SOURCE=..\src\preparse.c
# End Source File
# Begin Source File

SOURCE=..\src\preparse.h
# End Source File
# Begin Source File

SOURCE=..\src\rand.c
# End Source File
# Begin Source File

SOURCE=..\src\rand.h
# End Source File
# Begin Source File

SOURCE=..\src\regex.c
# End Source File
# Begin Source File

SOURCE=..\src\resolve.c
# End Source File
# Begin Source File

SOURCE=..\src\resolve.h
# End Source File
# Begin Source File

SOURCE=..\src\scanners_utils.c
# End Source File
# Begin Source File

SOURCE=..\src\scanners_utils.h
# End Source File
# Begin Source File

SOURCE=..\src\services.c
# End Source File
# Begin Source File

SOURCE=..\src\services.h
# End Source File
# Begin Source File

SOURCE=..\src\snprintf.c
# End Source File
# Begin Source File

SOURCE=..\src\store.c
# End Source File
# Begin Source File

SOURCE=..\src\store.h
# End Source File
# Begin Source File

SOURCE=..\src\strutils.c
# End Source File
# Begin Source File

SOURCE=..\src\strutils.h
# End Source File
# Begin Source File

SOURCE=..\src\system.c
# End Source File
# Begin Source File

SOURCE=..\src\system.h
# End Source File
# Begin Source File

SOURCE=..\src\tcp.c
# End Source File
# Begin Source File

SOURCE=..\src\tcp.h
# End Source File
# Begin Source File

SOURCE=..\src\www_funcs.c
# End Source File
# End Group
# Begin Group "port"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\port\mstcpip.h
# End Source File
# Begin Source File

SOURCE=..\src\port\port.c
# End Source File
# Begin Source File

SOURCE=..\src\port\port.h
# End Source File
# Begin Source File

SOURCE=..\src\port\synscan.c
# End Source File
# Begin Source File

SOURCE=..\src\port\synscan.h
# End Source File
# Begin Source File

SOURCE=..\src\port\tcpscan.c
# End Source File
# Begin Source File

SOURCE=..\src\port\tcpscan.h
# End Source File
# End Group
# Begin Group "report"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\report\html_output.c
# End Source File
# Begin Source File

SOURCE=..\src\report\html_output.h
# End Source File
# Begin Source File

SOURCE=..\src\report\nsr_output.c
# End Source File
# Begin Source File

SOURCE=..\src\report\nsr_output.h
# End Source File
# Begin Source File

SOURCE=..\src\report\report.c
# End Source File
# Begin Source File

SOURCE=..\src\report\report.h
# End Source File
# Begin Source File

SOURCE=..\src\report\report_utils.c
# End Source File
# Begin Source File

SOURCE=..\src\report\report_utils.h
# End Source File
# Begin Source File

SOURCE=..\src\report\text_output.c
# End Source File
# Begin Source File

SOURCE=..\src\report\text_output.h
# End Source File
# Begin Source File

SOURCE=..\src\report\xml_output.c
# End Source File
# Begin Source File

SOURCE=..\src\report\xml_output.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\NaslLib.cpp
# End Source File
# Begin Source File

SOURCE=.\NaslLib.def
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\NaslLib.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
