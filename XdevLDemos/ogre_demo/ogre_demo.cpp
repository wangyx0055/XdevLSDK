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
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLInput/XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLOgre/XdevLOgre.h>
#include <XdevLTimer.h>

xdl::IPXdevLCore		core			= nullptr;
xdl::IPXdevLWindow		window			= nullptr;
xdl::IPXdevLKeyboard	keyboard		= nullptr;
xdl::IPXdevLOgre		ogre			= nullptr;

Ogre::Root*				ogreRoot		= nullptr;
Ogre::SceneManager* 	sceneManager	= nullptr;
Ogre::Camera*			camera			= nullptr;
Ogre::Viewport*			viewport		= nullptr;
Ogre::RenderWindow* 	ogreWindow		= nullptr;

Ogre::SceneNode*		cubeNode		= nullptr;
Ogre::Entity*			ent				= nullptr;

int createOgreObjects(){
	 // Get the pointer to the ogre root object.
	ogreRoot = ogre->getRoot();

	try{
		// Now we can create a scene manager.
		sceneManager = ogreRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, "MySceneManager");

		// We need a camera.
		camera = sceneManager->createCamera("Test");
		camera->setPosition(Ogre::Vector3(0.0f,0.0f,7.0f));
		camera->lookAt(Ogre::Vector3(0.0f,0.0f,0.0f));
		camera->setNearClipDistance(0.5f);
		camera->setFarClipDistance(5000.0f);
		camera->setAspectRatio(window->getWidth()/window->getHeight());


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
		light->setDiffuseColour( 1.0, 1.0, 1.0 );
		light->setDirection(Ogre::Vector3(0, 0, -1));

	}catch(...){
		std::cerr << "Could not setup Ogre scene.\n";
		return xdl::ERR_ERROR;
	}

	try{
	ent = sceneManager->createEntity( "MyCube", "Cube.mesh" );

	// To transform the spaceship we need a scenenode.
	cubeNode = sceneManager->getRootSceneNode()->createChildSceneNode( "MyCube" );

	}catch(...){
		if(!ent){
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

int createDevicesAndModules(xdl::XdevLCore* core){

	window = xdl::getModule<xdl::IPXdevLWindow>(core, xdl::XdevLID("MyWindow"));
	if(!window)
		return xdl::ERR_ERROR;

	window->create();

	keyboard = xdl::getModule<xdl::IPXdevLKeyboard>(core, xdl::XdevLID("MyKeyboard"));
	if(!keyboard)
		return xdl::ERR_ERROR;

	ogre = xdl::getModule<xdl::IPXdevLOgre>(core, xdl::XdevLID("MyOgre"));
	if(!ogre)
		return xdl::ERR_ERROR;

	if(keyboard->attach(window) != xdl::ERR_OK)
		return xdl::ERR_ERROR;

	if(ogre->attach(window) !=  xdl::ERR_OK)
		return xdl::ERR_ERROR;

	return xdl::ERR_OK;
}

int main(int argc, char* argv[]) {
	
	// Create the core system.
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("ogre_demo.xml")) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Create all neccesary devices and modules.
	if(createDevicesAndModules(core) != xdl::ERR_OK)
		return xdl::ERR_ERROR;

	// Create rendering objects.
	if(createOgreObjects() != xdl::ERR_OK)
		return xdl::ERR_ERROR;


	xdl::XdevLButton* esc;
	keyboard->getButton(xdl::KEY_ESCAPE, &esc);
	
	xdl::XdevLButton* fullscreen;
	keyboard->getButton(xdl::KEY_F, &fullscreen);
	

	window->show();

	while(!esc->getClicked()) {
		core->update();
		
		if(fullscreen->getClicked()) {
			static xdl::xdl_bool fullscreenflag = false;
			fullscreenflag = !fullscreenflag;
			window->setFullscreen(fullscreenflag);
		}
		
		
		Ogre::Quaternion rotation;
		Ogre::Radian rotationInRad(Ogre::Degree(core->getTime()*30));
		rotation.FromAngleAxis(rotationInRad, Ogre::Vector3(0.8f, 1.0f, 0.7f));
		cubeNode->setOrientation(rotation);

		ogre->update();
		

		std::cout << "FPS: " << std::setw(10) << (1.0/core->getDT()) << "\r";
	}
	
	xdl::destroyCore(core);
		
	return 0;
}
