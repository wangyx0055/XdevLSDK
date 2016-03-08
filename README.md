# XdevLSDK
XdevL is a powerful cross-platform C++ Object and Plug-In Oriented programming framework which helps you developing 
Games or device oriented applications. The whole system can be extended using/developing additional Plugins which
makes it extreme flexible.

# Why would you use it?
* Lot less re-compiling and re-linking of your program.A bugfix of a plugin will not force you to recompile your project.
Only that plugin needs to be compiled.
* Extreme Modular. The system can be extended using new plugins.
* Uses modern C++11/(14) features.
* You can use it to build your own game engine.
* Tons of (well let's say a lot :D) plugins to help you out. See below.

# Here some features
* Uses C++11/(14) standard.
* Cross Platform (GNU/Linux, MacOSX and Windows supported. Android and IOS are planed.)
* Supports plugins and modules which makes the system extreme expandable and flexible.
* Shared memory,Semaphore support
* Basic threading support thread::Thread and thread::Mutex and thread::ConditionalVariable.
* Xml file parsing (it's the tinyxml project http://www.grinninglizard.com/tinyxml/)
* Extended string/filename parsing.
* Support for fast delegates. (Another way of callback functions).
* Array manipulators.
* Support for different types of control devices like, Keyboard, Mouse and Joystick
* Cross platform 3D rendering using XdevLRAI. One implementation is using OpenGL 3.3+ and Vulkan is planned.
* Audio playback and record using XdevLAudioPlayback and XdevLAudioCapture.
* Bluetooth and Serial port support.
* Networking TCP/UDP.
* File/Folder handling using XdevLFile, XdevLDirectory and XdevLDirectoryWatcher.
* OpenGL context using native version as GLX, WGL, CGL and EGL.
* Support for font rendering using XdevLFont and XdevLRAI
* Image import support for Jpeg, Png, Bmp and TGA files.

# Build

The whole XdevLSDK can be build using CMake. In the root folder of XdevLSDK type:

cmake -DCMAKE_BUILD_TYPE=Debug 

### Note
Do not use a subfolder building XdevLSDK. At the moment it is important to run the CMake script within
the root folder where XdevLSDK is located.
The static library of XdevL will be located in ../XdevLSDK/XdevL/lib and the plugins
will be located in the ../XdevLSDK/XdevLPlugins/lib

# Setup

I recommend adding an evnironment variable to your system. This helps your XdevL app to find the
plugins easier. In your shipping (release) version you can copy the plugins wherever you want.

Add this: XDEVL_PLUGINS=../XdevLSDK/XdevLPlugins

