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
#include <XdevLWindow/XdevLWindow.h>
#include <XdevLInput/XdevLKeyboard/XdevLKeyboard.h>
#include <XdevLAudio/XdevLAudio.h>
#include <iostream>
#include <iomanip>
#include <cmath>

xdl::xdl_bool play_pause = true;
xdl::xdl_bool play_pause2 = false;
xdl::xdl_float sound_source_gain = 0.5;

// Flag to run/stop the application.
xdl::xdl_bool run = xdl::xdl_true;

//
// This callback function we use to exit from the application.
//
void eventCallbackFunction(xdl::XdevLEvent& event) {
	switch(event.type) {
		case xdl::XDEVL_CORE_EVENT: {
			if(event.core.event == xdl::XDEVL_CORE_SHUTDOWN)
				run = xdl::xdl_false;
		}
		break;
		case xdl::XDEVL_KEY_PRESSED: {

		} break;
	}
}

int main(int argc, char* argv[]) {

	xdl::IPXdevLCore core = nullptr;

	// Create the core system.
	if(xdl::createCore(&core, argc, argv, xdl::XdevLFileName("audio_demo.xml")) != xdl::ERR_OK) {
		return xdl::ERR_ERROR;
	}

	// Register a listener callback function to get events. In this case to stop the application.
	core->registerListenerCallbackFunction(eventCallbackFunction);

	// Create a window because we can use keyboard/mouse only if we have one (hopefully will be fixed in later versions of XdevL).
	xdl::IPXdevLWindow window = xdl::getModule<xdl::IPXdevLWindow> (core,  xdl::XdevLID("MyWindow"));
	if(nullptr == window) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	if(window->create() == xdl::ERR_ERROR) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Create the keyboard module to get access to the keyboard device.
	xdl::IPXdevLKeyboard keyboard = xdl::getModule<xdl::IPXdevLKeyboard> (core,  xdl::XdevLID("MyKeyboard"));
	if(nullptr == keyboard) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Attach this keyboard to the window.
	if(keyboard->attach(window) != xdl::ERR_OK) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Get the module which we have created already to get access to the audio device.
	// We requested the creation of the module within the 'audio_demo.xml' file.
	xdl::IPXdevLAudio audio = xdl::getModule<xdl::IPXdevLAudio> (core,  xdl::XdevLID("MyAudio"));
	if(nullptr == audio) {
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}


	// To store sounds we need a buffer so let's create one.
	xdl::IPXdevLAudioBuffer abuffer = audio->createAudioBufferFromFile(xdl::XdevLFileName("media/loop_a.wav"));
	if(abuffer == nullptr) {
		std::cerr << "Could not create audio buffer.\n";
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// Ok, now we need a source in the virtual environment that emits the sounds.
	// Usualy in the real world everything is a sound emitter. Every object can
	// create sound and the class XdevLAudioSource is excatly for that. We can
	// give a location and speed to a source.
	xdl::IPXdevLAudioSource sound_source_1 = audio->createAudioSource(abuffer);
	if(sound_source_1 == nullptr) {
		std::cerr << "Could not create audio source for audio buffer.\n";
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	// And here another sound with it's sound source.
	xdl::IPXdevLAudioBuffer bbuffer = audio->createAudioBufferFromFile(xdl::XdevLFileName("media/loop_b.wav"));
	if(bbuffer == nullptr) {
		std::cerr << "Could not create audio buffer.\n";
		xdl::destroyCore(core);
		return xdl::ERR_ERROR;
	}

	xdl::IPXdevLAudioSource sound_source_2 = audio->createAudioSource(bbuffer);
	if(sound_source_2 == nullptr) {
		std::cerr << "Could not create audio source for audio buffer.\n";
		return xdl::ERR_ERROR;
	}

	// Set the loop state.
	sound_source_1->setLoop(true);
	sound_source_2->setLoop(true);

	// Get keyboard buttons.
	xdl::IPXdevLButton escape = nullptr;
	keyboard->getButton(xdl::KEY_ESCAPE, &escape);

	xdl::IPXdevLButton F1 = nullptr;;
	keyboard->getButton(xdl::KEY_F1, &F1);


	xdl::IPXdevLButton F2 = nullptr;;
	keyboard->getButton(xdl::KEY_F2, &F2);

	xdl::IPXdevLButton increase_volume = nullptr;;
	keyboard->getButton(xdl::KEY_I, &increase_volume);

	xdl::IPXdevLButton decrease_volume = nullptr;;
	keyboard->getButton(xdl::KEY_D, &decrease_volume);

	std::cout << "Press F1: play/pause first sound\n";
	std::cout << "Press F2: play/pause second sound\n";
	std::cout << "Press I/D increase/decrease the main volume.\n";
	std::cout << "Sounds by (c) 2009 Cengiz Terzibas.\n\n";

	audio->setGain(0.5f);

//	sound_source_1->setAudioBuffer(abuffer);
//	sound_source_2->setAudioBuffer(bbuffer);
	sound_source_1->setLoop(true);

	// Start playing sound.
	sound_source_1->play();

	window->show();

	// Start the main loop.
	while(run) {

		core->update();

		if(escape->getClicked())
			break;

		if(F1->getClicked()) {
			play_pause = !play_pause;

			if(play_pause)
				sound_source_1->play();
			else
				sound_source_1->pause();

		}
		if(increase_volume->getClicked()) {
			sound_source_gain += 0.1f;
			audio->setGain(sound_source_gain);
		}

		if(decrease_volume->getClicked()) {
			sound_source_gain -= 0.1f;
			audio->setGain(sound_source_gain);
		}

		if(F2->getClicked()) {
			play_pause2 = !play_pause2;

			if(play_pause2)
				sound_source_2->play();
			else
				sound_source_2->pause();

		}

		std::cout << std::setprecision(3);

		xdl::xdl_int play_time = static_cast<xdl::xdl_int>(sound_source_1->getPlayPosSec() *1000);
		std::cout << "Loop Time1 " << play_time/60000 << ":" << play_time/1000%60 <<":" << play_time/10%100;
		xdl::xdl_int play_time2 = static_cast<xdl::xdl_int>(sound_source_2->getPlayPosSec() *1000);
		std::cout << std::setw(5) << "  Loop Time2 " << play_time2/60000 << ":" << play_time2/1000%60 <<":" << play_time2/10%100;
		std::cout << "\r";

	}

	xdl::destroyCore(core);

	return xdl::ERR_OK;
}
