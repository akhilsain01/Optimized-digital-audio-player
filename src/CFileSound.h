/**
 * \file CFileSound.h
 *
 * \brief interface class CFileSound
 * \date 17.11.2023
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de
 *
 * \see File
 */
#ifndef FILESOUND_H_
#define FILESOUND_H_

#include "sndfile.h"
#include "CFileBase.h"
#include <string>
using namespace std;

/**
 * \brief Soundfile handling by libsndfile library
 *
 * reads sound data from file to given buffer, writes sound data from given buffer to file
 *
 * collects metadata from soundfile into SF_INFO structure
 *
 * uses libsndfile library
 *
 * \see
 * http://mega-nerd.com/libsndfile/api.html
 */
class CFileSound: public CFileBase {
private:
	/**
	 * \brief pointer on soundfile handler
	 *
	 * is the same as FILE * in C
	 */
	SNDFILE *m_pSFile;
	/**
	 * metadata of soundfile
	 *
	 * - samplerate
	 * - channels
	 * - format (see libsndfile API documentation)
	 * - sections
	 * - seekable
	 */
	SF_INFO m_sfinfo;

public:
	/**
	 * \brief constructor
	 *
	 * initializes m_sfinfo with zeroes and initializes attributes
	 *
	 * \param path path to soundfile
	 * \param mode file access mode
	 */
	CFileSound(const string path, const string mode = "r");
	/**
	 * \brief destructor
	 *
	 * closes soundfile
	 */
	~CFileSound();

	/**
	 * \brief opens the sound file selected by parameter path in constructor
	 */
	void open();
	/**
	 * \brief closes the sound file selected by parameter path in constructor
	 */
	void close();
	/**
	 * \brief reads the content of the sound file
	 *
	 * \param buf - address of buffer to store floating point audio data
	 * \param frameNum - number of frames in buffer
	 * \return total number of frames read
	 */
	uint64_t read(float *buf, uint64_t frameNum);
	/**
	 * \brief writes the content of the sound file
	 *
	 * \param buf - address of buffer to store floating point audio data
	 * \param frameNum - size of the buffer in frames
	 * \return total number of frames written
	 */
	uint64_t write(float *buf, uint64_t frameNum);
	/**
	 * \brief rewind sound file
	 *
	 * sets the file pointer of an open sound file back to the start
	 */
	void rewind();
	/**
	 * \brief prints info of the sound file on the console
	 *
	 * the info contains file information delivered by the file functions
	 * \see CFileBase::print() and information of the audio data delivered by
	 * libsndfile \see m_sfinfo
	 */
	void print(void);
	/**
	 * \brief delivers the number of frames (samples / channels) contained in the sound file
	 * \return number of frames
	 */
	uint64_t getNumFrames();
	/**
	 * \brief delivers the sample rate of the sound file
	 * \return sample rate in Hz
	 */
	uint32_t getSampleRate();
	/**
	 * \brief delivers number of audio channels
	 * \return number of audio channels
	 */
	uint16_t getNumChannels();
	/**
	 * \brief delivers the format of the soundfile / the audio data
	 * \return data format
	 */
	uint32_t getFormat();

	/**
	 * \brief sets the sample rate for audiofile
	 * \param fs sample rate in Hz
	 */
	void setSampleRate(uint32_t fs);
	/**
	 * \brief sets the number of channels
	 * \param numChan number of channels
	 */
	void setNumChannels(uint16_t numChan);
	/**
	 * \brief sets the format for the audiofile
	 * \param format type and subtype of the audio format
	 *
	 * \see
	 */
	void setFormat(uint32_t format);
};
#endif /* FILESOUND_H_ */
