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

#include <XdevLRAI/GL/glew.h>
#include <XdevLRAI/XdevLRAI.h>

#include <cmath>
#include <tm/tm.h>

class XDEVL_VERTEX_COLOR;


static const GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,

	1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,

	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,

	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,

	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,

	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,

	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,

	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,

	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

static const GLfloat g_normal_buffer_data[] = {
	-1.0f, 0.0f,0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,

	0.0f, 0.0f,-1.0f,
	0.0f, 0.0f,-1.0f,
	0.0f, 0.0f,-1.0f,

	0.0f, -1.0f, 0.0f,
	0.0f, -1.0f,0.0f,
	0.0f, -1.0f,0.0f,

	0.0f, 0.0f,-1.0f,
	0.0f, 0.0f,-1.0f,
	0.0f, 0.0f,-1.0f,

	-1.0f, 0.0f,0.0f,
	-1.0f, 0.0f, 0.0f,
	-1.0f, 0.0f,0.0f,

	0.0f,-1.0f, 0.0f,
	0.0f,-1.0f, 0.0f,
	0.0f,-1.0f,0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	1.0f,0.0f,0.0f,
	1.0f, 0.0f,0.0f,

	1.0f,0.0f,0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f,0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f,0.0f,
	0.0f, 1.0f,0.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f,0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f
};

static const GLfloat g_color_buffer_data[] = {
	0.583f,  0.771f,  0.014f, 1.0f,
	0.609f,  0.115f,  0.436f, 1.0f,
	0.327f,  0.483f,  0.844f, 1.0f,
	0.822f,  0.569f,  0.201f, 1.0f,
	0.435f,  0.602f,  0.223f, 1.0f,
	0.310f,  0.747f,  0.185f, 1.0f,
	0.597f,  0.770f,  0.761f, 1.0f,
	0.559f,  0.436f,  0.730f, 1.0f,
	0.359f,  0.583f,  0.152f, 1.0f,
	0.483f,  0.596f,  0.789f, 1.0f,
	0.559f,  0.861f,  0.639f, 1.0f,
	0.195f,  0.548f,  0.859f, 1.0f,
	0.014f,  0.184f,  0.576f, 1.0f,
	0.771f,  0.328f,  0.970f, 1.0f,
	0.406f,  0.615f,  0.116f, 1.0f,
	0.676f,  0.977f,  0.133f, 1.0f,
	0.971f,  0.572f,  0.833f, 1.0f,
	0.140f,  0.616f,  0.489f, 1.0f,
	0.997f,  0.513f,  0.064f, 1.0f,
	0.945f,  0.719f,  0.592f, 1.0f,
	0.543f,  0.021f,  0.978f, 1.0f,
	0.279f,  0.317f,  0.505f, 1.0f,
	0.167f,  0.620f,  0.077f, 1.0f,
	0.347f,  0.857f,  0.137f, 1.0f,
	0.055f,  0.953f,  0.042f, 1.0f,
	0.714f,  0.505f,  0.345f, 1.0f,
	0.783f,  0.290f,  0.734f, 1.0f,
	0.722f,  0.645f,  0.174f, 1.0f,
	0.302f,  0.455f,  0.848f, 1.0f,
	0.225f,  0.587f,  0.040f, 1.0f,
	0.517f,  0.713f,  0.338f, 1.0f,
	0.053f,  0.959f,  0.120f, 1.0f,
	0.393f,  0.621f,  0.362f, 1.0f,
	0.673f,  0.211f,  0.457f, 1.0f,
	0.820f,  0.883f,  0.371f, 1.0f,
	0.982f,  0.099f,  0.879f, 1.0f
};

class MyOpenGLApp : public xdl::XdevLApplication {
	public:

		MyOpenGLApp(int argc, char** argv, const xdl::XdevLFileName& xml_filename) throw() :
			xdl::XdevLApplication(argc, argv, xml_filename),
			m_rai(nullptr),
			m_frameBuffer(nullptr),
			m_frameBufferArray(nullptr),
			m_appIsRunning(xdl::xdl_true)	{
			getCore()->registerListener(this);
		}

