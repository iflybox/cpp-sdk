/** 
 * @file	qisv.h
 * @brief   iFLY Speech Verifier Header File
 * 
 *  This file contains the quick application programming interface (API) declarations 
 *  of ISV. Developer can include this file in your project to build applications.
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
 * @version	1.0
 * @date	2009/11/27
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2009/11/27	<td>jdyu	<td>Create this file</tr>
  * <tr> <td>2.0		<td>2010/11/10	<td>msp20	<td>modify this file</tr>
 * </table>
 * 
 */
#ifndef __QISV_H__
#define __QISV_H__

#ifdef __cplusplus
extern "C" {
#endif /* C++ */

#include "msp_types.h"

/** 
 * @fn		QISVSessionBegin
 * @brief	Begin a Verifier Session
 * 
 *  Create a verifier session to enroll, delete, verify or identify voice-print(s).
 * 
 * @return	return sessionID of current session, NULL is failed.
 * @param	const char* params		- [in] parameters when the session created.
 * @param	int *errorCode			- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char* MSPAPI QISVSessionBegin(const char* params, int *errorCode);
typedef const char* (MSPAPI *Proc_QISVSessionBegin)(const char* params, int *errorCode);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI QISVSessionBeginW(const wchar_t* params, int *errorCode);
typedef const wchar_t* (MSPAPI *Proc_QISVSessionBeginW)(const wchar_t* params, int *errorCode);
#endif

/** 
 * @fn		QISVUserEnroll
 * @brief	User Model Enrollment
 * 
 *  Enroll a user model
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] the session id
 * @param	const char* userId		- [in] User id to enrollment
 * @param	const char* params		- [in] parameters
 * @see		
 */
int MSPAPI QISVUserEnroll(const char* sessionID, const char* userId, const char* params);
typedef int (MSPAPI *Proc_QISVUserEnroll)(const char* sessionID, const char* userId, const char* params);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QISVUserEnrollW(const wchar_t* sessionID, const wchar_t* userId, const wchar_t* params);
typedef int (MSPAPI *Proc_QISVUserEnrollW)(const wchar_t* sessionID, const wchar_t* userId, const wchar_t* params);
#endif

/** 
 * @fn		QISVUserUnenroll
 * @brief	User Model Unenroll
 * 
 *  delete a user model
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] the session id
 * @param	const char* userId		- [in] User id to enrollment
 * @see		
 */
int MSPAPI QISVUserUnenroll(const char* sessionID, const char* userId);
typedef int (MSPAPI *Proc_QISVUserUnenroll)(const char* sessionID, const char* userId);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QISVUserUnenrollW(const wchar_t* sessionID, const wchar_t* userId);
typedef int (MSPAPI *Proc_QISVUserUnenrollW)(const wchar_t* sessionID, const wchar_t* userId);
#endif

/** 
 * @fn		QISVUserVerify
 * @brief	User Verify
 * 
 *  Verify a user.
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] the session id
 * @param	const char* userId		- [in] User id to verify
 * @param	const char* params		- [in] parameters
 * @see		
 */
int MSPAPI QISVUserVerify(const char* sessionID, const char* userId, const char* params);
typedef int (MSPAPI *Proc_QISVUserVerify)(const char* sessionID, const char* userId, const char* params);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QISVUserVerifyW(const wchar_t* sessionID, const wchar_t* userId, const wchar_t* params);
typedef int (MSPAPI *Proc_QISVUserVerifyW)(const wchar_t* sessionID, const wchar_t* userId, const wchar_t* params);
#endif

/** 
 * @fn		QISVUserIdentify
 * @brief	User Model Enrollment
 * 
 *  Identify the user from a list.
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] the session id
 * @param	const char* userIds		- [in] User id list
 * @param	const char* params		- [in] parameters
 * @see		
 */
int MSPAPI QISVUserIdentify(const char* sessionID, const char* userIds, const char* params);
typedef int (MSPAPI *Proc_QISVUserIdentify)(const char* sessionID, const char* userIds, const char* params);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QISVUserIdentifyW(const wchar_t* sessionID, const wchar_t* userIds, const wchar_t* params);
typedef int (MSPAPI *Proc_QISVUserIdentifyW)(const wchar_t* sessionID, const wchar_t* userIds, const wchar_t* params);
#endif

/** 
 * @fn		QISVAudioWrite
 * @brief	Write Audio Data to Verifier Session
 * 
 *  Writing binary audio data to verifier.
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] The session id returned by recog_begin
 * @param	const void* waveData	- [in] Binary data of waveform
 * @param	unsigned int waveLen	- [in] Waveform data size in bytes
 * @param	int audioStatus			- [in] Audio status, can be 
 * @param	int *epStatus			- [out] ISRepState
 * @param	int *svStatus			- [out] ISRrecRecognizerStatus, see isr_rec.h
 * @see		
 */
int MSPAPI QISVAudioWrite(const char* sessionID, const void* waveData, unsigned int waveLen, int audioStatus, int *epStatus, int *svStatus);
typedef int (MSPAPI *Proc_QISVAudioWrite)(const char* sessionID, const void* waveData, unsigned int waveLen, int audioStatus, int *epStatus, int *svStatus);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QISVAudioWriteW(const wchar_t* sessionID, const void* waveData, unsigned int waveLen, int audioStatus, int *epStatus, int *verStatus);
typedef int (MSPAPI *Proc_QISVAudioWriteW)(const wchar_t* sessionID, const void* waveData, unsigned int waveLen, int audioStatus, int *epStatus, int *svStatus);
#endif

/** 
 * @fn		QISVGetResult
 * @brief	Get verifier Result in Specified Format
 * 
 *  Get verifier result in specified format.
 * 
 * @return	int svResult - verifier result string in specified format.
 * @param	const char* sessionID	- [in] session id returned by session begin
 * @param	int *errorCode			- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char* MSPAPI QISVGetResult(const char* sessionID, int* errorCode);
typedef const char* (MSPAPI *Proc_QISVGetResult)(const char* sessionID, int* errorCode);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI QISVGetResultW(const wchar_t* sessionID, int* errorCode);
typedef const wchar_t* (MSPAPI *Proc_QISVGetResultW)(const wchar_t* sessionID, int* errorCode);
#endif

/** 
 * @fn		QISVSessionEnd
 * @brief	End a Verifier Session
 * 
 *  End the verifier session, release all resource.
 * 
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* sessionID	- [in] session id string
 * @param	const char* hints		- [in] user hints to end session, hints will be logged to CallLog
 * @see		
 */
int MSPAPI QISVSessionEnd(const char* sessionID, const char* hints);
typedef int (MSPAPI *Proc_QISVSessionEnd)(const char* sessionID, const char* hints);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QISVSessionEndW(const wchar_t* sessionID, const wchar_t* hints);
typedef int (MSPAPI *Proc_QISVSessionEndW)(const wchar_t* sessionID, const wchar_t* hints);
#endif

/** 
 * @fn		QISVDownloadData
 * @brief	Download User Specific Data
 * 
 *  Download data such as password text, etc.
 * 
 * @return	const void* MSPAPI		- Return buffer of downloaded data, size retued by varible dataLen.
 * @param	const char* sessionID	- [in] session id returned by session begin,if without session, set NULL
 * @param	const char* dataName	- [in] data name,should be unique to diff other data.
 * @param	unsigned int* dataLen	- [out] length of received data.
 * @param	int* errorCode			- [out] error code if failed, 0 to success.
 * @see		
 */
const void* MSPAPI QISVDownloadData(const char* sessionID, const char* dataName, unsigned int* dataLen, int* errorCode);
typedef const void* (MSPAPI* Proc_QISVDownloadData)(const char* sessionID, const char* dataName, unsigned int* dataLen, int* errorCode);
#ifdef MSP_WCHAR_SUPPORT
const void* MSPAPI QISVDownloadDataW(const wchar_t* sessionID, const wchar_t* dataName, unsigned int* dataLen, int* errorCode);
typedef const void* (MSPAPI* Proc_QISVDownloadDataW)(const wchar_t* sessionID, const wchar_t* dataName, unsigned int* dataLen, int* errorCode);
#endif

/** 
 * @fn		QISVWaveformEnroll
 * @brief	Enroll a Waveform File with UserId
 * 
 *  Enroll voice-print using waveform file and return result
 * 
 * @return	const char* svResult	- Return the result string of enrollment
 * @param	const char* sessionID	- [in] session id string
 * @param	const char* waveFile	- [in] waveform file uri to enroll voice-print
 * @param	const char* waveFmt		- [in] waveform format string
 * @param	const char* userId		- [in] User id to enroll
 * @param	const char* params		- [in] parameters
 * @param	int *enrollStatus		- [out] enrollment status
 * @param	int *errorCode			- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char* MSPAPI QISVWaveformEnroll(const char* sessionID, const char* waveFile, const char* waveFmt, const char* userId, const char* params, int *enrollStatus, int *errorCode);
typedef const char* (MSPAPI *Proc_QISVWaveformEnroll)(const char* sessionID, const char* waveFile, const char* waveFmt, const char* userId, const char* params, int *recogStatus, int *errorCode);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI QISVWaveformEnrollW(const wchar_t* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* userId, const wchar_t* params, int *recogStatus, int *errorCode);
typedef const wchar_t* (MSPAPI *Proc_QISVWaveformEnrollW)(const wchar_t* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* userId, const wchar_t* params, int *recogStatus, int *errorCode);
#endif

/** 
 * @fn		QISVWaveformVerify
 * @brief	Verify a Waveform File with UserId
 * 
 *  Verify voice-print using waveform file and return result
 * 
 * @return	const char* svResult	- Return the result string of verification
 * @param	const char* sessionID	- [in] session id string
 * @param	const char* waveFile	- [in] waveform file uri to verify
 * @param	const char* waveFmt		- [in] waveform format string
 * @param	const char* userId		- [in] user id to verify
 * @param	const char* params		- [in] parameters
 * @param	int *verifyStatus		- [out] verify status
 * @param	int *errorCode			- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char* MSPAPI QISVWaveformVerify(const char* sessionID, const char* waveFile, const char* waveFmt, const char* userId, const char* params, int *verifyStatus, int *errorCode);
typedef const char* (MSPAPI *Proc_QISVWaveformVerify)(const char* sessionID, const char* waveFile, const char* waveFmt, const char* userId, const char* params, int *verifyStatus, int *errorCode);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI QISVWaveformVerifyW(const char* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* userId, const wchar_t* params, int *verifyStatus, int *errorCode);
typedef const wchar_t* (MSPAPI *Proc_QISVWaveformVerifyW)(const char* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* userId, const wchar_t* params, int *verifyStatus, int *errorCode);
#endif

/** 
 * @fn		QISVWaveformIdentify
 * @brief	Identify a Waveform File with UserIds
 * 
 *  Identify a waveform file and return result
 * 
 * @return	const char* svResult	- Return the result string of idenfication
 * @param	const char* waveFile	- [in] waveform file uri to identify
 * @param	const char* waveFmt		- [in] waveform format string
 * @param	const char* userIds		- [in] user ids to verify
 * @param	const char* params		- [in] parameters
 * @param	int *identifyStatus		- [out] identify status
 * @param	int *errorCode			- [out] return 0 on success, otherwise return error code.
 * @see		
 */
const char* MSPAPI QISVWaveformIdentify(const char* sessionID, const char* waveFile, const char* waveFmt, const char* userIds, const char* params, int *identifyStatus, int *errorCode);
typedef const char* (MSPAPI *Proc_QISVWaveformIdentify)(const char* sessionID, const char* waveFile, const char* waveFmt, const char* userIds, const char* params, int *identifyStatus, int *errorCode);
#ifdef MSP_WCHAR_SUPPORT
const wchar_t* MSPAPI QISVWaveformIdentifyW(const char* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* userIds, const wchar_t* params, int *identifyStatus, int *errorCode);
typedef const wchar_t* (MSPAPI *Proc_QISVWaveformIdentifyW)(const char* sessionID, const wchar_t* waveFile, const wchar_t* waveFmt, const wchar_t* userIds, const wchar_t* params, int *identifyStatus, int *errorCode);
#endif

/*
 * Initialize and fini, these functions is optional.
 * To call them in some cases necessarily.
 */

/** 
 * @fn		QISVInit
 * @brief	Initialize API
 * 
 *  Load API module with specified configurations.
 * 
 * @date	2009/11/26
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* configs	- [in] configurations to initialize
 * @see		
 */
int MSPAPI QISVInit(const char* configs);
typedef int (MSPAPI *Proc_QISVInit)(const char* configs);
#ifdef MSP_WCHAR_SUPPORT
int MSPAPI QISVInitW(const wchar_t* configs);
typedef int (MSPAPI *Proc_QISVInitW)(const wchar_t* configs);
#endif

/** 
 * @fn		QISVFini
 * @brief	Uninitialize API
 * 
 *  Unload API module, the last function to be called.
 * 
 * @author	jdyu
 * @date	2009/11/26
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @see		
 */
int MSPAPI QISVFini(void);
typedef int (MSPAPI *Proc_QISVFini)(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* C++ */

#endif /* __QISV_H__ */