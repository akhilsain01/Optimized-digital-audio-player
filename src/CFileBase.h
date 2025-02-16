/**
 * \file CFileBase.h
 *
 * \brief interface class CFileBase
 * \date 17.11.2023
 * \author A. Wirth <antje.wirth@h-da.de>
 * \author H. Frank <holger.frank@h-da.de>
 *
 * \see File
 */
#ifndef FILEBASE_H_
#define FILEBASE_H_

#include <string>
using namespace std;

/**
 * \brief attributes and methods for common text file operations
 *
 * uses C functions like fopen() etc. and not the C++ classes like
 * fstream()
 *
 * supports text files only (file access mode)
 */
class CFileBase {
public:
	/**
	 * \brief errors occurred during file operations
	 */
	enum FILEERROR {
		E_UNKNOWNOPENMODE,
		E_NOFILE,
		E_FILENOTOPEN,
		E_NOBUFFER,
		E_READ,
		E_CANTREAD,
		E_CANTWRITE,
		E_WRITE,
		E_SPECIAL
	};

private:
	/**
	 * \brief access mode of file object instance
	 *
	 * the validation has to be implemented in open() of the derived class
	 *
	 * + can optionally be appended for update mode
	 *
	 * binary mode (suffix b) is not supported
	 */
	string m_mode;

protected:
	/**
	 * \brief full path of the file
	 *
	 * the path can be absolute or relative to the location of the exe file
	 *
	 * all methods of the class operate either with slash (LINUX/MacOS) or
	 * backslash (WINDOWS)
	 */
	string m_path;

public:
	/**
	 * \brief initializes attributes of instance
	 *
	 * validates mode parameter
	 *
	 * \param path full path of the file
	 * \param mode file access mode
	 *
	 * \exception
	 * - invalid access mode
	 */
	CFileBase(const string& path, const string& mode = "r");

	/**
	 * \brief currently no function
	 */
	virtual ~CFileBase();

	/**
	 * \brief prints info of the instance on console
	 *
	 * - instance class
	 * - access mode
	 * - file path
	 */
	virtual void print(void);

protected:
	/**
	 * \brief check if file has write access
	 *
	 * \return true: write access, false: no write access
	 */
	bool isFileW();
	/**
	 * \brief check if file has read access
	 *
	 * \return true: read access, false: no read access
	 */
	bool isFileR();
	/**
	 * \brief check if file has write append access
	 *
	 * \return true: write append access, false: no write append access
	 */
	bool isFileWA();
	/**
	 * \brief gets last error as printable text
	 *
	 * \param err number of last error occurred
	 * \return error text
	 */
	string getErrorTxt(FILEERROR err);
	/**
	 * \brief gets file access mode as printable text
	 * \return access mode text
	 */
	string getModeTxt();
};
#endif /* FILEBASE_H_ */
