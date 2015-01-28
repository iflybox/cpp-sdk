/** 
 * @file	ttslib.h
 * @brief	TTS库加载器
 * 
 *  可以实现对TTS接口库的加载
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
#ifndef __TTSLIB_H__
#define __TTSLIB_H__

#include "iFly_TTS.h"
#include "iFly_SEM.h"
#include "qtts.h"
#include "utils/sulib.h"
#include "SEMEngine.h"

namespace sp {

// 标准接口库
class tts_lib
	: public sp::su_lib<>
	{
public:
	tts_lib(const char * lib = 0)
		: sp::su_lib<>(lib)
		{
		}

protected:
	virtual int load_procs(void)
		{
		su_proc_load(TTSInitializeEx)
		su_proc_load(TTSUninitialize)
		su_proc_load(TTSConnect)
		su_proc_load(TTSDisconnect)
		su_proc_load(TTSSynthText)
		su_proc_load(TTSFetchNext)
		su_proc_load(TTSSynthTextEx)
		su_proc_load(TTSSynthText2File)
		su_proc_load(TTSClean)
		su_proc_load(TTSGetParam)
		su_proc_load(TTSSetParam)
		su_proc_load(TTSLoadUserLib)
		su_proc_load(TTSUnloadUserLib)
		su_proc_load(TTSFormatMessage)
		su_proc_load(TTSAbout)
		su_proc_load(TTSInitialize)
		su_proc_load(TTSGetSynthParam)
		su_proc_load(TTSSetSynthParam)

		su_proc_load(SEMFindFirstEngine)
		su_proc_load(SEMFindNextEngine)
		su_proc_load(SEMFindFirstVoice)
		su_proc_load(SEMFindNextVoice)
		su_proc_load(SEMFindClose)
		su_proc_load(SEMGetEngineInfo)
		su_proc_load(SEMEngineCtrl)
		return 0;
		}

private:
	su_proc_decl(TTSInitializeEx)
	su_proc_decl(TTSUninitialize)
	su_proc_decl(TTSConnect)
	su_proc_decl(TTSDisconnect)
	su_proc_decl(TTSSynthText)
	su_proc_decl(TTSFetchNext)
	su_proc_decl(TTSSynthTextEx)
	su_proc_decl(TTSSynthText2File)
	su_proc_decl(TTSClean)
	su_proc_decl(TTSGetParam)
	su_proc_decl(TTSSetParam)
	su_proc_decl(TTSLoadUserLib)
	su_proc_decl(TTSUnloadUserLib)
	su_proc_decl(TTSFormatMessage)
	su_proc_decl(TTSAbout)
	su_proc_decl(TTSInitialize)
	su_proc_decl(TTSGetSynthParam)
	su_proc_decl(TTSSetSynthParam)

	su_proc_decl(SEMFindFirstEngine)
	su_proc_decl(SEMFindNextEngine)
	su_proc_decl(SEMFindFirstVoice)
	su_proc_decl(SEMFindNextVoice)
	su_proc_decl(SEMFindClose)
	su_proc_decl(SEMGetEngineInfo)
	su_proc_decl(SEMEngineCtrl)
	};

/** 
 * @class	tts_knl
 * @brief	TTS Kernel库的加载和卸载
 * 
 *  只能加载4.0、5.0及以后的内核库版本。
 * 
 * @author	jdyu
 * @date	2008/4/24
 */
class tts_knl
	: public sp::su_lib<>
	{
public:
	typedef sp::su_lib<> parent;

	tts_knl(const char * lib = 0)
		: parent		(lib)
		, tts_engine_	(0)
		{
		}
	virtual ~tts_knl(void)
		{
		close();
		}
	ISEMEngine_1_0 * tts_engnie(void)
		{
		return tts_engine_;
		}

	int close(void)
		{
		if ( tts_engine_ )
			{
			int ret = this->DestroyEngine()((ISEMEngine*)tts_engine_);
			SP_ASSERT(ret==0);
			tts_engine_ = 0;
			}
		return parent::close();
		}

protected:
	// 加载函数
	virtual int load_procs(void)
		{
		su_proc_load(CreateEngine)
		su_proc_load(DestroyEngine)
		
		int ret = this->CreateEngine()(0, (ISEMEngine**)&tts_engine_);
		return ret;
		}

private:
	su_proc_decl(CreateEngine)
	su_proc_decl(DestroyEngine)

	// 引擎接口
	ISEMEngine_1_0 * tts_engine_;
	};

// 快速接口
class qtts_lib
	: public sp::su_lib<>
	{
public:
	qtts_lib(const char * lib = 0)
		: sp::su_lib<>(lib)
		{
		}

protected:
	virtual int load_procs(void)
		{
		su_proc_load(QTTSSessionBegin)
		su_proc_load(QTTSTextPut)
		su_proc_load(QTTSAudioGet)
		su_proc_load(QTTSSessionEnd)
		su_proc_load(QTTSSynthToFile)
		su_proc_load(QTTSInit)
		su_proc_load(QTTSFini)
		su_proc_load(QTTSGetParam)
		su_proc_load(QTTSLogEvent)

#ifdef MSP_WCHAR_SUPPORT
		su_proc_load(QTTSSessionBeginW)
		su_proc_load(QTTSTextPutW)
		su_proc_load(QTTSAudioGetW)
		su_proc_load(QTTSSessionEndW)
		su_proc_load(QTTSSynthToFileW)
		su_proc_load(QTTSInitW)
		su_proc_load(QTTSGetParamW)
		su_proc_load(QTTSLogEventW)
#endif
		return 0;
		}

private:
	su_proc_decl(QTTSSessionBegin)
	su_proc_decl(QTTSTextPut)
	su_proc_decl(QTTSAudioGet)
	su_proc_decl(QTTSSessionEnd)
	su_proc_decl(QTTSSynthToFile)
	su_proc_decl(QTTSInit)
	su_proc_decl(QTTSFini)
	su_proc_decl(QTTSGetParam)
	su_proc_decl(QTTSLogEvent)
	
#ifdef MSP_WCHAR_SUPPORT
	su_proc_decl(QTTSSessionBeginW)
	su_proc_decl(QTTSTextPutW)
	su_proc_decl(QTTSAudioGetW)
	su_proc_decl(QTTSSessionEndW)
	su_proc_decl(QTTSSynthToFileW)
	su_proc_decl(QTTSInitW)
	su_proc_decl(QTTSGetParamW)
	su_proc_decl(QTTSLogEventW)
#endif
	};

} /* namespace sp */

#endif /* __TTSLIB_H__ */
