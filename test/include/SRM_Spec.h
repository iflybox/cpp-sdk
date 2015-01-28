/** 
 * @file	SRM_Spec.h
 * @brief	SRM接口的内部定义文件
 * 
 * 这里包含了SRM接口所有未公开的定义，这些定义在调用SRM接口时提供了一些方便的使用方法，
 * 同时也增加了SRM接口的可维护性
 * 注意：这个文件可能在C/C++下公用，必须有定义为C语言风格
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2004-7-20
 * 
 * @see		iFly_SRM.h
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2004-8-9	<td>jdyu	<td>Create this file</tr>
 *  <tr> <td>1.1		<td>2005-11-15	<td>jdyu	<td>扩充</tr>
 * </table>
 * 
 */

#ifndef _SRM_SPEC_H_
#define _SRM_SPEC_H_

#include "iFly_SRM.h"

#define ISP_MAX_HOSTINFO	128
#define ISP_MAX_TIMESTR		 32

/**
 * @brief	INFO_TYPE
 *  在调用SRMNodeQueryInfo函数时使用
 */
enum
{
	SRM_HOSTINFO_ALL                = 30,
	SRM_NODEINFO_ALL                = 31,
};

/**
 * @brief	SRMSetParam/SRMGetParam函数使用的参数类型
 * 内部参数定义
 */
enum
{
	SRM_PARAM_CFGFILE			    = 100,
};

/**
* @brief	SRMSetParam/SRMGetParam函数使用的参数类型
* 内部参数定义
*/
typedef enum SRM_NODE_STATUS
{
	SRM_NODESTATUS_UNKNOWN			= 0,
	SRM_NODESTATUS_STARTED			= 1,
	SRM_NODESTATUS_STOPPED			= 2,
	SRM_NODESTATUS_PAUSED           = 3,
} SRM_NODE_STATUS;

/**
 * @brief	SRM_Init_Info
 *  SRM初始化信息的结构，内部使用，作为pvReserved传入
 */
typedef struct SRM_Init_Info
{
	int				cbSize;
	ISP_NODE_TYPE	type;
	long	        dwLocalIP;
} SRM_Init_Info, * SRM_Init_Info_p;

/**
 * 平台主机信息格式的定义。
 */
typedef struct ISP_Host_Info
{
	int		cbsize;							/* < = sizeof ISP_Host_Info */
	int		type;							/* < 主机类型：ISP_HOST_TYPE | ISP_HOST_TYPE */
	char	ip[TTS_IP_MAXLEN];				/* < 主机IP地址 */
	char	name[ISP_MAX_HOSTINFO];			/* < 主机名称 */
	char	os[ISP_MAX_HOSTINFO];			/* < 操作系统描述信息 */
	char	cpu[ISP_MAX_HOSTINFO];			/* < CPU描述信息 */
	size_t	cpu_num;						/* < CPU个数 */
	size_t	mem_total;						/* < 物理内存总数 */
	size_t	netadpt_num;					/* < 网卡数量 */
} ISP_Host_Info, * ISP_Host_Info_p;

/* 2.0版本中使用的主机信息 */
/* 为了和老版本兼容加入的，以后切勿修改 */
typedef struct ISP_Host_Info_20
{
	int		type;							/* < 主机类型：ISP_HOST_TYPE | ISP_HOST_TYPE */
	char	ip[TTS_IP_MAXLEN];				/* < 主机IP地址 */
	char	name[ISP_MAX_HOSTINFO];			/* < 主机名称 */
	char	os[ISP_MAX_HOSTINFO];			/* < 操作系统描述信息 */
	char	cpu[ISP_MAX_HOSTINFO];			/* < CPU描述信息 */
	size_t	cpu_num;						/* < CPU个数 */
	size_t	mem_total;						/* < 物理内存总数 */
} ISP_Host_Info_20, * ISP_Host_Info_20_p;

/**
 * 平台节点信息格式的定义。
 */
typedef struct ISP_Node_Info
{
    int				cbsize;                 /* < = sizeof ISP_Node_Info */
    unsigned int	ip;                     /* < node的ip地址 */
    unsigned short	type;                   /* < 节点类型 */
    unsigned short	pid;                    /* < 节点的pid */
    unsigned short	status;                 /* < 节点的状态 */
    unsigned short	max_lic;                /* < 最大授权数 */
    unsigned short	svc_port;               /* < 平台服务端口 */
    unsigned short	rr_port;                /* < 平台管理RR端口 */
    unsigned short	ev_port;                /* < 平台管理Event端口 */
    unsigned short  lb_sta;                 /* < SES节点的负载均衡状态 */
    char			starttime[32];          /* < 开始时间 */
    char			exe[MAX_PATH];          /* < 可执行文件路径 */
    char			version[32];            /* < 主要版本(文件版本 产品版本) */
    char			file_time[32];          /* < 文件时间(创建时间 修改时间) */
    char			val_ips[128];           /* < 可用ip列表 */
	int				firewall;				/* < 防火墙是否启用 */
} ISP_Node_Info, * ISP_Node_Info_p;

/* 2.0中使用的结构体 */
/* 为了和老版本兼容加入的，以后切勿修改 */
typedef struct ISP_Node_Info_20
{
	unsigned int	ip;						/* < node的ip地址 */
	unsigned short	type;					/* < 节点类型 */
	unsigned short	pid;					/* < 节点的pid */
	unsigned short	status;					/* < 节点的状态 */
	unsigned short	max_lic;				/* < 最大授权数 */
	unsigned short	svc_port;				/* < 平台服务端口 */
	unsigned short	rr_port;				/* < 平台管理RR端口 */
	unsigned short	ev_port;				/* < 平台管理Event端口 */
	char			starttime[32];			/* < 开始时间 */
	char			exe[MAX_PATH];			/* < 可执行文件路径 */
} ISP_Node_Info_20, * ISP_Node_Info_20_p;

/* 可以返回filter的获得最佳服务器函数 */
int SRMAPI SRMGetBestServerEx(const char* pszSvcName, char* pszFilter, char* pszServer);

#endif /* _SRM_SPEC_H_ */
