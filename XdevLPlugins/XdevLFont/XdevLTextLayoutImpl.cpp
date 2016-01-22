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

#include "XdevLTextLayoutImpl.h"
#include <XdevLFont/XdevLFontImpl.h>

namespace xdl {

	std::string vertex_shader_330(
"layout(location = 0) in vec2 iposition;                            \
layout(location = 4) in vec4 icolor;                                \
layout(location = 9) in vec2 itexCoord;                             \
                                                                    \
uniform mat4 projMatrix = mat4(1.0f, 0.0f, 0.0f, 0.0f,              \
                                        0.0f, 1.0f, 0.0f, 0.0f,     \
                                        0.0f, 0.0f, 1.0f, 0.0f,     \
                                        0.0f, 0.0f, 0.0f, 1.0f);    \
                                                                    \
out vec2 tcoord;                                                    \
out vec4 color;                                                     \
                                                                    \
void main(void) {                                                   \
                                                                    \
	vec4 pos = projMatrix * vec4(iposition.xy, 0, 1);               \
                                                                    \
	tcoord = itexCoord;                                             \
                                                                    \
	color =  icolor;                                                \
                                                                    \
	gl_Position = pos;                                              \
                                                                    \
}                                                                   \
");

	std::string fragment_shader_simple_font_330(
"                                                                                       \
in vec4 color;                                                                          \
in vec2 tcoord;                                                                         \
                                                                                        \
uniform sampler2D texture0;                                                             \
                                                                                        \
uniform float buffer            = 0.8f;                                                 \
uniform float gamma             = 0.4f;                                                 \
uniform int 	dft             = 1;                                                    \
uniform int 	effect          = 2;                                                    \
uniform vec4 	glowColor       = vec4(0.1, 0.1, 0.1, 1.0);                             \
uniform vec2	shadowOffset    = vec2(-0.004, 0.004);                                  \
out vec4 ocolor;                                                                        \
                                                                                        \
void main(void) {                                                                       \
                                                                                        \
    vec4 basecolor = color;                                                             \
                                                                                        \
    basecolor = vec4(color.r, color.g, color.b, texture(texture0, tcoord).a);           \
    if(effect == 1) {                                                                   \
        basecolor += glowColor*texture(texture0, tcoord + shadowOffset);                \
    }                                                                                   \
    ocolor = basecolor;                                                                 \
}                                                                                       \
");


	std::string fragment_shader_330(
"                                                                                       \
in vec4 color;                                                                          \
in vec2 tcoord;                                                                         \
\
uniform sampler2D texture0;                                                             \
\
uniform float smooth_step_buffer 	= 0.8f;                                               \
uniform float gamma 	= 0.4f;                                                         \
uniform int 	dft			= 1;                                                        \
uniform int 	effect 	= 2;                                                            \
uniform vec4 	glowColor = vec4(0.1, 0.1, 0.1, 1.0);                                   \
uniform vec2	shadowOffset = vec2(-0.004, 0.004);                                     \
out vec4 ocolor;                                                                        \
                                                                                        \
void main(void) {                                                                       \
                                                                                        \
    vec4 basecolor = color;                                                             \
                                                                                        \
    if(dft == 0) {                                                                      \
        basecolor = vec4(color.r, color.g, color.b, texture(texture0, tcoord).a);       \
        if(effect == 1) {                                                               \
            basecolor += glowColor*texture(texture0, tcoord + shadowOffset);            \
        }                                                                               \
    } else {                                                                            \
        if(dft == 1) {                                                                  \
                                                                                        \
            float dist = texture(texture0, tcoord).a;                                   \
            basecolor = color*smoothstep(smooth_step_buffer - gamma, smooth_step_buffer + gamma, dist);         \
                                                                                        \
            if(effect == 1) {                                                           \
                float glowTexel 	=  texture(texture0, tcoord + shadowOffset).a;      \
                vec4 glowc				=  glowColor*smoothstep(0.3, 0.5, glowTexel);   \
                                                                                        \
                basecolor = glowc + basecolor;                                          \
                                                                                        \
            } else if(effect == 2) {                                                    \
                float glowTexel 	=  texture(texture0, tcoord).a;                     \
                vec4 glowc				=  glowColor*smoothstep(0.1, 0.2, glowTexel);   \
                                                                                        \
                basecolor = glowc + basecolor;                                          \
                                                                                        \
            } else {                                                                    \
                basecolor *= color*smoothstep(smooth_step_buffer - gamma, smooth_step_buffer + gamma, dist);    \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
    ocolor = basecolor;                                                                 \
}                                                                                       \
");

