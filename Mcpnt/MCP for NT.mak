# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=MCP for NT - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to MCP for NT - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MCP for NT - Win32 Release" && "$(CFG)" !=\
 "MCP for NT - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "MCP for NT.mak" CFG="MCP for NT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MCP for NT - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MCP for NT - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "MCP for NT - Win32 Debug"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "MCP for NT - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\MCP for NT.exe" "$(OUTDIR)\MCP for NT.hlp"

CLEAN : 
	-@erase "$(INTDIR)\ActionObjects.obj"
	-@erase "$(INTDIR)\Calculator.obj"
	-@erase "$(INTDIR)\camac.obj"
	-@erase "$(INTDIR)\CamacTester.obj"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ComputerDelay.obj"
	-@erase "$(INTDIR)\Configure.obj"
	-@erase "$(INTDIR)\DataChild.obj"
	-@erase "$(INTDIR)\DataReadout.obj"
	-@erase "$(INTDIR)\DataView.obj"
	-@erase "$(INTDIR)\Display.obj"
	-@erase "$(INTDIR)\EditorChild.obj"
	-@erase "$(INTDIR)\EditorView.obj"
	-@erase "$(INTDIR)\EVALUATE.OBJ"
	-@erase "$(INTDIR)\FastTimer.obj"
	-@erase "$(INTDIR)\FileIO.obj"
	-@erase "$(INTDIR)\GpibReader.obj"
	-@erase "$(INTDIR)\GpibWriter.obj"
	-@erase "$(INTDIR)\Hardware.obj"
	-@erase "$(INTDIR)\HardwareDefaults.obj"
	-@erase "$(INTDIR)\HfsFitDlg.obj"
	-@erase "$(INTDIR)\LSIChild.obj"
	-@erase "$(INTDIR)\LSIView.obj"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MCP for NT.hlp"
	-@erase "$(INTDIR)\MCP for NT.obj"
	-@erase "$(INTDIR)\MCP for NT.pch"
	-@erase "$(INTDIR)\MCP for NT.res"
	-@erase "$(INTDIR)\MCP for NTDoc.obj"
	-@erase "$(INTDIR)\MCP for NTView.obj"
	-@erase "$(INTDIR)\MeasThread.obj"
	-@erase "$(INTDIR)\minuit.obj"
	-@erase "$(INTDIR)\MinuitChild.obj"
	-@erase "$(INTDIR)\MinuitDlg.obj"
	-@erase "$(INTDIR)\MonitorChild.obj"
	-@erase "$(INTDIR)\MonitorView.obj"
	-@erase "$(INTDIR)\NetReader.obj"
	-@erase "$(INTDIR)\NetworkWriter.obj"
	-@erase "$(INTDIR)\Page1am.obj"
	-@erase "$(INTDIR)\Page1AS.obj"
	-@erase "$(INTDIR)\Page1Bo.obj"
	-@erase "$(INTDIR)\Page1FR.obj"
	-@erase "$(INTDIR)\Page1fs.obj"
	-@erase "$(INTDIR)\Page1FV.obj"
	-@erase "$(INTDIR)\Page1FW.obj"
	-@erase "$(INTDIR)\Page1GR.obj"
	-@erase "$(INTDIR)\Page1GW.obj"
	-@erase "$(INTDIR)\Page1HP.obj"
	-@erase "$(INTDIR)\Page1HV.obj"
	-@erase "$(INTDIR)\Page1KR.obj"
	-@erase "$(INTDIR)\Page1KW.obj"
	-@erase "$(INTDIR)\Page1LV.obj"
	-@erase "$(INTDIR)\Page1MF.obj"
	-@erase "$(INTDIR)\Page1MS.obj"
	-@erase "$(INTDIR)\Page1PM.obj"
	-@erase "$(INTDIR)\Page1PR.obj"
	-@erase "$(INTDIR)\Page1RS.obj"
	-@erase "$(INTDIR)\Page1TI.obj"
	-@erase "$(INTDIR)\Page1TR.obj"
	-@erase "$(INTDIR)\Page2FV.obj"
	-@erase "$(INTDIR)\Page2HP.obj"
	-@erase "$(INTDIR)\Page2HV.obj"
	-@erase "$(INTDIR)\Page2LV.obj"
	-@erase "$(INTDIR)\Page2RS.obj"
	-@erase "$(INTDIR)\Page3am.obj"
	-@erase "$(INTDIR)\Page3AS.obj"
	-@erase "$(INTDIR)\Page3Bo.obj"
	-@erase "$(INTDIR)\Page3FR.obj"
	-@erase "$(INTDIR)\Page3fs.obj"
	-@erase "$(INTDIR)\Page3FV.obj"
	-@erase "$(INTDIR)\Page3FW.obj"
	-@erase "$(INTDIR)\Page3GR.obj"
	-@erase "$(INTDIR)\Page3GW.obj"
	-@erase "$(INTDIR)\Page3HP.obj"
	-@erase "$(INTDIR)\Page3HV.obj"
	-@erase "$(INTDIR)\Page3KR.obj"
	-@erase "$(INTDIR)\Page3KW.obj"
	-@erase "$(INTDIR)\Page3LV.obj"
	-@erase "$(INTDIR)\Page3MF.obj"
	-@erase "$(INTDIR)\Page3MS.obj"
	-@erase "$(INTDIR)\Page3PM.obj"
	-@erase "$(INTDIR)\Page3PR.obj"
	-@erase "$(INTDIR)\Page3RS.obj"
	-@erase "$(INTDIR)\Page3TI.obj"
	-@erase "$(INTDIR)\Page3TR.obj"
	-@erase "$(INTDIR)\PropShtam.obj"
	-@erase "$(INTDIR)\PropShtAS.obj"
	-@erase "$(INTDIR)\PropShtBo.obj"
	-@erase "$(INTDIR)\PropShtFR.obj"
	-@erase "$(INTDIR)\PropShtfs.obj"
	-@erase "$(INTDIR)\PropShtFV.obj"
	-@erase "$(INTDIR)\PropShtFW.obj"
	-@erase "$(INTDIR)\PropShtGR.obj"
	-@erase "$(INTDIR)\PropShtGW.obj"
	-@erase "$(INTDIR)\PropShtHP.obj"
	-@erase "$(INTDIR)\PropShtHV.obj"
	-@erase "$(INTDIR)\PropShtKR.obj"
	-@erase "$(INTDIR)\PropShtKW.obj"
	-@erase "$(INTDIR)\PropShtLV.obj"
	-@erase "$(INTDIR)\PropShtMF.obj"
	-@erase "$(INTDIR)\PropShtMS.obj"
	-@erase "$(INTDIR)\PropShtPM.obj"
	-@erase "$(INTDIR)\PropShtPR.obj"
	-@erase "$(INTDIR)\PropShtRS.obj"
	-@erase "$(INTDIR)\PropShtTI.obj"
	-@erase "$(INTDIR)\PropShtTR.obj"
	-@erase "$(INTDIR)\rpcapi32.obj"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StepTimer.obj"
	-@erase "$(INTDIR)\TimerDelay.obj"
	-@erase "$(INTDIR)\TriggerPartSweep.obj"
	-@erase "$(OUTDIR)\MCP for NT.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MCP for NT.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MCP for NT.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
BSC32_FLAGS=/o"$(OUTDIR)/MCP for NT.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib mpr.lib fitfunc.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=winmm.lib mpr.lib fitfunc.lib /nologo /subsystem:windows\
 /incremental:no /pdb:"$(OUTDIR)/MCP for NT.pdb" /machine:I386\
 /out:"$(OUTDIR)/MCP for NT.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ActionObjects.obj" \
	"$(INTDIR)\Calculator.obj" \
	"$(INTDIR)\camac.obj" \
	"$(INTDIR)\CamacTester.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\ComputerDelay.obj" \
	"$(INTDIR)\Configure.obj" \
	"$(INTDIR)\DataChild.obj" \
	"$(INTDIR)\DataReadout.obj" \
	"$(INTDIR)\DataView.obj" \
	"$(INTDIR)\Display.obj" \
	"$(INTDIR)\EditorChild.obj" \
	"$(INTDIR)\EditorView.obj" \
	"$(INTDIR)\EVALUATE.OBJ" \
	"$(INTDIR)\FastTimer.obj" \
	"$(INTDIR)\FileIO.obj" \
	"$(INTDIR)\GpibReader.obj" \
	"$(INTDIR)\GpibWriter.obj" \
	"$(INTDIR)\Hardware.obj" \
	"$(INTDIR)\HardwareDefaults.obj" \
	"$(INTDIR)\HfsFitDlg.obj" \
	"$(INTDIR)\LSIChild.obj" \
	"$(INTDIR)\LSIView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MCP for NT.obj" \
	"$(INTDIR)\MCP for NT.res" \
	"$(INTDIR)\MCP for NTDoc.obj" \
	"$(INTDIR)\MCP for NTView.obj" \
	"$(INTDIR)\MeasThread.obj" \
	"$(INTDIR)\minuit.obj" \
	"$(INTDIR)\MinuitChild.obj" \
	"$(INTDIR)\MinuitDlg.obj" \
	"$(INTDIR)\MonitorChild.obj" \
	"$(INTDIR)\MonitorView.obj" \
	"$(INTDIR)\NetReader.obj" \
	"$(INTDIR)\NetworkWriter.obj" \
	"$(INTDIR)\Page1am.obj" \
	"$(INTDIR)\Page1AS.obj" \
	"$(INTDIR)\Page1Bo.obj" \
	"$(INTDIR)\Page1FR.obj" \
	"$(INTDIR)\Page1fs.obj" \
	"$(INTDIR)\Page1FV.obj" \
	"$(INTDIR)\Page1FW.obj" \
	"$(INTDIR)\Page1GR.obj" \
	"$(INTDIR)\Page1GW.obj" \
	"$(INTDIR)\Page1HP.obj" \
	"$(INTDIR)\Page1HV.obj" \
	"$(INTDIR)\Page1KR.obj" \
	"$(INTDIR)\Page1KW.obj" \
	"$(INTDIR)\Page1LV.obj" \
	"$(INTDIR)\Page1MF.obj" \
	"$(INTDIR)\Page1MS.obj" \
	"$(INTDIR)\Page1PM.obj" \
	"$(INTDIR)\Page1PR.obj" \
	"$(INTDIR)\Page1RS.obj" \
	"$(INTDIR)\Page1TI.obj" \
	"$(INTDIR)\Page1TR.obj" \
	"$(INTDIR)\Page2FV.obj" \
	"$(INTDIR)\Page2HP.obj" \
	"$(INTDIR)\Page2HV.obj" \
	"$(INTDIR)\Page2LV.obj" \
	"$(INTDIR)\Page2RS.obj" \
	"$(INTDIR)\Page3am.obj" \
	"$(INTDIR)\Page3AS.obj" \
	"$(INTDIR)\Page3Bo.obj" \
	"$(INTDIR)\Page3FR.obj" \
	"$(INTDIR)\Page3fs.obj" \
	"$(INTDIR)\Page3FV.obj" \
	"$(INTDIR)\Page3FW.obj" \
	"$(INTDIR)\Page3GR.obj" \
	"$(INTDIR)\Page3GW.obj" \
	"$(INTDIR)\Page3HP.obj" \
	"$(INTDIR)\Page3HV.obj" \
	"$(INTDIR)\Page3KR.obj" \
	"$(INTDIR)\Page3KW.obj" \
	"$(INTDIR)\Page3LV.obj" \
	"$(INTDIR)\Page3MF.obj" \
	"$(INTDIR)\Page3MS.obj" \
	"$(INTDIR)\Page3PM.obj" \
	"$(INTDIR)\Page3PR.obj" \
	"$(INTDIR)\Page3RS.obj" \
	"$(INTDIR)\Page3TI.obj" \
	"$(INTDIR)\Page3TR.obj" \
	"$(INTDIR)\PropShtam.obj" \
	"$(INTDIR)\PropShtAS.obj" \
	"$(INTDIR)\PropShtBo.obj" \
	"$(INTDIR)\PropShtFR.obj" \
	"$(INTDIR)\PropShtfs.obj" \
	"$(INTDIR)\PropShtFV.obj" \
	"$(INTDIR)\PropShtFW.obj" \
	"$(INTDIR)\PropShtGR.obj" \
	"$(INTDIR)\PropShtGW.obj" \
	"$(INTDIR)\PropShtHP.obj" \
	"$(INTDIR)\PropShtHV.obj" \
	"$(INTDIR)\PropShtKR.obj" \
	"$(INTDIR)\PropShtKW.obj" \
	"$(INTDIR)\PropShtLV.obj" \
	"$(INTDIR)\PropShtMF.obj" \
	"$(INTDIR)\PropShtMS.obj" \
	"$(INTDIR)\PropShtPM.obj" \
	"$(INTDIR)\PropShtPR.obj" \
	"$(INTDIR)\PropShtRS.obj" \
	"$(INTDIR)\PropShtTI.obj" \
	"$(INTDIR)\PropShtTR.obj" \
	"$(INTDIR)\rpcapi32.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StepTimer.obj" \
	"$(INTDIR)\TimerDelay.obj" \
	"$(INTDIR)\TriggerPartSweep.obj"

