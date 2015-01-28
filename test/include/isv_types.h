/**
 * @file    isv_types.h
 * @brief   iFLY Speaker Verifier Header File
 * 
 *  This file contains the application programming interface (API) declarations 
 *  of ISV. Developer can include this file in your project to build applications.
 *  For more information, please read the developer guide.
 
 *  Use of this software is subject to certain restrictions and limitations set
 *  forth in a license agreement entered into between iFLYTEK, Co,LTD.
 *  and the licensee of this software.  Please refer to the license
 *  agreement for license use rights and restrictions.
 *
 *  Copyright (C)    1999 - 2008 by ANHUI USTC IFLYTEK. Co,LTD.
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

#ifndef __ISV_TYPES_H__
#define __ISV_TYPES_H__

/**
 * C/C++ wchar_t support
 */
#ifdef __cplusplus
# include <cwchar>
#else  /* c */
# include <wchar.h>
#endif /* wchar */



/**
 * ISV API type
 */
#if defined(_MSC_VER)            /* Microsoft Visual C++ */
  #if !defined(ISVAPI)
    #define ISVAPI __stdcall
  #endif
  #pragma pack(push, 8)
#elif defined(__BORLANDC__)      /* Borland C++ */
  #if !defined(ISVAPI)
    #define ISVAPI __stdcall
  #endif
  #pragma option -a8
#elif defined(__WATCOMC__)       /* Watcom C++ */
  #if !defined(ISVAPI)
    #define ISVAPI __stdcall
  #endif
  #pragma pack(push, 8)
#else                            /* Any other including Unix */
  #if !defined(ISVAPI)
    #define ISVAPI
  #endif
#endif


/**
 * True and false
 */
#ifndef FALSE
#define FALSE   0
#endif	/* FALSE */

#ifndef TRUE
#define TRUE   1
#endif	/* TRUE */

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Opaque data types
 */
typedef struct ISVverifier { void * internal_; } ISVverifier, * ISV_INST;
typedef struct ISVEnum { void * internal_; } ISVEnum , * ISV_EnumHandle;

#define ISV_DATE_STRLEN 40
#define ISV_MAX_USER_STRLEN 64
#define ISV_MAX_MODEL_STRLEN 64
#define ISV_MAX_MODEL_INFO_STRLEN 1024


/**
 * Speech information struct
 */
typedef struct ISVSpeech
{
   /**
    * Speech data. 
    *
	* @param type media type of speech
    *    "audio/basic"
    *    "audio/basic;rate=16000"
    *    "audio/x-alaw-basic"
    *    "audio/x-alaw-basic;rate=16000"
    *    "audio/L16;rate=8000"
    *    "audio/L16;rate=16000"
    *    "audio/voxware"
	* @param len_bytes Length of speech buffer, in bytes
    * @param data speech buffer
    * @param status speech status, not use now
    */
  const wchar_t * type;
  unsigned int len_bytes;
  void * data;
  int status;  
} ISV_Speech;

/**
 * Statistics that are pertinent to speaker verifier output
 */
typedef struct ISVResult
{
   /**
    * Verifier result. 
    *
	* @param user_id user of result
	* @param user_model_id model of result
    * @param raw_score raw score
    * @param normalized_score normalized from raw score
	* @param confidence speech quality, not use now
	* @param confidence refuse - 0; accept - 1
	* @param error with err when gets result for specified user model
    */
  wchar_t user_id[ISV_MAX_USER_STRLEN];
  wchar_t user_model_id[ISV_MAX_MODEL_STRLEN];
  float raw_score;
  float normalized_score;
  float confidence;
  int decision;
  int error;
} ISV_Result;

#ifdef __cplusplus
}
#endif

/* Reset the structure packing alignments for different compilers. */
#if defined(_MSC_VER)            /* Microsoft Visual C++ */
  #pragma pack(pop)
#elif defined(__BORLANDC__)      /* Borland C++ */
  #pragma option -a.
#elif defined(__WATCOMC__)       /* Watcom C++ */
  #pragma pack(pop)
#endif

#endif /* __ISV_TYPES_H__ */
