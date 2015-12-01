#include <XdevL.h>
#include <tinyxml.h>

std::string projectName("Default");

std::string sourceFile("default");
std::string resourceFile("default");

int main(int argc, char **argv) {

	std::string projectFileName = projectName + ".project";
	std::string sourceFileName = sourceFile + ".cpp";
	std::string resourceFileName = resourceFile + ".xml";

	TiXmlDocument document;


	TiXmlElement* CodeLite_Project = new TiXmlElement("CodeLite_Project");

	CodeLite_Project->SetAttribute("Name", projectName.c_str());
	CodeLite_Project->SetAttribute("InternalType", "Console");

	TiXmlElement* VirtualDirectoryResource = new TiXmlElement("VirtualDirectory");
	VirtualDirectoryResource->SetAttribute("Name", "resource");


	TiXmlElement* ResourceFile = new TiXmlElement("File");
	ResourceFile->SetAttribute("Name", resourceFileName.c_str());

	VirtualDirectoryResource->LinkEndChild(ResourceFile);

	TiXmlElement* VirtualDirectorySource = new TiXmlElement("VirtualDirectory");
	VirtualDirectorySource->SetAttribute("Name", "src");


	TiXmlElement* SourceFile = new TiXmlElement("File");
	SourceFile->SetAttribute("Name", sourceFileName.c_str());

	VirtualDirectorySource->LinkEndChild(SourceFile);



	TiXmlElement* Settings = new TiXmlElement("Settings");
	Settings->SetAttribute("Type", "Executable");

	TiXmlElement* GlobalSettings = new TiXmlElement("GlobalSettings");
	Settings->LinkEndChild(GlobalSettings);

	TiXmlElement* Compiler = new TiXmlElement("Compiler");
	GlobalSettings->LinkEndChild(Compiler);
	Compiler->SetAttribute("Options", "-std=c++11");

	TiXmlElement* IncludePath1 = new TiXmlElement("IncludePath");
	IncludePath1->SetAttribute("Value", ".");
	TiXmlElement* IncludePath2 = new TiXmlElement("IncludePath");
	IncludePath2->SetAttribute("Value", "$(XDEVL_HOME)/include");
	TiXmlElement* IncludePath3 = new TiXmlElement("IncludePath");
	IncludePath3->SetAttribute("Value", "$(XDEVL_PLUGINS)");
	TiXmlElement* IncludePath4 = new TiXmlElement("IncludePath");
	IncludePath4->SetAttribute("Value", "$(XDEVL_EXTERNAL)/include");

	Compiler->LinkEndChild(IncludePath1);
	Compiler->LinkEndChild(IncludePath2);
	Compiler->LinkEndChild(IncludePath3);
	Compiler->LinkEndChild(IncludePath4);


	TiXmlElement* Linker = new TiXmlElement("Linker");
	GlobalSettings->LinkEndChild(Linker);

	TiXmlElement* LibraryPath1 = new TiXmlElement("LibraryPath");
	LibraryPath1->SetAttribute("Value", ".");
	TiXmlElement* LibraryPath2 = new TiXmlElement("LibraryPath");
	LibraryPath2->SetAttribute("Value", "$(XDEVL_EXTERNAL)/lib");

	Linker->LinkEndChild(LibraryPath1);
	Linker->LinkEndChild(LibraryPath2);

	TiXmlElement* ResourceCompiler = new TiXmlElement("ResourceCompiler");
	ResourceCompiler->SetAttribute("Options", "");
	GlobalSettings->LinkEndChild(ResourceCompiler);

	TiXmlElement* Configuration = new TiXmlElement("Configuration");
	Configuration->SetAttribute("Name", "Debug");
	Configuration->SetAttribute("CompilerType", "gnu g++");
	Configuration->SetAttribute("DebuggerType", "GNU gdb debugger");
	Configuration->SetAttribute("Type", "Executable");
	Configuration->SetAttribute("BuildCmpWithGlobalSettings", "append");
	Configuration->SetAttribute("BuildLnkWithGlobalSettings", "append");
	Configuration->SetAttribute("BuildResWithGlobalSettings", "append");
	Settings->LinkEndChild(Configuration);

	TiXmlElement* ConfigurationCompiler = new TiXmlElement("Compiler");
	{
		ConfigurationCompiler->SetAttribute("Options", "-g;-O0;-Wall");
		ConfigurationCompiler->SetAttribute("C_Options", "-g;-O0;-Wall");
		ConfigurationCompiler->SetAttribute("Assembler", "");
		ConfigurationCompiler->SetAttribute("Required", "yes");
		ConfigurationCompiler->SetAttribute("PreCompiledHeader", "");
		ConfigurationCompiler->SetAttribute("PCHInCommandLine", "no");
		ConfigurationCompiler->SetAttribute("PCHFlags", "");
		ConfigurationCompiler->SetAttribute("PCHFlagsPolicy", "0");

		Configuration->LinkEndChild(ConfigurationCompiler);
	}


	{
		TiXmlElement* ConfigurationLinker = new TiXmlElement("Linker");
		ConfigurationLinker->SetAttribute("Options", "");
		ConfigurationLinker->SetAttribute("Required", "yes");
			TiXmlElement* Library1 = new TiXmlElement("Library");
			Library1->SetAttribute("Value", "XdevLd");
			TiXmlElement* Library2 = new TiXmlElement("Library");
			Library2->SetAttribute("Value", "dl");
			ConfigurationLinker->LinkEndChild(Library1);
			ConfigurationLinker->LinkEndChild(Library2);

		Configuration->LinkEndChild(ConfigurationLinker);
	}

	CodeLite_Project->LinkEndChild(VirtualDirectoryResource);
	CodeLite_Project->LinkEndChild(VirtualDirectorySource);
	CodeLite_Project->LinkEndChild(Settings);

	document.LinkEndChild(CodeLite_Project);

	document.SaveFile(projectFileName.c_str());

	return 0;
}
