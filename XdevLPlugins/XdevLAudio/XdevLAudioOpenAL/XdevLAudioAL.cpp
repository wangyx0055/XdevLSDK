
#include <iostream>
#include <string>
#include <XdevLCoreMediator.h>
#include "XdevLAudioAL.h"
#include <XdevLPlatform.h>

#include <fstream>

xdl::XdevLModuleDescriptor moduleAudioDesc {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[0],
	xdl::copyright,
	xdl::description,
	XDEVLAUDIO_MAJOR_VERSION,
	XDEVLAUDIO_MINOR_VERSION,
	XDEVLAUDIO_PATCH_VERSION
};

xdl::XdevLModuleDescriptor moduleAudioRecordDescriptor {
	xdl::vendor,
	xdl::author,
	xdl::moduleNames[1],
	xdl::copyright,
	xdl::description2,
	XDEVLAUDIORECORD_MODULE_MAJOR_VERSION,
	XDEVLAUDIORECORD_MODULE_MINOR_VERSION,
	XDEVLAUDIORECORD_MODULE_PATCH_VERSION
};

xdl::XdevLPluginDescriptor pluginDescriptor {
	xdl::pluginName,
	xdl::moduleNames,
	XDEVLAUDIO_MODULE_MAJOR_VERSION,
	XDEVLAUDIO_MODULE_MINOR_VERSION,
	XDEVLAUDIO_MODULE_PATCH_VERSION
};

XDEVL_PLUGIN_INIT_DEFAULT
XDEVL_PLUGIN_SHUTDOWN_DEFAULT
XDEVL_PLUGIN_DELETE_MODULE_DEFAULT
XDEVL_PLUGIN_GET_DESCRIPTOR_DEFAULT(pluginDescriptor);

XDEVL_PLUGIN_CREATE_MODULE {
	XDEVL_PLUGIN_CREATE_MODULE_INSTANCE(xdl::XdevLAudioPlaybackAL, moduleAudioDesc);
	XDEVL_PLUGIN_CREATE_MODULE_NOT_FOUND
}

namespace xdl {


	struct RIFF_Header {
		xdl_int8 chunkID[4];
		xdl_int32 chunkSize;
	};

	struct RIFF_Type {
		xdl_int8 ID[4];
	};

	struct WAVE_Format {
		xdl_int16 audioFormat;
		xdl_int16 numChannels;
		xdl_int32 sampleRate;
		xdl_int32 byteRate;
		xdl_int16 blockAlign;
		xdl_int16 bitsPerSample;
	};

	xdl_bool isChunckID(const RIFF_Header& header, const xdl_char a,  const xdl_char b,  const xdl_char c,  const xdl_char d) {
		if(header.chunkID[0] != a || header.chunkID[1] != b || header.chunkID[2] != c || header.chunkID[3] != d) {
			return xdl_false;
		}
		return xdl_true;;
	}

	xdl_bool isRIFFID(const RIFF_Type& type, const xdl_char a,  const xdl_char b,  const xdl_char c,  const xdl_char d) {
		if(type.ID[0] != a || type.ID[1] != b || type.ID[2] != c || type.ID[3] != d) {
			return xdl_false;
		}
		return xdl_true;;
	}


	std::string getALErrorString(ALenum err) {
		switch(err) {
			case AL_NO_ERROR:
				return std::string("AL_NO_ERROR");
				break;

			case AL_INVALID_NAME:
				return std::string("AL_INVALID_NAME");
				break;

			case AL_INVALID_ENUM:
				return std::string("AL_INVALID_ENUM");
				break;

			case AL_INVALID_VALUE:
				return std::string("AL_INVALID_VALUE");
				break;

			case AL_INVALID_OPERATION:
				return std::string("AL_INVALID_OPERATION");
				break;

			case AL_OUT_OF_MEMORY:
				return std::string("AL_OUT_OF_MEMORY");
				break;
		};
		return std::string("UNKNOWN_AL_ERROR");
	}

