# Microsoft Developer Studio Project File - Name="OrionScript" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=OrionScript - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OrionScript.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OrionScript.mak" CFG="OrionScript - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OrionScript - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "OrionScript - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OrionScript - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ORIONSCRIPT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I "..\..\Libraries\Zend\Zend" /I "..\..\Libraries\Zend\tsrm" /I "." /I "..\..\Libraries\pcre-5.0" /D "NDEBUG" /D PHPAPI= /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ORIONSCRIPT_EXPORTS" /D "ZEND_WIN32" /D ZEND_DEBUG=0 /D HAVE_STRING_H=1 /D LIBZEND_EXPORTS=1 /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Zend.lib wsock32.lib pcre.lib /nologo /dll /machine:I386 /libpath:"..\..\Libraries\Zend\Zend\Release\\" /libpath:"..\..\Libraries\pcre-5.0\Release"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Release\OrionScript.dll ..	cd Release	e:\borland\cbuilder5\bin\implib -a OrionScript_Borland.lib OrionScript.dll	cd ..
# End Special Build Tool

!ELSEIF  "$(CFG)" == "OrionScript - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ORIONSCRIPT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /ZI /Od /I "..\..\Libraries\Zend\Zend" /I "..\..\Libraries\Zend\tsrm" /I "." /I "..\..\Libraries\pcre-5.0" /D "_DEBUG" /D PHPAPI= /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ORIONSCRIPT_EXPORTS" /D "ZEND_WIN32" /D ZEND_DEBUG=0 /D HAVE_STRING_H=1 /D LIBZEND_EXPORTS=1 /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Zend.lib wsock32.lib pcre.lib /nologo /dll /debug /machine:I386 /pdbtype:sept /libpath:"..\..\Libraries\Zend\Zend\Debug" /libpath:"..\..\Libraries\pcre-5.0\Release"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy Debug\OrionScript.dll ..
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "OrionScript - Win32 Release"
# Name "OrionScript - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "modules"

# PROP Default_Filter ""
# Begin Group "base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\modules\base\array.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\base64.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\basic_functions.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\datetime.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\dns.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\formatted_print.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\incomplete_class.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\lcg.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\math.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\microtime.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\parsedate.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\rand.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\scanf.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\sha1.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\string.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\strnatcmp.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\type.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\uniqid.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\uuencode.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\var.c
# End Source File
# Begin Source File

SOURCE=.\modules\base\var_unserializer.c
# End Source File
# End Group
# Begin Group "pcre"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\modules\pcre\php_pcre.c
# End Source File
# End Group
# Begin Group "ctype"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\modules\ctype\ctype.c
# End Source File
# End Group
# Begin Group "orion"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\modules\orion\mod_orion.c
# End Source File
# End Group
# End Group
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\win32\globals.c
# End Source File
# Begin Source File

SOURCE=.\win32\pwd.c
# End Source File
# Begin Source File

SOURCE=.\win32\readdir.c
# End Source File
# Begin Source File

SOURCE=.\win32\time.c
# End Source File
# Begin Source File

SOURCE=.\win32\winutil.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\OrionScript.def
# End Source File
# Begin Source File

SOURCE=.\php_sprintf.c
# End Source File
# Begin Source File

SOURCE=.\reentrancy.c
# End Source File
# Begin Source File

SOURCE=.\snprintf.c
# End Source File
# Begin Source File

SOURCE=.\spprintf.c
# End Source File
# Begin Source File

SOURCE=.\strlcat.c
# End Source File
# Begin Source File

SOURCE=.\strlcpy.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\OrionScript.h
# End Source File
# Begin Source File

SOURCE=.\php.h
# End Source File
# Begin Source File

SOURCE=.\snprintf.h
# End Source File
# Begin Source File

SOURCE=.\spprintf.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\OrionScript.rc
# End Source File
# End Group
# End Target
# End Project
