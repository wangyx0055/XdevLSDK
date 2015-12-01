# -----------------------------------------------------------------------------
# Author: Cengiz Terzibas
# Copyright: 2005 - 2011 Cengiz Terzibas
# Brief : Pack xdevl sdk related stuff into one zip
#
import os, zlib, zipfile, string

sdk_name = 'XdevLSDK-1.0.0-SRC.zip'
target_version = ''

# Get the xgw environment path
xdevl_sdk_dir = '..' #os.environ['XDEVL_HOME']


# Create a zip file
f = zipfile.ZipFile(sdk_name, 'w')

# Pack include files
os.chdir(xdevl_sdk_dir)

f.write("XdevL/XdevLWin.workspace", "XdevLSDK/XdevL/XdevLWin.workspace")
f.write("XdevL/XdevLUnix.workspace", "XdevLSDK/XdevL/XdevLUnix.workspace")
f.write("XdevL/XdevLMACOSX.workspace", "XdevLSDK/XdevL/XdevLMACOSX.workspace")

f.write("XdevL/XdevLVS2010.sln", "XdevLSDK/XdevL/XdevLVS2010.sln")
f.write("XdevL/build/vs2010/XdevL.vcxproj", "XdevLSDK/XdevL/build/vs2010/XdevL.vcxproj")
f.write("XdevL/XdevLCore/vs2010/XdevLCore.vcxproj", "XdevLSDK/XdevL/XdevLCore/vs2010/XdevLCore.vcxproj")
#f.write("XdevL/XdevLCore/vs8/XdevLCore.vcproj", "XdevLSDK/XdevL/XdevLCore/vs8/XdevLCore.vcproj")
f.write("XdevL/XdevLCore/cl1/XdevLCoreLinux.project", "XdevLSDK/XdevL/XdevLCore/cl1/XdevLCoreLinux.project")
f.write("XdevL/XdevLCore/cl1/XdevLCoreWin32.project", "XdevLSDK/XdevL/XdevLCore/cl1/XdevLCoreWin32.project")
f.write("XdevL/XdevLCore/cl1/XdevLCoreMACOSX.project", "XdevLSDK/XdevL/XdevLCore/cl1/XdevLCoreMACOSX.project")
f.write("XdevL/change_notes.txt", "XdevLSDK/XdevL/change_notes.txt")
f.write("XdevL/knownbugs.txt", "XdevLSDK/XdevL/knownbugs.txt")
f.write("XdevL/Acknowledgments.txt", "XdevLSDK/XdevL/Acknowledgments.txt")

f.write("XdevL/doc/XdevLSDKManual.odt", "XdevLSDK/XdevL/doc/XdevLSDKManual.odt")
f.write("XdevL/doc/XdevL.dox", "XdevLSDK/XdevL/doc/XdevL.dox")
f.write("XdevL/doc/TinyXml.dox", "XdevLSDK/XdevL/doc/TinyXml.dox")
f.write("XdevL/doc/XdevLMainPage.txt", "XdevLSDK/XdevL/doc/XdevLMainPage.txt")
f.write("XdevL/doc/XdevL.chm", "XdevLSDK/XdevL/doc/XdevL.chm")
f.write("XdevL/doc/TinyXml.chm", "XdevLSDK/XdevL/doc/TinyXml.chm")
f.write("XdevL/doc/XdevLPlugins.chm", "XdevLSDK/XdevL/doc/XdevLPlugins.chm")



for file in os.listdir('XdevL/doc/photo/'):
    include_name = 'XdevL/doc/photo/' + file
    
    # Write file only if it's an header file
    if include_name.endswith('png'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)


for file in os.listdir('XdevL/license/'):
    include_name = 'XdevL/license/' + file
    
    # Write file only if it's an header file
    if include_name.endswith('txt'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)



for file in os.listdir('XdevL/include/'):
    include_name = 'XdevL/include/' + file
    
    # Write file only if it's an header file
    if include_name.endswith('h'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)

# Pack src files
for file in os.listdir('XdevL/src/'):
    include_name = 'XdevL/src/' + file
    
    # Write file only if it's an header file
    if include_name.endswith('.cpp'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)


for file in os.listdir('XdevL/build/vs8/'):
    include_name = 'XdevL/build/vs8/' + file
    
    # Write file only if it's an header file
    if include_name.endswith('.vcproj'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)

for file in os.listdir('XdevL/build/cl1'):
    include_name = 'XdevL/build/cl1/' + file
    
    # Write file only if it's an header file
    if include_name.endswith('.project'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)