	std::string getALCErrorString(ALenum err) {
		switch(err) {
			case ALC_NO_ERROR:
				return std::string("AL_NO_ERROR");
				break;

			case ALC_INVALID_DEVICE:
				return std::string("ALC_INVALID_DEVICE");
				break;

			case ALC_INVALID_CONTEXT:
				return std::string("ALC_INVALID_CONTEXT");
				break;

			case ALC_INVALID_ENUM:
				return std::string("ALC_INVALID_ENUM");
				break;

			case ALC_INVALID_VALUE:
				return std::string("ALC_INVALID_VALUE");
				break;

			case ALC_OUT_OF_MEMORY:
				return std::string("ALC_OUT_OF_MEMORY");
				break;
		};
		return std::string("UNKNOWN_ALC_ERROR");
	}



	ALenum wrapAudioBufferFormat(XdevLAudioBufferFormat format, xdl_uint numberOfChannels) {
		ALenum alFormat = 0;
		switch(format) {
			case AUDIO_BUFFER_FORMAT_S8: {
				if(numberOfChannels == 1) {
					alFormat = AL_FORMAT_MONO8;
				} else if(numberOfChannels == 2) {
					alFormat = AL_FORMAT_STEREO8;
				} else {
					alFormat = AL_FORMAT_MONO8;
				}
			}
			break;
			case AUDIO_BUFFER_FORMAT_S16: {
				if(numberOfChannels == 1) {
					alFormat = AL_FORMAT_MONO16;
				} else if(numberOfChannels == 2) {
					alFormat = AL_FORMAT_STEREO16;
				} else {
					alFormat = AL_FORMAT_MONO8;
				}
			}
			break;
			case AUDIO_BUFFER_FORMAT_UNKNOWN:
			default: {
				alFormat = AL_FORMAT_MONO8;
			}
			break;
		}
		return alFormat;
	}




	XdevLAudioPlaybackAL::XdevLAudioPlaybackAL(XdevLModuleCreateParameter* parameter, const XdevLModuleDescriptor& descriptor) :
		XdevLModuleImpl<XdevLAudio>(parameter, descriptor),
		m_device(nullptr),
		m_context(nullptr),
		m_previous(nullptr),
		m_callbackFunction(nullptr), 
		m_userData(nullptr) {
		m_audioSources.reserve(10);
		m_audioSources.resize(10);
		m_audioBuffers.reserve(10);
		m_audioBuffers.resize(10);
	}

	XdevLAudioPlaybackAL::~XdevLAudioPlaybackAL() {
	}

	xdl_int XdevLAudioPlaybackAL::setCallbackFunction(callbackFunctionType callbackFuntion, void* userData) {
		m_callbackFunction = callbackFuntion;
		m_userData = userData;
	}

	xdl_int XdevLAudioPlaybackAL::makeCurrent() {
		m_previous = alcGetCurrentContext();
		alcMakeContextCurrent(m_context);
		return ERR_OK;
	}

	xdl_int XdevLAudioPlaybackAL::releaseCurrent() {
		alcMakeContextCurrent(m_previous);
		return ERR_OK;
	}
	xdl_int XdevLAudioPlaybackAL::init() {
		m_device = alcOpenDevice(nullptr);
		if(m_device == nullptr) {
			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
			return ERR_ERROR;
		}
		m_context = alcCreateContext(m_device, nullptr);
		if(m_context == nullptr) {
			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
			return ERR_ERROR;
		}

		alcMakeContextCurrent(m_context);

		alDistanceModel(AL_NONE);


		return ERR_OK;
	}

	xdl_int XdevLAudioPlaybackAL::shutdown() {
		// Delete all sources.
		std::vector<XdevLAudioSourceAL*>::iterator asi(m_audioSources.begin());
		for(; asi != m_audioSources.end(); ++asi) {
			delete(*asi);
		}

		// Delete all buffers.
		std::vector<XdevLAudioBufferAL*>::iterator abi(m_audioBuffers.begin());
		for(; abi != m_audioBuffers.end(); ++abi)
			delete *abi;

//		alutExit();
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);

