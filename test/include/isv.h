/**
* @file    isv.h
* @brief   iFLY Speaker Verifier Header File
* 
*  This file contains Speaker Verifier API declarations 
*  of ISV. Developer can include this file in your project to build applications.
*  For more information, please read the developer guide.

*  Use of this software is subject to certain restrictions and limitations set
*  forth in a license agreement entered into between iFLYTEK, Co,LTD.
*  and the licensee of this software.  Please refer to the license
*  agreement for license use rights and restrictions.
*
*  Copyright (C)    1999 - 2008 by ANHUI USTC iFLYTEK, Co,LTD.
*                   All rights reserved.
* 
* @author  Speech Dept. iFLYTEK.
* @version 1.0
* @date    2008/10/07
* 
* @see        
* 
* History:
* index    version        date            author        notes
* 0        1.0            2008/10/07      Speech        Create this file
*/

#ifndef __ISV_H__
#define __ISV_H__

#include "isv_types.h"
#include "isv_errors.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The following are generic errors that should always be checked for:
 * ISV_ERROR_NOTINIT if ISVInitialize hasn't been called
 * ISV_ERROR_INVALID_PARA if any required parameters are NULL or
 *   zero-length strings, etc
 * ISV_ERROR_OUT_OF_MEMORY if a memory allocation failed
 * Other codes not in isv_error.h  if some
 *   non-memory-allocation OS request fails
 * ISV_ERROR_VD_DB_EXCEPTION database exception occur while data opreation
 *
 * ISV_ERROR_FAIL unspecified failure
 * ISV_ERROR_GENERAL unspecified error
 * ISV_ERROR_EXCEPTION unspecified fatal error
 */

/** 
 * @ brief ISVInitialize 
 *
 * Global initialization
 *    this should be invoked once at process start-up
 *    this function can only be called once per process
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_NO_LICENSE no license
 *  ISV_ERROR_BUSY already initialize
 *  ISV_ERROR_LOAD_MODULE load module failed  
 *  ISV_ERROR_RES_MISSING load resource failed
 *  ISV_ERROR_VD_INVALID_REPO invalid repository source
 *  ISV_ERROR_VD_INVALID_CONN_STR invalid connect string for specified repository
 *
 * @param const wchar_t * engine    -[in] The engine name or path to initialize
 *                                   if null, use default engine specified at install time
 * @param void * reserved           -[in,out] Reserved, must be null
 * @see
 */
int ISVAPI ISVInitialize(const wchar_t * engine, void * reserved);
typedef int (ISVAPI *Proc_ISVInitialize)(const wchar_t * engine, void *reserved);

/** 
 * @ brief ISVUninitialize 
 *
 * Global shutdown
 *    this should be invoked once at process start-up
 *    this function can only be called once per process
 *    this function must be called when process will be terminated
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_NOT_INIT ISVInitialize is not correctly called
 *
 * @param
 * @see
 */
int ISVAPI ISVUninitialize(void);
typedef int (ISVAPI *Proc_ISVUninitialize)(void);

/** 
 * @ brief ISVCreateUser 
 *
 * Create one user record
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_NO_LICENSE number of user already get license value
 *  ISV_ERROR_VD_USER_EXIST user already be created
 *  ISV_ERROR_VD_INVALID_ID_STR user id is illegal
 *
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @see
 */
int ISVAPI ISVCreateUser(const wchar_t * user_id);
typedef int (ISVAPI *Proc_ISVCreateUser)(const wchar_t * user_id);

/** 
 * @ brief ISVDeleteUser 
 *
 * Delete one user record
 *    this delete one user record
 *    this also delete models and model records which related deleted user
 *    this also delete features and feature records which related deleted model
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_VD_USER_NOT_EXIST user not exist
 *  ISV_ERROR_VD_INVALID_ID_STR user id is illegal
 *
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @see
 */
int ISVAPI ISVDeleteUser(const wchar_t * user_id);
typedef int (ISVAPI *Proc_ISVDeleteUser)(const wchar_t * user_id);

/** 
 * @ brief ISVClearUser 
 *
 * Clear one user record
 *    this delete models and reset model records with default value
 *    this delete features and feature records which related above models
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_VD_USER_NOT_EXIST user not exist
 *  ISV_ERROR_VD_INVALID_ID_STR user id is illegal
 *
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @see
 */
