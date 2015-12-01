#include <XdevL.h>
#include <XdevLFileSystem/XdevLFileSystem.h>

#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

//
// Default values for the plugin
//
string pluginPrefix("XdevL");
string pluginSuffix("");
string pluginName("Blank");
string pluginImplementationSuffix("Impl");
//
// Default values for the module
//
string moduleName("Blank");
string modulePrefix("XdevL");
string moduleSuffix("");
string moduleImplementationSuffix("Impl");

string namespaceName("xdl");


void createModuleInterfaceFile(ostream& out, const string& createPluginName, const string& createModuleName) {
	string headerGuard = pluginPrefix + "_" + pluginName + pluginSuffix + "_H";
	transform(headerGuard.begin(), headerGuard.end(),headerGuard.begin(), ::toupper);
	
	out << "#ifndef " << headerGuard << endl;
	out << "#define " << headerGuard << endl << endl;
	
	out << "#include <XdevLModule.h>" << endl << endl;
	
	out << "namespace " << namespaceName << " {" << endl << endl;
	
	out << "\tclass " << createModuleName << " : public " << "XdevLModule" << " { " << endl << endl;
	out << "\tpublic:" << endl << endl;
	out << "\t\tvirtual ~" << createModuleName << "() {};" << endl << endl;
	
	out << "\t};" << endl << endl;
	
	
	out << endl << "}" << endl << endl;
	out << "#endif" << endl;
}

void createModuleImplementationHeaderFile(ostream& out, const string& createPluginName, const string& createModuleName) {
	string headerGuard = pluginPrefix + "_" + pluginName + pluginSuffix + "_" + pluginImplementationSuffix + "_H";
	transform(headerGuard.begin(), headerGuard.end(),headerGuard.begin(), ::toupper);
	
	out << "#ifndef " << headerGuard << endl;
	out << "#define " << headerGuard << endl << endl;
	
	out << "#include <" <<  createPluginName << "/" << createPluginName << ".h>" << endl << endl;
	out << "#include <XdevLPluginImpl.h>" << endl << endl;
	
	out << "namespace " << namespaceName << " {" << endl << endl;
	
	out << "\tstatic const XdevLString vendor {" << endl;
	out << "\t\t\"Vendors Name\"" << endl;
	out << "\t};" << endl << endl;
	out << "\tstatic const XdevLString author {" << endl;
	out << "\t\t\"Authors Name\"" << endl;
	out << "\t};" << endl << endl;
	out << "\tstatic const XdevLString copyright {" << endl;
	out << "\t\t\"Copyright here.\"" << endl;
	out << "\t};" << endl << endl;
	out << "\tstatic const XdevLString pluginName {" << endl;
	out << "\t\t\"" << createPluginName << "\"" << endl;
	out << "\t};" << endl << endl;
	out << "\tstatic const XdevLString moduleDescription {" << endl;
	out << "\t\t\"This is an empty module\"" << endl;
	out << "\t};" << endl << endl;;
	out << "\tstatic std::vector<XdevLModuleName>	moduleNames {" << endl;
	out << "\t\tXdevLModuleName(\"" << createModuleName << "\")," << endl;
	out << "\t};" << endl << endl;
	
	out << "\tclass " << createModuleName +  moduleImplementationSuffix << " : public " << "XdevLModuleImpl<" << createModuleName << "> { " << endl << endl;
	
	out << "\tpublic:" << endl << endl;
	out << "\t\t" << createModuleName + moduleImplementationSuffix << "(XdevLModuleCreateParameter* parameter);" << endl << endl;
	
	out << "\t\tvirtual ~" << createModuleName + moduleImplementationSuffix << "();" << endl << endl;
	
	out << "\t};" << endl << endl;
	
	
	out << endl << "}" << endl << endl;
	out << "#endif" << endl;
}

