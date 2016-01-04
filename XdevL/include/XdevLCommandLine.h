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
#ifndef XDEVL_COMMAND_LINE_H
#define XDEVL_COMMAND_LINE_H

#include <XdevLTypes.h>
#include <vector>
#include <string>
#include <map>
#include <sstream>

namespace xdl {

	/**
		@class XdevLParameter
		@brief A command line parameter.
	*/
	class XdevLParameter {
		public:

			enum XdevLParameterType {
			  BOOL_PARAMETER,
			  INT_PARAMETER,
			  STRING_PARAMETER
			};

			XdevLParameter(const std::string& cm, XdevLParameterType type) :
				m_command(cm),
				m_desc("No description"),
				m_type(type),
				m_setFlag(xdl::xdl_false) {
			}

			XdevLParameter(const std::string& cm, const std::string& desc, XdevLParameterType type) :
				m_command(cm),
				m_desc(desc),
				m_type(type),
				m_setFlag(xdl::xdl_false) {
			}

			const std::string& getCommand() {
				return m_command;
			}

			const std::string& getDescription() {
				return m_desc;
			}

			XdevLParameterType getType() {
				return m_type;
			}

			xdl::xdl_bool getSet() {
				return m_setFlag;
			}

		protected:
			std::string					m_command;
			std::string					m_desc;
			XdevLParameterType 	m_type;
			xdl::xdl_bool				m_setFlag;
	};


	class XdevLBoolParameter : public XdevLParameter {
		public:
			XdevLBoolParameter(const std::string& cm, xdl::xdl_bool df) :
				XdevLParameter(cm, BOOL_PARAMETER),
				m_state(df) {
			}
			XdevLBoolParameter(const std::string& cm, const std::string& desc, xdl::xdl_bool df) :
				XdevLParameter(cm, desc, BOOL_PARAMETER),
				m_state(df) {
			}
			xdl::xdl_bool getValue() {
				return m_state;
			}
			void setValue(xdl::xdl_bool state) {
				m_state 	= state;
				m_setFlag = xdl::xdl_true;
			}
		private:
			xdl::xdl_bool m_state;
	};

	class XdevLIntParameter: public XdevLParameter {
		public:
			XdevLIntParameter(const std::string& cm, xdl::xdl_int df) :
				XdevLParameter(cm, INT_PARAMETER),
				m_state(df) {
			}

			XdevLIntParameter(const std::string& cm, const std::string& desc, xdl::xdl_int df) :
				XdevLParameter(cm, desc, INT_PARAMETER),
				m_state(df) {
			}

			xdl::xdl_int getValue() {
				return m_state;
			}

			void setValue(xdl::xdl_int state) {
				m_state = state;
				m_setFlag = xdl::xdl_true;
			}
		private:
			xdl::xdl_int m_state;
	};

	class XdevLStringParameter: public XdevLParameter {
		public:
			XdevLStringParameter(const std::string& cm, const std::string& df) :
				XdevLParameter(cm, STRING_PARAMETER),
				m_state(df) {
			}

			XdevLStringParameter(const std::string& cm, const std::string& desc, const std::string& df) :
				XdevLParameter(cm, desc, STRING_PARAMETER),
				m_state(df) {
			}

			const std::string& getValue() {
				return m_state;
			}

			void setValue(const std::string& state) {
				m_state = state;
				m_setFlag = xdl::xdl_true;
			}

		private:
			std::string m_state;
	};

	/**
		@class XdevLCommandLine
		@brief Class to manage command line arguments.
	*/
	class XdevLCommandLineParser {
		public:

			/// Type for the arguments.
			typedef std::vector<std::string> Arguments;

			XdevLCommandLineParser(xdl_int argc, xdl_char** argv) {
				if((argc != 0) || (argv != NULL)) {
					// Create strings out of the argument vector.
					this->m_args = Arguments(argv, argv+argc);
				}
			}

			void add(XdevLParameter* parameter) {
				assert(parameter && "XdevLCommandLine::add: Not a valid parameter!");
				if(m_parameters.find(parameter->getCommand()) != m_parameters.end()) {
					std::cerr << "XdevLCommandLine::add: Command '" + parameter->getCommand() + "' already defined!" << std::endl;
					exit(-1);
				}
				m_parameters[parameter->getCommand()] = parameter;
			}

			void parse() {

				// Skip if no arguments where passed.
				if(this->m_args.size() == 0) {
					return;
				}

				for(Arguments::iterator argument = this->m_args.begin() + 1; argument != this->m_args.end(); ++argument) {

					//
					// Get the correct registered parameter and assign the values to it.
					//
					std::string arg = *argument;
					arg = arg.substr(arg.find("-") + 1, arg.size());

					//
					// Is it a help command?
					//
					if((arg == "h") || (arg == "help")) {
						std::cout << "\n--- Help Menu ----------------------------------------------------------------\n" << std::endl;

						for(std::map<std::string, XdevLParameter*>::const_iterator parameter = m_parameters.begin();
						    parameter != m_parameters.end();
						    ++parameter) {
							std::cout << "-" << parameter->second->getCommand() << ": " << parameter->second->getDescription() << std::endl;
						}

						std::cout << "\n------------------------------------------------------------------------------\n" << std::endl;

						exit(0);
					}

					XdevLParameter* parameter = m_parameters[arg];
					if(parameter == NULL) {
						continue;
					}

					switch(parameter->getType()) {
						case XdevLParameter::BOOL_PARAMETER: {
							XdevLBoolParameter* sp = static_cast<XdevLBoolParameter*>(parameter);
							argument++;
							std::stringstream ss(*argument);
							xdl::xdl_bool state;
							ss >> state;
							sp->setValue(state);
						}
						break;
						case XdevLParameter::INT_PARAMETER: {
							XdevLIntParameter* sp = static_cast<XdevLIntParameter*>(parameter);
							argument++;
							std::stringstream ss(*argument);
							xdl::xdl_int state;
							ss >> state;
							sp->setValue(state);
						}
						break;
						case XdevLParameter::STRING_PARAMETER: {
							XdevLStringParameter* sp = static_cast<XdevLStringParameter*>(parameter);
							argument++;
							sp->setValue(*argument);
						}
						break;
					}
				}
			}

			/// Returns the number of arguments.
			xdl_int getNumberOfArguments() {
				return this->m_args.size();
			}

			/// Returns a specific argument from the argument list.
			std::string& getArgument(xdl_uint idx) {
				return this->m_args[idx];
			}

			/// Returns the hole list of arguments.
			Arguments& getArguments() {
				return this->m_args;
			}



		private:

			// Holds a list of arguments.
			Arguments 	m_args;

			std::map<std::string, XdevLParameter*> m_parameters;

	};

}

#endif
