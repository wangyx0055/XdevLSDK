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

#include "XdevLX11Environment.h"

extern xdl::XdevLModuleDescriptor windowX11DisplayDesc;
extern xdl::XdevLModuleDescriptor windowEventServerX11Desc;
extern xdl::XdevLModuleDescriptor cursorX11Desc;

namespace xdl {

	std::shared_ptr<XdevLX11Environment> globalX11Display;

	XdevLX11Environment::XdevLX11Environment(XdevLCoreMediator* core) : m_core(core) {
		std::cout << "XdevLX11Environment::XdevLX11Environment()\n";

		XdevLModuleCreateParameter parameter;

		parameter.setModuleId(XdevLID("XdevLDisplay"));
		display = std::make_shared<XdevLDisplayX11>(&parameter, windowX11DisplayDesc);

		parameter.setModuleId(XdevLID("XdevLWindowEventServer"));
		windowEventServer = std::make_shared<XdevLWindowEventServerX11>(&parameter, windowEventServerX11Desc);

		parameter.setModuleId(XdevLID("XdevLCursor"));
		cursor = std::make_shared<XdevLCursorX11>(&parameter, cursorX11Desc);

		// Register within the Core if this is using one.
		if(m_core) {
			m_core->registerModule(display);
			m_core->registerModule(windowEventServer);
			m_core->registerModule(cursor);
		}
	}


	XdevLX11Environment::~XdevLX11Environment() {
		std::cout << "XdevLX11Environment::~XdevLX11Environment()\n";
		if(m_core) {
			m_core->deleteModule(windowEventServer->getID());
			m_core->deleteModule(cursor->getID());
			m_core->deleteModule(display->getID());
		}
	}
}
