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
    /* TTS_IN_FLAG_SYLLLIST_MODE    音节链模式 */
    TTS_IN_FLAG_SYNTHCELLLIST_MODE      = 1,
    /* TTS_IN_FLAG_VUFMSELECT_MODE  虚拟不定长挑选模式 */
    TTS_IN_FLAG_VUFMSELECT_MODE         = 2
};

/** 
 * 保留区段的开始
 */
#define TTS_PARAM_RESERVED_BASE           0x0A00

/** 
 * 未公开的一些参数类型
 */
enum
{
    /* 获得当前实例的资源包 */
    TTS_PARAM_CURPACKAGE                = (TTS_PARAM_RESERVED_BASE + 0x1),
    /* 获取后端处理的音节链跟Viterbi的矩阵 */
    /* 目的是提供挑选虚拟不定长的信息 */
    TTS_PARAM_VUFMINFO                  = (TTS_PARAM_RESERVED_BASE + 0x2),

    /* 设置本段音频的播放总时长 */
    TTS_PARAM_AUDTOTALTIME              = (TTS_PARAM_RESERVED_BASE + 0x10),

    /* 设置开头播放的静音长度 */
    TTS_PARAM_GAPHEAD                   = (TTS_PARAM_LOCAL_BASE    + 22),
    /* 设置结尾播放的静音长度 */
    TTS_PARAM_GAPTAIL                   = (TTS_PARAM_LOCAL_BASE    + 24),
};

/** 
 * 未公开的参数取值
 */

/* 唇形数据头的有关定义 */
/* 输出唇形数据头标识(使用TTS_PARAM_AUDIODATAFMT时) */
enum
{
    TTS_AHF_LIPINFO                     = 20
};

/* TTSAbout可以使用的查询类型 */
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
