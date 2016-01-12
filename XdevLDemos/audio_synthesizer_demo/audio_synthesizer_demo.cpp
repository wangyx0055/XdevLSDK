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

#include <XdevL.h>
#include <XdevLAudio/XdevLAudio.h>

#include <signal.h>
#include <cmath>

xdl::IPXdevLCore core = nullptr;
xdl::XdevLAudioCapture* alsa_capture = nullptr;
xdl::XdevLAudioPlayback* alsa_playback = nullptr;

xdl::XdevLTimer timer;

xdl::xdl_float samplingRate = 44100.0f;
xdl::xdl_uint8* sinusTempBuffer = nullptr;
xdl::xdl_float volume = 1000.0f;
xdl::xdl_float Hz = 440.0f;
xdl::xdl_float phase = 0.0f;


xdl::xdl_float playSine() {
	phase += (Hz * 2.0 * M_PI)/samplingRate;
	if(phase > 2 * M_PI) {
		phase = phase - ( 2.0 * M_PI);
	}
	return sin( phase) ;
}

xdl::xdl_float playSawtooth() {
	phase += (Hz * 2.0 * M_PI) / samplingRate;
	if(phase > 2 * M_PI) {
		phase = phase - ( 2.0 * M_PI);
	}
	return (1.0 - (1.0/M_PI) * phase);
}

xdl::xdl_float playTriangle() {
	phase += (Hz * 2.0 * M_PI)/ samplingRate;
	xdl::xdl_float y = 0.0;
	if(phase < M_PI) {
		y = -1.0 + (2.0/M_PI)*phase;
	} else {
		y = 3.0 - (2.0/M_PI)*phase;
	}
	if(phase > 2.0 * M_PI) {
		phase = phase - ( 2.0 * M_PI);
	}
	return y;
}

xdl::xdl_float playSquare() {
	phase += (Hz * 2.0 * M_PI)/ samplingRate;
	xdl::xdl_float y = 0.0;
	if(phase < M_PI) {
		y = 1.0;
	} else {
		y = -1.0;
	}
	if(phase > 2.0 * M_PI) {
		phase = phase - ( 2.0 * M_PI);
	}
	return y;
}

xdl::xdl_float playNoise() {
	return (1.0f/(xdl::xdl_float)RAND_MAX) * (xdl::xdl_float)rand() ;
}

void exitHandle(int signal) {

	if(nullptr != sinusTempBuffer) {
		delete [] sinusTempBuffer;
	}

	// Destroy the Core.
	xdl::destroyCore(core);

	exit(0);
}


xdl::xdl_uint callback(xdl::xdl_uint8* buffer, xdl::xdl_uint samples, void* userData) {
	xdl::xdl_int16* sampleBuffer = (xdl::xdl_int16*)buffer;
	for(xdl::xdl_uint sample = 0; sample != samples; sample++) {
		sampleBuffer[sample] = playSquare() * volume;
	}
	return samples;
}

int main (int argc, char *argv[]) {
	// Let's register the CTRL + c signal handler.
	if(signal(SIGINT, exitHandle) == SIG_ERR) {
		std::cerr <<  "Failed to set SIGINT handler." << std::endl;
		return -1;
	}

	// Register termination handler.
	if(signal(SIGTERM, exitHandle) == SIG_ERR) {
		std::cerr <<  "Failed to set SIGTERM handler." << std::endl;
		return -1;
	}

	if(xdl::createCore(&core) != xdl::ERR_OK) {
		return -1;
	}

	if(core->plug(xdl::XdevLPluginName("XdevLAudioAlsa"), xdl::XdevLVersion(0, 1, 0)) != xdl::ERR_OK) {
		xdl::destroyCore(core);
		return -1;
	}

	alsa_playback = xdl::createModule<xdl::XdevLAudioPlayback*>(core, xdl::XdevLModuleName("XdevLAudioPlayback"), xdl::XdevLID("MyAlsaPlayback"));
	if(isModuleNotValid(alsa_playback)) {
		xdl::destroyCore(core);
		return -1;
	}

	xdl::IPXdevLAudioBuffer buffer2 = alsa_playback->createAudioBuffer(xdl::AUDIO_BUFFER_FORMAT_S16, xdl::AUDIO_SAMPLE_RATE_44100, 1, 0, nullptr);
	if(nullptr == buffer2) {
		xdl::destroyCore(core);
		return -1;
	}
	
	alsa_playback->setCallbackFunction(callback, nullptr);

	for(;;) {
		core->update();
		alsa_playback->update2();
	}

	xdl::destroyCore(core);
}
