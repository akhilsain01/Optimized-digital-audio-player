/*
 * CFileFilter.h
 *
 *  Created on: Feb 6, 2025
 *      Author: padmavathisrinivasa
 */

#ifndef CFILEFILTER_H_
#define CFILEFILTER_H_
#include "CFileBase.h"
/*
 *
 */
class CFileFilter: public CFileBase {
private:
	 FILE* m_pFile;

	 string m_type;
	 int m_order;
	 string m_info;
     int m_fs;
     float* m_a;
     float* m_b;
     int blen;
     int alen;

public:
	CFileFilter(const string path, const string mode="r");
	virtual ~CFileFilter();

	void open();
	void close();
	long read(int fs);
	void print(); //filter file contents printed together with file base print
	string getFilterType();
	int getOrder();
	string getFilterInfo();
	int getSamplingFreq();
	float* getACoeffs();
	float* getBCoeffs();
	int getAlen();
	int getBlen();
	string getFilepath();



};

#endif /* CFILEFILTER_H_ */