# Pack XdevLCore include files
for file in os.listdir('XdevL/XdevLCore/'):
    include_name = 'XdevL/XdevLCore/' + file
    
    if include_name.endswith('.h'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)
    
    if include_name.endswith('.cpp'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)
		
for file in os.listdir('XdevL/XdevLCore/OIS/include/'):
    include_name = 'XdevL/XdevLCore/OIS/include/' + file
    
    if include_name.endswith('.h'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)


for file in os.listdir('XdevL/XdevLCore/OIS/src/'):
    include_name = 'XdevL/XdevLCore/OIS/src/' + file
    
    if include_name.endswith('.cpp'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)


for file in os.listdir('XdevL/XdevLCore/OIS/include/win32/'):
    include_name = 'XdevL/XdevLCore/OIS/include/win32/' + file
    
    if include_name.endswith('.h'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)

for file in os.listdir('XdevL/XdevLCore/OIS/src/win32/'):
    include_name = 'XdevL/XdevLCore/OIS/src/win32/' + file
    
    if include_name.endswith('.cpp'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)


for file in os.listdir('XdevL/XdevLCore/OIS/include/linux/'):
    include_name = 'XdevL/XdevLCore/OIS/include/linux/' + file
    
    if include_name.endswith('.h'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)

for file in os.listdir('XdevL/XdevLCore/OIS/src/linux/'):
    include_name = 'XdevL/XdevLCore/OIS/src/linux/' + file
    
    if include_name.endswith('.cpp'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)
		
for file in os.listdir('XdevL/XdevLCore/OIS/include/mac/'):
    include_name = 'XdevL/XdevLCore/OIS/include/mac/' + file
    
    if include_name.endswith('.h'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)

for file in os.listdir('XdevL/XdevLCore/OIS/src/mac/'):
    include_name = 'XdevL/XdevLCore/OIS/src/mac/' + file
    
    if include_name.endswith('.cpp'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)
		
 
# Pack XdevLCore files
for file in os.listdir('XdevL/XdevLCore/OIS/'):
    include_name = 'XdevL/XdevLCore/OIS/' + file
    
    # Write file only if it's an header file
    if include_name.endswith('.cpp'):
        virtual = "XdevLSDK/" + include_name
        print("Packing to: ", virtual)
        f.write(include_name, virtual)
			
# ---------------------------------------------------------------
# XdevLPlugins
#

f.write("XdevLPlugins/doc/XdevLPlugins.dox", "XdevLSDK/XdevLPlugins/doc/XdevLPlugins.dox")
f.write("XdevLPlugins/doc/XdevLPluginsMainPage.txt", "XdevLSDK/XdevLPlugins/doc/XdevLPluginsMainPage.txt")
f.write("XdevLPlugins/license/license_glew.txt", "XdevLSDK/XdevLPlugins/license/license_glew.txt")
f.write("XdevLPlugins/license/license_ogre.txt", "XdevLSDK/XdevLPlugins/license/license_ogre.txt")

f.write("XdevLPlugins/XdevLAudio/XdevLAudio.h", "XdevLSDK/XdevLPlugins/XdevLAudio/XdevLAudio.h")
f.write("XdevLPlugins/XdevLAudio/XdevLAudioAL.h", "XdevLSDK/XdevLPlugins/XdevLAudio/XdevLAudioAL.h")
f.write("XdevLPlugins/XdevLAudio/XdevLAudioAL.cpp", "XdevLSDK/XdevLPlugins/XdevLAudio/XdevLAudioAL.cpp")
#f.write("XdevLPlugins/XdevLAudio/XdevLAudioAL.vcproj", "XdevLSDK/XdevLPlugins/XdevLAudio/XdevLAudioAL.vcproj")
f.write("XdevLPlugins/XdevLAudio/vs2010/XdevLAudioAL.vcxproj", "XdevLSDK/XdevLPlugins/XdevLAudio/vs2010/XdevLAudioAL.vcxproj")
f.write("XdevLPlugins/XdevLAudio/XdevLAudioAL.project", "XdevLSDK/XdevLPlugins/XdevLAudio/XdevLAudioAL.project")
f.write("XdevLPlugins/XdevLAudio/XdevLAudioALWin.project", "XdevLSDK/XdevLPlugins/XdevLAudio/XdevLAudioALWin.project")