		~MyOpenGLApp() {

			m_rai->destroy(m_vd);
			m_rai->destroy(m_vertexDeclaration);
			m_rai->destroy(m_frameBuffer);
			m_rai->destroy(m_vs);
			m_rai->destroy(m_fs);
			m_rai->destroy(m_sp);

		}

		virtual void main(const Arguments& argv) throw() {

			if(initRenderDevice() != xdl::ERR_OK) {
				return;
			}

			if(initFramebuffer() != xdl::ERR_OK) {
				return;
			}

			if(initInputHandling() != xdl::ERR_OK) {
				return;
			}

			if(initRenderAssets() != xdl::ERR_OK) {
				return;
			}

			getWindow()->show();

			xdl::xdl_double old_time = getCore()->getTime();

			// Start main loop.
			while(m_appIsRunning) {


				// Update core events.
				getCore()->update();

				// Get current time step.
				xdl::xdl_double dT = getCore()->getTime() - old_time;
				old_time = getCore()->getTime();

				handleInputEvents(dT);

				// Draw and calculate stuff.
				handleGraphics(dT);

			}

		}


		virtual xdl::xdl_int notify(xdl::XdevLEvent& event) {
			switch(event.type) {
				case xdl::XDEVL_CORE_EVENT: {
					if(event.core.type == xdl::XDEVL_CORE_SHUTDOWN) {
						std::cout << "XDEVL_CORE_SHUTDOWN received.\n";
						m_appIsRunning = xdl::xdl_false;
					}
				}
				break;
				case xdl::XDEVL_WINDOW_EVENT: {
					switch(event.window.event) {
						case xdl::XDEVL_WINDOW_MOVED:
						case xdl::XDEVL_WINDOW_RESIZED: {
							createScreenVertexArray(getWindow());
						}
						break;
					}
				}
				break;
			}
			return xdl::ERR_OK;
		}

		void handleGraphics(xdl::xdl_double dT) {

			if(m_left_mouse_button->getPressed()) {
				rx += static_cast<float>(y_axis->getDeltaValue()) * 0.01;
				ry += static_cast<float>(x_axis->getDeltaValue()) * 0.01;
			}

			//
			// Let's render stuff into the framebuffer object with low resolution.
			//
			m_frameBuffer->activate();

			xdl::xdl_uint list[] = {xdl::XDEVL_COLOR_TARGET0};
			m_frameBuffer->activateColorTargets(1, list);

			m_frameBuffer->clearColorTargets(0.0f, 0.305f, 0.596f, 1.0f);
			m_frameBuffer->activateDepthTarget(xdl::xdl_true);
			m_frameBuffer->clearDepthTarget(1.0f);

			xdl::xdl_float aspect_ratio = getWindow()->getWidth()/getWindow()->getHeight();

			tmath::mat4 proj, rotx, roty, trans, model;
			tmath::perspective(45.0f, aspect_ratio, 1.0f, 110.0f, proj);

			tmath::rotate_x(rx, rotx);
			tmath::rotate_y(ry, roty);
			tmath::translate(0.0f, 0.0f, -3.0f, trans);

			model =   trans * rotx * roty;



			m_sp->activate();
			m_sp->setUniformMatrix4(m_projViewMatrix, 1, proj);
			m_sp->setUniformMatrix4(m_modelMatrix, 1, model);
			m_sp->deactivate();

			m_rai->setActiveVertexArray(m_va);
			m_rai->setActiveShaderProgram(m_sp);

			m_rai->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 36);



			//
			// Now stop rendering into the frambuffer object.
			//
			m_frameBuffer->deactivate();



			//
			// Render into the second half of the normal framebuffer.
			//

			m_rai->setViewport(0, 0, getWindow()->getWidth()/2.0, getWindow()->getHeight());
			m_rai->clearColorTargets(0.0f, 0.305f, 0.596f, 1.0f);
			m_rai->clearDepthTarget(1.0);

