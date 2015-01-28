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
    TTSDWORD    cbSize;                                /* �ṹ��С */
    TTSDWORD    dwType;                                /* �������� */
    TTSDWORD    dwStatus;                              /* ����״̬ */
    TTSCHAR     szName[SEM_ENGINENAME_LEN_MAX];        /* �������� */
    TTSCHAR     szVer[SEM_ENGINEVER_LEN_MAX];          /* ����汾�� */
    TTSCHAR     szProducer[SEM_PRODUCERNAME_LEN_MAX];  /* ���������� */
    TTSCHAR     szInstPath[SEM_INSTPATH_LEN_MAX];      /* ���氲װ·�� */
    TTSCHAR     szIP[TTS_IP_MAXLEN];                   /* ���ڷ�������IP��ַ */
    TTSDWORD    dwPid;                                 /* ���ڷ�������pid */
    TTSDWORD    dwLicense;                             /* ����Ӳ����Ȩ�� */
    TTSDWORD    dwStdEfficency;                        /* ����Ļ�׼Ч�� */
    TTSDWORD    dwCPCount;                             /* ����֧�ֵĴ���ҳ���� */
    TTSDWORD    arrCodepage[SEM_CODEPAGE_NUM_MAX];     /* ����֧�ֵĴ���ҳID���� */
    TTSDWORD    dwMLCount;                             /* ����֧�ֵı���������� */
    TTSDWORD    arrMarklang[SEM_MARKLANG_NUM_MAX];     /* ����֧�ֵı������ID���� */
    TTSCHAR     szUserlib[SEM_USERLIB_LEN_MAX];        /* ����֧�ֵĶ�����Դ������(��;�ָ�) */
    TTSDWORD    dwScore;                               /* ����ķ��� */
    TTSDWORD    dwLangCount;                           /* ����֧�ֵ��������� */
    TTSDWORD    dwLanguage[SEM_VOICE_LANG_MAX];        /* ����֧�ֵ�����ID���� */
    TTSDWORD    dwReserved;                            /* ���� */
} TEngineInfoSta, *PTEngineInfoSta;

/* Engine dynamic information structure */
typedef struct
{
    TTSDWORD    cbSize;                                /* �ṹ��С */
    TTSDWORD    dwStatus;                              /* ����״̬ */
    TTSDWORD    dwTotalSvc;                            /* ���������������ܷ���ʵ���� */
    TTSDWORD    dwTotalErr;                            /* ���������������ܳ������ */
    double      dbTotalRecv;                           /* ���������������ܽ����ֽ��� */
    double      dbTotalSend;                           /* ���������������ܷ����ֽ��� */
    TTSDWORD    dwTotalEff;                            /* ��������Ч�� */
    TTSDWORD    dwActEff;                              /* ���浱ǰЧ�� */
    TTSDWORD    dwCurSvc;                              /* �������������� */
    TTSDWORD    dwCurRecvRate;                         /* �������ʱ��ν����ֽ����� */
    TTSDWORD    dwCurSendRate;                         /* �������ʱ��η����ֽ����� */
    TTSDWORD    dwCurErr;                              /* �������������� */
    TTSDWORD    dwInstCount;                           /* ���浱ǰ�ʵ������ */
    TTSDWORD    dwReserved;                            /* ���� */
} TEngineInfoDyn, *PTEngineInfoDyn;

/* Engine statistic record structure */
typedef struct
{
    TTSDWORD    dwSvcCount;                            /* ��λʱ���ڷ���Ĵ��� */
    TTSDWORD    dwRecvBytes;                           /* ��λʱ���ڽ��յ��ı������� */
    TTSDWORD    dwSendBytes;                           /* ��λʱ���ڷ��͵��ı������� */
    TTSDWORD    dwErrCount;                            /* ��λʱ���ڳ���Ĵ��� */
} TEngineStatRec, *PTEngineStatRec;

#pragma pack()

#pragma pack(2)

/* Engine static information structure compatible for iSP20, don't modify it */
typedef struct
{
	TTSWORD     cbSize;                                /* �ṹ��С */
	TTSDWORD    dwType;                                /* �������� */
	TTSDWORD    dwStatus;                              /* ����״̬ */
	TTSCHAR     szName[SEM_ENGINENAME_LEN_MAX];        /* �������� */
	TTSCHAR     szVer[SEM_ENGINEVER_LEN_MAX];          /* ����汾�� */
	TTSCHAR     szProducer[SEM_PRODUCERNAME_LEN_MAX];  /* ���������� */
	TTSCHAR     szInstPath[SEM_INSTPATH_LEN_MAX];      /* ���氲װ·�� */
	TTSCHAR     szIP[TTS_IP_MAXLEN];                   /* ���ڷ�������IP��ַ */
	TTSDWORD    dwPid;                                 /* ���ڷ�������pid */
	TTSDWORD    dwLicense;                             /* ����Ӳ����Ȩ�� */
	TTSDWORD    dwStdEfficency;                        /* ����Ļ�׼Ч�� */
	TTSDWORD    dwCPCount;                             /* ����֧�ֵĴ���ҳ���� */
	TTSDWORD    arrCodepage[SEM_CODEPAGE_NUM_MAX];     /* ����֧�ֵĴ���ҳID���� */
	TTSDWORD    dwMLCount;                             /* ����֧�ֵı���������� */
	TTSDWORD    arrMarklang[SEM_MARKLANG_NUM_MAX];     /* ����֧�ֵı������ID���� */
	TTSCHAR     szUserlib[SEM_USERLIB_LEN_MAX];        /* ����֧�ֵĶ�����Դ������(��;�ָ�) */
	TTSDWORD    dwReserved;                            /* ���� */
} TEngineInfoSta_20, *PTEngineInfoSta_20;

/* Engine dynamic information structure compatible for iSP20, don't modify it */
typedef struct
{
    TTSWORD     cbSize;                                /* �ṹ��С */
    TTSDWORD    dwStatus;                              /* ����״̬ */
    TTSDWORD    dwTotalSvc;                            /* ���������������ܷ���ʵ���� */
    TTSDWORD    dwTotalErr;                            /* ���������������ܳ������ */
    double      dbTotalRecv;                           /* ���������������ܽ����ֽ��� */
    double      dbTotalSend;                           /* ���������������ܷ����ֽ��� */
    TTSDWORD    dwTotalEff;                            /* ��������Ч�� */
    TTSDWORD    dwActEff;                              /* ���浱ǰЧ�� */
    TTSDWORD    dwCurSvc;                              /* �������������� */
    TTSDWORD    dwCurRecvRate;                         /* �������ʱ��ν����ֽ����� */
    TTSDWORD    dwCurSendRate;                         /* �������ʱ��η����ֽ����� */
    TTSDWORD    dwCurErr;                              /* �������������� */
    TTSDWORD    dwReserved;                            /* ���� */
} TEngineInfoDyn_20, *PTEngineInfoDyn_20;

#pragma pack()

#ifdef __cplusplus
}    /* extern "C" */
#endif

#endif    /* IFLY_SEM_INNER_H */
