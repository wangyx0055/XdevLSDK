# XdevLSDK
XdevL is a powerful cross-platform C++ Object and Plug-In Oriented programming framework which helps you developing 
Games or device oriented applications. The whole system can be extended using/developing additional Plugins which
makes it extreme flexible.

Here some features:

* Switch to C++11
* XdevLOpenGLContext plugin that helps to create a OpenGL context. GLX, CGL, WGL and EGL are supported.
* XdevLFileSystem plugin that helps you to handling files and folders. A directory watcher is added that helps you to track if files or folders gets created/deleted or modified.
* XdevLRAI is attempt to make a cross platform/3D api rendering interface. At the moment it supports OpenGL based renderer. Vulkan API will be the next which will be implemented and DX9/10/11/12 can be done in future releases.
* XdevLFont that uses XdevLRAI to render text in your 3D graphics projects.
* XdevLWindow supports now native windows using X11, Wayland, Cocoa, Win32 and SDL2. SDL2 is used on platforms which are not supported yet in the XdevL framework. Android, IOS and HTML5 are planned.
* XdevLSharedMemory, XdevLSemaphore and XdevLRingBuffer
* XdevLDelegates for fast delegates. Is another way of handling callback programming.
* XdevLAudioPlayback and XdevLAudioCapture to play and record natively. With those new interfaces it is/will be possible to write your own synthesizer and record from the mic. Future ideas are to have plugins that can be linked to add effects during playback or recording.
* XdevLBluetooth for data transfer over bluetooth support.
* XdevLSerial for data transfer over serial port.
* XdevLFTDI for FTDI Usb serial port devics.
* XdevLArrayModifier to modify arrays without worrying about making casting erros.
* XdevLImage/XdevLImagePlugin Image import support .bmp files. Soon it will support .png, .jpg and .tga files.
* XdevLNetwork to handle TCP/UDP sockets. Supports either to send raw data or to send arrays that holds a structure description to make your code more cross platform. 
* XdevLKeyboard, XdevLMouse and XdevLJoystick to get easy access to your keyboard, mouse and joysticks.
* XdevLOgre that create a basic OGRE instance. With this and the whole XdevL plugins you are ready to write your own game engine.