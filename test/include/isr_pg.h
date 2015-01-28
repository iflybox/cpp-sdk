#ifndef __ISR_PG_H__
#define __ISR_PG_H__

#include "isr_rec.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
/// ����Ϊ��չPG�ӿڣ�ʵ�ָ����﷨�ļ����뱣�湦��
//////////////////////////////////////////////////////////////////////////

/* Personal Grammar Handle */
typedef void* ISR_PG_INST;

typedef struct
{
	const wchar_t*	phraseID;		/* ������ǩ��id */
	const wchar_t*	phraseNL;		/* ������ǩ�ĺ��� */
	int				weight;			/* ������ǩ��Ȩ�� */
	const wchar_t*	phoneme;		/* ������ǩ�ķ��� */
}ISRPGPhrase;

/**
* ���ظ����﷨URI�������﷨�Ĳ�������Ҫʹ�øú������صľ��
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
* @param grURI         Personal grammar uri to load
* @param phraseID      Phrase ID to be add, set NULL if no add operation
*
* @returns
*  0:
*    Personal Grammar was successfully loaded.
*  else:
*    Load Personal Grammar failed�� return error code.
*/
int ISRAPI ISRLoadPersonalGrammar(ISR_PG_INST* pg, const wchar_t* grURI, ISRPGPhrase* new_phrase, int num_min_needed);
typedef int (ISRAPI *Proc_ISRLoadPersonalGrammar)(ISR_PG_INST* pg, const wchar_t* grURI, ISRPGPhrase* new_phrase, int num_min_needed);

/**
* �������θ����﷨������ͬʱ��������﷨URI�������﷨�Ĳ�������Ҫʹ�øú������صľ��
*
* @param pg            Personal grammar handle as returned
* @param bSave         Save modification or not
*
* @returns
*  0:
*    Personal Grammar was successfully unloaded.
*  else:
*    UnLoad Personal Grammar failed�� return error code.
*/
int ISRAPI ISRUnLoadPersonalGrammar(ISR_PG_INST pg, int bSave);
typedef int (ISRAPI *Proc_ISRUnLoadPersonalGrammar)(ISR_PG_INST pg, int bSave);

/**
* ���ݸ����﷨����clashУ���ʶ���﷨
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
*
* @returns
*  0:
*    New Phrase was successfully added.
*  else:
*    Add new Phrase failed�� return error code.
*/
int ISRAPI ISRBuildPersonalGrammar(ISR_PG_INST pg, wchar_t* gr_content, int* len);
typedef int (ISRAPI *Proc_ISRBuildPersonalGrammar)(ISR_PG_INST pg, wchar_t* gr_content, int* len);

/**
* ����������ǩ��������������±�ǩ
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
* @param ve_result     VE Result to be process
*
* @returns
*  0:
*    New Phrase was successfully added.
*  else:
*    no new Phrase added�� return error code.
*/
int ISRAPI ISRProcessPersonalGrammar(ISR_PG_INST pg, const wchar_t* ve_result);
typedef int (ISRAPI *Proc_ISRProcessPersonalGrammar)(ISR_PG_INST pg,  const wchar_t* ve_result);

/**
* ȡ��������ӵ�������ǩ���÷���������ݣ�ֻ��ȡ�������һ��
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
*
* @returns
*  0:
*    Personal Grammar was successfully loaded.
*  else:
*    Rollback failed�� return error code.
*/
int ISRAPI ISRRollBackPersonalGrammar(ISR_PG_INST pg);
typedef int (ISRAPI *Proc_ISRRollBackPersonalGrammar)(ISR_PG_INST pg);

/**
* �޸�������ǩ����Ϣ������id��nl��weight
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
* @param phraseID      Phrase id to be modified
* @param newPhrase     Modified info of phrase
*
* @returns
*  0:
*    Personal Grammar was successfully loaded.
*  else:
*    Modify Phrase failed�� return error code.
*/
int ISRAPI ISRModifyPersonalGrammar(ISR_PG_INST pg, const wchar_t* phraseID, ISRPGPhrase* newPhrase);
typedef int (ISRAPI *Proc_ISRModifyPersonalGrammar)(ISR_PG_INST pg, const wchar_t* phraseID, ISRPGPhrase* newPhrase);

/**
* ɾ��ָ����������ǩ
*
* @param pg            Handle as returned by ISRLoadPersonalGrammar()
* @param phraseID      Phrase ID to be deleted
*
* @returns
*  0:
*    Personal Grammar was successfully loaded.
*  else:
*    Delete Phrase failed�� return error code.
*/
int ISRAPI ISRDeletePersonalGrammar(ISR_PG_INST pg, const wchar_t* phraseID);
typedef int (ISRAPI *Proc_ISRDeletePersonalGrammar)(ISR_PG_INST pg, const wchar_t* phraseID);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // __ISR_PG_H__
