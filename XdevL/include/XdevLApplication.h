/*
	Copyright (c) 2005 - 2016 Cengiz Terzibas

	Permission is hereby granted, free of charge, to any person obtaining a copy of 
	this software and associated documentation files (the "Software"), to deal in the 
	Software without restriction, including without limitation the rights to use, copy, 
	modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
	and to permit persons to whom the Software is furnished to do so, subject to the 
	following conditions:

	The above copyright notice and this permission notice shall be included in all copies 
	or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
	DEALINGS IN THE SOFTWARE.

	cengiz@terzibas.de
*/

#ifndef XDEVL_APPLICATION_H
#define XDEVL_APPLICATION_H

#include <XdevL.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLInput/XdevLMouse/XdevLMouse.h>
#include <XdevLInput/XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLInput/XdevLJoystick/XdevLJoystick.h>

#include <fstream>
#include <vector>
#include <string>


namespace xdl {

	class XdevLMouse;
	class XdevLKeyboard;
	class XdevLWindow;
	class XdevLJoystick;

	/**
		@class XdevLApplication
		@brief The main class for all XdevL applications.
		@author Cengiz Terzibas
	*/
	class XdevLApplication : public XdevLListener {
		public:
			/// Type for the arguments.
			typedef std::vector<std::string> Arguments;

			/// Type for one argument.
			typedef std::string Argument;

			XdevLApplication(int argc = 0, char** argv = NULL, const XdevLFileName& xml_filename = XdevLFileName()) throw() : rdstream(NULL), m_id("XdevLApplication") {

				this->m_argc = argc;
				this->m_argv = argv;

				// Create strings out of the argument vector.
				this->m_args = Arguments(argv, argv+argc);

				// Flag for redirecting the output stream.
				bool rds = false;

				int argcursor = -1;

				Arguments::iterator arg(this->m_args.begin());

				// Check argument list.
				while(arg != this->m_args.end()) {


					argcursor++;
					// Redirect stream only if we didn't do it already.
					if((*arg == "-rds") && (rds == false)) {
						rds = true;
						std::cout << "Redirecting stream requested." << std::endl;
						if(argcursor + 1 > argc) {
							std::cerr << "Please specify the argument for the -rds command." << std::endl;
							std::cerr << "Ignoring -rds" << std::endl;
						}
						continue;
					}

					// If the user wished to redirect the stream we should do so.
					if((rds == true) && (rdstream == NULL)) {
						// Redirect the stdio to a file.
						rdstream = freopen((*arg).c_str(), "w", stdout);
						if(rdstream == NULL) {
							std::cerr << "Couldn't redirect the output stream." << std::endl;
						} else {
							std::cout << "Stream redirected." << std::endl;
						}
						continue;
					}

					arg++;
				}


				// Create the m_core system.
				if(createCore(&m_core, argc, argv, xml_filename) != ERR_OK) {
					throw "Could not find XdevLCore XML file";
				}

				if(xml_filename != NULL) {


					// Get the window instance.
					m_window = getModule<XdevLWindow*>(getCore(), XdevLID("MyWindow"));
					if (!m_window) {
						throw;
					}

					m_window->create();

					m_cursor = getModule<XdevLCursor*>(getCore(), XdevLID("XdevLCursor"));
					if (!m_cursor) {
						throw;
					}

					// Get the keyboard instance.
					m_keyboard = getModule<XdevLKeyboard*>(getCore(),  XdevLID("MyKeyboard"));
					if (!m_keyboard) {
						throw;
					}

					// Get the mouse instance.
					m_mouse = getModule<XdevLMouse*>(getCore(),  XdevLID("MyMouse"));
					if(!m_mouse) {
						throw;
					}

					// Get the instance of the joystick server module.
					m_joystickServer = getModule<IPXdevLJoystickServer>(getCore(), XdevLID("MyJoystickServer"));
					if(isModuleValid(m_joystickServer)) {
						// Get the instance to the m_keyboard module.
						m_joystick = getModule<XdevLJoystick*>(getCore(), XdevLID("MyJoystick"));
						if(!m_joystick) {
							std::cerr << "NO Joystick found.\n";
						} else {
							if(m_joystickServer->getNumJoysticks() > 0) {
								m_joystick->create(m_joystickServer, xdl::XdevLJoystickId::JOYSTICK_DEFAULT);
							}
						}
					}

				} else {

					//
					// Plug all neccessary plugins.
					//
					getCore()->plug(XdevLPluginName("XdevLWindowSDL-1.0.0"));
					getCore()->plug(XdevLPluginName("XdevLKeyboard-1.0.0"));
					getCore()->plug(XdevLPluginName("XdevLMouse-1.0.0"));
					getCore()->plug(XdevLPluginName("XdevLJoystick-1.0.0"));

					//
					// Create all modules.
					//

					// Create an instance of the windows module.
					m_window = createModule<XdevLWindow*>(getCore(), XdevLModuleName("XdevLWindowDevice"), XdevLID("MyWindow"));
					if(!m_window)
						throw;

					m_cursor = getModule<XdevLCursor*>(getCore(), XdevLID("XdevLCursor"));
					if(!m_cursor)
						throw;

					// Create an instance of the keyboard module.
					m_keyboard = createModule<XdevLKeyboard*>(getCore(), XdevLModuleName("XdevLKeyboard"), XdevLID("MyKeyboard"));
					if(!m_keyboard)
						throw;

					// Create and instance of the mouse module.
					m_mouse = createModule<XdevLMouse*>(getCore(), XdevLModuleName("XdevLMouse"), XdevLID("MyMouse"));
					if(!m_mouse)
						throw;

					// Get the instance of the joystick server module.
					m_joystickServer = createModule<IPXdevLJoystickServer>(getCore(), XdevLModuleName("XdevLJoystickServer"), XdevLID("MyJoystickServer"));
					if(isModuleValid(m_joystickServer)) {
						// Get the instance to the m_keyboard module.
						m_joystick = createModule<XdevLJoystick*>(getCore(), XdevLModuleName("XdevLJoystick"), XdevLID("MyJoystick"));
						if(!m_joystick) {
							std::cerr << "NO Joystick found.\n";
						} else {
							if(m_joystickServer->getNumJoysticks() > 0) {
								m_joystick->create(m_joystickServer, xdl::XdevLJoystickId::JOYSTICK_DEFAULT);
							}
						}
					}
				}

				// Attach the m_keyboard to the m_window.
				if(m_keyboard->attach(m_window) != ERR_OK) {
					throw;
				}

				// Attach the mouse to the m_window.
				if(m_mouse->attach(m_window) != ERR_OK) {
					throw;
				}

			}

