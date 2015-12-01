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
#include "XdevLAudio.h"
#include <vector>

namespace xdl {

	class XdevLAudio;
	class deviceSource;

	// Holds the major version number of the plugin.
	const xdl_uint XdevLAudioPluginMajorVersion = XDEVLAUDIO_MAJOR_VERSION;

	// Holds the Minor version number of the plugin.
	const xdl_uint XdevLAudioPluginMinorVersion = XDEVLAUDIO_MINOR_VERSION;

	// Holds the Patch version number of the plugin.
	const xdl_uint XdevLAudioPluginPatchVersion = XDEVLAUDIO_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLAudioALMajorVersion = XDEVLAUDIO_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLAudioALMinorVersion = XDEVLAUDIO_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint	XdevLAudioALPatchVersion = XDEVLAUDIO_MODULE_PATCH_VERSION;



	// Holds the Major version number.
	const xdl_uint XdevLAudioALRecordMajorVersion = XDEVLAUDIORECORD_MODULE_MAJOR_VERSION;

	// Holds the Minor version number.
	const xdl_uint XdevLAudioALRecordMinorVersion = XDEVLAUDIORECORD_MODULE_MINOR_VERSION;

	// Holds the Patch version number.
	const xdl_uint	XdevLAudioALRecordPatchVersion = XDEVLAUDIORECORD_MODULE_PATCH_VERSION;



	static const XdevLString vendor {
		"www.codeposer.net"
	};
	static const XdevLString author {
		"Cengiz Terzibas"
	};
	static const XdevLString copyright {
		"(c) 2005 - 2012 Cengiz Terzibas."
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
		XdevLModuleName("XdevLAudio"),
		XdevLModuleName("XdevLAudioRecord")
	};

	/**
		@class XdevLAudioBufferAL
		@brief Holds the raw audio stream data.
		@author Cengiz Terzibas
	*/
	class XdevLAudioBufferAL : public XdevLAudioBuffer {
		public:

			XdevLAudioBufferAL(ALuint id) : m_id(id) {};

			virtual ~XdevLAudioBufferAL() {
				alDeleteBuffers(1, &m_id);
			};

			/// Returns the size of the audio buffer in bytes.
			/**
				@return Size of the buffer in bytes.
			*/
			virtual xdl_int getSize() {
				xdl_int size = 0;
				alGetBufferi(m_id, AL_SIZE, &size);
				return size;
			}

			/// Returns the frequence of the audio data.
			/**
				@return The base frequence of the recorded audio data.
			*/
			virtual xdl_int getSamplingRate() {
				xdl_int freq = 0;
				alGetBufferi(m_id, AL_FREQUENCY, &freq);
				return freq;
			}

			/// Returns the number of channels.
			/**
				@return The number of channels used in this buffer.
			*/
			virtual xdl_int getChannels() {
				xdl_int chan = 0;
				alGetBufferi(m_id, AL_CHANNELS, &chan);
				return chan;
			}

			/// Returns the resolution of one audio data element in bits.
			/**
				@return The resolution of one element in the audio buffer in bits.
			*/
			virtual xdl_int getBits() {
				xdl_int bits = 0;
				alGetBufferi(m_id, AL_BITS, &bits);
				return bits;
			}
			const xdl_uint& getID() const {
				return m_id;
			}

			virtual xdl_uint8* getData() {
				return nullptr;
			}
		private:
			/// Holds the buffer identification.
			ALuint m_id;
	};

	/**
		@class XdevLAudioSourceAL
		@brief Source for audio data.
		@author Cengiz Terzibas

		An audio source means a source that creates sound. In real life this could be everything.
		For instance a telephone, car, plane, steps of a human etc. A source can have
		a direction, position and velocity.
	*/
	class XdevLAudioSourceAL : public XdevLAudioSource {
			friend class XdevLAudioAL;
		public:
			XdevLAudioSourceAL();
			virtual ~XdevLAudioSourceAL();
			virtual void setAudioBuffer(XdevLAudioBuffer* buffer);
			/// Sets the position of the source.
			/**
				@param x X coordinate of the source in global space.
				@param y Y coordinate of the source in global space.
				@param z Z coordinate of the source in global space.
			*/
			virtual void setPos(xdl_float x, xdl_float y, xdl_float z);
			/// Sets the velocity of the source.
			/**
				@param vx Velocity component in x direction in global space.
				@param vy Velocity component in y direction in global space.
				@param vz Velocity component in z direction in global space.
			*/
			virtual void setVel(xdl_float vx, xdl_float vy, xdl_float vz);
			/// Sets the orientation of the source.
			/**
				@param x X coordinate of the direction in global space.
				@param y Y coordinate of the direction in global space.
				@param z Z coordinate of the direction in global space.
			*/
			virtual  void setOri(xdl_float x, xdl_float y, xdl_float z);
			/// Sets the pitch of the source.
			/**
				@param Changes the frequence of the source.
			*/
			virtual void setPitch(xdl_float pitch);
			/// Sets the gain of the source.
			/**
				@param gain Controlls the gain (volume) of the source.
			*/
			virtual void setGain(xdl_float gain);
			/// Sets the play position
			/**
				@param pos Sets the play position. This value
				uses second units.
			*/
			virtual void setPlayPosSec(xdl_float pos);
			/// Sets the play position
			/**
				@param pos Sets the play position. This value
				uses sample units.
			*/
			virtual void setPlayPosSample(xdl_int pos);
			/// Returns the play position
			/**
				@return Returns the current play position in second units.
			*/
			virtual xdl_float getPlayPosSec();
			/// Returns the play position
			/**
				@return Returns the current play position in sample units.
			*/
			virtual xdl_int getPlayPosSample();
			/// Sets the loop state of the source
			/**
				@param state Set state = false if the source shouldn't
				be looped. Default value for the loop state of the
				source is true.
			*/
			virtual void setLoop(xdl_bool state);
			/// Returns the playing state of the source
			/**
				@return Returns true if the source is playing
				at the moment. Otherwise it returns false.
			*/
			virtual xdl_bool isPlaying();
			/// Starts to play a source.
			virtual void play();
			/// Stops a playing source.
			virtual void stop();
			/// Pauses a playing source.
			virtual void pause();

