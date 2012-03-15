# Microsoft Developer Studio Project File - Name="nasl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=nasl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "nasl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "nasl.mak" CFG="nasl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "nasl - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "nasl - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "nasl - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "WINDOWSNT" /D "NESSUSNT" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"../bin/nasl.exe"

!ELSEIF  "$(CFG)" == "nasl - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "WINDOWSNT" /D "NESSUSNT" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"../bin/nasl.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "nasl - Win32 Release"
# Name "nasl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "port"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\port\mstcpip.h
# End Source File
# Begin Source File

SOURCE=.\port\port.c
# End Source File
# Begin Source File

SOURCE=.\port\port.h
# End Source File
# Begin Source File

SOURCE=.\port\synscan.c
# End Source File
# Begin Source File

SOURCE=.\port\synscan.h
# End Source File
# Begin Source File

SOURCE=.\port\tcpscan.c
# End Source File
# Begin Source File

SOURCE=.\port\tcpscan.h
# End Source File
# End Group
# Begin Group "report"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\report\html_output.c
# End Source File
# Begin Source File

SOURCE=.\report\html_output.h
# End Source File
# Begin Source File

SOURCE=.\report\nsr_output.c
# End Source File
# Begin Source File

SOURCE=.\report\nsr_output.h
# End Source File
# Begin Source File

SOURCE=.\report\report.c
# End Source File
# Begin Source File

SOURCE=.\report\report.h
# End Source File
# Begin Source File

SOURCE=.\report\report_utils.c
# End Source File
# Begin Source File

SOURCE=.\report\report_utils.h
# End Source File
# Begin Source File

SOURCE=.\report\text_output.c
# End Source File
# Begin Source File

SOURCE=.\report\text_output.h
# End Source File
# Begin Source File

SOURCE=.\report\xml_output.c
# End Source File
# Begin Source File

SOURCE=.\report\xml_output.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\arglists.c
# End Source File
# Begin Source File

SOURCE=.\attack.c
# End Source File
# Begin Source File

SOURCE=.\bpf_share.c
# End Source File
# Begin Source File

SOURCE=.\capture_packet.c
# End Source File
# Begin Source File

SOURCE=.\diff.c
# End Source File
# Begin Source File

SOURCE=.\exec.c
# End Source File
# Begin Source File

SOURCE=.\find_service.c
# End Source File
# Begin Source File

SOURCE=.\ftp_funcs.c
# End Source File
# Begin Source File

SOURCE=.\ftp_funcs.h
# End Source File
# Begin Source File

SOURCE=.\getopt.c
# End Source File
# Begin Source File

SOURCE=.\getopt1.c
# End Source File
# Begin Source File

SOURCE=.\harglists.c
# End Source File
# Begin Source File

SOURCE=.\hlst.c
# End Source File
# Begin Source File

SOURCE=.\hmacmd5.c
# End Source File
# Begin Source File

SOURCE=.\iflist.c
# End Source File
# Begin Source File

SOURCE=.\kb.c
# End Source File
# Begin Source File

SOURCE=.\lint.c
# End Source File
# Begin Source File

SOURCE=.\md5.c
# End Source File
# Begin Source File

SOURCE=.\nasl.c
# End Source File
# Begin Source File

SOURCE=.\nasl_cmd_exec.c
# End Source File
# Begin Source File

SOURCE=.\nasl_crypto.c
# End Source File
# Begin Source File

SOURCE=.\nasl_crypto.h
# End Source File
# Begin Source File

SOURCE=.\nasl_crypto2.c
# End Source File
# Begin Source File

SOURCE=.\nasl_debug.c
# End Source File
# Begin Source File

SOURCE=.\nasl_func.c
# End Source File
# Begin Source File

SOURCE=.\nasl_grammar_tab.c
# End Source File
# Begin Source File

SOURCE=.\nasl_host.c
# End Source File
# Begin Source File

SOURCE=.\nasl_host.h
# End Source File
# Begin Source File

SOURCE=.\nasl_http.c
# End Source File
# Begin Source File

SOURCE=.\nasl_http.h
# End Source File
# Begin Source File

SOURCE=.\nasl_init.c
# End Source File
# Begin Source File

SOURCE=.\nasl_lex_ctxt.c
# End Source File
# Begin Source File

SOURCE=.\nasl_misc_funcs.c
# End Source File
# Begin Source File

SOURCE=.\nasl_misc_funcs.h
# End Source File
# Begin Source File

SOURCE=.\nasl_nessusd_glue.c
# End Source File
# Begin Source File

SOURCE=.\nasl_nessusd_glue.h
# End Source File
# Begin Source File

SOURCE=.\nasl_packet_forgery.c
# End Source File
# Begin Source File

SOURCE=.\nasl_packet_forgery.h
# End Source File
# Begin Source File

