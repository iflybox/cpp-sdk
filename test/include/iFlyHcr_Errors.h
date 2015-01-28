#ifndef __IFLYHCR_ERRORS_H__
#define __IFLYHCR_ERRORS_H__

enum
{
    /* errors occur in iFlyHcrModuleCreate */
    iFlyHcr_ERROR_ModuleCreate_INVPARAM          = 5001, /* 0x1389 */
    iFlyHcr_ERROR_ResourceCreate_FAIL            = 5002, /* 0x138A */
    iFlyHcr_ERROR_CreateInstance_FAIL            = 5003, /* 0x138B */

    /* errors occur in iFlyHcrModuleDestroy */
    iFlyHcr_ERROR_ModuleDestroy_INVINST          = 5004, /* 0x138C */
    iFlyHcr_ERROR_DestroyInstance_FAIL           = 5005, /* 0x138D */

    /* errors occur in iFlyHcrModuleStart */
    iFlyHcr_ERROR_ModuleStart_INVINST            = 5006, /* 0x138E */
    iFlyHcr_ERROR_ExitRecog_FAIL                 = 5007, /* 0x138F */
    iFlyHcr_ERROR_SetParam_FAIL                  = 5008, /* 0x1390 */
    iFlyHcr_ERROR_SetWritingArea_FAIL            = 5009, /* 0x1391 */
    iFlyHcr_ERROR_InitRecog_FAIL                 = 5010, /* 0x1392 */

    /* errors occur in iFlyHcrModuleAppendStrokes */
    iFlyHcr_ERROR_AppendStrokes_INVINST          = 5011, /* 0x1393 */
    iFlyHcr_ERROR_AppendStrokes_LastCall_FAIL    = 5012, /* 0x1394 */
    iFlyHcr_ERROR_AppendStrokes_ILLEGALPOINTS    = 5013, /* 0x1395 */
    iFlyHcr_ERROR_AppendStrokes_INVPOINT         = 5014, /* 0x1396 */
    iFlyHcr_ERROR_ProcessPoints_FAIL             = 5015, /* 0x1397 */
    iFlyHcr_ERROR_DIVISION_EXCEPTION             = 5016, /* 0x1398 */
    iFlyHcr_ERROR_UpdateRecog_FAIL               = 5017, /* 0x1399 */

    /* errors occur in iFlyHcrModuleGetResult */
    iFlyHcr_ERROR_GetResult_INVINST              = 5018, /* 0x139A */
    iFlyHcr_ERROR_GetResult_LastCall_FAIL        = 5019, /* 0x139B */

    /* errors occur in iFlyHcrSetPredictData */
    iFlyHcr_ERROR_SetPredict_INVINPUT            = 5020, /* 0x139C */
    iFlyHcr_ERROR_SetPredict_INVINST             = 5021, /* 0x139D */
    iFlyHcr_ERROR_SetPredict_LastCall_FAIL       = 5022, /* 0x139E */
    iFlyHcr_ERROR_SetPredict_ILLEGALINPUT        = 5026, /* 0x13A2 */

    /* errors occur in iFlyHcrGetPredictResult */
    iFlyHcr_ERROR_GetPredict_INVINST             = 5023, /* 0x139F */
    iFlyHcr_ERROR_GetPredict_LastCall_FAIL       = 5024, /* 0x13A0 */
    iFlyHcr_ERROR_GetPredict_INVCALL             = 5025, /* 0x13A1 */
};


#endif /* __IFLYHCR_ERRORS_H__ */