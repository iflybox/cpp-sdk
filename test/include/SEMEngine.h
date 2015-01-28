#ifndef __SEM_ENGINE_H__
#define __SEM_ENGINE_H__

#include "SEM_Spec.h"

#define ISEMENGINE_VERSION			0x00010001

/************************************************************************/
/* 1.1版本的接口                                                        */
/************************************************************************/

typedef struct ISEMEngine
{
	/************************************************************************/
	/* 引擎信息查询接口                                                     */
	/************************************************************************/
	virtual TTSDWORD		Version() = 0;
	virtual const char *	EngineName() = 0;
	virtual PTVoiceInfo		VoiceInfo(TTSDWORD* pdwCount) = 0;
	virtual PTEngineInfoSta	EngineInfo() = 0;
	/* 检查引擎的状态，解决Vocalizer服务器意外关闭检测的问题 */
	virtual TTSDWORD		CheckStatus(TTSDWORD dwType) = 0;

	/************************************************************************/
	/* 精简iFly TTS SDK 1.2接口                                             */
	/************************************************************************/
	virtual TTSRETVAL		Init(const char *pszEngine, void *pvReserved) = 0;
	virtual TTSRETVAL		Fini() = 0;
	virtual HTTSINSTANCE	Connect(PTTSConnectStruct pConnect) = 0;
	virtual TTSRETVAL		Disconnect(HTTSINSTANCE hTTSInst) = 0;
	virtual TTSRETVAL		Synth(HTTSINSTANCE hTTSInst, PTTSData pTTSData) = 0;
	virtual TTSRETVAL		Fetch(HTTSINSTANCE hTTSInst, PTTSData pTTSData) = 0;
	virtual TTSRETVAL		Clean(HTTSINSTANCE hTTSInst) = 0;
	virtual TTSRETVAL		GetParam(HTTSINSTANCE hTTSInst, TTSDWORD dwParamType, void *pParam, TTSINT32 *pnSize) = 0;
	virtual TTSRETVAL		SetParam(HTTSINSTANCE hTTSInst, TTSDWORD dwParamType, void *pParam, TTSINT32 nSize) = 0;
	virtual TTSRETVAL		LoadUserLib(HTTSINSTANCE hTTSInst, HTTSUSERLIB *lphUserLib, PTTSVOID pUserLib, TTSINT32 nSize) = 0;
	virtual TTSRETVAL		UnloadUserLib(HTTSINSTANCE hTTSInst, HTTSUSERLIB hUserLib) = 0;
} * PISEMEngine;

/************************************************************************/
/* 1.0版本的接口，为兼容目的保留                                        */
/************************************************************************/

#define ISEMENGINE_VERSION_1_0		0x00010000

typedef struct ISEMEngine_1_0
{
	/************************************************************************/
	/* 引擎信息查询接口                                                     */
	/************************************************************************/
	virtual TTSDWORD		Version() = 0;
	virtual const char *	EngineName() = 0;
	virtual PTVoiceInfo		VoiceInfo(TTSDWORD* pdwCount) = 0;
	virtual PTEngineInfoSta	EngineInfo() = 0;

	/************************************************************************/
	/* 精简iFly TTS SDK 1.2接口                                             */
	/************************************************************************/
	virtual TTSRETVAL		Init(const char *pszEngine, void *pvReserved) = 0;
	virtual TTSRETVAL		Fini() = 0;
	virtual HTTSINSTANCE	Connect(PTTSConnectStruct pConnect) = 0;
	virtual TTSRETVAL		Disconnect(HTTSINSTANCE hTTSInst) = 0;
	virtual TTSRETVAL		Synth(HTTSINSTANCE hTTSInst, PTTSData pTTSData) = 0;
	virtual TTSRETVAL		Fetch(HTTSINSTANCE hTTSInst, PTTSData pTTSData) = 0;
	virtual TTSRETVAL		Clean(HTTSINSTANCE hTTSInst) = 0;
	virtual TTSRETVAL		GetParam(HTTSINSTANCE hTTSInst, TTSDWORD dwParamType, void *pParam, TTSINT32 *pnSize) = 0;
	virtual TTSRETVAL		SetParam(HTTSINSTANCE hTTSInst, TTSDWORD dwParamType, void *pParam, TTSINT32 nSize) = 0;
	virtual TTSRETVAL		LoadUserLib(HTTSINSTANCE hTTSInst, HTTSUSERLIB *lphUserLib, PTTSVOID pUserLib, TTSINT32 nSize) = 0;
	virtual TTSRETVAL		UnloadUserLib(HTTSINSTANCE hTTSInst, HTTSUSERLIB hUserLib) = 0;
	/* 以下两个函数引擎可以不实现，直接返回TTSERR_NOTSUPP即可 */
	virtual TTSRETVAL		SynthEx(HTTSINSTANCE hTTSInst, PTTSData pTTSData, PTTSCallBacks pTTSCallBacks, TTSBOOL bASynch, PTTSVOID pUserData) = 0;
	virtual TTSRETVAL		Synth2File(HTTSINSTANCE hTTSInst, PTTSData pTTSData, const char* pszOutFile, PTTSCallBacks pTTSCallBacks, TTSBOOL bASynch, PTTSVOID pUserData) = 0;
} * PISEMEngine1_0;

#if defined(__cplusplus)
extern "C" {
#endif

// 创建接口
TTSRETVAL TTSLIBAPI CreateEngine(TTSDWORD dwParam, PISEMEngine *ppInterface);
typedef TTSRETVAL TTSLIBAPI (*Proc_CreateEngine)(TTSDWORD dwParam, PISEMEngine *ppInterface);

// 销毁接口
TTSRETVAL TTSLIBAPI DestroyEngine(PISEMEngine pInterface);
typedef TTSRETVAL TTSLIBAPI (*Proc_DestroyEngine)(PISEMEngine pInterface);

#if defined(__cplusplus)
}                        /* terminate the extern "C" for c plus plus */
#endif

#endif	/* __SEM_ENGINE_H__ */