"$(OUTDIR)\MCP for NT.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\MCP for NT.exe" "$(OUTDIR)\MCP for NT.bsc"\
 "$(OUTDIR)\MCP for NT.hlp"

CLEAN : 
	-@erase "$(INTDIR)\ActionObjects.obj"
	-@erase "$(INTDIR)\ActionObjects.sbr"
	-@erase "$(INTDIR)\Calculator.obj"
	-@erase "$(INTDIR)\Calculator.sbr"
	-@erase "$(INTDIR)\camac.obj"
	-@erase "$(INTDIR)\camac.sbr"
	-@erase "$(INTDIR)\CamacTester.obj"
	-@erase "$(INTDIR)\CamacTester.sbr"
	-@erase "$(INTDIR)\ChildFrm.obj"
	-@erase "$(INTDIR)\ChildFrm.sbr"
	-@erase "$(INTDIR)\ComputerDelay.obj"
	-@erase "$(INTDIR)\ComputerDelay.sbr"
	-@erase "$(INTDIR)\Configure.obj"
	-@erase "$(INTDIR)\Configure.sbr"
	-@erase "$(INTDIR)\DataChild.obj"
	-@erase "$(INTDIR)\DataChild.sbr"
	-@erase "$(INTDIR)\DataReadout.obj"
	-@erase "$(INTDIR)\DataReadout.sbr"
	-@erase "$(INTDIR)\DataView.obj"
	-@erase "$(INTDIR)\DataView.sbr"
	-@erase "$(INTDIR)\Display.obj"
	-@erase "$(INTDIR)\Display.sbr"
	-@erase "$(INTDIR)\EditorChild.obj"
	-@erase "$(INTDIR)\EditorChild.sbr"
	-@erase "$(INTDIR)\EditorView.obj"
	-@erase "$(INTDIR)\EditorView.sbr"
	-@erase "$(INTDIR)\EVALUATE.OBJ"
	-@erase "$(INTDIR)\EVALUATE.SBR"
	-@erase "$(INTDIR)\FastTimer.obj"
	-@erase "$(INTDIR)\FastTimer.sbr"
	-@erase "$(INTDIR)\FileIO.obj"
	-@erase "$(INTDIR)\FileIO.sbr"
	-@erase "$(INTDIR)\GpibReader.obj"
	-@erase "$(INTDIR)\GpibReader.sbr"
	-@erase "$(INTDIR)\GpibWriter.obj"
	-@erase "$(INTDIR)\GpibWriter.sbr"
	-@erase "$(INTDIR)\Hardware.obj"
	-@erase "$(INTDIR)\Hardware.sbr"
	-@erase "$(INTDIR)\HardwareDefaults.obj"
	-@erase "$(INTDIR)\HardwareDefaults.sbr"
	-@erase "$(INTDIR)\HfsFitDlg.obj"
	-@erase "$(INTDIR)\HfsFitDlg.sbr"
	-@erase "$(INTDIR)\LSIChild.obj"
	-@erase "$(INTDIR)\LSIChild.sbr"
	-@erase "$(INTDIR)\LSIView.obj"
	-@erase "$(INTDIR)\LSIView.sbr"
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\MainFrm.sbr"
	-@erase "$(INTDIR)\MCP for NT.hlp"
	-@erase "$(INTDIR)\MCP for NT.obj"
	-@erase "$(INTDIR)\MCP for NT.pch"
	-@erase "$(INTDIR)\MCP for NT.res"
	-@erase "$(INTDIR)\MCP for NT.sbr"
	-@erase "$(INTDIR)\MCP for NTDoc.obj"
	-@erase "$(INTDIR)\MCP for NTDoc.sbr"
	-@erase "$(INTDIR)\MCP for NTView.obj"
	-@erase "$(INTDIR)\MCP for NTView.sbr"
	-@erase "$(INTDIR)\MeasThread.obj"
	-@erase "$(INTDIR)\MeasThread.sbr"
	-@erase "$(INTDIR)\minuit.obj"
	-@erase "$(INTDIR)\minuit.sbr"
	-@erase "$(INTDIR)\MinuitChild.obj"
	-@erase "$(INTDIR)\MinuitChild.sbr"
	-@erase "$(INTDIR)\MinuitDlg.obj"
	-@erase "$(INTDIR)\MinuitDlg.sbr"
	-@erase "$(INTDIR)\MonitorChild.obj"
	-@erase "$(INTDIR)\MonitorChild.sbr"
	-@erase "$(INTDIR)\MonitorView.obj"
	-@erase "$(INTDIR)\MonitorView.sbr"
	-@erase "$(INTDIR)\NetReader.obj"
	-@erase "$(INTDIR)\NetReader.sbr"
	-@erase "$(INTDIR)\NetworkWriter.obj"
	-@erase "$(INTDIR)\NetworkWriter.sbr"
	-@erase "$(INTDIR)\Page1am.obj"
	-@erase "$(INTDIR)\Page1am.sbr"
	-@erase "$(INTDIR)\Page1AS.obj"
	-@erase "$(INTDIR)\Page1AS.sbr"
	-@erase "$(INTDIR)\Page1Bo.obj"
	-@erase "$(INTDIR)\Page1Bo.sbr"
	-@erase "$(INTDIR)\Page1FR.obj"
	-@erase "$(INTDIR)\Page1FR.sbr"
	-@erase "$(INTDIR)\Page1fs.obj"
	-@erase "$(INTDIR)\Page1fs.sbr"
	-@erase "$(INTDIR)\Page1FV.obj"
	-@erase "$(INTDIR)\Page1FV.sbr"
	-@erase "$(INTDIR)\Page1FW.obj"
	-@erase "$(INTDIR)\Page1FW.sbr"
	-@erase "$(INTDIR)\Page1GR.obj"
	-@erase "$(INTDIR)\Page1GR.sbr"
	-@erase "$(INTDIR)\Page1GW.obj"
	-@erase "$(INTDIR)\Page1GW.sbr"
	-@erase "$(INTDIR)\Page1HP.obj"
	-@erase "$(INTDIR)\Page1HP.sbr"
	-@erase "$(INTDIR)\Page1HV.obj"
	-@erase "$(INTDIR)\Page1HV.sbr"
	-@erase "$(INTDIR)\Page1KR.obj"
	-@erase "$(INTDIR)\Page1KR.sbr"
	-@erase "$(INTDIR)\Page1KW.obj"
	-@erase "$(INTDIR)\Page1KW.sbr"
	-@erase "$(INTDIR)\Page1LV.obj"
	-@erase "$(INTDIR)\Page1LV.sbr"
	-@erase "$(INTDIR)\Page1MF.obj"
	-@erase "$(INTDIR)\Page1MF.sbr"
	-@erase "$(INTDIR)\Page1MS.obj"
	-@erase "$(INTDIR)\Page1MS.sbr"
	-@erase "$(INTDIR)\Page1PM.obj"
	-@erase "$(INTDIR)\Page1PM.sbr"
	-@erase "$(INTDIR)\Page1PR.obj"
	-@erase "$(INTDIR)\Page1PR.sbr"
	-@erase "$(INTDIR)\Page1RS.obj"
	-@erase "$(INTDIR)\Page1RS.sbr"
	-@erase "$(INTDIR)\Page1TI.obj"
	-@erase "$(INTDIR)\Page1TI.sbr"
	-@erase "$(INTDIR)\Page1TR.obj"
	-@erase "$(INTDIR)\Page1TR.sbr"
	-@erase "$(INTDIR)\Page2FV.obj"
	-@erase "$(INTDIR)\Page2FV.sbr"
	-@erase "$(INTDIR)\Page2HP.obj"
	-@erase "$(INTDIR)\Page2HP.sbr"
	-@erase "$(INTDIR)\Page2HV.obj"
	-@erase "$(INTDIR)\Page2HV.sbr"
	-@erase "$(INTDIR)\Page2LV.obj"
	-@erase "$(INTDIR)\Page2LV.sbr"
	-@erase "$(INTDIR)\Page2RS.obj"
	-@erase "$(INTDIR)\Page2RS.sbr"
	-@erase "$(INTDIR)\Page3am.obj"
	-@erase "$(INTDIR)\Page3am.sbr"
	-@erase "$(INTDIR)\Page3AS.obj"
	-@erase "$(INTDIR)\Page3AS.sbr"
	-@erase "$(INTDIR)\Page3Bo.obj"
	-@erase "$(INTDIR)\Page3Bo.sbr"
	-@erase "$(INTDIR)\Page3FR.obj"
	-@erase "$(INTDIR)\Page3FR.sbr"
	-@erase "$(INTDIR)\Page3fs.obj"
	-@erase "$(INTDIR)\Page3fs.sbr"
	-@erase "$(INTDIR)\Page3FV.obj"
	-@erase "$(INTDIR)\Page3FV.sbr"
	-@erase "$(INTDIR)\Page3FW.obj"
	-@erase "$(INTDIR)\Page3FW.sbr"
	-@erase "$(INTDIR)\Page3GR.obj"
	-@erase "$(INTDIR)\Page3GR.sbr"
	-@erase "$(INTDIR)\Page3GW.obj"
	-@erase "$(INTDIR)\Page3GW.sbr"
	-@erase "$(INTDIR)\Page3HP.obj"
	-@erase "$(INTDIR)\Page3HP.sbr"
	-@erase "$(INTDIR)\Page3HV.obj"
	-@erase "$(INTDIR)\Page3HV.sbr"
	-@erase "$(INTDIR)\Page3KR.obj"
	-@erase "$(INTDIR)\Page3KR.sbr"
	-@erase "$(INTDIR)\Page3KW.obj"
	-@erase "$(INTDIR)\Page3KW.sbr"
	-@erase "$(INTDIR)\Page3LV.obj"
	-@erase "$(INTDIR)\Page3LV.sbr"
	-@erase "$(INTDIR)\Page3MF.obj"
	-@erase "$(INTDIR)\Page3MF.sbr"
	-@erase "$(INTDIR)\Page3MS.obj"
	-@erase "$(INTDIR)\Page3MS.sbr"
	-@erase "$(INTDIR)\Page3PM.obj"
	-@erase "$(INTDIR)\Page3PM.sbr"
	-@erase "$(INTDIR)\Page3PR.obj"
	-@erase "$(INTDIR)\Page3PR.sbr"
	-@erase "$(INTDIR)\Page3RS.obj"
	-@erase "$(INTDIR)\Page3RS.sbr"
	-@erase "$(INTDIR)\Page3TI.obj"
	-@erase "$(INTDIR)\Page3TI.sbr"
	-@erase "$(INTDIR)\Page3TR.obj"
	-@erase "$(INTDIR)\Page3TR.sbr"
	-@erase "$(INTDIR)\PropShtam.obj"
	-@erase "$(INTDIR)\PropShtam.sbr"
	-@erase "$(INTDIR)\PropShtAS.obj"
	-@erase "$(INTDIR)\PropShtAS.sbr"
	-@erase "$(INTDIR)\PropShtBo.obj"
	-@erase "$(INTDIR)\PropShtBo.sbr"
	-@erase "$(INTDIR)\PropShtFR.obj"
	-@erase "$(INTDIR)\PropShtFR.sbr"
	-@erase "$(INTDIR)\PropShtfs.obj"
	-@erase "$(INTDIR)\PropShtfs.sbr"
	-@erase "$(INTDIR)\PropShtFV.obj"
	-@erase "$(INTDIR)\PropShtFV.sbr"
	-@erase "$(INTDIR)\PropShtFW.obj"
	-@erase "$(INTDIR)\PropShtFW.sbr"
	-@erase "$(INTDIR)\PropShtGR.obj"
	-@erase "$(INTDIR)\PropShtGR.sbr"
	-@erase "$(INTDIR)\PropShtGW.obj"
	-@erase "$(INTDIR)\PropShtGW.sbr"
	-@erase "$(INTDIR)\PropShtHP.obj"
	-@erase "$(INTDIR)\PropShtHP.sbr"
	-@erase "$(INTDIR)\PropShtHV.obj"
	-@erase "$(INTDIR)\PropShtHV.sbr"
	-@erase "$(INTDIR)\PropShtKR.obj"
	-@erase "$(INTDIR)\PropShtKR.sbr"
	-@erase "$(INTDIR)\PropShtKW.obj"
	-@erase "$(INTDIR)\PropShtKW.sbr"
	-@erase "$(INTDIR)\PropShtLV.obj"
	-@erase "$(INTDIR)\PropShtLV.sbr"
	-@erase "$(INTDIR)\PropShtMF.obj"
	-@erase "$(INTDIR)\PropShtMF.sbr"
	-@erase "$(INTDIR)\PropShtMS.obj"
	-@erase "$(INTDIR)\PropShtMS.sbr"
	-@erase "$(INTDIR)\PropShtPM.obj"
	-@erase "$(INTDIR)\PropShtPM.sbr"
	-@erase "$(INTDIR)\PropShtPR.obj"
	-@erase "$(INTDIR)\PropShtPR.sbr"
	-@erase "$(INTDIR)\PropShtRS.obj"
	-@erase "$(INTDIR)\PropShtRS.sbr"
	-@erase "$(INTDIR)\PropShtTI.obj"
	-@erase "$(INTDIR)\PropShtTI.sbr"
	-@erase "$(INTDIR)\PropShtTR.obj"
	-@erase "$(INTDIR)\PropShtTR.sbr"
	-@erase "$(INTDIR)\rpcapi32.obj"
	-@erase "$(INTDIR)\rpcapi32.sbr"
	-@erase "$(INTDIR)\Splash.obj"
	-@erase "$(INTDIR)\Splash.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\StepTimer.obj"
	-@erase "$(INTDIR)\StepTimer.sbr"
	-@erase "$(INTDIR)\TimerDelay.obj"
	-@erase "$(INTDIR)\TimerDelay.sbr"
	-@erase "$(INTDIR)\TriggerPartSweep.obj"
	-@erase "$(INTDIR)\TriggerPartSweep.sbr"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\MCP for NT.bsc"
	-@erase "$(OUTDIR)\MCP for NT.exe"
	-@erase "$(OUTDIR)\MCP for NT.ilk"
	-@erase "$(OUTDIR)\MCP for NT.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /c
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/MCP for NT.pch"\
 /Yu"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/MCP for NT.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