void createModuleImplementationSourceFile(ostream& out, const string& createPluginName, const string& createModuleName) {
	string headerGuard = pluginPrefix + "_" + pluginName + pluginSuffix + "_" + pluginImplementationSuffix + "_H";
	string pluginImplementationName = createPluginName;
	string moduleImplementationName = createModuleName + moduleImplementationSuffix;
	string moduleNameUpper = createModuleName;

	transform(headerGuard.begin(), headerGuard.end(),headerGuard.begin(), ::toupper);
	transform(pluginImplementationName.begin(), pluginImplementationName.end(),pluginImplementationName.begin(), ::toupper);
	transform(moduleNameUpper.begin(), moduleNameUpper.end(),moduleNameUpper.begin(), ::toupper);

	out << "#include \"" <<  createPluginName + pluginImplementationSuffix << ".h\"" << endl << endl;


	//
	// Add plugin descriptor instance
	//
	out << "xdl::XdevLPluginDescriptor pluginDescriptor {" << endl;
	out << "\txdl::pluginName," << endl;
	out << "\txdl::moduleNames," << endl;
	out << "\t" << pluginImplementationName + "_PLUGIN_MAJOR_VERSION," << endl;
	out << "\t" << pluginImplementationName + "_PLUGIN_MINOR_VERSION," << endl;
	out << "\t" << pluginImplementationName + "_PLUGIN_PATCH_VERSION" << endl;
	out << "};" << endl << endl;

	out << "xdl::XdevLModuleDescriptor moduleDescriptor {" << endl;
	out << "\txdl::vendor," << endl;
	out << "\txdl::author," << endl;
	out << "\txdl::moduleNames[0]," << endl;
	out << "\txdl::copyright," << endl;
	out << "\txdl::moduleDescription," << endl;
	out << "\t" << moduleNameUpper + "_MODULE_MAJOR_VERSION," << endl;
	out << "\t" << moduleNameUpper + "_MODULE_MINOR_VERSION," << endl;
	out << "\t" << moduleNameUpper + "_MODULE_PATCH_VERSION" << endl;
	out << "};" << endl << endl;


	out << "extern \"C\" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {" << endl;
	out << "\tif(moduleDescriptor.getName() == parameter->getModuleName()) {" << endl;
	out << "\t\txdl::XdevLModule* obj  = new xdl::" << moduleImplementationName << "(parameter);" << endl;
	out << "\t\tif(!obj)" << endl;
	out << "\t\t\treturn xdl::ERR_ERROR;" << endl;
	out << "\t\tparameter->setModuleInstance(obj);" << endl;
	out << "\t\treturn xdl::ERR_OK;" << endl;
	out << "\t}" << endl << endl;
	out << "\treturn xdl::ERR_MODULE_NOT_FOUND;" << endl;
	out << "}"  << endl << endl;

	out << "extern \"C\" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {" << endl;
	out << "\tif(obj)" << endl;
	out << "\t\tdelete obj;" << endl;
	out << "}" << endl << endl;

	out << "extern \"C\" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {" << endl;
	out << "\treturn &pluginDescriptor;" << endl;
	out << "}" << endl << endl;


	out << "namespace " << namespaceName << " {" << endl << endl;

	out << "\t\t" << moduleImplementationName << "::" << moduleImplementationName << "(XdevLModuleCreateParameter* parameter) : " << endl;
	out << "\t\t\tXdevLModuleImpl<" << createModuleName << "> (parameter, moduleDescriptor) {" << endl;
	out << "\t\t" << "}" << endl << endl;

	out << "\t\t" << moduleImplementationName << "::~" << moduleImplementationName << "() {}" << endl << endl;

	out << endl << "}" << endl << endl;

}

