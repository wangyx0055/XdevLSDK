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

#ifndef XDEVL_AUDIO_H
#define XDEVL_AUDIO_H

#include <XdevLModule.h>

#include <XdevLAudio/XdevLAudioBuffer.h>
#include <XdevLAudio/XdevLAudioSource.h>

namespace xdl {

	/**
		@enum XdevLAudioStreamType
		@brief Types of audio stream types.
		@author Cengiz Terzibas
	*/
	enum XdevLAudioStreamType {
	    AUDIO_STREAM_PLAYBACK,
	    AUDIO_STREAM_CAPTURE
	};

	/**
		@enum XdevLAudioBufferFormat
		@brief Types of audio formats.
		@author Cengiz Terzibas
	*/
	enum XdevLAudioBufferFormat {
	    AUDIO_BUFFER_FORMAT_S8,
	    AUDIO_BUFFER_FORMAT_U8,
	    AUDIO_BUFFER_FORMAT_S16,
	    AUDIO_BUFFER_FORMAT_U16,
	    AUDIO_BUFFER_FORMAT_S24,
	    AUDIO_BUFFER_FORMAT_U24,
	    AUDIO_BUFFER_FORMAT_S32,
	    AUDIO_BUFFER_FORMAT_U32,
	    AUDIO_BUFFER_FORMAT_FLOAT,
	    AUDIO_BUFFER_FORMAT_DOUBLE,
	    AUDIO_BUFFER_FORMAT_UNKNOWN
	};

	inline xdl_uint XdevLAudioBufferFormatBitSize(XdevLAudioBufferFormat format) {
		switch(format) {
			case AUDIO_BUFFER_FORMAT_S8:
			case AUDIO_BUFFER_FORMAT_U8:
				return 8;
			case AUDIO_BUFFER_FORMAT_S16:
			case AUDIO_BUFFER_FORMAT_U16:
				return 16;
			case AUDIO_BUFFER_FORMAT_S24:
			case AUDIO_BUFFER_FORMAT_U24:
				return 24;
			case AUDIO_BUFFER_FORMAT_S32:
			case AUDIO_BUFFER_FORMAT_U32:
			case AUDIO_BUFFER_FORMAT_FLOAT:
				return 32;
			case AUDIO_BUFFER_FORMAT_DOUBLE:
				return 64;
			default:
				break;
		}
		return AUDIO_BUFFER_FORMAT_UNKNOWN;
	}

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

	typedef xdl_uint(*callbackFunctionType)(xdl_uint8* buffer, xdl_uint samples, void* userData);

	/**
		@class XdevLAudio
		@brief Class that manages audio context, buffers and sources.
		@author Cengiz Terzibas
	*/
	class XdevLAudio : public XdevLModule {
		public:
			virtual ~XdevLAudio() {};

			virtual xdl_int setCallbackFunction(callbackFunctionType callbackFuntion, void* userData = nullptr) = 0;

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
			virtual xdl_int createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data, IPXdevLAudioBuffer* buffer) = 0;

			/// Creates an audio source.
			/**
				@param src The valid pointer to an XdevLAudioSource pointer which will hold the source object.
				@param buffer A pointer to an valid XdevLAudioBuffer;
				@return
				- @b ERR_OK If it was successful. The (*src) pointer will have a valid pointer to an
				XdevLAudioSource object.
				- @b ERR_ERROR If it fails to create a source. The pointer (*src) will be set to NULL.
			*/
			virtual xdl_int createAudioSource(IPXdevLAudioSource* src, XdevLAudioBuffer* buffer) = 0;

			/// Sets the gain of the listener.
			virtual void setGain(xdl_float gain) = 0;

			/// Makes the audio context to the current.
			virtual xdl_int makeCurrent() = 0;

			/// Release current context.
			virtual xdl_int releaseCurrent() = 0;

			virtual xdl_uint getSamplingRate() = 0;
			virtual xdl_uint getBufferSize() = 0;
			virtual xdl_uint getNumberOfChannels() = 0;
			virtual xdl_uint getFormatSizeInBytes() = 0;
			virtual xdl_uint getPeriodTime() = 0;
			virtual xdl_uint getPeriodSize() = 0;
			virtual xdl_int update2() = 0;
	};

	class XdevLAudioPlayback : public XdevLAudio {

		public:
			virtual ~XdevLAudioPlayback() {};
			virtual xdl_int write(xdl_uint8* buffer) = 0;
	};

	class XdevLAudioCapture : public XdevLAudio {

		public:
			virtual ~XdevLAudioCapture() {};
			virtual xdl_int read(xdl_uint8* buffer) = 0;
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

	typedef XdevLAudioPlayback	IXdevLAudioPlayback;
	typedef XdevLAudioPlayback*	IPXdevLAudioPlayback;

	typedef XdevLAudioCapture	IXdevLAudioCapture;
	typedef XdevLAudioCapture*	IPXdevLAudioCapture;



}

#endif
