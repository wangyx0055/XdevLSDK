/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2012 Cengiz Terzibas

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

#include <iostream>
#include <XdevLCore.h>
#include <XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLMouse/XdevLMouse.h>
#include <XdevLJoystick/XdevLJoystick.h>
#include "XdevLWindowWayland.h"
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include <sstream>
#include <cstddef>

xdl::XdevLModuleDescriptor xdl::XdevLWindowWayland::m_moduleDescriptor {
	xdl::windowWaylandVendor,
	xdl::windowWaylandAuthor,
	xdl::windowWaylandModuleNames[0],
	xdl::windowWaylandCopyright,
	xdl::windowWaylandDescription,
	xdl::XdevLWindowMajorVersion,
	xdl::XdevLWindowMinorVersion,
	xdl::XdevLWindowPatchVersion
};

xdl::XdevLPluginDescriptor m_pluginDescriptor {
	xdl::windowWaylandPluginName,
	xdl::window_moduleNames,
	xdl::XdevLWindowPluginMajorVersion,
	xdl::XdevLWindowPluginMinorVersion,
	xdl::XdevLWindowPluginPatchVersion
};

static xdl::xdl_int reference_counter = 0;
static xdl::WaylandDisplay display;

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {

	if(reference_counter == 0) {

		display = wl_display_connect(nullptr);
		if(display == nullptr) {
			return xdl::ERR_ERROR;
		}
	}

	if(xdl::XdevLWindowWayland::m_moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj = new xdl::XdevLWindowWayland(parameter);
		if(nullptr == obj) {
			return xdl::ERR_ERROR;
		}

		parameter->setModuleInstance(obj);

		reference_counter++;

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;

	reference_counter--;

	// Only Quit SDL if this is the last module.
	if(reference_counter == 0) {
		if(display != nullptr) {
			wl_display_disconnect(display);
			display = nullptr;
		}
	}
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor()  {
	return &m_pluginDescriptor;
}

namespace xdl {

	static void global_registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version) {

		XdevLWindowWayland* windowWayland = (XdevLWindowWayland*)(data);

		printf("Got a registry event for %s id %d\n", interface, id);
		if(strcmp(interface, "wl_compositor") == 0) {
			windowWayland->setCompositor((WaylandCompositor)wl_registry_bind(registry, id, &wl_compositor_interface, 1));
		} else if(strcmp(interface, "wl_shell") == 0) {
			windowWayland->setShell((WaylandShell)wl_registry_bind(registry, id, &wl_shell_interface, 1));
		}
	}

	static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id) {
		printf("Got a registry losing event for %d\n", id);
	}

	static const struct wl_registry_listener registry_listener = {
		global_registry_handler,
		global_registry_remover
	};


	XdevLWindowWayland::XdevLWindowWayland(XdevLModuleCreateParameter* parameter) :
		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, m_moduleDescriptor), m_compositor(NULL),
		m_registry(NULL),
		m_surface(NULL),
		m_shell(NULL),
		m_shellSurface(NULL) {}


	xdl_int XdevLWindowWayland::init() {

		XdevLWindowImpl::init();

		//
		// We get the registry for the window which holds extension
		// of the wayland server.
		//
		m_registry = wl_display_get_registry(display);
		if(m_registry == nullptr) {
			XDEVL_MODULE_ERROR("Could not get registry.");
			return ERR_ERROR;
		}

		//
		// Now we tell the registry that we want to listen to events.
		//
		wl_registry_add_listener(m_registry, &registry_listener, this);

		//
		// Sends event to get pointer to wayland extension.
		//
		wl_display_dispatch(display);

		//
		// Wait until we get reply.
		//
		wl_display_roundtrip(display);

		// We now have for sure the Compositor extension. With that
		// we can create a surface (Window).
		m_surface 			= wl_compositor_create_surface(m_compositor);
		wl_surface_set_user_data(m_surface, this);

		//
		// Now to visualize the furface on different devices a shell is used.
		// Let's put the surface into a shell.
		//
		m_shellSurface 	= wl_shell_get_shell_surface(m_shell, m_surface);



		m_eglWindow = wl_egl_window_create(m_surface, getWidth(), getHeight());


		return ERR_OK;
	}

	void* XdevLWindowWayland::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLWindowWayland::shutdown() {
		if(m_eglWindow != nullptr) {
			wl_egl_window_destroy(m_eglWindow);
			m_eglWindow = nullptr;
		}
		return ERR_OK;
	}

	xdl_int XdevLWindowWayland::update() {
		return ERR_OK;
	}

	void XdevLWindowWayland::setSize(const XdevLWindowSize& size) {
		XdevLWindowImpl::setSize(size);
		wl_egl_window_resize(m_eglWindow, size.width, size.height, 0, 0);
	}

	void XdevLWindowWayland::setPosition(const XdevLWindowPosition& position) {
		XdevLWindowImpl::setPosition(position);
	}

	void XdevLWindowWayland::setTitle(const XdevLWindowTitle& title) {
		XdevLWindowImpl::setTitle(title);
	}

	void XdevLWindowWayland::setFullscreen(xdl_bool state) {
		XdevLWindowImpl::setFullscreen(state);
	}

	void XdevLWindowWayland::setX(XdevLWindowPosition::type x) {
	}

	void XdevLWindowWayland::setY(XdevLWindowPosition::type y) {
	}

	void XdevLWindowWayland::setWidth(XdevLWindowSize::type width) {
	}

	void XdevLWindowWayland::setHeight(XdevLWindowSize::type height) {
	}

	void XdevLWindowWayland::showPointer() {
	}

	void XdevLWindowWayland::hidePointer() {
	}

	XdevLWindowPosition::type XdevLWindowWayland::getX() {
		return m_position.x;
	}

	XdevLWindowPosition::type XdevLWindowWayland::getY() {
		return m_position.y;
	}

	XdevLWindowSize::type XdevLWindowWayland::getWidth() {
		return m_size.width;
	}

	XdevLWindowSize::type XdevLWindowWayland::getHeight() {
		return m_size.height;
	}

	const XdevLWindowSize& XdevLWindowWayland::getSize() {
		return m_size;
	}

	const XdevLWindowPosition& XdevLWindowWayland::getPosition() {
		return m_position;
	}

	const XdevLWindowTitle& XdevLWindowWayland::getTitle() {
		return m_title;
	}

	xdl_bool  XdevLWindowWayland::getFullscreen() {
		return XdevLWindowImpl::getFullscreen();
	}

	xdl_int  XdevLWindowWayland::getColorDepth() {
		return XdevLWindowImpl::getColorDepth();
	}

	xdl_bool  XdevLWindowWayland::getHidePointer() {
		return XdevLWindowImpl::getHidePointer();
	}

	void  XdevLWindowWayland::show() {
		wl_shell_surface_set_toplevel(m_shellSurface);
		wl_display_flush(display);
	}

	void XdevLWindowWayland::hide() {
	}

	void XdevLWindowWayland::raise() {

	}

	xdl_bool XdevLWindowWayland::isHidden() {
		return xdl_false;
	}

	void XdevLWindowWayland::setPointerPosition(xdl_uint x, xdl_uint y) {

	}

	void XdevLWindowWayland::clipPointerPosition(xdl_uint x, xdl_uint y, xdl_uint width, xdl_uint height) {

	}

	void XdevLWindowWayland::grabPointer() {

	}

	void XdevLWindowWayland::ungrabPointer() {

	}

	void XdevLWindowWayland::grabKeyboard() {

	}

	void XdevLWindowWayland::ungrabKeyboard() {

	}

	void XdevLWindowWayland::setInputFocus() {

	}

	xdl_bool XdevLWindowWayland::hasFocus() {
		return xdl_false;
	}

	xdl_int XdevLWindowWayland::getInputFocus(XdevLWindow** window) {

	}

	void XdevLWindowWayland::setParent(XdevLWindow* window) {

	}
	void XdevLWindowWayland::SetType(XdevLWindowTypes type) {

		switch(type) {

			case WINDOW_NORMAL: {
			}
			break;
			case WINDOW_TOOLTIP: {
			}
			break;

		}

	}

	//
	// ---------------------------------------------------------------------------
	//

	void XdevLWindowWayland::setCompositor(WaylandCompositor compositor) {
		m_compositor = compositor;
	}

	void XdevLWindowWayland::setShell(WaylandShell shell) {
		m_shell = shell;
	}


	xdl_int XdevLWindowWayland::InitEGL() {
		static const EGLint context_attribs[] = {
			EGL_CONTEXT_CLIENT_VERSION, 2,
			EGL_NONE
		};
		const char *extensions;

		EGLint config_attribs[] = {
			EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			EGL_RED_SIZE, 1,
			EGL_GREEN_SIZE, 1,
			EGL_BLUE_SIZE, 1,
			EGL_ALPHA_SIZE, 1,
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
			EGL_NONE
		};

		EGLint major, minor, n, count, i, size;


		m_egl.m_eglDisplay = eglGetDisplay(display);

		EGLBoolean ret = eglInitialize(m_egl.m_eglDisplay, &major, &minor);
		if(ret != EGL_TRUE) {
			return ERR_ERROR;
		}

		ret = eglBindAPI(EGL_OPENGL_ES_API);
		if(ret != EGL_TRUE) {
			return ERR_ERROR;
		}

		if(!eglGetConfigs(m_egl.m_eglDisplay, nullptr, 0, &count) || count < 1) {
			return ERR_ERROR;
		}

		EGLConfig* configs = new EGLConfig[count];

		ret = eglChooseConfig(m_egl.m_eglDisplay, config_attribs, configs, count, &n);
		if(ret != EGL_TRUE) {
			return ERR_ERROR;
		}

		for(i = 0; i < n; i++) {
			eglGetConfigAttrib(m_egl.m_eglDisplay, configs[i], EGL_BUFFER_SIZE, &size);
			if(m_bufferSize == size) {
				m_egl.m_eglConfig = configs[i];
				break;
			}
		}
		delete [] configs;

		if(m_egl.m_eglConfig == nullptr) {
			return ERR_ERROR;
		}

		m_egl.m_eglContext = eglCreateContext(m_egl.m_eglDisplay, m_egl.m_eglConfig, EGL_NO_CONTEXT, context_attribs);
		if(m_egl.m_eglContext == nullptr) {
			return ERR_ERROR;
		}

//		display->swap_buffers_with_damage = nullptr;
//		extensions = eglQueryString(m_egl.m_eglDisplay, EGL_EXTENSIONS);
//		if (extensions &&
//		    strstr(extensions, "EGL_EXT_swap_buffers_with_damage") &&
//		    strstr(extensions, "EGL_EXT_buffer_age"))
//			display->swap_buffers_with_damage =
//			  (PFNEGLSWAPBUFFERSWITHDAMAGEEXTPROC)
//			  eglGetProcAddress("eglSwapBuffersWithDamageEXT");
//
//		if (display->swap_buffers_with_damage)
//			printf("has EGL_EXT_buffer_age and EGL_EXT_swap_buffers_with_damage\n");

	}

}