BSC32_FLAGS=/o"$(OUTDIR)/MCP for NT.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ActionObjects.sbr" \
	"$(INTDIR)\Calculator.sbr" \
	"$(INTDIR)\camac.sbr" \
	"$(INTDIR)\CamacTester.sbr" \
	"$(INTDIR)\ChildFrm.sbr" \
	"$(INTDIR)\ComputerDelay.sbr" \
	"$(INTDIR)\Configure.sbr" \
	"$(INTDIR)\DataChild.sbr" \
	"$(INTDIR)\DataReadout.sbr" \
	"$(INTDIR)\DataView.sbr" \
	"$(INTDIR)\Display.sbr" \
	"$(INTDIR)\EditorChild.sbr" \
	"$(INTDIR)\EditorView.sbr" \
	"$(INTDIR)\EVALUATE.SBR" \
	"$(INTDIR)\FastTimer.sbr" \
	"$(INTDIR)\FileIO.sbr" \
	"$(INTDIR)\GpibReader.sbr" \
	"$(INTDIR)\GpibWriter.sbr" \
	"$(INTDIR)\Hardware.sbr" \
	"$(INTDIR)\HardwareDefaults.sbr" \
	"$(INTDIR)\HfsFitDlg.sbr" \
	"$(INTDIR)\LSIChild.sbr" \
	"$(INTDIR)\LSIView.sbr" \
	"$(INTDIR)\MainFrm.sbr" \
	"$(INTDIR)\MCP for NT.sbr" \
	"$(INTDIR)\MCP for NTDoc.sbr" \
	"$(INTDIR)\MCP for NTView.sbr" \
	"$(INTDIR)\MeasThread.sbr" \
	"$(INTDIR)\minuit.sbr" \
	"$(INTDIR)\MinuitChild.sbr" \
	"$(INTDIR)\MinuitDlg.sbr" \
	"$(INTDIR)\MonitorChild.sbr" \
	"$(INTDIR)\MonitorView.sbr" \
	"$(INTDIR)\NetReader.sbr" \
	"$(INTDIR)\NetworkWriter.sbr" \
	"$(INTDIR)\Page1am.sbr" \
	"$(INTDIR)\Page1AS.sbr" \
	"$(INTDIR)\Page1Bo.sbr" \
	"$(INTDIR)\Page1FR.sbr" \
	"$(INTDIR)\Page1fs.sbr" \
	"$(INTDIR)\Page1FV.sbr" \
	"$(INTDIR)\Page1FW.sbr" \
	"$(INTDIR)\Page1GR.sbr" \
	"$(INTDIR)\Page1GW.sbr" \
	"$(INTDIR)\Page1HP.sbr" \
	"$(INTDIR)\Page1HV.sbr" \
	"$(INTDIR)\Page1KR.sbr" \
	"$(INTDIR)\Page1KW.sbr" \
	"$(INTDIR)\Page1LV.sbr" \
	"$(INTDIR)\Page1MF.sbr" \
	"$(INTDIR)\Page1MS.sbr" \
	"$(INTDIR)\Page1PM.sbr" \
	"$(INTDIR)\Page1PR.sbr" \
	"$(INTDIR)\Page1RS.sbr" \
	"$(INTDIR)\Page1TI.sbr" \
	"$(INTDIR)\Page1TR.sbr" \
	"$(INTDIR)\Page2FV.sbr" \
	"$(INTDIR)\Page2HP.sbr" \
	"$(INTDIR)\Page2HV.sbr" \
	"$(INTDIR)\Page2LV.sbr" \
	"$(INTDIR)\Page2RS.sbr" \
	"$(INTDIR)\Page3am.sbr" \
	"$(INTDIR)\Page3AS.sbr" \
	"$(INTDIR)\Page3Bo.sbr" \
	"$(INTDIR)\Page3FR.sbr" \
	"$(INTDIR)\Page3fs.sbr" \
	"$(INTDIR)\Page3FV.sbr" \
	"$(INTDIR)\Page3FW.sbr" \
	"$(INTDIR)\Page3GR.sbr" \
	"$(INTDIR)\Page3GW.sbr" \
	"$(INTDIR)\Page3HP.sbr" \
	"$(INTDIR)\Page3HV.sbr" \
	"$(INTDIR)\Page3KR.sbr" \
	"$(INTDIR)\Page3KW.sbr" \
	"$(INTDIR)\Page3LV.sbr" \
	"$(INTDIR)\Page3MF.sbr" \
	"$(INTDIR)\Page3MS.sbr" \
	"$(INTDIR)\Page3PM.sbr" \
	"$(INTDIR)\Page3PR.sbr" \
	"$(INTDIR)\Page3RS.sbr" \
	"$(INTDIR)\Page3TI.sbr" \
	"$(INTDIR)\Page3TR.sbr" \
	"$(INTDIR)\PropShtam.sbr" \
	"$(INTDIR)\PropShtAS.sbr" \
	"$(INTDIR)\PropShtBo.sbr" \
	"$(INTDIR)\PropShtFR.sbr" \
	"$(INTDIR)\PropShtfs.sbr" \
	"$(INTDIR)\PropShtFV.sbr" \
	"$(INTDIR)\PropShtFW.sbr" \
	"$(INTDIR)\PropShtGR.sbr" \
	"$(INTDIR)\PropShtGW.sbr" \
	"$(INTDIR)\PropShtHP.sbr" \
	"$(INTDIR)\PropShtHV.sbr" \
	"$(INTDIR)\PropShtKR.sbr" \
	"$(INTDIR)\PropShtKW.sbr" \
	"$(INTDIR)\PropShtLV.sbr" \
	"$(INTDIR)\PropShtMF.sbr" \
	"$(INTDIR)\PropShtMS.sbr" \
	"$(INTDIR)\PropShtPM.sbr" \
	"$(INTDIR)\PropShtPR.sbr" \
	"$(INTDIR)\PropShtRS.sbr" \
	"$(INTDIR)\PropShtTI.sbr" \
	"$(INTDIR)\PropShtTR.sbr" \
	"$(INTDIR)\rpcapi32.sbr" \
	"$(INTDIR)\Splash.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\StepTimer.sbr" \
	"$(INTDIR)\TimerDelay.sbr" \
	"$(INTDIR)\TriggerPartSweep.sbr"

