/**
 * @file    qhcr.h
 * @brief   iFLY Speech Recognizer Header File
 * 
 *  This file contains the quick application programming interface (API) declarations 
 *  of ISR. Developer can include this file in your project to build applications.
 *  For more information, please read the developer guide.
 
 *  Use of this software is subject to certain restrictions and limitations set
 *  forth in a license agreement entered into between iFLYTEK, Co,LTD.
 *  and the licensee of this software.  Please refer to the license
 *  agreement for license use rights and restrictions.
 *
 *  Copyright (C)    1999 - 2007 by ANHUI USTC iFLYTEK, Co,LTD.
 *                   All rights reserved.
 * 
 * @author  Speech Dept. iFLYTEK.
 * @version 1.0
 * @date    2010/09/14
 * 
 * @see        
 * 
 * History:
 * index    version        date            author        notes
 * 0        1.0            2010/09/14      Speech        Create this file
 */

#ifndef __QHCR_H__
#define __QHCR_H__

#ifdef __cplusplus
extern "C" 
{
#endif /* C++ */

#include "msp_types.h"

/** 
 * @fn		QHCRSessionBegin
 * @brief	Begin a Recognizer Session
 * 
 *  Create a recognizer session to recognize audio data
 * 
 * @return	return sessionID of current session, NULL is failed.
 * @param	const char* grammarList		- [in] grammars list, inline grammar support only one.
 * @param	const char* params			- [in] parameters when the session created.
 * @param	int *errorCode				- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char* MSPAPI QHCRSessionBegin(const char* params, int* errorCode);
typedef const char* (MSPAPI *Proc_QHCRSessionBegin)(const char* params, int* errorCode);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI QHCRSessionBeginW(const wchar_t* params, int* errorCode);
typedef const wchar_t* (MSPAPI *Proc_QHCRSessionBeginW)(const wchar_t* params, int* errorCode);
#endif

/** 
 * @fn		QHCRDataWrite
 * @brief	Write Audio Data to Recognizer Session
 * 
 *  Writing binary audio data to recognizer.
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] The session id returned by recog_begin
 * @param	const void* waveData	- [in] Binary data of waveform
 * @param	unsigned int waveLen	- [in] Waveform data size in bytes
 * @param	int audioStatus			- [in] Audio status, can be 
 * @param	int *epStatus			- [out] ISRepState
 * @param	int *recogStatus		- [out] ISRrecRecognizerStatus, see isr_rec.h
 * @see		
 */
int MSPAPI QHCRDataWrite(const char* sessionID, const char* params, const void* data, unsigned int dataLen, int dataStatus);
typedef int (MSPAPI *Proc_QHCRDataWrite)(const char* sessionID, const char* params, const void* data, unsigned int dataLen, int dataStatus);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QHCRDataWriteW(const wchar_t* sessionID, const void* data, unsigned int dataLen, int dataStatus);
typedef int (MSPAPI *Proc_QHCRDataWriteW)(const wchar_t* sessionID, const void* data, unsigned int dataLen, int dataStatus);
#endif

/** 
 * @fn		QHCRGetResult
 * @brief	Get Recognize Result in Specified Format
 * 
 *  Get recognize result in Specified format.
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] session id returned by session begin
 * @param	int* rsltStatus			- [out] status of recognition result, 0: success, 1: no match, 2: incomplete, 5:speech complete
 * @param	int *errorCode			- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char * MSPAPI QHCRGetResult(const char* sessionID, const char* params, int* rsltStatus, int *errorCode);
typedef const char * (MSPAPI *Proc_QHCRGetResult)(const char* sessionID, const char* params, int* rsltStatus, int *errorCode);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI QHCRGetResultW(const wchar_t* sessionID, int* rsltStatus, int *errorCode);
typedef const wchar_t* (MSPAPI *Proc_QHCRGetResultW)(const wchar_t* sessionID, int* rsltStatus, int *errorCode);
#endif

/** 
 * @fn		QHCRSessionEnd
 * @brief	End a Recognizer Session
 * 
 *  End the recognizer session, release all resource.
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] session id string to end
 * @param	const char* hints	- [in] user hints to end session, hints will be logged to CallLog
 * @see		
 */
int MSPAPI QHCRSessionEnd(const char* sessionID, const char* hints);
typedef int (MSPAPI *Proc_QHCRSessionEnd)(const char* sessionID, const char* hints);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QHCRSessionEndW(const wchar_t* sessionID, const wchar_t* hints);
typedef int (MSPAPI *Proc_QHCRSessionEndW)(const wchar_t* sessionID, const wchar_t* hints);
#endif

/** 
 * @fn		QHCRWaveformRecog
 * @brief	Recognize a Waveform File
 * 
 *  Recognize a waveform file and return XML result
 * 
 * @return	const char*	- Return rec result in string format, NULL if failed, result is error code.
 * @param	const char* waveFile	- [in] waveform file uri to recognize
 * @param	const char* waveFmt		- [in] waveform format string
 * @param	const char* grammarList	- [in] grammars used by recognizer
 * @param	const char* params		- [in] parameters
 * @param	int *recogStatus		- [out] recognize status
 * @param	int *result				- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char* MSPAPI QHCRWaveformRecog(const char* sessionID, const char* waveFile, const char* waveFmt, const char* grammarList, const char* params, int *recogStatus, int *result);
typedef const char* (MSPAPI *Proc_QHCRWaveformRecog)(const char* sessionID, const char* waveFile, const char* waveFmt, const char* grammarList, const char* params, int *recogStatus, int *result);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI QHCRWaveformRecogW(const wchar_t* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* grammarList, const wchar_t* params, int *recogStatus, int *result);
typedef const wchar_t* (MSPAPI *Proc_QHCRWaveformRecogW)(const wchar_t* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* grammarList, const wchar_t* params, int *recogStatus, int *result);
#endif

/*
 * Initialize and fini, these functions is optional.
 * To call them in some cases necessarily.
 */

/** 
 * @fn		QHCRInit
 * @brief	Initialize API
 * 
 *  Load API module with specified configurations.
 * 
 * @date	2009/11/26
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* configs	- [in] configurations to initialize
 * @see		
 */
int MSPAPI QHCRInit(const char* configs);
typedef int (MSPAPI *Proc_QHCRInit)(const char* configs);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QHCRInitW(const wchar_t* configs);
typedef int (MSPAPI *Proc_QHCRInitW)(const wchar_t* configs);
#endif

/** 
 * @fn		QHCRFini
 * @brief	Uninitialize API
 * 
 *  Unload API module, the last function to be called.
 * 
 * @author	jdyu
 * @date	2009/11/26
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @see		
 */
int MSPAPI QHCRFini(void);
typedef int (MSPAPI *Proc_QHCRFini)(void);

/*
 * Internal interface for debugging use.
 * To call them in some cases necessarily.
 */

/** 
 * @fn		QHCRLogEvent
 * @brief	Log user events to ISR call-logging
 * 
 *  Logging user events to ISR call-logging, useful to tag user's comments.
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] session id returned by session begin
 * @param	const char *event		- [in] event name.
 * @param	const char *value		- [in] event message string.
 * @see		
 */
int MSPAPI QHCRLogEvent(const char* sessionID, const char* event, const char* value);
typedef int (MSPAPI *Proc_QHCRLogEvent)(const char* sessionID, const char* event, const char* value);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QHCRLogEventW(const wchar_t* sessionID, const wchar_t* event, const wchar_t* value);
typedef int (MSPAPI *Proc_QHCRLogEventW)(const wchar_t* sessionID, const wchar_t* event, const wchar_t* value);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif /* C++ */

#endif /* __QHCR_H__ */
