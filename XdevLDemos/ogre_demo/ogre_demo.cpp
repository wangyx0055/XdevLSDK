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
#include <XdevLApplication.h>
#include <XdevLOgre/XdevLOgre.h>
#include <XdevLTimer.h>


class MyOgreApp : public xdl::XdevLApplication {
	public:
		MyOgreApp(int argc, char** argv, const xdl::XdevLFileName& xml_filename) throw() :
			xdl::XdevLApplication(argc, argv, xml_filename),
			ogre(nullptr),
			ogreRoot(nullptr),
			sceneManager(nullptr),
			camera(nullptr),
			viewport(nullptr),
			ogreWindow(nullptr),
			cubeNode(nullptr),
			ent(nullptr),
			appIsRunning(xdl::xdl_true)	{
			getCore()->registerListener(this);
		}

		void main(const xdl::XdevLApplication::Arguments& argv) throw() override;
		/// Create XdevL related stuff.
		xdl::xdl_int createDevicesAndModules();

		/// Create OGRE related stuff.
		xdl::xdl_int createOgreObjects();

		/// Callback for XdevL events.
		xdl::xdl_int notify(xdl::XdevLEvent& event) override;

	private:

		xdl::IPXdevLOgre		ogre;
		Ogre::Root*					ogreRoot;
		Ogre::SceneManager* sceneManager;
		Ogre::Camera*				camera;
		Ogre::Viewport*			viewport;
		Ogre::RenderWindow* ogreWindow;
		Ogre::SceneNode*		cubeNode;
		Ogre::Entity*				ent;
		xdl::XdevLButton* 	esc;
		xdl::XdevLButton* 	fullscreen;
		xdl::xdl_bool 			appIsRunning;
};

void MyOgreApp::main(const xdl::XdevLApplication::Arguments& argv) throw() {

	//
	// Create/Get necessary modules.
	//
	if(createDevicesAndModules() != xdl::ERR_OK) {
		return;
	}

	//
	// Create OGRE objects for rendering etc.
	//
	if(createOgreObjects() != xdl::ERR_OK) {
		return;
	}

	//
	// Start main loop.
	//
	while(appIsRunning && !esc->getClicked()) {
		
		// Update the core system.
		getCore()->update();

		// Check if user pressed 'F' for fullscreen.
		if(fullscreen->getClicked()) {
			static xdl::xdl_bool fullscreenflag = false;
			fullscreenflag = !fullscreenflag;
			getWindow()->setFullscreen(fullscreenflag);
		}

		// Do some rendering.
		Ogre::Quaternion rotation;
		Ogre::Radian rotationInRad(Ogre::Degree(getCore()->getTime()*30));
		rotation.FromAngleAxis(rotationInRad, Ogre::Vector3(0.8f, 1.0f, 0.7f));
		cubeNode->setOrientation(rotation);

		ogre->update();

		std::cout << "FPS: " << std::setw(10) << (1.0/getCore()->getDT()) << "\r";
	}

}

xdl::xdl_int MyOgreApp::createDevicesAndModules() {

	ogre = xdl::getModule<xdl::IPXdevLOgre>(getCore(), xdl::XdevLID("MyOgre"));
	if(xdl::isModuleNotValid(ogre)) {
		return xdl::ERR_ERROR;
	}

	if(ogre->attach(getWindow()) !=  xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Assign keys.
	getKeyboard()->getButton(xdl::KEY_ESCAPE, &esc);
	getKeyboard()->getButton(xdl::KEY_F, &fullscreen);

	return xdl::ERR_OK;
}

xdl::xdl_int MyOgreApp::createOgreObjects() {
	// Get the pointer to the ogre root object.
	ogreRoot = ogre->getRoot();

	try {
		// Now we can create a scene manager.
		sceneManager = ogreRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, "MySceneManager");

		// We need a camera.
		camera = sceneManager->createCamera("Test");
		camera->setPosition(Ogre::Vector3(0.0f,0.0f,7.0f));
		camera->lookAt(Ogre::Vector3(0.0f,0.0f,0.0f));
		camera->setNearClipDistance(0.5f);
		camera->setFarClipDistance(5000.0f);
		camera->setAspectRatio(getWindow()->getWidth()/getWindow()->getHeight());


		// Now we assign the camera as our viewport.
		ogreWindow = ogre->getWindow();
		viewport = ogreWindow->addViewport(camera);

		// Let's make the background color a bit nice.
		viewport->setBackgroundColour(Ogre::ColourValue(0.0f, 0.305f, 0.596f));
		sceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));

		// We need a light.
		Ogre::Light* light = sceneManager->createLight("MyLight");
		light->setType(Ogre::Light::LT_DIRECTIONAL);
		light->setPosition(Ogre::Vector3(0.0f, 0.0f, 4.0f));
		light->setDiffuseColour(1.0, 1.0, 1.0);
		light->setDirection(Ogre::Vector3(0, 0, -1));

	} catch(...) {
		std::cerr << "Could not setup Ogre scene.\n";
		return xdl::ERR_ERROR;
	}

	try {
		ent = sceneManager->createEntity("MyCube", "Cube.mesh");

		// To transform the spaceship we need a scenenode.
		cubeNode = sceneManager->getRootSceneNode()->createChildSceneNode("MyCube");

	} catch(...) {
		if(!ent) {
			std::cerr << "Creating the mesh object failed.\n";
			return xdl::ERR_ERROR;
		}
	}

	// Attach our mesh to the scene node.
	cubeNode->attachObject(ent);

	// Transform the node
	cubeNode->setPosition(0.0f, 0.0f, 0.0f);

	return xdl::ERR_OK;
}

xdl::xdl_int MyOgreApp::notify(xdl::XdevLEvent& event) {
	switch(event.type) {
		case xdl::XDEVL_CORE_EVENT: {
			if(event.core.type == xdl::XDEVL_CORE_SHUTDOWN) {
				std::cout << "XDEVL_CORE_SHUTDOWN received.\n";
				appIsRunning = xdl::xdl_false;
			}
		}
		break;
		case xdl::XDEVL_WINDOW_EVENT: {
			switch(event.window.event) {
				case xdl::XDEVL_WINDOW_MOVED:
				case xdl::XDEVL_WINDOW_RESIZED: {

				}
				break;
			}
		}
		break;
	}
	return xdl::XdevLApplication::notify(event);
}

XdevLStartMain(MyOgreApp, "ogre_demo.xml")
