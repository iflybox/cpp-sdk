/** 
 * @file	isrlib.h
 * @brief	识别相关动态库的加载器
 * 
 *  目前实现了isr_rec.dll、isr_ep.dll和isr_ve.dll
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
 * </table>
 * 
 */
#ifndef __ISRLIB_H__
#define __ISRLIB_H__

#include "utils/sulib.h"
#include "isr_rec.h"
#include "isr_spec.h"
#include "isr_ep.h"
#include "isr_ve.h"
#include "isr_api.h"
#include "qisr.h"

// Sun OS pack
#ifndef WIN32
#pragma pack(push)
#pragma pack(8)
#endif // sun

namespace sp {

// default reclib name
const wchar_t rec_lib_name[] = L"isr_rec"
	#ifdef _DEBUG
	L"d"
	#endif /* _DEBUG */
	;

/** 
 * @class	rec_lib
 * 
 * @brief	识别动态库加载器
 * 
 *  完成加载卸载识别动态库，并定位其API地址
 * 
 * @author	jdyu
 * @date	2007-7-16
 * 
 * @see		
 */
class rec_lib
	: public sp::su_lib<sp::loader, wchar_t>
	{
public:
	rec_lib(const wchar_t * lib = rec_lib_name)
		: sp::su_lib<sp::loader, wchar_t>(lib)
		{
		}

protected:
	virtual int load_procs(void)
		{
		su_proc_load_w(ISRrecInitialize)
		su_proc_load_w(ISRrecUninitialize)
		su_proc_load_w(ISRrecAcousticStateLoad)
		su_proc_load_w(ISRrecAcousticStateSave)
		su_proc_load_w(ISRrecAcousticStateReset)
		su_proc_load_w(ISRrecAcousticStateQuerySize)
		su_proc_load_w(ISRrecRecognizerCreate)
		su_proc_load_w(ISRrecRecognizerDestroy)
		su_proc_load_w(ISRrecGrammarLoad)
		su_proc_load_w(ISRrecGrammarCompile)
		su_proc_load_w(ISRrecGrammarActivate)
		su_proc_load_w(ISRrecGrammarDeactivate)
		su_proc_load_w(ISRrecGrammarFree)
		su_proc_load_w(ISRrecRecognizerSetParameter)
		su_proc_load_w(ISRrecRecognizerGetParameter)
		su_proc_load_w(ISRrecSessionBegin)
		su_proc_load_w(ISRrecSessionEnd)
		su_proc_load_w(ISRrecRecognizerStart)
		su_proc_load_w(ISRrecAudioWrite)
		su_proc_load_w(ISRrecRecognizerCompute)
		su_proc_load_w(ISRrecRecognizerInterpret)
		su_proc_load_w(ISRrecGetWaveform)
		su_proc_load_w(ISRrecGetXMLResult)
		su_proc_load_w(ISRrecParseDTMFResults)
		su_proc_load_w(ISRrecLogEvent)
		su_proc_load_w(ISRrecRecognizerStop)
		return 0;
		}

private:
	// 识别库
	su_proc_decl_w(ISRrecInitialize)
	su_proc_decl_w(ISRrecUninitialize)
	su_proc_decl_w(ISRrecAcousticStateLoad)
	su_proc_decl_w(ISRrecAcousticStateSave)
	su_proc_decl_w(ISRrecAcousticStateReset)
	su_proc_decl_w(ISRrecAcousticStateQuerySize)
	su_proc_decl_w(ISRrecRecognizerCreate)
	su_proc_decl_w(ISRrecRecognizerDestroy)
	su_proc_decl_w(ISRrecGrammarLoad)
	su_proc_decl_w(ISRrecGrammarCompile)
	su_proc_decl_w(ISRrecGrammarActivate)
	su_proc_decl_w(ISRrecGrammarDeactivate)
	su_proc_decl_w(ISRrecGrammarFree)
	su_proc_decl_w(ISRrecRecognizerSetParameter)
	su_proc_decl_w(ISRrecRecognizerGetParameter)
	su_proc_decl_w(ISRrecSessionBegin)
	su_proc_decl_w(ISRrecSessionEnd)
	su_proc_decl_w(ISRrecRecognizerStart)
	su_proc_decl_w(ISRrecAudioWrite)
	su_proc_decl_w(ISRrecRecognizerCompute)
	su_proc_decl_w(ISRrecRecognizerInterpret)
	su_proc_decl_w(ISRrecGetWaveform)
	su_proc_decl_w(ISRrecGetXMLResult)
	su_proc_decl_w(ISRrecParseDTMFResults)
	su_proc_decl_w(ISRrecLogEvent)
	su_proc_decl_w(ISRrecRecognizerStop)
	};


// default eplib name
const wchar_t ep_lib_name[] = L"isr_ep"
	#ifdef _DEBUG
	L"d"
	#endif /* _DEBUG */
	;

/** 
 * @class	ep_lib
 * 
 * @brief	EP lib
 * 
 *  端点检测库
 * 
 * @author	jdyu
 * @date	2008/4/15
 */
class ep_lib
	: public sp::su_lib<sp::loader, wchar_t>
	{
public:
	ep_lib(const wchar_t * lib = ep_lib_name)
		: sp::su_lib<sp::loader, wchar_t>(lib)
		{
		}

	virtual int load_procs(void)
		{
		su_proc_load_w(ISRepStateLoad)
		su_proc_load_w(ISRepStateSave)
		su_proc_load_w(ISRepStateQuerySize)
		su_proc_load_w(ISRepStateReset)
		su_proc_load_w(ISRepOpen)
		su_proc_load_w(ISRepClose)
		su_proc_load_w(ISRepDetectorCreate)
		su_proc_load_w(ISRepDetectorDestroy)
		su_proc_load_w(ISRepSessionBegin)
		su_proc_load_w(ISRepSessionEnd)
		su_proc_load_w(ISRepStart)
		su_proc_load_w(ISRepStop)
		su_proc_load_w(ISRepGetParameter)
		su_proc_load_w(ISRepSetParameter)
		su_proc_load_w(ISRepPromptDone)
		su_proc_load_w(ISRepRead)
		su_proc_load_w(ISRepWrite)
		return 0;
		}

private:
	su_proc_decl_w(ISRepStateLoad)
	su_proc_decl_w(ISRepStateSave)
	su_proc_decl_w(ISRepStateQuerySize)
	su_proc_decl_w(ISRepStateReset)
	su_proc_decl_w(ISRepOpen)
	su_proc_decl_w(ISRepClose)
	su_proc_decl_w(ISRepDetectorCreate)
	su_proc_decl_w(ISRepDetectorDestroy)
	su_proc_decl_w(ISRepSessionBegin)
	su_proc_decl_w(ISRepSessionEnd)
	su_proc_decl_w(ISRepStart)
	su_proc_decl_w(ISRepStop)
	su_proc_decl_w(ISRepGetParameter)
	su_proc_decl_w(ISRepSetParameter)
	su_proc_decl_w(ISRepPromptDone)
	su_proc_decl_w(ISRepRead)
	su_proc_decl_w(ISRepWrite)
	};


// default ve lib name
const wchar_t ve_lib_name[] = L"isr_ve"
	#ifdef _DEBUG
	L"d"
	#endif /* _DEBUG */
	;

/** 
 * @class	ve_lib
 * 
 * @brief	语音录入库加载器
 * 
 *  完成加载卸载识别动态库，并定位其API地址
 * 
 * @author	jdyu
 * @date	2008-05-09
 * 
 * @see		
 */
class ve_lib
	: public sp::su_lib<sp::loader, wchar_t>
	{
public:
	ve_lib(const wchar_t * lib = ve_lib_name)
		: sp::su_lib<sp::loader, wchar_t>(lib)
		{
		}

	virtual int load_procs(void)
		{
		su_proc_load_w(ISRveBegin)
		su_proc_load_w(ISRveEnd)
		su_proc_load_w(ISRvePrepare)
		su_proc_load_w(ISRveProcess)
		return 0;
		}

private:
	// 语音录入库
	su_proc_decl_w(ISRveBegin)
	su_proc_decl_w(ISRveEnd)
	su_proc_decl_w(ISRvePrepare)
	su_proc_decl_w(ISRveProcess)
	};

// default qisr_lib name
const wchar_t wqisr_lib_name[] = L"qisr"
	#ifdef _DEBUG
	L"d"
	#endif /* _DEBUG */
	;

/** 
 * @class	wqisr_lib
 * @brief	加载qisr库
 * @author	jdyu
 * @date	2010/12/1
 * @see		
 */
class wqisr_lib 
	: public sp::su_lib<sp::loader, wchar_t>
	{
public:
	wqisr_lib(const wchar_t * lib = wqisr_lib_name)
		: sp::su_lib<sp::loader, wchar_t>(lib)
		{
		}
	virtual int load_procs(void)
		{
		su_proc_load_w(QISRInit)
		su_proc_load_w(QISRSessionBegin)
		su_proc_load_w(QISRGrammarActivate)
		su_proc_load_w(QISRAudioWrite)
		su_proc_load_w(QISRGetResult)
		su_proc_load_w(QISRUploadData)
		su_proc_load_w(QISRSessionEnd)
		su_proc_load_w(QISRGetParam)
		su_proc_load_w(QISRWaveformRecog)
		su_proc_load_w(QISRLogEvent)

#ifdef MSP_WCHAR_SUPPORT
		su_proc_load_w(QISRInitW)
		su_proc_load_w(QISRSessionBeginW)
		su_proc_load_w(QISRGrammarActivateW)
		su_proc_load_w(QISRAudioWriteW)
		su_proc_load_w(QISRGetResultW)
		su_proc_load_w(QISRSessionEndW)
		su_proc_load_w(QISRWaveformRecogW)
		su_proc_load_w(QISRLogEventW)
#endif
		su_proc_load_w(QISRFini)
		return 0;
	}

private:
	su_proc_decl_w(QISRInit)
	su_proc_decl_w(QISRSessionBegin)
	su_proc_decl_w(QISRGrammarActivate)
	su_proc_decl_w(QISRAudioWrite)
	su_proc_decl_w(QISRGetResult)
	su_proc_decl_w(QISRUploadData)
	su_proc_decl_w(QISRSessionEnd)
	su_proc_decl_w(QISRGetParam)
	su_proc_decl_w(QISRWaveformRecog)
	su_proc_decl_w(QISRLogEvent)

#ifdef MSP_WCHAR_SUPPORT
	su_proc_decl_w(QISRInitW)
	su_proc_decl_w(QISRSessionBeginW)
	su_proc_decl_w(QISRGrammarActivateW)
	su_proc_decl_w(QISRAudioWriteW)
	su_proc_decl_w(QISRGetResultW)
	su_proc_decl_w(QISRSessionEndW)
	su_proc_decl_w(QISRWaveformRecogW)
	su_proc_decl_w(QISRLogEventW)
#endif
	su_proc_decl_w(QISRFini)
	};

// default qisr_lib name
const char qisr_lib_name[] = "qisr"
	#ifdef _DEBUG
	"d"
	#endif /* _DEBUG */
	;

/** 
 * @class	qisr_lib
 * @brief	加载qisr库
 * @author	jdyu
 * @date	2010/12/1
 * @see		
 */
class qisr_lib 
	: public sp::su_lib<sp::loader, char>
	{
public:
	qisr_lib(const char * lib = qisr_lib_name)
		: sp::su_lib<sp::loader, char>(lib)
		{
		}
	virtual int load_procs(void)
		{
		su_proc_load(QISRInit)
		su_proc_load(QISRSessionBegin)
		su_proc_load(QISRGrammarActivate)
		su_proc_load(QISRAudioWrite)
		su_proc_load(QISRGetResult)
		su_proc_load(QISRUploadData)
		su_proc_load(QISRSessionEnd)
		su_proc_load(QISRGetParam)
		su_proc_load(QISRWaveformRecog)
		su_proc_load(QISRLogEvent)
		//su_proc_load(QISRGetSessionParams)

#ifdef MSP_WCHAR_SUPPORT
		su_proc_load(QISRInitW)
		su_proc_load(QISRSessionBeginW)
		su_proc_load(QISRGrammarActivateW)
		su_proc_load(QISRAudioWriteW)
		su_proc_load(QISRGetResultW)
		su_proc_load(QISRSessionEndW)
		su_proc_load(QISRWaveformRecogW)
		su_proc_load(QISRLogEventW)
#endif
		su_proc_load(QISRFini)
		return 0;
		}

private:
	su_proc_decl(QISRInit)
	su_proc_decl(QISRSessionBegin)
	su_proc_decl(QISRGrammarActivate)
	su_proc_decl(QISRAudioWrite)
	su_proc_decl(QISRGetResult)
	su_proc_decl(QISRUploadData)
	su_proc_decl(QISRSessionEnd)
	su_proc_decl(QISRGetParam)
	su_proc_decl(QISRWaveformRecog)
	su_proc_decl(QISRLogEvent)
	//su_proc_decl(QISRGetSessionParams)

#ifdef MSP_WCHAR_SUPPORT
	su_proc_decl(QISRInitW)
	su_proc_decl(QISRSessionBeginW)
	su_proc_decl(QISRGrammarActivateW)
	su_proc_decl(QISRAudioWriteW)
	su_proc_decl(QISRGetResultW)
	su_proc_decl(QISRSessionEndW)
	su_proc_decl(QISRWaveformRecogW)
	su_proc_decl(QISRLogEventW)
#endif
	su_proc_decl(QISRFini)
	};

const wchar_t rpi_lib_name[] = L"isr_api"
	#ifdef _DEBUG
	L"d"
	#endif // DEBUG
	;

class rpi_lib
	: public sp::su_lib<sp::loader, wchar_t>
	{
public:
	rpi_lib(const wchar_t * lib = rpi_lib_name)
		: sp::su_lib<sp::loader, wchar_t>(lib)
		{
		}

	virtual int load_procs(void)
		{
		su_proc_load_w(ISRInitialize)
		su_proc_load_w(ISRUninitialize)
		su_proc_load_w(ISRRecognizerCreate)
		su_proc_load_w(ISRRecognizerDestroy)
		su_proc_load_w(ISRGrammarActivate)
		su_proc_load_w(ISRGrammarDeactivate)
		su_proc_load_w(ISRSetParameter)
		su_proc_load_w(ISRGetParameter)
		su_proc_load_w(ISRRecognizerStart)
		su_proc_load_w(ISRAudioWrite)
		su_proc_load_w(ISRPromptDone)
		su_proc_load_w(ISRRecognizerCompute)
		su_proc_load_w(ISRGetWaveform)
		su_proc_load_w(ISRGetXMLResult)
		su_proc_load_w(ISRParseDTMFResults)
		su_proc_load_w(ISRLogEvent)
		su_proc_load_w(ISRRecognizerStop)
		su_proc_load_w(ISRGetRecHandle)
		su_proc_load_w(ISRGetEpHandle)
		return 0;
		}

private:
	// 语音录入库
	su_proc_decl_w(ISRInitialize)
	su_proc_decl_w(ISRUninitialize)
	su_proc_decl_w(ISRRecognizerCreate)
	su_proc_decl_w(ISRRecognizerDestroy)
	su_proc_decl_w(ISRGrammarActivate)
	su_proc_decl_w(ISRGrammarDeactivate)
	su_proc_decl_w(ISRSetParameter)
	su_proc_decl_w(ISRGetParameter)
	su_proc_decl_w(ISRRecognizerStart)
	su_proc_decl_w(ISRAudioWrite)
	su_proc_decl_w(ISRPromptDone)
	su_proc_decl_w(ISRRecognizerCompute)
	su_proc_decl_w(ISRGetWaveform)
	su_proc_decl_w(ISRGetXMLResult)
	su_proc_decl_w(ISRParseDTMFResults)
	su_proc_decl_w(ISRLogEvent)
	su_proc_decl_w(ISRRecognizerStop)
	su_proc_decl_w(ISRGetRecHandle)
	su_proc_decl_w(ISRGetEpHandle)
	};

} /* namespace sp */

// Sun OS pack
#ifndef WIN32
#pragma pack(pop)
#endif // sun

#endif /* __ISRLIB_H__ */
