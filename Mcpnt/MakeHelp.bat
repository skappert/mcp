@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by MCP FOR NT.HPJ. >"hlp\MCP for NT.hm"
echo. >>"hlp\MCP for NT.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\MCP for NT.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\MCP for NT.hm"
echo. >>"hlp\MCP for NT.hm"
echo // Prompts (IDP_*) >>"hlp\MCP for NT.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\MCP for NT.hm"
echo. >>"hlp\MCP for NT.hm"
echo // Resources (IDR_*) >>"hlp\MCP for NT.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\MCP for NT.hm"
echo. >>"hlp\MCP for NT.hm"
echo // Dialogs (IDD_*) >>"hlp\MCP for NT.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\MCP for NT.hm"
echo. >>"hlp\MCP for NT.hm"
echo // Frame Controls (IDW_*) >>"hlp\MCP for NT.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\MCP for NT.hm"
REM -- Make help for Project MCP FOR NT


echo Building Win32 Help files
start /wait hcrtf -x "hlp\MCP for NT.hpj"
echo.
if exist Debug\nul copy "hlp\MCP for NT.hlp" Debug
if exist Debug\nul copy "hlp\MCP for NT.cnt" Debug
if exist Release\nul copy "hlp\MCP for NT.hlp" Release
if exist Release\nul copy "hlp\MCP for NT.cnt" Release
echo.


