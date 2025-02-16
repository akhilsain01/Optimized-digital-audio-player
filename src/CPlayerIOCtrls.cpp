/**
 * \file CPlayerIOCtrls.cpp
 *
 * \brief implementation of CPlayerIOCtrls
 * \date 30.10.2020
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 */
#include "windows.h"
#include "ctype.h"
#include <iostream>
using namespace std;

#include "CException.h"
#include "CPlayerIOCtrls.h"

CPlayerIOCtrls::CPlayerIOCtrls() {
	/*
	 * get the address of the one and only console thread object
	 */
	m_thread = CConsoleThread::getInstance();

	// all "LEDs" off
	/*
	 * print only a carriage return (\r) and no line feed (\n) so
	 * the bit pattern is always printed in the same line in the
	 * external console window
	 *
	 * in the eclipse console view \r is always printed with an
	 * additional line feed, so the patterns are printed in
	 * subsequent lines
	 */
	m_binPattern = "0000000000000000\t00000000\r";
}

CPlayerIOCtrls::~CPlayerIOCtrls() {
	close();
}

/**
 * INTERFACE of CPlayerCVDevice: implementation for the console
 */
void CPlayerIOCtrls::open() {
	m_thread->open();
	return;
}

void CPlayerIOCtrls::close() {
	// nothing to do, but must be provided, because of the mandatory interface of
	// player control/visualization devices
}

void CPlayerIOCtrls::writeBarPattern(uint16_t data) {
	string binPattern;
	data = reverseBits(data, sizeof(data));
	for (uint8_t i = 0; i < 16; i++) {
		data & 0x8000 ? binPattern += "1" : binPattern += "0";
		data <<= 1;
	}

	m_binPattern.replace(0,16,binPattern);
	m_thread->writeConsole(m_binPattern);
	return;
}

void CPlayerIOCtrls::writeBarPattern(uint8_t data) {
	string binPattern;
	data = reverseBits(data, sizeof(data));
	for (uint8_t i = 0; i < 8; i++) {
		data & 0x80 ? binPattern += "1" : binPattern += "0";
		data <<= 1;
	}
	m_binPattern.replace(17,8,binPattern);
	m_thread->writeConsole(m_binPattern);
	return;
}

uint16_t CPlayerIOCtrls::reverseBits(uint16_t value, uint8_t byteSize) {
	uint16_t rev = 0;
	uint8_t bitSize = 8 * byteSize;
	for (uint8_t i = 0; i < bitSize; i++) {
		rev |= ((value & (1 << i) ? 1 : 0) << (bitSize - 1 - i));
	}
	return rev;
}

bool CPlayerIOCtrls::keyPressed() {
	return m_thread->enterPressed();
}

string CPlayerIOCtrls::getStateStr() {
	return m_thread->getStateStr();
}

string CPlayerIOCtrls::getLastErrorStr() {
	return m_thread->getLastErrorStr();
}

int CPlayerIOCtrls::getState() {
	// besser: static_cast<int>(...)
	return (int) m_thread->getState();
}

int CPlayerIOCtrls::getLastError() {
	// besser: static_cast<int>(...)
	return (int) m_thread->getLastError();
}

