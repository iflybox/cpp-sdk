#ifndef __SEM_ENGINE_H__
#define __SEM_ENGINE_H__

#include "SEM_Spec.h"

#define ISEMENGINE_VERSION			0x00010001

/************************************************************************/
/* 1.1�汾�Ľӿ�                                                        */
/************************************************************************/

typedef struct ISEMEngine
{
	/************************************************************************/
	/* ������Ϣ��ѯ�ӿ�                                                     */
	/************************************************************************/
	virtual TTSDWORD		Version() = 0;
	virtual const char *	EngineName() = 0;
	virtual PTVoiceInfo		VoiceInfo(TTSDWORD* pdwCount) = 0;
	virtual PTEngineInfoSta	EngineInfo() = 0;
	/* ��������״̬�����Vocalizer����������رռ������� */
	virtual TTSDWORD		CheckStatus(TTSDWORD dwType) = 0;

	/************************************************************************/
	/* ����iFly TTS SDK 1.2�ӿ�                                             */
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
/* 1.0�汾�Ľӿڣ�Ϊ����Ŀ�ı���                                        */
/************************************************************************/

#define ISEMENGINE_VERSION_1_0		0x00010000

typedef struct ISEMEngine_1_0
{
	/************************************************************************/
	/* ������Ϣ��ѯ�ӿ�                                                     */
	/************************************************************************/
	virtual TTSDWORD		Version() = 0;
	virtual const char *	EngineName() = 0;
	virtual PTVoiceInfo		VoiceInfo(TTSDWORD* pdwCount) = 0;
	virtual PTEngineInfoSta	EngineInfo() = 0;

	/************************************************************************/
	/* ����iFly TTS SDK 1.2�ӿ�                                             */
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
	/* ������������������Բ�ʵ�֣�ֱ�ӷ���TTSERR_NOTSUPP���� */
	virtual TTSRETVAL		SynthEx(HTTSINSTANCE hTTSInst, PTTSData pTTSData, PTTSCallBacks pTTSCallBacks, TTSBOOL bASynch, PTTSVOID pUserData) = 0;
	virtual TTSRETVAL		Synth2File(HTTSINSTANCE hTTSInst, PTTSData pTTSData, const char* pszOutFile, PTTSCallBacks pTTSCallBacks, TTSBOOL bASynch, PTTSVOID pUserData) = 0;
} * PISEMEngine1_0;

#if defined(__cplusplus)
extern "C" {
#endif

// �����ӿ�
TTSRETVAL TTSLIBAPI CreateEngine(TTSDWORD dwParam, PISEMEngine *ppInterface);
typedef TTSRETVAL TTSLIBAPI (*Proc_CreateEngine)(TTSDWORD dwParam, PISEMEngine *ppInterface);

// ���ٽӿ�
TTSRETVAL TTSLIBAPI DestroyEngine(PISEMEngine pInterface);
typedef TTSRETVAL TTSLIBAPI (*Proc_DestroyEngine)(PISEMEngine pInterface);

#if defined(__cplusplus)
}                        /* terminate the extern "C" for c plus plus */
#endif

#endif	/* __SEM_ENGINE_H__ */