void createPluginCMakeProjectFile(ostream& out, const string& createPluginName, const string& createModuleName) {
	string definitionName = createPluginName;
	string definitionModuleName = createModuleName;
	transform(definitionName.begin(), definitionName.end(),definitionName.begin(), ::toupper);
	transform(definitionModuleName.begin(), definitionModuleName.end(),definitionModuleName.begin(), ::toupper);

	out << "cmake_minimum_required(VERSION 2.8)" << endl << endl;
	out << "project(" << createPluginName << ")" << endl << endl;

	out << "set(" << createPluginName << "Library " << createPluginName << ")" << endl << endl;

	out << "set(HEADERS_DIR ${XDEVL_PLUGINS}/" + createPluginName + ")" << endl;
	out << "set(SRC_DIR ${XDEVL_PLUGINS}/" + createPluginName + ")" << endl << endl;

	out << "set(" << definitionName << "_PLUGIN_MAJOR_VERSION 0)" << endl;
	out << "set(" << definitionName << "_PLUGIN_MINOR_VERSION 1)" << endl;
	out << "set(" << definitionName << "_PLUGIN_PATCH_VERSION 0)" << endl;
	out << "set(" << definitionName << "_PLUGIN_VERSION \"${" << definitionName << "_PLUGIN_MAJOR_VERSION}.${" << definitionName << "_PLUGIN_MINOR_VERSION}.${" << definitionName << "_PLUGIN_PATCH_VERSION}\")" << endl << endl;

	out << "add_definitions(" << endl;
	out << "	-D" << definitionName << "_PLUGIN_MAJOR_VERSION=${" << definitionName << "_PLUGIN_MAJOR_VERSION}" << endl;
	out << "	-D" << definitionName << "_PLUGIN_MINOR_VERSION=${" << definitionName << "_PLUGIN_MINOR_VERSION}" << endl;
	out << "	-D" << definitionName << "_PLUGIN_PATCH_VERSION=${" << definitionName << "_PLUGIN_PATCH_VERSION}" << endl;
	out << ")" << endl << endl;


	out << "set(" << definitionModuleName << "_MODULE_MAJOR_VERSION 0)" << endl;
	out << "set(" << definitionModuleName << "_MODULE_MINOR_VERSION 1)" << endl;
	out << "set(" << definitionModuleName << "_MODULE_PATCH_VERSION 0)" << endl;
	out << "set(" << definitionModuleName << "_MODULE_VERSION \"${" << definitionModuleName << "_MODULE_MAJOR_VERSION}.${" << definitionModuleName << "_MODULE_MINOR_VERSION}.${" << definitionModuleName << "_MODULE_PATCH_VERSION}\")" << endl << endl;

	out << "add_definitions(" << endl;
	out << "	-D" << definitionModuleName << "_MODULE_MAJOR_VERSION=${" << definitionModuleName << "_MODULE_MAJOR_VERSION}" << endl;
	out << "	-D" << definitionModuleName << "_MODULE_MINOR_VERSION=${" << definitionModuleName << "_MODULE_MINOR_VERSION}" << endl;
	out << "	-D" << definitionModuleName << "_MODULE_PATCH_VERSION=${" << definitionModuleName << "_MODULE_PATCH_VERSION}" << endl;
	out << ")" << endl << endl;


	out << 	
"\
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${XDEVL_PLUGINS}/lib)\n\
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${XDEVL_PLUGINS}/lib)\n\
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)\n\
\n\
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${XDEVL_PLUGINS}/lib)\n\
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG  ${XDEVL_PLUGINS}/lib)\n\
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${XDEVL_PLUGINS}/lib)\n\
\n\
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${XDEVL_PLUGINS}/lib)\n\
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG  ${XDEVL_PLUGINS}/lib)\n\
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${XDEVL_PLUGINS}/lib)\n\
" << endl;

//	out << "set(CMAKE_DEBUG_POSTFIX d)" << endl << endl;

	out << "set(HEADER_INTERFACE_FILE ${HEADERS_DIR}/" << createPluginName << ".h)" << endl << endl;

out << 
"set(HEADER_FILES\n\
	${HEADER_INTERFACE_FILE}\n\
	${HEADERS_DIR}/" << createPluginName + pluginImplementationSuffix << ".h" << endl;
out << ")" << endl << endl;

out << 
"set(SOURCE_FILES\n\
	${SRC_DIR}/" <<createPluginName + pluginImplementationSuffix << ".cpp" << endl;
out << ")" << endl << endl;


	out <<	
"\
include_directories(\n\
	${XDEVL_HOME}/include\n\
	${XDEVL_PLUGINS}\n\
)\n\
\n\
link_directories(\n\
	${XDEVL_HOME}/lib\n\
	${XDEVL_EXTERNALS}/lib\n\
)\
" << endl;
			
	out	<< 
"\
if(PLATFORM_MACOSX)\n\
	set( CMAKE_EXE_LINKER_FLAGS  \"${CMAKE_EXE_LINKER_FLAGS} -F${XDEVL_EXTERNALS}/lib\")\n\
endif()\n\
\n\
if(CMAKE_BUILD_TYPE MATCHES Debug)\n\
	set(LIBS XdevLd)\n\
else()\n\
	set(LIBS XdevL)\n\
endif()\n\
\n\
if(PLATFORM_MACOSX)\n\
	set(LIBS ${LIBS} dl)\n\
elseif(PLATFORM_LINUX)\n\
	set(LIBS ${LIBS} dl)\n\
elseif(PLATFORM_WINDOWS)\n\
\n\
endif()\
" << endl << endl;

out <<
"add_library(${" << createPluginName << "Library" << "} SHARED\n\
    ${HEADER_FILES}\n\
    ${SOURCE_FILES}\n\
)" << endl << endl;

out	<< "target_link_libraries(${" << createPluginName << "Library" << "} ${LIBS})" << endl << endl;

out << "set_property(TARGET ${" << createPluginName << "Library" << "} PROPERTY PREFIX \"\")" << endl << endl;

out << "if(CMAKE_BUILD_TYPE MATCHES Debug)" << endl;
out << "set_property(TARGET ${" << createPluginName << "Library" << "} PROPERTY SUFFIX \"-${" << definitionName << "_PLUGIN_VERSION}d${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}\")" << endl;
out << "else()" << endl;
out << "set_property(TARGET ${" << createPluginName << "Library" << "} PROPERTY SUFFIX \"-${" << definitionName << "_PLUGIN_VERSION}${XDEVL_SHARED_LIBRARY_FILE_EXTENSION}\")" << endl;
out << "endif()" << endl << endl;

out << "install(FILES ${HEADER_INTERFACE_FILE} DESTINATION \"${XDEVL_PLUGINS_INCLUDE_INSTALL_PATH}/${PROJECT_NAME}\")" << endl;
out << "install(TARGETS ${" << createPluginName << "Library" << "} DESTINATION ${XDEVL_PLUGINS_INSTALL_PATH})" << endl << endl;
}