"$(OUTDIR)\MCP for NT.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 winmm.lib mpr.lib fitfunc.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /nodefaultlib
LINK32_FLAGS=winmm.lib mpr.lib fitfunc.lib /nologo /subsystem:windows\
 /incremental:yes /pdb:"$(OUTDIR)/MCP for NT.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/MCP for NT.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ActionObjects.obj" \
	"$(INTDIR)\Calculator.obj" \
	"$(INTDIR)\camac.obj" \
	"$(INTDIR)\CamacTester.obj" \
	"$(INTDIR)\ChildFrm.obj" \
	"$(INTDIR)\ComputerDelay.obj" \
	"$(INTDIR)\Configure.obj" \
	"$(INTDIR)\DataChild.obj" \
	"$(INTDIR)\DataReadout.obj" \
	"$(INTDIR)\DataView.obj" \
	"$(INTDIR)\Display.obj" \
	"$(INTDIR)\EditorChild.obj" \
	"$(INTDIR)\EditorView.obj" \
	"$(INTDIR)\EVALUATE.OBJ" \
	"$(INTDIR)\FastTimer.obj" \
	"$(INTDIR)\FileIO.obj" \
	"$(INTDIR)\GpibReader.obj" \
	"$(INTDIR)\GpibWriter.obj" \
	"$(INTDIR)\Hardware.obj" \
	"$(INTDIR)\HardwareDefaults.obj" \
	"$(INTDIR)\HfsFitDlg.obj" \
	"$(INTDIR)\LSIChild.obj" \
	"$(INTDIR)\LSIView.obj" \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\MCP for NT.obj" \
	"$(INTDIR)\MCP for NT.res" \
	"$(INTDIR)\MCP for NTDoc.obj" \
	"$(INTDIR)\MCP for NTView.obj" \
	"$(INTDIR)\MeasThread.obj" \
	"$(INTDIR)\minuit.obj" \
	"$(INTDIR)\MinuitChild.obj" \
	"$(INTDIR)\MinuitDlg.obj" \
	"$(INTDIR)\MonitorChild.obj" \
	"$(INTDIR)\MonitorView.obj" \
	"$(INTDIR)\NetReader.obj" \
	"$(INTDIR)\NetworkWriter.obj" \
	"$(INTDIR)\Page1am.obj" \
	"$(INTDIR)\Page1AS.obj" \
	"$(INTDIR)\Page1Bo.obj" \
	"$(INTDIR)\Page1FR.obj" \
	"$(INTDIR)\Page1fs.obj" \
	"$(INTDIR)\Page1FV.obj" \
	"$(INTDIR)\Page1FW.obj" \
	"$(INTDIR)\Page1GR.obj" \
	"$(INTDIR)\Page1GW.obj" \
	"$(INTDIR)\Page1HP.obj" \
	"$(INTDIR)\Page1HV.obj" \
	"$(INTDIR)\Page1KR.obj" \
	"$(INTDIR)\Page1KW.obj" \
	"$(INTDIR)\Page1LV.obj" \
	"$(INTDIR)\Page1MF.obj" \
	"$(INTDIR)\Page1MS.obj" \
	"$(INTDIR)\Page1PM.obj" \
	"$(INTDIR)\Page1PR.obj" \
	"$(INTDIR)\Page1RS.obj" \
	"$(INTDIR)\Page1TI.obj" \
	"$(INTDIR)\Page1TR.obj" \
	"$(INTDIR)\Page2FV.obj" \
	"$(INTDIR)\Page2HP.obj" \
	"$(INTDIR)\Page2HV.obj" \
	"$(INTDIR)\Page2LV.obj" \
	"$(INTDIR)\Page2RS.obj" \
	"$(INTDIR)\Page3am.obj" \
	"$(INTDIR)\Page3AS.obj" \
	"$(INTDIR)\Page3Bo.obj" \
	"$(INTDIR)\Page3FR.obj" \
	"$(INTDIR)\Page3fs.obj" \
	"$(INTDIR)\Page3FV.obj" \
	"$(INTDIR)\Page3FW.obj" \
	"$(INTDIR)\Page3GR.obj" \
	"$(INTDIR)\Page3GW.obj" \
	"$(INTDIR)\Page3HP.obj" \
	"$(INTDIR)\Page3HV.obj" \
	"$(INTDIR)\Page3KR.obj" \
	"$(INTDIR)\Page3KW.obj" \
	"$(INTDIR)\Page3LV.obj" \
	"$(INTDIR)\Page3MF.obj" \
	"$(INTDIR)\Page3MS.obj" \
	"$(INTDIR)\Page3PM.obj" \
	"$(INTDIR)\Page3PR.obj" \
	"$(INTDIR)\Page3RS.obj" \
	"$(INTDIR)\Page3TI.obj" \
	"$(INTDIR)\Page3TR.obj" \
	"$(INTDIR)\PropShtam.obj" \
	"$(INTDIR)\PropShtAS.obj" \
	"$(INTDIR)\PropShtBo.obj" \
	"$(INTDIR)\PropShtFR.obj" \
	"$(INTDIR)\PropShtfs.obj" \
	"$(INTDIR)\PropShtFV.obj" \
	"$(INTDIR)\PropShtFW.obj" \
	"$(INTDIR)\PropShtGR.obj" \
	"$(INTDIR)\PropShtGW.obj" \
	"$(INTDIR)\PropShtHP.obj" \
	"$(INTDIR)\PropShtHV.obj" \
	"$(INTDIR)\PropShtKR.obj" \
	"$(INTDIR)\PropShtKW.obj" \
	"$(INTDIR)\PropShtLV.obj" \
	"$(INTDIR)\PropShtMF.obj" \
	"$(INTDIR)\PropShtMS.obj" \
	"$(INTDIR)\PropShtPM.obj" \
	"$(INTDIR)\PropShtPR.obj" \
	"$(INTDIR)\PropShtRS.obj" \
	"$(INTDIR)\PropShtTI.obj" \
	"$(INTDIR)\PropShtTR.obj" \
	"$(INTDIR)\rpcapi32.obj" \
	"$(INTDIR)\Splash.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\StepTimer.obj" \
	"$(INTDIR)\TimerDelay.obj" \
	"$(INTDIR)\TriggerPartSweep.obj"

"$(OUTDIR)\MCP for NT.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "MCP for NT - Win32 Release"
# Name "MCP for NT - Win32 Debug"

!IF  "$(CFG)" == "MCP for NT - Win32 Release"

!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=".\MCP for NT.cpp"
DEP_CPP_MCP_F=\
	".\ActionObjects.h"\
	".\camac.h"\
	".\ChildFrm.h"\
	".\DataChild.h"\
	".\DataView.h"\
	".\Hardware.h"\
	".\HardwareConfig.h"\
	".\HardwareDefaults.h"\
	".\LSIChild.h"\
	".\LSIView.h"\
	".\MainFrm.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\MCP for NTView.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\Splash.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\MCP for NT.obj" : $(SOURCE) $(DEP_CPP_MCP_F) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\MCP for NT.obj" : $(SOURCE) $(DEP_CPP_MCP_F) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\MCP for NT.sbr" : $(SOURCE) $(DEP_CPP_MCP_F) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)/MCP for NT.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MCP for NT.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"

# ADD CPP /Yc"stdafx.h"

BuildCmds= \
	$(CPP) /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)/" /Fp"$(INTDIR)/MCP for NT.pch"\
 /Yc"stdafx.h" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c $(SOURCE) \
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\StdAfx.sbr" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

"$(INTDIR)\MCP for NT.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
   $(BuildCmds)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp
DEP_CPP_MAINF=\
	".\ActionObjects.h"\
	".\Calculator.h"\
	".\camac.h"\
	".\CamacTester.h"\
	".\DataChild.h"\
	".\DataView.h"\
	".\display.h"\
	".\EditorChild.h"\
	".\EditorView.h"\
	".\GpibReader.h"\
	".\GpibWriter.h"\
	".\Hardware.h"\
	".\HardwareConfig.h"\
	".\MainFrm.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\NetReader.h"\
	".\NetworkWriter.h"\
	".\rpcapi32.h"\
	".\Splash.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\MainFrm.sbr" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ChildFrm.cpp
DEP_CPP_CHILD=\
	".\ActionObjects.h"\
	".\camac.h"\
	".\ChildFrm.h"\
	".\DataChild.h"\
	".\DataView.h"\
	".\HardwareConfig.h"\
	".\LSIChild.h"\
	".\LSIView.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\MCP for NTView.h"\
	".\measthread.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\Page1am.h"\
	".\Page1fs.h"\
	".\Page1HP.h"\
	".\Page1HV.h"\
	".\Page1RS.h"\
	".\Page2HP.h"\
	".\Page2HV.h"\
	".\Page2RS.h"\
	".\Page3am.h"\
	".\Page3fs.h"\
	".\Page3HP.h"\
	".\Page3HV.h"\
	".\Page3RS.h"\
	".\PropShtam.h"\
	".\PropShtfs.h"\
	".\PropShtHP.h"\
	".\PropShtHV.h"\
	".\PropShtRS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\ChildFrm.obj" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\ChildFrm.sbr" : $(SOURCE) $(DEP_CPP_CHILD) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=".\MCP for NTDoc.cpp"
