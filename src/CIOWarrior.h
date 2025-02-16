/**
 * \file CIOWarrior.h
 *
 * \brief Interface for CIOWarrior, CIoWarriorExt
 * \date 05.10.21
 * \author Antje Wirth <antje.wirth@h-da.de>
 * \author Holger Frank <holger.frank@h-da.de>
 */
#ifndef CIOWARRIOR_H_
#define CIOWARRIOR_H_

#include <SKSLib.h>
#include <wtypes.h>
#include <iowkit.h>
#include "CPlayerCVDevice.h"


/**
 * \brief Controller for a bare IoW40Kit device
 */
class CIOWarrior:public CPlayerCVDevice {
public:
	// todo define the enums ERRORS and STATES here (see UML class diagram)
	enum STATES{
		S_NOTREADY,S_READY
	};
	enum ERRORS{
		E_OK,
		E_NODEVICE,
		E_INVALIDDEVICE,
		E_WRITEERROR,
		E_DEVICENOTREADY,
		E_NOEXTENSION
	};

protected:
	// todo define the attributes according to the UML class diagram here
	IOWKIT_HANDLE m_handle;
	IOWKIT40_IO_REPORT m_reportIn;
	IOWKIT40_IO_REPORT m_reportOut;
	STATES m_state;
	ERRORS m_lastError;

public:
	// todo define the public methods according to the UML class diagram here
	/**
	 * \brief initializes attributes
	 */
	// todo define constructor here
     CIOWarrior();


	/**
	 * \brief closes the device
	 * \transition
	 * S_READY -> S_NOTREADY
	 */
	// todo define destructor here
    ~CIOWarrior();


	/**
	 * \brief Opens IOWarrior device.
	 *
	 * The default value sets all outputs to high. In this state all port pins
	 * can also be read as input
	 *
	 * \exception
	 * - no device found
	 * - wrong device id (IoW found, but no IoW40)
	 *
	 * \transition
	 * S_NOTREADY -> S_READY
	 *
	 * \test
	 * to test the validation of the IoW device type, connect an IoW24Kit
	 * instead of IoW40Kit
	 *
	 * functional description
	 * ======================
	 * - skips all steps if already in READY state
	 * - opens IoWDevice, throws exception if no device found
	 * - reads product id, closes device and throws error if not an IoWarrior40 detected
	 * - sets all output port pins to logical 1
	 * - sets state to READY
	 * - sets last Error to OK
	 */
	//todo define open() here
      void open();


	/**
	 * \brief Closes IOWarrior device.
	 *
	 * the value of finalOutVal should be selected so, that the power
	 * consumption of the device is minimized(e.g. all LEDs off). this depends
	 * on the attached circuits. 0xffxxxxxx switches built-in LEDs on port 3 off
	 * (they are low active)
	 *
	 * \transition
	 * S_READY -> S_NOTREADY
	 *
	 * functional description
	 * ======================
	 * - skips if state is NOTREADY
	 * - sets all output pins to logical 1
	 * - closes IoW device
	 * - sets state to NOTREADY
	 */
	//todo define close() here
     void close();


	/**
	 * \brief writes 8-bit pattern to IOWarrior port 3 (8 red LED on base board)
	 * \param data 8-bit pattern shown on LEDs
	 *
	 * the pattern is handled in positive logic, i.e. 1-bit switches the LED on,
	 * 0-bit switches the LED off
	 *
	 * \exception
	 * - device not ready
	 * - write error
	 *
	 *
	 * functional description
	 * ======================
	 * - throws error if NOTREADY
	 * - sets all LED on whose bit is set in data
	 */
	// todo define writeLEDs here
     void writeBarPattern(uint8_t data);


	/**
	 * \brief detects if the key was pressed
	 *
	 * \return
	 * - true: key pressed
	 * - false: key released or no change since last scan of the IoW chip
	 *
	 * \exception
	 * - device not ready
	 *
	 *
	 * functional description
	 * ======================
	 * - throws error if NOTREADY
	 * - check if new report is available (any input pin has changed)
	 * - returns true if pressed SW1 is detected, otherwise returns false
	 */
	// todo define keyPressed() here
     bool keyPressed();


	/**
	 * \brief Prints the current state of IOWarrior instance.
	 *
	 * functional description
	 * ======================
	 * prints m_lastError and m_state both as strings and numerical values on
	 * the console
	 */
	// todo define printState() here
    void printState();


	/**
	 * \brief delivers current state of IOWarrior object
	 * \return state as numerical value
	 */
	// todo define getState() here
    STATES getState();


	/**
	 * \brief delivers current state of IOWarrior object as a string
	 * \return state as string
	 */
	// todo define getStateStr() here
    string getStateStr();


	/**
	 * \brief delivers last error of IOWarrior instance
	 * \return error number
	 */
	// todo define getLastError() here
    ERRORS getLastError();


	/**
	 * \brief delivers last error of IOWarrior instance
	 * \return last error text
	 */
	// todo define getLastErrorStr()
     string getLastErrorStr();


	/**
	 * \brief prints device info on console
	 *
	 * prints the infos of CIOWarrior:printDeviceInfo() plus a message that the
	 * extension is available
	 *
	 * \exception
	 * - device not ready
	 *
	 *
	 * functional description
	 * ======================
	 * - throws error if NOTREADY
	 * - reads serial number, Product revision and API version
	 * - prints read data on console
	 */
	// todo define printDeviceInfo() here
     void printDeviceInfo();

protected:
	// todo define the protected methods according to the UML class diagram here
	/**
	 * writes all 4 bytes of m_reportOut on the IoW40 ports
	 *
	 * \exception
	 * - write error
	 *
	 *
	 * functional description
	 * ======================
	 * - writes m_reportOut to all output pins
	 * - checks if write was successful
	 * - stores error no., closes IoW and throws exception if not
	 */
	// todo define writeReportOut() here
     void writeReportOut();
};
#endif /* CIOWARRIOR_H_ */
