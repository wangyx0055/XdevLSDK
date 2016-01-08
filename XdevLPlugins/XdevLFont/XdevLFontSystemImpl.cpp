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

#include <XdevLFont/XdevLFontSystemImpl.h>

#include "XdevLFontImpl.h"
#include "lodepng.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cassert>


namespace xdl {

	XdevLFontSystemImpl::XdevLFontSystemImpl(XdevLModuleCreateParameter* parameter, xdl::XdevLModuleDescriptor& moduleDescriptor) :
		XdevLModuleImpl<XdevLFontSystem>(parameter, moduleDescriptor),
		screenWidth(0),
		screenHeight(0),
		m_rai(nullptr),
		createTextureFromFile(nullptr) {
	}

	XdevLFontSystemImpl::~XdevLFontSystemImpl() {
	}

	xdl_uint XdevLFontSystemImpl::getScreenWidth() const {
		return screenWidth;
	}

	xdl_uint XdevLFontSystemImpl::getScreenHeight() const {
		return screenHeight;
	}

	xdl_int XdevLFontSystemImpl::init(xdl_uint screenWidth, xdl_uint screenHeight, IPXdevLRAI rai) {
		m_rai = rai;
		m_rai->getDescriptor().registerDependency(this);
		return ERR_OK;
	}

	xdl_int XdevLFontSystemImpl::shutdown() {
		return ERR_OK;
	}


	void XdevLFontSystemImpl::setCreateTextureCallback(XdevLFontSystem::createTextureFromFileCallbackFunction function) {
		assert(function && " XdevLFontImpl::setCreateTextureCallback: Parameter not valid.");
		createTextureFromFile = function;
	}

	IPXdevLFont XdevLFontSystemImpl::createFromFontFile(const XdevLFileName& fontInfoFilename) {
		assert(m_rai && " XdevLFontImpl::createFromFontFile: XdevLFontSystem not initialized.");

		std::vector<std::string> fileNames;
		xdl_int numberOfTextures = 1;
		xdl_uint fontSize = 0;

		std::ifstream infile(fontInfoFilename.toString().c_str());
		if(infile.is_open()) {
			std::string tmp;
			std::getline(infile, tmp);
			std::getline(infile, tmp);

			std::stringstream ss(tmp);
			std::getline(infile, tmp);
			ss << tmp;
			ss >> fontSize;

			auto font = new XdevLFontImpl();
			font->setFontSize(fontSize);

			ss.str("");
			ss.clear();

			std::getline(infile, tmp);
			ss << tmp;
			ss >> numberOfTextures;

			IPXdevLTexture texture = nullptr;

			for(auto id = 0; id < numberOfTextures; id++) {
				std::string filename;
				std::getline(infile, filename);

				// Did the user specify a external functions to create a texture out of an image file?
				if(createTextureFromFile) {
					// Yes, they use that to create the texture.
					texture = createTextureFromFile(XdevLFileName(filename));
				} else {
					// No, let's use the lodepng project import PNG files.
					std::vector<xdl_uint8> image;
					xdl_uint width, height;

					xdl_int error = lodepng::decode(image, width, height, filename.c_str());
					if(error) {
						std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
						return std::shared_ptr<XdevLFontImpl>();
					}

					// This flipping is neccessary because the library flips the picture up side down.
					// Method is brute force and needs unessarly memory. It creates a vector the same size
					// and starts copying which is not effective at all.
					std::vector<xdl_uint8> vflipedimage(image);
					for(xdl_uint y = 0; y != height; y++) {
						for(xdl_uint x = 0; x != width*4; x++) {
							vflipedimage[x + y*width*4] = image[x + (height - 1 - y)*width*4];
						}
					}

					m_rai->createTexture(&texture);

					texture->init(width, height, XDEVL_RGBA, XDEVL_RGBA, vflipedimage.data());
					image.clear();
					vflipedimage.clear();
				}
				if(texture == nullptr) {
					std::cout << "XdevLFontImpl::importFromFontFile: Could not create texture: " << filename << std::endl;
					assert(texture && "FontTexture not created.");
				}
				texture->lock();
				texture->setTextureFilter(XDEVL_TEXTURE_MIN_FILTER, XDEVL_NEAREST);
				texture->setTextureFilter(XDEVL_TEXTURE_MAG_FILTER, XDEVL_NEAREST);
				texture->setTextureWrap(XDEVL_TEXTURE_WRAP_S, XDEVL_CLAMP_TO_EDGE);
				texture->setTextureWrap(XDEVL_TEXTURE_WRAP_T, XDEVL_CLAMP_TO_EDGE);
				texture->unlock();

				font->addFontTexture(texture);

			}

			calculateGlyphInformation(font, infile);

			return std::shared_ptr<XdevLFontImpl>(font);
		}

		return nullptr;

	}


