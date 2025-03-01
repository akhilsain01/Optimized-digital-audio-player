/**
 * \file CSimpleAudioOutStream.h
 * \brief interface of CSimpleAudioOutStream
 *
 *  Created on: Dec 3rd, 2024
 *      Author: Akhil Sai Nallapati
 */

#ifndef CSIMPLEAUDIOPLAYER_H_
#define CSIMPLEAUDIOPLAYER_H_

#include <PortAudio.h>
#include <SKSLib.h>
#include <stdint.h>

/**
 * \brief handles playback of audio data in buffer on the default audio output
 * device of the system
 *
 * uses library PortAudio (https://www.portaudio.com/)
 *
 * multiple instances of this class can be created simultaneously
 */
class CSimpleAudioOutStream {
	// todo define your class' interface here
public:
	enum STATES{
		S_NOTREADY,
		S_READY,
		S_PLAYING
	};
private:
	 PaStream* m_stream;
	 PaError err;
	 STATES m_state;
public:
	CSimpleAudioOutStream();
	~CSimpleAudioOutStream();

	void open(uint16_t nChannels,uint32_t Samplerate, int framesPerBlock);
	void close();
	void start();
	void stop();
	void resume();
	void play(float* sbuf,int framesPerBlock);
	void pause();


};

#endif /* CSIMPLEAUDIOPLAYER_H_ */
