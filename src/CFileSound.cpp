/**
 * \file CFileSound.cpp
 *
 * \brief implementation class CFileSound
 * \date 17.11.2023
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de
 *
 * \see File
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

#include <SKSLib.h>
#include "CFileSound.h"

CFileSound::CFileSound(string path, const string mode) :
		CFileBase(path, mode) {
	memset(&m_sfinfo, 0, sizeof(m_sfinfo));
	m_pSFile = NULL;
//	cout << "CFileSound@" << hex << this << dec << " created" << endl;
}

CFileSound::~CFileSound() {
	close();
//	cout << "CFileSound@" << hex << this << dec << " destroyed" << endl;
}

/*
 * overloaded method from CFileBase
 */
void CFileSound::print(void) {
	if (m_pSFile != 0) {
		CFileBase::print();
		cout << "CFileSound@" << hex << this << dec << endl;
		cout << " Soundfile: channels(" << m_sfinfo.channels << ") frames("
				<< m_sfinfo.frames;
		cout << ") fs(" << m_sfinfo.samplerate / 1000. << "kHz)" << endl;
		cout << "Duration: " << m_sfinfo.frames / m_sfinfo.samplerate << "s"
				<< endl;
	} else
		throw CException(this, typeid(this).name(), __FUNCTION__, E_FILENOTOPEN,
				m_path + " " + getErrorTxt(E_FILENOTOPEN));
}

void CFileSound::open() {
	int sf_mode;		// file accessmode for sndfile lib
	if (isFileR() && (isFileWA() || isFileW())) {
		sf_mode = SFM_RDWR;
	} else if (isFileW() || isFileWA()) {
		sf_mode = SFM_WRITE;
	} else if (isFileR()) {
		sf_mode = SFM_READ;
	} else
		throw CException(this, typeid(this).name(), __FUNCTION__,
				E_UNKNOWNOPENMODE, getErrorTxt(E_UNKNOWNOPENMODE));

	m_pSFile = sf_open(m_path.c_str(), sf_mode, &m_sfinfo);
	if (!m_pSFile)
		throw CException(this, typeid(this).name(), __FUNCTION__,
				sf_error(m_pSFile), m_path + ": " + sf_strerror(m_pSFile));

//	cout << "CFileSound@" << hex << this << dec << " '" << m_path << "' opened"
//			<< endl;
}

void CFileSound::close() {
	if (m_pSFile != NULL) {
		sf_close(m_pSFile);
		m_pSFile = NULL;
//		cout << "CFileSound@" << hex << this << dec << " '" << m_path
//				<< "' closed" << endl;
	}
}

uint64_t CFileSound::read(float *buf, uint64_t frameNum) {
	if (m_pSFile == NULL)
		throw CException(this, typeid(this).name(), __FUNCTION__, E_FILENOTOPEN,
				getErrorTxt(E_FILENOTOPEN));
	if (buf == NULL)
		throw CException(this, typeid(this).name(), __FUNCTION__, E_NOBUFFER,
				getErrorTxt(E_NOBUFFER));
	if (isFileW() || isFileWA())
		throw CException(this, typeid(this).name(), __FUNCTION__, E_CANTREAD,
				getErrorTxt(E_CANTREAD));

	uint64_t szread = sf_readf_float(m_pSFile, buf, frameNum);
	// returns 0 if no data left to read
	return szread;
}

uint64_t CFileSound::write(float *buf, uint64_t frameNum) {
	if (m_pSFile == NULL)
		throw CException(this, typeid(this).name(), __FUNCTION__, E_FILENOTOPEN,
				getErrorTxt(E_FILENOTOPEN));
	if (buf == NULL)
		throw CException(this, typeid(this).name(), __FUNCTION__, E_NOBUFFER,
				getErrorTxt(E_NOBUFFER));
	if (isFileR())
		throw CException(this, typeid(this).name(), __FUNCTION__, E_CANTWRITE,
				getErrorTxt(E_CANTWRITE));

	uint64_t szwrite = sf_writef_float(m_pSFile, buf, frameNum);
	if (szwrite != frameNum) {
		close();
		throw CException(this, typeid(this).name(), __FUNCTION__, E_WRITE,
				getErrorTxt(E_WRITE));
	}
	return szwrite;
}

void CFileSound::rewind() {
	if (m_pSFile == NULL)
		throw CException(this, typeid(this).name(), __FUNCTION__, E_FILENOTOPEN,
				getErrorTxt(E_FILENOTOPEN));

	sf_seek(m_pSFile, 0, SEEK_SET);
}

uint64_t CFileSound::getNumFrames() {
	return m_sfinfo.frames;
}

uint32_t CFileSound::getSampleRate() {
	return m_sfinfo.samplerate;
}

uint32_t CFileSound::getFormat() {
	return m_sfinfo.format;
}

uint16_t CFileSound::getNumChannels() {
	return m_sfinfo.channels;
}

void CFileSound::setSampleRate(uint32_t fs) {
	m_sfinfo.samplerate = fs;
}

void CFileSound::setNumChannels(uint16_t numChan) {
	m_sfinfo.channels = numChan;
}

void CFileSound::setFormat(uint32_t format) {
	m_sfinfo.format = format;
}
