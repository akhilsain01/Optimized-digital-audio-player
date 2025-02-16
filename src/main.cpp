/**
 * \file main.cpp
 * \brief main, test and auxiliary functions for ASDD Lab05
 *
 * \date 17.01.2022
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 */
#include "CAudioPlayerController.h"
#include "CFileFilter.h"
#include "CFilter.h"

/**
 * horizontal divider for test list output
 */
string hDivider(80, '-');

/*
 * *****************************************************
 * declarations of test functions (to be called in main)
 * *****************************************************
 */
// laboratory tasks
void Test_Lab01_SoundFilterPlayTest(string &soundfile, string &sndfile_w,
		string &fltfile);

int main(void) {
	setvbuf(stdout, NULL, _IONBF, 0);
	cout << "Systemintegration started" << endl;

	/*
	 * todo: Check if Test_Lab01_SoundFilterPlayTest() works
	 * comment out after check
	 */
	string sndname = "jazzyfrenchy";
	string sndf = ".\\files\\sounds\\" + sndname + ".wav";
	string sndfw = ".\\files\\sounds\\" + sndname + "_filtered.wav";
	string fltf = ".\\files\\filters\\2000Hz_lowpass_Order6.txt";
	try {
	Test_Lab01_SoundFilterPlayTest(sndf, sndfw, fltf);

	/*
	 * todo: comment in for Lab Task 2
	 */
//	CAudioPlayerController myController; 	// create the controller
//
//	myController.run();					// run the controller
	} catch (CException &e) {
		cout << e << endl;
	}
	cout << "Systemintegration finished" << endl;
}

void Test_Lab01_SoundFilterPlayTest(string &soundfile, string &sndfile_w,
		string &fltfile) {
	cout << endl << hDivider << endl << __FUNCTION__ << " started." << endl
			<< endl;

// todo insert the code of Test_Lab01_SoundFilterPlayTest() here
	CSimpleAudioOutStream m_stream;
	CFileSound sndF(soundfile);
	sndF.open();

	CFileSound sndF1(sndfile_w,"w");
	sndF1.setFormat(sndF.getFormat());
	sndF1.setNumChannels(sndF.getNumChannels());
	sndF1.setSampleRate(sndF.getSampleRate());
	sndF1.open();

	CFileFilter filterfile(fltfile);
	filterfile.open();
	filterfile.read(sndF.getSampleRate());
    float* ca=filterfile.getACoeffs();
    float* cb=filterfile.getBCoeffs();
    uint16_t order=filterfile.getOrder();

    CFilter filter(filterfile.getFilepath(),ca,cb,order,sndF.getNumChannels());

	int readSize=0;
	int framesPerBlock =sndF.getSampleRate()/10;
	int sbufSize=sndF.getNumChannels()*framesPerBlock;
    float* sbufBlock=new float[sbufSize];
    float* sbufFilt=new float[sbufSize];

    m_stream.open(sndF.getNumChannels(), sndF.getSampleRate(), framesPerBlock);
    m_stream.start();

    do{
         readSize=sndF.read(sbufBlock, framesPerBlock);
         filter.filter(sbufBlock, sbufFilt, framesPerBlock);
         m_stream.play(sbufFilt, framesPerBlock);

    }
  while(readSize==framesPerBlock);
    m_stream.stop();
    m_stream.close();
    sndF.close();
    sndF1.close();

    delete[] sbufBlock;
    delete[] sbufFilt;

	cout << endl << __FUNCTION__ << " finished." << endl << hDivider << endl;
}