			~XdevLApplication() {
				// If the user requested redirection of the output stream
				// we have to free the FILE resource we used to do that.
				if(rdstream != NULL) {
					fclose(rdstream);
				}

				// Destroy the core.
				destroyCore(m_core);
			}

			/// Start the main function.
			void run() throw() {

				// Call the main method.
				this->main(this->m_args);

			}

			/// Override this method to implement your C++ application.
			virtual void main(const Arguments& argv) throw() = 0;

			virtual XdevLModuleDescriptor* getDescriptor() const {
				return NULL;
			}

			virtual const XdevLID& getID() const {
				return m_id;
			}

			XdevLCore* getCore() {
				return m_core;
			}

			XdevLWindow* getWindow() {
				return m_window;
			}

			XdevLCursor* getCursor() {
				return m_cursor;
			}

			XdevLKeyboard* getKeyboard() {
				return m_keyboard;
			}

			XdevLMouse* getMouse() {
				return m_mouse;
			}

			XdevLJoystick* getJoystick() {
				return m_joystick;
			}

		private:

			XdevLCore* 		m_core;
			XdevLWindow* 		m_window;
			XdevLCursor*		m_cursor;
			XdevLKeyboard* 	m_keyboard;
			XdevLMouse*		m_mouse;
			IPXdevLJoystickServer m_joystickServer;
			XdevLJoystick*		m_joystick;

			// Holds the redirected stream file pointer.
			FILE* rdstream;

			// Holds a list of arguments.
			Arguments 	m_args;

			// Holds the argc argument of the main function.
			xdl_int 		m_argc;

			// Holds the argv arguments of the main function.
			xdl_char** 	m_argv;

			XdevLID m_id;
	};

#define XdevLStartMain(APP_NAME, XML_FILENAME)	int main(int argc, char* argv[]) {try {APP_NAME app(argc, argv, xdl::XdevLFileName(XML_FILENAME)); app.run();} catch(...) {std::cerr << "Application: Error occurred.\n";} return 0;}

}

#endif
