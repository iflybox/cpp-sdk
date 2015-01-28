/** 
 * @file	iFlyCFError.h
 * @brief	讯飞Cloud Foundation出错代码
 * 
 * 本文件定义了讯飞Cloud Foundation的错误代码及其文字说明
 * 
 * @author	jingzhang
 * @version	1.0
 * @date	2010-12-9
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2010-12-9	<td>jingzhang	<td>Create this file</tr>
 *  <tr> <td>2.0		<td>2010-4-8	<td>jingzhang	<td>Second Edition</tr>
 * </table>
 * 
 */
#ifndef __IFLYCFERROR_H__
#define __IFLYCFERROR_H__

#include "iFlyCFDef.h"

/* 为了避免和系统错误代码冲突定义错误代码从如下值开始 */

#ifdef __cplusplus
extern  "C" {
#endif
	
	#define CF_ERROR_GENERAL_START     30100
	#define CF_ERROR_NET_START		   30200
	#define DS_ERROR_GENERAL_START     31100
	#define DC_ERROR_GENERAL_START     32100 

	typedef enum
	{
		CF_SUCCESS					= 0,
		CF_ERROR_FAIL				= -1,
		CF_ERROR_EXCEPTION			= -2,

		/* General errors 30100 */
		CF_ERROR_GENERAL			= 30100,
		CF_ERROR_OUT_OF_MEMORY		= 30101,
		CF_ERROR_FILE_NOT_FOUND		= 30102,
		CF_ERROR_NOT_SUPPORT		= 30103,
		CF_ERROR_NOT_IMPLEMENT		= 30104,
		CF_ERROR_ACCESS				= 30105,
		CF_ERROR_INVALID_PARA		= 30106,
		CF_ERROR_INVALID_PARA_VALUE = 30107,
		CF_ERROR_INVALID_HANDLE		= 30108,
		CF_ERROR_INVALID_DATA		= 30109,
		CF_ERROR_NO_LICENSE			= 30110,
		CF_ERROR_NOT_INIT			= 30111,
		CF_ERROR_NULL_HANDLE		= 30112,
		CF_ERROR_OVERFLOW			= 30113,
		CF_ERROR_TIME_OUT			= 30114,
		CF_ERROR_OPEN_FILE			= 30115,
		CF_ERROR_NOT_FOUND			= 30116,
		CF_ERROR_NO_ENOUGH_BUFFER	= 30117,
		CF_ERROR_NO_DATA			= 30118,
		CF_ERROR_NO_MORE_DATA		= 30119,
		CF_ERROR_SKIPPED			= 30120,
		CF_ERROR_ALREADY_EXIST		= 30121,
		CF_ERROR_LOAD_MODULE		= 30122,
		CF_ERROR_BUSY				= 30123,
		CF_ERROR_INVALID_CONFIG		= 30124,
		CF_ERROR_VERSION_CHECK		= 30125,
		CF_ERROR_CANCELED			= 30126,
		CF_ERROR_INVALID_MEDIA_TYPE	= 30127,
		CF_ERROR_CONFIG_INITIALIZE	= 30128,
		CF_ERROR_CREATE_HANDLE		= 30129,
		CF_ERROR_CODING_LIB_NOT_LOAD= 30130,
		CF_ERROR_OPERATION_NOT_SUP	= 30131,
		CF_ERROR_INPROC				= 30132,
		CF_ERROR_TOOMANYREQ			= 30133,
		CF_ERROR_CACHE_FULL         = 30134,
		CF_ERROR_TIMEOUT			= 30135,
		CF_ERROR_GENERAL_LAST,

		/* network error 30200*/
		CF_ERROR_NET_GENERAL		= 30200,
		CF_ERROR_NET_OPENSOCK       = 30201,/* Open socket */
		CF_ERROR_NET_CONNECTSOCK    = 30202,/* Connect socket */
		CF_ERROR_NET_ACCEPTSOCK     = 30203,/* Accept socket */
		CF_ERROR_NET_SENDSOCK       = 30204,/* Send socket data */
		CF_ERROR_NET_RECVSOCK       = 30205,/* Recv socket data */
		CF_ERROR_NET_INVALIDSOCK    = 30206,/* Invalid socket handle */
		CF_ERROR_NET_BADADDRESS     = 30207,/* Bad network address */
		CF_ERROR_NET_BINDSEQUENCE   = 30208,/* Bind after listen/connect */
		CF_ERROR_NET_NOTOPENSOCK    = 30209,/* Socket is not opened */
		CF_ERROR_NET_NOTBIND        = 30210,/* Socket is not bind to an address */
		CF_ERROR_NET_NOTLISTEN      = 30211,/* Socket is not listening */
		CF_ERROR_NET_CONNECTCLOSE   = 30212,/* The other side of connection is closed */
		CF_ERROR_NET_NOTDGRAMSOCK   = 30213,/* The socket is not datagram type */
		CF_ERROR_NET_DNS     		= 30214,/* domain name is invalid or dns server does not function well */
		CF_ERROR_NET_LAST,

		/* Distributed Storage System Error 31100*/
		DS_ERROR_GENERAL			= 31100,
		DS_ERROR_OUTOFRANGE			= 31101,
		DS_ERROR_QUOTA				= 31102,
		VD_ERROR_VIRNOEXIST			= 31103,
		VD_ERROR_REALNOEXIST		= 31104,
		VD_ERROR_VIREXISTED			= 31105,
		DS_ERROR_HDFS_DOWN			= 31106,
		DS_ERROR_LAST,

		/* Distributed Coordinator Error 32100*/
		DC_ERROR_GENERAL					= 32100,
		DC_ERROR_SYSTEM						= 32101,
		DC_ERROR_RUNTIMEINCONSISTENCY		= 32102,	/* A runtime inconsistency was found */
		DC_ERROR_DATAINCONSISTENCY			= 32103,	/* A data inconsistency was found */
		DC_ERROR_CONNECTIONLOSS				= 32104,	/* Connection to the server has been lost */
		DC_ERROR_MARSHALLINGERROR			= 32105,	/* Error while marshalling or unmarshalling data */
		DC_ERROR_INVALIDSTATE				= 32106,	/* Invalid zhandle state */
		DC_ERROR_NOAUTH						= 32107,	/* Not authenticated */
		DC_ERROR_BADVERSION					= 32108,	/* Version conflict */
		DC_ERROR_NOCHILDRENFOREPHEMERALS	= 32109,	/* Ephemeral nodes may not have children */
		DC_ERROR_NODEEXISTS					= 32110,	/* The node already exists */
		DC_ERROR_NOTEMPTY					= 32111,	/* The node has children */
		DC_ERROR_SESSIONEXPIRED				= 32112,	/* The session has been expired by the server */
		DC_ERROR_INVALIDCALLBACK			= 32113,	/* Invalid callback specified */
		DC_ERROR_INVALIDACL					= 32114,	/* Invalid ACL specified */
		DC_ERROR_AUTHFAILED					= 32115,	/* Client authentication failed */
		DC_ERROR_CLOSING					= 32116,	/* ZooKeeper is closing */
		DC_ERROR_NOTHING					= 32117,	/* (not error) no server responses to process */
		DC_ERROR_SESSIONMOVED				= 32118,	/* session moved to another server, so operation is ignored */ 
		DC_ERROR_API						= 32119,	/* API error */
		DC_ERROR_LAST

	} cf_error_t;

	static const char* cf_error_g[] =
	{
		"general error",
		"out of memory",
		"file not found",
		"not support",
		"not implement",
		"access deny",
		"invalid parameter",
		"invalid parameter value",
		"invalid handle",
		"invalid data",
		"no license",
		"not initialized",
		"null handle",
		"overflow",
		"time out",
		"cannot open file",
		"not found",
		"no enough buffer",
		"no data",
		"no more data",
		"skipped",
		"already exist",
		"load modular",
		"busy now",
		"invalid config",
		"version check",
		"user cancel",
		"invalid media type",
		"config initialize",
		"create handle",
		"coding library not loaded",
		"operation not supported",
		"in process",
		"too many request",
		"cache full",
		"time out",
		"last general error"
	};

	static const char* cf_error_net[] =
	{
		"general network error",
		"open socket",
		"connect socket",
		"accept socket",
		"send socket data",
		"receive socket data",
		"invalid socket handle",
		"bad network address",
		"socket address bind",
		"socket not open",
		"socket not bind",
		"socket not listening",
		"peer closed",
		"socket not datagram type",
		"invalid domain or DNS malfunctioning",
		"last network error"
	};

	static const char* ds_error[] =
	{
		"distributed storage system general error",
		"offset out of range",
		"disk quota limited",
		"virtual path not existed",
		"real path not existed",
		"virtual path already existed",
		"HDFS system down or cannot connect",
		"distributed storage system last error"
	};

	static const char* dc_error[] =
	{
		"dc general error",
		"system error",
		"runtime inconsistency was found",
		"data inconsistency was found",
		"connection to the server has been lost",
		"error while marshalling or unmarshalling data",
		"invalid zhandle state",
		"not authenticated",
		"version conflict",
		"ephemeral nodes may not have children",
		"the node already exists",
		"the node has children",
		"the session has been expired by the server",
		"invalid callback specified",
		"invalid ACL specified",
		"client authentication failed",
		"zooKeeper is closing",
		"(not error) no server responses to process",
		"session moved to another server, so operation is ignored",
		"API error",
		"dc last error"
	};

	static const char*  cf_error_str(cf_error_t errnum)
	{
		static const char* success_str = "success";
		static const char* failed_str = "failed";
		static const char* exception_str = "exception";

		if (errnum == CF_SUCCESS)
			return success_str;
		if (errnum == CF_ERROR_EXCEPTION)
			return exception_str;
		
		if ((errnum >= CF_ERROR_GENERAL_START) && (errnum < CF_ERROR_GENERAL_LAST))
			return cf_error_g[errnum - CF_ERROR_GENERAL_START];
		if ((errnum >= CF_ERROR_NET_START) && (errnum < CF_ERROR_NET_LAST))
			return cf_error_net[errnum - CF_ERROR_NET_START];
		if ((errnum >= DS_ERROR_GENERAL_START) && (errnum < DS_ERROR_LAST))
			return ds_error[errnum - DS_ERROR_GENERAL_START];
		if ((errnum >= DC_ERROR_GENERAL_START) && (errnum < DC_ERROR_LAST))
			return dc_error[errnum - DC_ERROR_GENERAL_START];

		return failed_str;
	}

	static const char * cf_error_int_str(int errnum)
	{
		static const char* failed_str = "failed";

		if ( (errnum == CF_SUCCESS) || (errnum == CF_ERROR_FAIL) || (errnum == CF_ERROR_EXCEPTION)
			|| (errnum >= CF_ERROR_GENERAL_START) && (errnum < CF_ERROR_GENERAL_LAST)
			|| (errnum >= CF_ERROR_NET_START) && (errnum < CF_ERROR_NET_LAST)
			|| (errnum >= DS_ERROR_GENERAL_START) && (errnum < DS_ERROR_LAST)
			|| (errnum >= DC_ERROR_GENERAL_START) && (errnum < DC_ERROR_LAST))
			return cf_error_str((cf_error_t)errnum);
		else
			return failed_str;
	}

#ifdef __cplusplus
}
#endif

#endif /* __IFLYDSSERROR_H__ */
