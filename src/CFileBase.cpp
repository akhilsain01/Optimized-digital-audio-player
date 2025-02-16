/**
 * \file CFileBase.cpp
 *
 * \brief implementation class CFileBase
 * \date 17.11.2023
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 *
 * \see File
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
using namespace std;

#include <SKSLib.h>
#include "CFileBase.h"

/*
 * Constructor
 */
CFileBase::CFileBase(const string &path, const string &mode) {
	m_mode = "";		// default unknown mode
	m_path = "";

	if ((mode == "r") || (mode == "w") || (mode == "w+")) {
		m_mode = mode;
		m_path = path;
	} else {
		throw CException(this, typeid(this).name(), __FUNCTION__, 0,
				"invalid file access mode '" + mode + "'");
	}
//	cout << "CFileBase@" << hex << this << dec << " created" << endl;
}

CFileBase::~CFileBase() {
//	cout << "CFileBase@" << hex << this << dec << " destroyed" << endl;
}

void CFileBase::print(void) {
//	cout << "CFileBase@" << hex << this << dec << " [" << getModeTxt() << "]: "
//			<< m_path << endl;
}

/*
 * utility methods to get the open mode
 */
bool CFileBase::isFileW() {
	if (m_mode != "w")
		return false;
	return true;
}

bool CFileBase::isFileR() {
	if (m_mode != "r")
		return false;
	return true;
}

bool CFileBase::isFileWA() {
	if (m_mode != "w+")
		return false;
	return true;
}

string CFileBase::getErrorTxt(FILEERROR err) {
	switch (err) {
	case E_UNKNOWNOPENMODE:
		return string("unknown file open mode");
	case E_NOFILE:
		return string("file not found");
	case E_FILENOTOPEN:
		return string("file not open");
	case E_NOBUFFER:
		return string("no data buffer available");
	case E_CANTREAD:
		return string("file has been opened in write only mode");
	case E_READ:
		return string("error during read");
	case E_CANTWRITE:
		return string("file has been opened in read only mode");
	case E_WRITE:
		return string("error during write");
	case E_SPECIAL:
		return string("file error: ");
	default:
		return string("unknown error");
	}
}

// here, we may have a combination of different modes
string CFileBase::getModeTxt() {
	string sret = "unknown mode";
	if (m_mode == "r")
		sret = "read mode";
	if (m_mode == "w") {
		sret = "write mode";
	}
	if (m_mode == "w+") {
		sret = "write/append mode";
	}
	return sret;
}
