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

#ifndef XDEVL_AUDIOALSA_IMPL_H
#define XDEVL_AUDIOALSA_IMPL_H


#include <XdevLAudio/XdevLAudio.h>
#include <XdevLPluginImpl.h>

#include <alsa/asoundlib.h>

namespace xdl {

	static const XdevLString vendor {
		"www.codeposer.net"
	};

	static const XdevLString author {
		"Cengiz Terzibas"
	};

	static const XdevLString copyright {
		"(c) 2005 - 2016 Cengiz Terzibas."
	};

	static const XdevLString pluginName {
		"XdevLAudioAlsa"
	};

	static const XdevLString moduleDescription {
		"This is an empty module"
	};

	static std::vector<XdevLModuleName>	moduleNames {
		XdevLModuleName("XdevLAudioPlayback"),
		XdevLModuleName("XdevLAudioCapture")
	};

	class XdevLAudioBufferAlsa : public XdevLAudioBuffer {
		public:
			XdevLAudioBufferAlsa(_snd_pcm_format format, XdevLAudioSamplingRate samplingRate, xdl_uint channels);
			virtual ~XdevLAudioBufferAlsa() {}

			virtual xdl_int getFormat();
			virtual xdl_int getSize() ;
			virtual xdl_int getSamplingRate();
			virtual xdl_int getChannels();
			virtual xdl_int getBits();
			virtual xdl_uint getID();
			virtual xdl_uint8* getData();
			virtual xdl_int lock();
			virtual xdl_int unlock();
			virtual xdl_int upload(xdl_int8* src, xdl_uint size);

			_snd_pcm_format			m_bufferFormat;
			xdl_uint				m_bufferFormatSize;
			xdl_uint 				m_samplingRate;
			xdl_uint				m_channels;
			xdl_uint				m_bufferSizeInBytes;
			xdl_uint8*				m_buffer;
	};

	class XdevLAudioAlsaBase {

		public:

			XdevLAudioAlsaBase(const XdevLModuleDescriptor& descriptor);

			virtual ~XdevLAudioAlsaBase();
			xdl_int init();
			xdl_int shutdown();
			IPXdevLAudioBuffer create(XdevLAudioStreamType streamType, XdevLAudioBufferFormat bufferFormat, XdevLAudioSamplingRate samplingRate, xdl_uint channels);
			xdl_int write(xdl_uint8* buffer);
			xdl_int read(xdl_uint8* buffer);

			xdl_uint getSamplingRate();
			xdl_uint getBufferSize();
			xdl_uint getNumberOfChannels();
			xdl_uint getFormatSizeInBytes();
			xdl_uint getPeriodSize();
			xdl_uint getPeriodTime();



			void setCallbackFunction(callbackFunctionType callbackFuntion, void* userData);
			IPXdevLAudioBuffer createAudioBufferFromFile(const XdevLFileName& filename);
			IPXdevLAudioBuffer createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data);
			void setGain(xdl_float gain);
			xdl_int makeCurrent();
			xdl_int releaseCurrent();


			xdl_int update2();
		private:
			XdevLString getDeviceName();
			xdl_int openDevice(snd_pcm_stream_t streamType);
			xdl_int setHardwareParameters(xdl_uint numberOfChannels, _snd_pcm_format bufferformat, xdl_uint samplingRate);
			xdl_int applyHardwareParameters();
			xdl_int setPeriodSize(snd_pcm_uframes_t frames);

			void debugDump();
			
			const XdevLModuleDescriptor& getDescriptor() const {
				return m_descriptor;
			}
		private:
			const XdevLModuleDescriptor m_descriptor;

			// Handle for the PCM device.
			snd_pcm_t*				m_handle;

			// This structure contains information about the hardware and can be used to specify the
			// configuration to be used for the PCM stream.
			snd_pcm_hw_params_t*	m_hwParams;

			// Name of the PCM device, like plughw:0,0 .The first number is the number of the soundcard the
			// second number is the number of the device.
			XdevLString				m_name;

			// Holds the internal sampling rate.
			xdl_uint 				m_samplingRate;
			_snd_pcm_format			m_bufferFormat;
			xdl_uint				m_bufferFormatSize;
			xdl_uint				m_channels;
			xdl_uint				m_samples;
			xdl_uint				m_bufferSize;
			snd_pcm_uframes_t 		m_periodSize;
			xdl_uint				m_periodTime;
			snd_pcm_stream_t 		m_streamType;

			XdevLAudioStreamType	m_supportedStreamType;
			callbackFunctionType 	m_callbackFunction;
			xdl_float				m_gain;
			void*					m_userData;
			xdl_uint8* 				m_buffer;
	};



	class XdevLAudioPlaybackImpl : public XdevLAudioAlsaBase, public XdevLModuleImpl<XdevLAudioPlayback> {

		public:

			XdevLAudioPlaybackImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);

			virtual ~XdevLAudioPlaybackImpl();
			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
//			virtual xdl_int create(XdevLAudioBufferFormat bufferFormat, XdevLAudioSamplingRate samplingRate, xdl_uint channels, XdevLAudioBuffer** buffer) override;
			virtual xdl_int write(xdl_uint8* buffer) override;

			virtual xdl_uint getSamplingRate();
			virtual xdl_uint getBufferSize() override;
			virtual xdl_uint getNumberOfChannels() override;
			virtual xdl_uint getFormatSizeInBytes() override;
			virtual xdl_uint getPeriodSize() override;
			virtual xdl_uint getPeriodTime() override;



			virtual void setCallbackFunction(callbackFunctionType callbackFuntion, void* userData) override;
			virtual IPXdevLAudioBuffer createAudioBufferFromFile(const XdevLFileName& filename) override;
			virtual IPXdevLAudioBuffer createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data) override;
			virtual IPXdevLAudioSource createAudioSource(IPXdevLAudioBuffer buffer) override;
			virtual void setGain(xdl_float gain) override;
			virtual xdl_int makeCurrent() override;
			virtual xdl_int releaseCurrent() override;


			virtual xdl_int update2();
	};


		class XdevLAudioCaptureImpl : public XdevLAudioAlsaBase, public XdevLModuleImpl<XdevLAudioCapture> {

		public:

			XdevLAudioCaptureImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor);

			virtual ~XdevLAudioCaptureImpl();
			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;
	//		virtual xdl_int create(XdevLAudioBufferFormat bufferFormat, XdevLAudioSamplingRate samplingRate, xdl_uint channels, XdevLAudioBuffer** buffer) override;
			virtual xdl_int read(xdl_uint8* buffer) override;

			virtual xdl_uint getSamplingRate();
			virtual xdl_uint getBufferSize() override;
			virtual xdl_uint getNumberOfChannels() override;
			virtual xdl_uint getFormatSizeInBytes() override;
			virtual xdl_uint getPeriodSize() override;
			virtual xdl_uint getPeriodTime() override;



			virtual void setCallbackFunction(callbackFunctionType callbackFuntion, void* userData) override;
			virtual IPXdevLAudioBuffer createAudioBufferFromFile(const XdevLFileName& filename) override;
			virtual IPXdevLAudioBuffer createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data) override;
			virtual IPXdevLAudioSource createAudioSource(IPXdevLAudioBuffer buffer) override;
			virtual void setGain(xdl_float gain) override;
			virtual xdl_int makeCurrent() override;
			virtual xdl_int releaseCurrent() override;


			virtual xdl_int update2();
	};
}

#endif