int ISVAPI ISVClearUser(const wchar_t * user_id);
typedef int (ISVAPI *Proc_ISVClearUser)(const wchar_t * user_id);

/** 
 * @ brief ISVCreateUserModel 
 *
 * Create one user model record
 *    if user not exist and auto create user is allowed in config, 
 *    it will automatically create a user record
 *    
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_NO_LICENSE number of user already get license value
 *  ISV_ERROR_VD_MODEL_EXIST model already exist
 *  ISV_ERROR_VD_INVALID_ID_STR model id or user id is illegal
 *
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @param const wchar_t * user_model_id -[in] A string that uniquely identifies the model for user
 * @see
 */
int ISVAPI ISVCreateUserModel(const wchar_t * user_id, const wchar_t * user_model_id);
typedef int (ISVAPI *Proc_ISVCreateUserModel)(const wchar_t * user_id, const wchar_t * user_model_id);

/** 
 * @ brief ISVDeleteUserModel 
 *
 * Delete one user model record
 *    this delete one user model and model record
 *    this also delete features and feature record which related the model    
 *    
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_VD_USER_NOT_EXIST user not exist
 *  ISV_ERROR_VD_MODEL_NOT_EXIST model not exist
 *  ISV_ERROR_VD_INVALID_ID_STR model id or user id is illegal
 *
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @param const wchar_t * user_model_id -[in] A string that uniquely identifies the model for user
 * @see
 */
int ISVAPI ISVDeleteUserModel(const wchar_t * user_id, const wchar_t * user_model_id);
typedef int (ISVAPI *Proc_ISVDeleteUserModel)(const wchar_t * user_id, const wchar_t * user_model_id);

/** 
 * @ brief ISVDeleteUserModel 
 *
 * Clear one user model record
 *    this delete one user model and reset model record with default value
 *    this also delete features and feature record which related the model    
 *    
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_VD_USER_NOT_EXIST user not exist
 *  ISV_ERROR_VD_MODEL_NOT_EXIST model not exist
 *  ISV_ERROR_VD_INVALID_ID_STR model id or user id is illegal
 *
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @param const wchar_t * user_model_id -[in] A string that uniquely identifies the model for user
 * @see
 */
int ISVAPI ISVClearUserModel(const wchar_t * user_id, const wchar_t * user_model_id);
typedef int (ISVAPI *Proc_ISVClearUserModel)(const wchar_t * user_id, const wchar_t * user_model_id);

/** 
 * @ brief ISVQueryModelInfo 
 *
 * Query user model information   
 *    
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_VD_USER_NOT_EXIST user not exist
 *  ISV_ERROR_VD_MODEL_NOT_EXIST model not exist
 *  ISV_ERROR_INVALID_PARA invalid parameter in filed_value
 *  ISV_ERROR_NO_ENOUGH_BUFFER input buffer filed_value is too small
 *
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @param const wchar_t * user_model_id -[in] A string that uniquely identifies the model for user
 * @param wchar_t * field_value     -[in,out] A string buffer which imported in the form of "vmExist;gender;..."
 *                                  and exported in the form of "vmExist=0;gender=0;..."
 * @param unsigned int * len        -[in,out] Length of field_value.if buffer is not enough, right size will be gave out
 * @see
 */
int ISVAPI ISVQueryModelInfo(const wchar_t * user_id, const wchar_t * user_model_id, wchar_t * field_value, unsigned int * len);
typedef int (ISVAPI *Proc_ISVQueryModelInfo)(const wchar_t * user_id, const wchar_t * user_model_id, wchar_t * field_value, unsigned int * len);

/** 
 * @ brief ISVEnumAllUser 
 *
 * Enum all users, regardless of the existence of the user's model
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *
 * @param ISV_EnumHandle enum_handle -[out] The result handle which can be converted to final string by ISVGetEnumString,
                                     the string's format¡ª¡ª¡°user_id1;user_id2;user_id3...¡±
 * @param unsigned int * user_count -[out] user count
 * @see
 */
int ISVAPI ISVEnumAllUser(ISV_EnumHandle * enum_handle, unsigned int * user_count);
typedef int (ISVAPI *Proc_ISVEnumAllUser)(ISV_EnumHandle * enum_handle, unsigned int * user_count);

