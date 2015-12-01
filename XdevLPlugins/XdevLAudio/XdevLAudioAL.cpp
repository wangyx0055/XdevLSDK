
#include <iostream>
#include <string>
#include <XdevLCoreMediator.h>
#include "XdevLAudioAL.h"
#include <XdevLPlatform.h>

#include <fstream>

xdl::XdevLModuleDescriptor audioALModuleDesc {xdl::vendor,
                                              xdl::author,
                                              xdl::moduleNames[0],
                                              xdl::copyright,
                                              xdl::description,
                                              xdl::XdevLAudioALMajorVersion,
                                              xdl::XdevLAudioALMinorVersion,
                                              xdl::XdevLAudioALPatchVersion };

xdl::XdevLModuleDescriptor xdl::XdevLAudioALRecord::m_moduleRecordDescriptor {vendor,
                                                                              author,
                                                                              moduleNames[1],
                                                                              copyright,
                                                                              description2,
                                                                              XdevLAudioALRecordMajorVersion,
                                                                              XdevLAudioALRecordMinorVersion,
                                                                              XdevLAudioALRecordPatchVersion };

xdl::XdevLPluginDescriptor pluginDescriptor {	xdl::pluginName,
                                              xdl::moduleNames,
                                              xdl::XdevLAudioPluginMajorVersion,
                                              xdl::XdevLAudioPluginMinorVersion,
                                              xdl::XdevLAudioPluginPatchVersion };



