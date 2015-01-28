/** 
 * @file	REMEngine.h
 * @brief	
 * 
 * detail...
 * 
 * @author	hjye
 * @version	1.0
 * @date	2006��8��15��
 * 
 * @see		
 * 
 * @par �汾��¼��
 * <table border=1>
 *  <tr> <th>�汾	<th>����			<th>����	<th>��ע </tr>
 *  <tr> <td>1.0	<td>2006��8��15��	<td>hjye	<td>���� </tr>
 * </table>
 */

#ifndef __REM_ENGINE_H__
#define __REM_ENGINE_H__

#include "iFly_REM.h"

#define IREMENGINE_VERSION			0x00010000

/************************************************************************/
/* 1.0�汾�Ľӿ�                                                        */
/************************************************************************/

typedef struct IREMEngine
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
	/* REM Engine�����ӿ�                                             */
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
} * PIREMEngine;

#if defined(__cplusplus)
extern "C" {
#endif

// �����ӿ�
TTSRETVAL TTSLIBAPI RemCreateEngine(TTSDWORD dwParam, PIREMEngine *ppInterface);
typedef TTSRETVAL TTSLIBAPI (*Proc_RemCreateEngine)(TTSDWORD dwParam, PIREMEngine *ppInterface);

// ���ٽӿ�
TTSRETVAL TTSLIBAPI RemDestroyEngine(PIREMEngine pInterface);
typedef TTSRETVAL TTSLIBAPI (*Proc_RemDestroyEngine)(PIREMEngine pInterface);

#if defined(__cplusplus)
} /* terminate the extern "C" for c plus plus */
#endif

#endif	/* __REM_ENGINE_H__ */
