/**
 * \file CAudioPlayerController.cpp
 * \brief implementation of CAudioPlayerController
 *
 * \date 10.01.2023
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 */
#define USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>			// functions to scan files in folders (used in Lab04prep_DBAdminInsert)
using namespace std;

#include "CException.h"
#include "CUserInterface.h"
#include "CAudioPlayerController.h"
#include "CFileFilter.h"
#include "CFilter.h"
#include "CFilterDelay.h"

CAudioPlayerController::CAudioPlayerController() {
	m_pSFile = NULL;		// association with 1 or 0 CSoundFile-objects
	m_pFilter = NULL;		// association with 1 or 0 CFilter-objects
}

CAudioPlayerController::~CAudioPlayerController() {
	if (m_pSFile)
		delete m_pSFile;
	if (m_pFilter)
		delete m_pFilter;
}

void CAudioPlayerController::run() {
	// if an exception has been thrown by init(), the user is not able to use the player
	// therefore it is handled by main (unrecoverable error)
	init();

	/***************************************************************
	 * main menue of the player
	 *
	 * note: the last item of the menu must be empty (see CUserInterfaceCmdIOW code)
	 *
	 ***************************************************************/
	string mainMenue[] = { "select sound", "select filter", "play",
			"choose amplitude scale", "terminate player", "" };
	while (1) {
		// if an exception will be thrown by one of the methods, the main menu will be shown
		// after an error message has been displayed. The user may decide, what to do (recoverable error)
		// For instance, if the user selects a filter without having selected a sound file before, an error
		// message tells the user to select a sound file. He/She may decide to do this and then select a filter
		// once more. In this case, the error condition is eliminated and the program may continue regularly.
		try {
			// display the menu and get the user's choice
			int selitem = m_ui.getListSelection(mainMenue, "choose an action");
			// process the user's choice by calling the appropriate CAudioPlayerControllerMethod
			switch (selitem) {
			case 0:
				chooseSound();
				break;
			case 1:
				chooseFilter();
				break;
			case 2:
				play();
				break;
			case 3:
				chooseAmplitudeScale();
				break;
			case 4:
				return;
			default:
				m_ui.printMessage("invalid selection. \n");
			}
		} catch (CException &e) // Exception from the ASDD components
		{
			string eMsg = "Error from: ";
			m_ui.printMessage(
					eMsg + e.getSrcAsString() + "" + e.getErrorText() + "\n");
		} catch (const std::invalid_argument &e) // exception from stod occurs if the last input is not a number
		{
			m_ui.printMessage("Enter a number to choose an action. \n");
		}
	}
}

void CAudioPlayerController::init() {
	// choose player control/visualization device
	string cvMenu[] = { "IOWarrior", "console", "" };
	int cvSel = m_ui.getListSelection(cvMenu,
			"select a device for player control / visualization:");
	// initialize the user interface
	switch (cvSel) {
	case 0:
		m_ui.init(CUserInterface::IOWARRIOR_EXT);
		break;
	case 1:
	default:
		m_ui.init(CUserInterface::CONSOLE);
		break;
	}
}

void CAudioPlayerController::chooseFilter() {
	if (!m_pSFile) // a sound file must have been created by the chooseSoundFile method before
	{
		m_ui.printMessage(
				"Error from selectFilter: No sound file. Select sound file before filter!\n");
		return;
	}

	string chosenFile;
	int fid = CUI_UNKNOWN;
	int delay_ms = 0;
	float gFB = 0., gFF = 0.;

	// show the menu and get the user's choice
	string fltMenue[] = { "delay filter", "other filter", "remove filter", "" };
	int idChoice = m_ui.getListSelection(fltMenue, "choose a filter type");

	if (idChoice == 0) // delay filter
			{
		// ask the user for the values of delay (ms), feed forward gain and feedback gain
		if (false == _configDelayFilter(delay_ms, gFF, gFB)) {
			m_ui.printMessage(
					"Error from selectFilter: Wrong configuration for delay filter. Did not change filter. \n");
			return;
		} else {
			// create a new delay filter (delete the old filter, if there already was one)
			_createDelayFilter(delay_ms, gFF, gFB);
		}
	} else if (idChoice == 1)	// "normal" filter
			{
		string filePath = ".\\files\\filters\\", fileExt = ".txt";
		// show a li st of the filters available in the path and get the users choice (fid) and
		// the appropriate filter file (chosenFile)
		fid = _chooseFilterFile(chosenFile, filePath, fileExt);
		if (fid == CUI_UNKNOWN)		// the user has chosen a filter
		{
			// wrong ID
			m_ui.printMessage(
					"Error from selectFilter: No filter data available! Did not change filter. \n");
		} else {
			// create the filter for the current sound file (delete the old filter, if there already was one)
			_createFilter(chosenFile);
		}
	} else // remove the current filter
	{
		if (m_pFilter) // if there was a filter object from a preceding choice of the user
		{
			delete m_pFilter;	// ... delete this
			m_pFilter = NULL;		// currently we have no filter
		}
		m_ui.printMessage("Message from selectFilter: Filter removed. \n");
		return;
	}
}

