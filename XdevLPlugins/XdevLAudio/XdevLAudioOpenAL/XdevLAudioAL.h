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
#include <XdevLAudio/XdevLAudio.h>

#include "XdevLAudioBufferAL.h"
#include "XdevLAudioSourceAL.h"
#include <vector>

namespace xdl {

	class XdevLAudio;
	class deviceSource;

	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005 - 2015 Cengiz Terzibas."
	};
	static const XdevLString pluginName {
		"XdevLAudio"
	};
	static const XdevLString description {
		"OpenAL device module. Creates an OpenAL context."
	};
	static const XdevLString description2 {
		"Supports audio recording with OpenAL."
	};

	static std::vector<XdevLModuleName>	moduleNames {
		XdevLModuleName("XdevLAudioPlayback"),
		XdevLModuleName("XdevLAudioCapture")
	};

	/**
		@class XdevLAudioPlaybackAL
		@brief Class to support mouse devices
		@author Cengiz Terzibas
	*/
	class XdevLAudioPlaybackAL : public XdevLModuleImpl<XdevLAudio> {
		public:
			XdevLAudioPlaybackAL(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLAudioPlaybackAL();

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;

			virtual xdl_int setCallbackFunction(callbackFunctionType callbackFuntion, void* userData) override;
			virtual xdl_int createAudioBufferFromFile(const XdevLFileName& filename, IPXdevLAudioBuffer* buffer) override;
			virtual xdl_int createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data, IPXdevLAudioBuffer* buffer) override;
			virtual xdl_int createAudioSource(IPXdevLAudioSource* src, IPXdevLAudioBuffer buffer) override;
			virtual void setGain(xdl_float gain) override;
			virtual xdl_int makeCurrent() override;
			virtual xdl_int releaseCurrent() override;

			virtual xdl_uint getSamplingRate(){return 0;}
			virtual xdl_uint getBufferSize(){return 0;}
			virtual xdl_uint getNumberOfChannels(){return 0;}
			virtual xdl_uint getFormatSizeInBytes(){return 0;}
			virtual xdl_uint getPeriodTime(){return 0;}
			virtual xdl_uint getPeriodSize(){return 0;}
			virtual xdl_int update2(){return 0;}
		protected:
			ALCdevice* m_device;
			ALCcontext* m_context;
			ALCcontext* m_previous;
			callbackFunctionType m_callbackFunction;
			void* m_userData;
			std::vector<XdevLAudioSourceAL*> m_audioSources;
			std::vector<XdevLAudioBufferAL*> m_audioBuffers;
	};


//	/**
//		@class XdevLAudioALRecord
//		@class Extended version of the XdevLAudioAL version which supports recording.
//		@author Cengiz Terzibas
//	*/
//	class XdevLAudioALRecord : public XdevLModuleImpl<XdevLAudioCapture> {
//		public:
//			XdevLAudioALRecord(XdevLModuleCreateParameter* parameter);
//			virtual ~XdevLAudioALRecord() {};
//
//			virtual xdl_int init() override;
//			virtual xdl_int shutdown() override;
//
//			virtual xdl_int setCallbackFunction(callbackFunctionType callbackFuntion);
//
//
//
//
//
//
//			virtual void start();
//			virtual void stop();
//			virtual xdl_int device(const XdevLString& deviceName, XdevLAudioBuffer* audioBuffer, xdl_int samples);
//			virtual xdl_int device(const XdevLString& deviceName, XdevLAudioBufferFormat format,  XdevLAudioSamplingRate samplerate, xdl_uint channels, xdl_int buffer_size);
//			virtual xdl_int capture(XdevLAudioBuffer* audioBuffer);
//			virtual xdl_int capture(xdl_int8* buffer, xdl_int samples);
//			virtual xdl_int sampleSize();
//
//			virtual xdl_int read(xdl_int8* buffer) override;
//
//		protected:
//			ALCdevice* 	m_recordDevice;
//			ALenum 		m_audioFormat;
//			ALCuint 	m_sampleRate;
//			ALCsizei	m_bufferSize;
//			ALCsizei	m_sampleSize;
//			xdl_bool	m_recordingStarted;
//	};

	/// Returns AL error codes as string.
	std::string getALErrorString(ALenum err);
	/// Returns ALC error codes as string.
	std::string getALCErrorString(ALenum err);
	ALenum wrapAudioBufferFormat(XdevLAudioBufferFormat format, xdl_uint numberOfChannels);

}

#endif
