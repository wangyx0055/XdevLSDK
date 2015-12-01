#! /usr/bin/env python


# -----------------------------------------------------------------------------
# Author: Cengiz Terzibas
# Brief : Pack xdevl plugins related stuff into one zip
#
import os, zlib, zipfile, string, StringIO

sdk_name = 'XdevLPlugins.zip'
target_version = ''

# Get the xgw environment path
xdevl_plugins_dir = os.environ['XDEVL_PLUGINS']
xdevl_plugins_lib_dir = os.environ['XDEVL_PLUGINS']

# Create a zip file
f = zipfile.ZipFile(sdk_name, 'w')

# Pack include files
os.chdir(xdevl_plugins_dir)
plugin1 = 'XdevLOpenGL/XdevLOpenGL.h'
plugin2 = 'XdevLOgre/XdevLOgre.h'
plugin3 = 'XdevLAudioAL/XdevLAudio.h'
plugin4 = 'XdevLVicon/XdevLVicon.h'
plugin5 = 'XdevLOptiTrack/XdevLOptiTrack.h'
plugin6 = 'XdevLColOgre/ColladaDatabase.h'
plugin7 = 'XdevLMotionPlatform/XdevLMotionPlatform.h'
plugin8 = 'XdevLDataTranslation/XdevLDataTranslation.h'

print plugin1
print plugin2
print plugin3
print plugin4
print plugin5
print plugin6
print plugin7
print plugin8


f.write(plugin1)
f.write(plugin2)
f.write(plugin3)
f.write(plugin4)
f.write(plugin5)
f.write(plugin6)
f.write(plugin7)
f.write(plugin8)


# Pack library files
os.chdir(xdevl_plugins_lib_dir)    
for file in os.listdir('lib/' + target_version):
    # Check if this is an .svn folder
    lib_name = 'lib/' + target_version + "/" + file
    
    # Write file only if it's a .lib or .dll or .so or .a
#    if lib_name.endswith('lib') or lib_name.endswith('dll') or lib_name.endswith('a') or lib_name.endswith('so'):
    if lib_name.endswith('dll') or lib_name.endswith('so'):
        print "Packing ", lib_name
        f.write(lib_name)
			
f.close()
