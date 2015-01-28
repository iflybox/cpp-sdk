/** 
 * @file	SRM_Spec.h
 * @brief	SRM�ӿڵ��ڲ������ļ�
 * 
 * ���������SRM�ӿ�����δ�����Ķ��壬��Щ�����ڵ���SRM�ӿ�ʱ�ṩ��һЩ�����ʹ�÷�����
 * ͬʱҲ������SRM�ӿڵĿ�ά����
 * ע�⣺����ļ�������C/C++�¹��ã������ж���ΪC���Է��
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
 *  <tr> <td>1.1		<td>2005-11-15	<td>jdyu	<td>����</tr>
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
 *  �ڵ���SRMNodeQueryInfo����ʱʹ��
 */
enum
{
	SRM_HOSTINFO_ALL                = 30,
	SRM_NODEINFO_ALL                = 31,
};

/**
 * @brief	SRMSetParam/SRMGetParam����ʹ�õĲ�������
 * �ڲ���������
 */
enum
{
	SRM_PARAM_CFGFILE			    = 100,
};

/**
* @brief	SRMSetParam/SRMGetParam����ʹ�õĲ�������
* �ڲ���������
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
 *  SRM��ʼ����Ϣ�Ľṹ���ڲ�ʹ�ã���ΪpvReserved����
 */
typedef struct SRM_Init_Info
{
	int				cbSize;
	ISP_NODE_TYPE	type;
	long	        dwLocalIP;
} SRM_Init_Info, * SRM_Init_Info_p;

/**
 * ƽ̨������Ϣ��ʽ�Ķ��塣
 */
typedef struct ISP_Host_Info
{
	int		cbsize;							/* < = sizeof ISP_Host_Info */
	int		type;							/* < �������ͣ�ISP_HOST_TYPE | ISP_HOST_TYPE */
	char	ip[TTS_IP_MAXLEN];				/* < ����IP��ַ */
	char	name[ISP_MAX_HOSTINFO];			/* < �������� */
	char	os[ISP_MAX_HOSTINFO];			/* < ����ϵͳ������Ϣ */
	char	cpu[ISP_MAX_HOSTINFO];			/* < CPU������Ϣ */
	size_t	cpu_num;						/* < CPU���� */
	size_t	mem_total;						/* < �����ڴ����� */
	size_t	netadpt_num;					/* < �������� */
} ISP_Host_Info, * ISP_Host_Info_p;

/* 2.0�汾��ʹ�õ�������Ϣ */
/* Ϊ�˺��ϰ汾���ݼ���ģ��Ժ������޸� */
typedef struct ISP_Host_Info_20
{
	int		type;							/* < �������ͣ�ISP_HOST_TYPE | ISP_HOST_TYPE */
	char	ip[TTS_IP_MAXLEN];				/* < ����IP��ַ */
	char	name[ISP_MAX_HOSTINFO];			/* < �������� */
	char	os[ISP_MAX_HOSTINFO];			/* < ����ϵͳ������Ϣ */
	char	cpu[ISP_MAX_HOSTINFO];			/* < CPU������Ϣ */
	size_t	cpu_num;						/* < CPU���� */
	size_t	mem_total;						/* < �����ڴ����� */
} ISP_Host_Info_20, * ISP_Host_Info_20_p;

/**
 * ƽ̨�ڵ���Ϣ��ʽ�Ķ��塣
 */
typedef struct ISP_Node_Info
{
    int				cbsize;                 /* < = sizeof ISP_Node_Info */
    unsigned int	ip;                     /* < node��ip��ַ */
    unsigned short	type;                   /* < �ڵ����� */
    unsigned short	pid;                    /* < �ڵ��pid */
    unsigned short	status;                 /* < �ڵ��״̬ */
    unsigned short	max_lic;                /* < �����Ȩ�� */
    unsigned short	svc_port;               /* < ƽ̨����˿� */
    unsigned short	rr_port;                /* < ƽ̨����RR�˿� */
    unsigned short	ev_port;                /* < ƽ̨����Event�˿� */
    unsigned short  lb_sta;                 /* < SES�ڵ�ĸ��ؾ���״̬ */
    char			starttime[32];          /* < ��ʼʱ�� */
    char			exe[MAX_PATH];          /* < ��ִ���ļ�·�� */
    char			version[32];            /* < ��Ҫ�汾(�ļ��汾 ��Ʒ�汾) */
    char			file_time[32];          /* < �ļ�ʱ��(����ʱ�� �޸�ʱ��) */
    char			val_ips[128];           /* < ����ip�б� */
	int				firewall;				/* < ����ǽ�Ƿ����� */
} ISP_Node_Info, * ISP_Node_Info_p;

/* 2.0��ʹ�õĽṹ�� */
/* Ϊ�˺��ϰ汾���ݼ���ģ��Ժ������޸� */
typedef struct ISP_Node_Info_20
{
	unsigned int	ip;						/* < node��ip��ַ */
	unsigned short	type;					/* < �ڵ����� */
	unsigned short	pid;					/* < �ڵ��pid */
	unsigned short	status;					/* < �ڵ��״̬ */
	unsigned short	max_lic;				/* < �����Ȩ�� */
	unsigned short	svc_port;				/* < ƽ̨����˿� */
	unsigned short	rr_port;				/* < ƽ̨����RR�˿� */
	unsigned short	ev_port;				/* < ƽ̨����Event�˿� */
	char			starttime[32];			/* < ��ʼʱ�� */
	char			exe[MAX_PATH];			/* < ��ִ���ļ�·�� */
} ISP_Node_Info_20, * ISP_Node_Info_20_p;

/* ���Է���filter�Ļ����ѷ��������� */
int SRMAPI SRMGetBestServerEx(const char* pszSvcName, char* pszFilter, char* pszServer);

#endif /* _SRM_SPEC_H_ */
