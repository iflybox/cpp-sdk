/**
 * @file    TTS_Spec.h
 * @brief   iFLY TTS SDK private header file for special requirements
 * 
 * This file contains the TTS application programming interface(API) declarations 
 * of iFLYTEK. TTS developer can include this file in your project to build applications.
 * For more information, please read the developer guide.
 * Copyright (C)    1999 - 2004 by ANHUI USTC IFLYTEK. Co.,LTD.
 *                  All rights reserved.
 * 
 * @author  Speech Dept. iFLYTEK.
 * @version 1.2
 * @date    2001/02/06
 * 
 * @see     iFly_TTS.h
 * 
 * Histroy:
 * index    version        date            author        notes
 * 0        1.1            2001/02/06      jdyu          Create this file
 * 0        1.1            2002/12/13      jdyu          Add DSS defines
 */
#ifndef __TTS_SPEC__
#define __TTS_SPEC__

#include "iFly_TTS.h"

/* Private TTS In flags */
enum
{
    /* TTS_IN_FLAG_SYLLLIST_MODE    ������ģʽ */
    TTS_IN_FLAG_SYNTHCELLLIST_MODE      = 1,
    /* TTS_IN_FLAG_VUFMSELECT_MODE  ���ⲻ������ѡģʽ */
    TTS_IN_FLAG_VUFMSELECT_MODE         = 2
};

/** 
 * �������εĿ�ʼ
 */
#define TTS_PARAM_RESERVED_BASE           0x0A00

/** 
 * δ������һЩ��������
 */
enum
{
    /* ��õ�ǰʵ������Դ�� */
    TTS_PARAM_CURPACKAGE                = (TTS_PARAM_RESERVED_BASE + 0x1),
    /* ��ȡ��˴������������Viterbi�ľ��� */
    /* Ŀ�����ṩ��ѡ���ⲻ��������Ϣ */
    TTS_PARAM_VUFMINFO                  = (TTS_PARAM_RESERVED_BASE + 0x2),

    /* ���ñ�����Ƶ�Ĳ�����ʱ�� */
    TTS_PARAM_AUDTOTALTIME              = (TTS_PARAM_RESERVED_BASE + 0x10),

    /* ���ÿ�ͷ���ŵľ������� */
    TTS_PARAM_GAPHEAD                   = (TTS_PARAM_LOCAL_BASE    + 22),
    /* ���ý�β���ŵľ������� */
    TTS_PARAM_GAPTAIL                   = (TTS_PARAM_LOCAL_BASE    + 24),
};

/** 
 * δ�����Ĳ���ȡֵ
 */

/* ��������ͷ���йض��� */
/* �����������ͷ��ʶ(ʹ��TTS_PARAM_AUDIODATAFMTʱ) */
enum
{
    TTS_AHF_LIPINFO                     = 20
};

/* TTSAbout����ʹ�õĲ�ѯ���� */
enum
{
    TTS_ABOUT_PRODUCTNAME               = 1,
    TTS_ABOUT_COPYRIGHT                 = 2,
    TTS_ABOUT_AUTHOR                    = 3,
    TTS_ABOUT_BUILDDATE                 = 4,
    TTS_ABOUT_VERMAJOR                  = 5,
    TTS_ABOUT_VERMINOR                  = 6,
    TTS_ABOUT_VERBUILD                  = 7,
    TTS_ABOUT_LICNUMBER                 = 8
};

#endif /* #ifndef __TTS_SPEC__ */
