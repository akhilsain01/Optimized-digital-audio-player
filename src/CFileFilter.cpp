/*
 * CFileFilter.cpp
 *
 *  Created on: Feb 6, 2025
 *      Author: Akhil Sai Nallapati
 */

#include "CFileFilter.h"
#include "CFileBase.h"
#include "SKSLib.h"
#include <iomanip>

CFileFilter::CFileFilter(const string path, const string mode) :
                           CFileBase(path,mode){
	// TODO Auto-generated constructor stub
	m_pFile=NULL;
    m_order=0;
    m_fs=0;
    m_a=NULL;
    m_b=NULL;
    blen=0;
    alen=0;

}

CFileFilter::~CFileFilter() {
	// TODO Auto-generated destructor stub
}

void CFileFilter::open(){
	m_pFile= fopen(m_path.c_str(),"r");
	if(m_pFile==NULL)
		throw CException(CException::SRC_File,-1,"Unable to open file");
}
void CFileFilter::print() {

	CFileBase::print();
	if (m_b != NULL && m_a != NULL) {
		cout << "filter type :" << m_type << "\t Sampling freq :" << m_fs
				<< "\t filter order :" << m_order << endl;
		cout << "filter info :" << m_info << endl;
		cout << fixed << setprecision(8);
		cout << "b coeffs :";
		for (int i = 0; i < m_order + 1; i++) {
			cout << m_b[i] << "\t";
		}
		cout << endl;
		cout << "a coeffs :";
		for (int i = 0; i < m_order + 1; i++) {
			cout << m_a[i] << "\t";
		}
		cout << endl;
	} else
		throw CException(CException::SRC_File, -1,
				" fs not available , so NO coeffecients available");
}

void CFileFilter::close(){
    if(m_a!=NULL&&m_b!=NULL){
    	delete m_a;   // this NULL hepls in overcoming crashes
    	m_a=NULL;
    	delete m_b;
    	m_a=NULL;
    }
	fclose(m_pFile);
}
long CFileFilter::read(int fs){
    if(!fs)
    return 0;
//    	throw CException(CException::SRC_File,-1,"No sampling frequency available");

    m_fs=fs;
    fseek(m_pFile,0,SEEK_SET);

    int sbufSize=360;
    char sbuf[sbufSize];
    int end=0,pos=0;
    fgets(sbuf,sbufSize,m_pFile);
    string s=sbuf;
    end=s.find(";",pos);
    m_type=s.substr(pos,end-pos);
    pos=end+1;
    end=s.find(";",pos);
    string order=s.substr(pos,end-pos);
    m_order=stoi(order);
    pos=end+1;
    m_info=s.substr(pos);

    int fsr=0,i=0;
    while(NULL!=fgets(sbuf,sbufSize,m_pFile)){
    	fsr=stoi(sbuf);
		if (fsr != m_fs) {
			char *pgot;
			while (pgot) {                          //skip b coeffecients
				pgot = fgets(sbuf, sbufSize, m_pFile);
				if (NULL != strrchr(sbuf, '\n'))
					break;
			}
			while (pgot) {                          //skip a coeffecients
				pgot = fgets(sbuf, sbufSize, m_pFile);
				if (NULL != strrchr(sbuf, '\n'))
					break;
			}
    	}
		else{
			m_b=new float[m_order+1];
			m_a=new float[m_order+1];
			char sep;
			for (i = 0; i < m_order + 1; i++) {
				if (EOF == fscanf(m_pFile, "%f%c", &m_b[i], &sep))
					break;
				if (sep == '\n')
					break;
			}
			blen=i;
			if(sep!='\n')
				fscanf(m_pFile,"%c",&sep);
			for (i = 0; i < m_order + 1; i++) {
				if (EOF == fscanf(m_pFile, "%f%c", &m_a[i], &sep))
					break;
				if (sep == '\n')
					break;
			}
			alen=i;
			if(sep!='\n')
				fscanf(m_pFile,"%c",&sep);

		return ftell(m_pFile);
		}
	}
  return 0;

}

float* CFileFilter::getACoeffs(){
	if(m_a==NULL)
		throw CException(CException::SRC_File,-1,"NO a filter coeffectients avilable ");

	return m_a;
}
float* CFileFilter::getBCoeffs(){
	if(m_b==NULL)
		throw CException(CException::SRC_File,-1,"NO b filter coeffectients avilable ");

	return m_b;
}

string CFileFilter::getFilterInfo(){
	if(m_info=="")
			throw CException(CException::SRC_File,-1,"Filter info not avilable, please open file ");

	return m_info;
}

string CFileFilter::getFilterType(){
	if(m_type=="")
			throw CException(CException::SRC_File,-1,"type not avilable, please open file ");

	return m_type;
}

int CFileFilter::getAlen(){
	if(m_a==NULL)
			throw CException(CException::SRC_File,-1,"NO a filter coeffectients avilable ");

	return alen;
}
int CFileFilter::getBlen(){
	if(m_b==NULL)
			throw CException(CException::SRC_File,-1,"NO b filter coeffectients avilable ");

	return blen;
}
int CFileFilter::getOrder(){
	if(m_order==0)
			throw CException(CException::SRC_File,-1,"No order, please open file ");

	return m_order;
}

int CFileFilter::getSamplingFreq(){
	if(m_fs==0)
			throw CException(CException::SRC_File,-1,"fs not avilable, please open file ");
	return m_fs;
}

string CFileFilter::getFilepath() {
	return m_path;
}