extern "C" XDEVL_EXPORT xdl::xdl_int _create(xdl::XdevLModuleCreateParameter* parameter) {
	if(audioALModuleDesc.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLAudioAL(parameter);
		if(!obj)
			return xdl::ERR_ERROR;
		parameter->setModuleInstance(obj);
		return xdl::ERR_OK;
	}
	if(xdl::XdevLAudioALRecord::m_moduleRecordDescriptor.getName() == parameter->getModuleName()) {
		xdl::XdevLModule* obj  = new xdl::XdevLAudioALRecord(parameter);
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

	XdevLAudioSourceAL::XdevLAudioSourceAL() :
		m_id(0),
		m_audioBuffer(nullptr) {

		alGenSources(1, &m_id);

		setLoop(xdl_false);
	}

	XdevLAudioSourceAL::~XdevLAudioSourceAL() {
		alDeleteSources(1, &m_id);
	}

	void XdevLAudioSourceAL::setAudioBuffer(XdevLAudioBuffer* buffer) {

		// If we have a resource, delete it.
//    if(m_id != 0) {
//      alDeleteSources(1, &m_id);
//      alGenSources(1, &m_id);
//    }
		m_audioBuffer = buffer;
		alSourcei(m_id, AL_BUFFER, m_audioBuffer->getID());
		//alSourceQueueBuffers(m_id,1, &m_audioBuffer->getID());
//		setPitch(1.0f);
//		setGain(1.0f);
//		setLoop(true);
//		setPos(0.0f, 0.0f, 0.0f);
//		setVel(0.0f, 0.0f, 0.0f);
	}

	void XdevLAudioSourceAL::setPos(xdl_float x, xdl_float y, xdl_float z) {
		ALfloat pos[3] = {x,y,z};
		alSourcefv(m_id, AL_POSITION, pos);
	}

	void XdevLAudioSourceAL::setVel(xdl_float vx, xdl_float vy, xdl_float vz) {
		ALfloat vel[3] = {vx,vy,vz};
		alSourcefv(m_id, AL_VELOCITY, vel);
	}

	void XdevLAudioSourceAL::setOri(xdl_float x, xdl_float y, xdl_float z) {
		ALfloat ori[3] = {x,y,z};
		alSourcefv(m_id, AL_ORIENTATION, ori);
	}

	void XdevLAudioSourceAL::setPitch(xdl_float pitch) {
		alSourcef(m_id, AL_PITCH, (ALfloat)pitch);
	}

	void XdevLAudioSourceAL::setGain(xdl_float gain) {
		alSourcef(m_id, AL_GAIN, (ALfloat)gain);
	}

	void XdevLAudioSourceAL::setPlayPosSec(xdl_float pos) {
		alSourcef(m_id, AL_SEC_OFFSET, pos);
	}

	void XdevLAudioSourceAL::setPlayPosSample(xdl_int pos) {
		alSourcei(m_id, AL_SAMPLE_OFFSET, pos);
	}

	xdl_float XdevLAudioSourceAL::getPlayPosSec() {
		static xdl_float sec = 0.0f;
		alGetSourcef(m_id, AL_SEC_OFFSET, &sec);
		return sec;
	}

	xdl_int XdevLAudioSourceAL::getPlayPosSample() {
		static xdl_int sample = 0;
		alGetSourcei(m_id, AL_SAMPLE_OFFSET, &sample);
		return sample;
	}

	void XdevLAudioSourceAL::setLoop(xdl_bool state) {
		ALint loop = AL_FALSE;
		if(state)
			loop = AL_TRUE;
		alSourcei(m_id, AL_LOOPING, loop);
	}

	xdl_bool XdevLAudioSourceAL::isPlaying() {
		ALint state;
		if(alIsSource(m_id) == AL_FALSE) {
			return AL_FALSE;
		}
		alGetSourceiv(m_id, AL_SOURCE_STATE, &state);
		return ((state == AL_PLAYING)
		        || (state == AL_PAUSED)) ? true : false;
	}

	void XdevLAudioSourceAL::play() {
		alSourcePlay(m_id);
	}

	void XdevLAudioSourceAL::stop() {
		alSourceStop(m_id);
	}

	void XdevLAudioSourceAL::pause() {
		alSourcePause(m_id);
	}



	XdevLAudioAL::XdevLAudioAL(XdevLModuleCreateParameter* parameter) :
		XdevLModuleImpl<XdevLAudio>(parameter, audioALModuleDesc),
		m_device(nullptr),
		m_context(nullptr),
		m_previous(nullptr) {
		m_audioSources.reserve(10);
		m_audioSources.resize(10);
		m_audioBuffers.reserve(10);
		m_audioBuffers.resize(10);
	}

	XdevLAudioAL::~XdevLAudioAL() {
	}

	xdl_int XdevLAudioAL::makeCurrent() {
		m_previous = alcGetCurrentContext();
		alcMakeContextCurrent(m_context);
		return ERR_OK;
	}

	xdl_int XdevLAudioAL::releaseCurrent() {
		alcMakeContextCurrent(m_previous);
		return ERR_OK;
	}
	xdl_int XdevLAudioAL::init() {
		m_device = alcOpenDevice(NULL);
		if(m_device == NULL) {
			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
			return ERR_ERROR;
		}
		m_context = alcCreateContext(m_device, NULL);
		if(m_context == NULL) {
			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
			return ERR_ERROR;
		}

		alcMakeContextCurrent(m_context);


		setPos(0.0f, 0.0f, 0.0f);
		setOri(0.0f, 0.0f, 0.0f);
		setVel(0.0f, 0.0f, 0.0f);
		
		alDistanceModel(AL_NONE);


		return ERR_OK;
	}

	xdl_int XdevLAudioAL::shutdown() {
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

	xdl_int XdevLAudioAL::createAudioBufferFromFile(const XdevLFileName& filename, XdevLAudioBuffer** buffer) {
		ALuint id = 0;

		XdevLAudioBufferFormat audioFormat = AUDIO_BUFFER_UNKNOWN;
		xdl_int size									= 0;
		xdl_int samplingRate 					= 0;

		std::vector<char> rawData;
		std::ifstream ifile(filename, std::ifstream::binary);

		RIFF_Header riff_header {0};
		ifile.read((char*)&riff_header, sizeof(RIFF_Header));
		if(isChunckID(riff_header, 'R', 'I', 'F', 'F')) {

			RIFF_Type riff_type {0};
			ifile.read((char*)&riff_type, sizeof(RIFF_Type));

			if(isRIFFID(riff_type, 'W', 'A', 'V', 'E')) {

				do {
					
					// Read chunck header.
					RIFF_Header	wave_format_header {0};
					ifile.read((char*)&wave_format_header, sizeof(RIFF_Header));
					
					//
					// Check which chunck type it is.
					//
					if(isChunckID(wave_format_header, 'f', 'm', 't', ' ')) {

						WAVE_Format	wave_format {0};
						ifile.read((char*)&wave_format, sizeof(WAVE_Format));
						size = wave_format_header.chunkSize;
						samplingRate = wave_format.sampleRate;

						if(wave_format.numChannels == 1) {
							if(wave_format.bitsPerSample == 8)
								audioFormat = AUDIO_BUFFER_MONO8;
							else if(wave_format.bitsPerSample == 16)
								audioFormat = AUDIO_BUFFER_MONO16;
						} else if(wave_format.numChannels == 2) {
							if(wave_format.bitsPerSample == 8)
								audioFormat = AUDIO_BUFFER_STEREO8;
							else if(wave_format.bitsPerSample == 16)
								audioFormat = AUDIO_BUFFER_STEREO16;
						}

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
					if(ifile.tellg() == -1) break;
					
					
				}while(true);
				
			}

		}

		ifile.close();

		//create our openAL buffer and check for success
		alGenBuffers(1, &id);


		ALuint format = 0;
		switch(audioFormat) {
			case AUDIO_BUFFER_MONO8 		:
				format = AL_FORMAT_MONO8;
				break;
			case AUDIO_BUFFER_MONO16		:
				format = AL_FORMAT_MONO16;
				break;
			case AUDIO_BUFFER_STEREO8 	:
				format = AL_FORMAT_STEREO8;
				break;
			case AUDIO_BUFFER_STEREO16 	:
				format = AL_FORMAT_STEREO16;
				break;
			case AUDIO_BUFFER_UNKNOWN:
			default:
				break;
		}

		alBufferData(id, format, (void*)rawData.data(), rawData.size(), samplingRate);

		XDEVL_MODULE_INFO("AudioFile: " << filename << ", ID: " << id << " successfully created. " << std::endl);
		XdevLAudioBufferAL* buf = new XdevLAudioBufferAL(id);

		*buffer = buf;

		m_audioBuffers.push_back(buf);

		return ERR_OK;
	}

	xdl_int XdevLAudioAL::createAudioBuffer(xdl_int format, xdl_int freq, xdl_int size, void* data, XdevLAudioBuffer** buffer) {
		ALenum error;
		ALuint albuf;
		alGenBuffers(1, &albuf);
		error = alGetError();
		if(error != AL_NO_ERROR) {
			XDEVL_MODULE_ERROR(getALErrorString(error) << std::endl);
			return ERR_ERROR;
		}

		ALenum fmat = AL_FORMAT_MONO8;
		switch(format) {
			case AUDIO_BUFFER_MONO8:
				break;
			case AUDIO_BUFFER_MONO16:
				fmat = AL_FORMAT_MONO16;
				break;
			case AUDIO_BUFFER_STEREO8:
				fmat = AL_FORMAT_STEREO8;
				break;
			case AUDIO_BUFFER_STEREO16:
				fmat = AL_FORMAT_STEREO16;
				break;
			default:
				break;
		}
		alBufferData(albuf, fmat, (ALvoid*)data, size, freq);
		error = alGetError();
		if(error != AL_NO_ERROR) {
			XDEVL_MODULE_ERROR(getALErrorString(error) << std::endl);
			return ERR_ERROR;
		}

		XdevLAudioBufferAL* buf = new XdevLAudioBufferAL(albuf);
		*buffer = buf;

		return ERR_OK;
	}

	xdl_int XdevLAudioAL::createAudioSource(XdevLAudioSource** src, XdevLAudioBuffer* buffer) {

		XdevLAudioSourceAL* audioSource = new XdevLAudioSourceAL();
		if(!audioSource) {
			XDEVL_MODULE_ERROR("Could not allocate\n");
			return ERR_ERROR;
		}

		alSourcei(audioSource->getID(), AL_BUFFER,  buffer->getID());
		//alSourceQueueBuffers(audioSource->getID(),1, &asource->buffer->getID());
//		audioSource->setPitch(1.0f);
//		audioSource->setGain(1.0f);
//		audioSource->setLoop(true);
//		audioSource->setPos(0.0f, 0.0f, 0.0f);
//		audioSource->setVel(0.0f, 0.0f, 0.0f);
		*src = audioSource;
		m_audioSources.push_back(audioSource);
		return ERR_OK;
	}

	void XdevLAudioAL::setPos(xdl_float x, xdl_float y, xdl_float z) {
		xdl_float pos[3] = {x,y,z};
		xdl_int ret = AL_NO_ERROR;
		alListenerfv(AL_POSITION, pos);
		if(alGetError() != AL_NO_ERROR) {
			XDEVL_MODULE_WARNING(getALErrorString(ret) << "\n");
		}
	}

	void XdevLAudioAL::setVel(xdl_float vx, xdl_float vy, xdl_float vz) {
		xdl_float vel[3] = {vx,vy,vz};
		xdl_int ret = AL_NO_ERROR;
		alListenerfv(AL_VELOCITY, vel);
		if(alGetError() != AL_NO_ERROR) {
			XDEVL_MODULE_WARNING(getALErrorString(ret) << "\n");
		}
	}

	void XdevLAudioAL::setOri(xdl_float x, xdl_float y, xdl_float z) {
		xdl_float ori[3] = {x,y,z};
		xdl_int ret = AL_NO_ERROR;
		alListenerfv(AL_ORIENTATION, ori);
		if(alGetError() != AL_NO_ERROR) {
			XDEVL_MODULE_WARNING(getALErrorString(ret) << "\n");
		}
	}

	void XdevLAudioAL::setGain(xdl_float gain) {
		alListenerf(AL_GAIN, (ALfloat)gain);
		xdl_int ret = AL_NO_ERROR;
		if((ret = alGetError()) != AL_NO_ERROR) {
			XDEVL_MODULE_WARNING(getALErrorString(ret) << "\n");
		}
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

	XdevLAudioALRecord::XdevLAudioALRecord(XdevLModuleCreateParameter* parameter) : XdevLModuleImpl<XdevLAudioRecord>(parameter, m_moduleRecordDescriptor),
		m_recordDevice(NULL) {};

	xdl_int XdevLAudioALRecord::init() {

		if(alcIsExtensionPresent(NULL, "AL_EXT_capture") == AL_TRUE) {
			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
			return ERR_ERROR;
		}

		std::cout << alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER) << std::endl;

		return ERR_OK;
	}


	xdl_int XdevLAudioALRecord::shutdown() {
		if(alcCaptureCloseDevice(m_recordDevice) == AL_FALSE) {
			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
			return ERR_ERROR;
		}
		return ERR_OK;
	}


	xdl_int XdevLAudioALRecord::device(const XdevLString& deviceName, xdl_int samplerate, xdl_int format, xdl_int samples) {

		m_audioFormat = AL_FORMAT_MONO8;
		switch(format) {
			case AUDIO_BUFFER_MONO8:
				m_bufferSize = 1;
				break;
			case AUDIO_BUFFER_MONO16:
				m_audioFormat = AL_FORMAT_MONO16;
				m_bufferSize = 2;
				break;
			case AUDIO_BUFFER_STEREO8:
				m_audioFormat = AL_FORMAT_STEREO8;
				m_bufferSize = 2;
				break;
			case AUDIO_BUFFER_STEREO16:
				m_audioFormat = AL_FORMAT_STEREO16;
				m_bufferSize = 4;
				break;
			default:
				break;
		}

		m_sampleSize = m_bufferSize;

		m_sampleRate = AUDIO_SAMPLE_RATE_11025;
		switch(samplerate) {
			case AUDIO_SAMPLE_RATE_8000:
				m_sampleRate = AUDIO_SAMPLE_RATE_8000;
				break;
			case AUDIO_SAMPLE_RATE_11025:
				m_sampleRate = AUDIO_SAMPLE_RATE_11025;
				break;
			case AUDIO_SAMPLE_RATE_22050:
				m_sampleRate = AUDIO_SAMPLE_RATE_22050;
				break;
			case AUDIO_SAMPLE_RATE_44100:
				m_sampleRate = AUDIO_SAMPLE_RATE_44100;
				break;
			case AUDIO_SAMPLE_RATE_48000:
				m_sampleRate = AUDIO_SAMPLE_RATE_48000;
				break;
			case AUDIO_SAMPLE_RATE_96000:
				m_sampleRate = AUDIO_SAMPLE_RATE_96000;
				break;
			default:
				break;
		}

		m_bufferSize *=  samples;

		m_recordDevice = alcCaptureOpenDevice(deviceName.toString().c_str(), m_sampleRate, m_audioFormat, m_bufferSize);
		if(m_recordDevice == NULL) {
			XDEVL_MODULE_ERROR(getALCErrorString(alGetError()) << std::endl);
			return ERR_ERROR;
		}

		return ERR_OK;
	}

	void XdevLAudioALRecord::start() {
		alcCaptureStart(m_recordDevice);
	}

	void XdevLAudioALRecord::stop() {
		alcCaptureStop(m_recordDevice);
	}

	xdl_int XdevLAudioALRecord::capture(xdl_int8* buffer, xdl_int samples) {
		ALint tmp = 0;
		while(tmp < samples) {
			alcGetIntegerv(m_recordDevice, ALC_CAPTURE_SAMPLES, (ALCsizei)sizeof(ALbyte), &tmp);
		}
		alcCaptureSamples(m_recordDevice, (ALCvoid*)buffer, samples);
		return ERR_OK;
	}

	xdl_int XdevLAudioALRecord::sampleSize() {
		return m_sampleSize;
	}

}
