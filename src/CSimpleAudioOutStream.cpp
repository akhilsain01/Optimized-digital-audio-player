/**
 * \file CSimpleAudioOutStream.cpp
 * \brief implementation of CSimpleAudioOutStream
 *
 *  Created on: Dec 3rd, 2024
 *  Author: Akhil Sai Nallapati
 */
#include <stdlib.h>
#include <memory.h>
#include <string>
using namespace std;
#include "CSimpleAudioOutStream.h"
// todo implement your class here
int numOBJ=0;
CSimpleAudioOutStream::CSimpleAudioOutStream() {
	m_stream = NULL;
	err = paNotInitialized;
	m_state = S_NOTREADY;
	numOBJ++;
}
CSimpleAudioOutStream::~CSimpleAudioOutStream() {
	numOBJ--;
	close();
}

void CSimpleAudioOutStream::open(uint16_t nChannels, uint32_t sampleRate,
		int framesPerBlock) {
	if (m_state == S_READY)
		return;
	if (framesPerBlock < 0)
		throw CException(CException::SRC_SimpleAudioDevice, -1,
				"FramesPerBlock is negative");

	if (m_state == S_NOTREADY) {
		err = Pa_Initialize();

		if (err == paNoError) {

			PaStreamParameters outputParam;
			outputParam.device = Pa_GetDefaultOutputDevice();

			if (outputParam.device != paNoDevice) {

				outputParam.channelCount = nChannels;
				outputParam.sampleFormat = paFloat32;
				outputParam.suggestedLatency = Pa_GetDeviceInfo(
						outputParam.device)->defaultHighOutputLatency;

				outputParam.hostApiSpecificStreamInfo = NULL;
				err = Pa_OpenStream(&m_stream, NULL, &outputParam, sampleRate,
						framesPerBlock, paNoFlag, NULL, NULL);

//				if (err == paNoError) {
//					const PaStreamInfo *pStreamInfo = Pa_GetStreamInfo(m_stream);
//					cout << " Stream properties ";
//					cout << " output latency= " << pStreamInfo->outputLatency;
//					cout << "s sample rate= " << pStreamInfo->sampleRate
//							<< endl;
//				}
				m_state = S_READY;
			}

		}
	}
}

void CSimpleAudioOutStream::close(){
	if(m_state==S_NOTREADY)
		return;
	if(m_state==S_READY){

		err=Pa_CloseStream(m_stream);
		if(err!=paNoError)
		    	 throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));

		Pa_Terminate();
		m_state=S_NOTREADY;

	}
}

void CSimpleAudioOutStream::play(float* sbuf,int framesPerBlock){
	if(m_state==S_NOTREADY)
		throw CException(CException::SRC_SimpleAudioDevice,-1,"calling start() without opening stream!!");
	if(m_state==S_PLAYING){
     err= Pa_WriteStream(m_stream,sbuf,framesPerBlock);
     if(err!=paNoError)
    	 throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));
	}
}

void CSimpleAudioOutStream::start(){
	if(m_state==S_PLAYING)
		return;
	if(m_state==S_NOTREADY)
		throw CException(CException::SRC_SimpleAudioDevice,-1,"calling start() without opening stream!!");
	if(m_state==S_READY){
         err=Pa_StartStream(m_stream);
         m_state=S_PLAYING;
         if(err!=paNoError)
            	 throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));
	}
}

void  CSimpleAudioOutStream::stop(){
	if(m_state==S_NOTREADY)
		throw CException(CException::SRC_SimpleAudioDevice,-1,"calling stop() without opening starting stream!!");
	if(m_state==S_READY)
		return;
    if(m_state==S_PLAYING){
    	err=Pa_StopStream(m_stream);
    	 if(err!=paNoError)
    	            	 throw CException(CException::SRC_SimpleAudioDevice,err,Pa_GetErrorText(err));

    m_state=S_READY;

    }
}

void CSimpleAudioOutStream::resume(){
	start();
}

void CSimpleAudioOutStream::pause(){
	stop();
}
