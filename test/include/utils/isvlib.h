/** 
 * @file	isvlib.h
 * @brief	声纹识别
 * 
 *  声纹识别几个动态库的加载
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2008/4/15
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2008/4/15	<td>jdyu	<td>Create this file</tr>
 *  <tr> <td>1.0		<td>2008/11/5	<td>isv		<td>change this file for isv</tr>
 * </table>
 * 
 */
#ifndef __ISVLIB_H__
#define __ISVLIB_H__

#include "utils/sulib.h"
#include "isv.h"
#include "isv_ve.h"
#include "isv_vd.h"

// Sun OS pack
#ifndef WIN32
#pragma pack(push)
#pragma pack(8)
#endif // sun

namespace sp {

//verifier top interface for end user
const wchar_t svt_lib_name[] = L"isv"
	#ifdef _DEBUG
	L"d"
	#endif
	L".dll";

class svt_lib
	: public sp::su_lib<sp::loader, wchar_t>
	{
public:
	svt_lib(const wchar_t* lib = svt_lib_name)
		: sp::su_lib<sp::loader, wchar_t>(lib)
		{
		}
	
	virtual int load_procs(void)
		{
		su_proc_load_w(ISVInitialize)
		su_proc_load_w(ISVUninitialize)
		su_proc_load_w(ISVCreateUser)
		su_proc_load_w(ISVDeleteUser)
		su_proc_load_w(ISVClearUser)
		su_proc_load_w(ISVCreateUserModel)
		su_proc_load_w(ISVDeleteUserModel)
		su_proc_load_w(ISVClearUserModel)
		su_proc_load_w(ISVQueryModelInfo)
		su_proc_load_w(ISVEnumAllUser)
		su_proc_load_w(ISVEnumAllModel)
		su_proc_load_w(ISVEnumUserModel)
		su_proc_load_w(ISVGetEnumString)
		su_proc_load_w(ISVDestroyEnumHandle)
		su_proc_load_w(ISVCreateInst)
		su_proc_load_w(ISVDestroyInst)
		su_proc_load_w(ISVEnroll)
		su_proc_load_w(ISVVerifyText)
		su_proc_load_w(ISVVerify)
		su_proc_load_w(ISVIdentify)
		su_proc_load_w(ISVModelAdapt)
		su_proc_load_w(ISVGetParam)
		su_proc_load_w(ISVSetParam)
		su_proc_load_w(ISVSessionBegin)
		su_proc_load_w(ISVSessionEnd)

		return 0;
		}

private:
	su_proc_decl_w(ISVInitialize)
	su_proc_decl_w(ISVUninitialize)
	su_proc_decl_w(ISVCreateUser)
	su_proc_decl_w(ISVDeleteUser)
	su_proc_decl_w(ISVClearUser)
	su_proc_decl_w(ISVCreateUserModel)
	su_proc_decl_w(ISVDeleteUserModel)
	su_proc_decl_w(ISVClearUserModel)
	su_proc_decl_w(ISVQueryModelInfo)
	su_proc_decl_w(ISVEnumAllUser)
	su_proc_decl_w(ISVEnumAllModel)
	su_proc_decl_w(ISVEnumUserModel)
	su_proc_decl_w(ISVGetEnumString)
	su_proc_decl_w(ISVDestroyEnumHandle)
	su_proc_decl_w(ISVCreateInst)
	su_proc_decl_w(ISVDestroyInst)
	su_proc_decl_w(ISVEnroll)
	su_proc_decl_w(ISVVerifyText)
	su_proc_decl_w(ISVVerify)
	su_proc_decl_w(ISVIdentify)
	su_proc_decl_w(ISVModelAdapt)
	su_proc_decl_w(ISVGetParam)
	su_proc_decl_w(ISVSetParam)
	su_proc_decl_w(ISVSessionBegin)
	su_proc_decl_w(ISVSessionEnd)
	};

//isp not use

// verifier engine interface
const wchar_t sve_lib_name[] = L"isv_ve"
	#ifdef _DEBUG
	L"d"
	#endif
	L".dll"
	;

class sve_lib
	: public sp::su_lib<sp::loader, wchar_t>
	{
public:
	virtual int load_procs(void)
		{
		su_proc_load_w(ISVEInitialize)
		su_proc_load_w(ISVEUninitialize)
		su_proc_load_w(ISVECreateInst)
		su_proc_load_w(ISVEDestroyInst)
		su_proc_load_w(ISVEComputeFeature)
		su_proc_load_w(ISVEGetFeatureInfo)
		su_proc_load_w(ISVESerializeFeature)
		su_proc_load_w(ISVEDeserializeFeature)
		su_proc_load_w(ISVEDestroyFeature)
		su_proc_load_w(ISVEComputeModel)
		su_proc_load_w(ISVEGetModelInfo)
		su_proc_load_w(ISVESerializeModel)
		su_proc_load_w(ISVEDeserializeModel)
		su_proc_load_w(ISVEDestroyModel)
		su_proc_load_w(ISVEVerifyText)
		su_proc_load_w(ISVEVerify)
		su_proc_load_w(ISVEIdentifyBegin)
		su_proc_load_w(ISVEIdentify)
		su_proc_load_w(ISVEIdentifyEnd)
		su_proc_load_w(ISVEModelAdapt)
		su_proc_load_w(ISVEGetParam)
		su_proc_load_w(ISVESetParam)
		return 0;
		}

private:
	su_proc_decl_w(ISVEInitialize)
	su_proc_decl_w(ISVEUninitialize)
	su_proc_decl_w(ISVECreateInst)
	su_proc_decl_w(ISVEDestroyInst)
	su_proc_decl_w(ISVEComputeFeature)
	su_proc_decl_w(ISVEGetFeatureInfo)
	su_proc_decl_w(ISVESerializeFeature)
	su_proc_decl_w(ISVEDeserializeFeature)
	su_proc_decl_w(ISVEDestroyFeature)
	su_proc_decl_w(ISVEComputeModel)
	su_proc_decl_w(ISVEGetModelInfo)
	su_proc_decl_w(ISVESerializeModel)
	su_proc_decl_w(ISVEDeserializeModel)
	su_proc_decl_w(ISVEDestroyModel)
	su_proc_decl_w(ISVEVerifyText)
	su_proc_decl_w(ISVEVerify)
	su_proc_decl_w(ISVEIdentifyBegin)
	su_proc_decl_w(ISVEIdentify)
	su_proc_decl_w(ISVEIdentifyEnd)
	su_proc_decl_w(ISVEModelAdapt)
	su_proc_decl_w(ISVEGetParam)
	su_proc_decl_w(ISVESetParam)
	};

//verifier data  interface
const wchar_t svd_lib_name[] = L"isv_vd"
	#ifdef _DEBUG
	L"d"
	#endif
	L".dll"
	;

class svd_lib
	: public sp::su_lib<sp::loader, wchar_t>
	{
public:
	virtual int load_procs()
		{
		su_proc_load_w(ISVDOpen)
		su_proc_load_w(ISVDClose)
		su_proc_load_w(ISVDCreateUser)
		su_proc_load_w(ISVDDeleteUser)
		su_proc_load_w(ISVDClearUser)
		su_proc_load_w(ISVDCreateUserModel)
		su_proc_load_w(ISVDDeleteUserModel)
		su_proc_load_w(ISVDClearUserModel)
		su_proc_load_w(ISVDEnumAllUser)
		su_proc_load_w(ISVDEnumAllModel)
		su_proc_load_w(ISVDEnumUserModel)
		su_proc_load_w(ISVDEnumModelFeature)
		su_proc_load_w(ISVDGetEnumString)
		su_proc_load_w(ISVDDestroyEnumHandle)
		su_proc_load_w(ISVDQueryModelInfo)
		su_proc_load_w(ISVDUpdateModelInfo)
		su_proc_load_w(ISVDStreamOutFeature)
		su_proc_load_w(ISVDGetFeatureData)
		su_proc_load_w(ISVDSaveModelFeature)
		su_proc_load_w(ISVDSaveFeatureUtter)
		su_proc_load_w(ISVDStreamOutModel)
		su_proc_load_w(ISVDGetModelData)
		su_proc_load_w(ISVDSaveUserModel)
		su_proc_load_w(ISVDDestroyDataHandle)
		return 0;
		}

private:
	su_proc_decl_w(ISVDOpen)
	su_proc_decl_w(ISVDClose)
	su_proc_decl_w(ISVDCreateUser)
	su_proc_decl_w(ISVDDeleteUser)
	su_proc_decl_w(ISVDClearUser)
	su_proc_decl_w(ISVDCreateUserModel)
	su_proc_decl_w(ISVDDeleteUserModel)
	su_proc_decl_w(ISVDClearUserModel)
	su_proc_decl_w(ISVDEnumAllUser)
	su_proc_decl_w(ISVDEnumAllModel)
	su_proc_decl_w(ISVDEnumUserModel)
	su_proc_decl_w(ISVDEnumModelFeature)
	su_proc_decl_w(ISVDGetEnumString)
	su_proc_decl_w(ISVDDestroyEnumHandle)
	su_proc_decl_w(ISVDQueryModelInfo)
	su_proc_decl_w(ISVDUpdateModelInfo)
	su_proc_decl_w(ISVDStreamOutFeature)
	su_proc_decl_w(ISVDGetFeatureData)
	su_proc_decl_w(ISVDSaveModelFeature)
	su_proc_decl_w(ISVDSaveFeatureUtter)
	su_proc_decl_w(ISVDStreamOutModel)
	su_proc_decl_w(ISVDGetModelData)
	su_proc_decl_w(ISVDSaveUserModel)
	su_proc_decl_w(ISVDDestroyDataHandle)
	};

}/* namespace sp */

// Sun OS pack
#ifndef WIN32
#pragma pack(pop)
#endif // sun

#endif /* __ISVLIB_H__ */
