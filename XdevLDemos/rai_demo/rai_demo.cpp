/*
	Application: rai_demo.cpp
	Author     : Cengiz Terzibas
	Brief      : Demonstrates how to use the XdevLRAI using OpenGL renderer.

*/

#include <XdevL.h>
#include <XdevLFont/XdevLFont.h>
#include <XdevLFont/XdevLFontSystem.h>
#include <XdevLFont/XdevLTextLayout.h>

#include <XdevLWindow/XdevLWindow.h>
#include <XdevLRAI/XdevLRAI.h>

#include <XdevLMouse/XdevLMouse.h>
#include <XdevLKeyboard/XdevLKeyboard.h>
#include <iomanip>

#include <tm/tm.h>

xdl::IPXdevLCore core					= nullptr;
xdl::IPXdevLWindow window				= nullptr;
xdl::IPXdevLCursor cursor				= nullptr;
xdl::IPXdevLKeyboard keyboard 			= nullptr;
xdl::IPXdevLMouse mouse					= nullptr;
xdl::IPXdevLRAI rai						= nullptr;
xdl::IPXdevLFontSystem fontSystem		= nullptr;
xdl::IPXdevLTextLayout textLayoutSystem	= nullptr;

static xdl::xdl_bool fullscreenflag = false;

std::string vertex_shader_source = {
	"\
layout(location = 0) in vec3 iposition;\
layout(location = 1) in vec4 icolor;\
layout(location = 2) in vec3 inormal;\
\
uniform mat4 projViewMatrix;\
uniform mat4 modelMatrix;\
\
out vec4 color;\
out vec3 normal;\
\
void main() {\
\
	color = icolor;\
\
	normal = normalize(modelMatrix*vec4(inormal, 0.0)).xyz;\
\
    gl_Position = projViewMatrix * modelMatrix * vec4(iposition, 1.0);\
}"
};

std::string fragment_shader_source = {
	"\
in vec4 color;\
in vec3 normal;\
\
vec3 lightDir = vec3(0.0, 0.0, -1.0);\
\
layout(location = 0) out vec4 ocolor;\
\
void main() {\
\
	float fs = max(0.0, dot(normal, -lightDir));\
\
    ocolor = fs*color;\
}"
};


