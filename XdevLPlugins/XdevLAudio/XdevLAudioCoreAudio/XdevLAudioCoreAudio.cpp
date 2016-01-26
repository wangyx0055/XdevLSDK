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

extern "C" XDEVL_EXPORT xdl::xdl_int _init_plugin(xdl::XdevLPluginCreateParameter* parameter) {
	return xdl::ERR_OK;
}

extern "C" XDEVL_EXPORT xdl::xdl_int _shutdown_plugin() {
	return xdl::ERR_OK;
}

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


	static OSStatus inputCallback(void *inRefCon,
	                              AudioUnitRenderActionFlags * ioActionFlags,
	                              const AudioTimeStamp * inTimeStamp,
	                              UInt32 inBusNumber, UInt32 inNumberFrames,
	                              AudioBufferList * ioData) {
		/* err = AudioUnitRender(afr->fAudioUnit, ioActionFlags, inTimeStamp, inBusNumber, inNumberFrames, afr->fAudioBuffer); */
		/* !!! FIXME: write me! */
		std::cout << "inputCallback\n" << std::endl;
		return noErr;
	}

	static OSStatus outputCallback(void *inRefCon,
	                               AudioUnitRenderActionFlags * ioActionFlags,
	                               const AudioTimeStamp * inTimeStamp,
	                               UInt32 inBusNumber, UInt32 inNumberFrames,
	                               AudioBufferList * ioData) {
		std::cout << "outputCallback\n" << std::endl;
//    SDL_AudioDevice *this = (SDL_AudioDevice *) inRefCon;
//    AudioBuffer *abuf;
//    UInt32 remaining, len;
//    void *ptr;
//    UInt32 i;
//
//    /* Only do anything if audio is enabled and not paused */
//    if (!this->enabled || this->paused) {
//        for (i = 0; i < ioData->mNumberBuffers; i++) {
//            abuf = &ioData->mBuffers[i];
//            SDL_memset(abuf->mData, this->spec.silence, abuf->mDataByteSize);
//        }
//        return 0;
//    }
//
//    /* No SDL conversion should be needed here, ever, since we accept
//       any input format in OpenAudio, and leave the conversion to CoreAudio.
//     */
//    /*
//       SDL_assert(!this->convert.needed);
//       SDL_assert(this->spec.channels == ioData->mNumberChannels);
//     */
//
//    for (i = 0; i < ioData->mNumberBuffers; i++) {
//        abuf = &ioData->mBuffers[i];
//        remaining = abuf->mDataByteSize;
//        ptr = abuf->mData;
//        while (remaining > 0) {
//            if (this->hidden->bufferOffset >= this->hidden->bufferSize) {
//                /* Generate the data */
//                SDL_LockMutex(this->mixer_lock);
//                (*this->spec.callback)(this->spec.userdata,
//                            this->hidden->buffer, this->hidden->bufferSize);
//                SDL_UnlockMutex(this->mixer_lock);
//                this->hidden->bufferOffset = 0;
//            }
//
//            len = this->hidden->bufferSize - this->hidden->bufferOffset;
//            if (len > remaining)
//                len = remaining;
//            SDL_memcpy(ptr, (char *)this->hidden->buffer +
//                       this->hidden->bufferOffset, len);
//            ptr = (char *)ptr + len;
//            remaining -= len;
//            this->hidden->bufferOffset += len;
//        }
//    }

		return 0;
	}


#if MACOSX_COREAUDIO
	const AudioObjectPropertyAddress aliveAddress = { kAudioDevicePropertyDeviceIsAlive, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMaster};

	static OSStatus deviceUnplugged(AudioObjectID devid, UInt32 num_addr, const AudioObjectPropertyAddress *addrs, void *data) {
//    SDL_AudioDevice *this = (SDL_AudioDevice *) data;
//    SDL_bool dead = SDL_FALSE;
//    UInt32 isAlive = 1;
//    UInt32 size = sizeof (isAlive);
//    OSStatus error;
//
//    if (!this->enabled) {
//        return 0;  /* already known to be dead. */
//    }
//
//    error = AudioObjectGetPropertyData(this->hidden->deviceID, &alive_address,
//                                       0, NULL, &size, &isAlive);
//
//    if (error == kAudioHardwareBadDeviceError) {
//        dead = SDL_TRUE;  /* device was unplugged. */
//    } else if ((error == kAudioHardwareNoError) && (!isAlive)) {
//        dead = SDL_TRUE;  /* device died in some other way. */
//    }
//
//    if (dead) {
//        SDL_OpenedAudioDeviceDisconnected(this);
//    }

		return 0;
	}