/** 
 * @ brief ISVEnumAllMode
 *
 * Enum all model type, does not duplicate
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *
 * @param ISV_EnumHandle enum_handle -[out] The result handle which can be converted to final string by ISVGetEnumString,
                                     the string's format¡ª¡ª¡°model_id1;model_id2;model_id3...¡±
 * @param unsigned int * model_count -[out] model count
 * @see
 */
int ISVAPI ISVEnumAllModel(ISV_EnumHandle * enum_handle, unsigned int * model_count);
typedef int (ISVAPI *Proc_ISVEnumAllModel)(ISV_EnumHandle * enum_handle, unsigned int * model_count);

/** 
 * @ brief ISVEnumAllMode
 *
 * Enum all model type of one user
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_VD_USER_NOT_EXIST user not exist
 *  ISV_ERROR_VD_INVALID_ID_STR user id is illegal
 *
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @param ISV_EnumHandle enum_handle -[out] The result handle which can be converted to final string by ISVGetEnumString,
                                     the string's format¡ª¡ª¡°model_id1;model_id2;model_id3...¡±
 * @param unsigned int * model_count -[out] model count
 * @see
 */
int ISVAPI ISVEnumUserModel(const wchar_t * user_id, ISV_EnumHandle * enum_handle, unsigned int * model_count);
typedef int (ISVAPI *Proc_ISVEnumUserModel)(const wchar_t * user_id, ISV_EnumHandle * enum_handle, unsigned int * model_count);

/** 
 * @ brief ISVGetEnumString
 *
 * Convert enum handle to final result string
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_INVALID_HANDLE invalid enum handle
 *  ISV_ERROR_NULL_HANDLE null enum handle
 *
 * @param ISV_EnumHandle enum_handle -[in] Enum handle, the result of enum operation such as ISVEnumUserModel
 * @param const wchar_t ** enum_str -[out] Enum string, the real result of enum operation
 * @see
 */
int ISVAPI ISVGetEnumString(ISV_EnumHandle enum_handle, const wchar_t ** enum_str);
typedef int (ISVAPI *Proc_ISVGetEnumString)(ISV_EnumHandle enum_handle, const wchar_t ** enum_str);

/** 
 * @ brief ISVDestroyEnumHandle
 *
 * Destroy enum handle
 *    the handle which created by enum operation must be destroyed by this function,
 *    otherwise there will be handle leak
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_INVALID_HANDLE invalid enum handle
 *  ISV_ERROR_NULL_HANDLE null enum handle
 *
 * @param ISV_EnumHandle enum_handle -[in] Enum handle, the result of enum operation such as ISVEnumUserModel
 * @see
 */
int ISVAPI ISVDestroyEnumHandle(ISV_EnumHandle enum_handle);
typedef int (ISVAPI *Proc_ISVDestroyEnumHandle)(ISV_EnumHandle enum_handle);

/** 
 * @ brief ISVCreateInst
 *
 * Create a verifier resource
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_NO_LICENSE not license
 *
 * @param ISV_INST * isv_inst       -[out] Verifier handle
 * @param const wchar_t * params    -[in] Parameter list with format-¡°param1=value1,param2=value2...¡±,
 *                                  can be NULL
 * @see
 */
int ISVAPI ISVCreateInst(ISV_INST * isv_inst, const wchar_t * params);
typedef int (ISVAPI *Proc_ISVCreateInst)(ISV_INST * isv_inst, const wchar_t * params);

/** 
 * @ brief ISVDestroyInst
 *
 * Destroy and de-register a verifier resource.
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 * @param ISV_INST isv_inst       -[in] Verifier handle
 * @see
 */
int ISVAPI ISVDestroyInst(ISV_INST isv_inst);
typedef int (ISVAPI *Proc_ISVDestroyInst)(ISV_INST isv_inst);

/** 
 * @ brief ISVEnroll 
 *
 * Enroll a user's utterance to generate the user's specified model
 *    if utterance is not good enough, it will be discarded
 *    if utterance is not enough to generate the model, the utterance' feature will be save for next time
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_INVALID_MEDIA_TYPE format of input audio is not supported
 *  ISV_ERROR_VD_MODEL_EXIST model is already correctly enrolled
 *  ISV_ERROR_NO_MORE_DATA until this enrollment, the total utterance is not enough
 *  ISV_ERROR_VD_INVALID_ID_STR model id or user id is illegal
 *
 * @param ISV_INST isv_inst         -[in] Verifier handle
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @param ISV_Speech * speech       -[in]Speech object, input of utterance and format 
 * @see
 */
