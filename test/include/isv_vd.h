/**
 * @file    isv_vd.h
 * @brief   iFLY Speech Verifier Header File
 * 
 *  This file contains the speech verifier data interface (API) declarations 
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

#ifndef __ISV_VD_H__
#define __ISV_VD_H__

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
typedef struct ISVData { void * internal_; } ISVData, * ISV_DataHandle;

/**
 *  Model data struct
 */
typedef struct ISVModelData
{
  wchar_t user_id[ISV_MAX_USER_STRLEN];
  wchar_t user_model_id[ISV_MAX_MODEL_STRLEN];
  void * data;
  unsigned int len_bytes;
  int error;
  unsigned int success_veri;
  unsigned int failed_veri;
  wchar_t time_stamp[ISV_DATE_STRLEN];
}ISV_ModelData;

/**
 *  Feature data struct
 */
typedef struct ISVFeatureData
{
  int feature_key;
  void * data;
  unsigned int len_bytes;
  int error;
  wchar_t time_stamp[ISV_DATE_STRLEN];
} ISV_FeatureData;


/* -- start document interface -- */
int ISVAPI ISVDOpen(const wchar_t * engine, const wchar_t * repo_conn_str, const wchar_t * repo_type);
typedef int (ISVAPI *Proc_ISVDOpen)(const wchar_t * engine, const wchar_t * repo_conn_str, const wchar_t * repo_type);

int ISVAPI ISVDClose(void);
typedef int (ISVAPI *Proc_ISVDClose)(void);

int ISVAPI ISVDCreateUser(const wchar_t * user_id);
typedef int (ISVAPI *Proc_ISVDCreateUser)(const wchar_t * user_id);

int ISVAPI ISVDDeleteUser(const wchar_t * user_id);
typedef int (ISVAPI *Proc_ISVDDeleteUser)(const wchar_t * user_id);

int ISVAPI ISVDClearUser(const wchar_t * user_id);
typedef int (ISVAPI *Proc_ISVDClearUser)(const wchar_t * user_id);

int ISVAPI ISVDCreateUserModel(const wchar_t * user_id,	const wchar_t * user_model_id);
typedef int (ISVAPI *Proc_ISVDCreateUserModel)(const wchar_t * user_id,	const wchar_t * user_model_id);

int ISVAPI ISVDDeleteUserModel(const wchar_t * user_id,	const wchar_t * user_model_id);
typedef int (ISVAPI *Proc_ISVDDeleteUserModel)(const wchar_t * user_id,	const wchar_t * user_model_id);

int ISVAPI ISVDClearUserModel(const wchar_t * user_id, const wchar_t * user_model_id);
typedef int (ISVAPI *Proc_ISVDClearUserModel)(const wchar_t * user_id, const wchar_t * user_model_id);

int ISVAPI ISVDEnumAllUser(ISV_EnumHandle * enum_handle, unsigned int * count);
typedef int (ISVAPI *Proc_ISVDEnumAllUser)(ISV_EnumHandle * enum_handle, unsigned int * count);

int ISVAPI ISVDEnumAllModel(ISV_EnumHandle * enum_handle, unsigned int * count);
typedef int (ISVAPI *Proc_ISVDEnumAllModel)(ISV_EnumHandle * enum_handle, unsigned int * count);

int ISVAPI ISVDEnumUserModel(const wchar_t * user_id,	ISV_EnumHandle * enum_handle, unsigned int * count);
typedef int (ISVAPI *Proc_ISVDEnumUserModel)(const wchar_t * user_id,	ISV_EnumHandle * enum_handle, unsigned int * count);

int ISVAPI ISVDEnumModelFeature(const wchar_t * user_id, const wchar_t * user_model_id, ISV_EnumHandle * enum_handle, unsigned int * count);
typedef int (ISVAPI *Proc_ISVDEnumModelFeature)(const wchar_t * user_id, const wchar_t * user_model_id, ISV_EnumHandle * enum_handle, unsigned int * count);

int ISVAPI ISVDGetEnumString(ISV_EnumHandle enum_handle, const wchar_t ** enum_str);
typedef int (ISVAPI *Proc_ISVDGetEnumString)(ISV_EnumHandle enum_handle, const wchar_t ** enum_str);

int ISVAPI ISVDDestroyEnumHandle(ISV_EnumHandle enum_handle);
typedef int (ISVAPI *Proc_ISVDDestroyEnumHandle)(ISV_EnumHandle enum_handle);

int ISVAPI ISVDQueryModelInfo(const wchar_t * user_id, const wchar_t * user_model_id, wchar_t * field_value, unsigned int * len);
typedef int (ISVAPI *Proc_ISVDQueryModelInfo)(const wchar_t * user_id, const wchar_t * user_model_id, wchar_t * field_value, unsigned int * len);

int ISVAPI ISVDUpdateModelInfo(const wchar_t * user_id, const wchar_t * user_model_id, const wchar_t * field_value);
typedef int (ISVAPI *Proc_ISVDUpdateModelInfo)(const wchar_t * user_id, const wchar_t * user_model_id, const wchar_t * field_value);

int ISVAPI ISVDStreamOutModel(const wchar_t * user_model_pair, ISV_DataHandle * model_data_handle, unsigned int * model_count);
typedef int (ISVAPI *Proc_ISVDStreamOutModel)(const wchar_t * user_model_pair, ISV_DataHandle * model_data_handle, unsigned int* model_count);

int ISVAPI ISVDGetModelData(ISV_DataHandle model_data_handle, const ISV_ModelData ** model_data);
typedef int (ISVAPI *Proc_ISVDGetModelData)(ISV_DataHandle model_data_handle, const ISV_ModelData ** model_data);

int ISVAPI ISVDSaveUserModel(ISV_ModelData * model_data, unsigned int model_count, int update);
typedef int (ISVAPI *Proc_ISVDSaveUserModel)(ISV_ModelData * model_data, unsigned int model_count, int update);

int ISVAPI ISVDStreamOutFeature(const wchar_t * feature_key_list, ISV_DataHandle * feature_data_handle, unsigned int*  feature_count, int utter_data);
typedef int (ISVAPI *Proc_ISVDStreamOutFeature)(const wchar_t * feature_key, ISV_DataHandle * feature_data_handle, unsigned int*  feature_count, int utter_data);

int ISVAPI ISVDGetFeatureData(ISV_DataHandle feature_data_handle, const ISV_FeatureData ** feature_data);
typedef int (ISVAPI *Proc_ISVDGetFeatureData)(ISV_DataHandle feature_data_handle, const ISV_FeatureData ** feature_data);

int ISVAPI ISVDDestroyDataHandle(ISV_DataHandle data_handle);
typedef int (ISVAPI *Proc_ISVDDestroyDataHandle)(ISV_DataHandle data_handle);

int ISVAPI ISVDSaveModelFeature(const wchar_t * user_id, const wchar_t * user_model_id, ISV_FeatureData * feature_data, unsigned int feature_count);
typedef int (ISVAPI *Proc_ISVDSaveModelFeature)(const wchar_t * user_id, const wchar_t * user_model_id, ISV_FeatureData * feature_data, unsigned int feature_count);

int ISVAPI ISVDSaveFeatureUtter(ISV_FeatureData * utter_data, unsigned int utter_count);
typedef int (ISVAPI *Proc_ISVDSaveFeatureUtter)(ISV_FeatureData * utter_data, unsigned int utter_count);


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