int main(int argc, char** argv) {

	//
	// Check arguments for information from the user.
	//
	vector<string> arguments(argv, argv+argc);
	if(arguments.size() > 0) {

		auto argument(arguments.begin());
		argument++; // Skip first argument which is the name of the program.

		while(argument != arguments.end()) {
			//
			// Did the user specified a plugin name?
			//
			if(*argument == "-p") {
				argument++;
				if(argument != arguments.end() && ((*argument).find_first_of("-") == string::npos)) {
					pluginName = *argument;
				} else {
					cerr << "Invalid parameter for -p" << endl;
					exit(-1);
				}
			} 
			//
			// Did the user specify a module name?
			//
			else if( *argument == "-m") {
				argument++;
				if(argument != arguments.end() && ((*argument).find_first_of("-") == string::npos)) {
					moduleName = *argument;
				} else {
					cerr << "Invalid parameter for -m" << endl;
					exit(-1);
				}
			} else if( *argument == "-h") {
				cout << "XdevLBT V 0.1" << endl;
				cout << "To create plugins for XdevL. Default name of the plugin: 'Blank' module: 'Blank' " << endl << endl;
				cout << "-p : Plugin name" << endl;
				cout << "-m : Module name" << endl;
				exit(0);
			}else {
				argument++;
			}
		}
	}

	//
	// Create neccessary names.
	// 
	string createPluginName = pluginPrefix + pluginName + pluginSuffix;
	string createClassName = modulePrefix + moduleName + moduleSuffix;

	// Try to use XdevLFileSystem plugin to create a folder. If not don't use it
	xdl::XdevLPluginDescriptor pluginDescriptor(xdl::XdevLPluginName("XdevLFileSystem"), xdl::XdevLVersion(0, 1, 0));
	xdl::XdevLModuleDescriptor moduleDescriptor(xdl::XdevLModuleName("XdevLDirectory"), xdl::XdevLVersion(0, 1, 0));
	xdl::XdevLDirectory* directory =  xdl::createModule<xdl::XdevLDirectory*>(pluginDescriptor, moduleDescriptor);
	if(directory) {
		directory->make(xdl::XdevLString(createPluginName));
		directory->changeTo(xdl::XdevLString(createPluginName));
	}

	//
	// Create the files and write into it.
	// 
	ofstream pluginInterfaceHeaderFile(createPluginName + ".h");
	createModuleInterfaceFile(pluginInterfaceHeaderFile, createPluginName, createClassName);
	pluginInterfaceHeaderFile.close();

	ofstream pluginInterfaceImplementationFile(createPluginName + pluginImplementationSuffix + ".h");
	createModuleImplementationHeaderFile(pluginInterfaceImplementationFile, createPluginName, createClassName);
	pluginInterfaceImplementationFile.close();

	ofstream pluginSourceImplementationFile(createPluginName + pluginImplementationSuffix + ".cpp");
	createModuleImplementationSourceFile(pluginSourceImplementationFile, createPluginName, createClassName);
	pluginSourceImplementationFile.close();

	ofstream cmakefile("CMakeLists.txt");
	createPluginCMakeProjectFile(cmakefile, createPluginName, createClassName);
	cmakefile.close();


	return 0;
}