int ISVAPI ISVEnroll(ISV_INST isv_inst, const wchar_t * user_id, const ISV_Speech * speech);
typedef int (ISVAPI *Proc_ISVEnroll)(ISV_INST isv_inst, const wchar_t * user_id, const ISV_Speech * speech);

/** 
 * @ brief ISVVerifyText
 *
 * Check if the given utterance match the claimed text
 *    text language is specified by param
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_INVALID_MEDIA_TYPE format of input audio is not supported
 *  ISV_ERROR_VE_TEXT_LENGTH text length is invalid
 *
 * @param ISV_INST isv_inst         -[in] Verifier handle
 * @param ISV_Speech * speech       -[in] Speech object, input of utterance and format
 * @param const wchar_t * speech_text -[in] The claimed text
 * @param float * match_score       -[out] match score
 * @para int * decision             -[out] match decision, 0-match, 1-not match, the threshold value is specified by param
 * @see
 */
int ISVAPI ISVVerifyText(ISV_INST isv_inst, const wchar_t * speech_text, const ISV_Speech * speech, float * match_score, int * decision);
typedef int (ISVAPI *Proc_ISVVerifyText)(ISV_INST isv_inst, const wchar_t * speech_text,  const ISV_Speech * speech, float * match_score, int * decision);

/** 
 * @ brief ISVVerify
 *
 * Use this function to authenticate an utterance for given user model
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_NO_LICENSE no license
 *  ISV_ERROR_INVALID_MEDIA_TYPE format of input audio is not supported
 *  ISV_ERROR_VD_MODEL_NOT_EXIST model not exist
 *  ISV_ERROR_VD_USER_NOT_EXIST user not exist
 *  ISV_ERROR_VD_INVALID_ID_STR model id or user id is illegal
 *  
 * @param ISV_INST isv_inst         -[in] Verifier handle
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @param ISV_Speech * speech       -[in] Speech object, input of utterance and format
 * @param ISV_Result * result       -[out] Verify result
 * @see
 */
int ISVAPI ISVVerify(ISV_INST isv_inst, const wchar_t * user_id, const ISV_Speech * speech,	ISV_Result * result);
typedef int (ISVAPI *Proc_ISVVerify)(ISV_INST isv_inst, const wchar_t * user_id, const ISV_Speech * speech,	ISV_Result * result);

/** 
 * @ brief ISVIdentify
 *
 * Use this function to authenticate an utterance for given user model list
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_NO_LICENSE no license
 *  ISV_ERROR_INVALID_MEDIA_TYPE format of input audio is not supported
 *  ISV_ERROR_VD_INVALID_ID_STR model id or user id is illegal
 *
 * @param ISV_INST isv_inst         -[in] Verifier handle
 * @param const wchar_t * user_id_list -[in] User id list 
 * @param ISV_Speech * speech       -[in] Speech object, input of utterance and format
 * @param unsigned int * best_num   -[in,out] The best result number 
 * @param ISV_Result * result       -[in,out] The verify results collection, size is specified by best_num
 * @see
 */
int ISVAPI ISVIdentify(ISV_INST isv_inst, const wchar_t * user_id_list, const ISV_Speech * speech, unsigned int * best_num, ISV_Result * result);
typedef int (ISVAPI *Proc_ISVIdentify)(ISV_INST isv_inst, const wchar_t * user_id_list, const ISV_Speech * speech, unsigned int * best_num, ISV_Result * result);

/** 
 * @ brief ISVModelAdapt
 *
 * Use this function to adapt user model by given utterance
 *    this function improves the user model whith a newly collected utterance
 *    do not use this function unless you are highly certain of the utterance identity
 *    otherwise performing  model adaptation with wrong data can reduce the accuracy of user model
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_INVALID_MEDIA_TYPE format of input audio is not supported
 *  ISV_ERROR_VD_MODEL_NOT_EXIST model not exist
 *  ISV_ERROR_VD_USER_NOT_EXIST user not exist
 *  ISV_ERROR_ADAPT_LOW_SCORE model after adapted got lower score
 *  ISV_ERROR_VD_VM_OUT_OF_DATE while adapting, the model have be changed
 *  ISV_ERROR_VD_INVALID_ID_STR model id or user id is illegal
 *
 * @param ISV_INST isv_inst         -[in] Verifier handle
 * @param const wchar_t * user_id   -[in] A string that uniquely identifies the user
 * @param ISV_Speech * speech       -[in] Speech object, input of utterance and format
 * @see
 */
