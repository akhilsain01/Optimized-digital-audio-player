/**
 * \file CPlayerIOCtrls.h
 *
 * \brief header file of the modified PlayerIO control class
 * 		  the class handles:
 *        - thread controlled key input
 *        - binary pattern output
 */
#ifndef CPlayerIOCtrls_H_
#define CPlayerIOCtrls_H_

#include "CException.h"
#include "CConsoleThread.h"
#include "CPlayerCVDevice.h"

/**
 * PlayerIO control class
 *
 * modified for ASDD-Lab05 (systemintegration)
 *
 * the class handles:
 * - thread controlled key input
 * - binary pattern output
 */
class CPlayerIOCtrls: public CPlayerCVDevice {
private:
	/**
	 * \brief binary pattern for console output via CConsoleIO
	 *
	 * this string contains the 16-bit binary pattern and subsequent 8-bit
	 * pattern divided by a tab because the output via CConsoleIO is implemented
	 * by only on string object
	 */
	string m_binPattern;

protected:
	CConsoleThread *m_thread;

public:
	/**
	 * Constructor
	 * initializes the attributes with initial values
	 */
	CPlayerIOCtrls();

	/**
	 * Destructor
	 */
	virtual ~CPlayerIOCtrls();

	/**
	 * \brief Starts both of the threads
	 *
	 * throws an exception if it failed to start the threads.
	 */
	void open();

	/**
	 * \brief stops the threads
	 */
	void close();

	/**
	 * \brief Prints a binary formatted 16-bit bar pattern on the screen.
	 *
	 * throws an exception if the object is not in ready state
	 *
	 * \param data [in] to show as binary 16-bit pattern on the screen
	 */
	void writeBarPattern(uint16_t data);

	/**
	 * \brief Prints a binary formatted 8-bit bar pattern on the screen.
	 *
	 * throws an exception if the object is not in ready state
	 *
	 * \param data [in] to show as binary 8-bit pattern on the screen
	 */
	void writeBarPattern(uint8_t data);

	/**
	 * \brief monitors the return key
	 *
	 * throws an exception if the object is not in ready state
	 *
	 * \return
	 * - true:  user pressed ENTER
	 * - false: otherwise
	 */
	bool keyPressed();

	/**
	 * \return current state of the instance
	 */
	string getStateStr();

	/**
	 * \return last error of the instance
	 */
	string getLastErrorStr();

	/**
	 * \brief Queries the current state of instance
	 * \return current state of instance as number
	 */
	int getState();

	/**
	 * \brief Queries the last error of instance
	 *
	 * \return number of last error
	 */
	int getLastError();
private:
	/**
	 * \brief reverses bit order of integer value
	 *
	 * necessary for writeBarPattern() to realize a left-to-right bit pattern
	 * (i.e. LSB is on the left, MSB on the right) for an amplitude meter
	 *
	 * \param value pattern to be reversed
	 * \param byteSize number of bytes of value
	 * \return reversed bit pattern
	 */
	uint16_t reverseBits(uint16_t value, uint8_t byteSize = 1);

};

#endif /* CPlayerIOCtrls_H_ */
