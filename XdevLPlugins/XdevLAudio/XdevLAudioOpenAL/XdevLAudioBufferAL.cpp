/*
	XdevL eXtended DEVice Library.

	Copyright © 2005-2008 Cengiz Terzibas

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

#ifndef XDEVL_AUDIO_AL_IMPL_H
#define XDEVL_AUDIO_AL_IMPL_H

#include <iostream>
#include <XdevLPluginImpl.h>
#include <XdevLUtils.h>
#if defined(__APPLE__)
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif
#include <vector>

#include "XdevLAudioBufferAL.h"
#include "XdevLAudioSourceAL.h"

namespace xdl {

	extern std::string getALErrorString(ALenum err);


	XdevLAudioBufferAL::XdevLAudioBufferAL(ALenum format, ALsizei freq, ALsizei size ) :
		m_id(0),
		m_format(format),
		m_freq(freq),
		m_size(size),
		m_locked(xdl_false) {
		alGenBuffers(1, &m_id);
	}

	XdevLAudioBufferAL::~XdevLAudioBufferAL() {
		alDeleteBuffers(1, &m_id);
	};

	xdl_int XdevLAudioBufferAL::getSize() {
		xdl_int size = 0;
		alGetBufferi(m_id, AL_SIZE, &size);
		return size;
	}
	
	xdl_int XdevLAudioBufferAL::getFormat() {
		return m_format;
	}

	xdl_int XdevLAudioBufferAL::getSamplingRate() {
		xdl_int freq = 0;
		alGetBufferi(m_id, AL_FREQUENCY, &freq);
		return freq;
	}

	xdl_int XdevLAudioBufferAL::getChannels() {
		xdl_int chan = 0;
		alGetBufferi(m_id, AL_CHANNELS, &chan);
		return chan;
	}

	xdl_int XdevLAudioBufferAL::getBits() {
		xdl_int bits = 0;
		alGetBufferi(m_id, AL_BITS, &bits);
		return bits;
	}

	xdl_uint XdevLAudioBufferAL::getID() {
		return m_id;
	}

	xdl_uint8* XdevLAudioBufferAL::getData() {
		return nullptr;
	}

	xdl_int XdevLAudioBufferAL::lock() {
		m_locked = xdl_true;
		return ERR_OK;
	}

	xdl_int XdevLAudioBufferAL::unlock() {
		m_locked = xdl_false;
		return ERR_OK;
	}

	xdl_int XdevLAudioBufferAL::upload(xdl_int8* src, xdl_uint size) {
		XDEVL_ASSERT( (m_locked == xdl_true), "You must use lock() before you can upload data into the buffer");

		// Initialize AL buffer and upload data.
		alBufferData(m_id, m_format, (ALvoid*)src, size, m_freq);
		ALenum error = alGetError();
		if(error != AL_NO_ERROR) {
			std::cerr << getALErrorString(error) << std::endl;
			return ERR_ERROR;
		}
		m_size = size;
		return ERR_OK;
	}

}

#endif