	template<typename T>
	xdl_float* ortho(T left, T right, T bottom, T top, T near_plane_distance, T far_plane_distance, xdl_float* mat) {
		T d = (right-left);
		T c = (top-bottom);
		mat[0] = 2.0f/d;
		mat[1] = 0.0f;
		mat[2] = 0.0f;
		mat[3] = 0.0f;
		mat[4] = 0.0f;
		mat[5] = 2.0f/c;
		mat[6] = 0.0f;
		mat[7] = 0.0f;
		mat[8] = 0.0f;
		mat[9] = 0.0f;
		mat[10] = -2.0f / (far_plane_distance - near_plane_distance);
		mat[11] = 0.0f;
		mat[12] = -(right + left) / d;
		mat[13] = -(top + bottom) / c;
		mat[14] = -(far_plane_distance + near_plane_distance) / (far_plane_distance - near_plane_distance);
		mat[15] = 1.0f;


		return mat;
	}

	XdevLTextLayoutImpl::XdevLTextLayoutImpl(XdevLModuleCreateParameter* parameter,  const XdevLModuleDescriptor& descriptor) :
		XdevLModuleImpl<XdevLTextLayout>(parameter,  descriptor),
		m_screenWidth(0),
		m_screenHeight(0),
		m_window(nullptr),
		m_rai(nullptr),
		m_font(nullptr),
		m_shaderProgram(nullptr),
		m_vertexShader(nullptr),
		m_fragmentShader(nullptr),
		m_texture(nullptr),
		m_vertexArray(nullptr),
		m_vd(nullptr),
		m_vertexBuffer(nullptr),
		m_staticVertexArray(nullptr),
		m_staticVertexBuffer(nullptr),
		m_gamma(0.2f),
		m_buffer(0.4f),
		m_dft(0),
		m_dpi(42.0f),
		m_scale(1.0f),
		m_rescale(1.0f),
		m_effectNumber(1),
		m_staticVertexListUploaded(xdl_false),
		m_usePixelUnits(xdl_false) {
		m_shadowOffset[0] = 0.0f;
		m_shadowOffset[1] = 0.0f;
	}

	xdl_int XdevLTextLayoutImpl::shutdown() {
		return ERR_OK;
	}

	xdl_int XdevLTextLayoutImpl::notify(xdl::XdevLEvent& event) {
		switch(event.type) {
			case xdl::XDEVL_WINDOW_EVENT: {
				switch(event.window.event) {

					//
					// Window size got changed we have to adapt here.
					//
					case xdl::XDEVL_WINDOW_MOVED:
					case xdl::XDEVL_WINDOW_RESIZED: {
//						m_screenWidth = event.window.width;
//						m_screenHeight = event.window.height;
					}
					break;
				}
			}
			break;
		}
		return XdevLModuleImpl::notify(event);
	}

	void XdevLTextLayoutImpl::usePixelUnits(xdl_bool state) {
		m_usePixelUnits = state;
	}

	void XdevLTextLayoutImpl::setScreenDimensions(xdl_uint screenWidth, xdl_uint screenHeight) {
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;
	}


	void XdevLTextLayoutImpl::setShadowOffset(xdl_float xOffset, xdl_float yOffset) {
		m_shadowOffset[0] = xOffset;
		m_shadowOffset[1] = yOffset;
	}

