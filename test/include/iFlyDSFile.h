/** 
 * @file	iFlyDSFile.h
 * @brief	存储系统抽象文件定义
 * 
 * detail...
 * 
 * @author	jingzhang
 * @version	1.0
 * @date	2012/2/23
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2012/2/23	<td>jingzhang	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __IFLYDSFILE_H__
#define __IFLYDSFILE_H__

#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#include "iFlyCFDef.h"
#include "iFlyCFError.h"

#ifdef __cplusplus
extern  "C" {
#endif

		/* 使用本地文件系统 */
	#define USE_LOCALFILE_SYSTEM "localfilesystem"

	/* 文件的打开方式 */
	#define DS_RDONLY O_RDONLY		// 0x0000
	#define DS_WRONLY O_WRONLY		// 0x0001
	#define DS_APPEND O_APPEND		// 0x0008
		
	/* 文件流的类型 */
	typedef enum stream_type
	{
		undefined = 0,
		input,
		output
	}stream_type;

	/* 文件指针 */
	typedef struct ds_file_t
	{
		ds_handle file;
		stream_type stream;
		int cache_open;	/* 该文件对应的cache是否已经打开 */
		int dss_open; /* 该文件对应的DSS文件是否已经打开 */
	} * DSFILE;
	 
	/* 文件类型 */
	typedef enum ds_file_kind {
        kind_file = 'F',
        kind_dir = 'D',
    }ds_file_kind;

	/* 文件信息 */
	typedef struct ds_file_info_t
	{
		ds_file_kind kind;		/* 类型文件或目录	*/
        char   *name;			/* 文件名			*/
		char   *owner;			/* 文件主			*/
		char   *group;			/* 依附于文件的组	*/
		short  permissions;		/* 文件的许可属性	*/
		short  replication;		/* 复制数量			*/
        time_t last_modified;	/* 最后修改时间sec	*/
		time_t last_access;		/* 最后访问时间sec	*/
        uint64 size;			/* 文件长度 byte		*/
        uint64 blocksize;		/* 块大小			*/
	} ds_file_info;

	typedef enum ds_event_t 
	{
		event_hdfs_connect_down,	/* hdfs连接中断 */
		event_hdfs_resume,			/* hdfs连接回复 */
		event_dsprx_connect_down,   /* dsproxy连接中断 */
		event_file_connect_down		/* 文件传输连接中断 */
	} ds_event;

	/* 回调函数返回数据 */
	typedef struct edata_t
	{
		ds_handle old_hd; /* 原先的句柄号 */
		ds_handle new_hd; /* 新的句柄号，如果没有设置则为-1 */
		DSFILE    old_fd; /* 原先的文件描述符 */
	} edata;

	/* 网络程序会遇到网络连接中断或者服务端崩溃等异常情况，客户端接口会将这些情况通过回调函数
	   通知给调用程序，调用程序必须对这些异常进行处理，目前系统包括的异常和处理方法如下:
	   (1) event_hdfs_connect_down 
	       说明：HDFS NameNode不可用
		   客户程序处理方法：客户程序收到这一通知后，应该保存data.old_hd，它指明了用户正在
		   使用的某个session的句柄，出现这一事件说明该session不再可用，如果用户继续使用该句柄
		   所有调用接口会返回错误，这时候应用程序应该暂停使用iDSS功能，直到出现event_hdfs_resume事件
	   (2) event_hdfs_resume
	       说明：HDFS NameNode可以使用
		   客户程序处理方法：客户程序在收到这一通知后说明HDFS已经可以使用，客户程序应该首先调用
		   ds_disconnect()函数释放那些在(1)中记录的句柄，因为NameNode重新启动后原有句柄已经无效，
		   然后，重新调用ds_connect连接iDSS。
	   (3) event_dsprx_connect_down
	       说明：连接的dsproxy不可用
		   客户程序处理方法：客户程序收到这一通知后，应该保存data.old_hd，它指明了用户正在
		   使用的某个session的句柄，出现这一事件说明该session不再可用，如果用户继续使用该句柄
		   所有调用接口会返回错误，这时候合适的做法是先调用ds_disconnect，释放掉这个死掉的连接，然后
		   调用ds_get_dsproxy()函数获得一个较好的dsproxy，重新调用重新调用ds_connect连接iDSS。
	   (4) event_file_connect_down
		   说明：文件传输通道崩溃
		   客户程序处理方法：户端程序收到这一通知后，应该保存data.old_fd,它指明了出错的文件描述符，
		   data.old_hd指明了连接句柄。出现这一事件说明该文件传输通道失效，调用ds_fclose()关闭这一文件，
		   同时用户应该调用ds_fopen重新打开要传输的文件。
		*/
	typedef int (*DS_EVENT_HOOK) (ds_event e, edata* data, void* cbdata);


#ifdef __cplusplus

}
#endif

#endif /* __IFLYDSFILE_H__ */