f.write("XdevLPlugins/XdevLOgre/XdevLOgre.h", "XdevLSDK/XdevLPlugins/XdevLOgre/XdevLOgre.h")
f.write("XdevLPlugins/XdevLOgre/XdevLOgreImpl.h", "XdevLSDK/XdevLPlugins/XdevLOgre/XdevLOgreImpl.h")
f.write("XdevLPlugins/XdevLOgre/XdevLOgreImpl.cpp", "XdevLSDK/XdevLPlugins/XdevLOgre/XdevLOgreImpl.cpp")
#f.write("XdevLPlugins/XdevLOgre/XdevLOgre.vcproj", "XdevLSDK/XdevLPlugins/XdevLOgre/XdevLOgre.vcproj")
f.write("XdevLPlugins/XdevLOgre/vs2010/XdevLOgre.vcxproj", "XdevLSDK/XdevLPlugins/XdevLOgre/vs2010/XdevLOgre.vcxproj")
f.write("XdevLPlugins/XdevLOgre/XdevLOgre.project", "XdevLSDK/XdevLPlugins/XdevLOgre/XdevLOgre.project")
f.write("XdevLPlugins/XdevLOgre/XdevLOgreWin.project", "XdevLSDK/XdevLPlugins/XdevLOgre/XdevLOgreWin.project")

f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGL.h", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGL.h")
f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGLImpl.h", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGLImpl.h")
f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGLImpl.cpp", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGLImpl.cpp")
f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGLWGL.h", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGLWGL.h")
f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGLWGL.cpp", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGLWGL.cpp")
f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGLGLX.h", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGLGLX.h")
f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGLGLX.cpp", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGLGLX.cpp")
#f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGL.vcproj", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGL.vcproj")
f.write("XdevLPlugins/XdevLOpenGL/vs2010/XdevLOpenGL.vcxproj", "XdevLSDK/XdevLPlugins/XdevLOpenGL/vs2010/XdevLOpenGL.vcxproj")
f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGL.project", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGL.project")
f.write("XdevLPlugins/XdevLOpenGL/XdevLOpenGLWin.project", "XdevLSDK/XdevLPlugins/XdevLOpenGL/XdevLOpenGLWin.project")

f.write("XdevLPlugins/XdevLOpenGL/GL/glew.h", "XdevLSDK/XdevLPlugins/XdevLOpenGL/GL/glew.h")
f.write("XdevLPlugins/XdevLOpenGL/GL/glew.c", "XdevLSDK/XdevLPlugins/XdevLOpenGL/GL/glew.c")
f.write("XdevLPlugins/XdevLOpenGL/GL/glext.h", "XdevLSDK/XdevLPlugins/XdevLOpenGL/GL/glext.h")
f.write("XdevLPlugins/XdevLOpenGL/GL/glxew.h", "XdevLSDK/XdevLPlugins/XdevLOpenGL/GL/glxew.h")
f.write("XdevLPlugins/XdevLOpenGL/GL/wglew.h", "XdevLSDK/XdevLPlugins/XdevLOpenGL/GL/wglew.h")
f.write("XdevLPlugins/XdevLOpenGL/GL/wglext.h", "XdevLSDK/XdevLPlugins/XdevLOpenGL/GL/wglext.h")

f.write("XdevLPlugins/XdevLSerial/XdevLSerial.h", "XdevLSDK/XdevLPlugins/XdevLSerial/XdevLSerial.h")
f.write("XdevLPlugins/XdevLSerial/XdevLSerialLinuxImpl.h", "XdevLSDK/XdevLPlugins/XdevLSerial/XdevLSerialLinuxImpl.h")
f.write("XdevLPlugins/XdevLSerial/XdevLSerialLinuxImpl.cpp", "XdevLSDK/XdevLPlugins/XdevLSerial/XdevLSerialLinuxImpl.cpp")
f.write("XdevLPlugins/XdevLSerial/XdevLSerialWinImpl.h", "XdevLSDK/XdevLPlugins/XdevLSerial/XdevLSerialWinImpl.h")
f.write("XdevLPlugins/XdevLSerial/XdevLSerialWinImpl.cpp", "XdevLSDK/XdevLPlugins/XdevLSerial/XdevLSerialWinImpl.cpp")
f.write("XdevLPlugins/XdevLSerial/vs2010/XdevLSerial.vcxproj", "XdevLSDK/XdevLPlugins/XdevLSerial/vs2010/XdevLSerial.vcxproj")
f.write("XdevLPlugins/XdevLSerial/XdevLSerialLinux.project", "XdevLSDK/XdevLPlugins/XdevLSerial/XdevLSerialLinux.project")
f.write("XdevLPlugins/XdevLSerial/XdevLSerial.project", "XdevLSDK/XdevLPlugins/XdevLSerial/XdevLSerial.project")


