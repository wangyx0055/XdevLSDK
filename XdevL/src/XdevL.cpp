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

#include <tinyxml.h>
#include "XdevL.h"
#include "XdevLCore.h"
#include "XdevLDynLib.h"
#include "XdevLUtils.h"
#include "XdevLCommandLine.h"
#include "XdevLError.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//
// The procedure of XdevL looking for the main XdevLCore plugin.
//
// 1. If use specified a

namespace xdl {

	XdevLVersion XdevLVersion::zero {0,0,0};

	// Typedef for the command line arguments.
	typedef std::vector<std::string> Arguments;

	// Typedefs for creating and deleting the XdevLCore object.
	typedef void (*DELETE_XDEVL_CORE)(XdevLModule* core);

	typedef xdl::XdevLModule*(*CREATE_XDEVL_MODULE)(const XdevLPluginDescriptor& pluginDescriptor, const XdevLModuleDescriptor& moduleDescriptor);
	typedef void (*DELETE_XDEVL_MODULE)(xdl::XdevLModule* obj);


	// Function pointer to the create function of the XdevLCore module.
	XdevLCreateModuleFunction create_xdevl_core = nullptr;

	// Function pointer to the delete function of the XdevLCore module.
	DELETE_XDEVL_CORE delete_xdevL_core = nullptr;

	// Holds the main XdevLCore XML filename.
	static std::string xml_file("");

	// Holds the environmanth path value for the XdevLCore plugin.
	static std::string xdevlcore_filename_environment_path;

	// Holds the version of XdevLCore.

	static std::string version_number;

	// Holds the XdevLCore plugins filename.
	static std::string xdevlcore_filename;

	// Holds the path to the XdevLCore plugin.
	static std::string xdevl_plugin_path;

	static xdl_bool verbose = false;

	std::ofstream file;

	// Is used to load shared objects dynamically.
	XdevLSharedLibrary 			dynamicLibraryLoader;

	// Holds the command line for the XdevLCore plugin.
	XdevLCommandLineParser* cmdl 	= nullptr;

	//
	// Check the local path for the XdevLCore plugin.
	//
	xdl_int checkLocal(std::string& pluginFilename) {


		// Make platform dependent fileformat.
		xstd::make_path(pluginFilename);
		xstd::make_path(xdevl_plugin_path);

		if(verbose) {
			std::cout << ">> XdevLCore plugins search information: " << std::endl;
			std::cout << ">> Version   : " << version_number << std::endl;
			std::cout << ">> Directory : " << xdevl_plugin_path << std::endl;
			std::cout << ">> Trying to load ... ";
		}
		// Let's look if the XdevLCore plugin is in the local folder or the specified
		// by the user using command line or xml file.

		std::ifstream fs;

		fs.open(pluginFilename.c_str());
		if(fs.is_open()) {
			fs.close();
			if(dynamicLibraryLoader.open(pluginFilename.c_str()) != 0) {
				std::cerr << "Could not open library : " << pluginFilename << std::endl;
				exit(-1);
			} else {
				if(verbose) {
					std::cout << "Found and loaded.\n";
				}
			}
		} else {
			// It was not at the specified path. Let's check if the
			// 'XDEVL_PLUGINS' path is set as a environment variable.
			if(verbose) {
				std::cout << "Not found.\n";

				std::cout << ">> Checking if XDEVL_PLUGINS environment variable is set ... ";
			}

			if(getenv("XDEVL_PLUGINS") == nullptr) {
				std::cerr << "It is not set. Can not find: " << pluginFilename << std::endl;
				exit(-1);
			}

			std::string evnironment_variable {getenv("XDEVL_PLUGINS")};
			if(evnironment_variable.size() == 0) {
				std::cerr << "It is not set. Can not find: " <<  pluginFilename << std::endl;
				exit(-1);
			} else {
				if(verbose) {
					std::cout << "It is set." << std::endl;
				}
			}

			//
			// OK, the XDEVL_PLUGINS environment variable is set.
			// Let's check if the XdevLCore plugins is there.
			//
			if(verbose) {
				std::cout << ">> Trying to load from there ... ";
			}
			xdevl_plugin_path = evnironment_variable + "/lib/";
			pluginFilename = xdevl_plugin_path + xstd::get_pathless(pluginFilename);

			fs.open(pluginFilename.c_str());
			if(fs.is_open()) {
				fs.close();
				// File found in the local path. Than lets load it.
				if(dynamicLibraryLoader.open(pluginFilename.c_str()) != 0) {
					std::cerr << "Could not open plugin: '" << pluginFilename << "'... Abord program." << std::endl;
					exit(-1);
				} else {
					if(verbose) {
						std::cout << "Found and loaded." << std::endl;
					}
				}
			} else {
				std::cerr << "Not found  ... Abord program." << std::endl;
				exit(-1);
			}
		}
		return ERR_OK;
	}


