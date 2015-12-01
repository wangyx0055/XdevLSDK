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

#ifndef XDEVL_AUDIO_H
#define XDEVL_AUDIO_H

#include <XdevLModule.h>

namespace xdl {

	/**
		@enum XdevLAudioBufferFormat
		@brief Types of audio formats.
		@author Cengiz Terzibas
	*/
	enum XdevLAudioBufferFormat {
	    AUDIO_BUFFER_MONO8,
	    AUDIO_BUFFER_MONO16,
	    AUDIO_BUFFER_STEREO8,
	    AUDIO_BUFFER_STEREO16,
	    AUDIO_BUFFER_UNKNOWN
	};

	/**
		@enum XdevLAudioSamplingRate
		@brief Audio sampling rates.
		@author Cengiz Terzibas
	*/
	enum XdevLAudioSamplingRate {
	    AUDIO_SAMPLE_RATE_8000 	= 8000,
	    AUDIO_SAMPLE_RATE_11025 = 11025,
	    AUDIO_SAMPLE_RATE_22050 = 22050,
	    AUDIO_SAMPLE_RATE_44100 = 44100,
	    AUDIO_SAMPLE_RATE_48000 = 48000,
	    AUDIO_SAMPLE_RATE_96000 = 96000
	};

	/**
		@class XdevLAudioBuffer
		@brief Buffer for audio data.
		@author Cengiz Terzibas
	*/
	class XdevLAudioBuffer {
		public:
			virtual ~XdevLAudioBuffer() {};
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
			virtual const xdl_uint& getID() const = 0;

			/// Returns a pointer to the data.
			virtual xdl_uint8* getData() = 0;
	};

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

			/// Sets the position of the source.
			/**
				@param x X coordinate of the source in global space.
				@param y Y coordinate of the source in global space.
				@param z Z coordinate of the source in global space.
			*/
			virtual void setPos(xdl_float x, xdl_float y, xdl_float z) = 0;

			/// Sets the velocity of the source.
			/**
				@param vx Velocity component in x direction in global space.
				@param vy Velocity component in y direction in global space.
				@param vz Velocity component in z direction in global space.
			*/
			virtual void setVel(xdl_float vx, xdl_float vy, xdl_float vz) = 0;

			/// Sets the orientation of the source.
			/**
				@param x X coordinate of the direction in global space.
				@param y Y coordinate of the direction in global space.
				@param z Z coordinate of the direction in global space.
			*/
			virtual void setOri(xdl_float x, xdl_float y, xdl_float z) = 0;

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

	/**
		@class XdevLAudio
		@brief Class that manages audio context, buffers and sources.
		@author Cengiz Terzibas
	*/
	class XdevLAudio : public XdevLModule {
		public:
			virtual ~XdevLAudio() {};
			/// Create audio buffer from file.
			/**
				At the moment only .wav files are supported.

				@param filename Filename that points to a valid audio file.
				@param buffer A pointer to an XdevLAudioBuffer;
				@return
				- @b ERR_OK If it was successful. The (*buffer) pointer will have a valid pointer to an
				XdevLAudioBuffer object.
				- @b ERR_ERROR If it fails to create a buffer. The pointer (*buffer) will be set to NULL.
			*/
			virtual xdl_int createAudioBufferFromFile(const XdevLFileName& filename, XdevLAudioBuffer** buffer) = 0;

			/// Creates an empty audio buffer.
			/**
				@param format Specifies the audioBufferFormat of the audio buffer.
				@param frequency Specifies the base frequence of the audio buffer.
				@param size The size of the buffer. The size depends on the format.
				@param data A buffer that will be copied into the audio buffer.
				@param buffer A pointer to an XdevLAudioBuffer;
				@return
				- @b ERR_OK If it was successful. The (*buffer) pointer will have a valid pointer to an
				XdevLAudioBuffer object.
				- @b ERR_ERROR If it fails to create a buffer. The pointer (*buffer) will be set to NULL.
			*/
			virtual xdl_int createAudioBuffer(xdl_int format, xdl_int frequency, xdl_int size, void* data, XdevLAudioBuffer** buffer) = 0;

			/// Creates an audio source.
			/**
				@param src The valid pointer to an XdevLAudioSource pointer which will hold the source object.
				@param buffer A pointer to an valid XdevLAudioBuffer;
				@return
				- @b ERR_OK If it was successful. The (*src) pointer will have a valid pointer to an
				XdevLAudioSource object.
				- @b ERR_ERROR If it fails to create a source. The pointer (*src) will be set to NULL.
			*/
			virtual xdl_int createAudioSource(XdevLAudioSource** src, XdevLAudioBuffer* buffer) = 0;

			/// Sets the position of the listener.
			virtual void setPos(xdl_float x, xdl_float y, xdl_float z) = 0;

			/// Sets the velocity of the listener.
			virtual void setVel(xdl_float vx, xdl_float vy, xdl_float vz) = 0;

			/// Sets the orientation of the listener.
			virtual void setOri(xdl_float x, xdl_float y, xdl_float z) = 0;

			/// Sets the gain of the listener.
			virtual void setGain(xdl_float gain) = 0;

			/// Makes the audio context to the current.
			virtual xdl_int makeCurrent() = 0;

			/// Release current context.
			virtual xdl_int releaseCurrent() = 0;
	};

	/**
		@class XdevLAudioRecord
		@brief Class to support audio recording.
		@author Cengiz Terzibas
	*/
	class XdevLAudioRecord : public XdevLModule  {
		public:
			virtual ~XdevLAudioRecord() {};

			/// Create the recording device with the specified properties.
			/**
				@param samplerate The sample rate. You can use the enum audioSampleRate or
				write the value manualy.
				@param format The sample format. Check audioBufferFormat.
				@param samples. Size of the internal ringbuffer in samples. It's not the size in bytes.
				It depends on the sample format you choose.
			*/
			virtual xdl_int device(const XdevLString& deviceName, xdl_int sample_rate, xdl_int format, xdl_int samples) = 0;

			/// Starts the recording in the background.
			virtual void start() = 0;

			/// Stops the recording.
			virtual void stop() = 0;

			/// Copy audio data from the internal buffer into the buffer pointer.
			/**
				Please make sure that the pointer buffer points to a memory with enough capacity to copy.
				the number of samples.
				@param buffer A pointer to a buffer where the recorded data will be copied.
				@param size The number of samples you want to copy into the buffer.
			*/
			virtual xdl_int capture(xdl_int8* buffer, xdl_int samples) = 0;

			/// Returns the sample size in bytes.
			virtual xdl_int sampleSize() = 0;
	};

	/**
		@class XdevLAudioContextScope
		@brief Scope class for the audio interface

		Can be used to make the context current and release
		automatically.

	*/
	class XdevLAudioContextScope {
		public:
			XdevLAudioContextScope(XdevLAudio* audio) : m_audio(audio) {
				m_audio->makeCurrent();
			}
			virtual ~XdevLAudioContextScope() {
				m_audio->releaseCurrent();
			}
		private:
			XdevLAudio* m_audio;
	};

	typedef XdevLAudioBuffer	IXdevLAudioBuffer;
	typedef XdevLAudioBuffer*	IPXdevLAudioBuffer;

	typedef XdevLAudioSource	IXdevLAudioSource;
	typedef XdevLAudioSource*	IPXdevLAudioSource;

	typedef XdevLAudio			IXdevLAudio;
	typedef XdevLAudio*			IPXdevLAudio;

	typedef XdevLAudioRecord	IXdevLAudioRecord;
	typedef XdevLAudioRecord*	IPXdevLAudioRecord;



}

#endif
