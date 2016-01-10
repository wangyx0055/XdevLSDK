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

#include <XdevLPlatform.h>
#include "XdevLOgreImpl.h"
#include <XdevLCoreMediator.h>
#include <XdevLPlatform.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLXstring.h>
#include <XdevLXfstring.h>
#include <XdevLUtils.h>

#ifdef XDEVL_PLATFORM_LINUX
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#elif defined(XDEVL_PLATFORM_APPLE)

#endif


xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	xdl::XdevLOgreMajorVersion,
	xdl::XdevLOgreMinorVersion,
	xdl::XdevLOgrePatchVersion
};

xdl::XdevLPluginDescriptor ogrePluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	xdl::XdevLOgrePluginMajorVersion,
	xdl::XdevLOgrePluginMinorVersion,
	xdl::XdevLOgrePluginPatchVersion
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(ogrePluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLOgreImpl, moduleDescriptor)
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {



	XdevLOgreImpl::XdevLOgreImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleImpl<XdevLOgre>(parameter, descriptor),
		m_xdlWindow(nullptr),
		m_ColorDepth(-1),
		m_Antialiasing(-1),
		m_VSync(xdl_false),
		m_colorDepth(32),
		m_gamma(xdl_false),
		m_useNVPerfHUD(xdl_false),
		m_displayFrequency(-1),
		m_root(nullptr),
		m_window(nullptr) {
		m_ogrePluginNames.push_back("RenderSystem_GL");
	}


	XdevLOgreImpl::~XdevLOgreImpl() {

	}


	Ogre::Root* XdevLOgreImpl::getRoot() {
		XDEVL_ASSERT( (m_xdlWindow != nullptr) || (m_root != nullptr), "XdevLOgre is not attached to a window.");
		return m_root;
	}

	Ogre::RenderWindow* XdevLOgreImpl::getWindow() {
		XDEVL_ASSERT( (m_xdlWindow != nullptr), "XdevLOgre is not attached to a window.");
		return m_window;
	}

	xdl_int XdevLOgreImpl::attach(XdevLWindow* window) {
		m_xdlWindow = window;
		Ogre::NameValuePairList StartInfo;
		std::stringstream ss;
#ifdef XDEVL_PLATFORM_WINDOWS
		HWND hwnd = static_cast<HWND>(window->getInternal("WIN32_HWND"));
		ss << (xdl_int)hwnd;
		StartInfo.insert(Ogre::NameValuePairList::value_type("externalWindowHandle",ss.str()));
#elif defined(XDEVL_PLATFORM_LINUX)
		Display* display = static_cast<Display*>(window->getInternal(XdevLInternalName("X11_DISPLAY")));
		Window  win = (Window)(window->getInternal(XdevLInternalName("X11_WINDOW")));
		xdl_uint screen_number = 0;
		StartInfo["parentWindowHandle"] =	Ogre::StringConverter::toString((long)display) + " :"  +
		                                    Ogre::StringConverter::toString((xdl_int)screen_number) + " :" +
		                                    Ogre::StringConverter::toString((long)win);
#elif defined(XDEVL_PLATFORM_APPLE)
		StartInfo["parentWindowHandle"] =  	Ogre::StringConverter::toString(window->getInternal(XdevLInternalName("MACOSX_WINDOW")));

#else
#error "Not supported paltform."
#endif

		// If color depth specified assign it.
		if(-1 != m_ColorDepth) {
			StartInfo.insert(Ogre::NameValuePairList::value_type("colourDepth", Ogre::StringConverter::toString(m_ColorDepth)));
		}

		// Assign vertical sync.
		StartInfo.insert(Ogre::NameValuePairList::value_type("vsync", Ogre::StringConverter::toString(m_VSync)));

		// Assign anti-aliasing
		if(-1 != m_Antialiasing) {
			StartInfo.insert(Ogre::NameValuePairList::value_type("FSAA", Ogre::StringConverter::toString(m_Antialiasing)));
		}

		// Assign gamma correctiong.
		StartInfo.insert(Ogre::NameValuePairList::value_type("gamma", Ogre::StringConverter::toString(m_gamma)));

		// Switch on/off nvperf.
		StartInfo.insert(Ogre::NameValuePairList::value_type("useNVPerfHUD", Ogre::StringConverter::toString(m_useNVPerfHUD)));

		// Assign display frequence.
		if(m_displayFrequency != -1) {
			StartInfo.insert(Ogre::NameValuePairList::value_type("displayFrequency", Ogre::StringConverter::toString(m_displayFrequency)));
		}

		XDEVL_MODULE_INFO("Window Title : " << m_xdlWindow->getTitle() << std::endl);
		XDEVL_MODULE_INFO("Window Width : " << m_xdlWindow->getWidth() << std::endl);
		XDEVL_MODULE_INFO("Window Height: " << m_xdlWindow->getHeight() << std::endl);

		try {
			m_window = m_root->createRenderWindow(m_xdlWindow->getTitle().toString(),
			                                      m_xdlWindow->getWidth(),
			                                      m_xdlWindow->getHeight(),
			                                      m_xdlWindow->getFullscreen(), &StartInfo);
		} catch(Ogre::Exception& e) {
			XDEVL_MODULE_ERROR(e.getFullDescription());
			return ERR_ERROR;
		}

		XDEVL_MODULE_INFO("Initializing Ogre resource groups.\n");
		// After reading resource infos we must initialize all groups
		try {
			Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		} catch(...) {
			XDEVL_MODULE_ERROR("Ogre resource manager error\n");
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLOgreImpl::readOgreInfo(TiXmlDocument* document) {
		TiXmlHandle docHandle(document);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLOgre").ToElement();
		if(!root) {
			XDEVL_MODULE_ERROR("Wrong xml file format for XdevL.\n");
			return ERR_ERROR;
		}

		if(root->Attribute("id")) {
			XdevLID id(root->Attribute("id"));
			if(getID() != id) {
				return ERR_OK;
			}
		} else {
			XDEVL_MODULE_ERROR("No 'id' attribute specified.");
			return ERR_ERROR;
		}

		if(root->Attribute("fsaa"))
			m_Antialiasing = xstd::from_string<xdl_int>(root->Attribute("fsaa"));
		if(root->Attribute("vsync"))
			m_VSync = xstd::from_string<xdl_bool>(root->Attribute("vsync"));
		if(root->Attribute("color_depth"))
			m_colorDepth = xstd::from_string<xdl_int>(root->Attribute("color_depth"));
		if(root->Attribute("gamma"))
			m_gamma = xstd::from_string<xdl_bool>(root->Attribute("gamma"));
		if(root->Attribute("use_nv_perf_hud"))
			m_useNVPerfHUD = xstd::from_string<xdl_bool>(root->Attribute("use_nv_perf_hud"));
		if(root->Attribute("display_frequency"))
			m_displayFrequency = xstd::from_string<xdl_int>(root->Attribute("display_frequency"));

		TiXmlElement* plugins = root->FirstChildElement("Plugins")->ToElement();
		TiXmlElement* child = 0;
		for(child = plugins->FirstChildElement(); child; child = child->NextSiblingElement()) {
			if(child->ValueTStr() == "Plugin")
				if(child->GetText()) {
					if(m_ogrePluginNames[0] == xstd::get_pathless(std::string(child->GetText()))) { // Skip the default RenderSystem if it is specified.
						m_ogrePluginNames[0] = child->GetText();
					} else {
						m_ogrePluginNames.push_back(child->GetText());
					}
				}
		}
		return ERR_OK;
	}

	xdl_int XdevLOgreImpl::init() {
		// Log informations are nice but too much are anoying so
		// let's create our own logger and say that we don't need
		// to much log information.
		if(Ogre::LogManager::getSingletonPtr() == 0) {
			Ogre::LogManager* mLogManager = new Ogre::LogManager();
			std::string logfilename = std::string(this->getID().getName().toString()) + ".log";
			mLogManager->createLog(logfilename,false,false,false);
#ifndef _NDEBUG
			Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_BOREME);
#else
			Ogre::LogManager::getSingleton().setLogDetail(Ogre::LL_LOW);
#endif
		}

		if(getMediator()->getXmlFilename() == nullptr)
			return ERR_ERROR;

		TiXmlDocument xmlDocument;
		if(!xmlDocument.LoadFile(getMediator()->getXmlFilename())) {
			XDEVL_MODULE_ERROR("Could not parse xml file: " << getMediator()->getXmlFilename() << std::endl);
			return ERR_ERROR;
		}

		if(readOgreInfo(&xmlDocument) != ERR_OK) {
			XDEVL_MODULE_WARNING("Parsing problems occurred of the Core XML file.\n");
		}

		// This is the main Ogre Object.
		m_root = new Ogre::Root("","");
		if(!m_root) {
			XDEVL_MODULE_ERROR("Could not create Ogre root object.\n");
			return ERR_ERROR;
		}

		for(size_t idx = 0; idx < m_ogrePluginNames.size(); ++idx) {
			xstd::make_unix_file(m_ogrePluginNames[idx]);
			std::string pathless	= xstd::get_pathless(m_ogrePluginNames[idx]);
			std::string path			= xstd::get_path(m_ogrePluginNames[idx]);
			std::string ext				= xstd::get_ext(pathless);
			std::string tmp;

			// First check if we have a valid extension name.
			if(ext == ".dll" || ext == ".so")
				tmp				= xstd::get_extless(pathless); // Yes, OK than let's use only the file
			else
				tmp = pathless;

#ifdef _WIN32
#ifdef XDEVL_DEBUG
			tmp += "_d";
#endif
#endif

			m_ogrePluginNames[idx].clear();
			m_ogrePluginNames[idx] = path + "/";
#ifdef _WIN32
			if(path == xstd::current_path_id)
				m_ogrePluginNames[idx] = "";
#endif
			m_ogrePluginNames[idx] += tmp;

			XDEVL_MODULE_INFO("Loading Ogre plugin: " << m_ogrePluginNames[idx] << ".\n");
			try {
				m_root->loadPlugin(m_ogrePluginNames[idx]);
			} catch(...) {
				XDEVL_MODULE_ERROR("Could not load Ogre plugin: " << m_ogrePluginNames[idx] << ".\n");
				return ERR_ERROR;
			}
		}

		Ogre::RenderSystem* rsystem = nullptr;
		rsystem = m_root->getRenderSystemByName("OpenGL Rendering Subsystem");
		if(!rsystem)
			rsystem = m_root->getRenderSystemByName("Direct3D9 Rendering Subsystem");

		if(!rsystem) {
			XDEVL_MODULE_ERROR("No OGRE Rendersystem found.\n");
			return ERR_ERROR;
		}

		m_root->setRenderSystem(rsystem);

		m_root->initialise(false);

// Read all resource information out

		if(readResourceInfo(&xmlDocument) != ERR_OK) {
			XDEVL_MODULE_ERROR("Check the <resource> section in your xml file.\n");
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLOgreImpl::readResourceInfo(TiXmlDocument* xmlDocument) {

		TiXmlHandle docHandle(xmlDocument);
		TiXmlElement* root = docHandle.FirstChild(XdevLCorePropertiesName.c_str()).FirstChildElement("XdevLOgre").ToElement();
		if(!root) {
			XDEVL_MODULE_WARNING("<XdevLOgre> section not found. Using default values for the device.\n");
			return ERR_OK;
		}
		while(root != nullptr) {
			if(root->Attribute("id")) {
				XdevLID id(root->Attribute("id"));
				if(getID() == id) {
					root = root->FirstChildElement("Resources");

					Ogre::ResourceGroupManager* rgm = Ogre::ResourceGroupManager::getSingletonPtr();
					std::vector<std::string> plugin_names;
					TiXmlElement* child = 0;
					for(child = root->FirstChildElement(); child; child = child->NextSiblingElement()) {
						if(child->ValueTStr() == "Location") {
							std::string group = "DEFAULT_RESOURCE_GROUP_NAME";
							group = child->Attribute("group");
							TiXmlElement* loc = 0;
							for(loc = child->FirstChildElement(); loc; loc = loc->NextSiblingElement()) {
								std::string type;
								if(loc->ValueTStr() == "Filesystem")
									type = "FileSystem";
								if(loc->ValueTStr() == "Zip")
									type = "Zip";
								try {
									rgm->addResourceLocation(loc->GetText(), type, group);
								} catch(...) {
									XDEVL_MODULE_WARNING("The file: " << loc->GetText() << " doesn't exsist.\n");
									return ERR_ERROR;
								}
							}
						}
					}
				}
			} else
				XDEVL_MODULE_ERROR("No 'id' attribute specified. Using default values for the device\n");


			root = root->NextSiblingElement();
		}
		return ERR_OK;
	}

	xdl_int XdevLOgreImpl::shutdown() {
		if(m_root) delete m_root;

		return ERR_OK;
	}

	xdl_int XdevLOgreImpl::update() {
		XDEVL_ASSERT( (m_xdlWindow != nullptr), "XdevLOgre is not attached to a window.");

		if(!m_root)
			return ERR_OK;

		Ogre::WindowEventUtilities::messagePump();//?

		if(!m_root->_fireFrameStarted()) {
			return ERR_ERROR ;
		}

		if(m_window)
			m_window->update(true);

		if(!m_root->_fireFrameEnded()) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	void* XdevLOgreImpl::getInternal(const xdl_char* id) {
		std::string data(id);
		if(data == "OGRE_ROOT")
			return m_root;
		if(data == "OGRE_WINDOW")
			return m_window;
		return nullptr;
	}

// -----------------------------------------------------------------------------
// Module 2
//
	/*
	XdevLOgreSpritesImpl::XdevLOgreSpritesImpl(XdevLModuleCreateParameter* parameter) :	XdevLModuleImpl<XdevLOgreSprites>(parameter){
		// Reserve memory for at least 30 sprites.
		m_sprites.reserve(30);

	}

	XdevLOgreSpritesImpl::~XdevLOgreSpritesImpl(){

	}

	XdevLModuleDescriptor* XdevLOgreSpritesImpl::getDescriptor() const {
		return &m_ogreSpriteModuleDesc;
	}

	xdl_int XdevLOgreSpritesImpl::init(XdevLOgre* ogre, const xdl_char* sceneManagerName ){

		m_root = ogre->getRoot();

		if(!m_root && !sceneManagerName){
			return ERR_ERROR;
		}

		m_sceneManager = m_root->getSceneManager(sceneManagerName);

		m_renderOp.vertexData = new  Ogre::VertexData();
		m_renderOp.vertexData->vertexStart=0;

		m_vd = m_renderOp.vertexData->vertexDeclaration;
		m_vd->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
		m_vd->addElement(0, Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3),
												Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);

		xdl_int size = m_sprites.size()*6;
		m_vb	=	Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
						m_vd->getVertexSize(0),
						size,// buffer size
						Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
						false);// use shadow buffer? no
		if(m_vb.isNull()){
			return ERR_ERROR;
		}

		m_sceneManager->addRenderQueueListener(this);

		return ERR_OK;
	}

	xdl_int XdevLOgreSpritesImpl::addSprite(const xdl_char* materialName, xdl_float x, xdl_float y,  xdl_float width, xdl_float height){
		m_sprites.push_back(new sprite(x,y,width,height));
		return ERR_OK;
	}

	void XdevLOgreSpritesImpl::renderBuffer(){
		Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();

		std::vector<sprite*>::iterator ic(m_sprites.begin());
		std::vector<sprite*>::iterator ie(m_sprites.end());

		float* buffer;
		float z=-1.0;

		buffer=(float*)m_vb->lock(Ogre::HardwareBuffer::HBL_DISCARD);

		for(;ic != ie; ++ic){
				// Bottom, Left
				*buffer++ = (*ic)->m_x;
				*buffer++ = (*ic)->m_y;
				*buffer++ = z;
				*buffer++ = 0.0f;
				*buffer++ = 1.0f;
				// Top, Right
				*buffer++ = (*ic)->m_x + (*ic)->m_width;
				*buffer++ = (*ic)->m_y + (*ic)->m_height;
				*buffer++ = z;
				*buffer++ = 1.0f;
				*buffer++ = 0.0f;
				// Top, Left
				*buffer++ = (*ic)->m_x;
				*buffer++ = (*ic)->m_y + (*ic)->m_height;
				*buffer++ = z;
				*buffer++ = 0.0f;
				*buffer++ = 0.0f;
				// Bottom, Left
				*buffer++ = (*ic)->m_x;
				*buffer++ = (*ic)->m_y;
				*buffer++ = z;
				*buffer++ = 0.0f;
				*buffer++ = 1.0f;
				// Bottom, Right
				*buffer++ = (*ic)->m_x + (*ic)->m_width;
				*buffer++ = (*ic)->m_y;
				*buffer++ = z;
				*buffer++ = 1.0f;
				*buffer++ = 1.0f;
				// Top, Right
				*buffer++ = (*ic)->m_x + (*ic)->m_width;
				*buffer++ = (*ic)->m_y + (*ic)->m_height;
				*buffer++ = z;
				*buffer++ = 1.0f;
				*buffer++ = 0.0f;
		}
		m_vb->unlock();
		rs->_render(m_renderOp);
	}

	void XdevLOgreSpritesImpl::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String &invocation, bool &skipThisInvocation){
		renderBuffer();
	}

	void XdevLOgreSpritesImpl::renderQueueEnded(Ogre::uint8 queueGroupId, const Ogre::String &invocation, bool &repeatThisInvocation){
		renderBuffer();
	}
	*/
}