f.write("XdevLPlugins/XdevLBluetooth/WinBluetoothWrapper.h", "XdevLSDK/XdevLPlugins/XdevLBluetooth/WinBluetoothWrapper.h")
f.write("XdevLPlugins/XdevLBluetooth/XdevLBluetooth.h", "XdevLSDK/XdevLPlugins/XdevLBluetooth/XdevLBluetooth.h")
f.write("XdevLPlugins/XdevLBluetooth/XdevLBluetoothLinuxImpl.h", "XdevLSDK/XdevLPlugins/XdevLBluetooth/XdevLBluetoothLinuxImpl.h")
f.write("XdevLPlugins/XdevLBluetooth/XdevLBluetoothLinuxImpl.cpp", "XdevLSDK/XdevLPlugins/XdevLBluetooth/XdevLBluetoothLinuxImpl.cpp")
f.write("XdevLPlugins/XdevLBluetooth/XdevLBluetoothWinImpl.h", "XdevLSDK/XdevLPlugins/XdevLBluetooth/XdevLBluetoothWinImpl.h")
f.write("XdevLPlugins/XdevLBluetooth/XdevLBluetoothWinImpl.cpp", "XdevLSDK/XdevLPlugins/XdevLBluetooth/XdevLBluetoothWinImpl.cpp")
#f.write("XdevLPlugins/XdevLBluetooth/vs2010/XdevLBluetooth.vcxproj", "XdevLSDK/XdevLPlugins/XdevLBluetooth/vs2010/XdevLBluetooth.vcxproj")
f.write("XdevLPlugins/XdevLBluetooth/XdevLBluetoothLinux.project", "XdevLSDK/XdevLPlugins/XdevLBluetooth/XdevLBluetoothLinux.project")
f.write("XdevLPlugins/XdevLBluetooth/XdevLBluetooth.project", "XdevLSDK/XdevLPlugins/XdevLBluetooth/XdevLBluetooth.project")



f.write("XdevLPlugins/XdevLPluginsWin.workspace", "XdevLSDK/XdevLPlugins/XdevLPluginsWin.workspace")
f.write("XdevLPlugins/XdevLPluginsUnix.workspace", "XdevLSDK/XdevLPlugins/XdevLPluginsUnix.workspace")
f.write("XdevLPlugins/XdevLPluginsMACOSX.workspace", "XdevLSDK/XdevLPlugins/XdevLPluginsMACOSX.workspace")
f.write("XdevLPlugins/XdevLPlugins.sln", "XdevLSDK/XdevLPlugins/XdevLPlugins.sln")
f.write("XdevLPlugins/XdevLPluginsVS2010.sln", "XdevLSDK/XdevLPlugins/XdevLPluginsVS2010.sln")


# ---------------------------------------------------------------
# XdevLDemos
#

f.write("XdevLDemos/XdevLDemosWin.workspace", "XdevLSDK/XdevLDemos/XdevLDemosWin.workspace")
f.write("XdevLDemos/XdevLDemosMacOSX.workspace", "XdevLSDK/XdevLDemos/XdevLDemosMacOSX.workspace")
f.write("XdevLDemos/XdevLDemosUnix.workspace", "XdevLSDK/XdevLDemos/XdevLDemosUnix.workspace")
#f.write("XdevLDemos/XdevLDemos.sln", "XdevLSDK/XdevLDemos/XdevLDemos.sln")
f.write("XdevLDemos/XdevLDemosVS2010.sln", "XdevLSDK/XdevLDemos/XdevLDemosVS2010.sln")


f.write("XdevLDemos/bin/media/Cube.mesh", "XdevLSDK/XdevLDemos/bin/media/Cube.mesh")
f.write("XdevLDemos/bin/media/loop_a.wav", "XdevLSDK/XdevLDemos/bin/media/loop_a.wav")
f.write("XdevLDemos/bin/media/loop_b.wav", "XdevLSDK/XdevLDemos/bin/media/loop_b.wav")
f.write("XdevLDemos/bin/media/Scene.material", "XdevLSDK/XdevLDemos/bin/media/Scene.material")