static const float g_vertex_buffer_data[] = {
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

static const float g_normal_buffer_data[] = {
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

static const float g_color_buffer_data[] = {
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};


int main(int argc, char* argv[]) {

	// Create the core system.
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("rai_demo.xml")) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Create a window so that we can draw something.
	window = xdl::getModule<xdl::IPXdevLWindow>(core, xdl::XdevLID("MyWindow"));
	if(!window) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Create a window so that we can draw something.
	cursor = xdl::getModule<xdl::IPXdevLCursor>(core, xdl::XdevLID("XdevLCursor"));
	if(!cursor) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Get the instance to the keyboard module.
	keyboard = xdl::getModule<xdl::IPXdevLKeyboard>(core, xdl::XdevLID("MyKeyboard"));
	if(!keyboard) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Get the instance to the keyboard module.
	mouse = xdl::getModule<xdl::IPXdevLMouse>(core, xdl::XdevLID("MyMouse"));
	if(!mouse) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	if(cursor->attach(window) != xdl::ERR_OK) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Get the OpenGL Rendering System.
	rai = xdl::getModule<xdl::IPXdevLRAI>(core, xdl::XdevLID("MyRAIGL"));
	if(!rai) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Get the FontSystem
	fontSystem = xdl::getModule<xdl::XdevLFontSystem*>(core, xdl::XdevLID("MyFontSystem"));
	if(!fontSystem) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Get the Text Layout System.
	textLayoutSystem = xdl::getModule<xdl::XdevLTextLayout*>(core, xdl::XdevLID("MyTextLayout"));
	if(!textLayoutSystem) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Create the RAI system.
	if(rai->create(window) != xdl::ERR_OK) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Attach the keyboard to the window.
	if(keyboard->attach(window) != xdl::ERR_OK) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Attach the mouse to the window.
	if(mouse->attach(window) != xdl::ERR_OK) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	//
	// Initialize font system.
	//

	fontSystem->init(window->getWidth(), window->getHeight(), rai);

	xdl::XdevLFont* font = fontSystem->createFromFontFile("fonts/default_info.txt");
	if(nullptr == font) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	textLayoutSystem->init(window, rai);
	textLayoutSystem->setScale(1.0f);
	textLayoutSystem->setDFT(0);
	textLayoutSystem->setEffect(0);
	textLayoutSystem->useFont(font);


	//
	// Create Buffers
	//

	xdl::XdevLVertexDeclaration* vd = nullptr;
	rai->createVertexDeclaration(&vd);
	vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 0);		// Position
	vd->add(4, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 1);		// Color
	vd->add(3, xdl::XDEVL_BUFFER_ELEMENT_FLOAT, 2);		// Normal

	std::vector<xdl::xdl_uint8*> list;
	list.push_back((xdl::xdl_uint8*)g_vertex_buffer_data);
	list.push_back((xdl::xdl_uint8*)g_color_buffer_data);
	list.push_back((xdl::xdl_uint8*)g_normal_buffer_data);

	xdl::XdevLVertexArray* va = nullptr;
	rai->createVertexArray(&va);
	va->init(list.size(), list.data(), 36, vd);


	//
	// Create basic shaders
	//

	xdl::XdevLVertexShader*			vs;
	xdl::XdevLFragmentShader*		fs;
	xdl::XdevLShaderProgram*		sp;

	// Create the shader object instances.
	rai->createShaderProgram(&sp);
	rai->createVertexShader(&vs);
	rai->createFragmentShader(&fs);

	// Create the source of the shaders.
	vs->addShaderCode(vertex_shader_source.c_str());
	fs->addShaderCode(fragment_shader_source.c_str());

	// Compile the shaders.
	vs->compile();
	fs->compile();

	// Link vertex and fragment shader to one program.
	sp->attach(vs);
	sp->attach(fs);
	sp->link();


	// Get the vertex attributes from the shaders. We need that to pass those two matrices
	xdl::xdl_int modelMatrix	= sp->getUniformLocation("modelMatrix");
	xdl::xdl_int projViewMatrix	= sp->getUniformLocation("projViewMatrix");


	//
	// Setup some other stuff
	//

	// Get a valid button object for the escape key.
	xdl::IPXdevLButton esc = nullptr;
	keyboard->getButton(xdl::KEY_ESCAPE, &esc);

	xdl::XdevLButton* fullscreen;
	keyboard->getButton(xdl::KEY_F, &fullscreen);

	xdl::IPXdevLAxis x_axis = nullptr;
	xdl::IPXdevLAxis y_axis = nullptr;
	xdl::IPXdevLButton left_mouse_button = nullptr;
	mouse->getAxis(xdl::AXIS_0, &x_axis);
	mouse->getAxis(xdl::AXIS_1, &y_axis);
	mouse->getButton(xdl::BUTTON_0, &left_mouse_button);

	window->show();

	xdl::xdl_float rx = 0.0f;
	xdl::xdl_float ry = 0.0f;

//	cursor->hide();
	cursor->setPosition(400, 300);
//	cursor->enableRelativeMotion();

	// Start main loop.
	while(!esc->getClicked()) {
		core->update();


		if(fullscreen->getClicked()) {
			fullscreenflag = !fullscreenflag;
			window->setFullscreen(fullscreenflag);
		}

		if(left_mouse_button->getPressed()) {
			rx += static_cast<float>(y_axis->getDeltaValue()) * 0.01;
			ry += static_cast<float>(x_axis->getDeltaValue())* 0.01;
//			std::cout << rx << " : " << ry << std::endl;
		}

		rai->setActiveDepthTest(xdl::xdl_true);
		rai->clearColorTargets(0.0f, 0.305f, 0.596f, 1.0f);
		rai->clearDepthTarget(1.0f);

		xdl::xdl_float aspect_ratio = window->getWidth()/window->getHeight();

		tmath::mat4 proj, rotx, roty, trans, model;
		tmath::perspective(45.0f, aspect_ratio, 1.0f, 110.0f, proj);

		tmath::rotate_x(rx, rotx);
		tmath::rotate_y(ry, roty);
		tmath::translate(0.0f, 0.0f, -3.0f, trans);

		model =   trans * rotx * roty;

		sp->activate();
		sp->setUniformMatrix4(projViewMatrix, 1, proj);
		sp->setUniformMatrix4(modelMatrix, 1, model);
		sp->deactivate();

		rai->setActiveVertexArray(va);
		rai->setActiveShaderProgram(sp);

		rai->drawVertexArray(xdl::XDEVL_PRIMITIVE_TRIANGLES, 36);


		std::wstringstream tmp;
		tmp << L"OpenGL Demo using XdevLFont: FPS: " <<  (1.0/core->getDT());


		textLayoutSystem->setColor(255, 255, 255, 255);
		textLayoutSystem->printText(tmp.str(), -1, 0.95);
		rai->swapBuffers();

	}

	rai->destroy(vd);
	rai->destroy(va);
	rai->destroy(vs);
	rai->destroy(fs);
	rai->destroy(sp);

	xdl::destroyCore(core);

	return 0;
}
