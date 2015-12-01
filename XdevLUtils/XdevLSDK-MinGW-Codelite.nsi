
!include "MUI2.nsh"
!include "winmessages.nsh"


; The name of the installer
Name "XdevLSDK 0.3.0 for MinGW-Codelite"

; The file to write
OutFile "XdevLSDK-0.3.0_MinGW_CodeLite.exe"

;--------------------------------
;Interface Configuration

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP doc\photo\xdevl-logo.png
  !define MUI_ABORTWARNING

; The default installation directory
InstallDir $DESKTOP\XdevLSDK

; Request application privileges for Windows Vista
RequestExecutionLevel user

; Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "license\license.txt"
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
;WriteUninstaller "XdevLSDKUninstall.exe"

SetOutPath $INSTDIR\XdevL\include
File include\*.h

SetOutPath $INSTDIR\XdevL\license
File license\license.txt
File license\ois_license.txt
File license\tinyxml_license.txt


SetOutPath $INSTDIR\XdevL\lib
File lib\libXdevL.a
File lib\libXdevLd.a
File lib\XdevLCore-0.3.0.so
File lib\XdevLCore-0.3.0d.so

SetOutPath $INSTDIR\XdevL
File change_notes.txt
File knownbugs.txt


SetOutPath $INSTDIR\XdevL\doc
File doc\TinyXml.chm
File doc\XdevL.chm
File doc\XdevLSDKManual.pdf

SectionEnd


; ------------------------------------------------------------------------------
; XdevLPlugins installation
;

Section "XdevLPlugins" xdevl_plugins_section

SetOutPath $INSTDIR\XdevLPlugins\lib\*
File ..\XdevLPlugins\lib\XdevLAudioAL-0.2.0.so
File ..\XdevLPlugins\lib\XdevLAudioAL-0.2.0d.so
File ..\XdevLPlugins\lib\XdevLOgre-0.2.0.so
File ..\XdevLPlugins\lib\XdevLOgre-0.2.0d.so
File ..\XdevLPlugins\lib\XdevLOpenGL-0.2.0.so
File ..\XdevLPlugins\lib\XdevLOpenGL-0.2.0d.so

SetOutPath $INSTDIR\XdevLPlugins\XdevLOpenGL
File ..\XdevLPlugins\XdevLOpenGL\XdevLOpenGL.h
SetOutPath $INSTDIR\XdevLPlugins\XdevLOpenGL\GL
File ..\XdevLPlugins\XdevLOpenGL\GL\glew.h
File ..\XdevLPlugins\XdevLOpenGL\GL\glext.h
File ..\XdevLPlugins\XdevLOpenGL\GL\wglew.h
File ..\XdevLPlugins\XdevLOpenGL\GL\wglext.h

SetOutPath $INSTDIR\XdevLPlugins\XdevLOgre
File ..\XdevLPlugins\XdevLOgre\XdevLOgre.h

SetOutPath $INSTDIR\XdevLPlugins\XdevLAudio
File ..\XdevLPlugins\XdevLAudio\XdevLAudio.h

SetOutPath $INSTDIR\XdevLPlugins\doc
File ..\XdevLPlugins\doc\XdevLPlugins.chm

SetOutPath $INSTDIR\XdevLPlugins\license
File ..\XdevLPlugins\license\license_glew.txt
File ..\XdevLPlugins\license\license_ogre.txt

SectionEnd


; ------------------------------------------------------------------------------
; XdevLDemos installation
;

Section "XdevLDemos" xdevl_demos_section

SetOutPath $INSTDIR\XdevLDemos\bin
File ..\XdevLDemos\bin\devices_demo.xml
File ..\XdevLDemos\bin\opengl_demo.xml
File ..\XdevLDemos\bin\ogre_demo.xml
File ..\XdevLDemos\bin\audio_demo.xml
File ..\XdevLDemos\bin\udp_client_demo.xml
File ..\XdevLDemos\bin\udp_server_demo.xml
File ..\XdevLDemos\bin\xml_demo.xml

SetOutPath $INSTDIR\XdevLDemos\bin\media
File /r /x .svn ..\XdevLDemos\bin\media\*

SetOutPath $INSTDIR\XdevLDemos\opengl_demo
File ..\XdevLDemos\opengl_demo\opengl_demo.cpp
File ..\XdevLDemos\opengl_demo\opengl_demo.project

SetOutPath $INSTDIR\XdevLDemos\ogre_demo
File ..\XdevLDemos\ogre_demo\ogre_demo.cpp
File ..\XdevLDemos\ogre_demo\ogre_demo.project

SetOutPath $INSTDIR\XdevLDemos\audio_demo
File ..\XdevLDemos\audio_demo\audio_demo.cpp
File ..\XdevLDemos\audio_demo\audio_demo.project

SetOutPath $INSTDIR\XdevLDemos\udp_client_demo
File ..\XdevLDemos\udp_client_demo\udp_client_demo.cpp
File ..\XdevLDemos\udp_client_demo\udp_client_demo.project

SetOutPath $INSTDIR\XdevLDemos\udp_server_demo
File ..\XdevLDemos\udp_server_demo\udp_server_demo.cpp
File ..\XdevLDemos\udp_server_demo\udp_server_demo.project

SetOutPath $INSTDIR\XdevLDemos\tcp_client_demo
File ..\XdevLDemos\tcp_client_demo\tcp_client_demo.cpp
File ..\XdevLDemos\tcp_client_demo\tcp_client_demo.project

SetOutPath $INSTDIR\XdevLDemos\tcp_server_demo
File ..\XdevLDemos\tcp_server_demo\tcp_server_demo.cpp
File ..\XdevLDemos\tcp_server_demo\tcp_server_demo.project

SetOutPath $INSTDIR\XdevLDemos\devices_demo
File ..\XdevLDemos\devices_demo\devices_demo.cpp
File ..\XdevLDemos\devices_demo\devices_demo.project

SetOutPath $INSTDIR\XdevLDemos\xml_demo
File ..\XdevLDemos\xml_demo\xml_demo.cpp
File ..\XdevLDemos\xml_demo\xml_demo.project

SetOutPath $INSTDIR\XdevLDemos
File ..\XdevLDemos\XdevLDemos.workspace

SectionEnd

; ---------------------------------------
; XdevLExternal installation
;

Section "XdevLExternal" xdevl_external_section

SetOutPath $INSTDIR\XdevLExternal
File /r ..\XdevLExternalMinGW\* 

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
 WriteRegExpandStr ${env_hkcu} XDEVL_EXTERNAL $INSTDIR\XdevLExternal
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
  !insertmacro MUI_DESCRIPTION_TEXT ${xdevl_external_section} $(DESC_xdevl_external_section)
  !insertmacro MUI_DESCRIPTION_TEXT ${xdevl_env_variables} $(DESC_xdevl_env_variables)
!insertmacro MUI_FUNCTION_DESCRIPTION_END