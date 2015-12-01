#ifndef XDEVL_COREAUDIO_IMPL_H
#define XDEVL_COREAUDIO_IMPL_H

#include <XdevLAudio/XdevLAudio.h>

#include <XdevLPluginImpl.h>


#if !defined(__IPHONEOS__)
#define MACOSX_COREAUDIO 1
#endif

#if MACOSX_COREAUDIO
#include <CoreAudio/CoreAudio.h>
#include <CoreServices/CoreServices.h>
#else
#include <AudioToolbox/AudioToolbox.h>
#endif

#include <AudioUnit/AudioUnit.h>

namespace xdl {

	static const XdevLString vendor {
		"Vendors Name"
	};

	static const XdevLString author {
		"Authors Name"
	};

	static const XdevLString copyright {
		"Copyright here."
	};

	static const XdevLString pluginName {
		"XdevLAudioCoreAudio"
	};

	static const XdevLString moduleDescription {
		"This is an empty module"
	};

	static std::vector<XdevLModuleName>	moduleNames {
		XdevLModuleName("XdevLAudioPlayback"),
		XdevLModuleName("XdevLAudioCapture")
	};

	class XdevLAudioBufferCoreAudioBuffer : public XdevLAudioBuffer {
			virtual ~XdevLAudioBufferCoreAudioBuffer() {}

			virtual xdl_int getFormat() override;
			virtual xdl_int getSize() ;
			virtual xdl_int getSamplingRate();
			virtual xdl_int getChannels();
			virtual xdl_int getBits();
			virtual xdl_uint getID();
			virtual xdl_uint8* getData();
			virtual xdl_int lock();
			virtual xdl_int unlock();
			virtual xdl_int upload(xdl_int8* src, xdl_uint size);
		public:
			xdl_uint				m_bufferFormatSize;
			xdl_uint 				m_samplingRate;
			xdl_uint				m_channels;
			xdl_uint				m_bufferSizeInBytes;
			xdl_uint8*				m_buffer;
	};

	class XdevLAudioCoreAudio : public XdevLModuleImpl<XdevLAudio> {

		public:

			XdevLAudioCoreAudio(XdevLModuleCreateParameter* parameter);

			virtual ~XdevLAudioCoreAudio();
			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;


			virtual xdl_int create(XdevLAudioStreamType streamType, XdevLAudioBufferFormat bufferFormat, XdevLAudioSamplingRate samplingRate, xdl_uint channels, XdevLAudioBuffer** buffer);
			virtual xdl_int write(xdl_uint8* buffer);
			virtual xdl_int read(xdl_uint8* buffer);
			virtual xdl_uint getBufferSize() override;
			virtual xdl_uint getNumberOfChannels() override ;
			virtual xdl_uint getFormatSizeInBytes() override;
			virtual xdl_uint getPeriodTime() override;
			virtual xdl_uint getSamplingRate() override;
			virtual xdl_uint getPeriodSize() override;
			virtual xdl_int update2() override;


			virtual xdl_int setCallbackFunction(callbackFunctionType callbackFuntion) override;
			virtual xdl_int createAudioBufferFromFile(const XdevLFileName& filename, XdevLAudioBuffer** buffer);
			virtual xdl_int createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data, XdevLAudioBuffer** buffer);
			virtual xdl_int createAudioSource(XdevLAudioSource** src, XdevLAudioBuffer* buffer) override;
			virtual void setGain(xdl_float gain) override;
			virtual xdl_int makeCurrent() override;
			virtual xdl_int releaseCurrent() override;




		private:
			AudioUnit audioUnit;
			xdl_bool m_audioUnitOpen;
			void *buffer;
			UInt32 bufferOffset;
			UInt32 bufferSize;
#if MACOSX_COREAUDIO
			AudioDeviceID deviceID;
#endif
			xdl_uint m_bufferSize;
			xdl_uint m_samplingRate;
			xdl_uint m_channels;
			xdl_uint m_bufferFormatSize;
			xdl_uint m_periodTime;
			xdl_float m_gain;

			callbackFunctionType m_callbackFunction;

	};


}

#endif
