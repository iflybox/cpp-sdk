/** 
 * @file	hdfsext.h
 * @brief	HDFS扩展接口
 * 
 * 本文件给出了扩展的HDFS接口
 * 
 *
 * @author	jingzhang
 * @version	1.0
 * @date	2012-2-23
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2012-2-23	<td>jingzhang	<td>Create this file</tr>
 * </table>
 * 
 */

#ifndef __HDFSEXT_H__
#define __HDFSEXT_H__

#include "iFlyDSFile.h"

#ifdef __cplusplus
extern  "C" {
#endif
	
	typedef DS_EVENT_HOOK HDFS_EVENT_HOOK;
	typedef cf_handle     hdfs_handle;
	typedef ds_file_info  hdfs_file_info;
	typedef DSFILE        HDFSFILE;

	#define hdfs_invalid_handle cf_invalid_handle

	/** 
	 * @brief 	hdfs_lasterror
	 *          返回当前出错代码，线程安全
	 * @return	DSAPI int	- Return 0 in success, otherwise return error code.
	 */
	DSAPI int hdfs_lasterror(void);
	typedef int (*Proc_hdfs_lasterror)(void);

	/** 
	 * @brief 	hdfs_initialize_ex
	 *			初始化系统的第一个函数，仅需要调用一次
	 * @return	DSAPI int	- 成功返回0，否则返回失败代码
	 * @param	int enable_dcache	- [in] - 是否使用分布式cache
	 * @param	DS_EVENT_HOOK fn	- [in] - 用来接收系统事件的Callback函数
	 * @param	void* reserv	- [in]
	 *          保留以后使用
	 * @see		
	 */
	ICFAPI int hdfs_initialize_ex(HDFS_EVENT_HOOK fn, void* cbdata, void* reserv);
	typedef int (*Proc_hdfs_initialize_ex)(HDFS_EVENT_HOOK fn, void* cbdata, void* reserv);

	/** 
	 * @brief 	hdfs_uninitialize
	 *			逆初始化的最后一个函数
	 * @return	无返回值，调用后不在可用.
	 * @see		
	 */
	ICFAPI void hdfs_uninitialize(void);
	typedef void (*Proc_hdfs_uninitialize)(void);
	
	/** 
	 * @brief 	hdfs_connect
	 *          连接分布式存储系统
	 * @return	hdfs_handle 如果成功返回存储系统句柄，如果失败返回hdfs_invalid_handle，errno指明失败原因
	 * @param	const char* dsURL	- [in] 分布式存储URL
	 *			dsURLS应符合如下形式 scheme://host:port/dir
	 *				scheme -协议，常用协议是hdfs（Hadoop分布式文件系统）
	 *						还可以是用户定义的虚拟目录如grammar
	 *				dir    -打开目录，如果该目录存在当前目录设置为该目录
	 *                      否则设置为根目录，对于虚拟目录根目录是是指映射
	 *                      到该虚拟目录的DSS目录
	 * @param	const char* uid	- [in]
	 *          指明一个用以表明用户身份的id
	 * @param	void* reserv	- [in]
	 *          保留以后使用
	 * @Comments: 不使用dsproxy时一个连接由dsURL中的host，port和uid共同标识。也就是说如果
	 *            host，port和uid都相同，则该函数应该返回相同的句柄
	 *            使用dsproxy时连接除了上述标识外还应该加上dsproxy的地址，也就是说客户端通过
	 *            不同的dsproxy以同一用户名连接同一个dsURL，返回不同的句柄
	 */
	ICFAPI hdfs_handle hdfs_connect(const char* dsURL, const char* uid, void* reserv);
	typedef hdfs_handle (*Proc_hdfs_connect)(const char* dsURL, const char* uid, void* reserv);

	/** 
	 * @brief 	hdfs_disconnect
	 *          断开分布式文件系统
	 * @return	int - Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *          hdfs_connect函数返回的句柄 
	 */
	ICFAPI int hdfs_disconnect(hdfs_handle dsh);
	typedef int (*Proc_hdfs_disconnect)(hdfs_handle dsh);

	/** 
	 * @brief 	hdfs_fopen
	 *			打开文件
	 * @return	HDFSFILE 成功返回文件描述付，失败返回NULL，errno指明失败原因
	 * @param	hdfs_handle dsh	- [in] 
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
	 * @Comments: buffersize是打开的流的缓冲区大小，该值被最终传给java的BufferedInputStream和BufferedOutputStream
	 *            两个类的构造函数，在Java SE的实现中如果不指定这个值，则默认为8192字节
	 *            hadoop.configuration （io.file.buffer.size）和代码中定义的该值为4096.
	 */
	ICFAPI HDFSFILE hdfs_fopen(hdfs_handle dsh, const char* path, int flags,
		int buffersize, short replication, int blocksize);
	typedef HDFSFILE (*Proc_hdfs_fopen)(hdfs_handle dsh, const char* path, int flags,
		int buffersize, short replication, int blocksize);

	/** 
	 * @brief 	hdfs_fclose
	 *			关闭文件
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	HDFSFILE file	- [in] 
	 *			准备关闭的文件指针	
	 */
    ICFAPI int hdfs_fclose(hdfs_handle dsh, HDFSFILE file);
	typedef int (*Proc_hdfs_fclose)(hdfs_handle dsh, HDFSFILE file);

	/** 
	 * @brief 	hdfs_read
	 *			读取文件
	 * @return	int 返回真实读取的字节数，返回-1为失败，errno指明失败原因
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	HDFSFILE file	- [in] 
	 *			文件指针
	 * @param	void* buffer	- [in] 
	 *			读取缓冲区
	 * @param	int length	- [in] range:(0, 0x7FFFFFFF]
	 *			缓冲区长度
	 */
    ICFAPI int hdfs_read(hdfs_handle dsh,  HDFSFILE file, void* buffer, int length);
	typedef int (*Proc_hdfs_read)(hdfs_handle dsh,  HDFSFILE file, void* buffer, int length);

	/** 
	 * @brief 	hdfs_write
	 *			写入文件
	 * @return	int 返回真实写入的字节数，返回-1为失败，errno指明失败原因
	 *			返回0为写入缓冲区满，如果返回0，可以稍等片刻后再次写入
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	HDFSFILE file	- [in] 
	 *			文件指针
	 * @param	const void* buffer	- [in] 
	 *			准备写入的内容缓存
	 * @param	int length	- [in] 
	 *			准备写入的内容缓存长度
	 */
    ICFAPI int hdfs_write(hdfs_handle dsh, HDFSFILE file, const void* buffer, int length);
	typedef int (*Proc_hdfs_write)(hdfs_handle dsh, HDFSFILE file, const void* buffer, int length);
	 
	/**
	 * @brief 	hdfs_flush
	 *			将写入缓冲中的内容flush到存储系统中
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	HDFSFILE file	- [in] 
	 *			文件指针
	 */
    ICFAPI int hdfs_flush(hdfs_handle dsh, HDFSFILE file);
	typedef int (*Proc_hdfs_flush)(hdfs_handle dsh, HDFSFILE file);

	/** 
	 * @brief 	hdfs_seek
	 *			将文件读取指针移到文件的某一位置
	 *			该函数只对read-only模式适用
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in]
	 *			连接的存储系统句柄
	 * @param	HDFSFILE file	- [in] 
	 *			文件指针
	 * @param	size_t offset	- [in] 
	 *			位移长度
	 */
    ICFAPI int hdfs_seek(hdfs_handle dsh, HDFSFILE file, size_t offset); 
	typedef int (*Proc_hdfs_seek)(hdfs_handle dsh, HDFSFILE file, size_t offset); 

	/** 
	 * @brief 	hdfs_tell
	 *			返回当前文件的offset值，以bytes为单位
	 * @return	size_t 返回当前文件的offset值，出错为-1，errno指明出错原因
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	HDFSFILE file	- [in] 
	 *			文件指针
	 */
    ICFAPI size_t hdfs_tell(hdfs_handle dsh, HDFSFILE file);
	typedef size_t (*Proc_hdfs_tell)(hdfs_handle dsh, HDFSFILE file);

	/** 
	 * @brief 	hdfs_create_dir
	 *			创建目录（虚拟目录）
				hadoop本身最大支持的目录深度为121级
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			创建的[虚拟]目录路径，支持绝对路径和相对路径	
	 * @param	const char* orignal_path	- [in] 
	 *			创建的目录时该参数传入NULL，创建虚拟目录时该参数传入虚拟目录映射的源目录	
	 */
    ICFAPI int hdfs_create_dir(hdfs_handle dsh, const char* path, const char* orignal_path);
	typedef int (*Proc_hdfs_create_dir)(hdfs_handle dsh, const char* path, const char* orignal_path);

	/** 
	 * @brief 	hdfs_exists
	 * 			判断一个文件/目录（包括虚拟目录）是否存在
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char *path	- [in] 
	 *			验证的路径名称
	 */
    ICFAPI int hdfs_exists(hdfs_handle dsh, const char *path);
	typedef int (*Proc_hdfs_exists)(hdfs_handle dsh, const char *path);

	/** 
	 * @brief 	hdfs_delete
	 *			删除一个文件/目录（包括虚拟目录）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
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
    ICFAPI int hdfs_delete(hdfs_handle dsh, const char* path, int recursive);
	typedef int (*Proc_hdfs_delete)(hdfs_handle dsh, const char* path, int recursive);

	/** 
	 * @brief 	hdfs_rename
	 *			重命名一个文件或目录（包括虚拟目录）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* old_path	- [in] 
	 *			原路径名称
	 * @param	const char* new_path	- [in] 
	 *			新路径名称
	 */
    ICFAPI int hdfs_rename(hdfs_handle dsh, const char* old_path, const char* new_path);
	typedef int (*Proc_hdfs_rename)(hdfs_handle dsh, const char* old_path, const char* new_path);

	/** 
	 * @brief 	hdfs_setreplication
	 *			设置文件的复制数目 （包括虚拟路径）
				文件的复制数目范围为：1—512
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			路径名称
	 * @param	short replication	- [in] 
	 *			新的复制数目
	 */
    ICFAPI int hdfs_setreplication(hdfs_handle dsh, const char* path, short replication);
	typedef int (*Proc_hdfs_setreplication)(hdfs_handle dsh, const char* path, short replication);

	/** 
	 * @brief 	hdfs_chown
	 *			设置文件归属
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in]
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in]
	 *			路径名称
	 * @param	const char* owner - [in]
	 *			文件主，如果传入NULL则只设置group
	 * @param	const char* group - [in] 
	 *			文件组，如果传入NULL则只设置owner
	 */
    ICFAPI int hdfs_chown(hdfs_handle dsh, const char* path, const char *owner, const char *group);
	typedef int (*Proc_hdfs_chown)(hdfs_handle dsh, const char* path, const char *owner, const char *group);

	/** 
	 * @brief 	hdfs_chmod
	 *			设置文件权限
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in]
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in]
	 *			路径名称
	 * @param	short mode - [in]
				文件权限
				hadoop中文件具有用户，组与其它的读写执行权限，形式为rwx。x统一设置为0。如设置文件为用户读写，组读，其它无权限，则应设置为rw_r_____。
				此处参数mode为（110100000）（2进制）所对应的10进制数，即416。
	 *			
	 */
    ICFAPI int hdfs_chmod(hdfs_handle dsh, const char* path, short mode);
	typedef int (*Proc_hdfs_chmod)(hdfs_handle dsh, const char* path, short mode);

	/** 
	 * @brief 	hdfs_utime
	 *			设置文件的时间信息
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in]
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in]
	 *			路径名称
	 * @param	time_t mtime - [in]
	 *			文件的修改时间，如果传入0，则只设置访问时间
	 * @param	time_t atime - [in]
	 *			文件的访问时间，如果传入0，则只设置修改时间
	 */
	ICFAPI int hdfs_utime(hdfs_handle dsh, const char* path, time_t mtime, time_t atime);
	typedef int (*Proc_hdfs_utime)(hdfs_handle dsh, const char* path, time_t mtime, time_t atime);

	/** 
	 * @brief 	hdfs_getworkingdir
	 *			活动当前工作目录
	 * @return	char* 如果成功返回指向buffer的指针，否则返回NULL，errno指明出错原因
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	char *buffer	- [in] 
	 *			用来存放当前目录的缓存
	 * @param	size_t length	- [in] 
	 *			缓存长度	
	 */
    ICFAPI char* hdfs_getworkingdir(hdfs_handle dsh, char *buffer, size_t length);
	typedef char* (*Proc_hdfs_getworkingdir)(hdfs_handle dsh, char *buffer, size_t length);

	/** 
	 * @brief 	hdfs_setworkingdir
	 *			设置当前工作目录
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			设置的目录路径，相对路径则相对于当前路径解析，虚拟路径也要解析		
	 */
    ICFAPI int hdfs_setworkingdir(hdfs_handle dsh, const char* path);
	typedef int (*Proc_hdfs_setworkingdir)(hdfs_handle dsh, const char* path);

	/** 
	 * @brief 	*hdfs_list_dir
	 *			列出目录（含虚拟目录）中的文件信息
	 * @return	hdfs_file_info 返回文件信息的数组，失败返回NULL，errno指明出错原因
	 *			该数组由系统动态分配需，用户需要调用释放函数释放
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			目录（含虚拟目录）路径
	 * @param	int *num	- [in] 
	 *			hdfs_file_info长度
	 */
    ICFAPI hdfs_file_info *hdfs_list_dir(hdfs_handle dsh, const char* path, int *num);
	typedef hdfs_file_info* (*Proc_hdfs_list_dir)(hdfs_handle dsh, const char* path, int *num);

	/** 
	 * @brief 	*hdfs_path_info
	 *			列出文件/目录（含虚拟目录）本身文件信息
	 * @return	hdfs_file_info 返回文件信息，失败返回NULL，errno指明出错原因
	 *			该信息由系统动态分配需，用户需要调用释放函数释放
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			目录（含虚拟目录）路径
	 */
    ICFAPI hdfs_file_info *hdfs_path_info(hdfs_handle dsh, const char* path);
	typedef hdfs_file_info* (*Proc_hdfs_path_info)(hdfs_handle dsh, const char* path);

	/** 
	 * @brief 	hdfs_free_file_info
	 *			释放hdfs_file_info信息
	 * @return	No return value.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	hdfs_file_info *fileinfo	- [in] 
	 *			hdfs_file_info的数组
	 * @param	int num	- [in] 	
	 *			数组元素的个数
	 */
	ICFAPI void hdfs_free_file_info(hdfs_handle dsh, hdfs_file_info *fileinfo, int num);
	typedef void (*Proc_hdfs_free_file_info)(hdfs_handle dsh, hdfs_file_info *fileinfo, int num);

	/** 
	 * @brief 	hdfs_find_change_notification
	 *			设置文件更新提醒监控文件修改
	 * @return	hdfs_handle 成功返回一个提醒句柄，失败返回hdfs_invalid_handle，errno指明失败原因
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			需要监视的文件路径，这个路径目前只支持文件
	 * @param	int watchsubtree	- [in] 
	 *			是否监视子目录和文件，这个参数目前只是为了和Windows API保持一致
	 * @param	int filter	- [in] 
	 *			监控过滤器，这个参数目前只是为了和Windows API保持一致
	 */
	ICFAPI hdfs_handle hdfs_find_change_notification(hdfs_handle dsh, const char* path, int watchsubtree, int filter);
	typedef hdfs_handle (*Proc_hdfs_find_change_notification)(hdfs_handle dsh, const char* path, int watchsubtree, int filter);

	/** 
	 * @brief 	hdfs_wait
	 *			等待句柄
	 * @return	DSAPI int	- Return 0 in success, -1超时，其它为错误代码.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	hdfs_handle handle	- [in] 
	 *			要等待的句柄
	 * @param	time_t ms	- [in]
	 *			超时时间
	 * @see		
	 */
	ICFAPI int hdfs_wait(hdfs_handle dsh, hdfs_handle handle, time_t ms);
	typedef int (*Proc_hdfs_wait)(hdfs_handle dsh, hdfs_handle handle, time_t ms);

	/** 
	 * @brief 	hdfs_copyfile
	 *			复制文件（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle sdsh	- [in] 
	 *			连接的源存储系统句柄
	 * @param	const char* src	- [in] 
	 *			源路径
	 * @param	hdfs_handle ddsh	- [in] 
	 *			连接的目标存储系统句柄
	 * @param	const char* dst	- [in] 
	 *			目标路径
	 */
    ICFAPI int hdfs_copyfile(hdfs_handle sdsh, const char* src, hdfs_handle ddsh, const char* dst);
	typedef int (*Proc_hdfs_copyfile)(hdfs_handle sdsh, const char* src, hdfs_handle ddsh, const char* dst);

	/** 
	 * @brief 	hdfs_movefile
	 *			移动文件（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle sdsh	- [in] 
	 *			连接的源存储系统句柄
	 * @param	const char* src	- [in] 
	 *			源路径
	 * @param	hdfs_handle ddsh	- [in] 
	 *			连接的目标存储系统句柄
	 * @param	const char* dst	- [in] 	
	 *			目标路径
	 */
    ICFAPI int hdfs_movefile(hdfs_handle sdsh, const char* src, hdfs_handle ddsh, const char* dst);
	typedef int (*Proc_hdfs_movefile)(hdfs_handle sdsh, const char* src, hdfs_handle ddsh, const char* dst);

	/** 
	 * @brief 	hdfs_from_local
	 *			从本地拷贝文件到DSS系统上（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* localpath	- [in] 
	 *			本地路径
	 * @param	const char* dst	- [in] 	
	 *			远程路径
	 */
	ICFAPI int hdfs_from_local(hdfs_handle dsh, const char* localpath, const char* dst);
	typedef int (*Proc_hdfs_from_local)(hdfs_handle dsh, const char* localpath, const char* dst);

	/** 
	 * @brief 	hdfs_to_local
	 *			从DSS系统上拷贝文件到本地（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* dst	- [in] 
	 *			远程路径
	 * @param	const char* localpath	- [in] 
	 *			本地路径
	 */
	ICFAPI int hdfs_to_local(hdfs_handle dsh, const char* dst, const char* localpath);
	typedef int (*Proc_hdfs_to_local)(hdfs_handle dsh, const char* dst, const char* localpath);

	/** 
	* @brief 	hdfs_get_hosts
	*			从DSS系统上查询文件所分布的主机
	* @return	char*** 
	*			返回这些主机的主机地址，如果没有返回空，errno指明出错代码
	* @param	hdfs_handle dsh	- [in] 
	*			连接的存储系统句柄
	* @param	const char* path	- [in] 
	*			文件路径	
	* @param   size_t start - [in] block的开始位置
	* @param   size_t length - [in] block长度
	*/
	ICFAPI char*** hdfs_get_hosts(hdfs_handle dsh, const char* path, size_t start, size_t length);
	typedef char*** (*Proc_hdfs_get_hosts)(hdfs_handle dsh, const char* path, size_t start, size_t length);

	/** 
	 * @brief 	hdfs_free_hosts
	 *			删除hdfs_get_hosts文件分配的用来存放主机地址的内存
	 * @return	No return value.
	 * @param	char ***hosts	- [in] 
	 *	        hdfs_get_hosts文件分配的用来存放主机地址的内存
	 */
    ICFAPI void hdfs_free_hosts(hdfs_handle dsh, char ***hosts);
	typedef void (*Proc_hdfs_free_hosts)(hdfs_handle dsh, char ***hosts);
	
	/** 
	 * @brief 	hdfs_default_blocksize
	 *			查询分布式存储系统默认块大小
	 * @return	size_t 
	 *			返回默认块大小，如果返回-1表示出错，errno指明出错代码
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 */
	ICFAPI size_t hdfs_default_blocksize(hdfs_handle dsh);
	typedef size_t (*Proc_hdfs_default_blocksize)(hdfs_handle dsh);

   /** 
	* @brief 	hdfs_capacity
	*			查询分布式存储系统容量
	* @return	size_t 
	*			返回分布式系统容量，如果返回-1表示出错，errno指明出错代码
	* @param	hdfs_handle dsh	- [in] 
	*			连接的存储系统句柄
	*/
	ICFAPI size_t hdfs_capacity(hdfs_handle dsh);
	typedef size_t (*Proc_hdfs_capacity)(hdfs_handle dsh);

    /** 
	 * @brief 	hdfs_used
	 *			查询分布式存储系统存储的总文件大小
	 * @return	size_t 
	 *			返回分布式存储的系统存储文件的总大小，如果返回-1表示出错，errno指明出错代码
	 * @param	hdfs_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 */
    ICFAPI size_t hdfs_used(hdfs_handle dsh);
	typedef size_t (*Proc_hdfs_used)(hdfs_handle dsh);

	/** 
	 * @brief 	hdfs_get_dsproxy
	 *			获得一个dsproxy的地址
	 * @return	int  返回0表示成功，否则为出错代码
	 * @param	char* inbuf	- [in]	   传入的用来获得dsproxy地址的buf 
	 *			size_t inbuflen - [in] 上述缓存区长度
	 *			int* num - [out]	   表示该dsproxy正在服务的客户端数目
	 */
    ICFAPI int hdfs_get_dsproxy(char* inbuf, size_t inbuflen, int* num);
	typedef int (*Proc_hdfs_get_dsproxy)(char* inbuf, size_t inbuflen, int* num);

	 /** 
	 * @brief 	hdfs_get_dsproxies
	 *			获得所有DSProxy地址，该地址中的所有地址按照当前服务的实例数递增排序，即负载最轻的排在前面
	 * @return	int  返回0表示成功，否则为出错代码
	 * @param	char* inbuf	- [in]	   传入的用来获得dsproxy地址的buf 
	 *			size_t inbuflen - [in] 上述缓存区长度
	 *			int* num - [out]	   表示正在服务的客户端数目
	  *			size_t innumlen - [in]	num参数的长度
	 *          size_t* counter - [out] 所有的dsproxy的个数
	 * @Comments: 地址用,分隔如dss://192.168.72.71:50180,dss://192.168.72.73:50180
	 */
    ICFAPI int hdfs_get_dsproxies(char* inbuf, size_t inbuflen, int* num, size_t innumlen, size_t* counter);
	typedef int (*Proc_hdfs_get_dsproxies)(char* inbuf, size_t inbuflen, int* num, size_t innumlen, size_t* counter);


#ifdef __cplusplus
}
#endif

#endif /* __IFLY_DSS_H__ */
