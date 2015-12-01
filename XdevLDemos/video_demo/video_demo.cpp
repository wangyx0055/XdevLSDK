#include <XdevL.h>
#include <XdevLTimer.h>
#include <XdevLOpenGL/XdevLOpenGL.h>
#include <XdevLVideo/XdevLVideo.h>
#include <iostream>


xdl::XdevLCorePtr core;
GLuint video_texture = 0;
GLfloat rot = 0.0f;
xdl::XdevLTimer gtimer;

xdl::XdevLWindowDevice* window = NULL;
xdl::XdevLKeyboardDevice* keyboard  = NULL;
xdl::XdevLOpenGL* opengl	= NULL;
xdl::XdevLVideo* video		= NULL;

void createVideoPlaneTexture(){
	glClearColor( 0.0f, 0.305f, 0.596f, 1.0f );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(0, window->getWidth(), 0, window->getHeight(), -1000, 1000);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();	

	glGenTextures(1,&video_texture);
	glBindTexture(GL_TEXTURE_2D, video_texture);
	glTexImage2D(GL_TEXTURE_2D,0, GL_RGB, video->getFrameWidth(), video->getFrameHeight(),0 , GL_RGB,  GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	
}

void drawVideoPlane(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, video_texture);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,video->getFrameWidth(), video->getFrameHeight(),GL_RGB, GL_UNSIGNED_BYTE, video->getFrameData());
	glLoadIdentity();
	glTranslatef(0.5f*video->getFrameWidth(), 0.5f*video->getFrameHeight(), 0.0f);
//	glRotatef(rot, 1.0f, 0.0f, 1.0f);
	glScalef(1.0, -1.0f, 1.0f);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);glVertex3f(-0.5f*video->getFrameWidth(), -0.5f*video->getFrameHeight(), 0.0f);
	glTexCoord2f(1.0f, 0.0f);glVertex3f(0.5f*video->getFrameWidth(), -0.5f*video->getFrameHeight(), 0.0f);
	glTexCoord2f(1.0f, 1.0f);glVertex3f(0.5f*video->getFrameWidth(), 0.5f*video->getFrameHeight(), 0.0f);
	glTexCoord2f(0.0f, 1.0f);glVertex3f(-0.5f*video->getFrameWidth(), 0.5f*video->getFrameHeight(), 0.0f);
	glEnd();
	glPopMatrix();

	//glDrawPixels(video->getFrameWidth(),video->getFrameHeight(),GL_RGB,GL_UNSIGNED_BYTE,video->getImage());

}

int main(int argc, char* argv[]) {
	// Create the core system and initiate all specified plugins and devices.
	core = xdl::createCore("video_demo.xml");

	// Create a window so that we can draw something.
	window = xdl::getModule<xdl::XdevLWindowDevice*>(core, WindowId);
	if(!window)
		return xdl::ERR_ERROR;

	// Create an object to get access to the keyboard XdevLDevice.
	keyboard = xdl::getModule<xdl::XdevLKeyboardDevice*>(core, KeyboardId);
	if(!keyboard)
		return xdl::ERR_ERROR;

	keyboard->attach(window);
	opengl = xdl::getModule<xdl::XdevLOpenGL*>(core,OpenGLId);
	if(!opengl){
		return xdl::ERR_ERROR;
	}
	
	
	opengl->attach(window);
	
	video = xdl::getModule<xdl::XdevLVideo*>(core, VideoId);
	if(!video){
		return xdl::ERR_ERROR;
	}
	
	// Open a video file.
	if(video->open("bleach_200.avi") != xdl::ERR_OK){
		std::cerr << "video not open\n";
		return 1;
	}
	// Set FPS of the player.
	video->setFPS(20.0f);
	
	createVideoPlaneTexture();

	xdl::XdevLButton* esc;
	keyboard->getButton(xdl::KEY_ESCAPE, &esc);

	while (true) {
		if (esc->getClicked())
			break;
		gtimer.update();
//		rot += static_cast<GLfloat>(gtimer.getDT() * 60.0);
		
		drawVideoPlane();
		video->update();
		opengl->swapBuffers();
		core->update();
	}

	return 0;
}
