/**
 * \file CFilterDelay.cpp
 * \brief implementation CFilterDelay
 *
 * \date 11.09.2019
 * \author A. Wirth <antje.wirth@h-da.de
 */
/************************************************************************
 *
 *	 					ATTENTION !!!!!!!!!
 *
 * This file contains only rudimentary code in a nonfunctional version
 * to make the start project compilable and executable in its initial version.
 *
 * It should be replaced by an complete implementation.
 *
 ************************************************************************/
#include <math.h>
#include <SKSLib.h>
#include "CFilterDelay.h"

CFilterDelay::CFilterDelay(float gFF, float gFB, uint16_t delay_ms, uint32_t fs,
		uint16_t channels) :
		CFilterBase((uint16_t) (delay_ms * fs / 1000.), channels) {
}

void CFilterDelay::reset() {
}

bool CFilterDelay::filter(float *inBuf, float *outBuf, uint16_t framesPerBuffer) {
	return false;
}

uint16_t CFilterDelay::getDelay() {
	return 0;
}

float CFilterDelay::getGainFF() {
	return 0.;
}

float CFilterDelay::getGainFB() {
	return 0.;
}

