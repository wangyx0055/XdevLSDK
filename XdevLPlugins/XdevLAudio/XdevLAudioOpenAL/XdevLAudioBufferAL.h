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

#ifndef XDEVL_AUDIO_BUFFER_IMPL_H
#define XDEVL_AUDIO_BUFFER_IMPL_H

#include <XdevLAudio/XdevLAudioBuffer.h>

namespace xdl {

	extern std::string getALErrorString(ALenum err);

	/**
		@class XdevLAudioBufferAL
		@brief Holds the raw audio stream data.
		@author Cengiz Terzibas
	*/
	class XdevLAudioBufferAL : public XdevLAudioBuffer {
		public:

			XdevLAudioBufferAL(ALenum format, ALsizei freq, ALsizei size );
			virtual ~XdevLAudioBufferAL();

			virtual xdl_int getFormat() override;
			virtual xdl_int getSize() override;
			virtual xdl_int getSamplingRate()  override;
			virtual xdl_int getChannels()  override;
			virtual xdl_int getBits()  override;
			virtual xdl_uint getID() override;
			virtual xdl_uint8* getData()  override;
			virtual xdl_int lock()  override;
			virtual xdl_int unlock()  override;
			virtual xdl_int upload(xdl_int8* src, xdl_uint size)  override;

		private:
			ALuint m_id;
			ALenum m_format;
			ALsizei m_freq;
			ALsizei m_size;
			xdl_bool m_locked;
	};
}

#endif
