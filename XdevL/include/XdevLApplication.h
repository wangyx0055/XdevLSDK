/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2015 Cengiz Terzibas

	This library is free software; you can redistribute it and/or modify it under the
	terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2.1 of the License, or (at your option) any later version.
	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
	See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
	Suite 330, Boston, MA 02111-1307 USA

	I would appreciate if you report all bugs to: cengiz@terzibas.de
*/

#ifndef XDEVL_APPLICATION_H
#define XDEVL_APPLICATION_H

#include <XdevL.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLMouse/XdevLMouse.h>
#include <XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLJoystick/XdevLJoystick.h>

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
					m_window = xdl::getModule<xdl::XdevLWindow*>(getCore(), xdl::XdevLID("MyWindow"));
					if (!m_window) {
						throw;
					}
					
					m_window->create();

					m_cursor = xdl::getModule<xdl::XdevLCursor*>(getCore(), XdevLID("XdevLCursor"));
					if (!m_cursor) {
						throw;
					}

					// Get the keyboard instance.
					m_keyboard = xdl::getModule<xdl::XdevLKeyboard*>(getCore(),  xdl::XdevLID("MyKeyboard"));
					if (!m_keyboard) {
						throw;
					}

					// Get the mouse instance.
					m_mouse = xdl::getModule<xdl::XdevLMouse*>(getCore(),  xdl::XdevLID("MyMouse"));
					if(!m_mouse) {
						throw;
					}

					// Get the joystick instance.
					m_joystick = xdl::getModule<xdl::XdevLJoystick*>(getCore(),  xdl::XdevLID("MyJoystick"));
					if(!m_joystick) {
						std::cerr << "NO Joystick found.\n";
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
					m_window = xdl::createModule<xdl::XdevLWindow*>(getCore(), XdevLModuleName("XdevLWindowDevice"), XdevLID("MyWindow"));
					if(!m_window)
						throw;

					m_cursor = xdl::getModule<xdl::XdevLCursor*>(getCore(), XdevLID("XdevLCursor"));
					if(!m_cursor)
						throw;

					// Create an instance of the keyboard module.
					m_keyboard = xdl::createModule<xdl::XdevLKeyboard*>(getCore(), XdevLModuleName("XdevLKeyboard"), XdevLID("MyKeyboard"));
					if(!m_keyboard)
						throw;

					// Create and instance of the mouse module.
					m_mouse = xdl::createModule<xdl::XdevLMouse*>(getCore(), XdevLModuleName("XdevLMouse"), XdevLID("MyMouse"));
					if(!m_mouse)
						throw;

					// Get the instance to the m_keyboard module.
					m_joystick = xdl::createModule<xdl::XdevLJoystick*>(getCore(), XdevLModuleName("XdevLJoystick"), XdevLID("MyJoystick"));
					if(!m_joystick) {
						std::cerr << "NO Joystick found.\n";
					}
				}

				// Attach the m_keyboard to the m_window.
				if(m_keyboard->attach(m_window) != xdl::ERR_OK) {
					throw;
				}

				// Attach the mouse to the m_window.
				if(m_mouse->attach(m_window) != xdl::ERR_OK) {
					throw;
				}

				// Attach the joystick to the m_window.
				if((m_joystick != NULL) && m_joystick->attach(m_window) != xdl::ERR_OK) {
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

			xdl::XdevLMouse* getMouse() {
				return m_mouse;
			}

			xdl::XdevLJoystick* getJoystick() {
				return m_joystick;
			}

		private:

			xdl::XdevLCore* 		m_core;
			xdl::XdevLWindow* 		m_window;
			xdl::XdevLCursor*		m_cursor;
			xdl::XdevLKeyboard* 	m_keyboard;
			xdl::XdevLMouse*		m_mouse;
			xdl::XdevLJoystick*		m_joystick;

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