#endif

	XdevLAudioCoreAudio::XdevLAudioCoreAudio(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLAudio> (parameter, moduleDescriptor),
		m_buffer(nullptr) {
	}

	XdevLAudioCoreAudio::~XdevLAudioCoreAudio() {}

	xdl_int XdevLAudioCoreAudio::init() {

		return ERR_OK;
	}

	xdl_int XdevLAudioCoreAudio::shutdown() {
		delete [] m_deviceList;

#if MACOSX_COREAUDIO
		AudioObjectRemovePropertyListener(deviceID, &aliveAddress, deviceUnplugged, this);
#endif

		AudioOutputUnitStop(audioUnit);


		AURenderCallbackStruct callback;
		const AudioUnitElement output_bus = 0;
		const AudioUnitElement input_bus = 1;
		const AudioUnitElement bus = ((m_streamType == AUDIO_STREAM_CAPTURE) ? input_bus : output_bus);
		const AudioUnitScope scope = ((m_streamType == AUDIO_STREAM_CAPTURE) ? kAudioUnitScope_Output : kAudioUnitScope_Input);

		memset(&callback, 0, sizeof(AURenderCallbackStruct));
		AudioUnitSetProperty(audioUnit, kAudioUnitProperty_SetRenderCallback, scope, bus, &callback, sizeof(callback));

#if MACOSX_COREAUDIO
		CloseComponent(audioUnit);
#else
		AudioComponentInstanceDispose(audioUnit);
#endif

		m_audioUnitOpen = xdl_false;

		return ERR_OK;
	}

	IPXdevLAudioBuffer XdevLAudioCoreAudio::createAudioBufferFromFile(const XdevLFileName& filename) {
		return nullptr;
	}

	IPXdevLAudioBuffer XdevLAudioCoreAudio::create(XdevLAudioStreamType streamType, XdevLAudioBufferFormat bufferFormat, XdevLAudioSamplingRate samplingRate, xdl_uint channels) {

		void* handle = nullptr;
		m_streamType = streamType;



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


#if MACOSX_COREAUDIO
		AudioDeviceID devid;
		UInt32 size = 0;
		UInt32 alive = 0;
		pid_t pid = 0;

		AudioObjectPropertyAddress property = { kAudioHardwarePropertyDevices, kAudioObjectPropertyScopeGlobal, kAudioObjectPropertyElementMaster};

		//
		// Get the number of devices.
		//
		UInt32 properySize = 0;
		result = AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &property, 0, nullptr, &properySize);
		if(kAudioHardwareNoError != result) {
			XDEVL_MODULE_ERROR("AudioObjectGetPropertyDataSize failed\n");
		}
		size = properySize / sizeof(AudioDeviceID);
		size = sizeof(AudioDeviceID);

		//
		// Get all devices data
		//
//		m_deviceList = new AudioDeviceID[size];
//		result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &property, 0, nullptr, &size, m_deviceList);
//		if(kAudioHardwareNoError != result) {
//			XDEVL_MODULE_ERROR("AudioObjectGetPropertyData failed\n");
//		}
//
		//
		// Get only device data for the use specified one. Either for Playback or Capture.
		//
		property.mSelector = ((captureMode) ? kAudioHardwarePropertyDefaultInputDevice : kAudioHardwarePropertyDefaultOutputDevice);
		result = AudioObjectGetPropertyData(kAudioObjectSystemObject, &property, 0, nullptr, &size, &devid);
		if(kAudioHardwareNoError != result) {
			XDEVL_MODULE_ERROR("AudioObjectGetPropertyData failed\n");
			return nullptr;
		}

		property.mSelector = kAudioDevicePropertyDeviceIsAlive;
		property.mScope = captureMode ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;
		property.mElement = kAudioObjectPropertyElementMaster;

		size = sizeof(alive);
		result = AudioObjectGetPropertyData(devid, &property, 0, nullptr, &size, &alive);
		if(kAudioHardwareNoError != result) {
			XDEVL_MODULE_ERROR("AudioObjectGetPropertyData failed\n");
			return nullptr;
		}

		if(!alive) {
			return nullptr;
		}

		property.mSelector = kAudioDevicePropertyHogMode;
		size = sizeof(pid);
		result = AudioObjectGetPropertyData(devid, &property, 0, nullptr, &size, &pid);

		/* some devices don't support this property, so errors are fine here. */
		if((result == noErr) && (pid != -1)) {

			return nullptr;
		}

		deviceID = devid;
#endif




#if MACOSX_COREAUDIO
		memset(&desc, 0, sizeof(ComponentDescription));
#else
		memset(&desc, 0, sizeof(AudioComponentDescription));
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
			return nullptr;
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
		result = AudioUnitSetProperty(audioUnit, kAudioOutputUnitProperty_CurrentDevice, kAudioUnitScope_Global, 0, &deviceID, sizeof(AudioDeviceID));
		if(kAudioHardwareNoError != result) {
			XDEVL_MODULE_ERROR("AudioUnitSetProperty failed\n");
			return nullptr;
		}
#endif



		const AudioUnitElement output_bus = 0;
		const AudioUnitElement input_bus = 1;
		const AudioUnitElement bus = ((captureMode) ? input_bus : output_bus);
		const AudioUnitScope scope = ((captureMode) ? kAudioUnitScope_Output : kAudioUnitScope_Input);


		// Set the data format of the audio unit.
		result = AudioUnitSetProperty(audioUnit, kAudioUnitProperty_StreamFormat, scope, bus, &description, sizeof(description));
		if(kAudioHardwareNoError != result) {
			XDEVL_MODULE_ERROR("AudioUnitSetProperty failed\n");
			return nullptr;
		}

		/* Set the audio callback */
		memset(&callback, 0, sizeof(AURenderCallbackStruct));

		callback.inputProc = ((captureMode) ? inputCallback : outputCallback);
		callback.inputProcRefCon = this;

		result = AudioUnitSetProperty(audioUnit, kAudioUnitProperty_SetRenderCallback, scope, bus, &callback, sizeof(callback));
		if(kAudioHardwareNoError != result) {
			XDEVL_MODULE_ERROR("AudioUnitSetProperty failed\n");
			return nullptr;
		}



		result = AudioUnitInitialize(audioUnit);
		if(kAudioHardwareNoError != result) {
			XDEVL_MODULE_ERROR("AudioUnitInitialize failed\n");
			return nullptr;
		}

		result = AudioOutputUnitStart(audioUnit);
		if(kAudioHardwareNoError != result) {
			XDEVL_MODULE_ERROR("AudioOutputUnitStart failed\n");
			return nullptr;
		}

#if MACOSX_COREAUDIO

		/* Fire a callback if the device stops being "alive" (disconnected, etc). */
		AudioObjectAddPropertyListener(deviceID, &aliveAddress, deviceUnplugged, this);
#endif

		m_buffer = new xdl_uint8[4096];

		return std::shared_ptr<XdevLAudioBuffer>(new XdevLAudioBufferCoreAudioBuffer());
	}

	IPXdevLAudioBuffer XdevLAudioCoreAudio::createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data) {
		return create(AUDIO_STREAM_PLAYBACK, format, samplingRate, channels);
	}

	IPXdevLAudioSource XdevLAudioCoreAudio::createAudioSource(IPXdevLAudioBuffer buffer) {
		return nullptr;
	}

	xdl_int XdevLAudioCoreAudio::write(xdl_uint8* buffer) {
		return ERR_OK;
	}

	xdl_int XdevLAudioCoreAudio::read(xdl_uint8* buffer) {
		return ERR_OK;
	}


	xdl_int XdevLAudioCoreAudio::update2() {
		if(nullptr == m_callbackFunction) {
			return ERR_ERROR;
		}

		xdl_uint frames_to_deliver = 100;

		/* deliver the data */

		xdl_uint frames = 0;
		do {
			frames += m_callbackFunction(m_buffer, frames_to_deliver, m_userData);
		} while(frames < frames_to_deliver);


//		if((err = snd_pcm_writei(m_handle, m_buffer, frames_to_deliver)) == -EPIPE) {
//			snd_pcm_prepare(m_handle);
//			XDEVL_MODULE_INFO("Alsa buffer overrun ..."<< std::endl);
//		} else if(err < 0) {
//			// Another issue.
//			XDEVL_MODULE_INFO(snd_strerror(err) << std::endl);
//		}


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

	void XdevLAudioCoreAudio::setCallbackFunction(callbackFunctionType callbackFuntion, void* userData) {
		m_callbackFunction = callbackFuntion;
		m_userData = userData;
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