f.write("XdevLDemos/bin/audio_demo.xml", "XdevLSDK/XdevLDemos/bin/audio_demo.xml")
f.write("XdevLDemos/bin/devices_demo.xml", "XdevLSDK/XdevLDemos/bin/devices_demo.xml")
f.write("XdevLDemos/bin/ogre_demo.xml", "XdevLSDK/XdevLDemos/bin/ogre_demo.xml")
f.write("XdevLDemos/bin/opengl_demo.xml", "XdevLSDK/XdevLDemos/bin/opengl_demo.xml")
f.write("XdevLDemos/bin/udp_client_demo.xml", "XdevLSDK/XdevLDemos/bin/udp_client_demo.xml")
f.write("XdevLDemos/bin/udp_server_demo.xml", "XdevLSDK/XdevLDemos/bin/udp_server_demo.xml")
f.write("XdevLDemos/bin/xml_demo.xml", "XdevLSDK/XdevLDemos/bin/xml_demo.xml")
f.write("XdevLDemos/bin/bluetooth_demo.xml", "XdevLSDK/XdevLDemos/bin/bluetooth_demo.xml")
f.write("XdevLDemos/bin/serial_demo.xml", "XdevLSDK/XdevLDemos/bin/serial_demo.xml")


f.write("XdevLDemos/audio_demo/audio_demo.cpp", "XdevLSDK/XdevLDemos/audio_demo/audio_demo.cpp")
#f.write("XdevLDemos/audio_demo/audio_demo.vcproj", "XdevLSDK/XdevLDemos/audio_demo/audio_demo.vcproj")
f.write("XdevLDemos/audio_demo/audio_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/audio_demo/audio_demoVS2010.vcxproj")
f.write("XdevLDemos/audio_demo/audio_demo.project", "XdevLSDK/XdevLDemos/audio_demo/audio_demo.project")

f.write("XdevLDemos/devices_demo/devices_demo.cpp", "XdevLSDK/XdevLDemos/devices_demo/devices_demo.cpp")
#f.write("XdevLDemos/devices_demo/devices_demo.vcproj", "XdevLSDK/XdevLDemos/devices_demo/devices_demo.vcproj")
f.write("XdevLDemos/devices_demo/devices_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/devices_demo/devices_demoVS2010.vcxproj")
f.write("XdevLDemos/devices_demo/devices_demo.project", "XdevLSDK/XdevLDemos/devices_demo/devices_demo.project")

f.write("XdevLDemos/ogre_demo/ogre_demo.cpp", "XdevLSDK/XdevLDemos/ogre_demo/ogre_demo.cpp")
#f.write("XdevLDemos/ogre_demo/ogre_demo.vcproj", "XdevLSDK/XdevLDemos/ogre_demo/ogre_demo.vcproj")
f.write("XdevLDemos/ogre_demo/ogre_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/ogre_demo/ogre_demoVS2010.vcxproj")
f.write("XdevLDemos/ogre_demo/ogre_demo.project", "XdevLSDK/XdevLDemos/ogre_demo/ogre_demo.project")

f.write("XdevLDemos/opengl_demo/opengl_demo.cpp", "XdevLSDK/XdevLDemos/opengl_demo/opengl_demo.cpp")
#f.write("XdevLDemos/opengl_demo/opengl_demo.vcproj", "XdevLSDK/XdevLDemos/opengl_demo/opengl_demo.vcproj")
f.write("XdevLDemos/opengl_demo/opengl_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/opengl_demo/opengl_demoVS2010.vcxproj")
f.write("XdevLDemos/opengl_demo/opengl_demo.project", "XdevLSDK/XdevLDemos/opengl_demo/opengl_demo.project")

f.write("XdevLDemos/udp_client_demo/udp_client_demo.cpp", "XdevLSDK/XdevLDemos/udp_client_demo/udp_client_demo.cpp")
#f.write("XdevLDemos/udp_client_demo/udp_client_demo.vcproj", "XdevLSDK/XdevLDemos/udp_client_demo/udp_client_demo.vcproj")
f.write("XdevLDemos/udp_client_demo/udp_client_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/udp_client_demo/udp_client_demoVS2010.vcxproj")
f.write("XdevLDemos/udp_client_demo/udp_client_demo.project", "XdevLSDK/XdevLDemos/udp_client_demo/udp_client_demo.project")