SOURCE=.\nasl_plugins.c
# End Source File
# Begin Source File

SOURCE=.\nasl_socket.c
# End Source File
# Begin Source File

SOURCE=.\nasl_socket.h
# End Source File
# Begin Source File

SOURCE=.\nasl_text_utils.c
# End Source File
# Begin Source File

SOURCE=.\nasl_text_utils.h
# End Source File
# Begin Source File

SOURCE=.\nasl_tree.c
# End Source File
# Begin Source File

SOURCE=.\nasl_var.c
# End Source File
# Begin Source File

SOURCE=.\network.c
# End Source File
# Begin Source File

SOURCE=.\pluginscheduler.c
# End Source File
# Begin Source File

SOURCE=.\plugs_req.c
# End Source File
# Begin Source File

SOURCE=.\plugutils.c
# End Source File
# Begin Source File

SOURCE=.\plugutils.h
# End Source File
# Begin Source File

SOURCE=.\preparse.c
# End Source File
# Begin Source File

SOURCE=.\rand.c
# End Source File
# Begin Source File

SOURCE=.\regex.c
# End Source File
# Begin Source File

SOURCE=.\resolve.c
# End Source File
# Begin Source File

SOURCE=.\scanners_utils.c
# End Source File
# Begin Source File

SOURCE=.\services.c
# End Source File
# Begin Source File

SOURCE=.\snprintf.c
# End Source File
# Begin Source File

SOURCE=.\store.c
# End Source File
# Begin Source File

SOURCE=.\strutils.c
# End Source File
# Begin Source File

SOURCE=.\system.c
# End Source File
# Begin Source File

SOURCE=.\tcp.c
# End Source File
# Begin Source File

SOURCE=.\www_funcs.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\attack.h
# End Source File
# Begin Source File

SOURCE=.\capture_packet.h
# End Source File
# Begin Source File

SOURCE=.\comm.h
# End Source File
# Begin Source File

SOURCE=.\diff.h
# End Source File
# Begin Source File

SOURCE=.\exec.h
# End Source File
# Begin Source File

SOURCE=.\getopt.h
# End Source File
# Begin Source File

SOURCE=.\harglists.h
# End Source File
# Begin Source File

SOURCE=.\hlst.h
# End Source File
# Begin Source File

SOURCE=.\hmacmd5.h
# End Source File
# Begin Source File

SOURCE=.\includes.h
# End Source File
# Begin Source File

SOURCE=.\libnessus.h
# End Source File
# Begin Source File

SOURCE=.\list.h
# End Source File
# Begin Source File

SOURCE=.\md5.h
# End Source File
# Begin Source File

SOURCE=.\nasl.h
# End Source File
# Begin Source File

SOURCE=.\nasl_cmd_exec.h
# End Source File
# Begin Source File

SOURCE=.\nasl_crypto2.h
# End Source File
# Begin Source File

SOURCE=.\nasl_debug.h
# End Source File
# Begin Source File

SOURCE=.\nasl_func.h
# End Source File
# Begin Source File

SOURCE=.\nasl_global_ctxt.h
# End Source File
# Begin Source File

SOURCE=.\nasl_grammar_tab.h
# End Source File
# Begin Source File

SOURCE=.\nasl_icmp.h
# End Source File
# Begin Source File

SOURCE=.\nasl_init.h
# End Source File
# Begin Source File

SOURCE=.\nasl_ip.h
# End Source File
# Begin Source File

SOURCE=.\nasl_lex_ctxt.h
# End Source File
# Begin Source File

SOURCE=.\nasl_raw.h
# End Source File
# Begin Source File

SOURCE=.\nasl_regex.h
# End Source File
# Begin Source File

SOURCE=.\nasl_tcp.h
# End Source File
# Begin Source File

SOURCE=.\nasl_tree.h
# End Source File
# Begin Source File

SOURCE=.\nasl_udp.h
# End Source File
# Begin Source File

SOURCE=.\nasl_var.h
# End Source File
# Begin Source File

SOURCE=.\network.h
# End Source File
# Begin Source File

SOURCE=.\pluginscheduler.h
# End Source File
# Begin Source File

SOURCE=.\plugs_req.h
# End Source File
# Begin Source File

SOURCE=.\preparse.h
# End Source File
# Begin Source File

SOURCE=.\rand.h
# End Source File
# Begin Source File

SOURCE=.\resolve.h
# End Source File
# Begin Source File

SOURCE=.\scanners_utils.h
# End Source File
# Begin Source File

SOURCE=.\services.h
# End Source File
# Begin Source File

SOURCE=.\store.h
# End Source File
# Begin Source File

SOURCE=.\strutils.h
# End Source File
# Begin Source File

SOURCE=.\system.h
# End Source File
# Begin Source File

SOURCE=.\tcp.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