int ISVAPI ISVModelAdapt(ISV_INST isv_inst, const wchar_t * user_id, const ISV_Speech * speech);
typedef int (ISVAPI *Proc_ISVModelAdapt)(ISV_INST isv_inst, const wchar_t * user_id, const ISV_Speech * speech);

/** 
 * @ brief ISVGetParam
 *
 * Get verifier parameters
 *    refer to the reference documentation for parameter descriptions and legal values
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_INVALID_PARA invalid parameter
 *  ISV_ERROR_NO_ENOUGH_BUFFER input buffer length is not enough
 *
 * @param ISV_INST isv_inst         -[in] Verifier handle
 * @param const wchar_t * param     -[in] Parameter name
 * @param wchar_t * value           -[in] Pointer to parameter string retrieved
 * @param unsigned int * len        -[in] Length of string buffer passed in
 *                                  if too short, required len will be returned here
 *                                  and ISV_ERROR_NO_ENOUGH_BUFFER will be set
 * @see
 */
int ISVAPI ISVGetParam(ISV_INST isv_inst, const wchar_t * param, wchar_t * value, unsigned int * len);
typedef int (ISVAPI *Proc_ISVGetParam)(ISV_INST isv_inst, const wchar_t * param, wchar_t * value, unsigned int * len);

/** 
 * @ brief ISVSetParam
 *
 * Set verifier parameters
 *    refer to the reference documentation for parameter descriptions and legal values
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_INVALID_PARA invalid parameter
 *  ISV_ERROR_INVALID_PARA_VALUE invalid parameter value
 *
 * @param ISV_INST isv_inst         -[in] Verifier handle
 * @param const wchar_t * param     -[in] Parameter name
 * @param const wchar_t * value     -[in] String value to set
 * @see
 */
int ISVAPI ISVSetParam(ISV_INST isv_inst, const wchar_t * param, const wchar_t * value);
typedef int (ISVAPI *Proc_ISVSetParam)(ISV_INST isv_inst, const wchar_t * param, const wchar_t * value);

/** 
 * @ brief ISVSessionBegin
 *
 * Marks the beginning of a call session
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_BUSY session-begin operation is already called but not ended
 *  ISV_ERROR_INVALID_PARA_VALUE channel or params is invalid
 *
 * @param ISV_INST isv_inst         -[in] Verifier handle
 * @param const wchar_t *  channel  -[in] Specifies the value for the CHAN token in the call log in all records
 *                                  this argument is mandatory. It may not be NULL or empty string, and must be
 *                                  unique for all verifier handles in the process
 * @param const wchar_t * params    -[in] Some other informations that user define, such as client application id
 * @see
 */
int ISVAPI ISVSessionBegin(ISV_INST isv_inst, const wchar_t * channel, const wchar_t * params);
typedef int (ISVAPI *Proc_ISVSessionBegin)(ISV_INST isv_inst, const wchar_t * channel, const wchar_t * params);

/** 
 * @ brief ISVSessionBegin
 *
 * Marks the end of the call session that began in ISVSessionBegin()
 *
 * @author iFLYTEK
 * @return 
 *  ISV_SUCCESS on success
 *  ISV_ERROR_NOT_BEGIN not invoke ISVSessionBegin first
 * 
 * @param ISV_INST isv_inst         -[in] Verifier handle
 * @see
 */
int ISVAPI ISVSessionEnd(ISV_INST isv_inst);
typedef int (ISVAPI *Proc_ISVSessionEnd)(ISV_INST isv_inst);

/**
* @brief ISVLogEvent
*
* Write an event and related information into the verifier event log
*
* @author iFLYTEK
* @return 
*  ISV_SUCCESS on success
* @param ISV_INST isv_inst         -[in] Verifier handle
* @param event                     -[in] Event name
* @param value                     -[in] String to be logged.  This value is recommended to be composed 
*                                  of a token/value list which matches the verifier logging format.
*
* @see
*/
int ISVAPI ISVLogEvent(ISV_INST isv_inst, const wchar_t * event, const wchar_t * value);
typedef int (ISVAPI *Proc_ISVLogEvent)(ISV_INST isv_inst, const wchar_t * event, const wchar_t * value);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __ISV_H__ */
