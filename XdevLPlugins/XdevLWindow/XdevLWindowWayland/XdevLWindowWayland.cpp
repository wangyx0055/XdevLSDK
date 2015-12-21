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
#include "XdevLWindowWayland.h"
#include <XdevLInputSystem.h>
#include <XdevLXstring.h>
#include <XdevLUtils.h>
#include <sstream>
#include <cstddef>


xdl::XdevLModuleDescriptor xdl::XdevLWindowWayland::m_moduleDescriptor {
	xdl::windowWaylandVendor,
	xdl::windowWaylandAuthor,
	xdl::windowWaylandModuleNames[xdl::XDEVL_WINDOW_MODULE_NAME],
	xdl::windowWaylandCopyright,
	xdl::windowWaylandDescription,
	xdl::XdevLWindowMajorVersion,
	xdl::XdevLWindowMinorVersion,
	xdl::XdevLWindowPatchVersion
};

xdl::XdevLModuleDescriptor windowEventServerModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_WINDOW_EVENT_SERVER_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLWAYLAND_EVENT_SERVER_MODULE_MAJOR_VERSION,
	XDEVLWAYLAND_EVENT_SERVER_MODULE_MINOR_VERSION,
	XDEVLWAYLAND_EVENT_SERVER_MODULE_PATCH_VERSION
};

xdl::XdevLModuleDescriptor cursorModuleDesc {
	xdl::window_vendor,
	xdl::window_author,
	xdl::window_moduleNames[xdl::XDEVL_CURSOR_MODULE_NAME],
	xdl::window_copyright,
	xdl::windowServerDescription,
	XDEVLWAYLAND_CURSOR_MODULE_MAJOR_VERSION,
	XDEVLWAYLAND_CURSOR_MODULE_MINOR_VERSION,
	XDEVLWAYLAND_CURSOR_MODULE_PATCH_VERSION
};


xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::windowWaylandPluginName,
	xdl::window_moduleNames,
	xdl::XdevLWindowPluginMajorVersion,
	xdl::XdevLWindowPluginMinorVersion,
	xdl::XdevLWindowPluginPatchVersion
};


static xdl::XdevLCursorWayland* waylandCursor = nullptr;
static xdl::XdevLWindowEventServerWayland* waylandEventServer = nullptr;


extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {
	if(xdl::initWayland() == xdl::ERR_ERROR) {
		return xdl::ERR_ERROR;
	}
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {
	xdl::shutdownWayland();
	return xdl::ERR_OK;
}


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {



	if(windowEventServerModuleDesc.getName() == parameter->getModuleName()) {
		if(xdl::windowEventServer == nullptr) {
			xdl::windowEventServer = new xdl::XdevLWindowEventServerWayland(parameter);
			xdl::XdevLWindowEventServerParameter = parameter;
		}
		parameter->setModuleInstance(xdl::windowEventServer);
		return xdl::ERR_OK;

	} else if(xdl::XdevLWindowWayland::m_moduleDescriptor.getName() == parameter->getModuleName()) {
		// If there is not event server first create one.
		if(xdl::windowEventServer == nullptr) {
			// If there is no even server active, create and activate it.
			waylandEventServer = static_cast<xdl::XdevLWindowEventServerWayland*>(parameter->getMediator()->createModule(xdl::XdevLModuleName("XdevLWindowEventServer"), xdl::XdevLID("XdevLWindowEventServer"), xdl::XdevLPluginName("XdevLWindowWayland")));
			xdl::windowEventServer = waylandEventServer;
		}

		xdl::XdevLModule* obj = new xdl::XdevLWindowWayland(parameter);
		if(nullptr == obj) {
			return xdl::ERR_ERROR;
		}

		parameter->setModuleInstance(obj);

		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor()  {
	return &pluginDescriptor;
}

namespace xdl {

	const XdevLID ButtonPressed("XDEVL_BUTTON_PRESSED");
	const XdevLID ButtonReleased("XDEVL_BUTTON_RELEASED");
	const XdevLID MouseButtonPressed("XDEVL_MOUSE_BUTTON_PRESSED");
	const XdevLID MouseButtonReleased("XDEVL_MOUSE_BUTTON_RELEASED");
	const XdevLID MouseMotion("XDEVL_MOUSE_MOTION");
	const XdevLID WindowEvent("XDEVL_WINDOW_EVENT");


	static void shm_format(void *data, struct wl_shm *wl_shm, uint32_t format) {
		XdevLString tmp;
		switch(format) {
			case WL_SHM_FORMAT_ARGB8888:
				tmp = "ARGB8888";
				break;
			case WL_SHM_FORMAT_XRGB8888:
				tmp = "XRGB8888";
				break;
			case WL_SHM_FORMAT_RGB565:
				tmp = "RGB565";
				break;
			default:
				tmp = "other format";
				break;
		}
		std::cout << "Possible shmem format: " << tmp << std::endl;
	}

	// The server checks if the client is alive, we do respond here.
	static void handle_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial) {
		wl_shell_surface_pong(shell_surface, serial);
	}

	// The size of the window got changed, so something.
	static void handle_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height) {
		XdevLWindowWayland* windowWayland = (XdevLWindowWayland*)(data);

		windowWayland->onSizeChanged(width, height);
	}

	static void handle_popup_done(void *data, struct wl_shell_surface *shell_surface) {
	}

	struct wl_shm_listener shm_listener = {
		shm_format
	};

	static const struct wl_shell_surface_listener shell_surface_listener = {
		handle_ping,
		handle_configure,
		handle_popup_done
	};

	void redraw(void *data, struct wl_callback *callback, uint32_t time);

	wl_callback_listener frame_listener = {
		redraw
	};

	void redraw(void *data, struct wl_callback *callback, uint32_t time) {
		XdevLWindowWayland* windowWayland = (XdevLWindowWayland*)(data);

		windowWayland->onPaint();
	}

	static wl_display* display = nullptr;
	static wl_registry* m_registry = nullptr;
	static wl_compositor* m_compositor = nullptr;
	static wl_shell* m_shell = nullptr;
	static wl_seat* m_seat = nullptr;
	static wl_shm* m_sharedMemory = nullptr;

	static void global_registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version) {

		printf("Got a registry event for %s id %d\n", interface, id);
		if(strcmp(interface, "wl_compositor") == 0) {
			if(nullptr == m_compositor) {
				m_compositor = (wl_compositor*)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
			}
		} else if(strcmp(interface, "wl_shell") == 0) {
			if(nullptr == m_shell) {
				m_shell = (wl_shell*)wl_registry_bind(registry, id, &wl_shell_interface, 1);
			}
		} else if(strcmp(interface, "wl_shm") == 0) {
			if(nullptr == m_sharedMemory) {
				m_sharedMemory = (wl_shm*)wl_registry_bind(registry, id, &wl_shm_interface, 1);
				wl_shm_add_listener(m_sharedMemory, &shm_listener, nullptr);
			}
		} else if(strcmp(interface, "wl_seat") == 0) {
			if(m_seat == nullptr) {
				m_seat = (wl_seat*)wl_registry_bind(registry, id, &wl_seat_interface, 1);
				XdevLWindowEventServerWayland::setSeat();
			}
		}
	}

	static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id) {
		printf("Got a registry losing event for %d\n", id);
	}

	static const struct wl_registry_listener registry_listener = {
		global_registry_handler,
		global_registry_remover
	};


	xdl_int initWayland() {

		//
		// Connect to the Wayland server.
		//
		display = wl_display_connect(nullptr);
		if(display == nullptr) {
			std::cerr << "## XdevLWindowWayland::wl_display_connect failed" << std::endl;
			return xdl::ERR_ERROR;
		}

		// We get the registry for the window which holds extension
		// of the wayland server.
		m_registry = wl_display_get_registry(display);
		if(m_registry == nullptr) {
			std::cerr << "## XdevLWindowWayland::wl_display_get_registry failed" << std::endl;
			return ERR_ERROR;
		}

		// Now we tell the registry that we want to listen to events.
		wl_registry_add_listener(m_registry, &registry_listener, nullptr);
		wl_display_dispatch(display);

		return ERR_OK;
	}

	void shutdownWayland() {

		if(m_sharedMemory) {
			wl_shm_destroy(m_sharedMemory);
		}

		if(m_shell) {
			wl_shell_destroy(m_shell);
		}

		if(m_compositor) {
			wl_compositor_destroy(m_compositor);
		}

		if(m_registry) {
			wl_registry_destroy(m_registry);
		}

		if(display != nullptr) {
			wl_display_disconnect(display);
			display = nullptr;
		}
	}



	XdevLWindowWayland::XdevLWindowWayland(XdevLModuleCreateParameter* parameter) :
		XdevLWindowImpl(XdevLWindowImpl::getWindowsCounter(), parameter, m_moduleDescriptor),
		m_surface(NULL),
		m_shellSurface(NULL) {
	}

	void XdevLWindowWayland::setFrameCallback(wl_callback* frameCallback) {
		m_frameCallback = frameCallback;
	}

	wl_surface* XdevLWindowWayland::getSurface() const {
		return m_surface;
	}

	wl_shell_surface* XdevLWindowWayland::getShellSurface() const {
		return m_shellSurface;
	}

	wl_callback* XdevLWindowWayland::getFrameCallback() const {
		return m_frameCallback;
	}

	wl_buffer* XdevLWindowWayland::getBuffer() const {
		return m_buffer;
	}

	xdl_int XdevLWindowWayland::init() {

		XdevLWindowImpl::init();

		wl_display_roundtrip(display);

		if(m_compositor == nullptr) {
			XDEVL_MODULE_ERROR("Wayland compositor object not received.\n");
			return ERR_ERROR;
		} else {
			XDEVL_MODULE_SUCCESS("Received Wayland compositor object proxy\n");
		}

		if(m_shell == nullptr) {
			XDEVL_MODULE_ERROR("Wayland shell object not received.\n");
			return ERR_ERROR;
		} else {
			XDEVL_MODULE_SUCCESS("Received Wayland shell object proxy\n");
		}

		if(m_sharedMemory == nullptr) {
			XDEVL_MODULE_ERROR("Wayland shared memory object not received.\n");
			return ERR_ERROR;
		} else {
			XDEVL_MODULE_SUCCESS("Received Wayland shared memory object proxy\n");
		}
		//
		// Now we create the surface we can see on the screen.
		//
		m_surface = wl_compositor_create_surface(m_compositor);
		if(m_surface == nullptr) {
			XDEVL_MODULE_ERROR("Can't create Wayland surface\n");
			return ERR_ERROR;
		} else {
			XDEVL_MODULE_SUCCESS("Created surface\n");
		}
		wl_surface_set_user_data(m_surface, this);

		m_shellSurface = wl_shell_get_shell_surface(m_shell, m_surface);
		if(m_shellSurface == nullptr) {
			XDEVL_MODULE_ERROR("Can't create Wayland shell surface\n");
			return ERR_ERROR;
		} else {
			XDEVL_MODULE_SUCCESS("Created shell surface\n");
		}
		wl_shell_surface_add_listener(m_shellSurface, &shell_surface_listener, this);

		// Bring the surface to the front.
		wl_shell_surface_set_toplevel(m_shellSurface);


//		m_frameCallback = wl_surface_frame(m_surface);
//		wl_callback_add_listener(m_frameCallback, &frame_listener, this);

		createOpaqueRegion(m_attribute.position.x, m_attribute.position.y, m_attribute.size.width, m_attribute.size.height);

		xdl_int ret = initializeEGL();
		if(ret == ERR_ERROR) {
			XDEVL_MODULE_ERROR("Initializing EGL failed.\n");
			return ERR_ERROR;
		}

		m_egl.m_eglWindow = wl_egl_window_create(m_surface, getWidth(), getHeight());
		if(m_egl.m_eglWindow == EGL_NO_SURFACE) {
			XDEVL_MODULE_ERROR("Can't create egl window\n");
			return ERR_ERROR;
		} else {
			XDEVL_MODULE_SUCCESS("Created egl window\n");
		}

		m_egl.m_eglSurface = eglCreateWindowSurface(m_egl.m_eglDisplay, m_egl.m_eglConfig, m_egl.m_eglWindow, nullptr);

		if(!eglMakeCurrent(m_egl.m_eglDisplay, m_egl.m_eglSurface, m_egl.m_eglSurface, m_egl.m_eglContext)) {
			XDEVL_MODULE_ERROR("eglMakeCurrent failed\n");
		}
		glClearColor((1.0f/255.0)*m_backgroundColor[0], (1.0f/255.0)*m_backgroundColor[1], (1.0f/255.0)*m_backgroundColor[2], (1.0f/255.0)*m_backgroundColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();

		if(!eglSwapBuffers(m_egl.m_eglDisplay, m_egl.m_eglSurface)) {
			XDEVL_MODULE_ERROR("eglSwapBuffers failed\n");
		}

		wl_display_flush(display);

		return ERR_OK;
	}

	xdl_int XdevLWindowWayland::create(const XdevLWindowAttribute& attribute) {
		XdevLWindowImpl::create(attribute);

		return create();
	}

	xdl_int XdevLWindowWayland::create() {

		return ERR_OK;
	}

	void XdevLWindowWayland::createOpaqueRegion(xdl_int x, xdl_int y, xdl_int width, xdl_int height) {
		m_region = wl_compositor_create_region(m_compositor);
		wl_region_add(m_region, x, y, width, height);
		wl_surface_set_opaque_region(m_surface, m_region);
	}

	void* XdevLWindowWayland::getInternal(const XdevLInternalName& id) {
		if(id == XdevLString("WAYLAND_DISPLAY")) {
			return (void*)display;
		} else if(id == XdevLString("WAYLAND_WINDOW")) {
			return (void*)m_egl.m_eglWindow;
		}
		return nullptr;
	}

	xdl_int XdevLWindowWayland::shutdown() {

		if(m_egl.m_eglWindow != nullptr) {
			wl_egl_window_destroy(m_egl.m_eglWindow);
			m_egl.m_eglWindow = nullptr;
		}

		return ERR_OK;
	}

	xdl_int XdevLWindowWayland::update() {

	}

	void XdevLWindowWayland::setSize(const XdevLWindowSize& size) {
		XdevLWindowImpl::setSize(size);

		wl_egl_window_resize(m_egl.m_eglWindow, size.width, size.height, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		eglSwapBuffers(m_egl.m_eglDisplay, m_egl.m_eglSurface);

		wl_region* region = wl_compositor_create_region(m_compositor);
		wl_region_add(region, 0, 0, size.width, size.height);
		wl_surface_set_opaque_region(m_surface, region);
		wl_region_destroy(region);

		wl_surface_commit(m_surface);

	}

	void XdevLWindowWayland::setPosition(const XdevLWindowPosition& position) {
		XdevLWindowImpl::setPosition(position);
	}

	void XdevLWindowWayland::setTitle(const XdevLWindowTitle& title) {
		XdevLWindowImpl::setTitle(title);
		wl_shell_surface_set_title(m_shellSurface, title.toString().c_str());
	}

	void XdevLWindowWayland::setFullscreen(xdl_bool state) {
		m_fullScreen = state;
		if(state) {
			wl_shell_surface_set_fullscreen(m_shellSurface, WL_SHELL_SURFACE_FULLSCREEN_METHOD_SCALE, 0, nullptr);
		} else {
			wl_shell_surface_set_toplevel(m_shellSurface);
		}
		wl_display_flush(display);
	}

	void XdevLWindowWayland::setX(XdevLWindowPosition::type x) {
	}

	void XdevLWindowWayland::setY(XdevLWindowPosition::type y) {
	}

	void XdevLWindowWayland::setWidth(XdevLWindowSize::type width) {
	}

	void XdevLWindowWayland::setHeight(XdevLWindowSize::type height) {
	}

	XdevLWindowPosition::type XdevLWindowWayland::getX() {
		return XdevLWindowImpl::getX();
	}

	XdevLWindowPosition::type XdevLWindowWayland::getY() {
		return XdevLWindowImpl::getY();
	}

	XdevLWindowSize::type XdevLWindowWayland::getWidth() {
		return XdevLWindowImpl::getWidth();
	}

	XdevLWindowSize::type XdevLWindowWayland::getHeight() {
		return XdevLWindowImpl::getHeight();
	}

	const XdevLWindowSize& XdevLWindowWayland::getSize() {
		return XdevLWindowImpl::getSize();
	}

	const XdevLWindowPosition& XdevLWindowWayland::getPosition() {
		return XdevLWindowImpl::getPosition();
	}

	const XdevLWindowTitle& XdevLWindowWayland::getTitle() {
		return XdevLWindowImpl::getTitle();
	}

	xdl_bool  XdevLWindowWayland::getFullscreen() {
		return XdevLWindowImpl::getFullscreen();
	}

	xdl_bool  XdevLWindowWayland::getHidePointer() {
		return XdevLWindowImpl::getHidePointer();
	}

	void  XdevLWindowWayland::show() {
		if(m_fullScreen) {
			wl_shell_surface_set_fullscreen(m_shellSurface, WL_SHELL_SURFACE_FULLSCREEN_METHOD_DEFAULT, 0, nullptr);
		} else {
			wl_shell_surface_set_toplevel(m_shellSurface);
		}
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

	void XdevLWindowWayland::grabPointer() {

	}

	void XdevLWindowWayland::ungrabPointer() {

	}

	void XdevLWindowWayland::setInputFocus() {

	}

	xdl_bool XdevLWindowWayland::hasFocus() {
		return xdl_false;
	}

	void XdevLWindowWayland::setParent(XdevLWindow* window) {
		XdevLWindowImpl::setParent(window);
	}

	void XdevLWindowWayland::setType(XdevLWindowTypes type) {
		XdevLWindowImpl::setType(type);
	}

	void XdevLWindowWayland::onPaint() {
		wl_callback_destroy(m_frameCallback);
		XdevLWindowPosition position = getPosition();
		XdevLWindowSize size = getSize();
		wl_surface_damage(m_surface, position.x, position.y, size.width, size.height);

		paint_pixels();

		m_frameCallback = wl_surface_frame(m_surface);
		wl_surface_attach(m_surface, m_buffer, 0, 0);


		wl_callback_add_listener(m_frameCallback, &frame_listener, this);

		wl_surface_commit(m_surface);
	}

	void XdevLWindowWayland::onSizeChanged(xdl_int width, xdl_int height) {
		XDEVL_MODULE_INFO("Size changed: (width, height): (" << width << ", " << height << ")\n");
		XdevLWindowSize size(width, height);
		setSize(size);

		wl_egl_window_resize(m_egl.m_eglWindow, width, height, 0, 0);

		wl_region *region = wl_compositor_create_region(m_compositor);
		wl_region_add(region, 0, 0, width, height);
		wl_surface_set_opaque_region(m_surface, region);
		wl_region_destroy(region);
	}

	//
	// ---------------------------------------------------------------------------
	//

	xdl_int XdevLWindowWayland::createBuffer() {

		// Couldn't create anonymous file.
		m_fd = createAnonymousFile(getWidth() * getHeight() * 4);
		if(-1 == m_fd) {
			XDEVL_MODULE_ERROR("mmap failed\n");
			return ERR_ERROR;
		}

		// Couldn't map memory.
		m_shm_data = (xdl_uint8*)mmap(NULL, getWidth() * getHeight() * 4, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
		if(m_shm_data == MAP_FAILED) {
			XDEVL_MODULE_ERROR("mmap failed\n");
			close(m_fd);
			return ERR_ERROR;
		}

		m_pool = wl_shm_create_pool(m_sharedMemory, m_fd, getWidth() * getHeight() * 4);
		m_buffer = wl_shm_pool_create_buffer(m_pool, 0, getWidth(), getHeight(), getWidth() * 4, WL_SHM_FORMAT_XRGB8888);
		wl_shm_pool_destroy(m_pool);

		return ERR_OK;
	}


	xdl_int XdevLWindowWayland::initializeEGL() {

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
			EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
			EGL_NONE
		};

		EGLint major, minor, n, count, i, size;


		m_egl.m_eglDisplay = eglGetDisplay((EGLNativeDisplayType)display);
		if(m_egl.m_eglDisplay == EGL_NO_DISPLAY) {
			XDEVL_MODULE_ERROR("Can't create egl display\n");
			return ERR_ERROR;
		} else {
			XDEVL_MODULE_SUCCESS("Created egl display\n");
		}

		EGLBoolean ret = eglInitialize(m_egl.m_eglDisplay, &major, &minor);
		if(ret != EGL_TRUE) {
			XDEVL_MODULE_ERROR("Can't initialize egl display\n");
			return ERR_ERROR;
		}

		ret = eglBindAPI(EGL_OPENGL_ES_API);
		if(ret != EGL_TRUE) {
			XDEVL_MODULE_ERROR("Can't bind egl API to EGL_OPENGL_ES_API\n");
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

		// TODO This is at the moment for only test purpose. As you can see only the first
		// config is used.
		for(i = 0; i < n; i++) {
			eglGetConfigAttrib(m_egl.m_eglDisplay, configs[i], EGL_BUFFER_SIZE, &size);
			printf("Buffer size for config %d is %d\n", i, size);
			// For now just use the first one.
			m_egl.m_eglConfig = configs[0];

		}
		delete [] configs;

		// If we do not found any configs stop here.
		if(m_egl.m_eglConfig == nullptr) {
			return ERR_ERROR;
		}

		m_egl.m_eglContext = eglCreateContext(m_egl.m_eglDisplay, m_egl.m_eglConfig, EGL_NO_CONTEXT, context_attribs);
		if(m_egl.m_eglContext == nullptr) {
			return ERR_ERROR;
		}
	}

	static uint32_t pixel_value = 0x0; // black

	void XdevLWindowWayland::paint_pixels() {
//		int n;
//		uint32_t *pixel = (uint32_t*)m_shm_data;
//
//
//		for(n =0; n < getWidth()*getHeight(); n++) {
//			*pixel++ = pixel_value;
//		}
//
//		// increase each RGB component by one
//		pixel_value += 0x10101;
//
//		// if it's reached 0xffffff (white) reset to zero
//		if(pixel_value > 0xffffff) {
//			pixel_value = 0x0;
//		}
	}

	int XdevLWindowWayland::setCloexecOrClose(int fd) {
		long flags;

		if(fd == -1)
			return -1;

		flags = fcntl(fd, F_GETFD);
		if(flags == -1)
			goto err;

		if(fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)
			goto err;

		return fd;

err:
		close(fd);
		return -1;
	}

	int XdevLWindowWayland::createTmpFileCloExec(char *tmpname) {
		int fd;

#ifdef HAVE_MKOSTEMP
		fd = mkostemp(tmpname, O_CLOEXEC);
		if(fd >= 0)
			unlink(tmpname);
#else
		fd = mkstemp(tmpname);
		if(fd >= 0) {
			fd = setCloexecOrClose(fd);
			unlink(tmpname);
		}
#endif

		return fd;
	}

	xdl_int XdevLWindowWayland::createAnonymousFile(off_t size) {
		static const char tmp[] = "/weston-shared-XXXXXX";
		const char *path;
		char *name;
		int fd;

		path = getenv("XDG_RUNTIME_DIR");
		if(!path) {
			XDEVL_MODULE_ERROR("XDG_RUNTIME_DIR environment variable not found.\n");
			return -1;
		}

		name = (char*)malloc(strlen(path) + sizeof(tmp));
		if(!name) {
			return -1;
		}
		strcpy(name, path);
		strcat(name, tmp);

		fd = createTmpFileCloExec(name);

		free(name);

		if(fd < 0)
			return -1;

		if(ftruncate(fd, size) < 0) {
			close(fd);
			return -1;
		}

		return fd;
	}


//
// -------------------------------------------------------------------------
//

	XdevLWindowServerWayland::XdevLWindowServerWayland(XdevLModuleCreateParameter* parameter) :
		XdevLWindowServerImpl(parameter) {

	}

	XdevLWindowServerWayland::~XdevLWindowServerWayland() {

	}

	xdl_int XdevLWindowServerWayland::createWindow(XdevLWindow** window,
	    const XdevLWindowTitle& title,
	    const XdevLWindowPosition& position,
	    const XdevLWindowSize& size) {

		*window = new XdevLWindowWayland(nullptr);

		return ERR_OK;
	}



//
// XdevLWindowEventServerWayland -----------------------------------------------
//



// Keyboard ----------------------------


#define BTN_MOUSE		0x110
#define BTN_LEFT		0x110
#define BTN_RIGHT		0x111
#define BTN_MIDDLE		0x112
#define BTN_SIDE		0x113
#define BTN_EXTRA		0x114
#define BTN_FORWARD		0x115
#define BTN_BACK		0x116
#define BTN_TASK		0x117

	const int scancodeTable[] = {
		KEY_UNKNOWN,
		/*  1 */    KEY_ESCAPE,
		/*  2 */    KEY_1,
		/*  3 */    KEY_2,
		/*  4 */    KEY_3,
		/*  5 */    KEY_4,
		/*  6 */    KEY_5,
		/*  7 */    KEY_6,
		/*  8 */    KEY_7,
		/*  9 */    KEY_8,
		/*  10 */   KEY_9,
		/*  11 */   KEY_0,
		/*  12 */   KEY_MINUS,
		/*  13 */   KEY_EQUALS,
		/*  14 */   KEY_BACKSPACE,
		/*  15 */   KEY_TAB,
		/*  16 */   KEY_Q,
		/*  17 */   KEY_W,
		/*  18 */   KEY_E,
		/*  19 */   KEY_R,
		/*  20 */   KEY_T,
		/*  21 */   KEY_Y,
		/*  22 */   KEY_U,
		/*  23 */   KEY_I,
		/*  24 */   KEY_O,
		/*  25 */   KEY_P,
		/*  26 */   KEY_LEFTBRACKET,
		/*  27 */   KEY_RIGHTBRACKET,
		/*  28 */   KEY_ENTER,
		/*  29 */   KEY_LCTRL,
		/*  30 */   KEY_A,
		/*  31 */   KEY_S,
		/*  32 */   KEY_D,
		/*  33 */   KEY_F,
		/*  34 */   KEY_G,
		/*  35 */   KEY_H,
		/*  36 */   KEY_J,
		/*  37 */   KEY_K,
		/*  38 */   KEY_L,
		/*  39 */   KEY_SEMICOLON,
		/*  40 */   KEY_APOSTROPHE,
		/*  41 */   KEY_GRAVE,
		/*  42 */   KEY_LSHIFT,
		/*  43 */   KEY_BACKSLASH,
		/*  44 */   KEY_Z,
		/*  45 */   KEY_X,
		/*  46 */   KEY_C,
		/*  47 */   KEY_V,
		/*  48 */   KEY_B,
		/*  49 */   KEY_N,
		/*  50 */   KEY_M,
		/*  51 */   KEY_COMMA,
		/*  52 */   KEY_PERIOD,
		/*  53 */   KEY_SLASH,
		/*  54 */   KEY_RSHIFT,
		/*  55 */   KEY_KP_MULTIPLY,
		/*  56 */   KEY_LALT,
		/*  57 */   KEY_SPACE,
		/*  58 */   KEY_CAPSLOCK,
		/*  59 */   KEY_F1,
		/*  60 */   KEY_F2,
		/*  61 */   KEY_F3,
		/*  62 */   KEY_F4,
		/*  63 */   KEY_F5,
		/*  64 */   KEY_F6,
		/*  65 */   KEY_F7,
		/*  66 */   KEY_F8,
		/*  67 */   KEY_F9,
		/*  68 */   KEY_F10,
		/*  69 */   KEY_NUMLOCK,
		/*  70 */   KEY_SCROLLLOCK,
		/*  71 */   KEY_KP_7,
		/*  72 */   KEY_KP_8,
		/*  73 */   KEY_KP_9,
		/*  74 */   KEY_KP_MINUS,
		/*  75 */   KEY_KP_4,
		/*  76 */   KEY_KP_5,
		/*  77 */   KEY_KP_6,
		/*  78 */   KEY_KP_PLUS,
		/*  79 */   KEY_KP_1,
		/*  80 */   KEY_KP_2,
		/*  81 */   KEY_KP_3,
		/*  82 */   KEY_KP_0,
		/*  83 */   KEY_KP_PERIOD,
		/*  84 */   KEY_SYSREQ,    /* ???? */
		/*  85 */   KEY_MODE,      /* ???? */
		/*  86 */   KEY_NONUSBACKSLASH,
		/*  87 */   KEY_F11,
		/*  88 */   KEY_F12,
		/*  89 */   KEY_UNKNOWN,
		/*  90 */   KEY_UNKNOWN,   /* Katakana */
		/*  91 */   KEY_UNKNOWN,   /* Hiragana */
		/*  92 */   KEY_UNKNOWN,   /* Henkan_Mode */
		/*  93 */   KEY_UNKNOWN,   /* Hiragana_Katakana */
		/*  94 */   KEY_UNKNOWN,   /* Muhenkan */
		/*  95 */   KEY_UNKNOWN,
		/*  96 */   KEY_KP_ENTER,
		/*  97 */   KEY_RCTRL,
		/*  98 */   KEY_KP_DIVIDE,
		/*  99 */   KEY_PRINTSCREEN,
		/* 100 */   KEY_RALT,      /* ISO_Level3_Shift, ALTGR, RALT */
		/* 101 */   KEY_UNKNOWN,   /* Linefeed */
		/* 102 */   KEY_HOME,
		/* 103 */   KEY_UP,
		/* 104 */   KEY_PAGEUP,
		/* 105 */   KEY_LEFT,
		/* 106 */   KEY_RIGHT,
		/* 107 */   KEY_END,
		/* 108 */   KEY_DOWN,
		/* 109 */   KEY_PAGEDOWN,
		/* 110 */   KEY_INSERT,
		/* 111 */   KEY_DELETE,
		/* 112 */   KEY_UNKNOWN,
		/* 113 */   KEY_MUTE,
		/* 114 */   KEY_VOLUMEDOWN,
		/* 115 */   KEY_VOLUMEUP,
		/* 116 */   KEY_POWER,
		/* 117 */   KEY_KP_EQUALS,
		/* 118 */   KEY_UNKNOWN,   /* plusminus */
		/* 119 */   KEY_PAUSE,
		/* 120 */   KEY_UNKNOWN,   /* XF86LaunchA */
		/* 121 */   KEY_UNKNOWN,   /* KP_Decimal */
		/* 122 */   KEY_UNKNOWN,   /* Hangul */
		/* 123 */   KEY_UNKNOWN,   /* Hangul_Hanja */
		/* 124 */   KEY_UNKNOWN,
		/* 125 */   KEY_LGUI,
		/* 126 */   KEY_RGUI,
		/* 127 */   KEY_APPLICATION,
		/* 128 */   KEY_CANCEL,
		/* 129 */   KEY_AGAIN,
		/* 130 */   KEY_UNKNOWN,   /* SunProps */
		/* 131 */   KEY_UNDO,
		/* 132 */   KEY_UNKNOWN,   /* SunFront */
		/* 133 */   KEY_COPY,
		/* 134 */   KEY_UNKNOWN,   /* SunOpen */
		/* 135 */   KEY_PASTE,
		/* 136 */   KEY_FIND,
		/* 137 */   KEY_CUT,
		/* 138 */   KEY_HELP,
		/* 139 */   KEY_UNKNOWN,   /* XF86MenuKB */
		/* 140 */   KEY_CALCULATOR,
		/* 141 */   KEY_UNKNOWN,
		/* 142 */   KEY_SLEEP,
		/* 143 */   KEY_UNKNOWN,   /* XF86WakeUp */
		/* 144 */   KEY_UNKNOWN,   /* XF86Explorer */
		/* 145 */   KEY_UNKNOWN,   /* XF86Send */
		/* 146 */   KEY_UNKNOWN,
		/* 147 */   KEY_UNKNOWN,   /* XF86Xfer */
		/* 148 */   KEY_APP1,      /* XF86Launch1 */
		/* 149 */   KEY_APP2,      /* XF86Launch2 */
		/* 150 */   KEY_WWW,
		/* 151 */   KEY_UNKNOWN,   /* XF86DOS */
		/* 152 */   KEY_UNKNOWN,   /* XF86ScreenSaver */
		/* 153 */   KEY_UNKNOWN,
		/* 154 */   KEY_UNKNOWN,   /* XF86RotateWindows */
		/* 155 */   KEY_MAIL,
		/* 156 */   KEY_AC_BOOKMARKS,   /* XF86Favorites */
		/* 157 */   KEY_COMPUTER,
		/* 158 */   KEY_AC_BACK,
		/* 159 */   KEY_AC_FORWARD,
		/* 160 */   KEY_UNKNOWN,
		/* 161 */   KEY_EJECT,
		/* 162 */   KEY_EJECT,
		/* 163 */   KEY_AUDIONEXT,
		/* 164 */   KEY_AUDIOPLAY,
		/* 165 */   KEY_AUDIOPREV,
		/* 166 */   KEY_AUDIOSTOP,
		/* 167 */   KEY_UNKNOWN,   /* XF86AudioRecord */
		/* 168 */   KEY_UNKNOWN,   /* XF86AudioRewind */
		/* 169 */   KEY_UNKNOWN,   /* XF86Phone */
		/* 170 */   KEY_UNKNOWN,
		/* 171 */   KEY_F13,       /* XF86Tools */
		/* 172 */   KEY_AC_HOME,
		/* 173 */   KEY_AC_REFRESH,
		/* 174 */   KEY_UNKNOWN,   /* XF86Close */
		/* 175 */   KEY_UNKNOWN,
		/* 176 */   KEY_UNKNOWN,
		/* 177 */   KEY_UNKNOWN,   /* XF86ScrollUp */
		/* 178 */   KEY_UNKNOWN,   /* XF86ScrollDown */
		/* 179 */   KEY_UNKNOWN,   /* parenleft */
		/* 180 */   KEY_UNKNOWN,   /* parenright */
		/* 181 */   KEY_UNKNOWN,   /* XF86New */
		/* 182 */   KEY_AGAIN,
		/* 183 */   KEY_F13,       /* XF86Tools */
		/* 184 */   KEY_F14,       /* XF86Launch5 */
		/* 185 */   KEY_F15,       /* XF86Launch6 */
		/* 186 */   KEY_F16,       /* XF86Launch7 */
		/* 187 */   KEY_F17,       /* XF86Launch8 */
		/* 188 */   KEY_F18,       /* XF86Launch9 */
		/* 189 */   KEY_F19,       /* null keysym */
		/* 190 */   KEY_UNKNOWN,
		/* 191 */   KEY_UNKNOWN,
		/* 192 */   KEY_UNKNOWN,   /* XF86TouchpadToggle */
		/* 193 */   KEY_UNKNOWN,
		/* 194 */   KEY_UNKNOWN,
		/* 195 */   KEY_MODE,
		/* 196 */   KEY_UNKNOWN,
		/* 197 */   KEY_UNKNOWN,
		/* 198 */   KEY_UNKNOWN,
		/* 199 */   KEY_UNKNOWN,
		/* 200 */   KEY_AUDIOPLAY,
		/* 201 */   KEY_UNKNOWN,   /* XF86AudioPause */
		/* 202 */   KEY_UNKNOWN,   /* XF86Launch3 */
		/* 203 */   KEY_UNKNOWN,   /* XF86Launch4 */
		/* 204 */   KEY_UNKNOWN,   /* XF86LaunchB */
		/* 205 */   KEY_UNKNOWN,   /* XF86Suspend */
		/* 206 */   KEY_UNKNOWN,   /* XF86Close */
		/* 207 */   KEY_AUDIOPLAY,
		/* 208 */   KEY_AUDIONEXT,
		/* 209 */   KEY_UNKNOWN,
		/* 210 */   KEY_PRINTSCREEN,
		/* 211 */   KEY_UNKNOWN,
		/* 212 */   KEY_UNKNOWN,   /* XF86WebCam */
		/* 213 */   KEY_UNKNOWN,
		/* 214 */   KEY_UNKNOWN,
		/* 215 */   KEY_MAIL,
		/* 216 */   KEY_UNKNOWN,
		/* 217 */   KEY_AC_SEARCH,
		/* 218 */   KEY_UNKNOWN,
		/* 219 */   KEY_UNKNOWN,   /* XF86Finance */
		/* 220 */   KEY_UNKNOWN,
		/* 221 */   KEY_UNKNOWN,   /* XF86Shop */
		/* 222 */   KEY_UNKNOWN,
		/* 223 */   KEY_STOP,
		/* 224 */   KEY_BRIGHTNESSDOWN,
		/* 225 */   KEY_BRIGHTNESSUP,
		/* 226 */   KEY_MEDIASELECT,
		/* 227 */   KEY_DISPLAYSWITCH,
		/* 228 */   KEY_KBDILLUMTOGGLE,
		/* 229 */   KEY_KBDILLUMDOWN,
		/* 230 */   KEY_KBDILLUMUP,
		/* 231 */   KEY_UNKNOWN,   /* XF86Send */
		/* 232 */   KEY_UNKNOWN,   /* XF86Reply */
		/* 233 */   KEY_UNKNOWN,   /* XF86MailForward */
		/* 234 */   KEY_UNKNOWN,   /* XF86Save */
		/* 235 */   KEY_UNKNOWN,   /* XF86Documents */
		/* 236 */   KEY_UNKNOWN,   /* XF86Battery */
		/* 237 */   KEY_UNKNOWN,   /* XF86Bluetooth */
		/* 238 */   KEY_UNKNOWN,   /* XF86WLAN */
	};

	static void keyboardHandleKeymap(void *data, struct wl_keyboard *keyboard, uint32_t format, int fd, uint32_t size) {

	}

	static void keyboardHandleEnter(void *data, struct wl_keyboard *keyboard,  uint32_t serial, struct wl_surface *surface, struct wl_array *keys) {
		XdevLWindowEventServerWayland* wes = (XdevLWindowEventServerWayland*)data;
		XdevLWindowWayland* window = (XdevLWindowWayland*)wl_surface_get_user_data(surface);
		wes->setFocusWindow(window);

		XdevLEvent ev;
		ev.common.timestamp = wes->getMediator()->getTimer().getTime64();
		ev.type				= XDEVL_WINDOW_EVENT;
		ev.window.event 	= XDEVL_WINDOW_INPUT_FOCUS_GAINED;
		ev.window.windowid	= window->getWindowID();

		wes->getMediator()->fireEvent(ev);
	}

	static void keyboardHandleLeave(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface) {
		XdevLWindowEventServerWayland* wes = (XdevLWindowEventServerWayland*)data;

		XdevLEvent ev;
		ev.common.timestamp = wes->getMediator()->getTimer().getTime64();
		ev.type				= XDEVL_WINDOW_EVENT;
		ev.window.event 	= XDEVL_WINDOW_INPUT_FOCUS_LOST;
		ev.window.windowid	= wes->getFocusWindow()->getWindowID();

		wes->getMediator()->fireEvent(ev);
	}

	static void keyboardHandleKey(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state_w) {
		XdevLWindowEventServerWayland* wes = (XdevLWindowEventServerWayland*)data;

		XdevLEvent ev;
		ev.common.timestamp 	= wes->getMediator()->getTimer().getTime64();
		ev.type 				= state_w ? ButtonPressed.getHashCode() : ButtonReleased.getHashCode();
		ev.window.windowid		= wes->getFocusWindow()->getWindowID();
//		ev.key.repeat 			= repeat;							// Is repeat on or off?
		ev.key.keycode			= scancodeTable[key];								// Which key button state has changed.
//		ev.key.mod 				= modstate;							// Save the modifier keys.

		wes->getMediator()->fireEvent(ev);
	}

	static void keyboardHandleModifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group) {

	}

	static const struct wl_keyboard_listener keyboardListener = {
		keyboardHandleKeymap,
		keyboardHandleEnter,
		keyboardHandleLeave,
		keyboardHandleKey,
		keyboardHandleModifiers,
	};

// Pointer -----------------------------

	static void pointerHandleEnter(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface, wl_fixed_t sx_w, wl_fixed_t sy_w) {
		XdevLWindowEventServerWayland* wes = (XdevLWindowEventServerWayland*)data;
		wes->setCurrentWindow(surface);

		XdevLWindowWayland* window = (XdevLWindowWayland*)wl_surface_get_user_data(surface);

		XdevLEvent ev;
		ev.common.timestamp = wes->getMediator()->getTimer().getTime64();
		ev.type				= XDEVL_WINDOW_EVENT;
		ev.window.event 	= XDEVL_WINDOW_ENTER;
		ev.window.data1		= sx_w;
		ev.window.data2		= sy_w;
		ev.window.windowid	= window->getWindowID();

		wes->getMediator()->fireEvent(ev);

	}
	static void pointerHandleLeave(void *data, struct wl_pointer *pointer, uint32_t serial, struct wl_surface *surface) {
		XdevLWindowEventServerWayland* wes = (XdevLWindowEventServerWayland*)data;
		wes->setCurrentWindow(nullptr);

		XdevLWindowWayland* window = (XdevLWindowWayland*)wl_surface_get_user_data(surface);

		XdevLEvent ev;
		ev.common.timestamp = wes->getMediator()->getTimer().getTime64();
		ev.type				= XDEVL_WINDOW_EVENT;
		ev.window.event 	= XDEVL_WINDOW_LEAVE;

		xdl_int x, y;
		wes->getCurrentPointerPosition(x, y);

		ev.window.data1	= x;
		ev.window.data2	= y;

		ev.window.windowid	= window->getWindowID();

		wes->getMediator()->fireEvent(ev);
	}
	static void pointerHandleMotion(void *data, struct wl_pointer *pointer, uint32_t time, wl_fixed_t sx_w, wl_fixed_t sy_w) {
		XdevLWindowEventServerWayland* wes = (XdevLWindowEventServerWayland*)data;

		XdevLEvent ev;
		ev.common.timestamp 	= wes->getMediator()->getTimer().getTime64();
		ev.type 				= MouseMotion.getHashCode();
		ev.motion.x				= wl_fixed_to_int(sx_w);
		ev.motion.y				= wl_fixed_to_int(sy_w);

		XdevLWindowWayland* window = (XdevLWindowWayland*)wl_surface_get_user_data(wes->getCurrentWindow());
		ev.window.windowid		= window->getWindowID();

		wes->getMediator()->fireEvent(ev);
		wes->setCurrentPointerPosition(ev.motion.x, ev.motion.y);

	}
	static void pointerHandleButton(void *data, struct wl_pointer *pointer, uint32_t serial, uint32_t time, uint32_t button, uint32_t state_w) {
		XdevLWindowEventServerWayland* wes = (XdevLWindowEventServerWayland*)data;
		XdevLWindowWayland* window = (XdevLWindowWayland*)wl_surface_get_user_data(wes->getCurrentWindow());

		//
		// TODO For now we just allow to move the window just with a button press.
		//
		if( (button == BTN_RIGHT) && (state_w == WL_POINTER_BUTTON_STATE_PRESSED)) {
			wl_shell_surface_move(window->getNativeShellSurface(), m_seat, serial);
		} else if( (button == BTN_MIDDLE) && (state_w == WL_POINTER_BUTTON_STATE_PRESSED)) {
			wl_shell_surface_resize(window->getNativeShellSurface(), m_seat, serial, 0);
		}



		//
		// Set which button we use.
		//
		XdevLEvent ev;
		xdl_uint32 state = state_w;
		switch (button) {
			case BTN_LEFT:
				ev.button.button = BUTTON_LEFT;
				break;
			case BTN_MIDDLE:
				ev.button.button = BUTTON_MIDDLE;
				break;
			case BTN_RIGHT:
				ev.button.button = BUTTON_RIGHT;
				break;
			case BTN_SIDE:
				ev.button.button = BUTTON_SIDE;
				break;
			case BTN_EXTRA:
				ev.button.button = BUTTON_EXTRA;
				break;
			default:
				return;
		}


		ev.common.timestamp = wes->getMediator()->getTimer().getTime64();
		ev.type = state ? MouseButtonPressed.getHashCode() : MouseButtonReleased.getHashCode();

		xdl_int x, y;
		wes->getCurrentPointerPosition(x, y);

		ev.button.x				= x;
		ev.button.y				= y;
		ev.window.windowid		= window->getWindowID();

		wes->getMediator()->fireEvent(ev);

	}
	static void pointerHandleAxis(void *data, struct wl_pointer *pointer, uint32_t time, uint32_t axis, wl_fixed_t value) {
		// TODO
	}

	static const struct wl_pointer_listener pointerListener = {
		pointerHandleEnter,
		pointerHandleLeave,
		pointerHandleMotion,
		pointerHandleButton,
		pointerHandleAxis,
	};

	static void seatHandleCapabilities(void *data, wl_seat* seat, uint32_t caps) {

		if(caps & WL_SEAT_CAPABILITY_POINTER) {
			waylandEventServer->setPointer(wl_seat_get_pointer(seat));
		}

		if( caps & WL_SEAT_CAPABILITY_KEYBOARD) {
			waylandEventServer->setKeyboard(wl_seat_get_keyboard(seat));
		}

		if( caps & WL_SEAT_CAPABILITY_TOUCH) {

		}
	}

	static const wl_seat_listener seatListener = {
		seatHandleCapabilities
	};

	XdevLWindowEventServerWayland::XdevLWindowEventServerWayland(XdevLModuleCreateParameter* parameter) :
		XdevLWindowEventServerImpl(parameter, windowEventServerModuleDesc),
		m_pointer(nullptr),
		m_currentSurface(nullptr),
		m_pointerPositionX(0),
		m_pointerPositionY(0) {
	}

	XdevLWindowEventServerWayland::~XdevLWindowEventServerWayland() {
	}

	xdl_int XdevLWindowEventServerWayland::registerWindowForEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::registerWindowForEvents(window);
	}

	xdl_int XdevLWindowEventServerWayland::unregisterWindowFromEvents(XdevLWindow* window) {
		return XdevLWindowEventServerImpl::unregisterWindowFromEvents(window);
	}

	xdl_int XdevLWindowEventServerWayland::init() {
		return ERR_OK;
	}

	void* XdevLWindowEventServerWayland::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLWindowEventServerWayland::shutdown() {
		if(m_pointer) {
			wl_pointer_destroy(m_pointer);
		}
		return ERR_OK;
	}

	xdl_int XdevLWindowEventServerWayland::update() {

		while(wl_display_prepare_read(display) < 0) {
			wl_display_dispatch_pending(display);
		}
		wl_display_flush(display);
		wl_display_read_events(display);
		return ERR_OK;
	}

	void XdevLWindowEventServerWayland::flush() {
	}

	void XdevLWindowEventServerWayland::setSeat() {
		wl_seat_add_listener(m_seat, &seatListener, windowEventServer);
		wl_seat_set_user_data(m_seat, windowEventServer);
	}
	void XdevLWindowEventServerWayland::setPointer(wl_pointer* pointer) {
		m_pointer = pointer;
		wl_pointer_set_user_data(pointer, this);
		wl_pointer_add_listener(pointer, &pointerListener, this);
	}

	void XdevLWindowEventServerWayland::setKeyboard(wl_keyboard* keyboard) {
		m_keyboard = keyboard;
		wl_keyboard_set_user_data(keyboard, this);
		wl_keyboard_add_listener(keyboard, &keyboardListener, this);
	}

	void XdevLWindowEventServerWayland::setCurrentWindow(wl_surface* surface) {
		m_currentSurface = surface;
	}

	void XdevLWindowEventServerWayland::setFocusWindow(XdevLWindow* window) {
		m_focusWindow = window;
	}

	XdevLWindow* XdevLWindowEventServerWayland::getFocusWindow() {
		return m_focusWindow;
	}

	void XdevLWindowEventServerWayland::setCurrentPointerPosition(xdl_int x, xdl_int y) {
		m_pointerPositionX = x;
		m_pointerPositionY = y;
	}

	void XdevLWindowEventServerWayland::getCurrentPointerPosition(xdl_int& x, xdl_int& y) {
		x = m_pointerPositionX;
		y = m_pointerPositionY;
	}


//
// XdevLCursorWayland ----------------------------------------------------------
//

	XdevLCursorWayland::XdevLCursorWayland(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLCursor>(parameter, cursorModuleDesc) {
	}

	XdevLCursorWayland::~XdevLCursorWayland() {
	}

	xdl_int XdevLCursorWayland::init() {
		return ERR_OK;
	}

	xdl_int XdevLCursorWayland::shutdown() {
		return ERR_OK;
	}

	void* XdevLCursorWayland::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLCursorWayland::attach(XdevLWindow* window) {
		return ERR_OK;
	}

	void XdevLCursorWayland::show() {
	}

	void XdevLCursorWayland::hide() {
	}

	void XdevLCursorWayland::setPosition(xdl_uint x, xdl_uint y) {
	}

	xdl_int XdevLCursorWayland::clip(xdl_uint x1, xdl_uint y1, xdl_uint x2, xdl_uint y2) {
		return ERR_OK;
	}

	void XdevLCursorWayland::releaseClip() {
	}

	xdl_int XdevLCursorWayland::enableRelativeMotion() {
		return ERR_OK;
	}

	void XdevLCursorWayland::disableRelativeMotion() {

	}

}