	xdl_int XdevLTextLayoutImpl::init(IPXdevLWindow window, IPXdevLRAI rai) {
		m_rai = rai;
		m_rai->getDescriptor().registerDependency(this);
		m_window = window;
		m_screenWidth = window->getWidth();
		m_screenHeight = window->getHeight();

		m_shaderProgram = m_rai->createShaderProgram();
		m_vertexShader = m_rai->createVertexShader();
		m_fragmentShader = m_rai->createFragmentShader();
		m_texture = m_rai->createTexture();

		m_vertexShader->addShaderCode(vertex_shader_330.c_str());
		if(m_vertexShader->compile() != ERR_OK) {
			std::cerr << "XdevLFont2D::init: Could not create vertex shader." << std::endl;
			return ERR_ERROR;
		}

		m_fragmentShader->addShaderCode(fragment_shader_330.c_str());
		if(m_fragmentShader->compile() != ERR_OK) {
			std::cerr << "XdevLFont2D::init: Could not create fragment shader." << std::endl;
			return ERR_ERROR;
		}

		m_shaderProgram->attach(m_vertexShader);
		m_shaderProgram->attach(m_fragmentShader);
		m_shaderProgram->link();

		m_projMatrix			= m_shaderProgram->getUniformLocation("projMatrix");
		m_texture0 				= m_shaderProgram->getUniformLocation("texture0");
		m_gammaid				= m_shaderProgram->getUniformLocation("gamma");
		m_bufferid				= m_shaderProgram->getUniformLocation("smooth_step_buffer");
		m_dftid					= m_shaderProgram->getUniformLocation("dft");
		m_effectid				= m_shaderProgram->getUniformLocation("effect");
		m_shadowOffsetid		= m_shaderProgram->getUniformLocation("shadowOffset");

		m_vd = rai->createVertexDeclaration();
		m_vd->add(2, XDEVL_BUFFER_ELEMENT_FLOAT, XDEVL_VERTEX_POSITION);
		m_vd->add(4, XDEVL_BUFFER_ELEMENT_UNSIGNED_BYTE, XDEVL_VERTEX_COLOR);
		m_vd->add(2, XDEVL_BUFFER_ELEMENT_FLOAT, XDEVL_VERTEX_TEXTURE_COORD);


		m_vertexBuffer = m_rai->createVertexBuffer();
		m_vertexBuffer->init();
		m_vertexArray = m_rai->createVertexArray();
		m_vertexArray->init(m_vertexBuffer, m_vd);


		m_staticVertexBuffer = m_rai->createVertexBuffer();
		m_staticVertexBuffer->init();
		m_staticVertexArray = m_rai->createVertexArray();
		m_staticVertexArray->init(m_staticVertexBuffer, m_vd);

		return ERR_OK;
	}

	void XdevLTextLayoutImpl::useFont(IPXdevLFont font) {
		assert(font && "XdevLTextLayoutImpl::render: Font value invalid (nullptr).");

		m_font = font;
		//
		// Set two pixel shadow offset.
		//
		m_shadowOffset[0] = -3.0f*(1.0f/(xdl_float)font->getTexture(0)->getWidth());
		m_shadowOffset[1] = 3.0f*(1.0f/(xdl_float)font->getTexture(0)->getHeight());
	}

	void  XdevLTextLayoutImpl::addDynamicText(const std::wstring&  text, xdl_float x, xdl_float y) {
		XdevLTextLayoutText info;
		info.text = text;
		info.x = x;
		info.y = y;
		info.color[0] = m_currentColor[0];
		info.color[1] = m_currentColor[1];
		info.color[2] = m_currentColor[2];
		info.color[3] = m_currentColor[3];
		info.scale    = m_rescale;
		m_textList.push_back(info);

	}