			const xdl_uint& getID() const {
				return m_id;
			}

		private:

			/// Holds the source identificator.
			ALuint				m_id;

			/// Holds the assigned XdevLAudioBuffer.
			XdevLAudioBuffer*	m_audioBuffer;
	};

	/**
		@class XdevLAudioAL
		@brief Class to support mouse devices
		@author Cengiz Terzibas

	*/
	class XdevLAudioAL : public XdevLModuleImpl<XdevLAudio> {
		public:
			XdevLAudioAL(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLAudioAL();

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;

			/// Creates an audio buffer.
			/**
				@param filename Filename that points to a valid audio file.
				@param buffer A pointer to an XdevLAudioBuffer;
				@return If it was successful ERR_OK will be returned and buffer will have
				a valid pointer to an XdevLAudioBuffer object. Otherwise ERR_ERROR will be
				returned and buffer will be NULL.
			*/
			virtual xdl_int createAudioBufferFromFile(const XdevLFileName& filename, XdevLAudioBuffer** buffer);
			/// Creates an empty buffer.
			/**
				@param format Specifies the format of the audio buffer.
				@param freuency Specifies the base frequence of the audio buffer.
				@param size The size of the buffer. The size depends on the format.
				@param data A buffer that will be copied into the audio buffer.
				@param buffer A pointer to an XdevLAudioBuffer;
				@return If it was successful ERR_OK will be returned and buffer will have
				a valid pointer to an XdevLAudioBuffer object. Otherwise ERR_ERROR will be
				returned and buffer will be NULL.
			*/
			virtual xdl_int createAudioBuffer(xdl_int format, xdl_int frequency, xdl_int size, void* data, XdevLAudioBuffer** buffer);
			/// Creates an audio source.
			/**
				@param filename Filename that points to a valid audio file.
				@param buffer A pointer to an valid XdevLAudioBuffer;
				@return If it was successful ERR_OK will be returned and src will have
				a valid pointer to an XdevLAudioSource object. Otherwise ERR_ERROR will be
				returned and src will be NULL.
			*/
			virtual xdl_int createAudioSource(XdevLAudioSource** src, XdevLAudioBuffer* buffer);
			/// Sets the position of the listener.
			virtual void setPos(xdl_float x, xdl_float y, xdl_float z);
			/// Sets the velocity of the listener.
			virtual void setVel(xdl_float vx, xdl_float vy, xdl_float vz);
			/// Sets the orientation of the listener.
			virtual void setOri(xdl_float x, xdl_float y, xdl_float z);
			/// Sets the gain of the listener.
			virtual void setGain(xdl_float gain);
			virtual xdl_int makeCurrent();
			virtual xdl_int releaseCurrent();
		protected:
			/// Holds the ALC XdevLDevice object.
			ALCdevice* m_device;
			/// Holds the ALC context.
			ALCcontext* m_context;
			/// Stores previous context.
			ALCcontext* m_previous;
			/// Holds audio sources.
			std::vector<XdevLAudioSourceAL*> m_audioSources;
			/// Holds audio sources
			std::vector<XdevLAudioBufferAL*> m_audioBuffers;
	};


	/**
		@class XdevLAudioALRecord
		@class Extended version of the XdevLAudioAL version which supports recording.
		@author Cengiz Terzibas
	*/
	class XdevLAudioALRecord : public XdevLModuleImpl<XdevLAudioRecord> {
		public:
			XdevLAudioALRecord(XdevLModuleCreateParameter* parameter);
			virtual ~XdevLAudioALRecord() {};

			static XdevLModuleDescriptor m_moduleRecordDescriptor;

			virtual xdl_int init() override;
			virtual xdl_int shutdown() override;


			virtual void start();
			virtual void stop();

			virtual xdl_int device(const XdevLString& deviceName, xdl_int samplerate, xdl_int format, xdl_int buffer_size);
			virtual xdl_int capture(xdl_int8* buffer, xdl_int samples);
			virtual xdl_int sampleSize();

		protected:
			ALCdevice* 	m_recordDevice;
			ALenum 			m_audioFormat;
			ALCuint 		m_sampleRate;
			ALCsizei		m_bufferSize;
			ALCsizei		m_sampleSize;
	};

/// Returns AL error codes as string.
	std::string getALErrorString(ALenum err);
/// Returns ALC error codes as string.
	std::string getALCErrorString(ALenum err);

}

#endif
