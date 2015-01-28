/** 
 * @file    iFly_SRM.h
 * @brief   iFLY Speech Resource Management SDK header file
 * 
 * This file contains the SRM application programming interface(API) declarations 
 * of iFLYTEK. iSP developer can include this file in your project to build applicstions.
 * For more information, please read the developer guide.
 * Copyright (C)    1999 - 2004 by ANHUI USTC IFLYTEK. Co.,LTD.
 *                    All rights reserved.
 * 
 * @author  Speech Dept. iFLYTEK.
 * @version 1.0
 * @date    2004/05/24
 * 
 * @see        
 * 
 * Histroy:
 * index    version        date            author        notes
 * 0        1.0            2004/05/24      jdyu          Create this file
 */

#ifndef _IFLY_SRM_H_
#define _IFLY_SRM_H_

#include "iFly_TTS.h"

#ifdef __cplusplus
extern "C" {
#endif

/* SRM export symbols */
#if defined(WIN32)
# ifdef SRM_EXPORTS
#  define SRMAPI __declspec(dllexport)
# else
#  define SRMAPI __declspec(dllimport)
# endif /* ifdef SRM_EXPORTS */
/* CALLBACK define */
# ifndef CALLBACK
#  define CALLBACK __stdcall
# endif /* ifndef CALLBACK */
#else    /* defined(WIN32) */
#  define SRMAPI
#  define CALLBACK
#endif /* defined(WIN32) */

/* basic types */
typedef void* SRM_HANDLE;

/* defines */
enum
{
	SRM_ALERT_DESC_LEN  = 200,
	SRM_ALERT_NAME_LEN  = 20,
};

/* Predefined value types of node information */
enum
{
    SRM_NONE    = 0,
    SRM_SZ      = 1,
    SRM_BINARY  = 3,
    SRM_DWORD   = 4,
};

/* tts service name */
static const char TTSSVC_NAME[] = "tts";

/**
 * @brief    Host types in iSP
 * include all host types in iSP platform
 */
typedef enum tagISP_HOST_TYPE
{
    host_null   = 0x0,         /* unknown host type */
    host_irsm   = 0x1,         /* master resource server */
    host_irsb   = 0x2,         /* backup resource server */
    host_irs    = 0x4,         /* resource server */
    host_iss    = 0x8,         /* speech server */
    host_ils    = 0x10,        /* log server */
    host_ics    = 0x20,        /* license server */
    host_isc    = 0x100,       /* speech client */
} ISP_HOST_TYPE;

/**
 * @brief   Process component types in iSP
 * include all press component types in iSP platform
 */
typedef enum tagISP_NODE_TYPE
{
    node_any    = 0,           /* any iSP node */
    node_srm    = 1,           /* speech resource manager */
    node_ses    = 2,           /* speech engines server */
    node_spw    = 4,           /* speech platform watcher */
    node_spc    = 8,           /* speech platform console */
    node_app    = 0x10,        /* speech applications */
	node_mrsv1  = 0x20,
	node_ltts	= 0x40,
	node_lasr	= 0x80,
	node_lisv	= 0x100,
    node_none   = -1,          /* none node */
} ISP_NODE_TYPE;

/**
 * @brief   System events in iSP
 * include all event id in iSP platform
 */
typedef enum tagISP_EVENT_ID
{
    SRM_EVENT_NULL                  = 0,
    SRM_EVENT_CONTROL,
    SRM_EVENT_ALERT,
    SRM_EVENT_IRSM_CHANGED,
    SRM_EVENT_IRSB_CHANGED,
    SRM_EVENT_NODE_ADDED,
    SRM_EVENT_NODE_REMOVED,
} ISP_EVENT_ID;

/**
 * @brief   Load balance status
 * include all node load balance status code
 */
enum
{
    SRM_LBSTATUS_UNKNOWN,
    SRM_LBSTATUS_ENABLE,
    SRM_LBSTATUS_DISABLE,
};

/**
 * @brief   Node control param
 * include all node control parameter
 */
enum
{
    SRM_NODECTRL_START              = 1,
    SRM_NODECTRL_STOP               = 2,
    SRM_NODECTRL_RESTART            = 3,
    SRM_NODECTRL_LBENABLE           = 4,
    SRM_NODECTRL_LBDISABLE          = 5,
    SRM_NODECTRL_STARTSVC           = 6,
    SRM_NODECTRL_STOPSVC            = 7,
};

/**
 * @brief	SRM monitor options
 *  monitor flag of SRMAddMonitor
 */
enum
{
    SRM_MONITOR_OPT_NONE            = 0x0,
    SRM_MONITOR_OPT_ALERT           = 0x1,
    SRM_MONITOR_OPT_UPDATE          = 0x2,
    SRM_MONITOR_OPT_CFG             = 0x4,
    SRM_MONITOR_OPT_SVC             = 0x8,
    SRM_MONITOR_OPT_ALL             = 0xFFFF,
};

/**
 * @brief   SRM node info type
 *  iSP Node info types used by SRMNodeQueryInfo
 */
enum
{
    SRM_NODEINFO_HOSTNAME           = 1,
    SRM_NODEINFO_OS                 = 2,
    SRM_NODEINFO_CPU                = 3,
    SRM_NODEINFO_MEM                = 4,
    SRM_NODEINFO_SVCPORT            = 5,
    SRM_NODEINFO_RRPORT             = 6,
    SRM_NODEINFO_EVPORT             = 7,
    SRM_NODEINFO_VERSION            = 8,
    SRM_NODEINFO_MODTIME            = 9,
    SRM_NODEINFO_LBSTATUS           = 10,
};

/**
 * @brief   Set IRS options
 *  the mode parameter of SRMSetIRS
 */
enum
{
    SRM_SETIRS_LOCAL               = 1,
    SRM_SETIRS_GLOBAL              = 2,
};

/* Configuration defines */
#define SRM_MAXSECTION_LEN          32
#define SRM_MAXKEY_LEN              32
#define SRM_MAXVALUE_LEN            252

/**
 * @brief   SRM configuration structure
 *  used in SRMSetParam data field
 */
typedef struct tagSRMConfig
{
    char    szSection[SRM_MAXSECTION_LEN];
    char    szKey    [SRM_MAXKEY_LEN];
    char    szValue  [SRM_MAXVALUE_LEN];
	int		bIsDefault;
} TSRMConfig, *PTSRMConfig;

/**
 * @brief   SRM parameters
 *  all support parameters of SRMSetParam and SRMGetParam
 */
enum
{
    SRM_PARAM_CFG                   = 1,
    SRM_PARAM_SRM                   = 2,
};

/**
 * @brief    ID of iSP node
 * This structure is a ID of iSP process component, and
 * will be parameter in function SRMNodeEnum()
 */
typedef struct tagSRMNode
{
    TTSDWORD    ip;
    TTSWORD     type;
    TTSWORD     pid;
} TSRMNode, *PSRMNode;

/* any iSP node */
static const TSRMNode ISP_ANY_NODE =
{
    0,          /* ip */
    node_any,   /* type */
    0           /* pid */
};

/**
 * @brief   Event information of iSP node
 * This structure will be parameter in the callback function SRM_NODE_ROUTINE
 */
typedef struct tagSRMEvent
{
    ISP_EVENT_ID event;                  /* [in] event id */
    TSRMNode     node;                   /* [in] node id */
    int          result;                 /* [in] control result */
    TTSDWORD     wParam;                 /* [in] wParam of the event */
    TTSDWORD     lParam;                 /* [in] lParam of the event */
} TSRMEvent, *PSRMEvent;

/**
 * @brief   Load information of iSP server
 * This structure is parameter used in the SRM_NODE_ROUTINE function 
 */
typedef struct tagSRMLoadInfo
{
    TTSDWORD     cbSize;                 /* sizeof this struct = sizeof(TSRMLoadInfo) */
    TTSDWORD     bench;                  /* benchmark of the host */
    TTSDWORD     cpu;                    /* cpu use percent */
    TTSDWORD     mem;                    /* available memory size (MB) */
    TTSDWORD     network;                /* network use ratio */
    TTSDWORD     actinst;                /* active instance count */
    TTSDWORD     memusage;               /* memory size used (MB) */
} TSRMLoadInfo, *PSRMLoadInfo;

/**
 * @brief	The prototype of iSP event callback function
 * This function will be called when some iSP event occurred, pEvent describe the event information
 */
typedef int (CALLBACK* SRM_ROUTINE)(PSRMEvent pEvent, void* pUserData);

/**
 * @brief   Information of control iSP node
 * Used in SRMNodeCtrl() function
 */
typedef struct tagSRMNodeCtrl
{
    TTSDWORD     dwCtrlParam;            /* [in] control action */
    void*        pCtrlData;              /* [in] data of control */
    TTSDWORD     dwDataSize;             /* [in] size of pCtrlData */

    TTSBOOL      bAsynch;                /* [in] asynchronous or synchronous */
    SRM_ROUTINE  pfnCtrlRoutine;         /* [in] the callback function of control routine */
    void*        pUserData;              /* [in] user-supplied callback data, and will be passed to pfnCtrlRoutine */

    TTSDWORD	dwReserved;              /* [in,out] reserved, must be 0 */
} TSRMNodeCtrl, *PSRMNodeCtrl;

/*
 * @brief   Information of node monitor
 * Used in SRMAddMonitor function
 */
typedef struct tagSRMMonitorOpt
{
    TTSDWORD     dwOption;               /* [in] monitor option */
    void*        pMonitorData;           /* [in] data of monitor */
    TTSDWORD     dwDataSize;             /* [in] size of pMonitorData */

    SRM_ROUTINE  pfnMonRoutine;          /* [in] callback function of monitoring routinue */
    void*        pUserData;              /* [in] User-supplied callback data, and will be passed to pfnMonRoutine */

    TTSDWORD     dwReserved;             /* [in,out] Reserved, must be 0 */
} TSRMMonitorOpt, *PSRMMonitorOpt;

/**
 * @brief   SRM alert level
 *  alert information level
 */
typedef enum tagSRM_ALERT_LVL
{
    alert_none = 0,                      /* NONE alert */
    alert_warning,                       /* warning alert information */
    alert_error,                         /* error alert */
    alert_critical,                      /* critical alert */
} SRM_ALERT_LVL;

/**
 * @brief   SRM alert information
 * iSP alert information, will be wParam in TSRMEvent structure
 */
typedef struct tagSRMAlert
{
    char          name[SRM_ALERT_NAME_LEN];
    int           error;
    SRM_ALERT_LVL level;
    char          desc[SRM_ALERT_DESC_LEN];
} TSRMAlert, *PSRMAlert;

/** 
 * @brief   SRMInitialize
 *
 * Global initialize SRM application programming interface,
 * should be the first SRM function to be called.
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          WSASYSNOTREADY     Indicates that the underlying network subsystem is not ready for network communication.
 *          WSAVERNOTSUPPORTED The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.
 *          WSAEINPROGRESS     A blocking Sockets operation is in progress.
 *          WSAEPROCLIM        Limit on the number of tasks supported by the Windows Sockets implementation has been reached.
 * @param   void* pvReserved    - [in,out] Reserved, must be NULL
 * @see     
 */
int SRMAPI SRMInitialize(void* pvReserved);
typedef int (*Proc_SRMInitialize)(void* pvReserved);

/** 
 * @brief   SRMUninitialize
 * 
 * Global uninitialize SRM application programming interface,
 * should be the last SRM function to be called.
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          WSANOTINITIALISED	Not initialized, or SRMInitialize failed.
 * @see     
 */
int SRMAPI SRMUninitialize(void);
typedef int (*Proc_SRMUninitialize)(void);

/** 
 * @brief   SRMNodeEnum
 *  Enumerate all ISP nodes of specified type
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          ERROR_INVALID_PARAMETER Invalid parameter
 *          ERROR_INSUFFICIENT_BUFFER The pNodes data area passed is too small.
 *          E_POINTER pNodes or pdwCount pointer is invalid
 * @param   TTSDWORD dwNodeType - [in] filter of enumerating, 0 to enumerate all nodes
 * @param   PSRMNode pNodes     - [out] buffer to save iSP node array, allocated by caller
 * @param   TTSDWORD* pdwCount  - [in,out] max count of the capability of pNodes, and return actual node count
 * @see     
 */
int SRMAPI SRMNodeEnum(TTSDWORD dwNodeType, PSRMNode pNodes, TTSDWORD* pdwCount);
typedef int (*Proc_SRMNodeEnum)(TTSDWORD dwNodeType, PSRMNode pNodes, TTSDWORD* pdwCount);

/** 
 * @brief   SRMGetIRS
 * 
 *  Get node id of the IRSM and IRSB
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          E_POINTER		pNodes or pdwCount pointer is invalid
 *          ERROR_NOT_FOUND IRSM and IRSB not found
 * @param   PSRMNode pIRSM	- [out] return id of the IRSM node, must not NULL
 * @param   PSRMNode pIRSB	- [out] return id of the IRSB node, can be NULL
 * @see     
 */
int SRMAPI SRMGetIRS(PSRMNode pIRSM, PSRMNode pIRSB);
typedef int (*Proc_SRMGetIRS)(PSRMNode pIRSM, PSRMNode pIRSB);

/** 
 * @brief   SRMSetIRS
 * 
 *  Set IRSM address
 * 
 * @author  junyan
 * @return  int -  Return 0 in success, otherwise return error code.
 * @param   const char * pszIRSM - [in] IRSM node ip[:port] address
 * @param   const PSRMNode node	 - [in] destination node to set, 0 to local node
 * @see     
 * @exception 
 */
int SRMAPI SRMSetIRS(const char* pszIRSM, const PSRMNode node);
typedef int (*Proc_SRMSetIRS)(const char* pszIRSM, const PSRMNode node);

/** 
 * @brief   SRMGetBestServer
 *
 *  Get the best server (minimal load) running specified service
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          ERROR_NOT_FOUND IRSM or service name not found
 *          E_POINTER       pdwServerIP or pwPort pointer is invalid
 * @param   const char* pszSvcName - [in]  Service name
 * @param   const char* pszFilter  - [in]  Service filter
 * @param   const char* pszServer  - [out] Return best server in "ip-num:port_num" form.(e.g., "192.168.73.202:13606")
 * @see     
 */
int SRMAPI SRMGetBestServer(const char* pszSvcName, const char* pszFilter, char* pszServer);
typedef int (*Proc_SRMGetBestServer)(const char* pszSvcName, const char* pszFilter, char* pszServer);

/** 
 * @brief	SRMNodeQueryInfo
 * 
 * Query specified information of the iSP node
 * 
 * @author  jdyu
 * @return  int SRMAPI	- Return 0 in success, otherwise return error code.
 *          ERROR_NOT_FOUND	The specified node not found
 *          ERROR_INVALID_PARAMETER Invalid parameter
 *          ERROR_INSUFFICIENT_BUFFER The pValue data area passed is too small.
 *          E_POINTER pNodes or pValue or pdwSize pointer is invalid
 * @param   PSRMNode pNode      - [in] Node id to get information
 * @param   TTSDWORD dwInfoType - [in] type of node information to query
 * @param   void* pValue        - [out] return the information specified of the node
 * @param   TTSDWORD* pdwSize   - [in,out] input the pValue buffer capacity, return actual size of the info
 * @see     
 */
int SRMAPI SRMNodeQueryInfo(PSRMNode pNode, TTSDWORD dwInfoType, void* pValue, TTSDWORD* pdwSize);
typedef int (*Proc_SRMNodeQueryInfo)(PSRMNode pNode, TTSDWORD dwInfoType, void* pValue, TTSDWORD* pdwSize);

/** 
 * @brief   SRMNodeGetLoad
 * 
 * Get real-time load information of the specified node.
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          ERROR_NOT_FOUND The specified node not found
 *          E_POINTER pNodes or pValue or pdwSize pointer is invalid
 * @param   PSRMNode pNode      - [in]  Node id
 * @param   PSRMLoadInfo pLoad  - [out] The load information of the node
 * @see     
 */
int SRMAPI SRMNodeGetLoad(PSRMNode pNode, PSRMLoadInfo pLoad);
typedef int (*Proc_SRMNodeGetLoad)(PSRMNode pNode, PSRMLoadInfo pLoad);

/** 
 * @brief   SRMSetParam
 * 
 * Get the configuration of specified node. Node configuration parameter includes
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          ERROR_NOT_FOUND	The specified node not found
 *          ERROR_INVALID_PARAMETER Invalid parameter
 *          E_POINTER pNodes or pValue or pdwSize pointer is invalid
 * @param   TTSDWORD dwNodeID   - [in] node id
 * @param   TTSDWORD dwParam    - [in] type of the parameter
 * @param   void* lpData        - [in] value of the parameter
 * @param   TTSDWORD dwSize     - [in] value size of the parameter
 * @see     
 */
int SRMAPI SRMSetParam(PSRMNode pNode, TTSDWORD dwParam, void* lpData, TTSDWORD dwSize);
typedef int (*Proc_SRMSetParam)(PSRMNode pNode, TTSDWORD dwParam, void* lpData, TTSDWORD dwSize);

/** 
 * @brief   SRMGetParam
 * 
 * Get the configuration of specified node.
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          ERROR_NOT_FOUND	The specified node not found
 *          ERROR_INVALID_PARAMETER Invalid parameter
 *          ERROR_INSUFFICIENT_BUFFER The pValue data area passed is too small.
 *          E_POINTER pNodes or pValue or pdwSize pointer is invalid
 * @param   PSRMNode pNode      - [in]  node id
 * @param   TTSDWORD dwParam    - [in]  type of the parameter
 * @param   void* lpData        - [out] return the value of the parameter
 * @param   TTSDWORD* pdwSize   - [in,out]  input the pValue buffer capacity, return actual size of the parameter info
 * @see     
 */
int SRMAPI SRMGetParam(PSRMNode pNode, TTSDWORD dwParam, void* lpData, TTSDWORD* pdwSize);
typedef int (*Proc_SRMGetParam)(PSRMNode pNode, TTSDWORD dwParam, void* lpData, TTSDWORD* pdwSize);

/** 
 * @brief   SRMNodeControl
 * 
 * Controlling the specified node to stop service, start service and so on.
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          ERROR_NOT_FOUND	The specified node not found
 *          ERROR_INVALID_PARAMETER Invalid parameter
 *          E_POINTER pNodes or pValue or pdwSize pointer is invalid
 * @param   PTSRMNodeCtrl pCtrlInfo - [in] The information of node controlling
 * @param   TTSDWORD dwUserData     - [in] User-supplied callback data, and will be passed to pfnNodeCtrlRoutine
 * @see     
 */
int SRMAPI SRMNodeControl(PSRMNode pNode, PSRMNodeCtrl pCtrlInfo);
typedef int (*Proc_SRMNodeControl)(PSRMNode pNode, PSRMNodeCtrl pCtrlInfo);

/** 
 * @brief   SRMAddMonitor
 * 
 * Add a monitoring hook to the node, the callback function will be called
 * when the status changed on specified node
 * 
 * @author  jdyu
 * @return  int - Return 0 in success, otherwise return error code.
 *          ERROR_NOT_FOUND	The specified node not found
 *          ERROR_INVALID_PARAMETER Invalid parameter
 *          E_POINTER pNodes or pValue or pdwSize pointer is invalid
 * @param   PSRMNode pNode          - [in] Node id to monitoring
 * @param   PSRMMonitorOpt pMonOpt	- [in] monitoring option
 * @param   SRM_HANDLE* phHandle    - [out] return handle of the added monitor
 * @see     
 */
int SRMAPI SRMAddMonitor(PSRMNode pNode, PSRMMonitorOpt pMonOpt, SRM_HANDLE* phHandle);
typedef int (*Proc_SRMAddMonitor)(PSRMNode pNode, PSRMMonitorOpt pMonOpt, SRM_HANDLE* phHandle);

/** 
 * @brief   SRMRemoveMonitor
 * 
 * Remove the monitor added by SRMAddMonitor,
 * pass INVALID_HANDLE_VALUE to remove all monitor
 * 
 * @author  jdyu
 * @return  int SRMAPI    - Return 0 in success, otherwise return error code.
 *          ERROR_NOT_FOUND	The specified monitor not found
 *          E_HANDLE hHandle is invalid
 * @param   SRM_HANDLE hHandle      - [in] Monitor handle to remove, INVALID_HANDLE_VALULE to remove all
 * @see     
 */
int SRMAPI SRMRemoveMonitor(SRM_HANDLE hHandle);
typedef int (*Proc_SRMRemoveMonitor)(SRM_HANDLE hHandle);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /* _IFLY_SRM_H_ */
