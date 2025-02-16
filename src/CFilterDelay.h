/**
 * \file CFilterDelay.h
 * \brief interface CFilterDelay
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
#ifndef CFILTERDELAY_H_
#define CFILTERDELAY_H_

#include "CFilterBase.h"

class CFilterDelay: public CFilterBase {
private:
	float m_gFF;
	float m_gFB;
	uint16_t m_firstZ;
	uint16_t m_delay_ms;

public:
	CFilterDelay(float gff, float gFB, uint16_t delay_ms, uint32_t fs, uint16_t channels = 2);
	void reset();
	bool filter(float *inBuf, float *outBuf, uint16_t framesPerBuffer);
	uint16_t getDelay();
	float getGainFF();
	float getGainFB();
};

#endif /* CFILTERDELAY_H_ */

