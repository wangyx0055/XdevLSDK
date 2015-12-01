#include "XdevLAudioAlsaImpl.h"

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLAUDIOALSA_PLUGIN_MAJOR_VERSION,
	XDEVLAUDIOALSA_PLUGIN_MINOR_VERSION,
	XDEVLAUDIOALSA_PLUGIN_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleDescriptorPlayback {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::moduleDescription,
	XDEVLAUDIOALSA_MODULE_MAJOR_VERSION,
	XDEVLAUDIOALSA_MODULE_MINOR_VERSION,
	XDEVLAUDIOALSA_MODULE_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleDescriptorCapture {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[1],
	xdl::copyright,
	xdl::moduleDescription,
	XDEVLAUDIOALSA_MODULE_MAJOR_VERSION,
	XDEVLAUDIOALSA_MODULE_MINOR_VERSION,
	XDEVLAUDIOALSA_MODULE_PATCH_VERSION
};


extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(moduleDescriptorPlayback.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLAudioPlaybackImpl(parameter, moduleDescriptorPlayback);
		if(!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	} else if(moduleDescriptorCapture.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLAudioCaptureImpl(parameter, moduleDescriptorCapture);
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

	_snd_pcm_format wrapXdevLAudioBufferFormatToAlsaFormat(XdevLAudioBufferFormat format) {
		switch(format) {
			case AUDIO_BUFFER_FORMAT_S8:
				return SND_PCM_FORMAT_S8;
			case AUDIO_BUFFER_FORMAT_U8:
				return SND_PCM_FORMAT_U8;
			case AUDIO_BUFFER_FORMAT_S16:
				return SND_PCM_FORMAT_S16;
			case AUDIO_BUFFER_FORMAT_U16:
				return SND_PCM_FORMAT_U16;
			case AUDIO_BUFFER_FORMAT_S24:
				return SND_PCM_FORMAT_S24;
			case AUDIO_BUFFER_FORMAT_U24:
				return SND_PCM_FORMAT_U24;
			case AUDIO_BUFFER_FORMAT_S32:
				return SND_PCM_FORMAT_S32;
			case AUDIO_BUFFER_FORMAT_U32:
				return SND_PCM_FORMAT_U32;
			case AUDIO_BUFFER_FORMAT_FLOAT:
				return SND_PCM_FORMAT_FLOAT;
			case AUDIO_BUFFER_FORMAT_DOUBLE:
				return SND_PCM_FORMAT_FLOAT64;
			default:
				break;
		}
		return SND_PCM_FORMAT_U8;
	}

	XdevLAudioBufferAlsa::XdevLAudioBufferAlsa(_snd_pcm_format format, XdevLAudioSamplingRate samplingRate, xdl_uint channels) :
		m_bufferFormat(format),
		m_samplingRate(samplingRate),
		m_channels(channels) {
	}

	xdl_int XdevLAudioBufferAlsa::getFormat() {
		switch(m_bufferFormat) {
			case SND_PCM_FORMAT_S8:
				return AUDIO_BUFFER_FORMAT_S8;
				break;
			case SND_PCM_FORMAT_U8:
				return AUDIO_BUFFER_FORMAT_U8;
				break;
			case SND_PCM_FORMAT_S16:
				return AUDIO_BUFFER_FORMAT_S16;
				break;
			case SND_PCM_FORMAT_U16:
				return AUDIO_BUFFER_FORMAT_U16;
				break;
			case SND_PCM_FORMAT_S24:
				return AUDIO_BUFFER_FORMAT_S24;
				break;
			case SND_PCM_FORMAT_U24:
				return AUDIO_BUFFER_FORMAT_U24;
				break;
			case SND_PCM_FORMAT_S32:
				return AUDIO_BUFFER_FORMAT_S32;
				break;
			case SND_PCM_FORMAT_U32:
				return AUDIO_BUFFER_FORMAT_U32;
				break;
			case SND_PCM_FORMAT_FLOAT:
				return AUDIO_BUFFER_FORMAT_FLOAT;
				break;
			case SND_PCM_FORMAT_FLOAT64:
				return AUDIO_BUFFER_FORMAT_DOUBLE;
				break;
		}
		return AUDIO_BUFFER_FORMAT_UNKNOWN;
	}

	xdl_int XdevLAudioBufferAlsa::getSize() {
		return m_bufferSizeInBytes;
	}

	xdl_int XdevLAudioBufferAlsa::getSamplingRate() {
		return m_samplingRate;
	}

	xdl_int XdevLAudioBufferAlsa::getChannels() {
		return m_channels;
	}

	xdl_int XdevLAudioBufferAlsa::getBits() {
		return m_bufferFormatSize * 8;
	}

	xdl_uint XdevLAudioBufferAlsa::getID() {
		return 0;
	}

	xdl_uint8* XdevLAudioBufferAlsa::getData() {
		return nullptr;
	}

	xdl_int XdevLAudioBufferAlsa::lock() {
		return ERR_OK;
	}

	xdl_int XdevLAudioBufferAlsa::unlock() {
		return ERR_OK;
	}

	xdl_int XdevLAudioBufferAlsa::upload(xdl_int8* src, xdl_uint size) {
		return ERR_OK;
	}



//
//
//




	XdevLAudioAlsaBase::XdevLAudioAlsaBase(const XdevLModuleDescriptor& descriptor) :
		m_descriptor(descriptor),
		m_handle(nullptr),
		m_hwParams(nullptr),
		m_name("plughw:0:0"),
		m_channels(1),
		m_callbackFunction(nullptr),
		m_buffer(nullptr) {
	}

	XdevLAudioAlsaBase::~XdevLAudioAlsaBase() {}

	xdl_int XdevLAudioAlsaBase::init() {

//		debugDump();

		return ERR_OK;
	}

	xdl_int XdevLAudioAlsaBase::shutdown() {

		if(nullptr != m_hwParams) {
			snd_pcm_hw_params_free(m_hwParams);
			m_hwParams = nullptr;
		}

		if(nullptr != m_handle) {
			snd_pcm_drop(m_handle);
			snd_pcm_drain(m_handle);
			snd_pcm_close(m_handle);
		}
		if(nullptr != m_buffer) {
			delete [] m_buffer;
		}
		return ERR_OK;
	}

	xdl_int XdevLAudioAlsaBase::createAudioBufferFromFile(const XdevLFileName& filename, XdevLAudioBuffer** buffer) {
		return ERR_OK;
	}

	xdl_int XdevLAudioAlsaBase::create(XdevLAudioStreamType streamType, XdevLAudioBufferFormat bufferFormat, XdevLAudioSamplingRate samplingRate, xdl_uint channels, XdevLAudioBuffer** buffer) {
		xdl_int err;

		m_supportedStreamType = streamType;
		m_samplingRate = samplingRate;
		m_channels = channels;
		m_bufferFormat = wrapXdevLAudioBufferFormatToAlsaFormat(bufferFormat);
		m_bufferFormatSize = XdevLAudioBufferFormatBitSize(bufferFormat)/8;

		switch(streamType) {
			case AUDIO_STREAM_PLAYBACK:
				m_streamType = SND_PCM_STREAM_PLAYBACK;
				break;
			case AUDIO_STREAM_CAPTURE:
				m_streamType = SND_PCM_STREAM_CAPTURE;
				break;
			default:
				m_streamType = SND_PCM_STREAM_PLAYBACK;
				break;
		}

		// Open device
		if(openDevice(m_streamType) != ERR_OK) {
			return ERR_ERROR;
		}

		// Set parameters for the device.
		if(setHardwareParameters(m_channels, m_bufferFormat, m_samplingRate) != ERR_OK) {
			return ERR_ERROR;
		}

		// Apply the parameters
		if(applyHardwareParameters() != ERR_OK) {
			return ERR_ERROR;
		}

		//
		// Print out some infos:
		//
		XDEVL_MODULE_INFO("PCM Device info \n");
		XDEVL_MODULE_INFO("PCM name        : " << snd_pcm_name(m_handle) << "\n");
		XDEVL_MODULE_INFO("PCM state       : " << snd_pcm_state_name(snd_pcm_state(m_handle)) << "\n");

		xdl_uint tmp;
		snd_pcm_hw_params_get_channels(m_hwParams, &tmp);
		XDEVL_MODULE_INFO("PCM channels    : " << tmp << "\n");

		snd_pcm_hw_params_get_rate(m_hwParams, &tmp, nullptr);
		XDEVL_MODULE_INFO("PCM rate        : " << tmp << "\n");

		snd_pcm_hw_params_get_period_time(m_hwParams, &tmp, nullptr);
		XDEVL_MODULE_INFO("PCM period time : " << tmp << "\n");
		XDEVL_MODULE_INFO("PCM period size : " << m_periodSize << "\n");
		XDEVL_MODULE_INFO("--------------------------------------------\n");

		// Now make the hardware ready to use.
		if ((err = snd_pcm_prepare (m_handle)) < 0) {
			return ERR_ERROR;
		}

		XdevLAudioBufferAlsa* bufferAlsa = new XdevLAudioBufferAlsa(m_bufferFormat, samplingRate, m_channels);

		return ERR_OK;
	}

	xdl_int XdevLAudioAlsaBase::createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data, XdevLAudioBuffer** buffer) {
		m_samplingRate = samplingRate;
		m_channels = channels;
		m_bufferFormat = wrapXdevLAudioBufferFormatToAlsaFormat(format);
		m_bufferFormatSize = XdevLAudioBufferFormatBitSize(format)/8;
		XdevLAudioBufferAlsa* tmp = new XdevLAudioBufferAlsa(m_bufferFormat, samplingRate, channels);

		*buffer = tmp;
	}

	XdevLString XdevLAudioAlsaBase::getDeviceName() {
		const char* deviceName = getenv("AUDIODEV");
		if(nullptr != deviceName) {
			return XdevLString(deviceName);
		}

		switch(m_channels) {
			case 6:
				return XdevLString("plug:surround51");
			case 4:
				return XdevLString("plug:surround40");
			default:
				break;
		}

		return XdevLString("default");
	}

	xdl_int XdevLAudioAlsaBase::openDevice(snd_pcm_stream_t streamType) {
		xdl_int err;
		//
		// Open the PCM device.
		//
		if ((err = snd_pcm_open(&m_handle, getDeviceName().toString().c_str(), streamType, 0)) < 0) {
			XDEVL_MODULE_INFO("Could not open PCM device: " << getDeviceName() << " ->" << snd_strerror(err) << std::endl);
			return ERR_ERROR;
		}
		return ERR_OK;
	}

	xdl_int XdevLAudioAlsaBase::setHardwareParameters(xdl_uint numberOfChannels, _snd_pcm_format bufferformat, xdl_uint samplingRate) {
		xdl_int err;

		if(nullptr != m_hwParams) {
			snd_pcm_hw_params_free(m_hwParams);
			m_hwParams = nullptr;
		}

		// Allocate parameters object.
		// TODO Do we have to free this allocated parameter?
		if ((err = snd_pcm_hw_params_malloc(&m_hwParams)) < 0) {
			XDEVL_MODULE_INFO("Could allocate memory for hardware parameter " << "->" << snd_strerror(err) << std::endl);
			return ERR_ERROR;
		}

		// Get the default values and put it into the structure.
		if ((err = snd_pcm_hw_params_any(m_handle, m_hwParams)) < 0) {
			return ERR_ERROR;
		}

		// Set interleaved mode for data access. (This tells how the samples for the different channels are ordered in RAM.
		// Interleaving means that we alternate samples for the left and right channel (LRLRLR)
		if ((err = snd_pcm_hw_params_set_access(m_handle, m_hwParams, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
			return ERR_ERROR;
		}

		// Desired buffer format ( 8, 16, 32 etc. bit)
		if ((err = snd_pcm_hw_params_set_format(m_handle, m_hwParams, bufferformat)) < 0) {
			XDEVL_MODULE_INFO("Could set hardware format:" << snd_strerror(err) << std::endl);
			return ERR_ERROR;
		}

		// Desired sampling rate.
		if ((err = snd_pcm_hw_params_set_rate_near(m_handle, m_hwParams, &samplingRate, 0)) < 0) {
			XDEVL_MODULE_INFO("Could not set the desired sampling rate: " << samplingRate << " : " << snd_strerror(err) << std::endl);
			return ERR_ERROR;
		}

		// Number of channels.
		if ((err = snd_pcm_hw_params_set_channels(m_handle, m_hwParams, numberOfChannels)) < 0) {
			if( (err = snd_pcm_hw_params_get_channels(m_hwParams, &numberOfChannels)) < 0) {
				XDEVL_MODULE_INFO("Could not set the number of channels: " << numberOfChannels << " : " << snd_strerror(err) << std::endl);
				return ERR_ERROR;
			}
		}
		return ERR_OK;
	}

	xdl_int XdevLAudioAlsaBase::applyHardwareParameters() {
		xdl_int err;

		// Write the parameters to the driver
		if( (err = snd_pcm_hw_params(m_handle, m_hwParams)) < 0) {
			XDEVL_MODULE_INFO("Could write parameters to hardware: " << snd_strerror(err) << std::endl);
			return ERR_ERROR;
		}

		//
		// Allocate buffer for one period
		//

		// TODO For now let's use this buffer to transfer data.
		snd_pcm_hw_params_get_period_size(m_hwParams, &m_periodSize, nullptr);
		m_bufferSize = m_periodSize * m_channels * m_bufferFormatSize;

		m_buffer = new xdl_uint8[m_bufferSize];

		snd_pcm_hw_params_get_period_time(m_hwParams, &m_periodTime, nullptr);

		return ERR_OK;
	}


	xdl_int XdevLAudioAlsaBase::setPeriodSize(snd_pcm_uframes_t frames) {

		if(snd_pcm_hw_params_set_period_size_near(m_handle, m_hwParams, &frames, nullptr) < 0) {
			return ERR_ERROR;
		}

		xdl_uint periods = 2;
		if(snd_pcm_hw_params_set_periods_near(m_handle, m_hwParams, &periods, nullptr) < 0) {
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	xdl_int XdevLAudioAlsaBase::write(xdl_uint8* buffer) {
		xdl_int err;
		if( (err = snd_pcm_writei(m_handle, buffer, m_periodSize)) == EPIPE) {
			snd_pcm_prepare(m_handle);
			XDEVL_MODULE_INFO("Alsa buffer overrun ..."<< std::endl);
		} else if(err < 0) {
			// Another issue.
			XDEVL_MODULE_INFO( snd_strerror(err) << std::endl);
		}
		return ERR_OK;
	}

	xdl_int XdevLAudioAlsaBase::read(xdl_uint8* buffer) {
		xdl_int err;
		err = snd_pcm_readi(m_handle, buffer, m_periodSize);
		if (err == -EPIPE) {
			/* EPIPE means overrun */
			fprintf(stderr, "overrun occurred\n");
			snd_pcm_prepare(m_handle);
		} else if (err < 0) {
			fprintf(stderr,"error from read: %s\n", snd_strerror(err));
		} else if (err != (int)m_periodSize) {
			fprintf(stderr, "short read, read %d frames\n", err);
		}
	}

	void XdevLAudioAlsaBase::debugDump() {
		int val;

		printf("ALSA library version: %s\n", SND_LIB_VERSION_STR);

		printf("\nPCM stream types:\n");
		for (val = 0; val <= SND_PCM_STREAM_LAST; val++) {
			if( snd_pcm_stream_name((snd_pcm_stream_t)val) == "PLAYBACK") {

			} else if( snd_pcm_stream_name((snd_pcm_stream_t)val) == "CAPTURE") {

			}
		}
		printf("\nPCM access types:\n");
		for (val = 0; val <= SND_PCM_ACCESS_LAST; val++)
			printf("  %s\n", snd_pcm_access_name((snd_pcm_access_t)val));

		printf("\nPCM formats:\n");
		for (val = 0; val <= SND_PCM_FORMAT_LAST; val++) {
			if (snd_pcm_format_name((snd_pcm_format_t)val) != NULL)
				printf("  %s (%s)\n",  snd_pcm_format_name((snd_pcm_format_t)val), snd_pcm_format_description( (snd_pcm_format_t)val));
		}
		printf("\nPCM subformats:\n");
		for (val = 0; val <= SND_PCM_SUBFORMAT_LAST; val++)
			printf("  %s (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)val), snd_pcm_subformat_description((snd_pcm_subformat_t)val));

		printf("\nPCM states:\n");
		for (val = 0; val <= SND_PCM_STATE_LAST; val++)
			printf("  %s\n",
			       snd_pcm_state_name((snd_pcm_state_t)val));
	}

	xdl_int XdevLAudioAlsaBase::update2() {
		if(nullptr == m_callbackFunction) {
			return ERR_ERROR;
		}

		xdl_int err;
		if ((err = snd_pcm_wait (m_handle, 1000)) < 0) {
			fprintf (stderr, "poll failed (%s)\n", strerror (err));
			return ERR_ERROR;
		}

		/* find out how much space is available for playback data */
		snd_pcm_sframes_t frames_to_deliver;
		if ((frames_to_deliver = snd_pcm_avail_update (m_handle)) < 0) {
			if (frames_to_deliver == -EPIPE) {
				fprintf (stderr, "an xrun occured\n");
				return ERR_ERROR;
			} else {
				fprintf (stderr, "unknown ALSA avail update return value (%d)\n", (xdl_uint)frames_to_deliver);
				return ERR_ERROR;
			}
		}

		frames_to_deliver = frames_to_deliver > m_periodSize ? m_periodSize : frames_to_deliver;

		/* deliver the data */

		xdl_uint frames = 0;
		do {
			frames += m_callbackFunction(m_buffer, frames_to_deliver, m_userData);
		} while ( frames < frames_to_deliver);


		if( (err = snd_pcm_writei(m_handle, m_buffer, frames_to_deliver)) == -EPIPE) {
			snd_pcm_prepare(m_handle);
			XDEVL_MODULE_INFO("Alsa buffer overrun ..."<< std::endl);
		} else if(err < 0) {
			// Another issue.
			XDEVL_MODULE_INFO( snd_strerror(err) << std::endl);
		}

		return ERR_OK;
	}

	xdl_uint XdevLAudioAlsaBase::getSamplingRate() {
		return m_samplingRate;
	}

	xdl_uint XdevLAudioAlsaBase::getBufferSize() {
		return m_bufferSize;
	}

	xdl_uint XdevLAudioAlsaBase::getNumberOfChannels() {
		return m_channels;
	}
	xdl_uint XdevLAudioAlsaBase::getFormatSizeInBytes() {
		return m_bufferFormatSize;
	}
	xdl_uint XdevLAudioAlsaBase::getPeriodTime() {
		return m_periodTime;
	}

	xdl_uint XdevLAudioAlsaBase::getPeriodSize() {
		return m_periodSize;
	}

	xdl_int XdevLAudioAlsaBase::setCallbackFunction(callbackFunctionType callbackFuntion, void* userData) {
		m_callbackFunction = callbackFuntion;
		m_userData = userData;
	}

	void XdevLAudioAlsaBase::setGain(xdl_float gain) {
		m_gain = gain;
	}
	xdl_int XdevLAudioAlsaBase::makeCurrent() {
		return ERR_OK;
	}
	xdl_int XdevLAudioAlsaBase::releaseCurrent() {
		return ERR_OK;
	}

	//
	// Playback
	//


	XdevLAudioPlaybackImpl::XdevLAudioPlaybackImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLAudioAlsaBase(descriptor),
		XdevLModuleImpl<XdevLAudioPlayback> (parameter, descriptor) {
	}

	XdevLAudioPlaybackImpl::~XdevLAudioPlaybackImpl() {}

	xdl_int XdevLAudioPlaybackImpl::init() {
		return XdevLAudioAlsaBase::init();
	}

	xdl_int XdevLAudioPlaybackImpl::shutdown() {
		return XdevLAudioAlsaBase::shutdown();
	}

	xdl_int XdevLAudioPlaybackImpl::createAudioBufferFromFile(const XdevLFileName& filename, XdevLAudioBuffer** buffer) {
		return XdevLAudioAlsaBase::createAudioBufferFromFile(filename, buffer);
	}

//	xdl_int XdevLAudioPlaybackImpl::create(XdevLAudioBufferFormat bufferFormat, XdevLAudioSamplingRate samplingRate, xdl_uint channels, XdevLAudioBuffer** buffer) {
//		return XdevLAudioAlsaBase::create(AUDIO_STREAM_PLAYBACK, bufferFormat, samplingRate, channels, buffer);
//	}

	xdl_int XdevLAudioPlaybackImpl::createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data, XdevLAudioBuffer** buffer) {
		return XdevLAudioAlsaBase::create(AUDIO_STREAM_PLAYBACK, format, samplingRate, channels, buffer);
	}

	xdl_int XdevLAudioPlaybackImpl::createAudioSource(XdevLAudioSource** src, XdevLAudioBuffer* buffer) {
		return ERR_ERROR;
	}

	xdl_int XdevLAudioPlaybackImpl::write(xdl_uint8* buffer) {
		return XdevLAudioAlsaBase::write(buffer);
	}

	xdl_int XdevLAudioPlaybackImpl::update2() {
		return XdevLAudioAlsaBase::update2();
	}

	xdl_uint XdevLAudioPlaybackImpl::getSamplingRate() {
		return XdevLAudioAlsaBase::getSamplingRate();
	}

	xdl_uint XdevLAudioPlaybackImpl::getBufferSize() {
		return XdevLAudioAlsaBase::getBufferSize();
	}

	xdl_uint XdevLAudioPlaybackImpl::getNumberOfChannels() {
		return XdevLAudioAlsaBase::getNumberOfChannels();
	}
	xdl_uint XdevLAudioPlaybackImpl::getFormatSizeInBytes() {
		return XdevLAudioAlsaBase::getFormatSizeInBytes();
	}
	xdl_uint XdevLAudioPlaybackImpl::getPeriodTime() {
		return XdevLAudioAlsaBase::getPeriodTime();
	}

	xdl_uint XdevLAudioPlaybackImpl::getPeriodSize() {
		return XdevLAudioAlsaBase::getPeriodSize();
	}

	xdl_int XdevLAudioPlaybackImpl::setCallbackFunction(callbackFunctionType callbackFuntion, void* userData) {
		return XdevLAudioAlsaBase::setCallbackFunction(callbackFuntion, userData);
	}

	void XdevLAudioPlaybackImpl::setGain(xdl_float gain) {
		return XdevLAudioAlsaBase::setGain(gain);
	}
	xdl_int XdevLAudioPlaybackImpl::makeCurrent() {
		return XdevLAudioAlsaBase::makeCurrent();
	}
	xdl_int XdevLAudioPlaybackImpl::releaseCurrent() {
		return XdevLAudioAlsaBase::releaseCurrent();
	}



	//
	// Capture
	//

	XdevLAudioCaptureImpl::XdevLAudioCaptureImpl(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLAudioAlsaBase(descriptor),
		XdevLModuleImpl<XdevLAudioCapture> (parameter, descriptor) {
	}

	XdevLAudioCaptureImpl::~XdevLAudioCaptureImpl() {}

	xdl_int XdevLAudioCaptureImpl::init() {
		return XdevLAudioAlsaBase::init();
	}

	xdl_int XdevLAudioCaptureImpl::shutdown() {
		return XdevLAudioAlsaBase::shutdown();
	}

	xdl_int XdevLAudioCaptureImpl::createAudioBufferFromFile(const XdevLFileName& filename, XdevLAudioBuffer** buffer) {
		return XdevLAudioAlsaBase::createAudioBufferFromFile(filename, buffer);
	}

//	xdl_int XdevLAudioCaptureImpl::create(XdevLAudioBufferFormat bufferFormat, XdevLAudioSamplingRate samplingRate, xdl_uint channels, XdevLAudioBuffer** buffer) {
//		return XdevLAudioAlsaBase::create(AUDIO_STREAM_CAPTURE, bufferFormat, samplingRate, channels, buffer);
//	}

	xdl_int XdevLAudioCaptureImpl::createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data, XdevLAudioBuffer** buffer) {
		return XdevLAudioAlsaBase::create(AUDIO_STREAM_CAPTURE, format, samplingRate, channels, buffer);
	}
	
	xdl_int XdevLAudioCaptureImpl::createAudioSource(XdevLAudioSource** src, XdevLAudioBuffer* buffer) {
		return ERR_ERROR;
	}

	xdl_int XdevLAudioCaptureImpl::read(xdl_uint8* buffer) {
		return XdevLAudioAlsaBase::read(buffer);
	}

	xdl_int XdevLAudioCaptureImpl::update2() {
		return XdevLAudioAlsaBase::update2();
	}

	xdl_uint XdevLAudioCaptureImpl::getSamplingRate() {
		return XdevLAudioAlsaBase::getSamplingRate();
	}

	xdl_uint XdevLAudioCaptureImpl::getBufferSize() {
		return XdevLAudioAlsaBase::getBufferSize();
	}

	xdl_uint XdevLAudioCaptureImpl::getNumberOfChannels() {
		return XdevLAudioAlsaBase::getNumberOfChannels();
	}
	xdl_uint XdevLAudioCaptureImpl::getFormatSizeInBytes() {
		return XdevLAudioAlsaBase::getFormatSizeInBytes();
	}
	xdl_uint XdevLAudioCaptureImpl::getPeriodTime() {
		return XdevLAudioAlsaBase::getPeriodTime();
	}

	xdl_uint XdevLAudioCaptureImpl::getPeriodSize() {
		return XdevLAudioAlsaBase::getPeriodSize();
	}

	xdl_int XdevLAudioCaptureImpl::setCallbackFunction(callbackFunctionType callbackFuntion, void* userData) {
		return XdevLAudioAlsaBase::setCallbackFunction(callbackFuntion, userData);
	}

	void XdevLAudioCaptureImpl::setGain(xdl_float gain) {
		return XdevLAudioAlsaBase::setGain(gain);
	}
	xdl_int XdevLAudioCaptureImpl::makeCurrent() {
		return XdevLAudioAlsaBase::makeCurrent();
	}
	xdl_int XdevLAudioCaptureImpl::releaseCurrent() {
		return XdevLAudioAlsaBase::releaseCurrent();
	}

}
