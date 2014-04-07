# Microsoft Developer Studio Project File - Name="MCP for NT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MCP for NT - Win32 Release
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "MCP for NT.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "MCP for NT.mak" CFG="MCP for NT - Win32 Release"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "MCP for NT - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "MCP for NT - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MCP for NT - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 camac.lib rpcapi32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 camac.lib rpcapi32.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "MCP for NT - Win32 Release"
# Name "MCP for NT - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\ActionObjects.cpp
# End Source File
# Begin Source File

SOURCE=.\Calculator.cpp
# End Source File
# Begin Source File

SOURCE=.\CamacTester.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ComputerDelay.cpp
# End Source File
# Begin Source File

SOURCE=.\Configure.cpp
# End Source File
# Begin Source File

SOURCE=.\DataChild.cpp
# End Source File
# Begin Source File

SOURCE=.\DataReadout.cpp
# End Source File
# Begin Source File

SOURCE=.\DataView.cpp
# End Source File
# Begin Source File

SOURCE=.\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorChild.cpp
# End Source File
# Begin Source File

SOURCE=.\EditorView.cpp
# End Source File
# Begin Source File

SOURCE=.\EVALUATE.CPP
# End Source File
# Begin Source File

SOURCE=.\FastTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\FileIO.cpp
# End Source File
# Begin Source File

SOURCE=.\Hardware.cpp
# End Source File
# Begin Source File

SOURCE=.\HardwareDefaults.cpp
# End Source File
# Begin Source File

SOURCE=.\HfsFitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LSIChild.cpp
# End Source File
# Begin Source File

SOURCE=.\LSIView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=".\MCP for NT.cpp"
# End Source File
# Begin Source File

SOURCE=".\hlp\MCP for NT.hpj"

!IF  "$(CFG)" == "MCP for NT - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=MCP for NT
InputPath=".\hlp\MCP for NT.hpj"

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=MCP for NT
InputPath=".\hlp\MCP for NT.hpj"

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\MCP for NT.rc"
# End Source File
# Begin Source File

SOURCE=".\MCP for NTDoc.cpp"
# End Source File
# Begin Source File

SOURCE=".\MCP for NTView.cpp"
# End Source File
# Begin Source File

SOURCE=.\MeasThread.cpp
# End Source File
# Begin Source File

SOURCE=.\minuit.cpp
# End Source File
# Begin Source File

SOURCE=.\MinuitChild.cpp
# End Source File
# Begin Source File

SOURCE=.\MinuitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorChild.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorView.cpp
# End Source File
# Begin Source File

SOURCE=.\NetReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1am.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1AS.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1Bo.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1fs.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1FV.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1HP.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1HV.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1MF.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1MS.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1PM.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1PR.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1RS.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1TI.cpp
# End Source File
# Begin Source File

SOURCE=.\Page1TR.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3am.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3AS.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3Bo.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3fs.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3FV.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3HP.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3HV.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3MF.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3MS.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3PM.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3PR.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3RS.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3TI.cpp
# End Source File
# Begin Source File

SOURCE=.\Page3TR.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtam.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtAS.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtBo.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtfs.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtFV.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtHP.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtHV.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtMF.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtMS.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtPM.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtPR.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtRS.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtTI.cpp
# End Source File
# Begin Source File

SOURCE=.\PropShtTR.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StepTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\TimerDelay.cpp
# End Source File
# Begin Source File

SOURCE=.\TriggerPartSweep.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\ActionObjects.h
# End Source File
# Begin Source File

SOURCE=.\Calculator.h
# End Source File
# Begin Source File

SOURCE=.\camac.h
# End Source File
# Begin Source File

SOURCE=.\CamacTester.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\ComputerDelay.h
# End Source File
# Begin Source File

SOURCE=.\Configure.h
# End Source File
# Begin Source File

SOURCE=.\DataChild.h
# End Source File
# Begin Source File

