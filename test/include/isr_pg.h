#ifndef __ISR_PG_H__
#define __ISR_PG_H__

#include "isr_rec.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
/// 以下为扩展PG接口，实现个人语法的加载与保存功能
//////////////////////////////////////////////////////////////////////////

/* Personal Grammar Handle */
typedef void* ISR_PG_INST;

typedef struct
{
	const wchar_t*	phraseID;		/* 语音标签的id */
	const wchar_t*	phraseNL;		/* 语音标签的含义 */
	int				weight;			/* 语音标签的权重 */
	const wchar_t*	phoneme;		/* 语音标签的发音 */
}ISRPGPhrase;

/**
* 加载个人语法URI，个人语法的操作都需要使用该函数返回的句柄
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
* @param grURI         Personal grammar uri to load
* @param phraseID      Phrase ID to be add, set NULL if no add operation
*
* @returns
*  0:
*    Personal Grammar was successfully loaded.
*  else:
*    Load Personal Grammar failed， return error code.
*/
int ISRAPI ISRLoadPersonalGrammar(ISR_PG_INST* pg, const wchar_t* grURI, ISRPGPhrase* new_phrase, int num_min_needed);
typedef int (ISRAPI *Proc_ISRLoadPersonalGrammar)(ISR_PG_INST* pg, const wchar_t* grURI, ISRPGPhrase* new_phrase, int num_min_needed);

/**
* 结束本次个人语法操作，同时保存个人语法URI，个人语法的操作都需要使用该函数返回的句柄
*
* @param pg            Personal grammar handle as returned
* @param bSave         Save modification or not
*
* @returns
*  0:
*    Personal Grammar was successfully unloaded.
*  else:
*    UnLoad Personal Grammar failed， return error code.
*/
int ISRAPI ISRUnLoadPersonalGrammar(ISR_PG_INST pg, int bSave);
typedef int (ISRAPI *Proc_ISRUnLoadPersonalGrammar)(ISR_PG_INST pg, int bSave);

/**
* 根据个人语法生成clash校验的识别语法
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
*
* @returns
*  0:
*    New Phrase was successfully added.
*  else:
*    Add new Phrase failed， return error code.
*/
int ISRAPI ISRBuildPersonalGrammar(ISR_PG_INST pg, wchar_t* gr_content, int* len);
typedef int (ISRAPI *Proc_ISRBuildPersonalGrammar)(ISR_PG_INST pg, wchar_t* gr_content, int* len);

/**
* 解析语音标签处理结果，以添加新标签
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
* @param ve_result     VE Result to be process
*
* @returns
*  0:
*    New Phrase was successfully added.
*  else:
*    no new Phrase added， return error code.
*/
int ISRAPI ISRProcessPersonalGrammar(ISR_PG_INST pg, const wchar_t* ve_result);
typedef int (ISRAPI *Proc_ISRProcessPersonalGrammar)(ISR_PG_INST pg,  const wchar_t* ve_result);

/**
* 取消最新添加的语音标签，该方法不会回溯，只会取消最近的一次
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
*
* @returns
*  0:
*    Personal Grammar was successfully loaded.
*  else:
*    Rollback failed， return error code.
*/
int ISRAPI ISRRollBackPersonalGrammar(ISR_PG_INST pg);
typedef int (ISRAPI *Proc_ISRRollBackPersonalGrammar)(ISR_PG_INST pg);

/**
* 修改语音标签的信息，包括id、nl、weight
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
* @param phraseID      Phrase id to be modified
* @param newPhrase     Modified info of phrase
*
* @returns
*  0:
*    Personal Grammar was successfully loaded.
*  else:
*    Modify Phrase failed， return error code.
*/
int ISRAPI ISRModifyPersonalGrammar(ISR_PG_INST pg, const wchar_t* phraseID, ISRPGPhrase* newPhrase);
typedef int (ISRAPI *Proc_ISRModifyPersonalGrammar)(ISR_PG_INST pg, const wchar_t* phraseID, ISRPGPhrase* newPhrase);

/**
* 删除指定的语音标签
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
* @param phraseID      Phrase ID to be deleted
*
* @returns
*  0:
*    Personal Grammar was successfully loaded.
*  else:
*    Delete Phrase failed， return error code.
*/
int ISRAPI ISRDeletePersonalGrammar(ISR_PG_INST pg, const wchar_t* phraseID);
typedef int (ISRAPI *Proc_ISRDeletePersonalGrammar)(ISR_PG_INST pg, const wchar_t* phraseID);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __ISR_PG_H__
