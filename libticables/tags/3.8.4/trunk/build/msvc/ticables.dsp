# Microsoft Developer Studio Project File - Name="ticables" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ticables - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ticables.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ticables.mak" CFG="ticables - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ticables - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ticables - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ticables - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir "C:\msvc\Output\ticables"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /w /W0 /GX /I "C:\Gtk2Dev\include" /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "TICABLES_EXPORTS" /D "__WIN32__" /D "__I386__" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"..\..\tests\ticables.dll"

!ELSEIF  "$(CFG)" == "ticables - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:\msvc\tilp"
# PROP Intermediate_Dir "C:\msvc\Output\ticables"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /I "C:\Gtk2Dev\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "ENABLE_NLS" /D "TICABLES_EXPORTS" /D "__WIN32__" /D "__I386__" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o /win32 "NUL"
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /debugtype:both /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /map

!ENDIF 

# Begin Target

# Name "ticables - Win32 Release"
# Name "ticables - Win32 Debug"
# Begin Group "Link cables"

# PROP Default_Filter ""
# Begin Group "TiglUsb"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\win32\tiglusb.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\link_avr.c
# End Source File
# Begin Source File

SOURCE=..\..\src\link_dev.c
# End Source File
# Begin Source File

SOURCE=..\..\src\link_dfl.c
# End Source File
# Begin Source File

SOURCE=..\..\src\link_par.c
# End Source File
# Begin Source File

SOURCE=..\..\src\link_ser.c
# End Source File
# Begin Source File

SOURCE=..\..\src\link_slv.c
# End Source File
# Begin Source File

SOURCE=..\..\src\link_tie.c
# End Source File
# Begin Source File

SOURCE=..\..\src\link_tig.c
# End Source File
# Begin Source File

SOURCE=..\..\src\link_vti.c
# End Source File
# Begin Source File

SOURCE=..\..\src\link_vtl.c
# End Source File
# Begin Source File

SOURCE=..\..\src\win32\porttalk.c
# End Source File
# End Group
# Begin Group "Others"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\detect.c
# End Source File
# Begin Source File

SOURCE=..\..\src\direct.c
# End Source File
# Begin Source File

SOURCE=..\..\src\error.c
# End Source File
# Begin Source File

SOURCE=..\..\src\intrface.c
# End Source File
# Begin Source File

SOURCE=..\..\src\logging.c
# End Source File
# Begin Source File

SOURCE=..\..\src\mapping.c
# End Source File
# Begin Source File

SOURCE=..\..\src\printl.c
# End Source File
# Begin Source File

SOURCE=..\..\src\type2str.c
# End Source File
# Begin Source File

SOURCE=..\..\src\verbose.c
# End Source File
# Begin Source File

SOURCE=..\..\src\win32\win32_detect.c
# End Source File
# Begin Source File

SOURCE=..\..\src\win32\win32_ioports.c
# End Source File
# Begin Source File

SOURCE=..\..\src\win32\win32_mapping.c
# End Source File
# End Group
# Begin Group "Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\src\Avr_link.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Cabl_def.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Cabl_err.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Cabl_int.h
# End Source File
# Begin Source File

SOURCE=..\..\src\default.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Dev_link.h
# End Source File
# Begin Source File

SOURCE=..\..\src\externs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\intl1.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Iocompat.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Ioports.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Links.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Logging.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Par_link.h
# End Source File
# Begin Source File

SOURCE=..\..\src\pause.h
# End Source File
# Begin Source File

SOURCE=..\..\src\plerror.h
# End Source File
# Begin Source File

SOURCE=..\..\src\porttalk_ioctl.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Probe.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Ser_link.h
# End Source File
# Begin Source File

SOURCE=..\..\src\slv_link.h
# End Source File
# Begin Source File

SOURCE=..\..\src\stdints.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Str.h
# End Source File
# Begin Source File

SOURCE=..\..\src\ticables.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Tie_link.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Tig_link.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Timeout.h
# End Source File
# Begin Source File

SOURCE=..\..\src\timodules.h
# End Source File
# Begin Source File

SOURCE=..\..\src\typedefs.h.old
# End Source File
# Begin Source File

SOURCE=..\..\src\Usb_defs.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Verbose.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Vti_link.h
# End Source File
# Begin Source File

SOURCE=..\..\src\Vtl_link.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\src\Cabl_ver.h
# End Source File
# Begin Source File

SOURCE=..\..\..\ChangeLog
# End Source File
# Begin Source File

SOURCE=.\ticables.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\Gtk2Dev\lib\intl.lib
# End Source File
# End Target
# End Project