	IPXdevLFont XdevLFontSystemImpl::createFontFromTexture(const XdevLFileName& fontInfoFilename, IPXdevLTexture texture) {
		assert(m_rai && " XdevLFontImpl::createFontFromTexture: XdevLFontSystem not initialized.");

		std::ifstream infile(fontInfoFilename);
		if(!infile.is_open()) {
			return nullptr;
		}
		auto font = std::shared_ptr<XdevLFontImpl>(new XdevLFontImpl());

		IPXdevLTexture tx = texture;
		font->addFontTexture(texture);

		tx->lock();
		tx->setTextureFilter(XDEVL_TEXTURE_MIN_FILTER, XDEVL_NEAREST);
		tx->setTextureFilter(XDEVL_TEXTURE_MAG_FILTER, XDEVL_NEAREST);
		tx->setTextureWrap(XDEVL_TEXTURE_WRAP_S, XDEVL_CLAMP_TO_EDGE);
		tx->setTextureWrap(XDEVL_TEXTURE_WRAP_T, XDEVL_CLAMP_TO_EDGE);
		tx->unlock();


		// One unit on the screen for x,y [-1 , 1]
		font->setUnitX(2.0f/(xdl_float)screenWidth);
		font->setUnitY(2.0f/(xdl_float)screenHeight);

		xdl_uint numberOfTextures = 1;
		xdl_uint fontSize = 0;


		std::string tmp;
		std::getline(infile, tmp);
		std::getline(infile, tmp);

		std::getline(infile, tmp);
		std::stringstream ss(tmp);
		ss >> numberOfTextures;
		ss.str("");
		ss.clear();

		// Get font size.
		std::getline(infile, tmp);
		ss << tmp;
		ss >> fontSize;

		font->setFontSize(fontSize);

		xdl_float numCols 	= (xdl_float)tx->getWidth()/(xdl_float)fontSize;

		// TODO Using maps to handle id of the glyphs? At the moment it is just a hack.
		while(!infile.eof()) {

			XdevLGlyphMetric gp;
			readLine(infile, gp);

			//
			// TODO This part to find the position of the glyph in the texture is some sort of hack.
			// Make it so that all of the information is the the fontInfo.txt file.
			//
			xdl_uint idx = gp.character_code - 32;

			xdl_float pos_x = idx % (xdl_int)numCols;
			xdl_float pos_y = idx / (xdl_int)numCols;

			xdl_float ds = 1.0/(xdl_float)tx->getWidth()*gp.width;
			xdl_float dt = 1.0/(xdl_float)tx->getWidth()*gp.height;

			xdl_float s = 1.0/numCols*pos_x ;
			xdl_float t = 1.0 - 1.0/numCols*pos_y - (1.0/(xdl_float)tx->getHeight())*((fontSize - gp.height - gp.top));

			//
			// Add an offset of x,y pixel offset to the s,t coordinates.
			//
			xdl_float offset_x = 0.0/(xdl_float)tx->getWidth();
			xdl_float offset_y = 0.0/(xdl_float)tx->getHeight();


			gp.vertices[0].x = gp.brearing_x;
			gp.vertices[0].y = (gp.height - gp.brearing_y);
			gp.vertices[0].s = s - offset_x;
			gp.vertices[0].t = t - dt - offset_y;

			gp.vertices[1].x = gp.brearing_x;
			gp.vertices[1].y = gp.brearing_y;
			gp.vertices[1].s = s - offset_x;
			gp.vertices[1].t = t + offset_y;

			gp.vertices[2].x = gp.brearing_x + gp.width;
			gp.vertices[2].y = gp.brearing_y;
			gp.vertices[2].s = s + ds + offset_x;
			gp.vertices[2].t = t + offset_y;

			gp.vertices[3].x = gp.brearing_x + gp.width;
			gp.vertices[3].y = (gp.height - gp.brearing_y);
			gp.vertices[3].s = s + ds + offset_x;
			gp.vertices[3].t = t - dt - offset_y;

			//
			// Find maximum value for the new line.
			//
			font->setNewLineSize(std::max(font->getNewLineSize(), gp.height));

			font->addGlyph(gp);

		}
		return font;

	}

