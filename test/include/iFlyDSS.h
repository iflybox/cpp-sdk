/** 
 * @file	iFlyDSS.h
 * @brief	讯飞分布式存储系统API
 * 
 * 本文件声明了讯飞分布式存储系统的API函数原型
 * 给出了各API函数语义参数和返回值类型和取值范围
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
 * </table>
 * 
 */

#ifndef __IFLY_DSS_H__
#define __IFLY_DSS_H__

#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#include "iFlyCFDef.h"
#include "iFlyCFError.h"

/* 使用本地文件系统 */
#define USE_LOCALFILE_SYSTEM "localfilesystem"

/* 文件的打开方式 */
#define DS_RDONLY O_RDONLY		// 0x0000
#define DS_WRONLY O_WRONLY		// 0x0001
#define DS_APPEND O_APPEND		// 0x0008
#define DS_RDNOCACHE				3
#define DS_WRNOCACHE				4
#define DS_APNOCACHE				5

#ifdef __cplusplus
extern  "C" {
#endif
	
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
		event_dsprx_connect_down,  /* dsproxy连接中断 */
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

	/** 
	 * @brief 	ds_initialize_ex
	 *			初始化系统的第一个函数，仅需要调用一次
	 * @return	DSAPI int	- 成功返回0，否则返回失败代码
	 * @param	int enable_dcache	- [in] - 是否使用分布式cache
	 * @param	DS_EVENT_HOOK fn	- [in] - 用来接收系统事件的Callback函数
	 * @param	void* reserv	- [in]
	 *          保留以后使用
	 * @see		
	 */
	DSAPI int ds_initialize_ex(int enable_dcache, DS_EVENT_HOOK fn, void* cbdata, void* reserv);
	typedef int (*Proc_ds_initialize_ex)(int enable_dcache, DS_EVENT_HOOK fn, void* cbdata, void* reserv);

	/** 
	 * @brief 	ds_initialize
	 *			初始化系统的第一个函数，仅需要调用一次
	 * @return	DSAPI int	- 成功返回0，否则返回失败代码
	 * @param	int enable_dcache	- [in] - 是否使用分布式cache
	 * @param	void* reserv	- [in]
	 *          保留以后使用
	 * @see		
	 */
	DSAPI int ds_initialize(int enable_dcache, void* reserv);
	typedef int (*Proc_ds_initialize)(int enable_dcache, void* reserv);

	/** 
	 * @brief 	ds_uninitialize
	 *			逆初始化的最后一个函数
	 * @return	无返回值，调用后不在可用.
	 * @see		
	 */
	DSAPI void ds_uninitialize(void);
	typedef void (*Proc_ds_uninitialize)(void);
	
	/** 
	 * @brief 	ds_connect
	 *          连接分布式存储系统
	 * @return	ds_handle 如果成功返回存储系统句柄，如果失败返回ds_invalid_handle，errno指明失败原因
	 * @param	const char* dsURL	- [in] 分布式存储URL
	 *			dsURLS应符合如下形式 scheme://host:port/dir
	 *				scheme -协议，常用协议是hdfs（Hadoop分布式文件系统）
	 *						还可以是用户定义的虚拟目录如grammar
	 *				dir    -打开目录，如果该目录存在当前目录设置为该目录
	 *                      否则设置为根目录，对于虚拟目录根目录是是指映射
	 *                      到该虚拟目录的DSS目录
	 * @param	const char* uid	- [in]
	 *          指明一个用以表明用户身份的id
	 * @param	const char* proxyAddr	- [in] 
	 *			使用dsproxy的地址，Linux下为NULL
	 * @param	void* reserv	- [in]
	 *          保留以后使用
	 * @Comments: 不使用dsproxy时一个连接由dsURL中的host，port和uid共同标识。也就是说如果
	 *            host，port和uid都相同，则该函数应该返回相同的句柄
	 *            使用dsproxy时连接除了上述标识外还应该加上dsproxy的地址，也就是说客户端通过
	 *            不同的dsproxy以同一用户名连接同一个dsURL，返回不同的句柄
	 */
	DSAPI ds_handle ds_connect(const char* dsURL, const char* uid, const char* proxyAddr, void* reserv);
	typedef ds_handle (*Proc_ds_connect)(const char* dsURL, const char* uid, const char* proxyAddr, void* reserv);

	/** 
	 * @brief 	ds_disconnect
	 *          断开分布式文件系统
	 * @return	int - Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *          ds_connect函数返回的句柄 
	 */
	DSAPI int ds_disconnect(ds_handle dsh);
	typedef int (*Proc_ds_disconnect)(ds_handle dsh);

	/** 
	 * @brief 	ds_fopen
	 *			打开文件
	 * @return	DSFILE 成功返回文件描述付，失败返回NULL，errno指明失败原因
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			打开的文件路径，支持虚拟路径上的文件
	 * @param	int flags	- [in] 
	 *			打开方式	DS_RDONLY只读方式打开
	 *					DS_WRONLY写方式打开
	 *					DS_APPEND追加方式打开
	 * @param	int buffersize	- [in] range [0, 0x7FFFFFFF]
	 *			指定打开文件时缓冲区的大小，0则使用默认值：4096
	 * @param	short replication	- [in] range [0, 0x7FFF]
	 *			指定文件副本的数目，0则使用默认值：3
	 * @param	int blocksize	- [in] range [0, 0x7FFF]
	 *			指定文件块的大小，0则使用默认值：64*1024*1024
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			指定打开文件是否使用cache，如果是（传入非0）则打开cache中的文件，否则直接打开idss中的文件
	 * @Comments: buffersize是打开的流的缓冲区大小，该值被最终传给java的BufferedInputStream和BufferedOutputStream
	 *            两个类的构造函数，在Java SE的实现中如果不指定这个值，则默认为8192字节
	 *            hadoop.configuration （io.file.buffer.size）和代码中定义的该值为4096.
	 */
	DSAPI DSFILE ds_fopen(ds_handle dsh, const char* path, int flags,
		int buffersize, short replication, int blocksize, int cache_flag);
	typedef DSFILE (*Proc_ds_fopen)(ds_handle dsh, const char* path, int flags,
		int buffersize, short replication, int blocksize, int cache_flag);

	/** 
	 * @brief 	ds_fclose
	 *			关闭文件
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			准备关闭的文件指针	
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			是否处理cache
	 */
    DSAPI int ds_fclose(ds_handle dsh, DSFILE file, int cache_flag);
	typedef int (*Proc_ds_fclose)(ds_handle dsh, DSFILE file, int cache_flag);

	/** 
	 * @brief 	ds_sync_fclose
	 *			关闭文件，当前仅提供给数据同步使用
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			准备关闭的文件指针	
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			是否处理cache
	 */
    DSAPI int ds_sync_fclose(ds_handle dsh, DSFILE file, int cache_flag);
	typedef int (*Proc_ds_sync_fclose)(ds_handle dsh, DSFILE file, int cache_flag);

	/** 
	 * @brief 	ds_read
	 *			读取文件
	 * @return	int 返回真实读取的字节数，返回-1为失败，errno指明失败原因
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			文件指针
	 * @param	void* buffer	- [in] 
	 *			读取缓冲区
	 * @param	int length	- [in] range:(0, 0x7FFFFFFF]
	 *			缓冲区长度
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			是否处理cache
	 */
    DSAPI int ds_read(ds_handle dsh,  DSFILE file, void* buffer, int length, int cache_flag);
	typedef int (*Proc_ds_read)(ds_handle dsh,  DSFILE file, void* buffer, int length, int cache_flag);

	/** 
	 * @brief 	ds_write
	 *			写入文件
	 * @return	int 返回真实写入的字节数，返回-1为失败，errno指明失败原因
	 *			返回0为写入缓冲区满，如果返回0，可以稍等片刻后再次写入
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			文件指针
	 * @param	const void* buffer	- [in] 
	 *			准备写入的内容缓存
	 * @param	int length	- [in] 
	 *			准备写入的内容缓存长度
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			是否处理cache
	 */
    DSAPI int ds_write(ds_handle dsh, DSFILE file, const void* buffer, int length, int cache_flag);
	typedef int (*Proc_ds_write)(ds_handle dsh, DSFILE file, const void* buffer, int length, int cache_flag);
	 
	/**
	 * @brief 	ds_flush
	 *			将写入缓冲中的内容flush到存储系统中
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			文件指针
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			是否处理cache
	 */
    DSAPI int ds_flush(ds_handle dsh, DSFILE file, int cache_flag);
	typedef int (*Proc_ds_flush)(ds_handle dsh, DSFILE file, int cache_flag);

	/** 
	 * @brief 	ds_seek
	 *			将文件读取指针移到文件的某一位置
	 *			该函数只对read-only模式适用
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in]
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			文件指针
	 * @param	size_t offset	- [in] 
	 *			位移长度
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			是否处理cache
	 */
    DSAPI int ds_seek(ds_handle dsh, DSFILE file, size_t offset, int cache_flag); 
	typedef int (*Proc_ds_seek)(ds_handle dsh, DSFILE file, size_t offset, int cache_flag); 

	/** 
	 * @brief 	ds_tell
	 *			返回当前文件的offset值，以bytes为单位
	 * @return	size_t 返回当前文件的offset值，出错为-1，errno指明出错原因
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			文件指针
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			是否处理cache
	 */
    DSAPI size_t ds_tell(ds_handle dsh, DSFILE file, int cache_flag);
	typedef size_t (*Proc_ds_tell)(ds_handle dsh, DSFILE file, int cache_flag);

	/** 
	 * @brief 	ds_create_dir
	 *			创建目录（虚拟目录）
				hadoop本身最大支持的目录深度为121级
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			创建的[虚拟]目录路径，支持绝对路径和相对路径	
	 * @param	const char* orignal_path	- [in] 
	 *			创建的目录时该参数传入NULL，创建虚拟目录时该参数传入虚拟目录映射的源目录	
	 */
    DSAPI int ds_create_dir(ds_handle dsh, const char* path, const char* orignal_path);
	typedef int (*Proc_ds_create_dir)(ds_handle dsh, const char* path, const char* orignal_path);

	/** 
	 * @brief 	ds_exists
	 * 			判断一个文件/目录（包括虚拟目录）是否存在
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char *path	- [in] 
	 *			验证的路径名称
	 */
    DSAPI int ds_exists(ds_handle dsh, const char *path);
	typedef int (*Proc_ds_exists)(ds_handle dsh, const char *path);

	/** 
	 * @brief 	ds_delete
	 *			删除一个文件/目录（包括虚拟目录）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			删除的路径名称
	 * @param	int recursive	- [in] 
	 *			如果删除目录该参数的非0值指明递归删除目录中的内容
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			是否处理cache
	 * @Comments: 如果删除的path是真实path，则删除对于真实path按照通常的语义进行，如果该path
	 *            是一个目录且有对应的虚拟目录，则如果这个真实目录被删除，其对应的虚拟目录也被删除
	 *            如果删除的path是虚拟path，如果recursive=1则删除虚拟path和对应的真实path，
	 *                                     如果recursive=0则只删除虚拟目录
	 */
    DSAPI int ds_delete(ds_handle dsh, const char* path, int recursive, int cache_flag);
	typedef int (*Proc_ds_delete)(ds_handle dsh, const char* path, int recursive, int cache_flag);

	/** 
	 * @brief 	ds_rename
	 *			重命名一个文件或目录（包括虚拟目录）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* old_path	- [in] 
	 *			原路径名称
	 * @param	const char* new_path	- [in] 
	 *			新路径名称
	 */
    DSAPI int ds_rename(ds_handle dsh, const char* old_path, const char* new_path);
	typedef int (*Proc_ds_rename)(ds_handle dsh, const char* old_path, const char* new_path);

	/** 
	 * @brief 	ds_setreplication
	 *			设置文件的复制数目 （包括虚拟路径）
				文件的复制数目范围为：1—512
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			路径名称
	 * @param	short replication	- [in] 
	 *			新的复制数目
	 */
    DSAPI int ds_setreplication(ds_handle dsh, const char* path, short replication);
	typedef int (*Proc_ds_setreplication)(ds_handle dsh, const char* path, short replication);

	/** 
	 * @brief 	ds_chown
	 *			设置文件归属
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in]
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in]
	 *			路径名称
	 * @param	const char* owner - [in]
	 *			文件主，如果传入NULL则只设置group
	 * @param	const char* group - [in] 
	 *			文件组，如果传入NULL则只设置owner
	 */
    DSAPI int ds_chown(ds_handle dsh, const char* path, const char *owner, const char *group);
	typedef int (*Proc_ds_chown)(ds_handle dsh, const char* path, const char *owner, const char *group);

	/** 
	 * @brief 	ds_chmod
	 *			设置文件权限
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in]
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in]
	 *			路径名称
	 * @param	short mode - [in]
				文件权限
				hadoop中文件具有用户，组与其它的读写执行权限，形式为rwx。x统一设置为0。如设置文件为用户读写，组读，其它无权限，则应设置为rw_r_____。
				此处参数mode为（110100000）（2进制）所对应的10进制数，即416。
	 *			
	 */
    DSAPI int ds_chmod(ds_handle dsh, const char* path, short mode);
	typedef int (*Proc_ds_chmod)(ds_handle dsh, const char* path, short mode);

	/** 
	 * @brief 	ds_utime
	 *			设置文件的时间信息
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in]
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in]
	 *			路径名称
	 * @param	time_t mtime - [in]
	 *			文件的修改时间，如果传入0，则只设置访问时间
	 * @param	time_t atime - [in]
	 *			文件的访问时间，如果传入0，则只设置修改时间
	 */
	DSAPI int ds_utime(ds_handle dsh, const char* path, time_t mtime, time_t atime);
	typedef int (*Proc_ds_utime)(ds_handle dsh, const char* path, time_t mtime, time_t atime);

	/** 
	 * @brief 	ds_getworkingdir
	 *			活动当前工作目录
	 * @return	char* 如果成功返回指向buffer的指针，否则返回NULL，errno指明出错原因
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	char *buffer	- [in] 
	 *			用来存放当前目录的缓存
	 * @param	size_t length	- [in] 
	 *			缓存长度	
	 */
    DSAPI char* ds_getworkingdir(ds_handle dsh, char *buffer, size_t length);
	typedef char* (*Proc_ds_getworkingdir)(ds_handle dsh, char *buffer, size_t length);

	/** 
	 * @brief 	ds_setworkingdir
	 *			设置当前工作目录
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			设置的目录路径，相对路径则相对于当前路径解析，虚拟路径也要解析		
	 */
    DSAPI int ds_setworkingdir(ds_handle dsh, const char* path);
	typedef int (*Proc_ds_setworkingdir)(ds_handle dsh, const char* path);

	/** 
	 * @brief 	*ds_list_dir
	 *			列出目录（含虚拟目录）中的文件信息
	 * @return	ds_file_info 返回文件信息的数组，失败返回NULL，errno指明出错原因
	 *			该数组由系统动态分配需，用户需要调用释放函数释放
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			目录（含虚拟目录）路径
	 * @param	int *num	- [in] 
	 *			ds_file_info长度
	 */
    DSAPI ds_file_info *ds_list_dir(ds_handle dsh, const char* path, int *num);
	typedef ds_file_info* (*Proc_ds_list_dir)(ds_handle dsh, const char* path, int *num);

	/** 
	 * @brief 	*ds_path_info
	 *			列出文件/目录（含虚拟目录）本身文件信息
	 * @return	ds_file_info 返回文件信息，失败返回NULL，errno指明出错原因
	 *			该信息由系统动态分配需，用户需要调用释放函数释放
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			目录（含虚拟目录）路径
	 */
    DSAPI ds_file_info *ds_path_info(ds_handle dsh, const char* path);
	typedef ds_file_info* (*Proc_ds_path_info)(ds_handle dsh, const char* path);

	/** 
	 * @brief 	ds_free_file_info
	 *			释放ds_file_info信息
	 * @return	No return value.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	ds_file_info *fileinfo	- [in] 
	 *			ds_file_info的数组
	 * @param	int num	- [in] 	
	 *			数组元素的个数
	 */
	DSAPI void ds_free_file_info(ds_handle dsh, ds_file_info *fileinfo, int num);
	typedef void (*Proc_ds_free_file_info)(ds_handle dsh, ds_file_info *fileinfo, int num);

	/** 
	 * @brief 	ds_find_change_notification
	 *			设置文件更新提醒监控文件修改
	 * @return	ds_handle 成功返回一个提醒句柄，失败返回ds_invalid_handle，errno指明失败原因
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			需要监视的文件路径，这个路径目前只支持文件
	 * @param	int watchsubtree	- [in] 
	 *			是否监视子目录和文件，这个参数目前只是为了和Windows API保持一致
	 * @param	int filter	- [in] 
	 *			监控过滤器，这个参数目前只是为了和Windows API保持一致
	 */
	DSAPI ds_handle ds_find_change_notification(ds_handle dsh, const char* path, int watchsubtree, int filter);
	typedef ds_handle (*Proc_ds_find_change_notification)(ds_handle dsh, const char* path, int watchsubtree, int filter);

	/** 
	 * @brief 	ds_wait
	 *			等待句柄
	 * @return	DSAPI int	- Return 0 in success, -1超时，其它为错误代码.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	ds_handle handle	- [in] 
	 *			要等待的句柄
	 * @param	time_t ms	- [in]
	 *			超时时间
	 * @see		
	 */
	DSAPI int ds_wait(ds_handle dsh, ds_handle handle, time_t ms);
	typedef int (*Proc_ds_wait)(ds_handle dsh, ds_handle handle, time_t ms);

	/** 
	 * @brief 	ds_copyfile
	 *			复制文件（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle sdsh	- [in] 
	 *			连接的源存储系统句柄
	 * @param	const char* src	- [in] 
	 *			源路径
	 * @param	ds_handle ddsh	- [in] 
	 *			连接的目标存储系统句柄
	 * @param	const char* dst	- [in] 
	 *			目标路径
	 */
    DSAPI int ds_copyfile(ds_handle sdsh, const char* src, ds_handle ddsh, const char* dst);
	typedef int (*Proc_ds_copyfile)(ds_handle sdsh, const char* src, ds_handle ddsh, const char* dst);

	/** 
	 * @brief 	ds_movefile
	 *			移动文件（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle sdsh	- [in] 
	 *			连接的源存储系统句柄
	 * @param	const char* src	- [in] 
	 *			源路径
	 * @param	ds_handle ddsh	- [in] 
	 *			连接的目标存储系统句柄
	 * @param	const char* dst	- [in] 	
	 *			目标路径
	 */
    DSAPI int ds_movefile(ds_handle sdsh, const char* src, ds_handle ddsh, const char* dst);
	typedef int (*Proc_ds_movefile)(ds_handle sdsh, const char* src, ds_handle ddsh, const char* dst);

	/** 
	 * @brief 	ds_from_local
	 *			从本地拷贝文件到DSS系统上（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* localpath	- [in] 
	 *			本地路径
	 * @param	const char* dst	- [in] 	
	 *			远程路径
	 */
	DSAPI int ds_from_local(ds_handle dsh, const char* localpath, const char* dst);
	typedef int (*Proc_ds_from_local)(ds_handle dsh, const char* localpath, const char* dst);

	/** 
	 * @brief 	ds_sync_from_local
	 *			从本地拷贝文件到DSS系统上（支持虚拟路径）,当前仅提供给数据同步使用
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* localpath	- [in] 
	 *			本地路径
	 * @param	const char* dst	- [in] 	
	 *			远程路径
	 */
	DSAPI int ds_sync_from_local(ds_handle dsh, const char* localpath, const char* dst);
	typedef int (*Proc_ds_sync_from_local)(ds_handle dsh, const char* localpath, const char* dst);

	/** 
	 * @brief 	ds_to_local
	 *			从DSS系统上拷贝文件到本地（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* dst	- [in] 
	 *			远程路径
	 * @param	const char* localpath	- [in] 
	 *			本地路径
	 */
	DSAPI int ds_to_local(ds_handle dsh, const char* dst, const char* localpath);
	typedef int (*Proc_ds_to_local)(ds_handle dsh, const char* dst, const char* localpath);

	/** 
	* @brief 	ds_get_hosts
	*			从DSS系统上查询文件所分布的主机
	* @return	char*** 
	*			返回这些主机的主机地址，如果没有返回空，errno指明出错代码
	* @param	ds_handle dsh	- [in] 
	*			连接的存储系统句柄
	* @param	const char* path	- [in] 
	*			文件路径	
	* @param   size_t start - [in] block的开始位置
	* @param   size_t length - [in] block长度
	*/
	DSAPI char*** ds_get_hosts(ds_handle dsh, const char* path, size_t start, size_t length);
	typedef char*** (*Proc_ds_get_hosts)(ds_handle dsh, const char* path, size_t start, size_t length);

	/** 
	 * @brief 	ds_free_hosts
	 *			删除ds_get_hosts文件分配的用来存放主机地址的内存
	 * @return	No return value.
	 * @param	char ***hosts	- [in] 
	 *	        ds_get_hosts文件分配的用来存放主机地址的内存
	 */
    DSAPI void ds_free_hosts(ds_handle dsh, char ***hosts);
	typedef void (*Proc_ds_free_hosts)(ds_handle dsh, char ***hosts);
	
	/** 
	 * @brief 	ds_default_blocksize
	 *			查询分布式存储系统默认块大小
	 * @return	size_t 
	 *			返回默认块大小，如果返回-1表示出错，errno指明出错代码
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 */
	DSAPI size_t ds_default_blocksize(ds_handle dsh);
	typedef size_t (*Proc_ds_default_blocksize)(ds_handle dsh);

   /** 
	* @brief 	ds_capacity
	*			查询分布式存储系统容量
	* @return	size_t 
	*			返回分布式系统容量，如果返回-1表示出错，errno指明出错代码
	* @param	ds_handle dsh	- [in] 
	*			连接的存储系统句柄
	*/
	DSAPI size_t ds_capacity(ds_handle dsh);
	typedef size_t (*Proc_ds_capacity)(ds_handle dsh);

    /** 
	 * @brief 	ds_used
	 *			查询分布式存储系统存储的总文件大小
	 * @return	size_t 
	 *			返回分布式存储的系统存储文件的总大小，如果返回-1表示出错，errno指明出错代码
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 */
    DSAPI size_t ds_used(ds_handle dsh);
	typedef size_t (*Proc_ds_used)(ds_handle dsh);

	/////////////////////////////////////////////////////////////////////////////////////

	 /** 
	 * @brief 	ds_get_dsproxy
	 *			获得一个dsproxy的地址
	 * @return	int  返回0表示成功，否则为出错代码
	 * @param	char* inbuf	- [in]	   传入的用来获得dsproxy地址的buf 
	 *			size_t inbuflen - [in] 上述缓存区长度
	 *			int* num - [out]	   表示该dsproxy正在服务的客户端数目
	 */
    DSAPI int ds_get_dsproxy(char* inbuf, size_t inbuflen, int* num);
	typedef int (*Proc_ds_get_dsproxy)(char* inbuf, size_t inbuflen, int* num);

	 /** 
	 * @brief 	ds_get_dsproxies
	 *			获得所有DSProxy地址，该地址中的所有地址按照当前服务的实例数递增排序，即负载最轻的排在前面
	 * @return	int  返回0表示成功，否则为出错代码
	 * @param	char* inbuf	- [in]	   传入的用来获得dsproxy地址的buf 
	 *			size_t inbuflen - [in] 上述缓存区长度
	 *			int* num - [out]	   表示正在服务的客户端数目
	  *			size_t innumlen - [in]	num参数的长度
	 *          size_t* counter - [out] 所有的dsproxy的个数
	 * @Comments: 地址用,分隔如dss://192.168.72.71:50180,dss://192.168.72.73:50180
	 */
    DSAPI int ds_get_dsproxies(char* inbuf, size_t inbuflen, int* num, size_t innumlen, size_t* counter);
	typedef int (*Proc_ds_get_dsproxies)(char* inbuf, size_t inbuflen, int* num, size_t innumlen, size_t* counter);

#ifdef WIN32
	 /** 
	 * @brief 	ds_get_current_dsproxy
	 *			获得当前正在使用的dsproxy地址
	 * @return	int  返回0表示成功，否则为出错代码
	 * @param	ds_handle dsh	- [in] 连接句柄 
	 */
	DSAPI const char* ds_get_current_dsproxy(ds_handle dsh);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __IFLY_DSS_H__ */
