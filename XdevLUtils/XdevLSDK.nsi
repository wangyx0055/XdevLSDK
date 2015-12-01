
!include "MUI2.nsh"
!include "winmessages.nsh"


; The name of the installer
Name "XdevLSDK 0.5.0"

; The file to write
OutFile "XdevLSDK-0.5.0.exe"

;--------------------------------
;Interface Configuration

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP ../Documents/Images/xdevl-logo.png
  !define MUI_ABORTWARNING

; The default installation directory
InstallDir $DESKTOP/XdevLSDK

; Request application privileges for Windows Vista
RequestExecutionLevel user

; Pages

  !insertmacro MUI_PAGE_WELCOME
;  !insertmacro MUI_PAGE_LICENSE "license/license.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES




; ------------------------------------------------------------------------------
; XdevL installation
;

Section "XdevL" xdevl_section

SetOutPath $INSTDIR

SetOutPath $INSTDIR/XdevL/include
File include/*.h

SectionEnd


; ------------------------------------------------------------------------------
; XdevLPlugins installation
;

Section "XdevLPlugins" xdevl_plugins_section

SetOutPath $INSTDIR/XdevLPlugins/lib/*
File ../XdevLPlugins/lib/XdevLOpenGL-1.0.0d.so

SectionEnd


; ------------------------------------------------------------------------------
; XdevLDemos installation
;

Section "XdevLDemos" xdevl_demos_section

SetOutPath $INSTDIR/XdevLDemos/bin
File ../XdevLDemos/bin\opengl_demo.xml

SetOutPath $INSTDIR/XdevLDemos\bin\media
File /r /x .svn ../XdevLDemos/bin/media/*

SetOutPath $INSTDIR/XdevLDemos/opengl_demo
File ../XdevLDemos/opengl_demo/opengl_demo.cpp
File ../XdevLDemos/opengl_demo/opengl_demo.project


SectionEnd

Section "Uninstall"

RMDIR /r "$INSTDIR\XdevL"
RMDIR /r "$INSTDIR\XdevLPlugins"
RMDIR /r "$INSTDIR\XdevLDemos"
RMDIR /r "$INSTDIR\XdevLExternal"
Delete $INSTDIR\XdevLSDKUninstall.exe"

SectionEnd


Section "Environment Variables" xdevl_env_variables
 !define env_hklm 'HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"'
 !define env_hkcu 'HKCU "Environment"'
 WriteRegExpandStr ${env_hkcu} XDEVL_HOME $INSTDIR\XdevL
 WriteRegExpandStr ${env_hkcu} XDEVL_PLUGINS $INSTDIR\XdevLPlugins
 ;make sure windows knows about the change
 SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
SectionEnd

LangString DESC_xdevl_section ${LANG_ENGLISH} "Install the XdevL main files."
LangString DESC_xdevl_plugins_section ${LANG_ENGLISH} "Install XdevL additional plug-ins."
LangString DESC_xdevl_demos_section ${LANG_ENGLISH} "Install XdevL demo source files."
LangString DESC_xdevl_external_section ${LANG_ENGLISH} "Install external libraries which are necessary for some demo projects."
LangString DESC_xdevl_env_variables ${LANG_ENGLISH} "Install the necessary XdevL environment variables."


!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${xdevl_section} $(DESC_xdevl_section)
  !insertmacro MUI_DESCRIPTION_TEXT ${xdevl_plugins_section} $(DESC_xdevl_plugins_section)
  !insertmacro MUI_DESCRIPTION_TEXT ${xdevl_demos_section} $(DESC_xdevl_demos_section)
  !insertmacro MUI_DESCRIPTION_TEXT ${xdevl_env_variables} $(DESC_xdevl_env_variables)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
