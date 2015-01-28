/**
 * @file    isv_ve.h
 * @brief   iFLY Speech Verifier Header File
 * 
 *  This file contains the speech verifier engine interface (API) declarations 
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

#ifndef __ISV_VE_H__
#define __ISV_VE_H__

#include "isv_types.h"
#include "isv_errors.h"


/**
 * ISV API type
 */
#if defined(_MSC_VER)            /* Microsoft Visual C++ */
  #pragma pack(push, 8)
#elif defined(__BORLANDC__)      /* Borland C++ */
  #pragma option -a8
#elif defined(__WATCOMC__)       /* Watcom C++ */
  #pragma pack(push, 8)
#else                            /* Any other including Unix */
#endif


#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Opaque data types
 */
 typedef struct ISVFeature { void * internal_; }ISVFeature, * ISV_FeatureHandle;
 typedef struct ISVModel { void * internal_; } ISVModel, * ISV_ModelHandle;
 
/**
 * Compute model type
 */
typedef enum ISVTextType
{
  ISV_TEXT_FREE,
  ISV_TEXT_PASSWORD,
} ISV_TextType;

/**
 * Statistics that are pertinent to feature data
 */
typedef struct ISVFeaturInfo
{
  int gender;
  int handset;
  float duration;
  float level;
  float snr;
  float clipped;
  int truncated;
  wchar_t data_created[ISV_DATE_STRLEN];
} ISV_FeatureInfo;

/**
 * Statistics that are pertinent to feature data
 */
typedef struct ISVModelInfo
{
  ISV_FeatureInfo feature_info;
  float consistente;
  int adaption_num;
  wchar_t data_created[ISV_DATE_STRLEN];
} ISV_ModelInfo;


/* -- start docme interface -- */
int ISVAPI ISVEInitialize(const wchar_t * engine, void * reserved);
typedef int (ISVAPI *Proc_ISVEInitialize)(const wchar_t * engine, void * reserved);

int ISVAPI ISVEUninitialize(void);
typedef int (ISVAPI *Proc_ISVEUninitialize)(void);

int ISVAPI ISVECreateInst(ISV_INST * isv_inst, const wchar_t * params);
typedef int (ISVAPI *Proc_ISVECreateInst)(ISV_INST * isv_inst, const wchar_t * params);

int ISVAPI ISVEDestroyInst(ISV_INST isv_inst);
typedef int (ISVAPI *Proc_ISVEDestroyInst)(ISV_INST isv_inst);

int ISVAPI ISVEComputeFeature(const ISV_Speech * speech, int end_pointed, ISV_FeatureHandle * feature_handle);
typedef int (ISVAPI *Proc_ISVEComputeFeature)(const ISV_Speech * speech, int end_pointed, ISV_FeatureHandle * feature_handle);

int  ISVAPI ISVEGetFeatureInfo(ISV_FeatureHandle feature_handle, ISV_FeatureInfo  * feature_info);
typedef int (ISVAPI *Proc_ISVEGetFeatureInfo)(ISV_FeatureHandle feature_handle, ISV_FeatureInfo  * feature_info);

int ISVAPI ISVESerializeFeature(ISV_FeatureHandle feature_handle, const void ** buffer, unsigned int * len_bytes);
typedef int (ISVAPI *Proc_ISVESerializeFeature)(ISV_FeatureHandle feature_handle, const void ** buffer, unsigned int * len_bytes);

int ISVAPI ISVEDeserializeFeature(void * buffer, unsigned int len_bytes, ISV_FeatureHandle * feature_handle);
typedef int (ISVAPI *Proc_ISVEDeserializeFeature)(void * buffer, unsigned int len_bytes, ISV_FeatureHandle * feature_handle);

int ISVAPI ISVEDestroyFeature(ISV_FeatureHandle feature_handle);
typedef int (ISVAPI *Proc_ISVEDestroyFeature)(ISV_FeatureHandle feature_handle);