DEP_CPP_MCP_FO=\
	".\ActionObjects.h"\
	".\FileIO.h"\
	".\HardwareConfig.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\MCP for NTDoc.obj" : $(SOURCE) $(DEP_CPP_MCP_FO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\MCP for NTDoc.obj" : $(SOURCE) $(DEP_CPP_MCP_FO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\MCP for NTDoc.sbr" : $(SOURCE) $(DEP_CPP_MCP_FO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=".\MCP for NTView.cpp"
DEP_CPP_MCP_FOR=\
	".\ActionObjects.h"\
	".\camac.h"\
	".\ChildFrm.h"\
	".\DataChild.h"\
	".\DataView.h"\
	".\Hardware.h"\
	".\HardwareConfig.h"\
	".\LSIChild.h"\
	".\LSIView.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\MCP for NTView.h"\
	".\measthread.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\Page1am.h"\
	".\Page1fs.h"\
	".\Page1HP.h"\
	".\Page1HV.h"\
	".\Page1RS.h"\
	".\Page2HP.h"\
	".\Page2HV.h"\
	".\Page2RS.h"\
	".\Page3am.h"\
	".\Page3fs.h"\
	".\Page3HP.h"\
	".\Page3HV.h"\
	".\Page3RS.h"\
	".\PropShtam.h"\
	".\PropShtfs.h"\
	".\PropShtHP.h"\
	".\PropShtHV.h"\
	".\PropShtRS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\MCP for NTView.obj" : $(SOURCE) $(DEP_CPP_MCP_FOR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\MCP for NTView.obj" : $(SOURCE) $(DEP_CPP_MCP_FOR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\MCP for NTView.sbr" : $(SOURCE) $(DEP_CPP_MCP_FOR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=".\MCP for NT.rc"
DEP_RSC_MCP_FOR_=\
	".\pillars.bmp"\
	".\res\135.ico"\
	".\res\APPS INF.ICO"\
	".\res\APPS WAR.ICO"\
	".\res\Galaxy3.ico"\
	".\res\ico00001.ico"\
	".\res\icon14.ico"\
	".\res\icon15.ico"\
	".\res\MCP for NT.rc2"\
	".\res\MCP for NTDoc.ico"\
	".\RES\origin.ico"\
	".\RES\origin2.ico"\
	".\res\PEAL BAC.ICO"\
	".\res\Toolbar.bmp"\
	".\res\toolbar1.bmp"\
	

"$(INTDIR)\MCP for NT.res" : $(SOURCE) $(DEP_RSC_MCP_FOR_) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=".\hlp\MCP for NT.hpj"

!IF  "$(CFG)" == "MCP for NT - Win32 Release"

# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=MCP for NT
InputPath=.\hlp\MCP for NT.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"

# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=MCP for NT
InputPath=.\hlp\MCP for NT.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
   "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Splash.cpp
DEP_CPP_SPLAS=\
	".\Splash.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Splash.obj" : $(SOURCE) $(DEP_CPP_SPLAS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Splash.obj" : $(SOURCE) $(DEP_CPP_SPLAS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Splash.sbr" : $(SOURCE) $(DEP_CPP_SPLAS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DataChild.cpp
DEP_CPP_DATAC=\
	".\ActionObjects.h"\
	".\DataChild.h"\
	".\DataView.h"\
	".\HardwareConfig.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\DataChild.obj" : $(SOURCE) $(DEP_CPP_DATAC) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\DataChild.obj" : $(SOURCE) $(DEP_CPP_DATAC) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\DataChild.sbr" : $(SOURCE) $(DEP_CPP_DATAC) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DataView.cpp

!IF  "$(CFG)" == "MCP for NT - Win32 Release"

DEP_CPP_DATAV=\
	".\ActionObjects.h"\
	".\Configure.h"\
	".\DataChild.h"\
	".\DataReadout.h"\
	".\DataView.h"\
	".\eval.h"\
	".\HardwareConfig.h"\
	".\HfsFitDlg.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\minuitdlg.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\Page1AS.h"\
	".\Page1PM.h"\
	".\Page3AS.h"\
	".\Page3PM.h"\
	".\PropShtAS.h"\
	".\PropShtPM.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

"$(INTDIR)\DataView.obj" : $(SOURCE) $(DEP_CPP_DATAV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"

DEP_CPP_DATAV=\
	".\ActionObjects.h"\
	".\Configure.h"\
	".\DataChild.h"\
	".\DataReadout.h"\
	".\DataView.h"\
	".\eval.h"\
	".\HardwareConfig.h"\
	".\HfsFitDlg.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\minuitdlg.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\Page1AS.h"\
	".\Page1PM.h"\
	".\Page3AS.h"\
	".\Page3PM.h"\
	".\PropShtAS.h"\
	".\PropShtPM.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

"$(INTDIR)\DataView.obj" : $(SOURCE) $(DEP_CPP_DATAV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\DataView.sbr" : $(SOURCE) $(DEP_CPP_DATAV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1RS.cpp
DEP_CPP_PAGE1=\
	".\MCP for NT.h"\
	".\Page1RS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1RS.obj" : $(SOURCE) $(DEP_CPP_PAGE1) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1RS.obj" : $(SOURCE) $(DEP_CPP_PAGE1) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1RS.sbr" : $(SOURCE) $(DEP_CPP_PAGE1) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtRS.cpp
DEP_CPP_PROPS=\
	".\MCP for NT.h"\
	".\Page1RS.h"\
	".\Page2RS.h"\
	".\Page3RS.h"\
	".\PropShtRS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtRS.obj" : $(SOURCE) $(DEP_CPP_PROPS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtRS.obj" : $(SOURCE) $(DEP_CPP_PROPS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtRS.sbr" : $(SOURCE) $(DEP_CPP_PROPS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3RS.cpp
DEP_CPP_PAGE3=\
	".\MCP for NT.h"\
	".\Page3RS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3RS.obj" : $(SOURCE) $(DEP_CPP_PAGE3) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3RS.obj" : $(SOURCE) $(DEP_CPP_PAGE3) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3RS.sbr" : $(SOURCE) $(DEP_CPP_PAGE3) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1HV.cpp
DEP_CPP_PAGE1H=\
	".\MCP for NT.h"\
	".\Page1HV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1HV.obj" : $(SOURCE) $(DEP_CPP_PAGE1H) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1HV.obj" : $(SOURCE) $(DEP_CPP_PAGE1H) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1HV.sbr" : $(SOURCE) $(DEP_CPP_PAGE1H) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3HV.cpp
DEP_CPP_PAGE3H=\
	".\Hardware.h"\
	".\MCP for NT.h"\
	".\Page3HV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3HV.obj" : $(SOURCE) $(DEP_CPP_PAGE3H) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3HV.obj" : $(SOURCE) $(DEP_CPP_PAGE3H) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3HV.sbr" : $(SOURCE) $(DEP_CPP_PAGE3H) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1HP.cpp
DEP_CPP_PAGE1HP=\
	".\MCP for NT.h"\
	".\Page1HP.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1HP.obj" : $(SOURCE) $(DEP_CPP_PAGE1HP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1HP.obj" : $(SOURCE) $(DEP_CPP_PAGE1HP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1HP.sbr" : $(SOURCE) $(DEP_CPP_PAGE1HP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3HP.cpp
DEP_CPP_PAGE3HP=\
	".\MCP for NT.h"\
	".\Page3HP.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3HP.obj" : $(SOURCE) $(DEP_CPP_PAGE3HP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3HP.obj" : $(SOURCE) $(DEP_CPP_PAGE3HP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3HP.sbr" : $(SOURCE) $(DEP_CPP_PAGE3HP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtHV.cpp
DEP_CPP_PROPSH=\
	".\MCP for NT.h"\
	".\Page1HV.h"\
	".\Page2HV.h"\
	".\Page3HV.h"\
	".\PropShtHV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtHV.obj" : $(SOURCE) $(DEP_CPP_PROPSH) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtHV.obj" : $(SOURCE) $(DEP_CPP_PROPSH) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtHV.sbr" : $(SOURCE) $(DEP_CPP_PROPSH) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtHP.cpp
DEP_CPP_PROPSHT=\
	".\MCP for NT.h"\
	".\Page1HP.h"\
	".\Page2HP.h"\
	".\Page3HP.h"\
	".\PropShtHP.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtHP.obj" : $(SOURCE) $(DEP_CPP_PROPSHT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtHP.obj" : $(SOURCE) $(DEP_CPP_PROPSHT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtHP.sbr" : $(SOURCE) $(DEP_CPP_PROPSHT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1am.cpp
DEP_CPP_PAGE1A=\
	".\MCP for NT.h"\
	".\Page1am.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1am.obj" : $(SOURCE) $(DEP_CPP_PAGE1A) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1am.obj" : $(SOURCE) $(DEP_CPP_PAGE1A) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1am.sbr" : $(SOURCE) $(DEP_CPP_PAGE1A) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3am.cpp
DEP_CPP_PAGE3A=\
	".\camac.h"\
	".\MCP for NT.h"\
	".\Page3am.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3am.obj" : $(SOURCE) $(DEP_CPP_PAGE3A) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3am.obj" : $(SOURCE) $(DEP_CPP_PAGE3A) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3am.sbr" : $(SOURCE) $(DEP_CPP_PAGE3A) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1fs.cpp
DEP_CPP_PAGE1F=\
	".\MCP for NT.h"\
	".\Page1fs.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1fs.obj" : $(SOURCE) $(DEP_CPP_PAGE1F) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1fs.obj" : $(SOURCE) $(DEP_CPP_PAGE1F) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1fs.sbr" : $(SOURCE) $(DEP_CPP_PAGE1F) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3fs.cpp
DEP_CPP_PAGE3F=\
	".\MCP for NT.h"\
	".\Page3fs.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3fs.obj" : $(SOURCE) $(DEP_CPP_PAGE3F) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3fs.obj" : $(SOURCE) $(DEP_CPP_PAGE3F) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3fs.sbr" : $(SOURCE) $(DEP_CPP_PAGE3F) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtfs.cpp
DEP_CPP_PROPSHTF=\
	".\MCP for NT.h"\
	".\Page1fs.h"\
	".\Page3fs.h"\
	".\PropShtfs.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtfs.obj" : $(SOURCE) $(DEP_CPP_PROPSHTF) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtfs.obj" : $(SOURCE) $(DEP_CPP_PROPSHTF) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtfs.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTF) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtam.cpp
DEP_CPP_PROPSHTA=\
	".\MCP for NT.h"\
	".\Page1am.h"\
	".\Page3am.h"\
	".\PropShtam.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtam.obj" : $(SOURCE) $(DEP_CPP_PROPSHTA) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtam.obj" : $(SOURCE) $(DEP_CPP_PROPSHTA) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtam.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTA) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSIChild.cpp
DEP_CPP_LSICH=\
	".\LSIChild.h"\
	".\LSIView.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\LSIChild.obj" : $(SOURCE) $(DEP_CPP_LSICH) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\LSIChild.obj" : $(SOURCE) $(DEP_CPP_LSICH) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\LSIChild.sbr" : $(SOURCE) $(DEP_CPP_LSICH) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\LSIView.cpp
DEP_CPP_LSIVI=\
	".\ActionObjects.h"\
	".\camac.h"\
	".\DataChild.h"\
	".\DataView.h"\
	".\Hardware.h"\
	".\HardwareConfig.h"\
	".\LSIChild.h"\
	".\LSIView.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\MCP for NTView.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\rpcapi32.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\LSIView.obj" : $(SOURCE) $(DEP_CPP_LSIVI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\LSIView.obj" : $(SOURCE) $(DEP_CPP_LSIVI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\LSIView.sbr" : $(SOURCE) $(DEP_CPP_LSIVI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FileIO.cpp
DEP_CPP_FILEI=\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\FileIO.obj" : $(SOURCE) $(DEP_CPP_FILEI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\FileIO.obj" : $(SOURCE) $(DEP_CPP_FILEI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\FileIO.sbr" : $(SOURCE) $(DEP_CPP_FILEI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ActionObjects.cpp
DEP_CPP_ACTIO=\
	".\ActionObjects.h"\
	".\camac.h"\
	".\ComputerDelay.h"\
	".\Configure.h"\
	".\DataChild.h"\
	".\DataView.h"\
	".\eval.h"\
	".\FastTimer.h"\
	".\FileIO.h"\
	".\Hardware.h"\
	".\HardwareConfig.h"\
	".\LSIChild.h"\
	".\LSIView.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\MCP for NTView.h"\
	".\measthread.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\Page1am.h"\
	".\Page1AS.h"\
	".\Page1Bo.h"\
	".\Page1fs.h"\
	".\Page1FV.h"\
	".\Page1FW.h"\
	".\Page1GR.h"\
	".\Page1GW.h"\
	".\Page1HP.h"\
	".\Page1HV.h"\
	".\Page1KR.h"\
	".\Page1KW.h"\
	".\Page1LV.h"\
	".\Page1MF.h"\
	".\Page1MS.h"\
	".\Page1PM.h"\
	".\Page1PR.h"\
	".\Page1RS.h"\
	".\page1ti.h"\
	".\Page1TR.h"\
	".\Page2FV.h"\
	".\Page2HP.h"\
	".\Page2HV.h"\
	".\Page2LV.h"\
	".\Page2RS.h"\
	".\Page3am.h"\
	".\Page3AS.h"\
	".\Page3Bo.h"\
	".\Page3FR.h"\
	".\Page3fs.h"\
	".\Page3FV.h"\
	".\Page3FW.h"\
	".\Page3GR.h"\
	".\Page3GW.h"\
	".\Page3HP.h"\
	".\Page3HV.h"\
	".\Page3KR.h"\
	".\Page3KW.h"\
	".\Page3LV.h"\
	".\Page3MF.h"\
	".\Page3MS.h"\
	".\Page3PM.h"\
	".\Page3PR.h"\
	".\Page3RS.h"\
	".\Page3TI.h"\
	".\Page3TR.h"\
	".\PropShtam.h"\
	".\PropShtAS.h"\
	".\PropShtBo.h"\
	".\PropShtFR.h"\
	".\PropShtfs.h"\
	".\PropShtFV.h"\
	".\PropShtFW.h"\
	".\PropShtGR.h"\
	".\PropShtGW.h"\
	".\PropShtHP.h"\
	".\PropShtHV.h"\
	".\PropShtKR.h"\
	".\PropShtKW.h"\
	".\PropShtLV.h"\
	".\PropShtMF.h"\
	".\PropShtMS.h"\
	".\PropShtPM.h"\
	".\PropShtPR.h"\
	".\PropShtRS.h"\
	".\PropShtTI.h"\
	".\PropShtTR.h"\
	".\rpcapi32.h"\
	".\StdAfx.h"\
	".\StepTimer.h"\
	".\TheDefines.h"\
	".\TimerDelay.h"\
	".\TriggerPartSweep.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\ActionObjects.obj" : $(SOURCE) $(DEP_CPP_ACTIO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\ActionObjects.obj" : $(SOURCE) $(DEP_CPP_ACTIO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\ActionObjects.sbr" : $(SOURCE) $(DEP_CPP_ACTIO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ComputerDelay.cpp
DEP_CPP_COMPU=\
	".\ComputerDelay.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\ComputerDelay.obj" : $(SOURCE) $(DEP_CPP_COMPU) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\ComputerDelay.obj" : $(SOURCE) $(DEP_CPP_COMPU) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\ComputerDelay.sbr" : $(SOURCE) $(DEP_CPP_COMPU) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TimerDelay.cpp
DEP_CPP_TIMER=\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	".\TimerDelay.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\TimerDelay.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\TimerDelay.obj" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\TimerDelay.sbr" : $(SOURCE) $(DEP_CPP_TIMER) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StepTimer.cpp
DEP_CPP_STEPT=\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\StepTimer.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\StepTimer.obj" : $(SOURCE) $(DEP_CPP_STEPT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\StepTimer.obj" : $(SOURCE) $(DEP_CPP_STEPT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\StepTimer.sbr" : $(SOURCE) $(DEP_CPP_STEPT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\FastTimer.cpp
DEP_CPP_FASTT=\
	".\FastTimer.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\FastTimer.obj" : $(SOURCE) $(DEP_CPP_FASTT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\FastTimer.obj" : $(SOURCE) $(DEP_CPP_FASTT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\FastTimer.sbr" : $(SOURCE) $(DEP_CPP_FASTT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtMF.cpp
DEP_CPP_PROPSHTM=\
	".\MCP for NT.h"\
	".\Page1MF.h"\
	".\Page3MF.h"\
	".\PropShtMF.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtMF.obj" : $(SOURCE) $(DEP_CPP_PROPSHTM) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtMF.obj" : $(SOURCE) $(DEP_CPP_PROPSHTM) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtMF.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTM) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtPM.cpp
DEP_CPP_PROPSHTP=\
	".\MCP for NT.h"\
	".\Page1PM.h"\
	".\Page3PM.h"\
	".\PropShtPM.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtPM.obj" : $(SOURCE) $(DEP_CPP_PROPSHTP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtPM.obj" : $(SOURCE) $(DEP_CPP_PROPSHTP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtPM.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\TriggerPartSweep.cpp
DEP_CPP_TRIGG=\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	".\TriggerPartSweep.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\TriggerPartSweep.obj" : $(SOURCE) $(DEP_CPP_TRIGG) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\TriggerPartSweep.obj" : $(SOURCE) $(DEP_CPP_TRIGG) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\TriggerPartSweep.sbr" : $(SOURCE) $(DEP_CPP_TRIGG) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1MF.cpp
DEP_CPP_PAGE1M=\
	".\MCP for NT.h"\
	".\Page1MF.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1MF.obj" : $(SOURCE) $(DEP_CPP_PAGE1M) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1MF.obj" : $(SOURCE) $(DEP_CPP_PAGE1M) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1MF.sbr" : $(SOURCE) $(DEP_CPP_PAGE1M) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3MF.cpp
DEP_CPP_PAGE3M=\
	".\MCP for NT.h"\
	".\Page3MF.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3MF.obj" : $(SOURCE) $(DEP_CPP_PAGE3M) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3MF.obj" : $(SOURCE) $(DEP_CPP_PAGE3M) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3MF.sbr" : $(SOURCE) $(DEP_CPP_PAGE3M) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1PM.cpp
DEP_CPP_PAGE1P=\
	".\MCP for NT.h"\
	".\Page1PM.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1PM.obj" : $(SOURCE) $(DEP_CPP_PAGE1P) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1PM.obj" : $(SOURCE) $(DEP_CPP_PAGE1P) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1PM.sbr" : $(SOURCE) $(DEP_CPP_PAGE1P) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3PM.cpp
DEP_CPP_PAGE3P=\
	".\MCP for NT.h"\
	".\Page3PM.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3PM.obj" : $(SOURCE) $(DEP_CPP_PAGE3P) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3PM.obj" : $(SOURCE) $(DEP_CPP_PAGE3P) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3PM.sbr" : $(SOURCE) $(DEP_CPP_PAGE3P) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3TR.cpp
DEP_CPP_PAGE3T=\
	".\MCP for NT.h"\
	".\Page3TR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3TR.obj" : $(SOURCE) $(DEP_CPP_PAGE3T) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3TR.obj" : $(SOURCE) $(DEP_CPP_PAGE3T) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3TR.sbr" : $(SOURCE) $(DEP_CPP_PAGE3T) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1TR.cpp
DEP_CPP_PAGE1T=\
	".\MCP for NT.h"\
	".\Page1TR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1TR.obj" : $(SOURCE) $(DEP_CPP_PAGE1T) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1TR.obj" : $(SOURCE) $(DEP_CPP_PAGE1T) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1TR.sbr" : $(SOURCE) $(DEP_CPP_PAGE1T) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtTR.cpp
DEP_CPP_PROPSHTT=\
	".\MCP for NT.h"\
	".\Page1TR.h"\
	".\Page3TR.h"\
	".\PropShtTR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtTR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtTR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtTR.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\minuit.cpp
DEP_CPP_MINUI=\
	".\ActionObjects.h"\
	".\ChildFrm.h"\
	".\DataChild.h"\
	".\DataView.h"\
	".\HardwareConfig.h"\
	".\LSIChild.h"\
	".\LSIView.h"\
	".\MainFrm.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\MCP for NTView.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	".\User.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\minuit.obj" : $(SOURCE) $(DEP_CPP_MINUI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\minuit.obj" : $(SOURCE) $(DEP_CPP_MINUI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\minuit.sbr" : $(SOURCE) $(DEP_CPP_MINUI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Calculator.cpp
DEP_CPP_CALCU=\
	".\ActionObjects.h"\
	".\Calculator.h"\
	".\HardwareConfig.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Calculator.obj" : $(SOURCE) $(DEP_CPP_CALCU) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Calculator.obj" : $(SOURCE) $(DEP_CPP_CALCU) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Calculator.sbr" : $(SOURCE) $(DEP_CPP_CALCU) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3AS.cpp
DEP_CPP_PAGE3AS=\
	".\MCP for NT.h"\
	".\Page3AS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3AS.obj" : $(SOURCE) $(DEP_CPP_PAGE3AS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3AS.obj" : $(SOURCE) $(DEP_CPP_PAGE3AS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3AS.sbr" : $(SOURCE) $(DEP_CPP_PAGE3AS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtAS.cpp
DEP_CPP_PROPSHTAS=\
	".\MCP for NT.h"\
	".\Page1AS.h"\
	".\Page3AS.h"\
	".\PropShtAS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtAS.obj" : $(SOURCE) $(DEP_CPP_PROPSHTAS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtAS.obj" : $(SOURCE) $(DEP_CPP_PROPSHTAS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtAS.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTAS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1AS.cpp
DEP_CPP_PAGE1AS=\
	".\MCP for NT.h"\
	".\Page1AS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1AS.obj" : $(SOURCE) $(DEP_CPP_PAGE1AS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1AS.obj" : $(SOURCE) $(DEP_CPP_PAGE1AS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1AS.sbr" : $(SOURCE) $(DEP_CPP_PAGE1AS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MinuitChild.cpp
DEP_CPP_MINUIT=\
	".\ActionObjects.h"\
	".\HardwareConfig.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\MinuitChild.obj" : $(SOURCE) $(DEP_CPP_MINUIT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\MinuitChild.obj" : $(SOURCE) $(DEP_CPP_MINUIT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\MinuitChild.sbr" : $(SOURCE) $(DEP_CPP_MINUIT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MinuitDlg.cpp
DEP_CPP_MINUITD=\
	".\MCP for NT.h"\
	".\minuitdlg.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	".\User.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\MinuitDlg.obj" : $(SOURCE) $(DEP_CPP_MINUITD) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\MinuitDlg.obj" : $(SOURCE) $(DEP_CPP_MINUITD) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\MinuitDlg.sbr" : $(SOURCE) $(DEP_CPP_MINUITD) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MeasThread.cpp
DEP_CPP_MEAST=\
	".\MCP for NT.h"\
	".\measthread.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\MeasThread.obj" : $(SOURCE) $(DEP_CPP_MEAST) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\MeasThread.obj" : $(SOURCE) $(DEP_CPP_MEAST) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\MeasThread.sbr" : $(SOURCE) $(DEP_CPP_MEAST) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\DataReadout.cpp
DEP_CPP_DATAR=\
	".\DataReadout.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\DataReadout.obj" : $(SOURCE) $(DEP_CPP_DATAR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\DataReadout.obj" : $(SOURCE) $(DEP_CPP_DATAR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\DataReadout.sbr" : $(SOURCE) $(DEP_CPP_DATAR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditorView.cpp
DEP_CPP_EDITO=\
	".\EditorChild.h"\
	".\EditorView.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\EditorView.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\EditorView.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\EditorView.sbr" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HfsFitDlg.cpp
DEP_CPP_HFSFI=\
	".\HfsFitDlg.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\HfsFitDlg.obj" : $(SOURCE) $(DEP_CPP_HFSFI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\HfsFitDlg.obj" : $(SOURCE) $(DEP_CPP_HFSFI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\HfsFitDlg.sbr" : $(SOURCE) $(DEP_CPP_HFSFI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EditorChild.cpp
DEP_CPP_EDITOR=\
	".\EditorChild.h"\
	".\EditorView.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\EditorChild.obj" : $(SOURCE) $(DEP_CPP_EDITOR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\EditorChild.obj" : $(SOURCE) $(DEP_CPP_EDITOR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\EditorChild.sbr" : $(SOURCE) $(DEP_CPP_EDITOR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Configure.cpp
DEP_CPP_CONFI=\
	".\Configure.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Configure.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Configure.obj" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Configure.sbr" : $(SOURCE) $(DEP_CPP_CONFI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\EVALUATE.CPP
DEP_CPP_EVALU=\
	".\ActionObjects.h"\
	".\ChildFrm.h"\
	".\evaluate.h"\
	".\HardwareConfig.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\EVALUATE.OBJ" : $(SOURCE) $(DEP_CPP_EVALU) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\EVALUATE.OBJ" : $(SOURCE) $(DEP_CPP_EVALU) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\EVALUATE.SBR" : $(SOURCE) $(DEP_CPP_EVALU) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Hardware.cpp
DEP_CPP_HARDW=\
	".\camac.h"\
	".\Hardware.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Hardware.obj" : $(SOURCE) $(DEP_CPP_HARDW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Hardware.obj" : $(SOURCE) $(DEP_CPP_HARDW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Hardware.sbr" : $(SOURCE) $(DEP_CPP_HARDW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtTI.cpp
DEP_CPP_PROPSHTTI=\
	".\MCP for NT.h"\
	".\page1ti.h"\
	".\Page3TI.h"\
	".\PropShtTI.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtTI.obj" : $(SOURCE) $(DEP_CPP_PROPSHTTI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtTI.obj" : $(SOURCE) $(DEP_CPP_PROPSHTTI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtTI.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTTI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1TI.cpp
DEP_CPP_PAGE1TI=\
	".\MCP for NT.h"\
	".\page1ti.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1TI.obj" : $(SOURCE) $(DEP_CPP_PAGE1TI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1TI.obj" : $(SOURCE) $(DEP_CPP_PAGE1TI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1TI.sbr" : $(SOURCE) $(DEP_CPP_PAGE1TI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3TI.cpp
DEP_CPP_PAGE3TI=\
	".\MCP for NT.h"\
	".\Page3TI.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3TI.obj" : $(SOURCE) $(DEP_CPP_PAGE3TI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3TI.obj" : $(SOURCE) $(DEP_CPP_PAGE3TI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3TI.sbr" : $(SOURCE) $(DEP_CPP_PAGE3TI) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\CamacTester.cpp
DEP_CPP_CAMAC=\
	".\camac.h"\
	".\CamacTester.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\CamacTester.obj" : $(SOURCE) $(DEP_CPP_CAMAC) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\CamacTester.obj" : $(SOURCE) $(DEP_CPP_CAMAC) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\CamacTester.sbr" : $(SOURCE) $(DEP_CPP_CAMAC) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1MS.cpp
DEP_CPP_PAGE1MS=\
	".\MCP for NT.h"\
	".\Page1MS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1MS.obj" : $(SOURCE) $(DEP_CPP_PAGE1MS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1MS.obj" : $(SOURCE) $(DEP_CPP_PAGE1MS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1MS.sbr" : $(SOURCE) $(DEP_CPP_PAGE1MS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3MS.cpp
DEP_CPP_PAGE3MS=\
	".\MCP for NT.h"\
	".\Page3MS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3MS.obj" : $(SOURCE) $(DEP_CPP_PAGE3MS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3MS.obj" : $(SOURCE) $(DEP_CPP_PAGE3MS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3MS.sbr" : $(SOURCE) $(DEP_CPP_PAGE3MS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtMS.cpp
DEP_CPP_PROPSHTMS=\
	".\MCP for NT.h"\
	".\Page1MS.h"\
	".\Page3MS.h"\
	".\PropShtMS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtMS.obj" : $(SOURCE) $(DEP_CPP_PROPSHTMS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtMS.obj" : $(SOURCE) $(DEP_CPP_PROPSHTMS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtMS.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTMS) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\HardwareDefaults.cpp
DEP_CPP_HARDWA=\
	".\HardwareDefaults.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\HardwareDefaults.obj" : $(SOURCE) $(DEP_CPP_HARDWA) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\HardwareDefaults.obj" : $(SOURCE) $(DEP_CPP_HARDWA) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\HardwareDefaults.sbr" : $(SOURCE) $(DEP_CPP_HARDWA) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NetReader.cpp
DEP_CPP_NETRE=\
	".\MCP for NT.h"\
	".\NetReader.h"\
	".\rpcapi32.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\NetReader.obj" : $(SOURCE) $(DEP_CPP_NETRE) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\NetReader.obj" : $(SOURCE) $(DEP_CPP_NETRE) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\NetReader.sbr" : $(SOURCE) $(DEP_CPP_NETRE) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MonitorView.cpp
DEP_CPP_MONIT=\
	".\ActionObjects.h"\
	".\Configure.h"\
	".\DataReadout.h"\
	".\HardwareConfig.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\Page1AS.h"\
	".\Page1PM.h"\
	".\Page3AS.h"\
	".\Page3PM.h"\
	".\PropShtAS.h"\
	".\PropShtPM.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\MonitorView.obj" : $(SOURCE) $(DEP_CPP_MONIT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\MonitorView.obj" : $(SOURCE) $(DEP_CPP_MONIT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\MonitorView.sbr" : $(SOURCE) $(DEP_CPP_MONIT) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\MonitorChild.cpp
DEP_CPP_MONITO=\
	".\ActionObjects.h"\
	".\HardwareConfig.h"\
	".\MCP for NT.h"\
	".\MCP for NTDoc.h"\
	".\minuit.h"\
	".\MinuitChild.h"\
	".\MonitorChild.h"\
	".\MonitorView.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\MonitorChild.obj" : $(SOURCE) $(DEP_CPP_MONITO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\MonitorChild.obj" : $(SOURCE) $(DEP_CPP_MONITO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\MonitorChild.sbr" : $(SOURCE) $(DEP_CPP_MONITO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Display.cpp
DEP_CPP_DISPL=\
	".\display.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Display.obj" : $(SOURCE) $(DEP_CPP_DISPL) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Display.obj" : $(SOURCE) $(DEP_CPP_DISPL) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Display.sbr" : $(SOURCE) $(DEP_CPP_DISPL) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1PR.cpp
DEP_CPP_PAGE1PR=\
	".\MCP for NT.h"\
	".\Page1PR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1PR.obj" : $(SOURCE) $(DEP_CPP_PAGE1PR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1PR.obj" : $(SOURCE) $(DEP_CPP_PAGE1PR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1PR.sbr" : $(SOURCE) $(DEP_CPP_PAGE1PR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3PR.cpp
DEP_CPP_PAGE3PR=\
	".\MCP for NT.h"\
	".\Page3PR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3PR.obj" : $(SOURCE) $(DEP_CPP_PAGE3PR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3PR.obj" : $(SOURCE) $(DEP_CPP_PAGE3PR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3PR.sbr" : $(SOURCE) $(DEP_CPP_PAGE3PR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtPR.cpp
DEP_CPP_PROPSHTPR=\
	".\MCP for NT.h"\
	".\Page1PR.h"\
	".\Page3PR.h"\
	".\PropShtPR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtPR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTPR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtPR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTPR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtPR.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTPR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3Bo.cpp
DEP_CPP_PAGE3B=\
	".\MCP for NT.h"\
	".\Page3Bo.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3Bo.obj" : $(SOURCE) $(DEP_CPP_PAGE3B) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3Bo.obj" : $(SOURCE) $(DEP_CPP_PAGE3B) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3Bo.sbr" : $(SOURCE) $(DEP_CPP_PAGE3B) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1Bo.cpp
DEP_CPP_PAGE1B=\
	".\MCP for NT.h"\
	".\Page1Bo.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1Bo.obj" : $(SOURCE) $(DEP_CPP_PAGE1B) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1Bo.obj" : $(SOURCE) $(DEP_CPP_PAGE1B) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1Bo.sbr" : $(SOURCE) $(DEP_CPP_PAGE1B) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtBo.cpp
DEP_CPP_PROPSHTB=\
	".\MCP for NT.h"\
	".\Page1Bo.h"\
	".\Page3Bo.h"\
	".\PropShtBo.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtBo.obj" : $(SOURCE) $(DEP_CPP_PROPSHTB) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtBo.obj" : $(SOURCE) $(DEP_CPP_PROPSHTB) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtBo.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTB) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3FV.cpp
DEP_CPP_PAGE3FV=\
	".\MCP for NT.h"\
	".\Page3FV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3FV.obj" : $(SOURCE) $(DEP_CPP_PAGE3FV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3FV.obj" : $(SOURCE) $(DEP_CPP_PAGE3FV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3FV.sbr" : $(SOURCE) $(DEP_CPP_PAGE3FV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1FV.cpp
DEP_CPP_PAGE1FV=\
	".\MCP for NT.h"\
	".\Page1FV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1FV.obj" : $(SOURCE) $(DEP_CPP_PAGE1FV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1FV.obj" : $(SOURCE) $(DEP_CPP_PAGE1FV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1FV.sbr" : $(SOURCE) $(DEP_CPP_PAGE1FV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtFV.cpp
DEP_CPP_PROPSHTFV=\
	".\MCP for NT.h"\
	".\Page1FV.h"\
	".\Page2FV.h"\
	".\Page3FV.h"\
	".\PropShtFV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtFV.obj" : $(SOURCE) $(DEP_CPP_PROPSHTFV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtFV.obj" : $(SOURCE) $(DEP_CPP_PROPSHTFV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtFV.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTFV) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\GpibWriter.cpp
DEP_CPP_GPIBW=\
	".\camac.h"\
	".\GpibWriter.h"\
	".\Hardware.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\GpibWriter.obj" : $(SOURCE) $(DEP_CPP_GPIBW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\GpibWriter.obj" : $(SOURCE) $(DEP_CPP_GPIBW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\GpibWriter.sbr" : $(SOURCE) $(DEP_CPP_GPIBW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page2FV.cpp
DEP_CPP_PAGE2=\
	".\MCP for NT.h"\
	".\Page2FV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page2FV.obj" : $(SOURCE) $(DEP_CPP_PAGE2) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page2FV.obj" : $(SOURCE) $(DEP_CPP_PAGE2) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page2FV.sbr" : $(SOURCE) $(DEP_CPP_PAGE2) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page2HV.cpp
DEP_CPP_PAGE2H=\
	".\camac.h"\
	".\MCP for NT.h"\
	".\Page2HV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page2HV.obj" : $(SOURCE) $(DEP_CPP_PAGE2H) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page2HV.obj" : $(SOURCE) $(DEP_CPP_PAGE2H) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page2HV.sbr" : $(SOURCE) $(DEP_CPP_PAGE2H) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page2HP.cpp
DEP_CPP_PAGE2HP=\
	".\MCP for NT.h"\
	".\Page2HP.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page2HP.obj" : $(SOURCE) $(DEP_CPP_PAGE2HP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page2HP.obj" : $(SOURCE) $(DEP_CPP_PAGE2HP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page2HP.sbr" : $(SOURCE) $(DEP_CPP_PAGE2HP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page2RS.cpp
DEP_CPP_PAGE2R=\
	".\Hardware.h"\
	".\MCP for NT.h"\
	".\Page2RS.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page2RS.obj" : $(SOURCE) $(DEP_CPP_PAGE2R) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page2RS.obj" : $(SOURCE) $(DEP_CPP_PAGE2R) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page2RS.sbr" : $(SOURCE) $(DEP_CPP_PAGE2R) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\GpibReader.cpp
DEP_CPP_GPIBR=\
	".\camac.h"\
	".\GpibReader.h"\
	".\Hardware.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\GpibReader.obj" : $(SOURCE) $(DEP_CPP_GPIBR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\GpibReader.obj" : $(SOURCE) $(DEP_CPP_GPIBR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\GpibReader.sbr" : $(SOURCE) $(DEP_CPP_GPIBR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtFR.cpp
DEP_CPP_PROPSHTFR=\
	".\MCP for NT.h"\
	".\Page3FR.h"\
	".\PropShtFR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtFR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTFR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtFR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTFR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtFR.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTFR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtFW.cpp
DEP_CPP_PROPSHTFW=\
	".\MCP for NT.h"\
	".\Page1FW.h"\
	".\Page3FW.h"\
	".\PropShtFW.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtFW.obj" : $(SOURCE) $(DEP_CPP_PROPSHTFW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtFW.obj" : $(SOURCE) $(DEP_CPP_PROPSHTFW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtFW.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTFW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtGR.cpp
DEP_CPP_PROPSHTG=\
	".\MCP for NT.h"\
	".\Page1GR.h"\
	".\Page3GR.h"\
	".\PropShtGR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtGR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTG) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtGR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTG) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtGR.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTG) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtGW.cpp
DEP_CPP_PROPSHTGW=\
	".\MCP for NT.h"\
	".\Page1GW.h"\
	".\Page3GW.h"\
	".\PropShtGW.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtGW.obj" : $(SOURCE) $(DEP_CPP_PROPSHTGW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtGW.obj" : $(SOURCE) $(DEP_CPP_PROPSHTGW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtGW.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTGW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtKR.cpp
DEP_CPP_PROPSHTK=\
	".\MCP for NT.h"\
	".\Page1KR.h"\
	".\Page3KR.h"\
	".\PropShtKR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtKR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTK) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtKR.obj" : $(SOURCE) $(DEP_CPP_PROPSHTK) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtKR.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTK) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtKW.cpp
DEP_CPP_PROPSHTKW=\
	".\MCP for NT.h"\
	".\Page1KW.h"\
	".\Page3KW.h"\
	".\PropShtKW.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtKW.obj" : $(SOURCE) $(DEP_CPP_PROPSHTKW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtKW.obj" : $(SOURCE) $(DEP_CPP_PROPSHTKW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtKW.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTKW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1GR.cpp
DEP_CPP_PAGE1G=\
	".\MCP for NT.h"\
	".\Page1GR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1GR.obj" : $(SOURCE) $(DEP_CPP_PAGE1G) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1GR.obj" : $(SOURCE) $(DEP_CPP_PAGE1G) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1GR.sbr" : $(SOURCE) $(DEP_CPP_PAGE1G) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1FW.cpp
DEP_CPP_PAGE1FW=\
	".\MCP for NT.h"\
	".\Page1FW.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1FW.obj" : $(SOURCE) $(DEP_CPP_PAGE1FW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1FW.obj" : $(SOURCE) $(DEP_CPP_PAGE1FW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1FW.sbr" : $(SOURCE) $(DEP_CPP_PAGE1FW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1FR.cpp
DEP_CPP_PAGE1FR=\
	".\MCP for NT.h"\
	".\Page1FR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1FR.obj" : $(SOURCE) $(DEP_CPP_PAGE1FR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1FR.obj" : $(SOURCE) $(DEP_CPP_PAGE1FR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1FR.sbr" : $(SOURCE) $(DEP_CPP_PAGE1FR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1KW.cpp
DEP_CPP_PAGE1K=\
	".\MCP for NT.h"\
	".\Page1KW.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1KW.obj" : $(SOURCE) $(DEP_CPP_PAGE1K) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1KW.obj" : $(SOURCE) $(DEP_CPP_PAGE1K) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1KW.sbr" : $(SOURCE) $(DEP_CPP_PAGE1K) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1GW.cpp
DEP_CPP_PAGE1GW=\
	".\MCP for NT.h"\
	".\Page1GW.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1GW.obj" : $(SOURCE) $(DEP_CPP_PAGE1GW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1GW.obj" : $(SOURCE) $(DEP_CPP_PAGE1GW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1GW.sbr" : $(SOURCE) $(DEP_CPP_PAGE1GW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1KR.cpp
DEP_CPP_PAGE1KR=\
	".\MCP for NT.h"\
	".\Page1KR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1KR.obj" : $(SOURCE) $(DEP_CPP_PAGE1KR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1KR.obj" : $(SOURCE) $(DEP_CPP_PAGE1KR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1KR.sbr" : $(SOURCE) $(DEP_CPP_PAGE1KR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3FR.cpp
DEP_CPP_PAGE3FR=\
	".\MCP for NT.h"\
	".\Page3FR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3FR.obj" : $(SOURCE) $(DEP_CPP_PAGE3FR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3FR.obj" : $(SOURCE) $(DEP_CPP_PAGE3FR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3FR.sbr" : $(SOURCE) $(DEP_CPP_PAGE3FR) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3FW.cpp
DEP_CPP_PAGE3FW=\
	".\MCP for NT.h"\
	".\Page3FW.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3FW.obj" : $(SOURCE) $(DEP_CPP_PAGE3FW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3FW.obj" : $(SOURCE) $(DEP_CPP_PAGE3FW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3FW.sbr" : $(SOURCE) $(DEP_CPP_PAGE3FW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3GR.cpp
DEP_CPP_PAGE3G=\
	".\MCP for NT.h"\
	".\Page3GR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3GR.obj" : $(SOURCE) $(DEP_CPP_PAGE3G) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3GR.obj" : $(SOURCE) $(DEP_CPP_PAGE3G) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3GR.sbr" : $(SOURCE) $(DEP_CPP_PAGE3G) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3GW.cpp
DEP_CPP_PAGE3GW=\
	".\MCP for NT.h"\
	".\Page3GW.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3GW.obj" : $(SOURCE) $(DEP_CPP_PAGE3GW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3GW.obj" : $(SOURCE) $(DEP_CPP_PAGE3GW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3GW.sbr" : $(SOURCE) $(DEP_CPP_PAGE3GW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3KR.cpp
DEP_CPP_PAGE3K=\
	".\MCP for NT.h"\
	".\Page3KR.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3KR.obj" : $(SOURCE) $(DEP_CPP_PAGE3K) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3KR.obj" : $(SOURCE) $(DEP_CPP_PAGE3K) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3KR.sbr" : $(SOURCE) $(DEP_CPP_PAGE3K) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3KW.cpp
DEP_CPP_PAGE3KW=\
	".\MCP for NT.h"\
	".\Page3KW.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3KW.obj" : $(SOURCE) $(DEP_CPP_PAGE3KW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3KW.obj" : $(SOURCE) $(DEP_CPP_PAGE3KW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3KW.sbr" : $(SOURCE) $(DEP_CPP_PAGE3KW) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\rpcapi32.cpp
DEP_CPP_RPCAP=\
	".\dbcsv32.h"\
	".\dbcsvapi.cpp"\
	".\errors.c"\
	".\ERRORS.H"\
	".\errsrv32.h"\
	".\rpcapi32.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\rpcapi32.obj" : $(SOURCE) $(DEP_CPP_RPCAP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\rpcapi32.obj" : $(SOURCE) $(DEP_CPP_RPCAP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\rpcapi32.sbr" : $(SOURCE) $(DEP_CPP_RPCAP) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\camac.cpp
DEP_CPP_CAMAC_=\
	".\camac.h"\
	".\camacdef.h"\
	".\Hardware.h"\
	".\MCP for NT.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\camac.obj" : $(SOURCE) $(DEP_CPP_CAMAC_) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\camac.obj" : $(SOURCE) $(DEP_CPP_CAMAC_) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\camac.sbr" : $(SOURCE) $(DEP_CPP_CAMAC_) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\NetworkWriter.cpp
DEP_CPP_NETWO=\
	".\MCP for NT.h"\
	".\NetworkWriter.h"\
	".\rpcapi32.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\NetworkWriter.obj" : $(SOURCE) $(DEP_CPP_NETWO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\NetworkWriter.obj" : $(SOURCE) $(DEP_CPP_NETWO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\NetworkWriter.sbr" : $(SOURCE) $(DEP_CPP_NETWO) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page1LV.cpp
DEP_CPP_PAGE1L=\
	".\MCP for NT.h"\
	".\Page1LV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page1LV.obj" : $(SOURCE) $(DEP_CPP_PAGE1L) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page1LV.obj" : $(SOURCE) $(DEP_CPP_PAGE1L) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page1LV.sbr" : $(SOURCE) $(DEP_CPP_PAGE1L) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page2LV.cpp
DEP_CPP_PAGE2L=\
	".\camac.h"\
	".\MCP for NT.h"\
	".\Page2LV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page2LV.obj" : $(SOURCE) $(DEP_CPP_PAGE2L) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page2LV.obj" : $(SOURCE) $(DEP_CPP_PAGE2L) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page2LV.sbr" : $(SOURCE) $(DEP_CPP_PAGE2L) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Page3LV.cpp
DEP_CPP_PAGE3L=\
	".\Hardware.h"\
	".\MCP for NT.h"\
	".\Page3LV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\Page3LV.obj" : $(SOURCE) $(DEP_CPP_PAGE3L) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\Page3LV.obj" : $(SOURCE) $(DEP_CPP_PAGE3L) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\Page3LV.sbr" : $(SOURCE) $(DEP_CPP_PAGE3L) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\PropShtLV.cpp
DEP_CPP_PROPSHTL=\
	".\MCP for NT.h"\
	".\Page1LV.h"\
	".\Page2LV.h"\
	".\Page3LV.h"\
	".\PropShtLV.h"\
	".\StdAfx.h"\
	".\TheDefines.h"\
	

!IF  "$(CFG)" == "MCP for NT - Win32 Release"


"$(INTDIR)\PropShtLV.obj" : $(SOURCE) $(DEP_CPP_PROPSHTL) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ELSEIF  "$(CFG)" == "MCP for NT - Win32 Debug"


"$(INTDIR)\PropShtLV.obj" : $(SOURCE) $(DEP_CPP_PROPSHTL) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"

"$(INTDIR)\PropShtLV.sbr" : $(SOURCE) $(DEP_CPP_PROPSHTL) "$(INTDIR)"\
 "$(INTDIR)\MCP for NT.pch"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
################################################################################
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
################################################################################
################################################################################
# Section MCP for NT : {7B705997-4924-11D0-AF9B-00A024889213}
# 	1:17:CG_IDS_DISK_SPACE:113
# 	1:19:CG_IDS_PHYSICAL_MEM:112
# 	1:25:CG_IDS_DISK_SPACE_UNAVAIL:114
# 	2:10:SysInfoKey:1234
# End Section
################################################################################
################################################################################
# Section MCP for NT : {7B705992-4924-11D0-AF9B-00A024889213}
# 	2:10:CMainFrame:Palette support added
# End Section
################################################################################
################################################################################
# Section MCP for NT : {B6805000-A509-11CE-A5B0-00AA006BBF16}
# 	1:23:CG_IDR_POPUP_DATA_CHILD:118
# 	1:26:CG_IDR_POPUP_MCPFOR_NTVIEW:109
# End Section
################################################################################
