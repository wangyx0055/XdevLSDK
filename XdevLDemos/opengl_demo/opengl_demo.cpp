/*
	Application: opengl_demo.cpp
	Author     : Cengiz Terzibas
	Brief      : Demonstrates how to render using OpenGL.

*/

#include <XdevL.h>
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLOpenGL/XdevLOpenGL.h>
#include <XdevLKeyboard/XdevLKeyboard.h>
#include <iomanip>

GLfloat LightAmbient[]	= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]	=	{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]	= { 0.0f, 0.0f, -1.0f, 1.0f };


struct vertex{
	GLfloat x,y,z;
	GLfloat nx, ny, nz;
	GLfloat u,v;	
};

vertex cubeVertices[] = {
	{-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 	0.0f, 0.0f},
	{1.0f,	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 	1.0f, 0.0f},
	{1.0f,	1.0f, 1.0f,  0.0f, 0.0f, 1.0f,	1.0f, 1.0f},
	{-1.0f, 1.0f, 1.0f,	 0.0f, 0.0f, 1.0f,	0.0f, 1.0f},
	
	{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f,-1.0f,	1.0f, 0.0f},
	{-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,	1.0f, 1.0f},
	{1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,	0.0f, 1.0f},
	{1.0f, -1.0f, -1.0f, 0.0f, 0.0f,-1.0f,	0.0f, 0.0f},
	

	{-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	0.0f, 1.0f},
	{-1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f,	0.0f, 0.0f},
	{ 1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f,	1.0f, 0.0f},
	{ 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	1.0f, 1.0f},

	{-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	0.0f, 1.0f},
	{-1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f,	0.0f, 0.0f},
	{ 1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f,	1.0f, 0.0f},
	{ 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	1.0f, 1.0f},
};

void drawCube(){
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

		glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

		glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
	glEnd();
}

int main(int argc, char* argv[]) {
	
	xdl::IPXdevLCore core = nullptr;

	// Create the core system.
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("opengl_demo.xml")) != xdl::ERR_OK){
		return xdl::ERR_ERROR;
	}
	
	// Create a window so that we can draw something.
	xdl::IPXdevLWindow window = xdl::getModule<xdl::IPXdevLWindow>(core, xdl::XdevLID("MyWindow"));
	if(!window)
		return xdl::ERR_ERROR;

	// Get the instance to the keyboard module.
	xdl::IPXdevLKeyboard keyboard = xdl::getModule<xdl::IPXdevLKeyboard>(core, xdl::XdevLID("MyKeyboard"));
	if(!keyboard)
		return xdl::ERR_ERROR;

	// Get the OpenGL context.
	xdl::IPXdevLOpenGL330 opengl = xdl::getModule<xdl::IPXdevLOpenGL330>(core, xdl::XdevLID("MyOpenGL"));
	if(!opengl) {
		return xdl::ERR_ERROR;
	}
	
	if(opengl->createContext(window) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Attach the keyboard to the window.
	if(keyboard->attach(window) != xdl::ERR_OK){
		return xdl::ERR_ERROR;
	}

	// Set the viewport
	glViewport(0,0, window->getWidth(), window->getHeight());

	// Set the projection and modelview matrix.
	glClearColor( 0.0f, 0.305f, 0.596f, 1.0f );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum(-0.35f, 0.35f, -0.25f, 0.25f, 0.5f, 1000.0f);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// Create a light source
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	// Enable depth test.
	glEnable(GL_DEPTH_TEST);

	GLfloat rot = 0.0f;

	std::cout << "OpenGL Vendor        : " << opengl->getVendor() << std::endl;
	std::cout << "OpenGL Version       : " << opengl->getVersion() << std::endl;
	std::cout << "OpenGL Shader Version: " << opengl->getShaderVersion() << std::endl;

	// Get a valid button object for the escape key.
	xdl::IPXdevLButton esc = nullptr;
	keyboard->getButton(xdl::KEY_ESCAPE, &esc);

	xdl::XdevLButton* fullscreen;
	keyboard->getButton(xdl::KEY_F, &fullscreen);

	window->show();

	// Start main loop.
	while(!esc->getClicked()) {
		core->update();

		if(fullscreen->getClicked()) {
			static xdl::xdl_bool fullscreenflag = false;
			fullscreenflag = !fullscreenflag;
			window->setFullscreen(fullscreenflag);
		}

		rot = static_cast<GLfloat>(core->getTime()*30.0);

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glViewport(0,0, window->getWidth(), window->getHeight());
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -4.0f);
		glRotatef(rot, 0.8f, 1.0f, 0.7f);
		drawCube();
		glPopMatrix();

		opengl->swapBuffers();

		std::cout << "FPS: " << std::setw(10) << (1.0/core->getDT()) << "\r";
	}


	xdl::destroyCore(core);

	return 0;
}