		return ERR_OK;
	}

	xdl_int XdevLAudioPlaybackAL::createAudioBufferFromFile(const XdevLFileName& filename, IPXdevLAudioBuffer* buffer) {
		ALuint id = 0;

		XdevLAudioBufferFormat audioFormat = AUDIO_BUFFER_FORMAT_UNKNOWN;
		xdl_uint numberOfChannels = 1;
		xdl_int size									= 0;
		xdl_int samplingRate 					= 0;

		std::vector<char> rawData;
		std::ifstream ifile(filename, std::ifstream::binary);

		RIFF_Header riff_header {{0}};
		ifile.read((char*)&riff_header, sizeof(RIFF_Header));
		if(isChunckID(riff_header, 'R', 'I', 'F', 'F')) {

			RIFF_Type riff_type {{0}};
			ifile.read((char*)&riff_type, sizeof(RIFF_Type));

			if(isRIFFID(riff_type, 'W', 'A', 'V', 'E')) {

				do {

					// Read chunck header.
					RIFF_Header	wave_format_header {{0}};
					ifile.read((char*)&wave_format_header, sizeof(RIFF_Header));

					//
					// Check which chunck type it is.
					//
					if(isChunckID(wave_format_header, 'f', 'm', 't', ' ')) {

						WAVE_Format	wave_format {0};
						ifile.read((char*)&wave_format, sizeof(WAVE_Format));
						size = wave_format_header.chunkSize;
						samplingRate = wave_format.sampleRate;
						numberOfChannels = wave_format.numChannels;

						if(wave_format.bitsPerSample == 8)
							audioFormat = AUDIO_BUFFER_FORMAT_S8;
						else if(wave_format.bitsPerSample == 16)
							audioFormat = AUDIO_BUFFER_FORMAT_S16;


					} else if(isChunckID(wave_format_header, 'd', 'a', 't', 'a')) {

						//
						// Read the raw data here.
						//
						rawData.reserve(wave_format_header.chunkSize);
						rawData.resize(wave_format_header.chunkSize);
						ifile.read(rawData.data(), wave_format_header.chunkSize);

					} else {
						// TODO Do the info stuff
						ifile.seekg(wave_format_header.chunkSize, std::ios_base::cur);
					}

					// Stop if we have a problem.
					// TODO better solution.
					int pos = ifile.tellg();
					if (pos == -1) {
						break;
					}

				} while(true);

			}

		}

		ifile.close();

		ALuint format = 0;
		switch(audioFormat) {
			case AUDIO_BUFFER_FORMAT_S8: {
				if(numberOfChannels == 1) {
					format = AL_FORMAT_MONO8;
				} else if(numberOfChannels == 2) {
					format = AL_FORMAT_STEREO8;
				} else {
					XDEVL_MODULE_ERROR("Number of channels not supported.");
					return ERR_ERROR;
				}
			}
			break;
			case AUDIO_BUFFER_FORMAT_S16: {
				if(numberOfChannels == 1) {
					format = AL_FORMAT_MONO16;
				} else if(numberOfChannels == 2) {
					format = AL_FORMAT_STEREO16;
				} else {
					XDEVL_MODULE_ERROR("Number of channels not supported.");
					return ERR_ERROR;
				}
			}
			break;
			case AUDIO_BUFFER_FORMAT_UNKNOWN:
			default: {
				format = AL_FORMAT_MONO8;
				XDEVL_MODULE_ERROR("AUDIO_BUFFER_FORMAT_UNKNOWN");
			}
			break;
		}

		XdevLAudioBufferAL* buf = new XdevLAudioBufferAL(format, samplingRate, rawData.size());
		buf->lock();
		buf->upload((xdl_int8*)rawData.data(), rawData.size());
		buf->unlock();

		*buffer = buf;

		m_audioBuffers.push_back(buf);
		XDEVL_MODULE_INFO("AudioFile: " << filename << ", ID: " << id << " successfully created. " << std::endl);
		return ERR_OK;
	}

	xdl_int XdevLAudioPlaybackAL::createAudioBuffer(XdevLAudioBufferFormat format, XdevLAudioSamplingRate samplingRate, xdl_uint channels, xdl_int size, void* data, IPXdevLAudioBuffer* buffer) {

		ALenum fmat = wrapAudioBufferFormat(format, channels);

		XdevLAudioBufferAL* buf = new XdevLAudioBufferAL(fmat, samplingRate, size);
		buf->lock();
		buf->upload((xdl_int8*)data, size);
		buf->unlock();

		*buffer = buf;

		return ERR_OK;
	}

	xdl_int XdevLAudioPlaybackAL::createAudioSource(IPXdevLAudioSource* src, IPXdevLAudioBuffer buffer) {

		XdevLAudioSourceAL* audioSource = new XdevLAudioSourceAL();
		if(!audioSource) {
			XDEVL_MODULE_ERROR("Could not allocate\n");
			return ERR_ERROR;
		}

		alSourcei(audioSource->getID(), AL_BUFFER,  buffer->getID());
		*src = audioSource;
		m_audioSources.push_back(audioSource);
		return ERR_OK;
	}

	void XdevLAudioPlaybackAL::setGain(xdl_float gain) {
		alListenerf(AL_GAIN, (ALfloat)gain);
		xdl_int ret = AL_NO_ERROR;
		if((ret = alGetError()) != AL_NO_ERROR) {
			XDEVL_MODULE_WARNING(getALErrorString(ret) << "\n");
		}
	}

