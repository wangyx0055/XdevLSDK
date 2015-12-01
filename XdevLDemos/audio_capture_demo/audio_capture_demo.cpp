#include <XdevL.h>
#include <XdevLAudio/XdevLAudio.h>

#include <signal.h>

xdl::IPXdevLCore core = nullptr;
xdl::XdevLAudioCapture* alsa_capture = nullptr;
xdl::XdevLAudioPlayback* alsa_playback = nullptr;

xdl::XdevLTimer timer;

void exitHandle(int signal) {

	// Destroy the Core.
	xdl::destroyCore(core);

	exit(0);
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

	alsa_capture = xdl::createModule<xdl::XdevLAudioCapture*>(core, xdl::XdevLModuleName("XdevLAudioCapture"), xdl::XdevLID("MyAlsaRecord"));
	if(isModuleNotValid(alsa_capture)) {
		xdl::destroyCore(core);
		return -1;
	}

	alsa_playback = xdl::createModule<xdl::XdevLAudioPlayback*>(core, xdl::XdevLModuleName("XdevLAudioPlayback"), xdl::XdevLID("MyAlsaPlayback"));
	if(isModuleNotValid(alsa_playback)) {
		xdl::destroyCore(core);
		return -1;
	}

	xdl::XdevLAudioBuffer* buffer = nullptr;
	if(alsa_capture->createAudioBuffer(xdl::AUDIO_BUFFER_FORMAT_S16, xdl::AUDIO_SAMPLE_RATE_44100, 1, 0, nullptr,  &buffer) != xdl::ERR_OK) {
		xdl::destroyCore(core);
		return -1;
	}

	xdl::XdevLAudioBuffer* buffer2 = nullptr;
	if(alsa_playback->createAudioBuffer(xdl::AUDIO_BUFFER_FORMAT_S16, xdl::AUDIO_SAMPLE_RATE_44100, 1, 0, nullptr, &buffer2) != xdl::ERR_OK) {
		xdl::destroyCore(core);
		return -1;
	}

	std::cout << "Started recording. Please say something into your microphone for 5 seconds" << std::endl;

	std::ofstream file("audio.raw", std::ios::binary);

	xdl::xdl_uint8* tmp = new xdl::xdl_uint8[alsa_capture->getBufferSize()];

	xdl::xdl_int loops = 5000000/alsa_capture->getPeriodTime();

	for(xdl::xdl_int loop = 0; loop < loops; loop++) {
		alsa_capture->read(tmp);
		file.write((char*)tmp, alsa_capture->getBufferSize());
	}

	file.close();


	// Playback raw file.
	//
	std::cout << "Finished recording. Playing back raw audio file." << std::endl;

	std::ifstream file_read("audio.raw", std::ios::binary);
	file_read.seekg (0, std::ios::beg);

	xdl::xdl_uint8* tmp2 = new xdl::xdl_uint8[alsa_playback->getBufferSize()];

	for(;;) {
		file_read.read((char*)tmp2, alsa_capture->getBufferSize());
		alsa_playback->write(tmp2);
	}
	
	file_read.close();

	xdl::destroyCore(core);
}