	void XdevLTextLayoutImpl::render() {
		assert(m_font && "XdevLTextLayoutImpl::render: Font not set. Use 'useFont' before calling the draw method.");
		//
		// Do we have anything to draw?
		//
		if(m_textList.size() == 0) {
			return;
		}


		m_rai->setActiveShaderProgram(m_shaderProgram);

		//
		// Set Signed Distance Field shader stuff.
		//
		m_shaderProgram->setUniform(m_bufferid, 			m_buffer);
		m_shaderProgram->setUniform(m_gammaid,  			m_gamma);
		m_shaderProgram->setUniformi(m_dftid, 				m_dft);
		m_shaderProgram->setUniformi(m_effectid, 			m_effectNumber);
		m_shaderProgram->setUniform2v(m_shadowOffsetid, 1,	m_shadowOffset);

		//
		// Set transformation stuff.
		//
//		tmath::mat4 proj,trans, m;
//		tmath::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f, proj);
//		tmath::translate(0.0f, 0.0f, -1.8f, trans);
//		tmath::roty(56.0f, m);
//		m_shaderProgram->setUniformMatrix4(m_projViewModelMatrix, 1, proj*trans*m);


		//
		// Activate the neccessary texture.
		//
		m_shaderProgram->setUniformi(m_texture0, 0);


		//
		// Set some rendering states.
		//
		m_rai->setActiveBlendMode(xdl_true);
		m_rai->setBlendMode(XDEVL_BLEND_SRC_ALPHA, XDEVL_BLEND_ONE_MINUS_SRC_ALPHA);
		m_rai->setActiveDepthTest(xdl_false);

		//
		// Create the vertices for the dynamic text.
		//
		layoutVertexBuffer(m_textList, m_dynamicVertexMap);

		//
		// Draw all glyphs.
		//
		for(auto& ib : m_dynamicVertexMap) {

			//
			// Activate the corresponding texture.
			//
			m_font->getTexture(ib.first)->activate(0);


			//
			// Upload the vertex data to the GPU.
			//
			m_vertexBuffer->lock();
			m_vertexBuffer->upload((xdl_uint8*)ib.second.data(), ib.second.size()* sizeof(XdevLGlyphVertex));
			m_vertexBuffer->unlock();

			//
			// Draw everything.
			//
			m_rai->setActiveVertexArray(m_vertexArray);
			m_rai->drawVertexArray(XDEVL_PRIMITIVE_TRIANGLES, ib.second.size());
		}
		m_rai->setActiveBlendMode(xdl_false);

		//
		// Clear the dynamic text list.
		//
		m_textList.clear();
		m_dynamicVertexMap.clear();
		m_shaderProgram->deactivate();
	}

