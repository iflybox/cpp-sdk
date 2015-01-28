/**
 * @file    isv_erros.h
 * @brief   iFLY Speaker Verifier Header File
 * 
 *  This file contains the error code declarations 
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

#ifndef __ISV_ERRORS_H__
#define __ISV_ERRORS_H__

/**
* The enumeration ISV_ERROR enumerates possible error codes
*  ISV_SUCCESS function completed successfully
*  ISV_ERROR_FAIL 
*  ISV_ERROR_EXCEPTION
*  ISV_ERROR_GENERAL generic error begin
*  ISV_ERROR_OUT_OF_MEMORY out of memory
*  ISV_ERROR_FILE_NOT_FOUND file not found
*  ISV_ERROR_NOT_SUPPORT operation not support
*  ISV_ERROR_NOT_IMPLEMENT 
*  ISV_ERROR_ACCESS access failed
*  ISV_ERROR_INVALID_PARA invalid parameter
*  ISV_ERROR_INVALID_PARA_VALUE invalid parameter value
*  ISV_ERROR_INVALID_HANDLE invalid handle
*  ISV_ERROR_INVALID_DATA invalid data
*  ISV_ERROR_NO_LICENSE no license
*  ISV_ERROR_NOT_INIT not initialize
*  ISV_ERROR_NULL_HANDLE empty handle
*  ISV_ERROR_OVERFLOW memory overflow
*  ISV_ERROR_TIME_OUT time out
*  ISV_ERROR_OPEN_FILE open file failed
*  ISV_ERROR_NO_ENOUGH_BUFFER buffer not enough
*  ISV_ERROR_NO_DATA no data
*  ISV_ERROR_NO_MORE_DATA need more data
*  ISV_ERROR_RES_MISSING resource load failed
*  ISV_ERROR_LOAD_MODULE load module failed
*  ISV_ERROR_BUSY some operations already are in progress
*  ISV_ERROR_INVALID_CONFIG invalid config
*  ISV_ERROR_CANCELED operation is canceled
*  ISV_ERROR_INVALID_MEDIA_TYPE audio type is not supported
*  ISV_ERROR_NULL_PTR null pointer
*  ISV_ERROR_NOT_BEGIN end before beginning
*  ISV_ERROR_VE_GENERAL verifier engine error begin
*  ISV_ERROR_VE_INVALID_FEATURE invalid feature data
*  ISV_ERROR_VE_INVALID_MODEL invalid model data
*  ISV_ERROR_VE_TRUNCATED utter is truncated
*  ISV_ERROR_VE_MUCH_NOISE utter has too much noise
*  ISV_ERROR_VE_VR_MODULE load vr module failed
*  ISV_ERROR_VE_ADAPT_LOW_SCORE model after adapted get lower score
*  ISV_ERROR_VE_UTTER_TOO_SHORT utter duration is not enough
*  ISV_ERROR_VE_TEXT_LENGTH length of input text is invalid
*  ISV_ERROR_VD_GENERAL verifier data-management error begin
*  ISV_ERROR_VD_INVALID_REPO invalid repository source
*  ISV_ERROR_VD_INVALID_CONN_STR invalid connect string for specified repository
*  ISV_ERROR_VD_USER_EXIST user already exist
*  ISV_ERROR_VD_USER_NOT_EXIST user not exist
*  ISV_ERROR_VD_MODEL_EXIST model for specified user already exist
*  ISV_ERROR_VD_MODEL_NOT_EXIST model for specified user not exist
*  ISV_ERROR_VD_FEATURE_NOT_EXIST feature not exsit
*  ISV_ERROR_VD_DB_INIT init database failed
*  ISV_ERROR_VD_INVALID_ID_STR invalid odbc connection string
*  ISV_ERROR_VD_VM_OUT_OF_DATE  updating voiceprint model  is out of date
*  ISV_ERROR_VD_VM_EXIST voiceprint model has already been exist
*  ISV_ERROR_VD_VM_NOT_EXIST voiceprint model not exist
*  ISV_ERROR_VD_DB_EXCEPTION database IO exception
*  ISV_ERROR_NET_GENERAL verifier network error begin
*  ISV_ERROR_NET_AUDIO_SEND sending audio error from client to server
*/
enum
{
 /* Generic error defines */
 ISV_SUCCESS                          =  0,
 ISV_ERROR_FAIL                       = -1,
 ISV_ERROR_EXCEPTION                  = -2,