SOURCE=.\DataReadout.h
# End Source File
# Begin Source File

SOURCE=.\DataView.h
# End Source File
# Begin Source File

SOURCE=.\display.h
# End Source File
# Begin Source File

SOURCE=.\EditorChild.h
# End Source File
# Begin Source File

SOURCE=.\EditorView.h
# End Source File
# Begin Source File

SOURCE=.\eval.h
# End Source File
# Begin Source File

SOURCE=.\evaluate.h
# End Source File
# Begin Source File

SOURCE=.\FastTimer.h
# End Source File
# Begin Source File

SOURCE=.\FileIO.h
# End Source File
# Begin Source File

SOURCE=.\Hardware.h
# End Source File
# Begin Source File

SOURCE=.\HardwareConfig.h
# End Source File
# Begin Source File

SOURCE=.\HardwareDefaults.h
# End Source File
# Begin Source File

SOURCE=.\HfsFitDlg.h
# End Source File
# Begin Source File

SOURCE=.\LSIChild.h
# End Source File
# Begin Source File

SOURCE=.\LSIView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=".\MCP for NT.h"
# End Source File
# Begin Source File

SOURCE=".\MCP for NTDoc.h"
# End Source File
# Begin Source File

SOURCE=".\MCP for NTView.h"
# End Source File
# Begin Source File

SOURCE=.\measthread.h
# End Source File
# Begin Source File

SOURCE=.\minuit.h
# End Source File
# Begin Source File

SOURCE=.\MinuitChild.h
# End Source File
# Begin Source File

SOURCE=.\minuitdlg.h
# End Source File
# Begin Source File

SOURCE=.\MonitorChild.h
# End Source File
# Begin Source File

SOURCE=.\MonitorView.h
# End Source File
# Begin Source File

SOURCE=.\NetReader.h
# End Source File
# Begin Source File

SOURCE=.\Page1am.h
# End Source File
# Begin Source File

SOURCE=.\Page1AS.h
# End Source File
# Begin Source File

SOURCE=.\Page1Bo.h
# End Source File
# Begin Source File

SOURCE=.\Page1fs.h
# End Source File
# Begin Source File

SOURCE=.\Page1FV.h
# End Source File
# Begin Source File

SOURCE=.\Page1HP.h
# End Source File
# Begin Source File

SOURCE=.\Page1HV.h
# End Source File
# Begin Source File

SOURCE=.\Page1MF.h
# End Source File
# Begin Source File

SOURCE=.\Page1MS.h
# End Source File
# Begin Source File

SOURCE=.\Page1PM.h
# End Source File
# Begin Source File

SOURCE=.\Page1PR.h
# End Source File
# Begin Source File

SOURCE=.\Page1RS.h
# End Source File
# Begin Source File

SOURCE=.\page1ti.h
# End Source File
# Begin Source File

SOURCE=.\Page1TR.h
# End Source File
# Begin Source File

SOURCE=.\Page3am.h
# End Source File
# Begin Source File

SOURCE=.\Page3AS.h
# End Source File
# Begin Source File

SOURCE=.\Page3Bo.h
# End Source File
# Begin Source File

SOURCE=.\Page3fs.h
# End Source File
# Begin Source File

SOURCE=.\Page3FV.h
# End Source File
# Begin Source File

SOURCE=.\Page3HP.h
# End Source File
# Begin Source File

SOURCE=.\Page3HV.h
# End Source File
# Begin Source File

SOURCE=.\Page3MF.h
# End Source File
# Begin Source File

SOURCE=.\Page3MS.h
# End Source File
# Begin Source File

SOURCE=.\Page3PM.h
# End Source File
# Begin Source File

SOURCE=.\Page3PR.h
# End Source File
# Begin Source File

SOURCE=.\Page3RS.h
# End Source File
# Begin Source File

SOURCE=.\Page3TI.h
# End Source File
# Begin Source File

SOURCE=.\Page3TR.h
# End Source File
# Begin Source File

