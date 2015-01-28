/**
 * @file    msp_common.h
 * @brief   Mobile Speech Platform Common Interface Header File
 * 
 *  This file contains the quick common programming interface (API) declarations 
 *  of MSP. Developer can include this file in your project to build applications.
 *  For more information, please read the developer guide.
 
 *  Use of this software is subject to certain restrictions and limitations set
 *  forth in a license agreement entered into between iFLYTEK, Co,LTD.
 *  and the licensee of this software.  Please refer to the license
 *  agreement for license use rights and restrictions.
 *
 *  Copyright (C)    1999 - 2011 by ANHUI USTC iFLYTEK, Co,LTD.
 *                   All rights reserved.
 * 
 * @author  Speech Dept. iFLYTEK.
 * @version 1.0
 * @date    2011/07/25
 * 
 * @see        
 * 
 * History:
 * index    version        date            author        notes
 * 0        1.0            2011/07/25      MSP30        Create this file
 */

#ifndef __MSP_COMMON_H__
#define __MSP_COMMON_H__

#include "msp_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* C++ */

/** 
 * @fn		MSPLogin
 * @brief	user login interface
 * 
 *  User login.
 * 
 * @return	int MSPAPI			- Return 0 in success, otherwise return error code.
 * @param	const char* usr		- [in] user name.
 * @param	const char* pwd		- [in] password.
 * @param	const char* params	- [in] parameters when user login.
 * @see		
 */
int MSPAPI MSPLogin(const char* usr, const char* pwd, const char* params);
typedef int (MSPAPI *Proc_MSPLogin)(const char* usr, const char* pwd, const char* params);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI MSPLoginW(const wchar_t* usr, const wchar_t* pwd, const wchar_t* params);
typedef int (MSPAPI *Proc_MSPLoginW)(const wchar_t* usr, const wchar_t* pwd, const wchar_t* params);
#endif

/** 
 * @fn		MSPLogout
 * @brief	user logout interface
 * 
 *  User logout
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @see		
 */
int MSPAPI MSPLogout();
typedef int (MSPAPI *Proc_MSPLogout)();
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI MSPLogoutW();
typedef int (MSPAPI *Proc_MSPLogoutW)();
#endif

/** 
 * @fn		MSPUploadData
 * @brief	Upload User Specific Data
 * 
 *  Upload data such as user config, custom grammar, etc.
 * 
 * @return	const char* MSPAPI		- data id returned by Server, used for special command.
 * @param	const char* dataName	- [in] data name, should be unique to diff other data.
 * @param	void* data				- [in] the data buffer pointer, data could be binary.
 * @param	unsigned int dataLen	- [in] length of data.
 * @param	const char* params		- [in] parameters about uploading data.
 * @param	int* errorCode			- [out] Return 0 in success, otherwise return error code.
 * @see		
 */
const char* MSPAPI MSPUploadData(const char* dataName, void* data, unsigned int dataLen, const char* params, int* errorCode);
typedef const char* (MSPAPI* Proc_MSPUploadData)(const char* dataName, void* data, unsigned int dataLen, const char* params, int* errorCode);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI MSPUploadDataW(const wchar_t* dataName, void* data, unsigned int dataLen, const wchar_t* params, int* errorCode);
typedef const wchar_t* (MSPAPI* Proc_MSPUploadDataW)(const wchar_t* dataName, void* data, unsigned int dataLen, const wchar_t* params, int* errorCode);
#endif

/** 
 * @fn		MSPDownloadData
 * @brief	Download User Specific Data
 * 
 *  Download data such as user config, etc.
 * 
 * @return	const void*	MSPAPI		- received data buffer pointer, data could be binary, NULL if failed or data does not exsit.
 * @param	const char* params		- [in] parameters about data to be downloaded.
 * @param	unsigned int* dataLen	- [out] length of received data.
 * @param	int* errorCode			- [out] Return 0 in success, otherwise return error code.
 * @see		
 */
const void* MSPAPI MSPDownloadData(const char* params, unsigned int* dataLen, int* errorCode);
typedef const void* (MSPAPI* Proc_MSPDownloadData)(const char* params, unsigned int* dataLen, int* errorCode);
#ifdef MSP_WCHAR_SUPPORT
const void* MSPAPI MSPDownloadDataW(const wchar_t* params, unsigned int* dataLen, int* errorCode);
typedef const void* (MSPAPI* Proc_MSPDownloadDataW)(const wchar_t* params, unsigned int* dataLen, int* errorCode);
#endif

/** 
 * @fn		MSPSearch
 * @brief	Search text for result
 * 
 *  Search text content, and got text result
 * 
 * @return	const void*	MSPAPI		- received data buffer pointer, data could be binary, NULL if failed or data does not exsit.
 * @param	const char* params		- [in] parameters about data to be downloaded.
 * @param	unsigned int* dataLen	- [out] length of received data.
 * @param	int* errorCode			- [out] Return 0 in success, otherwise return error code.
 * @see		
 */
const char* MSPAPI MSPSearch(const char* params, const char* text, unsigned int* dataLen, int* errorCode);
typedef const char* (MSPAPI* Proc_MSPSearch)(const char* params, const char* text, unsigned int* dataLen, int* errorCode);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI MSPSearchW(const wchar_t* params, const wchar_t* text, unsigned int* dataLen, int* errorCode);
typedef const wchar_t* (MSPAPI* Proc_MSPSearchW)(const wchar_t* params, const wchar_t* text, unsigned int* dataLen, int* errorCode);
#endif

#ifdef __cplusplus
} /* extern "C" */	
#endif /* C++ */

#endif /* __MSP_COMMON_H__ */