			m_sp->activate();
			m_rai->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES,36);
			m_sp->deactivate();


			//
			// Use the renderd texture from the framebuffer object to render into a squad.
			// Render that squad into the first half of the viewport.
			//

			glDisable(GL_DEPTH_TEST);
			glViewport(getWindow()->getWidth()/2.0, 0, getWindow()->getWidth()/2.0, getWindow()->getHeight());
			tmath::mat4 fbProjection;
			tmath::ortho(0.0f,
			             (float)getWindow()->getWidth(),
			             0.0f,
			             (float)getWindow()->getHeight(),
			             -1.0f,
			             1.0f, fbProjection);

			m_frameBufferSP->activate();
			m_frameBufferSP->setUniformMatrix4(m_frameBufferProjectionMatrix, 1, fbProjection);
			m_frameBufferSP->setUniformi(m_frameBufferTexture, 0);
			m_frameBuffer->getTexture(0)->activate(0);
			m_frameBufferSP->deactivate();

			m_rai->setActiveShaderProgram(m_frameBufferSP);
			m_rai->setActiveVertexArray(m_frameBufferArray);

			m_rai->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 6);


			m_rai->swapBuffers();
		}

		//
		// Initialize the rendering device.
		//
		xdl::xdl_int initRenderDevice() {
			// Get OpenGL Rendering System.
			m_rai = xdl::getModule<xdl::IPXdevLRAI>(getCore(),  xdl::XdevLID("MyRAIGL"));
			if(!m_rai) {
				return xdl::ERR_ERROR;
			}

			// We must attach the OpenGL context to a render m_window.
			if(m_rai->create(getWindow()) != xdl::ERR_OK) {
				return xdl::ERR_ERROR;
			}

			return xdl::ERR_OK;
		}

		//
		// Initialize and connect input devices.
		//
		xdl::xdl_int initInputHandling() {

			getKeyboard()->getButton(xdl::KEY_ESCAPE, &m_esc);
			getKeyboard()->getButton(xdl::KEY_F, &m_fullscreen);
			getMouse()->getButton(xdl::BUTTON_LEFT, &m_left_mouse_button);

			if((m_esc == nullptr) || (m_left_mouse_button == nullptr)) {
				return xdl::ERR_ERROR;
			}


			getMouse()->getAxis(xdl::AXIS_0, &x_axis);
			getMouse()->getAxis(xdl::AXIS_1, &y_axis);

			return xdl::ERR_OK;
		}

		void handleInputEvents(xdl::xdl_double dT) {

			if(m_esc->getClicked()) {
				m_appIsRunning = xdl::xdl_false;
			}

			if(m_fullscreen->getClicked()) {
				static xdl::xdl_bool fullscreen = xdl::xdl_false;
				fullscreen = !fullscreen;

				getWindow()->setFullscreen(fullscreen);

			}

		}

		//
		// Initialize the framebuffer.
		//
		xdl::xdl_int initFramebuffer() {

			m_rai->createFrameBuffer(&m_frameBuffer);
			m_frameBuffer->init(16, 16);
			m_frameBuffer->addColorTarget(0	, xdl::XDEVL_FB_COLOR_RGBA);
			m_frameBuffer->getTexture(0)->lock();
			m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MAG_FILTER, xdl::XDEVL_LINEAR);
			m_frameBuffer->getTexture(0)->setTextureFilter(xdl::XDEVL_TEXTURE_MIN_FILTER, xdl::XDEVL_NEAREST);
			m_frameBuffer->getTexture(0)->unlock();


			m_frameBuffer->addDepthTarget(xdl::XDEVL_FB_DEPTH_COMPONENT24);

			createScreenVertexArray(getWindow());

			m_rai->createShaderProgram(&m_frameBufferSP);

			m_rai->createVertexShader(&m_frameBufferVS);
			m_frameBufferVS->compileFromFile("frameBuffer_vs.glsl");

			m_rai->createFragmentShader(&m_frameBufferFS);
			m_frameBufferFS->compileFromFile("frameBuffer_fs.glsl");

			m_frameBufferSP->attach(m_frameBufferVS);
			m_frameBufferSP->attach(m_frameBufferFS);
			m_frameBufferSP->link();

			m_frameBufferProjectionMatrix = m_frameBufferSP->getUniformLocation("projMatrix");
			m_frameBufferTexture = m_frameBufferSP->getUniformLocation("texture0");


			return xdl::ERR_OK;
		}

		//
		// Initialize the rendering assets.
		//
		xdl::xdl_int initRenderAssets() {

			m_rai->createVertexDeclaration(&m_vd);
			m_vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 0);		// Position
			m_vd->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 1);		// Color
			m_vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 2);		// Normal

			std::vector<xdl::xdl_uint8*> list;
			list.push_back((xdl::xdl_uint8*)g_vertex_buffer_data);
			list.push_back((xdl::xdl_uint8*)g_color_buffer_data);
			list.push_back((xdl::xdl_uint8*)g_normal_buffer_data);


			m_rai->createVertexArray(&m_va);
			m_va->init(list.size(), list.data(), 36, m_vd);


			// Create the shader program.
			m_rai->createShaderProgram(&m_sp);

			m_rai->createVertexShader(&m_vs);
			m_vs->compileFromFile("vs1.vs");

			m_rai->createFragmentShader(&m_fs);
			m_fs->compileFromFile("fs1.fs");

			m_sp->attach(m_vs);
			m_sp->attach(m_fs);
			m_sp->link();


			// Get the vertex attributes for the array.

			m_modelMatrix		= m_sp->getUniformLocation("modelMatrix");
			m_projViewMatrix	= m_sp->getUniformLocation("projViewMatrix");

			return xdl::ERR_OK;
		}


		void createScreenVertexArray(xdl::IPXdevLWindow window) {

			// Layz destroying of the previous vertex array.
			if(m_frameBufferArray != nullptr) {
				m_rai->destroy(m_frameBufferArray);
			}


			xdl::xdl_float screen_vertex [] = {
				0.0f, 0.0f,
				(xdl::xdl_float)window->getWidth(), 0.0f,
				(xdl::xdl_float)window->getWidth(), (xdl::xdl_float)window->getHeight(),

				(xdl::xdl_float)window->getWidth(), (xdl::xdl_float)window->getHeight(),
				0.0f, (xdl::xdl_float)window->getHeight(),
				0.0f, 0.0f
			};

			xdl::xdl_float screen_uv [] = {
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,

				1.0f, 1.0f,
				0.0f, 1.0f,
				0.0f, 0.0f
			};

			m_rai->createVertexDeclaration(&m_vertexDeclaration);
			m_vertexDeclaration->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 0);
			m_vertexDeclaration->add(2, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 9);

			std::vector<xdl::xdl_uint8*> list2;
			list2.push_back((xdl::xdl_uint8*)screen_vertex);
			list2.push_back((xdl::xdl_uint8*)screen_uv);

			m_rai->createVertexArray(&m_frameBufferArray);
			m_frameBufferArray->init(list2.size(), list2.data(), 6, m_vertexDeclaration);
		}

	private:

		xdl::IPXdevLRAI 			m_rai;
		xdl::XdevLFrameBuffer*		m_frameBuffer;
		xdl::XdevLVertexDeclaration* m_vertexDeclaration;
		xdl::XdevLVertexArray*		m_frameBufferArray;
		xdl::XdevLVertexShader*		m_frameBufferVS;
		xdl::XdevLFragmentShader*	m_frameBufferFS;
		xdl::XdevLShaderProgram*	m_frameBufferSP;


		xdl::XdevLVertexArray*			m_va;
		xdl::XdevLVertexDeclaration*	m_vd;
		xdl::XdevLVertexShader*			m_vs;
		xdl::XdevLFragmentShader*		m_fs;
		xdl::XdevLShaderProgram*		m_sp;

		xdl::xdl_int m_modelMatrix;
		xdl::xdl_int m_projViewMatrix;
		xdl::xdl_int m_frameBufferProjectionMatrix;
		xdl::xdl_int m_frameBufferTexture;

		xdl::IPXdevLAxis x_axis;
		xdl::IPXdevLAxis y_axis;
		xdl::IPXdevLButton m_esc;
		xdl::IPXdevLButton m_left_mouse_button;
		xdl::IPXdevLButton m_fullscreen;

		xdl::xdl_bool m_appIsRunning;
		xdl::xdl_float rx;
		xdl::xdl_float ry;

};


XdevLStartMain(MyOpenGLApp, "glsl_demo.xml")
