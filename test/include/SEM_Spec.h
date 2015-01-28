/** 
 * @file    SEM.h
 * @brief   iFLY SEM SDK inner header file
 * 
 * This file contains the inner SEM application programming interface(API) declarations
 * of iFLYTEK. Advanced SEM developer can include this file in your project to build applications.
 * For more information, please read the developer guide.
 * Copyright (C)    1999 - 2004 by ANHUI USTC IFLYTEK. Co.,LTD.
 *                  All rights reserved.
 * 
 * @author    Speech Dept. iFLYTEK.
 * @version   1.0
 * @date      2004/05/31
 * 
 * @see       iFly_SEM.h
 * 
 * @par History:
 * <table border=1>
 *  <tr> <index> <th>version  <th>date       <th>author  <th>notes             </tr>
 *  <tr> <0>     <td>1.0      <td>2004/05/31 <td>hjye    <td>Create this file  </tr>
 * </table>
 */

#ifndef IFLY_SEM_INNER_H
#define IFLY_SEM_INNER_H

#include "iFly_SEM.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Basic Data Types
 */


/*
 * SEM common defines
 */

#define SEM_ENGINEVER_LEN_MAX				32
#define SEM_PRODUCERNAME_LEN_MAX            64
#define SEM_INSTPATH_LEN_MAX                1024
#define SEM_CODEPAGE_NUM_MAX                32
#define SEM_MARKLANG_NUM_MAX                32
#define SEM_USERLIB_LEN_MAX                 256

/* Engine type (domain) */
enum
{
    SEM_ENGINE_TYPE_UNKN,                   /* Unknown engine type */
    SEM_ENGINE_TYPE_NUMBER,                 /* Small numeric synthesise appropriation system */
    SEM_ENGINE_TYPE_UNIVERSAL,              /* Universal text synthesise system */
};

/* Engine status */
enum
{
    SEM_ENGINE_STATUS_RUNNING,              /* Service running */
    SEM_ENGINE_STATUS_BUSY,                 /* Service overloaded */
};

/* Information type of engine queried  */
/*
 *   1 -  19 : base static informations
 *  20 -  39 : base dynamic informations
 *  40 - 100 : reserved
 * 101 - 120 : extended static informations
 * 121 - 140 : extended dynamic informations
 * 141 - 199 : reserved
 * 200 - 299 : extended static engine informations
 * 300 - 399 : advanced informations
 */
enum
{
	/*   1 -  19 : base static informations */
    SEM_QUERY_EINFO_STATIC          = 19,   /* [TEngineInfoSta] Query all engine static info */

	/*  20 -  39 : base dynamic informations */
    SEM_QUERY_EINFO_DYNAMIC         = 39,   /* [TEngineInfoDyn] Query all engine dynamic info */

	/* 101 - 120 : extended static informations */
	SEM_QUERY_EINFO_TYPE            = 101,  /* [DWORD] Query engine type */
	SEM_QUERY_EINFO_BUILDDATE       = 102,  /* [String] Query build date of engine */

	/* 121 - 140 : extended dynamic informations */
	SEM_QUERY_EINFO_LIMITLICENSE    = 121,  /* [DWORD] Query soft limited license of engine */
    SEM_QUERY_EINFO_THROUGHPUT      = 122,  /* [DWORD] Query engine current throughput */
    SEM_QUERY_EINFO_STATREC         = 123,  /* [TEngineStatRec Array] Query all saved statistic records of engine */

	/* 300 - 399 : advanced informations */
	SEM_QUERY_EINFO_STATIC_STR      = 300,  /* [String] Query all engine static info */
	SEM_QUERY_EINFO_DYNAMIC_STR     = 301,  /* [TEngineInfoDyn] Query all engine dynamic info */
	SEM_QUERY_EINFO_VOICE_STR       = 302,  /* [String] Query all voice info in string format */
};

/*
 * SEM Data Structures
 */

#pragma pack(4)

/* Engine static information structure */
typedef struct
{
    TTSDWORD    cbSize;                                /* 结构大小 */
    TTSDWORD    dwType;                                /* 引擎类型 */
    TTSDWORD    dwStatus;                              /* 引擎状态 */
    TTSCHAR     szName[SEM_ENGINENAME_LEN_MAX];        /* 引擎名称 */
    TTSCHAR     szVer[SEM_ENGINEVER_LEN_MAX];          /* 引擎版本号 */
    TTSCHAR     szProducer[SEM_PRODUCERNAME_LEN_MAX];  /* 生产商名称 */
    TTSCHAR     szInstPath[SEM_INSTPATH_LEN_MAX];      /* 引擎安装路径 */
    TTSCHAR     szIP[TTS_IP_MAXLEN];                   /* 所在服务器的IP地址 */
    TTSDWORD    dwPid;                                 /* 所在服务器的pid */
    TTSDWORD    dwLicense;                             /* 引擎硬件授权数 */
    TTSDWORD    dwStdEfficency;                        /* 引擎的基准效率 */
    TTSDWORD    dwCPCount;                             /* 引擎支持的代码页数量 */
    TTSDWORD    arrCodepage[SEM_CODEPAGE_NUM_MAX];     /* 引擎支持的代码页ID数组 */
    TTSDWORD    dwMLCount;                             /* 引擎支持的标记语言数量 */
    TTSDWORD    arrMarklang[SEM_MARKLANG_NUM_MAX];     /* 引擎支持的标记语言ID数组 */
    TTSCHAR     szUserlib[SEM_USERLIB_LEN_MAX];        /* 引擎支持的定制资源包数组(以;分隔) */
    TTSDWORD    dwScore;                               /* 引擎的分数 */
    TTSDWORD    dwLangCount;                           /* 引擎支持的语种数量 */
    TTSDWORD    dwLanguage[SEM_VOICE_LANG_MAX];        /* 引擎支持的语种ID数组 */
    TTSDWORD    dwReserved;                            /* 保留 */
} TEngineInfoSta, *PTEngineInfoSta;

