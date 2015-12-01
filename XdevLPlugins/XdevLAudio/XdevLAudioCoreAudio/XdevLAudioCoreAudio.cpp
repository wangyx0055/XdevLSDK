#include "XdevLAudioCoreAudio.h"

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLCOREAUDIO_PLUGIN_MAJOR_VERSION,
	XDEVLCOREAUDIO_PLUGIN_MINOR_VERSION,
	XDEVLCOREAUDIO_PLUGIN_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::moduleDescription,
	XDEVLCOREAUDIO_MODULE_MAJOR_VERSION,
	XDEVLCOREAUDIO_MODULE_MINOR_VERSION,
	XDEVLCOREAUDIO_MODULE_PATCH_VERSION
};

extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(moduleDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLAudioCoreAudio(parameter);
		if(!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	}

	return xdl::ERR_MODULE_NOT_FOUND;
}

extern "C" XDEVL_EXPORT void _delete(xdl::XdevLModule* obj) {
	if(obj)
		delete obj;
}

extern "C" XDEVL_EXPORT xdl::XdevLPluginDescriptor* _getDescriptor() {
	return &pluginDescriptor;
}

namespace xdl {


	xdl_int XdevLAudioBufferCoreAudioBuffer::getFormat() {
		return AUDIO_BUFFER_FORMAT_UNKNOWN;
	}

	xdl_int XdevLAudioBufferCoreAudioBuffer::getSize() {
		return m_bufferSizeInBytes;
	}

	xdl_int XdevLAudioBufferCoreAudioBuffer::getSamplingRate() {
		return m_samplingRate;
	}

	xdl_int XdevLAudioBufferCoreAudioBuffer::getChannels() {
		return m_channels;
	}

	xdl_int XdevLAudioBufferCoreAudioBuffer::getBits() {
		return m_bufferFormatSize * 8;
	}

	xdl_uint XdevLAudioBufferCoreAudioBuffer::getID() {
		return 0;
	}

	xdl_uint8* XdevLAudioBufferCoreAudioBuffer::getData() {
		return nullptr;
	}

	xdl_int XdevLAudioBufferCoreAudioBuffer::lock() {
		return ERR_OK;
	}

	xdl_int XdevLAudioBufferCoreAudioBuffer::unlock() {
		return ERR_OK;
	}

	xdl_int XdevLAudioBufferCoreAudioBuffer::upload(xdl_int8* src, xdl_uint size) {
		return ERR_OK;
	}



//
//
//