	//
	// Creates the XdevLCore filename.
	//
	void createXdevLCoreFilename(const std::string& path, const std::string& version, std::string& result) {
		result = path + "/" + "XdevLCore-";
		result += version;
#ifdef XDEVL_DEBUG
		result += "d";
#endif
		result += XdevLSharedLibrary::extension;
	}

	XdevLCore* createCore(xdl_int argc,
	                      xdl_char* argv[],
	                      const XdevLFileName& xmlFilename,
	                      XdevLUserData* userDataList[],
	                      xdl_uint numberOfUserData) {
		xml_file = xmlFilename;

		std::stringstream vn;
		vn << XDEVL_MAJOR_VERSION << "." << XDEVL_MINOR_VERSION << "." << XDEVL_PATCH_VERSION;
		version_number = vn.str();

		//
		// All command line specified values will override values specified in the
		// xml file or default values. The following values can be overwritten.
		//
		//	- XML filename
		//	- XdevLPlugins plugins path
		// 	- XdevLCore plugins version number
		//

		xdl::XdevLCommandLineParser cmdParser(argc, argv);

		//
		// Set default parameters.
		//
		xdl::XdevLStringParameter versionNumberParameter("v", "The path to the plugins folder.", version_number);
		xdl::XdevLStringParameter pluginsPathParameter("pp", "The path to the plugins folder.", ".");
		xdl::XdevLStringParameter fileNameParameter("f", "The filename of the XML file.", xml_file);

		// Parse the command line arguments.
		cmdParser.add(&versionNumberParameter);
		cmdParser.add(&pluginsPathParameter);
		cmdParser.add(&fileNameParameter);
		cmdParser.parse();

		// Assign the values.
		version_number		= versionNumberParameter.getValue();
		xdevl_plugin_path = pluginsPathParameter.getValue();
		xml_file 					= fileNameParameter.getValue();

		//
		// Check if there is a main XML file where the user may have specified.
		// XdevLCore related information.
		// TODO The not specified comparision must be changed.
		if(xml_file != "Not Specified") {

			// Convert to platform dependent file format.
			xstd::make_path(xml_file);

			TiXmlDocument xmlDocument;
			if(!xmlDocument.LoadFile(xml_file.c_str())) {
				std::cerr << "## Couldn't find specified XML file." << std::endl;
				exit(-1);
			}

			TiXmlHandle docHandle(&xmlDocument);
			TiXmlElement* root = docHandle.FirstChild("XdevLCoreProperties").ToElement();
			if(!root) {
				std::cerr << "## XML format not correct." << std::endl;
				exit(-1);
			}

			//
			// Do we have a version forces to use by the user?
			//
			if(!versionNumberParameter.getSet()) {
				if(root->Attribute("version")) {
					// Use the one from the xml file.
					version_number = root->Attribute("version");
					std::cout << ">> Using version specified in XML file." << std::endl;
				}
			}
			if(!pluginsPathParameter.getSet()) {
				if(root->Attribute("plugins_path")) {
					// Use the one from the xml file.
					xdevl_plugin_path = root->Attribute("plugins_path");
					std::cout << ">> Using plugins folder specified in XML file." << std::endl;
				}
			}
		}

		//
		// Create the filename.
		//
		createXdevLCoreFilename(xdevl_plugin_path, version_number, xdevlcore_filename);

		// Load the XdevLCore plugin.
		xdl_int ret;
		if((ret = checkLocal(xdevlcore_filename)) != ERR_OK) {
			std::cerr << "## Could not find XdevLCore plugin at all." << std::endl;
			exit(-1);
		}


		//
		// Get the plugins create and delte function pointer.
		//
		create_xdevl_core = (XdevLCreateModuleFunction)(dynamicLibraryLoader.getFunctionAddress("_create"));
		if(!create_xdevl_core) {
			std::cerr << "## XdevLCore plugin method format wrong: CREATE_XDEVL_CORE not found." <<  std::endl;
			exit(-1);
		}
		delete_xdevL_core = (DELETE_XDEVL_CORE)(dynamicLibraryLoader.getFunctionAddress("_delete"));
		if(!delete_xdevL_core) {
			std::cerr << "## XdevLCore plugin method format wrong: DELETE_XDEVL_CORE not found." << std::endl;
			exit(-1);
		}

		//
		// Create the XdevLCore object.
		//
	//	cmdl = new xdl::XdevLCommandLineParser(argc, argv);
		XdevLUserData userData;

		userData.id = XdevLID("XDEVL_COMMAND_LINE_PARSER");
		userData.data = (void*)&cmdParser;


		XdevLModuleCreateParameter parameter;
		XdevLID CoreID("XdevLCoreMain");
		parameter.setPluginName(xdl::XdevLPluginName("XdevLCore"));
		parameter.setModuleName(xdl::XdevLModuleName("XdevLCore"));
		parameter.setUserParameter(&userData);
		parameter.setModuleId(CoreID);

		if(create_xdevl_core(&parameter) != ERR_OK) {
			std::cerr << "## Couldn't create XdevLCore object." << std::endl;
			exit(-1);
		}
		XdevLCore* xdevlCoreObject = static_cast<XdevLCore*>(parameter.getModuleInstance());


		XdevLEvent moduleInit;
		moduleInit.type = XDEVL_MODULE_EVENT;
		moduleInit.module.sender = CoreID.getHashCode();
		moduleInit.module.event = XDEVL_MODULE_INIT;
		xdevlCoreObject->notify(moduleInit);

		//
		// Initialize the XdevLCore system.
		//
		XdevLCoreParameters parameters;
		parameters.xmlFileName 			= xml_file;
		parameters.pluginsPath 			= xdevl_plugin_path;
		parameters.userDataList 		= userDataList;
		parameters.numberOfUserData = numberOfUserData;

		if(xdevlCoreObject->setParameters(parameters) != ERR_OK) {
			std::cerr << "## Couldn't initialize XdevLCore object." << std::endl;
			exit(-1);
		}

		return xdevlCoreObject;
	}

//
// Create the XdevLCore system.
//
	xdl_int createCore(XdevLCore** core,
	                   xdl_int argc,
	                   xdl_char* argv[],
	                   const XdevLFileName& xmlFile,
	                   XdevLUserData* userDataList[],
	                   xdl_uint numberOfUserData) {

		*core = createCore(argc, argv, xmlFile, userDataList, numberOfUserData);
		if(*core == nullptr) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}