	void XdevLTextLayoutImpl::layoutVertexBuffer(std::vector<XdevLTextLayoutText>& textList, std::map<xdl_uint, std::vector<XdevLGlyphVertex> >& vertexList) {
		assert(m_font && "XdevLTextLayoutImpl::render: Font not set. Use 'useFont' before calling the draw method.");

		//
		// Go through all text lines.
		//
		for(auto& textLine : textList) {

			//
			// Set cursor position.
			//
			xdl_float pen_x = textLine.x;
			xdl_float pen_y = textLine.y;

			//
			// Set the current scale for the whole line.
			// TODO Shall we do that for each letter?
			//
			m_rescale = textLine.scale;


			xdl_float unit_x 	=  m_rescale;
			xdl_float unit_y 	=  m_rescale;

			// Check if we use relative units. Mean not using pixel units.
			if(m_usePixelUnits == xdl_false) {
				unit_x *=  2.0/(xdl_float)m_screenWidth;
				unit_y *=  2.0/(xdl_float)m_screenHeight;
			}

			//
			// Go through all letters.
			//

			for(auto& letter : textLine.text) {

				//
				// Get the index for the letter.
				//
				char16_t idx = letter;

				//
				// Get the letters corrosponding glph info.
				//
				XdevLGlyphMetric glyphProperties = m_font->getGlyphMetric(idx);

				//
				// Is it a new line?
				//
				if(idx == '\n') {
					pen_x = textLine.x;
					pen_y -= unit_y * m_font->getNewLineSize();
					continue;
				}

				//
				// Or a white space?
				//
				else if(idx == ' ') {
					pen_x += unit_x * (xdl_float)(glyphProperties.advance_h);
					continue;
				}

				//
				// Prepare the vertices for 2 triangles.
				//
				// ->
				// 1-----2
				// |  /  |
				// 0-----3
				//

				XdevLGlyphVertex bottom_left, top_left, top_right, bottom_right;

				bottom_left.x = pen_x + unit_x * glyphProperties.vertices[0].x;
				bottom_left.y = pen_y - unit_y * glyphProperties.vertices[0].y;
				bottom_left.s = glyphProperties.vertices[0].s;
				bottom_left.t = glyphProperties.vertices[0].t;
				bottom_left.r = textLine.color[0];
				bottom_left.g = textLine.color[1];
				bottom_left.b = textLine.color[2];
				bottom_left.a = textLine.color[3];

				top_left.x = pen_x + unit_x * glyphProperties.vertices[1].x;
				top_left.y = pen_y + unit_y * glyphProperties.vertices[1].y;
				top_left.s = glyphProperties.vertices[1].s;
				top_left.t = glyphProperties.vertices[1].t;
				top_left.r = textLine.color[0];
				top_left.g = textLine.color[1];
				top_left.b = textLine.color[2];
				top_left.a = textLine.color[3];

				top_right.x = pen_x + unit_x * glyphProperties.vertices[2].x;
				top_right.y = pen_y + unit_y * glyphProperties.vertices[2].y;
				top_right.s = glyphProperties.vertices[2].s;
				top_right.t = glyphProperties.vertices[2].t;
				top_right.r = textLine.color[0];
				top_right.g = textLine.color[1];
				top_right.b = textLine.color[2];
				top_right.a = textLine.color[3];

				bottom_right.x = pen_x + unit_x * glyphProperties.vertices[3].x;
				bottom_right.y = pen_y - unit_y * glyphProperties.vertices[3].y;
				bottom_right.s = glyphProperties.vertices[3].s;
				bottom_right.t = glyphProperties.vertices[3].t;
				bottom_right.r = textLine.color[0];
				bottom_right.g = textLine.color[1];
				bottom_right.b = textLine.color[2];
				bottom_right.a = textLine.color[3];

				std::vector<XdevLGlyphVertex>& currentTexture = vertexList[glyphProperties.texture_id];

				currentTexture.push_back(bottom_left);
				currentTexture.push_back(top_left);
				currentTexture.push_back(top_right);
				currentTexture.push_back(bottom_left);
				currentTexture.push_back(top_right);
				currentTexture.push_back(bottom_right);

				pen_x += unit_x * (glyphProperties.advance_h);
			}

		}
	}

	void XdevLTextLayoutImpl::printText(const std::wstring& text, xdl_float x, xdl_float y) {

		std::vector<XdevLTextLayoutText> textList;

		XdevLTextLayoutText info;
		info.text = text;
		info.x = x;
		info.y = y;
		info.color[0] = m_currentColor[0];
		info.color[1] = m_currentColor[1];
		info.color[2] = m_currentColor[2];
		info.color[3] = m_currentColor[3];
		info.scale    = m_rescale;
		textList.push_back(info);


		m_rai->setActiveShaderProgram(m_shaderProgram);

		//
		// Set Signed Distance Field shader stuff.
		//
		m_shaderProgram->setUniform(m_bufferid, 	m_buffer);
		m_shaderProgram->setUniform(m_gammaid,  	m_gamma);
		m_shaderProgram->setUniformi(m_dftid, 		m_dft);
		m_shaderProgram->setUniformi(m_effectid, 	m_effectNumber);
		m_shaderProgram->setUniform2v(m_shadowOffsetid,1 , m_shadowOffset);


		//
		// Activate the neccessary texture.
		//
		m_shaderProgram->setUniformi(m_texture0, 0);

		//
		// Check if we have to use pixel units.
		//
		if(m_usePixelUnits) {
			xdl_float projectionMatrix[16];
			ortho<xdl_float>(0.0f,
			                 (xdl::xdl_float)m_screenWidth,
			                 0.0f,
			                 (xdl::xdl_float)m_screenHeight,
			                 -1.0f,
			                 1.0f, projectionMatrix);

			m_shaderProgram->setUniformMatrix4(m_projMatrix, 1, projectionMatrix);
		}

		//
		// Set some rendering states.
		//
		m_rai->setActiveBlendMode(xdl_true);
		m_rai->setBlendMode(XDEVL_BLEND_SRC_ALPHA, XDEVL_BLEND_ONE_MINUS_SRC_ALPHA);
		m_rai->setActiveDepthTest(xdl_false);


		//
		// Create the vertices for the dynamic text.
		//
		layoutVertexBuffer(textList, m_simpleTextVertexMap);

		//
		// Draw all glyphs.
		//
		for(auto& ib : m_simpleTextVertexMap) {

			//
			// Activate the corresponding texture.
			//
			m_font->getTexture(ib.first)->activate(0);


			//
			// Upload the vertex data to the GPU.
			//
			m_vertexBuffer->lock();
			m_vertexBuffer->upload((xdl_uint8*)ib.second.data(), ib.second.size()* sizeof(XdevLGlyphVertex));
			m_vertexBuffer->unlock();

			//
			// Draw everything.
			//
			m_rai->setActiveVertexArray(m_vertexArray);
			m_rai->drawVertexArray(XDEVL_PRIMITIVE_TRIANGLES, ib.second.size());
		}

		m_rai->setActiveBlendMode(xdl_false);

		//
		// Clear the dynamic text list.
		//
		m_textList.clear();
		m_simpleTextVertexMap.clear();
		m_shaderProgram->deactivate();
	}

