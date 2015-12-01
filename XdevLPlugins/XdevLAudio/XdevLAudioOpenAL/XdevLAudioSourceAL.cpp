
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
#include <vector>

#include <XdevLAudio/XdevLAudio.h>
#include "XdevLAudioBufferAL.h"
#include "XdevLAudioSourceAL.h"

namespace xdl {

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

	const xdl_uint& XdevLAudioSourceAL::getID() const {
		return m_id;
	}

}