	XdevLAudioCoreAudio::XdevLAudioCoreAudio(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLAudio> (parameter, moduleDescriptor) {
	}

	XdevLAudioCoreAudio::~XdevLAudioCoreAudio() {}

	xdl_int XdevLAudioCoreAudio::init() {

		return ERR_OK;
	}

	xdl_int XdevLAudioCoreAudio::shutdown() {

		return ERR_OK;
	}

	xdl_int XdevLAudioCoreAudio::createAudioBufferFromFile(const XdevLFileName& filename, XdevLAudioBuffer** buffer) {
		return ERR_OK;
	}

	xdl_int XdevLAudioCoreAudio::create(XdevLAudioStreamType streamType, XdevLAudioBufferFormat bufferFormat, XdevLAudioSamplingRate samplingRate, xdl_uint channels, XdevLAudioBuffer** buffer) {

		void* handle = nullptr;



		switch(bufferFormat) {
			case AUDIO_BUFFER_FORMAT_S8:
				break;
			case AUDIO_BUFFER_FORMAT_U8:
				break;
			case AUDIO_BUFFER_FORMAT_S16:
				break;
			case AUDIO_BUFFER_FORMAT_U16:
				break;
			case AUDIO_BUFFER_FORMAT_S32:
				break;
			case AUDIO_BUFFER_FORMAT_U32:
				break;
			case AUDIO_BUFFER_FORMAT_FLOAT:
				break;
			case AUDIO_BUFFER_FORMAT_DOUBLE:
				break;
			default:
				break;
		}

		AudioStreamBasicDescription description;
		description.mFormatID = kAudioFormatLinearPCM;
		description.mFormatFlags = kLinearPCMFormatFlagIsPacked;
		description.mChannelsPerFrame = channels;
		description.mSampleRate = samplingRate;
		description.mFramesPerPacket = 1;
		description.mBitsPerChannel = XdevLAudioBufferFormatBitSize(bufferFormat);
		description.mBytesPerFrame = description.mChannelsPerFrame * description.mBitsPerChannel/8;
		description.mBytesPerPacket = description.mFramesPerPacket * description.mBytesPerFrame;

#if MACOSX_COREAUDIO
		ComponentDescription desc;
		Component comp = nullptr;
#else
		AudioComponentDescription desc;
		AudioComponent comp = nullptr;
#endif

		OSStatus result = noErr;
		AURenderCallbackStruct callback;


		xdl_bool captureMode = xdl_false;
		switch(streamType) {
			case AUDIO_STREAM_PLAYBACK:
				captureMode = xdl_false;
				break;
			case AUDIO_STREAM_CAPTURE:
				captureMode = xdl_true;
				break;
			default:
				break;
		}

		const AudioUnitElement output_bus = 0;
		const AudioUnitElement input_bus = 1;

		const AudioUnitElement bus = ((captureMode) ? input_bus : output_bus);
		const AudioUnitScope scope = ((captureMode) ? kAudioUnitScope_Output : kAudioUnitScope_Input);


#if MACOSX_COREAUDIO
		AudioDeviceID devid;
		UInt32 size = 0;
		UInt32 alive = 0;
		pid_t pid = 0;

		AudioObjectPropertyAddress addr = {
			0,
			kAudioObjectPropertyScopeGlobal,
			kAudioObjectPropertyElementMaster
		};


		size = sizeof(AudioDeviceID);
		addr.mSelector = ((captureMode) ? kAudioHardwarePropertyDefaultInputDevice : kAudioHardwarePropertyDefaultOutputDevice);
		result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &addr, 0, NULL, &size, &devid);

		addr.mSelector = kAudioDevicePropertyDeviceIsAlive;
		addr.mScope = captureMode ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;

		size = sizeof(alive);
		result = AudioObjectGetPropertyData(devid, &addr, 0, nullptr, &size, &alive);

		if(!alive) {
			return ERR_ERROR;
		}

		addr.mSelector = kAudioDevicePropertyHogMode;
		size = sizeof(pid);
		result = AudioObjectGetPropertyData(devid, &addr, 0, nullptr, &size, &pid);

		/* some devices don't support this property, so errors are fine here. */
		if((result == noErr) && (pid != -1)) {

			return ERR_ERROR;
		}

		deviceID = devid;
#endif




		desc.componentType = kAudioUnitType_Output;
		desc.componentManufacturer = kAudioUnitManufacturer_Apple;
#if MACOSX_COREAUDIO
		desc.componentSubType = kAudioUnitSubType_DefaultOutput;
		comp = FindNextComponent(NULL, &desc); // TODO
#else
		desc.componentSubType = kAudioUnitSubType_RemoteIO;
		comp = AudioComponentFindNext(NULL, &desc);
#endif

		if(comp == nullptr) {
			XDEVL_MODULE_ERROR("Couldn't find the CoreAudio.\n");
			return ERR_ERROR;
		}


#if MACOSX_COREAUDIO
		result = OpenAComponent(comp, &audioUnit);
#else
		/*
		   AudioComponentInstanceNew only available on iPhone OS 2.0 and Mac OS X 10.6
		   We can't use OpenAComponent on iPhone because it is not present
		 */
		result = AudioComponentInstanceNew(comp, &audioUnit);
#endif

		m_audioUnitOpen = xdl_true;


#if MACOSX_COREAUDIO
		result = AudioUnitSetProperty(audioUnit, kAudioOutputUnitProperty_CurrentDevice,
		                              kAudioUnitScope_Global, 0,
		                              &deviceID,
		                              sizeof(AudioDeviceID));
#endif
		return ERR_OK;
	}

	xdl_int XdevLAudioCoreAudio::createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data, XdevLAudioBuffer** buffer) {

		return create(AUDIO_STREAM_PLAYBACK, format, samplingRate, channels, buffer);
	}

	xdl_int XdevLAudioCoreAudio::createAudioSource(XdevLAudioSource** src, XdevLAudioBuffer* buffer) {
		return ERR_OK;
	}

	xdl_int XdevLAudioCoreAudio::write(xdl_uint8* buffer) {
		return ERR_OK;
	}

	xdl_int XdevLAudioCoreAudio::read(xdl_uint8* buffer) {
		return ERR_OK;
	}


	xdl_int XdevLAudioCoreAudio::update2() {
		return ERR_OK;
	}

	xdl_uint XdevLAudioCoreAudio::getSamplingRate() {
		return m_samplingRate;
	}

	xdl_uint XdevLAudioCoreAudio::getPeriodSize() {
		return 0;
	}

	xdl_uint XdevLAudioCoreAudio::getBufferSize() {
		return m_bufferSize;
	}

	xdl_uint XdevLAudioCoreAudio::getNumberOfChannels() {
		return m_channels;
	}
	xdl_uint XdevLAudioCoreAudio::getFormatSizeInBytes() {
		return m_bufferFormatSize;
	}
	xdl_uint XdevLAudioCoreAudio::getPeriodTime() {
		return m_periodTime;
	}
	xdl_int XdevLAudioCoreAudio::setCallbackFunction(callbackFunctionType callbackFuntion) {
		m_callbackFunction = callbackFuntion;
		return ERR_OK;
	}

	void XdevLAudioCoreAudio::setGain(xdl_float gain) {
		m_gain = gain;
	}
	xdl_int XdevLAudioCoreAudio::makeCurrent() {
		return ERR_OK;
	}
	xdl_int XdevLAudioCoreAudio::releaseCurrent() {
		return ERR_OK;
	}

	//
	//
	//

//	XdevLAudioAlsaCapture(XdevLModuleCreateParameter* parameter) {
//
//	}
//
//
//	XdevLAudioAlsaCapture::~XdevLAudioAlsaCapture() {
//
//	}
//
//	xdl_int XdevLAudioAlsaCapture::read(xdl_int8* buffer, xdl_int samples) {
//		return 0;
//	}
}