	XdevLGlyphMetric& XdevLFontSystemImpl::readLine(std::ifstream& os, XdevLGlyphMetric& gp) {
		os >> gp.texture_id
		   >> gp.character_code
		   >> gp.left
		   >> gp.top
		   >> gp.width
		   >> gp.height
		   >> gp.advance_h
		   >> gp.advance_v
		   >> gp.brearing_x
		   >> gp.brearing_y
		   >> gp.vertices[0].s
		   >> gp.vertices[0].t
		   >> gp.vertices[3].s
		   >> gp.vertices[3].t;

		return gp;
	}

	void XdevLFontSystemImpl::calculateGlyphInformation(XdevLFontImpl* font, std::ifstream& os) {

		xdl_uint	count = 0;

		// TODO Using maps to handle id of the glyphs? At the moment it is just a hack.
		while(!os.eof()) {

			XdevLGlyphMetric gp;
			readLine(os, gp);

			//
			// Get the info for the glyph.
			//

			IPXdevLTexture tx = font->getTexture(gp.texture_id);

			//
			// Add an offset of x,y pixel offset to the x,y coordinates.
			// What this doesn is expanding the size of the quad in the same amound
			//
			xdl_float offset_sx = 0.0;
			xdl_float offset_sy = 0.0;

			//
			// Convert the pixel units into texture coordinate units
			//
			xdl_float s1 = (1.0f/(xdl_float)tx->getWidth())*gp.vertices[0].s;
			xdl_float t1 = (1.0f/(xdl_float)tx->getWidth())*gp.vertices[0].t;

			xdl_float s2 = (1.0f/(xdl_float)tx->getWidth())*gp.vertices[3].s;
			xdl_float t2 = (1.0f/(xdl_float)tx->getWidth())*gp.vertices[3].t;


			gp.vertices[0].x = gp.brearing_x - offset_sx;
			gp.vertices[0].y = (gp.height - gp.brearing_y) - offset_sy;
			gp.vertices[0].s = s1;
			gp.vertices[0].t = t2;

			gp.vertices[1].x = gp.brearing_x - offset_sx;
			gp.vertices[1].y = gp.brearing_y + offset_sy;
			gp.vertices[1].s = s1;
			gp.vertices[1].t = t1;

			gp.vertices[2].x = gp.brearing_x + gp.width + offset_sx;
			gp.vertices[2].y = gp.brearing_y + offset_sy;
			gp.vertices[2].s = s2;
			gp.vertices[2].t = t1;

			gp.vertices[3].x = gp.brearing_x + gp.width + offset_sx;
			gp.vertices[3].y = (gp.height - gp.brearing_y) - offset_sy;
			gp.vertices[3].s = s2;
			gp.vertices[3].t = t2;


			//
			// Find maximum value for the new line.
			//
			font->setNewLineSize(std::max(font->getNewLineSize(), gp.height));

			//
			// Store that glyph in the map
			//
			font->addGlyph(gp);

			count++;

		}
	}

}