 /* Common errors */
 ISV_ERROR_GENERAL                    = 20000, /* 0x4E20 Generic Error */
 ISV_ERROR_OUT_OF_MEMORY              = 20001, /* 0x4E21 */
 ISV_ERROR_FILE_NOT_FOUND             = 20002, /* 0x4E22 */
 ISV_ERROR_NOT_SUPPORT                = 20003, /* 0x4E23 */
 ISV_ERROR_NOT_IMPLEMENT              = 20004, /* 0x4E24 */
 ISV_ERROR_ACCESS                     = 20005, /* 0x4E25 */
 ISV_ERROR_INVALID_PARA               = 20006, /* 0x4E26 */
 ISV_ERROR_INVALID_PARA_VALUE         = 20007, /* 0x4E27 */
 ISV_ERROR_INVALID_HANDLE             = 20008, /* 0x4E28 */
 ISV_ERROR_INVALID_DATA               = 20009, /* 0x4E29 */
 ISV_ERROR_NO_LICENSE                 = 20010, /* 0x4E2A */
 ISV_ERROR_NOT_INIT                   = 20011, /* 0x4E2B */
 ISV_ERROR_NULL_HANDLE                = 20012, /* 0x4E2C */
 ISV_ERROR_OVERFLOW                   = 20013, /* 0x4E2D */
 ISV_ERROR_TIME_OUT                   = 20014, /* 0x4E2E */
 ISV_ERROR_OPEN_FILE                  = 20015, /* 0x4E2F */
 ISV_ERROR_NOT_FOUND                  = 20016, /* 0x4E30 */
 ISV_ERROR_NO_ENOUGH_BUFFER           = 20017, /* 0x4E31 */
 ISV_ERROR_NO_DATA                    = 20018, /* 0x4E32 */
 ISV_ERROR_NO_MORE_DATA               = 20019, /* 0x4E33 */
 ISV_ERROR_RES_MISSING                = 20020, /* 0x4E34 */
 ISV_ERROR_SKIPPED                    = 20021, /* 0x4E35 */
 ISV_ERROR_ALREADY_EXIST              = 20022, /* 0x4E36 */
 ISV_ERROR_LOAD_MODULE                = 20023, /* 0x4E37 */
 ISV_ERROR_BUSY                       = 20024, /* 0x4E38 */
 ISV_ERROR_INVALID_CONFIG             = 20025, /* 0x4E39 */
 ISV_ERROR_VERSION_CHECK              = 20026, /* 0x4E3A */
 ISV_ERROR_CANCELED                   = 20027, /* 0x4E3B */
 ISV_ERROR_INVALID_MEDIA_TYPE         = 20028, /* 0x4E3C */
 ISV_ERROR_NULL_PTR                   = 20029, /* 0x4E3D */
 ISV_ERROR_NOT_BEGIN                  = 20030, /* 0x4E3E */

 /* Error codes of engine */
 ISV_ERROR_VE_GENERAL                 = 20100, /* 0x4E84 */
 ISV_ERROR_VE_INVALID_FEATURE         = 20101, /* 0x4E85 */
 ISV_ERROR_VE_INVALID_MODEL           = 20102, /* 0x4E86 */
 ISV_ERROR_VE_TRUNCATED               = 20103, /* 0x4E87 */
 ISV_ERROR_VE_MUCH_NOISE              = 20104, /* 0x4E88 */
 ISV_ERROR_VE_VR_MODULE               = 20105, /* 0x4E89 */
 ISV_ERROR_VE_ADAPT_LOW_SCORE         = 20106, /* 0x4E8A*/
 ISV_ERROR_VE_UTTER_TOO_SHORT         = 20107, /* 0x4E8B */
 ISV_ERROR_VE_TEXT_LENGTH             = 20108, /* 0x4E8C */


 /* Error codes of data */
 ISV_ERROR_VD_GENERAL                 = 20200, /* 0x4EE8 */
 ISV_ERROR_VD_INVALID_REPO            = 20201, /* 0x4EE9 */
 ISV_ERROR_VD_INVALID_CONN_STR        = 20202, /* 0x4EEA */
 ISV_ERROR_VD_USER_EXIST              = 20203, /* 0x4EEB */
 ISV_ERROR_VD_USER_NOT_EXIST          = 20204, /* 0x4EEC */
 ISV_ERROR_VD_MODEL_EXIST             = 20205, /* 0x4EED */
 ISV_ERROR_VD_MODEL_NOT_EXIST         = 20206, /* 0x4EEE */
 ISV_ERROR_VD_FEATURE_NOT_EXIST       = 20207, /* 0x4EEF */
 ISV_ERROR_VD_DB_INIT                 = 20208, /* 0x4EF0 */
 ISV_ERROR_VD_INVALID_ID_STR          = 20209, /* 0x4EF1 */
 ISV_ERROR_VD_VM_OUT_OF_DATE          = 20210, /* Ox4EF2 */
 ISV_ERROR_VD_VM_EXIST                = 20211, /* 0X4EF3 */
 ISV_ERROR_VD_VM_NOT_EXIST            = 20212, /* 0x4EF4 */
 ISV_ERROR_VD_DB_EXCEPTION            = 20213, /* 0x4EF5 */

 /* Error codes of network */
 ISV_ERROR_NET_GENERAL                = 20300, /* 0x4F4C */
 ISV_ERROR_NET_AUDIO_SEND	          = 20301, /* 0x4F4D */
};

#endif /* __ISV_ERRORS_H__ */