int ISVAPI ISVEComputeModel(ISV_FeatureHandle * feature_handle, unsigned int feature_num, int text_type, ISV_ModelHandle * modle_handle);
typedef int (ISVAPI *Proc_ISVEComputeModel)(ISV_FeatureHandle * feature_handle, unsigned int feature_num, int text_type, ISV_ModelHandle * modle_handle);

int ISVAPI ISVEGetModelInfo(ISV_ModelHandle model_handle, ISV_ModelInfo * model_info);
typedef int (ISVAPI *Proc_ISVEGetModelInfo)(ISV_ModelHandle model_handle, ISV_ModelInfo * model_info);

int ISVAPI ISVESerializeModel(ISV_ModelHandle model_handle, const void ** buffer, unsigned int * len_bytes);
typedef int (ISVAPI *Proc_ISVESerializeModel)(ISV_ModelHandle model_handle, const void ** buffer, unsigned int * len_bytes);

int ISVAPI ISVEDeserializeModel(void * buffer, unsigned int len_bytes, ISV_ModelHandle * model_handle);
typedef int (ISVAPI *Proc_ISVEDeserializeModel)(void * buffer, unsigned int len_bytes, ISV_ModelHandle * model_handle);

int ISVAPI ISVEDestroyModel(ISV_ModelHandle model_handle);
typedef int (ISVAPI *Proc_ISVEDestroyModel)(ISV_ModelHandle model_handle);

int ISVAPI ISVEVerifyText(ISV_INST isv_inst, const wchar_t * speech_text, const ISV_Speech * speech, float * match_score, int * decision);
typedef int (ISVAPI *Proc_ISVEVerifyText)(ISV_INST isv_inst, const wchar_t * speech_text, const ISV_Speech * speech, float * match_score, int * decision);

int ISVAPI ISVEVerify(ISV_INST isv_inst, ISV_FeatureHandle feature_handle, ISV_ModelHandle model_handle,  ISV_Result * result);
typedef int (ISVAPI *Proc_ISVEVerify)(ISV_INST isv_inst, ISV_FeatureHandle feature_handle, ISV_ModelHandle model_handle,  ISV_Result * result);

int ISVAPI ISVEIdentifyBegin(ISV_INST isv_inst, ISV_FeatureHandle feature_handle);
typedef int (ISVAPI *Proc_ISVEIdentifyBegin)(ISV_INST isv_inst, ISV_FeatureHandle feature_handle);

int ISVAPI ISVEIdentify(ISV_INST isv_inst, ISV_ModelHandle model_handle, ISV_Result * result);
typedef int (ISVAPI *Proc_ISVEIdentify)(ISV_INST isv_inst, ISV_ModelHandle model_handle, ISV_Result * result);

int ISVAPI ISVEIdentifyEnd(ISV_INST isv_inst);
typedef int (ISVAPI *Proc_ISVEIdentifyEnd)(ISV_INST isv_inst);

int ISVAPI ISVEModelAdapt(ISV_INST isv_inst, ISV_ModelHandle model_handle, ISV_FeatureHandle feature_handle);
typedef int (ISVAPI *Proc_ISVEModelAdapt)(ISV_INST isv_inst, ISV_ModelHandle model_handle, ISV_FeatureHandle feature_handle);

int ISVAPI ISVEGetParam(ISV_INST isv_inst, const wchar_t * param, wchar_t * value, unsigned int * len);
typedef int (ISVAPI *Proc_ISVEGetParam)(ISV_INST isv_inst, const wchar_t * param, wchar_t * value, unsigned int * len);

int ISVAPI ISVESetParam(ISV_INST isv_inst, const wchar_t * param, const wchar_t * value);
typedef int (ISVAPI *Proc_ISVESetParam)(ISV_INST isv_inst, const wchar_t * param, const wchar_t * value);


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

#endif /* __ISV_VD_H__ */
