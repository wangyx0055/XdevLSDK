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

#ifndef XDEVL_AUDIO_BUFFER_H
#define XDEVL_AUDIO_BUFFER_H

#include <XdevLTypes.h>

namespace xdl {

	class XdevLAudioSource;
	
	/**
		@class XdevLAudioBuffer
		@brief Buffer that holds audio data.
		@author Cengiz Terzibas
	*/
	class XdevLAudioBuffer {
		public:
			virtual ~XdevLAudioBuffer() {};

			virtual xdl_int getFormat() = 0;

			/// Returns the size of the audio buffer in bytes.
			/**
				@return Size of the buffer in bytes.
			*/
			virtual xdl_int getSize() = 0;
			/// Returns the frequence of the audio data.
			/**
				@return The base frequence of the recorded audio data.
			*/
			virtual xdl_int getSamplingRate() = 0;
			/// Returns the number of channels.
			/**
				@return The number of channels used in this buffer.
			*/
			virtual xdl_int getChannels() = 0;
			/// Returns the resolution of one audio data element in bits.
			/**
				@return The resolution of one element in the audio buffer in bits.
			*/
			virtual xdl_int getBits() = 0;

			/// Returns the identification code.
			virtual xdl_uint getID()= 0;

			/// Returns a pointer to the data.
			virtual xdl_uint8* getData() = 0;

			/// Lock the buffer for modification.
			/**
				Locks the buffer for modification. Before one can upload/change data
				the buffer needs to be locked. After finishing the job unlock the buffer.
			*/
			virtual xdl_int lock() = 0;

			/// Unlock the buffer after modification.
			virtual xdl_int unlock() = 0;

			/// Upload data to the buffer.
			/**
				The buffer maybe be rezied dependend on the size value.

				@param src The pointer to the data array to upload.
				@param size The the number of bytes to upload from the 'src'.
			*/
			virtual xdl_int upload(xdl_int8* src, xdl_uint size) = 0;

	};

	typedef XdevLAudioBuffer	IXdevLAudioBuffer;
	typedef std::shared_ptr<XdevLAudioBuffer>	IPXdevLAudioBuffer;

}

#endif