	void XdevLTextLayoutImpl::setScale(xdl_float scale) {
		m_rescale = scale;
	}

	void  XdevLTextLayoutImpl::setColor(xdl_uint32 r, xdl_uint32 g, xdl_uint32 b, xdl_uint32 a) {
		m_currentColor[0] = r;
		m_currentColor[1] = g;
		m_currentColor[2] = b;
		m_currentColor[3] = a;
	}

	void XdevLTextLayoutImpl::setDPI(xdl_float dpi) {
		m_dpi = dpi;

		m_scale = m_dpi/(xdl_float)m_font->getFontSize();
	}

	void XdevLTextLayoutImpl::setDFT(xdl_bool enable) {
		m_dft = enable ? 1 : 0;
	}

	void  XdevLTextLayoutImpl::setEffect(xdl_uint effectNumber) {
		m_effectNumber = effectNumber;
	}


	void XdevLTextLayoutImpl::getTextDimension(const XdevLTextLayoutText& textLayout, xdl_float width, xdl_float height) {
		assert(m_font && "XdevLTextLayoutImpl::render: Font not set. Use 'useFont' before calling the draw method.");


		xdl_float w = 0.0f;
		xdl_float h = 0.0f;

		//
		// Set cursor position.
		//
		xdl_float pen_x = textLayout.x;
		xdl_float pen_y = textLayout.y;

		//
		// Set the current scale for the whole line.
		// TODO Shall we do that for each letter?
		//
		m_rescale = textLayout.scale;


		xdl_float unit_x 	=  m_rescale;
		xdl_float unit_y 	=  m_rescale;

		// Check if we use relative units. Mean not using pixel units.
		if(m_usePixelUnits == xdl_false) {
			unit_x *=  2.0/(xdl_float)m_screenWidth;
			unit_y *=  2.0/(xdl_float)m_screenHeight;
		}

		//
		// Go through all letters.
		//

		for(auto& letter : textLayout.text) {

			//
			// Get the index for the letter.
			//
			char16_t idx = letter;

			//
			// Get the letters corrosponding glph info.
			//
			XdevLGlyphMetric glyphProperties = m_font->getGlyphMetric(idx);

			//
			// Is it a new line?
			//
			if(idx == '\n') {
				pen_x = textLayout.x;
				pen_y -= unit_y * m_font->getNewLineSize();
				h = std::max(h, pen_y);
				continue;
			}

			//
			// Or a white space?
			//
			else if(idx == ' ') {
				pen_x += unit_x * (xdl_float)(glyphProperties.advance_h);
				continue;
			}

			pen_x += unit_x * (glyphProperties.advance_h);
			w = std::max(w, pen_x);
		}

		width = w;
		height = h;
	}
}
