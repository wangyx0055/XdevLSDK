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

#ifndef XDEVL_AUDIO_SOURCE_H
#define XDEVL_AUDIO_SOURCE_H

#include <XdevLTypes.h>

namespace xdl {

	/**
		@class XdevLAudioSource
		@brief Audio source in space.
		@author Cengiz Terzibas

		An audio source means a source that creates sound. In real life this could be everything.
		For instance a telephone, car, plane, steps of a human etc. A source can have
		a direction, position and velocity.
	*/
	class XdevLAudioSource {
			friend class XdevLAudio;
		public:
			virtual ~XdevLAudioSource() {};

			virtual void setAudioBuffer(XdevLAudioBuffer* buffer) = 0;

			/// Sets the pitch of the source.
			/**
				@param pitch Changes the frequence of the source.
			*/
			virtual void setPitch(xdl_float pitch) = 0;

			/// Sets the gain of the source.
			/**
				@param gain Controlls the gain (volume) of the source.
			*/
			virtual void setGain(xdl_float gain) = 0;

			/// Sets the play position
			/**
				@param pos Sets the play position. This value
				uses second units.
			*/
			virtual void setPlayPosSec(xdl_float pos) = 0;

			/// Sets the play position
			/**
				@param pos Sets the play position. This value
				uses sample units.
			*/
			virtual void setPlayPosSample(xdl_int pos) = 0;

			/// Returns the play position
			/**
				@return Returns the current play position in second units.
			*/
			virtual xdl_float getPlayPosSec() = 0;

			/// Returns the play position
			/**
				@return Returns the current play position in sample units.
			*/
			virtual xdl_int getPlayPosSample() = 0;

			/// Sets the loop state of the source
			/**
				@param state Set state = false if the source shouldn't
				be looped. Default value for the loop state of the
				source is true.
			*/
			virtual void setLoop(xdl_bool state) = 0;

			/// Returns the playing state of the source
			/**
				@return Returns true if the source is playing
				at the moment. Otherwise it returns false.
			*/
			virtual xdl_bool isPlaying() = 0;

			/// Starts to play a source.
			virtual void play() = 0;

			/// Stops a playing source.
			virtual void stop() = 0;

			/// Pauses a playing source.
			virtual void pause() = 0;

			/// Returns the identification code.
			virtual const xdl_uint& getID() const = 0;
	};

	typedef XdevLAudioSource	IXdevLAudioSource;
	typedef std::shared_ptr<XdevLAudioSource>	IPXdevLAudioSource;

}

#endif
