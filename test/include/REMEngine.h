/** 
 * @file	REMEngine.h
 * @brief	
 * 
 * detail...
 * 
 * @author	hjye
 * @version	1.0
 * @date	2006年8月15日
 * 
 * @see		
 * 
 * @par 版本记录：
 * <table border=1>
 *  <tr> <th>版本	<th>日期			<th>作者	<th>备注 </tr>
 *  <tr> <td>1.0	<td>2006年8月15日	<td>hjye	<td>创建 </tr>
 * </table>
 */

#ifndef __REM_ENGINE_H__
#define __REM_ENGINE_H__

#include "iFly_REM.h"

#define IREMENGINE_VERSION			0x00010000

/************************************************************************/
/* 1.0版本的接口                                                        */
/************************************************************************/

typedef struct IREMEngine
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
	/* REM Engine操作接口                                             */
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

// 创建接口
TTSRETVAL TTSLIBAPI RemCreateEngine(TTSDWORD dwParam, PIREMEngine *ppInterface);
typedef TTSRETVAL TTSLIBAPI (*Proc_RemCreateEngine)(TTSDWORD dwParam, PIREMEngine *ppInterface);

// 销毁接口
TTSRETVAL TTSLIBAPI RemDestroyEngine(PIREMEngine pInterface);
typedef TTSRETVAL TTSLIBAPI (*Proc_RemDestroyEngine)(PIREMEngine pInterface);

#if defined(__cplusplus)
} /* terminate the extern "C" for c plus plus */
#endif

#endif	/* __REM_ENGINE_H__ */
