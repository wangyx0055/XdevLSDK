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

#ifndef XDEVL_AUDIO_SOURCE_IMPL_H
#define XDEVL_AUDIO_SOURCE_IMPL_H

#include <XdevLAudio/XdevLAudioSource.h>

namespace xdl {

	/**
		@class XdevLAudioSourceAL
		@brief Source for audio data.
		@author Cengiz Terzibas
	*/
	class XdevLAudioSourceAL : public XdevLAudioSource {
			friend class XdevLAudioAL;
		public:
			XdevLAudioSourceAL();
			virtual ~XdevLAudioSourceAL();
			virtual void setAudioBuffer(XdevLAudioBuffer* buffer) override;
			virtual void setPitch(xdl_float pitch) override;
			virtual void setGain(xdl_float gain) override;
			virtual void setPlayPosSec(xdl_float pos) override;
			virtual void setPlayPosSample(xdl_int pos) override;
			virtual xdl_float getPlayPosSec() override;
			virtual xdl_int getPlayPosSample() override;
			virtual void setLoop(xdl_bool state) override;
			virtual xdl_bool isPlaying() override;
			virtual void play() override;
			virtual void stop() override;
			virtual void pause() override;
			virtual const xdl_uint& getID() const;

		private:

			/// Holds the source identificator.
			ALuint				m_id;

			/// Holds the assigned XdevLAudioBuffer.
			XdevLAudioBuffer*	m_audioBuffer;
	};

}

#endif