	XdevLModule* createModule(const XdevLPluginDescriptor& pluginDescriptor, const xdl::XdevLModuleDescriptor& moduleDescriptor) {

		XdevLString pluginName = pluginDescriptor.getName() + STRING("-") + XdevLString(pluginDescriptor.getVersion().toString());
#ifdef XDEVL_DEBUG
		pluginName += STRING("d");
#endif
		pluginName += XdevLSharedLibrary::extension;
		std::string tmp = pluginName.toString();
		xdl_int ret;
		if((ret = checkLocal(tmp)) != ERR_OK) {
			std::cerr << "## Could not find XdevLCore plugin at all." << std::endl;
			exit(-1);
		}

		//
		// Get the plugins create and delte function pointer.
		//
		CREATE_XDEVL_MODULE createModule = (CREATE_XDEVL_MODULE)(dynamicLibraryLoader.getFunctionAddress("_createModule"));
		if(!createModule) {
			return nullptr;
		}
		DELETE_XDEVL_MODULE deleteModule = (DELETE_XDEVL_MODULE)(dynamicLibraryLoader.getFunctionAddress("_delete"));
		if(!deleteModule) {
			return nullptr;
		}

		return createModule(pluginDescriptor, moduleDescriptor);

	}

	xdl_int destroyCore(XdevLCore* core) {
		XDEVL_ASSERT(core, "## Invalid argument!");

		// Create the message.
		xdl::XdevLEvent moduleShutdown;
		moduleShutdown.type = XDEVL_MODULE_EVENT;
		moduleShutdown.module.sender = 0;

		// Send the message to the core.
		core->sendEventTo(core->getID().getHashCode(), moduleShutdown);

		delete_xdevL_core(core);

		return ERR_OK;
	}

}
