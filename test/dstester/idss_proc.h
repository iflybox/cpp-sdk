//#include "iFlyDSS.h"

#ifdef __cplusplus
extern  "C" {
#endif

	/** 
	 * @brief 	ds_connect
	 *          连接分布式存储系统
	 * @return	ds_handle 如果成功返回存储系统句柄，如果失败返回ds_invalid_handle
	 * @param	const char* url	- [in]
	 *			url应符合如下形式 scheme://host:port/dir
	 *				scheme -协议，常用协议是dss（分布式存储系统）
	 *						还可以是用户定义的虚拟目录如grammar
	 *				dir    -打开目录，如果该目录存在当前目录设置为该目录
	 *                      否则设置为根目录，对于虚拟目录根目录是是指映射
	 *                      到该虚拟目录的DSS目录
	 * @param	const char* uid	- [in]
	 *          指明一个用以表明用户身份的id
	 * @param	bool keepalive	- [in]
	 *          指明该连接是否是一个使用keepalive的长连接，默认为true
	 */
	//DSAPI ds_handle ds_connect(const char* url, const char* uid, bool keepalive);
	typedef ds_handle (DSAPI *Proc_ds_connect)(const char* url, const char* uid, bool keepalive);

	/** 
	 * @brief 	ds_disconnect
	 *          断开分布式文件系统
	 * @return	int - Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *          ds_connect函数返回的句柄 
	 */
	//DSAPI int ds_disconnect(ds_handle dsh);
	typedef int (DSAPI *Proc_ds_disconnect)(ds_handle dsh);

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
	 * @param	int bufferSize	- [in] 
	 *			指定打开文件时缓冲区的大小，0则使用默认值
	 * @param	short replication	- [in] 
	 *			指定文件副本的数目，0则使用默认值
	 * @param	size_t blocksize	- [in] 
	 *			指定文件块的大小，0则使用默认值
	 */
	/*DSAPI DSFILE ds_fopen(ds_handle dsh, const char* path, int flags,
		int bufferSize, short replication, size_t blocksize);*/
	typedef DSFILE (DSAPI *Proc_ds_fopen)(ds_handle dsh, const char* path, int flags, int bufferSize, short replication, size_t blocksize);

	/** 
	 * @brief 	ds_fclose
	 *			关闭文件
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			准备关闭的文件指针	
	 */
    //DSAPI int ds_fclose(ds_handle dsh, DSFILE file);
	typedef int (DSAPI *Proc_ds_fclose)(ds_handle dsh, DSFILE file);

	/** 
	 * @brief 	ds_read
	 *			读取文件
	 * @return	size_t 返回真实读取的字节数，返回-1为失败 
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			文件指针
	 * @param	void* buffer	- [in] 
	 *			读取缓冲区
	 * @param	size_t length	- [in] 
	 *			缓冲区长度
	 */
    //DSAPI size_t ds_read(ds_handle dsh,  DSFILE file, void* buffer, size_t length);
	typedef size_t (DSAPI *Proc_ds_read)(ds_handle dsh,  DSFILE file, void* buffer, size_t length);
	
	/** 
	 * @brief 	ds_write
	 *			写入文件
	 * @return	size_t 返回真实写入的字节数，返回-1为失败
	 *			返回0为写入缓冲区满，如果返回0，可以稍等片刻后再次写入
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			文件指针
	 * @param	const void* buffer	- [in] 
	 *			准备写入的内容缓存
	 * @param	size_t length	- [in] 
	 *			准备写入的内容缓存长度
	 */
    //DSAPI size_t ds_write(ds_handle dsh, DSFILE file, const void* buffer, size_t length);
	typedef size_t (DSAPI *Proc_ds_write)(ds_handle dsh, DSFILE file, const void* buffer, size_t length);
	
	/**
	 * @brief 	ds_flush
	 *			将写入缓冲中的内容flush到存储系统中
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			文件指针
	 */
    //DSAPI int ds_flush(ds_handle dsh, DSFILE file);
	typedef int (DSAPI *Proc_ds_flush)(ds_handle dsh, DSFILE file);

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
	 */
    //DSAPI int ds_seek(ds_handle dsh, DSFILE file, size_t offset);
	typedef int (DSAPI *Proc_ds_seek)(ds_handle dsh, DSFILE file, size_t offset);

	/** 
	 * @brief 	ds_tell
	 *			返回当前文件的offset值，以bytes为单位
	 * @return	size_t 返回当前文件的offset值，出错为-1
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	DSFILE file	- [in] 
	 *			文件指针
	 */
    //DSAPI size_t ds_tell(ds_handle dsh, DSFILE file);
	typedef size_t (DSAPI *Proc_ds_tell)(ds_handle dsh, DSFILE file);

	/** 
	 * @brief 	ds_create_dir
	 *			创建目录（虚拟目录）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			创建的[虚拟]目录路径，支持绝对路径和相对路径	
	 * @param	const char* orignal_path	- [in] 
	 *			创建的目录时该参数传入NULL，创建虚拟目录时该参数传入虚拟目录映射的源目录	
	 */
    //DSAPI int ds_create_dir(ds_handle dsh, const char* path, const char* orignal_path);
	typedef int (DSAPI *Proc_ds_create_dir)(ds_handle dsh, const char* path, const char* orignal_path);

	/** 
	 * @brief 	ds_exists
	 * 			判断一个文件/目录（包括虚拟目录）是否存在
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char *path	- [in] 
	 *			验证的路径名称
	 */
    //DSAPI int ds_exists(ds_handle dsh, const char *path);
	typedef int (DSAPI *Proc_ds_exists)(ds_handle dsh, const char *path);

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
	 */
    //DSAPI int ds_delete(ds_handle dsh, const char* path, int recursive);
	typedef int (DSAPI *Proc_ds_delete)(ds_handle dsh, const char* path, int recursive);

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
    //DSAPI int ds_rename(ds_handle dsh, const char* old_path, const char* new_path);
	typedef int (DSAPI *Proc_ds_rename)(ds_handle dsh, const char* old_path, const char* new_path);

	/** 
	 * @brief 	ds_getworkingdir
	 *			活动当前工作目录
	 * @return	char* 如果成功返回指向buffer的指针，否则返回NULL
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	char *buffer	- [in] 
	 *			用来存放当前目录的缓存
	 * @param	size_t length	- [in] 
	 *			缓存长度	
	 */
    //DSAPI char* ds_getworkingdir(ds_handle dsh, char *buffer, size_t length);
	typedef char* (DSAPI *Proc_ds_getworkingdir)(ds_handle dsh, char *buffer, size_t length);

	/** 
	 * @brief 	ds_setworkingdir
	 *			设置当前工作目录
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			设置的目录路径，相对路径则相对于当前路径解析，虚拟路径也要解析		
	 */
    //DSAPI int ds_setworkingdir(ds_handle dsh, const char* path);
	typedef int (DSAPI *Proc_ds_setworkingdir)(ds_handle dsh, const char* path);

	/** 
	 * @brief 	*ds_list_dir
	 *			列出目录（含虚拟目录）中的文件信息
	 * @return	ds_file_info 返回文件信息的数组，失败返回NULL
	 *			该数组由系统动态分配需，用户需要调用释放函数释放
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			目录（含虚拟目录）路径
	 * @param	int *num	- [in] 
	 *			ds_file_info长度
	 */
    //DSAPI ds_file_info *ds_list_dir(ds_handle dsh, const char* path, int *num);
	typedef ds_file_info (DSAPI **Proc_ds_list_dir)(ds_handle dsh, const char* path, int *num);

	/** 
	 * @brief 	*ds_path_info
	 *			列出文件/目录（含虚拟目录）本身文件信息
	 * @return	ds_file_info 返回文件信息，失败返回NULL
	 *			该信息由系统动态分配需，用户需要调用释放函数释放
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			目录（含虚拟目录）路径
	 */
    //DSAPI ds_file_info *ds_path_info(ds_handle dsh, const char* path);
	typedef ds_file_info (DSAPI **Proc_ds_path_info)(ds_handle dsh, const char* path);

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
	//DSAPI void ds_free_file_info(ds_handle dsh, ds_file_info *fileinfo, int num);
	typedef void (DSAPI *Proc_ds_free_file_info)(ds_handle dsh, ds_file_info *fileinfo, int num);

	/** 
	 * @brief 	ds_find_change_notification
	 *			设置文件更新提醒监控文件修改
	 * @return	ds_handle 成功返回一个提醒句柄
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			需要监视的文件路径，这个路径目前只支持文件
	 * @param	int watchsubtree	- [in] 
	 *			是否监视子目录和文件，这个参数目前只是为了和Windows API保持一致
	 * @param	int filter	- [in] 
	 *			监控过滤器，这个参数目前只是为了和Windows API保持一致
	 */
	//DSAPI ds_handle ds_find_change_notification(ds_handle dsh, const char* path, int watchsubtree, int filter);
	typedef ds_handle (DSAPI *Proc_ds_find_change_notification)(ds_handle dsh, const char* path, int watchsubtree, int filter);

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
	//DSAPI int ds_wait(ds_handle dsh, ds_handle handle, time_t ms);
	typedef int (DSAPI *Proc_ds_wait)(ds_handle dsh, ds_handle handle, time_t ms);

	/** 
	 * @brief 	ds_copyfile
	 *			复制文件（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* src	- [in] 
	 *			源路径
	 * @param	const char* dst	- [in] 
	 *			目标路径
	 */
    //DSAPI int ds_copyfile(ds_handle dsh, const char* src, const char* dst);
	typedef int (DSAPI *Proc_ds_copyfile)(ds_handle dsh, const char* src, const char* dst);

	/** 
	 * @brief 	ds_movefile
	 *			移动文件（支持虚拟路径）
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* src	- [in] 
	 *			源路径
	 * @param	const char* dst	- [in] 	
	 *			目标路径
	 */
    //DSAPI int ds_movefile(ds_handle dsh, const char* src, const char* dst);
	typedef int (DSAPI *Proc_ds_movefile)(ds_handle dsh, const char* src, const char* dst);

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
	//DSAPI int ds_from_local(ds_handle dsh, const char* localpath, const char* dst);
	typedef int (DSAPI *Proc_ds_from_local)(ds_handle dsh, const char* localpath, const char* dst);

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
	//DSAPI int ds_to_local(ds_handle dsh, const char* dst, const char* localpath);
	typedef int (DSAPI *Proc_ds_to_local)(ds_handle dsh, const char* dst, const char* localpath);

	/** 
	 * @brief 	ds_get_hosts
	 *			从DSS系统上查询文件所分布的主机
	 * @return	char*** 
	 *			返回这些主机的主机地址，如果没有返回空
	 * @param	ds_handle dsh	- [in] 
	 *			连接的存储系统句柄
	 * @param	const char* path	- [in] 
	 *			文件路径	
	 */
    //char*** ds_get_hosts(ds_handle dsh, const char* path);
	typedef char*** (*Proc_ds_get_hosts)(ds_handle dsh, const char* path);

	/** 
	 * @brief 	ds_free_hosts
	 *			删除ds_get_hosts文件分配的用来存放主机地址的内存
	 * @return	No return value.
	 * @param	char ***hosts	- [in] 
	 *	        ds_get_hosts文件分配的用来存放主机地址的内存
	 */
    //void ds_free_hosts(char ***hosts);
	typedef void (*Proc_ds_free_hosts)(char ***hosts);

#ifdef __cplusplus
}
#endif