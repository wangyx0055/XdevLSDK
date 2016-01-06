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

#include <XdevL.h>
#include <XdevLArrayModifier.h>
#include <tinyxml.h>
#include <XdevLFileSystem/XdevLFileSystem.h>

xdl::IPXdevLCore core = nullptr;
xdl::IPXdevLDirectoryWatcher dirWatcher = nullptr;

xdl::XdevLString directoryToWatch(".");

void watchDirectory(const xdl::XdevLDirectoryWatcher::ItemTypes& types, const xdl::XdevLDirectoryWatcher::EventTypes& event, const xdl::XdevLString& name) {
	xdl::XdevLString tmp("EventType: ");
	switch(event) {
		case xdl::XdevLDirectoryWatcher::EventTypes::DW_OPEN: {
			tmp += xdl::XdevLString("Open");
		}
		break;
		case xdl::XdevLDirectoryWatcher::EventTypes::DW_CREATE: {
			tmp += xdl::XdevLString("Create");
			if(types == xdl::XdevLDirectoryWatcher::ItemTypes::DIRECTORY) {
				xdl::XdevLString tmp2(directoryToWatch);
				tmp2 += name;
				dirWatcher->addDirectoryToWatch(tmp2);
			}
		}
		break;
		case xdl::XdevLDirectoryWatcher::EventTypes::DW_MODIFY: {
			tmp += xdl::XdevLString("Modify");
		}
		break;
		case xdl::XdevLDirectoryWatcher::EventTypes::DW_ACCESS: {
			tmp += xdl::XdevLString("Access");
		}
		break;
		case xdl::XdevLDirectoryWatcher::EventTypes::DW_DELETE: {
			tmp += xdl::XdevLString("Delete");
		}
		break;
		case xdl::XdevLDirectoryWatcher::EventTypes::DW_CLOSE: {
			tmp += xdl::XdevLString("Close");
		}
		break;
		case xdl::XdevLDirectoryWatcher::EventTypes::DW_UNKNOWN:
		default:
			std::cerr << "Unknown DirectoryWatcher event." << std::endl;
		break;
	}

	tmp += xdl::XdevLString(", ");
	tmp += name;
	std::cout << tmp << std::endl;
}

int main(int argc, char** argv) {

	std::cout << XDEVL_ERROR(xdl::ERR_OK, xdl::XdevLString("Nothing special")) << std::endl;

	// Create the core system.
	if(xdl::createCore(&core) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Plug the XdevLFileSystem plugin to the core.
	xdl::plug(core, xdl::XdevLPluginName("XdevLFileSystem"), xdl::XdevLVersion(0, 1, 0));

	// Create a valid instance of the module XdevLFile to read/write files.
	xdl::IPXdevLFile file = xdl::createModule<xdl::IPXdevLFile>(core, xdl::XdevLModuleName("XdevLFile"), xdl::XdevLID("MyFile"));

	// Create a valid instance of the XdevLDirectory module to handle files and folders.
	xdl::IPXdevLDirectory dir = xdl::createModule<xdl::IPXdevLDirectory>(core, xdl::XdevLModuleName("XdevLDirectory"), xdl::XdevLID("MyDirectory"));

	// Create XdevLDirectoryWatcher to watch changes in a directory.
	dirWatcher = xdl::createModule<xdl::IPXdevLDirectoryWatcher>(core, xdl::XdevLModuleName("XdevLDirectoryWatcher"), xdl::XdevLID("MyDirectoryWatcher"));

	// Create and register a function that will be called if directory changes.
	xdl::XdevLDirectoryWatcher::XdevLDirectoryWatcherDelegateType dwd = xdl::XdevLDirectoryWatcher::XdevLDirectoryWatcherDelegateType::Create<&watchDirectory>();
	dirWatcher->registerDelegate(dwd);

	// Add the directory to watch.
	dirWatcher->addDirectoryToWatch(directoryToWatch);

	// Start watching.
	dirWatcher->start();


	dir->make(STRING("Hello"));

	// Create a filename.
	xdl::XdevLFileName filename("test.bin");

	// Open the file for read/write.
	if(file->open(xdl::XdevLOpenForReadWrite(), filename) == xdl::ERR_ERROR) {
		std::cerr << "Could not open file: " << filename << std::endl;
	}

	struct mydata {
		xdl::xdl_int8 value1;
		xdl::xdl_uint32 value2;
		xdl::xdl_uint8 value3;
		xdl::xdl_int16 value4;
	};
	mydata data;

	// Assign a modifier object to the temporary buffer.
	xdl::XdevLArrayModifier arrayModifier((xdl::xdl_uint8*)&data, sizeof(mydata));

	// Write values into that buffer.
	arrayModifier.write<xdl::xdl_int8>(-126);
	arrayModifier.write<xdl::xdl_uint32>(45654);
	arrayModifier.write<xdl::xdl_uint8>(66);
	arrayModifier.write<xdl::xdl_int16>(-15654);

	file->write((xdl::xdl_uint8*)&data, sizeof(mydata));

	data = {0};

	// Reset file cursor back to zero.
	file->seek(xdl::XdevLSeekSet(), 0);

	file->read8(data.value1);
	file->readU32(data.value2);
	file->readU8(data.value3);
	file->read16(data.value4);

	std::cout << (int)data.value1 << " " << data.value2 << " " << (unsigned int)data.value3 << " " << data.value4 << std::endl;

	file->close();
	file->remove(filename);


	std::cout << "\nType 'quit' to stop application.\n\n";

	std::string command;
	while(true) {
		std::cin >> command;
		if(command == "quit") {
			break;
		}
	}

	dirWatcher->stop();

	xdl::destroyCore(core);

	return 0;
}