//
// XdevLAudioALRecord
//

//	XdevLAudioALRecord::XdevLAudioALRecord(XdevLModuleCreateParameter* parameter) : XdevLModuleImpl<XdevLAudioRecord>(parameter, moduleAudioRecordDescriptor),
//		m_recordDevice(nullptr),
//		m_recordingStarted(xdl_false) {};
//
//	xdl_int XdevLAudioALRecord::init() {
//
//		if(alcIsExtensionPresent(nullptr, "AL_EXT_capture") == AL_TRUE) {
//			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
//			return ERR_ERROR;
//		}
//
//		std::cout << alcGetString(nullptr, ALC_CAPTURE_DEVICE_SPECIFIER) << std::endl;
//
//		return ERR_OK;
//	}
//
//
//	xdl_int XdevLAudioALRecord::shutdown() {
//		if(alcCaptureCloseDevice(m_recordDevice) == AL_FALSE) {
//			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
//			return ERR_ERROR;
//		}
//		return ERR_OK;
//	}
//
//
//	xdl_int XdevLAudioALRecord::device(const XdevLString& deviceName, XdevLAudioBuffer* audioBuffer, xdl_int samples) {
//		m_bufferSize = samples;
//
//		if(deviceName.toString() == "default") {
//			m_recordDevice = alcCaptureOpenDevice(nullptr, audioBuffer->getSamplingRate(), audioBuffer->getFormat(), samples);
//		} else {
//			m_recordDevice = alcCaptureOpenDevice(deviceName.toString().c_str(), m_sampleRate, m_audioFormat, m_bufferSize);
//		}
//		if(m_recordDevice == nullptr) {
//			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
//			return ERR_ERROR;
//		}
//
//		return ERR_OK;
//	}
//
//	xdl_int XdevLAudioALRecord::device(const XdevLString& deviceName, XdevLAudioBufferFormat format,  XdevLAudioSamplingRate samplerate, xdl_uint channels, xdl_int samples) {
//
//		m_audioFormat = wrapAudioBufferFormat(format, channels);
//
//		switch(format) {
//			case AUDIO_BUFFER_FORMAT_S8:
//				m_bufferSize = 1 * channels;
//				break;
//			case AUDIO_BUFFER_FORMAT_S16:
//				m_bufferSize = 2 * channels;
//				break;
//			default:
//				break;
//		}
//
//		m_sampleSize = m_bufferSize;
//
//		m_sampleRate = AUDIO_SAMPLE_RATE_11025;
//		switch(samplerate) {
//			case AUDIO_SAMPLE_RATE_8000:
//				m_sampleRate = AUDIO_SAMPLE_RATE_8000;
//				break;
//			case AUDIO_SAMPLE_RATE_11025:
//				m_sampleRate = AUDIO_SAMPLE_RATE_11025;
//				break;
//			case AUDIO_SAMPLE_RATE_22050:
//				m_sampleRate = AUDIO_SAMPLE_RATE_22050;
//				break;
//			case AUDIO_SAMPLE_RATE_44100:
//				m_sampleRate = AUDIO_SAMPLE_RATE_44100;
//				break;
//			case AUDIO_SAMPLE_RATE_48000:
//				m_sampleRate = AUDIO_SAMPLE_RATE_48000;
//				break;
//			case AUDIO_SAMPLE_RATE_96000:
//				m_sampleRate = AUDIO_SAMPLE_RATE_96000;
//				break;
//			default:
//				break;
//		}
//
//		m_bufferSize *=  samples;
//		if(deviceName.toString() == "default") {
//			m_recordDevice = alcCaptureOpenDevice(nullptr, m_sampleRate, m_audioFormat, m_bufferSize);
//		} else {
//			m_recordDevice = alcCaptureOpenDevice(deviceName.toString().c_str(), m_sampleRate, m_audioFormat, m_bufferSize);
//		}
//		if(m_recordDevice == nullptr) {
//			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
//			return ERR_ERROR;
//		}
//
//		return ERR_OK;
//	}
//
//	void XdevLAudioALRecord::start() {
//		alcCaptureStart(m_recordDevice);
//
//		ALenum error = alGetError();
//		if(error != AL_NO_ERROR) {
//			std::cerr << getALErrorString(error) << std::endl;
//		}
//
//		m_recordingStarted = xdl_true;
//	}
//
//	void XdevLAudioALRecord::stop() {
//		alcCaptureStop(m_recordDevice);
//
//		ALenum error = alGetError();
//		if(error != AL_NO_ERROR) {
//			std::cerr << getALErrorString(error) << std::endl;
//		}
//
//		m_recordingStarted = xdl_false;
//	}
//
//	xdl_int XdevLAudioALRecord::capture(XdevLAudioBuffer* audioBuffer) {
//		XDEVL_ASSERT(m_recordingStarted == true, "You have to use start() first");
//
//		static std::vector<xdl_int8> buffer;
//		if(buffer.size() < audioBuffer->getSize()) {
//			buffer.reserve(audioBuffer->getSize());
//			buffer.resize(audioBuffer->getSize());
//		}
//
//		ALint tmp = 0;
//		ALint capturedSamples = 0;
//		while( capturedSamples < audioBuffer->getSize() ) {
//			alcGetIntegerv(m_recordDevice, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALbyte), &tmp);
//			alcCaptureSamples(m_recordDevice, (ALCvoid*)buffer.data(), m_bufferSize);
//
//			capturedSamples += tmp;
//			std::cout << capturedSamples << std::endl;
//		}
//
//		ALenum error = alGetError();
//		if(error != AL_NO_ERROR) {
//			std::cerr << getALErrorString(error) << std::endl;
//		}
//
//		audioBuffer->lock();
//		audioBuffer->upload(buffer.data(), audioBuffer->getSize());
//		audioBuffer->unlock();
//
//		return ERR_OK;
//	}
//
//	xdl_int XdevLAudioALRecord::capture(xdl_int8* buffer, xdl_int samples) {
//		XDEVL_ASSERT(m_recordingStarted == true, "You have to use start() first");
//
//		ALint tmp = 0;
//		ALint capturedSamples = 0;
//		while(tmp < samples) {
//			alcGetIntegerv(m_recordDevice, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALbyte), &tmp);
//			capturedSamples += tmp;
//		}
//		alcCaptureSamples(m_recordDevice, (ALCvoid*)buffer, capturedSamples);
//		return ERR_OK;
//	}
//
//	xdl_int XdevLAudioALRecord::sampleSize() {
//		return m_sampleSize;
//	}
//
//	xdl_int XdevLAudioALRecord::read(xdl_uint8* buffer) {
//		return ERR_OK;
//	}

}