void CAudioPlayerController::play() {
	// todo delete next line and implement play() here
//	m_ui.printMessage("play() not yet implemented!\n");
	//m_pSFile->open();
	if(!m_pSFile){
       m_ui.printMessage("No Sound File Selected yet \n");
       return;
	}


	int framesPerB = m_pSFile->getSampleRate() / 8;
	int sbufSize = m_pSFile->getNumChannels() * framesPerB;
	float *sbuf = new float[sbufSize];
	float *sbufFilt = new float[sbufSize];
	int readSize=0;
	m_audioStream.open(m_pSFile->getNumChannels(), m_pSFile->getSampleRate(),
			framesPerB);
	m_audioStream.start();

	m_ui.printMessage("Press Enter to START/STOP/RESUME the Audio!!");
	while (1) {
		if (m_ui.keyPressed())
			break;
	}
   bool key=true;
	do{
		if(key){
		readSize=m_pSFile->read(sbuf, framesPerB);
		if(m_pFilter){
		m_pFilter->filter(sbuf, sbufFilt, framesPerB);
		m_ui.visualizeAmplitude(sbufFilt, readSize);
		m_audioStream.play(sbufFilt, readSize);

		}
		else{
			m_ui.visualizeAmplitude(sbuf, readSize);
			m_audioStream.play(sbuf, readSize);

		}
		}
		if (m_ui.keyPressed()){
			key=!key;
//			m_audioStream.pause();
//			while(m_ui.keyPressed()==false);
//			m_audioStream.resume();
		}
	}
    while(readSize==framesPerB);
	m_audioStream.stop();
	m_audioStream.close();

	m_pSFile->rewind();
}

void CAudioPlayerController::chooseSound() {
	// todo delete next line and implement chooseSound() here
//	m_ui.printMessage("chooseSound() not yet implemented!");
	string filePath=".\\files\\sounds\\",ext=".wav";
	string sndFList[14];
	int sid=CUI_UNKNOWN;
	int num =_getFiles(filePath,ext,sndFList,14);
	if(num){
		string* pSnd=new string[num+2];

		for(int i=0;i<num;i++){
           CFileSound sndFile(filePath+sndFList[i]);
           try{
        	   sndFile.open();

        	   pSnd[i]=sndFList[i]+"["+to_string(sndFile.getSampleRate())
        			   +"Hz,"+to_string(sndFile.getNumChannels())+" Channels]";
        	   sndFile.close();
           }
           catch(CException& e){
        	   if (e.getErrorCode() != CFileBase::E_FILENOTOPEN)
        		   sndFile.close();

           }
		}

		pSnd[num]="-1 no Snd file";

		sid=m_ui.getListSelection(pSnd, "Choose a sound file");
		if(sid==CUI_UNKNOWN)
			m_ui.printMessage("Please enter a valid input selection");

		if(sid!=CUI_UNKNOWN){
		CFileSound* snd=new CFileSound(filePath+sndFList[sid]);
		snd->open();

		if(m_pSFile)
		delete m_pSFile;

		m_pSFile=snd;
		}
  _adaptFilter();
  delete[] pSnd;
	}
	else
		m_ui.printMessage("No Sound Files available, please check folder of sounds!!");

}

void CAudioPlayerController::chooseAmplitudeScale() {
	// todo delete next line and implement chooseAmplitudeScale() here
//	m_ui.printMessage("chooseAmplitudeScale() not yet implemented!");
	string scaleMenu[] = { "Linear", "Logarthmic", "" };
	int usel = m_ui.getListSelection(scaleMenu, "choose type of scaling");
	switch(usel){
	case 0:
		m_ui.setAmplitudeScaling(CAmpMeter::SCALING_MODE_LIN);
		break;
	case 1:
		m_ui.setAmplitudeScaling(CAmpMeter::SCALING_MODE_LOG);
		break;
	default:
		m_ui.setAmplitudeScaling(CAmpMeter::SCALING_MODE_LIN);
		break;
	}
}

/**
 * private helper methods
 */
bool CAudioPlayerController::_configDelayFilter(int &delay_ms, float &gFF,
		float &gFB) {
	m_ui.printMessage("Delay filter configuration\n------------\n");
	delay_ms = abs(m_ui.getUserInputInt("delay in msec: "));
	gFF = m_ui.getUserInputFloat("feed forward gain: ");
	gFB = m_ui.getUserInputFloat("feed back gain  (0 <= gFB < 1): ");
	if ((gFB < 0.) || (gFB >= 1.))
		return false;
	return true;
}

