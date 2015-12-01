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

	WaylandCallbackListener frame_listener = {
		redraw
	};

	void redraw(void *data, struct wl_callback *callback, uint32_t time) {
		XdevLWindowWayland* windowWayland = (XdevLWindowWayland*)(data);

		windowWayland->onPaint();
	}





	static void global_registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version) {

		XdevLWindowWayland* windowWayland = (XdevLWindowWayland*)(data);

		printf("Got a registry event for %s id %d\n", interface, id);
		if(strcmp(interface, "wl_compositor") == 0) {
			windowWayland->setCompositor((WaylandCompositor)wl_registry_bind(registry, id, &wl_compositor_interface, 1));
		} else if(strcmp(interface, "wl_shell") == 0) {
			windowWayland->setShell((WaylandShell)wl_registry_bind(registry, id, &wl_shell_interface, 1));
		} else if(strcmp(interface, "wl_shm") == 0) {
			windowWayland->setSharedMemory((WaylandSharedMemory)wl_registry_bind(registry, id, &wl_shm_interface, 1));
			wl_shm_add_listener(windowWayland->getSharedMemory(), &shm_listener, NULL);
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

		// We get the registry for the window which holds extension
		// of the wayland server.
		m_registry = wl_display_get_registry(display);
		if(m_registry == nullptr) {
			XDEVL_MODULE_ERROR("Could not get registry.");
			return ERR_ERROR;
		}

		// Now we tell the registry that we want to listen to events.
		wl_registry_add_listener(m_registry, &registry_listener, this);

		// Sends event to get pointer to wayland extension.
		wl_display_dispatch(display);

		// Wait until we get reply.
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

		createOpaqueRegion(m_position.x, m_position.y, m_size.width, m_size.height);

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

		if(eglSwapBuffers(m_egl.m_eglDisplay, m_egl.m_eglSurface)) {
			fprintf(stderr, "Swapped buffers\n");
		} else {
			fprintf(stderr, "Swapped buffers failed\n");
		}

//		wl_surface_attach(m_surface, m_buffer, 0, 0);
//		wl_surface_commit(m_surface);

		wl_display_flush(display);

		return ERR_OK;
	}

	void XdevLWindowWayland::createOpaqueRegion(xdl_int x, xdl_int y, xdl_int width, xdl_int height) {
		m_region = wl_compositor_create_region(m_compositor);
		wl_region_add(m_region, x, y, width, height);
		wl_surface_set_opaque_region(m_surface, m_region);
	}

	void* XdevLWindowWayland::getInternal(const XdevLInternalName& id) {
		return nullptr;
	}

	xdl_int XdevLWindowWayland::shutdown() {

		if(m_egl.m_eglWindow != nullptr) {
			wl_egl_window_destroy(m_egl.m_eglWindow);
			m_egl.m_eglWindow = nullptr;
		}

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

		return ERR_OK;
	}

	xdl_int XdevLWindowWayland::update() {

		wl_surface_commit(m_surface);
		static float red = 0.0;
		static float sign = 1.0;
		glClearColor(red, 0, 0, 1);
		red += sign*0.01;
		if(red <= 0.0f) {
			sign = 1.0f;
		} else if(red >= 1.0f) {
			sign = -1.0f;
		}
		glClear(GL_COLOR_BUFFER_BIT);
		if(!eglSwapBuffers(m_egl.m_eglDisplay, m_egl.m_eglSurface)) {
			XDEVL_MODULE_ERROR("eglSwapBuffers failed\n");
		}

		while(wl_display_prepare_read(display) < 0) {
			wl_display_dispatch_pending(display);
		}
		wl_display_flush(display);
		wl_display_read_events(display);
		wl_display_dispatch_pending(display);

		printf("sdfsdf\n");


	}

	void XdevLWindowWayland::setSize(const XdevLWindowSize& size) {
		m_size = size;

		wl_egl_window_resize(m_egl.m_eglWindow, m_size.width, m_size.height, 0, 0);

		wl_region* region = wl_compositor_create_region(m_compositor);
		wl_region_add(region, 0, 0, size.width, size.height);
		wl_surface_set_opaque_region(m_surface, region);
		wl_region_destroy(region);
	}

	void XdevLWindowWayland::setPosition(const XdevLWindowPosition& position) {
		m_position = position;
	}

	void XdevLWindowWayland::setTitle(const XdevLWindowTitle& title) {
		XdevLWindowImpl::setTitle(title);
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
	void XdevLWindowWayland::setType(XdevLWindowTypes type) {

		switch(type) {

			case WINDOW_NORMAL: {
			}
			break;
			case WINDOW_TOOLTIP: {
			}
			break;

		}
	}

	void XdevLWindowWayland::onPaint() {
		wl_callback_destroy(m_frameCallback);
		wl_surface_damage(m_surface, m_position.x, m_position.y, m_size.width, m_size.height);

		paint_pixels();

		m_frameCallback = wl_surface_frame(m_surface);
		wl_surface_attach(m_surface, m_buffer, 0, 0);


		wl_callback_add_listener(m_frameCallback, &frame_listener, this);

		wl_surface_commit(m_surface);
	}

	void XdevLWindowWayland::onSizeChanged(xdl_int width, xdl_int height) {
		XDEVL_MODULE_INFO("Size changed: (width, height): (" << width << ", " << height << ")\n");
		m_size.width = width;
		m_size.height = height;

		wl_egl_window_resize(m_egl.m_eglWindow, width, height, 0, 0);

		wl_region *region = wl_compositor_create_region(m_compositor);
		wl_region_add(region, 0, 0, width, height);
		wl_surface_set_opaque_region(m_surface, region);
		wl_region_destroy(region);
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

	void XdevLWindowWayland::setSharedMemory(WaylandSharedMemory sharedMemory) {
		m_sharedMemory = sharedMemory;
	}


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
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
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
}