f.write("XdevLDemos/udp_server_demo/udp_server_demo.cpp", "XdevLSDK/XdevLDemos/udp_server_demo/udp_server_demo.cpp")
#f.write("XdevLDemos/udp_server_demo/udp_server_demo.vcproj", "XdevLSDK/XdevLDemos/udp_server_demo/udp_server_demo.vcproj")
f.write("XdevLDemos/udp_server_demo/udp_server_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/udp_server_demo/udp_server_demoVS2010.vcxproj")
f.write("XdevLDemos/udp_server_demo/udp_server_demo.project", "XdevLSDK/XdevLDemos/udp_server_demo/udp_server_demo.project")

f.write("XdevLDemos/udp_server_array_demo/udp_server_array_demo.cpp", "XdevLSDK/XdevLDemos/udp_server_array_demo/udp_server_array_demo.cpp")
f.write("XdevLDemos/udp_server_array_demo/udp_server_array_demo.project", "XdevLSDK/XdevLDemos/udp_server_array_demo/udp_server_array_demo.project")
f.write("XdevLDemos/bin/udp_server_array_demo.xml", "XdevLSDK/XdevLDemos/bin/udp_server_array_demo.xml")

f.write("XdevLDemos/udp_client_array_demo/udp_client_array_demo.cpp", "XdevLSDK/XdevLDemos/udp_client_array_demo/udp_client_array_demo.cpp")
f.write("XdevLDemos/udp_client_array_demo/udp_client_array_demo.project", "XdevLSDK/XdevLDemos/udp_client_array_demo/udp_client_array_demo.project")
f.write("XdevLDemos/bin/udp_client_array_demo.xml", "XdevLSDK/XdevLDemos/bin/udp_client_array_demo.xml")

f.write("XdevLDemos/tcp_client_demo/tcp_client_demo.cpp", "XdevLSDK/XdevLDemos/tcp_client_demo/tcp_client_demo.cpp")
#f.write("XdevLDemos/tcp_client_demo/tcp_client_demo.vcproj", "XdevLSDK/XdevLDemos/tcp_client_demo/tcp_client_demo.vcproj")
f.write("XdevLDemos/tcp_client_demo/tcp_client_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/tcp_client_demo/tcp_client_demoVS2010.vcxproj")
f.write("XdevLDemos/tcp_client_demo/tcp_client_demo.project", "XdevLSDK/XdevLDemos/tcp_client_demo/tcp_client_demo.project")

f.write("XdevLDemos/tcp_server_demo/tcp_server_demo.cpp", "XdevLSDK/XdevLDemos/tcp_server_demo/tcp_server_demo.cpp")
#f.write("XdevLDemos/tcp_server_demo/tcp_server_demo.vcproj", "XdevLSDK/XdevLDemos/tcp_server_demo/tcp_server_demo.vcproj")
f.write("XdevLDemos/tcp_server_demo/tcp_server_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/tcp_server_demo/tcp_server_demoVS2010.vcxproj")
f.write("XdevLDemos/tcp_server_demo/tcp_server_demo.project", "XdevLSDK/XdevLDemos/tcp_server_demo/tcp_server_demo.project")

f.write("XdevLDemos/serial_demo/serial_demo.cpp", "XdevLSDK/XdevLDemos/serial_demo/serial_demo.cpp")
f.write("XdevLDemos/serial_demo/serial_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/serial_demo/serial_demoVS2010.vcxproj")
f.write("XdevLDemos/serial_demo/serial_demo.project", "XdevLSDK/XdevLDemos/serial_demo/serial_demo.project")

f.write("XdevLDemos/bluetooth_demo/bluetooth_demo.cpp", "XdevLSDK/XdevLDemos/bluetooth_demo/bluetooth_demo.cpp")
f.write("XdevLDemos/bluetooth_demo/bluetooth_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/bluetooth_demo/bluetooth_demoVS2010.vcxproj")
f.write("XdevLDemos/bluetooth_demo/bluetooth_demo.project", "XdevLSDK/XdevLDemos/bluetooth_demo/bluetooth_demo.project")


f.write("XdevLDemos/xml_demo/xml_demo.cpp", "XdevLSDK/XdevLDemos/xml_demo/xml_demo.cpp")
#f.write("XdevLDemos/xml_demo/xml_demo.vcproj", "XdevLSDK/XdevLDemos/xml_demo/xml_demo.vcproj")
f.write("XdevLDemos/xml_demo/xml_demoVS2010.vcxproj", "XdevLSDK/XdevLDemos/xml_demo/xml_demoVS2010.vcxproj")
f.write("XdevLDemos/xml_demo/xml_demo.project", "XdevLSDK/XdevLDemos/xml_demo/xml_demo.project")
        		
f.close()
