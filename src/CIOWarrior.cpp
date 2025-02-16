/**
 * \file CIOWarrior.cpp
 *
 * \brief implementation of the IOWarrior40 control class
 * \author Antje Wirth <antje.wirth@h-da.de>
 * \author Holger Frank <holger.frank@h-da.de>
 */
using namespace std;
#include <iostream>
#include <string>
#include <typeinfo>
#include <SKSLib.h>
#include "CIOWarrior.h"

// todo implement methods of class CIoWarrior here
CIOWarrior::CIOWarrior(){
	m_handle=NULL;
    m_reportIn.ReportID=0;           //for plain IO communication
	m_reportOut.ReportID=0;
	m_state=S_NOTREADY;
	m_lastError=E_OK;

}
CIOWarrior::~CIOWarrior(){
    close();
}

void CIOWarrior::open(){
   if(m_state==S_READY)
	   return;
   if(m_state==S_NOTREADY){

	   m_handle=IowKitOpenDevice();
       if(m_handle==NULL){
    	   m_lastError=E_NODEVICE;
    	   throw CException(CException::SRC_IOWarrior,E_NODEVICE,"Unable to open device, please check connectoion");
             }
       if(IowKitGetProductId(m_handle)!=IOWKIT_PRODUCT_ID_IOW40){
           IowKitCloseDevice(m_handle);
           m_handle = NULL;
    	   throw CException(CException::SRC_IOWarrior,E_INVALIDDEVICE,"please use IOW40");
       }

       m_reportOut.Value=0xffffffff;
       m_lastError=E_OK;
       writeReportOut();
       m_state=S_READY;

   }
}

void CIOWarrior::close(){
	if(m_state==S_NOTREADY)
		return;

	m_reportOut.Value=0xffffffff;
	writeReportOut();
	IowKitCloseDevice(m_handle);
	m_state=S_NOTREADY;


}

void CIOWarrior::writeBarPattern(uint8_t data){
	if(m_state==S_NOTREADY)
		throw CException(CException::SRC_IOWarrior,E_DEVICENOTREADY,"Device is not ready");

	m_reportOut.Bytes[3]=~data;
	writeReportOut();

}

bool CIOWarrior::keyPressed() {
	if (m_state == S_READY) {
		bool currentState = false;
		static bool previousState = false;
		if (sizeof(m_reportIn)
				== IowKitReadNonBlocking(m_handle, IOW_PIPE_IO_PINS,
						(char*) &m_reportIn, sizeof(m_reportIn))) {
			currentState = ((m_reportIn.Bytes[0] & 0x03) == 0);   //& is bitwise AND operation
			if (currentState && (!previousState)) {
				previousState = currentState;
				return true;
			}
			previousState = currentState;
		}
	} else
		throw CException(CException::SRC_IOWarrior, E_DEVICENOTREADY,
				"Device is not ready");

	return false;
}

void CIOWarrior::printState(){

	cout << "State: " << getState() << " / " << getStateStr()
					<< endl << "Last Error: " << getLastError() << " / "
					<< getLastErrorStr() << endl;
}
CIOWarrior::STATES CIOWarrior::getState(){

	return m_state;
}

string CIOWarrior::getStateStr(){


    switch(m_state){
    case S_READY:
    	return "S_READY";
    	break;
    case S_NOTREADY:
        return "S_NOTREADY";
    }

   return "UNKNOWN_STATE";
}

CIOWarrior::ERRORS CIOWarrior::getLastError() {
	return m_lastError;
}

string CIOWarrior::getLastErrorStr() {

	switch(m_lastError){
	case E_OK:
		return "E_OK";
		break;
	case E_NODEVICE:
		return "E_NODEVICE";
		break;
	case E_INVALIDDEVICE:
		return "E_INVALIDDEVICE";
		break;
	case E_WRITEERROR:
		return "E_WRITEERROR";
		break;
	case E_DEVICENOTREADY:
		return "E_DEVICENOTREADY";
		break;
	case E_NOEXTENSION:
		return "E_NOEXTENSION";
	}

	return "UNKNOWN_ERROR";
}

void CIOWarrior::printDeviceInfo() {
	if(m_state==S_NOTREADY)
		 throw CException(CException::SRC_IOWarrior,E_DEVICENOTREADY,"Device is not ready");

	PWCHAR buffer=NULL;
	cout<<"Serial number:"<<IowKitGetSerialNumber(m_handle,buffer)<<"\t";
	cout<<"Product revison :"<<IowKitGetRevision(m_handle)<<"\t";
	cout<<"Api version :"<<IowKitVersion()<<endl;

}

void CIOWarrior::writeReportOut() {
	if(m_state==S_NOTREADY)
			throw CException(CException::SRC_IOWarrior,E_DEVICENOTREADY,"Device is not ready");

	ULONG res=IowKitWrite(m_handle, IOW_PIPE_IO_PINS,
						(char*) &m_reportOut, sizeof(m_reportOut));
	if(res!=sizeof(m_reportOut)){
       m_lastError=E_WRITEERROR;
       close();
       throw CException(CException::SRC_IOWarrior,m_lastError,"Unable to write pattern on to IOW40");
	}
}