/* Engine dynamic information structure */
typedef struct
{
    TTSDWORD    cbSize;                                /* 结构大小 */
    TTSDWORD    dwStatus;                              /* 引擎状态 */
    TTSDWORD    dwTotalSvc;                            /* 引擎自启动以来总服务实例数 */
    TTSDWORD    dwTotalErr;                            /* 引擎自启动以来总出错次数 */
    double      dbTotalRecv;                           /* 引擎自启动以来总接收字节数 */
    double      dbTotalSend;                           /* 引擎自启动以来总发送字节数 */
    TTSDWORD    dwTotalEff;                            /* 引擎总体效率 */
    TTSDWORD    dwActEff;                              /* 引擎当前效率 */
    TTSDWORD    dwCurSvc;                              /* 引擎最近服务次数 */
    TTSDWORD    dwCurRecvRate;                         /* 引擎最近时间段接收字节速率 */
    TTSDWORD    dwCurSendRate;                         /* 引擎最近时间段发送字节速率 */
    TTSDWORD    dwCurErr;                              /* 引擎最近出错次数 */
    TTSDWORD    dwInstCount;                           /* 引擎当前活动实例数量 */
    TTSDWORD    dwReserved;                            /* 保留 */
} TEngineInfoDyn, *PTEngineInfoDyn;

/* Engine statistic record structure */
typedef struct
{
    TTSDWORD    dwSvcCount;                            /* 单位时间内服务的次数 */
    TTSDWORD    dwRecvBytes;                           /* 单位时间内接收的文本数据量 */
    TTSDWORD    dwSendBytes;                           /* 单位时间内发送的文本数据量 */
    TTSDWORD    dwErrCount;                            /* 单位时间内出错的次数 */
} TEngineStatRec, *PTEngineStatRec;

#pragma pack()

#pragma pack(2)

/* Engine static information structure compatible for iSP20, don't modify it */
typedef struct
{
	TTSWORD     cbSize;                                /* 结构大小 */
	TTSDWORD    dwType;                                /* 引擎类型 */
	TTSDWORD    dwStatus;                              /* 引擎状态 */
	TTSCHAR     szName[SEM_ENGINENAME_LEN_MAX];        /* 引擎名称 */
	TTSCHAR     szVer[SEM_ENGINEVER_LEN_MAX];          /* 引擎版本号 */
	TTSCHAR     szProducer[SEM_PRODUCERNAME_LEN_MAX];  /* 生产商名称 */
	TTSCHAR     szInstPath[SEM_INSTPATH_LEN_MAX];      /* 引擎安装路径 */
	TTSCHAR     szIP[TTS_IP_MAXLEN];                   /* 所在服务器的IP地址 */
	TTSDWORD    dwPid;                                 /* 所在服务器的pid */
	TTSDWORD    dwLicense;                             /* 引擎硬件授权数 */
	TTSDWORD    dwStdEfficency;                        /* 引擎的基准效率 */
	TTSDWORD    dwCPCount;                             /* 引擎支持的代码页数量 */
	TTSDWORD    arrCodepage[SEM_CODEPAGE_NUM_MAX];     /* 引擎支持的代码页ID数组 */
	TTSDWORD    dwMLCount;                             /* 引擎支持的标记语言数量 */
	TTSDWORD    arrMarklang[SEM_MARKLANG_NUM_MAX];     /* 引擎支持的标记语言ID数组 */
	TTSCHAR     szUserlib[SEM_USERLIB_LEN_MAX];        /* 引擎支持的定制资源包数组(以;分隔) */
	TTSDWORD    dwReserved;                            /* 保留 */
} TEngineInfoSta_20, *PTEngineInfoSta_20;

/* Engine dynamic information structure compatible for iSP20, don't modify it */
typedef struct
{
    TTSWORD     cbSize;                                /* 结构大小 */
    TTSDWORD    dwStatus;                              /* 引擎状态 */
    TTSDWORD    dwTotalSvc;                            /* 引擎自启动以来总服务实例数 */
    TTSDWORD    dwTotalErr;                            /* 引擎自启动以来总出错次数 */
    double      dbTotalRecv;                           /* 引擎自启动以来总接收字节数 */
    double      dbTotalSend;                           /* 引擎自启动以来总发送字节数 */
    TTSDWORD    dwTotalEff;                            /* 引擎总体效率 */
    TTSDWORD    dwActEff;                              /* 引擎当前效率 */
    TTSDWORD    dwCurSvc;                              /* 引擎最近服务次数 */
    TTSDWORD    dwCurRecvRate;                         /* 引擎最近时间段接收字节速率 */
    TTSDWORD    dwCurSendRate;                         /* 引擎最近时间段发送字节速率 */
    TTSDWORD    dwCurErr;                              /* 引擎最近出错次数 */
    TTSDWORD    dwReserved;                            /* 保留 */
} TEngineInfoDyn_20, *PTEngineInfoDyn_20;

#pragma pack()

#ifdef __cplusplus
}    /* extern "C" */
#endif

#endif    /* IFLY_SEM_INNER_H */