void CAudioPlayerController::_createDelayFilter(int delay_ms, float gFF,
		float gFB) {
	// todo: comment out, if CDelayFilter exists
//	m_ui.printMessage(
//			"Delay filters are not yet implemented. Filter won't be changed. \n");
	// todo: comment in, if CDelayFilter exists
	if (m_pFilter)
		delete m_pFilter;
	m_pFilter = new CFilterDelay(gFF, gFB, delay_ms, m_pSFile->getSampleRate(),
			m_pSFile->getNumChannels());
}

int CAudioPlayerController::_chooseFilterFile(string &chosenFile,
		string filePath, string fileExt) {
	string filterlist[25];

	// get all files that are stored in filePath and that extension matches fileExt
	uint16_t numflt = _getFiles(filePath, fileExt, filterlist, 25); // get the list of filter files

	// list the files and their information
	uint16_t fid = CUI_UNKNOWN;
	if (numflt)									// if there are filters that fit
	{
		// prepare a string array for the user interface, that will contain  a menu with the selection of filters
		// there is place for an additional entry for an unfiltered sound and an empty string
		string *pFlt = new string[numflt + 2];

		for (int i = 0; i < numflt; i++) {
			// create a filter file object
			CFileFilter fltfile(filePath + filterlist[i]);
			try {
				fltfile.open();
				// check if it has a filter with an appropriate sampling frequency
				fltfile.read(m_pSFile->getSampleRate());
				// insert the filter data into the string array
				pFlt[i] = fltfile.getFilterType() + ", order="
						+ to_string(fltfile.getOrder())
						+ fltfile.getFilterInfo();
				fltfile.close();
			} catch (CException &e) {
				// file does not have the appropriate sampling frequency
				if (e.getErrorCode() != CFileBase::E_FILENOTOPEN)
					fltfile.close();
				// file could not be opened (nothing to do)
			}
		}

		// add the last menu entry for the choice of an unfiltered sound
		pFlt[numflt] = "-1 [no filter]";

		// pass the arrays to the user interface and wait for the user's input
		// if the user provides a filterID which is not in pFIDs, the method returns
		// CUI_UNKNOWN
		fid = m_ui.getListSelection(pFlt, "choose a filter");
		if (fid != CUI_UNKNOWN)
			chosenFile = filePath + filterlist[fid];

		// destroy the array
		delete[] pFlt;
	}
	return fid;
}

void CAudioPlayerController::_createFilter(string filterFile) {
	// if there was a filter object from a preceding choice of the user that does not fit anymore, delete this
	if (m_pFilter)
		delete m_pFilter;

	CFileFilter fltfile(filterFile);
	fltfile.open();
	fltfile.read(m_pSFile->getSampleRate());

	// create filter
	// Lab05 changed: get filter data
	uint8_t order = fltfile.getOrder();
	string type = fltfile.getFilterType();
	float *ac = fltfile.getACoeffs();
	float *bc = fltfile.getBCoeffs();
	m_pFilter = new CFilter(filterFile, ac, bc, order,
			m_pSFile->getNumChannels());
}

void CAudioPlayerController::_adaptFilter() {
	if (m_pFilter) {
		// check filter type
		if (typeid(*m_pFilter) == typeid(CFilter))
			_createFilter(((CFilter*) m_pFilter)->getFilePath());
		else {
			// todo: comment out, if CDelayFilter exists
//			m_ui.printMessage(
//					"Delay filters are not yet implemented. Filter will be deleted. \n");
//			if (m_pFilter)
//				delete m_pFilter;

			// todo: comment in, if CDelayFilter exists
			CFilterDelay *pdflt = (CFilterDelay*) m_pFilter;
			_createDelayFilter(pdflt->getDelay(), pdflt->getGainFF(),
					pdflt->getGainFB());
		}
	}
}

uint16_t CAudioPlayerController::_getFiles(string path, string ext,
		string *filelist, uint16_t maxNumFiles) {
	dirent *entry;
	DIR *dp;
	string file;

	dp = opendir(path.c_str());
	if (dp == NULL)
		throw(CException(this, typeid(this).name(), __FUNCTION__, -1,
				"Could not open folder." + path));

	uint16_t i = 0;
	while ((entry = readdir(dp))) {
		file = entry->d_name;
		if (file.rfind(ext) != string::npos) {
			if (i >= maxNumFiles) {
				m_ui.printMessage("Incomplete file list!\n");
				break;
			}
			filelist[i] = file;
			i++;
		}
	}
	closedir(dp);

	return i;
}

