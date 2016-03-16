#include "XdevLWindowServerCocoa.h"

namespace xdl {


	//
	// -------------------------------------------------------------------------
	//

	XdevLWindowServerCocoa::XdevLWindowServerCocoa(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& desriptor) :
		XdevLWindowServerImpl(parameter, desriptor) {

	}

	XdevLWindowServerCocoa::~XdevLWindowServerCocoa() {

	}

	xdl_int XdevLWindowServerCocoa::create(XdevLWindow** window, const XdevLWindowAttribute& attribute) {

//		XdevLWindowSDL* sdlWindow = new XdevLWindowSDL(nullptr);
//		sdlWindow->setTitle(title);
//		sdlWindow->setPosition(position);
//		sdlWindow->setSize(size);
//		sdlWindow->setWindowDecoration(xdl_false);
//		sdlWindow->create();
//		*window = sdlWindow;
//		m_windowList[sdlWindow->getWindowID()] = sdlWindow;
		return ERR_OK;
	}

}
