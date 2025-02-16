/**
 * \file CAmpMeter.h
 * \brief interface of amplitude meter class CAmpMeter
 *
 * \date 29.11.2022
 * \author A. Wirth <antje.wirth@h-da.de
 * \author H. Frank <holger.frank@h-da.de
 */

#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#include <SKSLib.h>
#include "CPlayerCVDevice.h"
#include "CAmpMeter.h"

CAmpMeter::CAmpMeter() {
	m_scmode = SCALING_MODE_LIN;		// logarithmic or linear bar?
	m_inValMax = 0;						// maximum input value
	for (int i = 0; i < 16; i++)
		m_thresholds[i] = 0;			// thresholds for the bar with 16 segments
	m_IoDev = NULL;						// address of the IOWarrior extension board control object (visualizer)
}

void CAmpMeter::init(CPlayerCVDevice *pIoDev, SCALING_MODE scmode, float inValMin,
		float inValMax, int8_t outLogValMin) {
	/*
	 * calculate the highest absolute value of the value range of the input signal
	 */
	m_inValMax = fmax(fabs(inValMax), fabs(inValMin));
	/*
	 * set the scaling mode
	 */
	m_scmode = scmode;
	/*
	 * a value of NULL for pIoDev is accepted here and won't throw an exception
	 * in order to get writeConsole() to work without an IoW attached.
	 *
	 * all other params are necessary to calculate the thresholds.
	 */
	m_IoDev = pIoDev;

	/*
	 * todo Calculate the threshold values according to the formulas from
	 * preparation task 2 and store them in m_thresholds.
	 *
	 *
	 * Linear scaling: 0 ... m_inValMax
	 * Logarithmic scaling: outLogValMin ... 0 [dB] (outLogValMin must be negative)
	 */
	if(outLogValMin>0)
		outLogValMin=-outLogValMin;

	float out_logVal=0;
	if(scmode==CAmpMeter::SCALING_MODE_LIN){
		float interval_width=(m_inValMax/16);    //remember m_inValMax is of type float , so i_w also should be float
		for(int i=0;i<16;i++){
			m_thresholds[i]=interval_width*i;
		}
	}
	else {
		if(scmode==CAmpMeter::SCALING_MODE_LOG){
			for(int i=0;i<16;i++){
				out_logVal=(outLogValMin/15)*(15-i);    //remember we should use outLogMin as negative only
				m_thresholds[i]=pow(10,(out_logVal/10.0f));
			}
		}
	}
}

void CAmpMeter::write(float *databuf, unsigned long databufsize) {
	if (NULL == databuf)
		throw CException(this, typeid(this).name(), __FUNCTION__, AMP_E_NOBUFFER,
				"Invalid data buffer");
	return write(_getValueFromBuffer(databuf, databufsize));
}

void CAmpMeter::write(float data) {
	if (NULL == m_IoDev)
		throw CException(this, typeid(this).name(), __FUNCTION__, AMP_E_NOVISUALIZER,
				"Can't do binary pattern output.");

	m_IoDev->writeBarPattern(_getBarPattern(data));
}

void CAmpMeter::writeConsole(float data) {
	uint16_t pattern = _getBarPattern(data);
	uint16_t revPattern = 0;

	/*
	 * the LEDs of the extension board are assigned from LSB leftmost (D1) to
	 * MSB rightmost (D16) and so _getBarPattern() is working
	 * => pattern for console output has to be reversed
	 */
	for (int i = 0; i < 16; i++) {
		revPattern = revPattern << 1 | (pattern & 1);
		pattern >>= 1;
	}

	cout << (bitset<16> ) revPattern << endl;
}

uint16_t CAmpMeter::_getBarPattern(float data) {
	uint16_t pattern = 0;
	/*
	 * todo: Calculate the appropriate bar pattern (pattern) for the absolute value of data.
	 * The value of data is a linear value in any case.
	 *
	 * The calculated bar pattern is used to visualize the absolute value of data on the
	 * IOWarrior extension board.
	 *
	 * Example for a bar pattern with 8 elements:
	 * pat is 0b11111111 , if the absolute value
	 * of data is greater than the threshold of the 8th interval.
	 *
	 * Logarithmic scaling mode:
	 * Before calculating the bar pattern, the absolute value of data shall be
	 * divided by the maximum input value to adjust the highest value to 0 dB
	 * (peak normalization).
	 */
	data=fabs(data);

    if(m_scmode==CAmpMeter::SCALING_MODE_LOG){
	data= data/m_inValMax;  //peak normalisation
    }
    uint64_t mask=1000000000000000;
     for(int i=0;i<16;i++){
    	 if(data>=m_thresholds[i]){
    	 pattern |= mask;
    	 mask>>=1;
    	 }
    	 else
    		 break;
     }
	return pattern;
}

float CAmpMeter::_getValueFromBuffer(float *databuf,
		unsigned long databufsize) {
	float maxVal = 0., tmpVal;
	for (unsigned long idx = 0; idx < databufsize; idx++) {
		tmpVal = fabs(databuf[idx]);
		if (maxVal < tmpVal) {
			maxVal = tmpVal;
		}
	}
	return maxVal;
}