SOURCE=.\PropShtam.h
# End Source File
# Begin Source File

SOURCE=.\PropShtAS.h
# End Source File
# Begin Source File

SOURCE=.\PropShtBo.h
# End Source File
# Begin Source File

SOURCE=.\PropShtfs.h
# End Source File
# Begin Source File

SOURCE=.\PropShtFV.h
# End Source File
# Begin Source File

SOURCE=.\PropShtHP.h
# End Source File
# Begin Source File

SOURCE=.\PropShtHV.h
# End Source File
# Begin Source File

SOURCE=.\PropShtMF.h
# End Source File
# Begin Source File

SOURCE=.\PropShtMS.h
# End Source File
# Begin Source File

SOURCE=.\PropShtPM.h
# End Source File
# Begin Source File

SOURCE=.\PropShtPR.h
# End Source File
# Begin Source File

SOURCE=.\PropShtRS.h
# End Source File
# Begin Source File

SOURCE=.\PropShtTI.h
# End Source File
# Begin Source File

SOURCE=.\PropShtTR.h
# End Source File
# Begin Source File

SOURCE=.\rpcapi32.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StepTimer.h
# End Source File
# Begin Source File

SOURCE=.\TheDefines.h
# End Source File
# Begin Source File

SOURCE=.\TimerDelay.h
# End Source File
# Begin Source File

SOURCE=.\TriggerPartSweep.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\135.ico
# End Source File
# Begin Source File

SOURCE=".\res\APPS INF.ICO"
# End Source File
# Begin Source File

SOURCE=".\res\APPS STO.ICO"
# End Source File
# Begin Source File

SOURCE=".\res\APPS WAR.ICO"
# End Source File
# Begin Source File

SOURCE=.\res\coll.bmp
# End Source File
# Begin Source File

SOURCE=".\res\MCP for NT.ico"
# End Source File
# Begin Source File

SOURCE=".\res\MCP for NT.rc2"
# End Source File
# Begin Source File

SOURCE=".\res\MCP for NTDoc.ico"
# End Source File
# Begin Source File

SOURCE=.\RES\origin.ico
# End Source File
# Begin Source File

SOURCE=.\RES\origin2.ico
# End Source File
# Begin Source File

SOURCE=".\res\PEAL BAC.ICO"
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# End Target
# End Project
# Section MCP for NT : {7B705998-4924-11D0-AF9B-00A024889213}
# 	0:8:Splash.h:E:\MSDEV\projects\MCPNT\Splash.h
# 	0:10:Splash.cpp:E:\MSDEV\projects\MCPNT\Splash.cpp
# 	1:10:IDB_SPLASH:117
# 	2:10:ResHdrName:resource.h
# 	2:11:ProjHdrName:stdafx.h
# 	2:10:WrapperDef:_SPLASH_SCRN_
# 	2:12:SplClassName:CSplashWnd
# 	2:21:SplashScreenInsertKey:4.0
# 	2:10:HeaderName:Splash.h
# 	2:10:ImplemName:Splash.cpp
# 	2:7:BmpID16:IDB_SPLASH
# End Section
# Section MCP for NT : {7B705997-4924-11D0-AF9B-00A024889213}
# 	1:17:CG_IDS_DISK_SPACE:113
# 	1:19:CG_IDS_PHYSICAL_MEM:112
# 	1:25:CG_IDS_DISK_SPACE_UNAVAIL:114
# 	2:10:SysInfoKey:1234
# End Section
# Section MCP for NT : {7B705992-4924-11D0-AF9B-00A024889213}
# 	2:10:CMainFrame:Palette support added
# End Section
# Section MCP for NT : {B6805000-A509-11CE-A5B0-00AA006BBF16}
# 	1:23:CG_IDR_POPUP_DATA_CHILD:118
# 	1:26:CG_IDR_POPUP_MCPFOR_NTVIEW:109
# End Section
