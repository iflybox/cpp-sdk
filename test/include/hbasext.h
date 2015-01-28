/** 
 * @file	hbase.h
 * @brief	hbase扩展接口
 * 
 * 本文件声明了讯飞分布式数据库系统的API函数原型
 * 给出了各API函数语义参数和返回值类型和取值范围
 *
 * @author	gyfan
 * @version	1.0
 * @date	2012-02-13
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2012-02-13	<td>gyfan	<td>Create this file</tr>
 * </table>
 * 
 */

#ifndef __HBASEEXT_H
#define __HBASEEXT_H

#include "iFlyDSFile.h"
	
#ifdef __cplusplus
	extern	"C" {
#endif

	typedef void * base_handler;
	typedef void * table_handler;

	typedef struct hbaseKeyInfo {		
		char * table;
		char * row;
		char * family;
		char * qualifier;
		long timestamp;
	}hbaseKeyInfo;

	typedef enum {
		IB_ERROR_MasterNotRunning,
		IB_ERROR_NoServerForRegion, 
		IB_ERROR_Region,
		IB_ERROR_RegionOffline, 
		IB_ERROR_RetriesExhausted,
		IB_ERROR_ScannerTimeout,
		IB_ERROR_TableExists,
		IB_ERROR_TableNotEnabled,
		IB_ERROR_TableNotFound,
		IB_ERROR_UnknownRegion, 
		IB_ERROR_UnknownScanner,
		IB_ERROR_ZooKeeperConnection
	}ib_error_t;
	/** 
	 * @brief 	ibase_init
	 *			初始化系统的第一个函数，仅需要调用一次
	 * @return	base_handler	- 如果成功返回系统句柄，如果失败返回NULL
	 * @param	const char* zookeeper_url	- [in] 
	 *			zookeeper 访问url
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI base_handler ibase_init(const char *zookeeper_url, int* errnum);
	typedef base_handler (*Proc_ibase_init)(const char *zookeeper_url, int* errnum);

	/** 
	 * @brief 	ibase_connect
	 *			连接目标表操作,单个线程不可复用其它线程的连接句柄，而应进行该函数的调用，获取新句柄
	 * @return	table_handler	- 如果成功返回系统句柄，如果失败返回NULL
	 * @param	base_handler base_handle	- [in] 
	 *			ibase_init返回的句柄
	 * @param	const char* table	- [in] 
	 *			目标表名
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI table_handler ibase_connect(base_handler base_handle, const char* table, int* errnum);
	typedef table_handler (*Proc_ibase_connect)(base_handler base_handle, const char* table, int* errnum);


	/** 
	 * @brief 	ibase_disconnect
	 *			连接目标表操作
	 * @return	bool 	- 成功返回ture，如果失败false
	 * @param	table_handler handle	- [in] 
	 *			ibase_connect返回的句柄
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI bool ibase_disconnect(table_handler handle, int* errnum);
	typedef bool(*Proc_ibase_disconnect)(table_handler handle, int* errnum);

	/** 
	 * @brief 	ibase_table_exist
	 *			判断一个ibase表是否存在
	 * @return	bool 	- Return true in success, otherwise return false
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect返回的句柄
	 * @param	const char *table	- [in] 
	 *			查询的目标表名称
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI int ibase_table_exist(table_handler handler,const char *table, int* errnum);
	typedef int (*Proc_ibase_table_exist)(table_handler handler,const char *table, int* errnum);

	/** 
	 * @brief 	ibase_get_Families
	 *			返回目标表所对应的各个列的名称
	 * @return	char** 	- 成功返回对应表的列名称地址，如果失败返回NULL
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect返回的句柄
	 * @param	const char *table	- [in] 
	 *			查询的目标表名称
	 * @param	int *numfamilies	- [out] 
	 *			该表所对应的列的总数（不大于256）
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI char** ibase_get_families(table_handler handler,const char *table, int *numfamilies, int* errnum);
	typedef char** (*Proc_ibase_get_families)(table_handler handler,const char *table, int *numfamilies, int* errnum);

	/** 
	 * @brief 	ibase_Families_free
	 *			删除ibase_get_Families文件分配的用来列名称的内存空间
	 * @return	No return value.
	 * @param	char ** cfamilies	- [in] 
	 *			调用ibase_get_Families所返回的内存地址
	 * @param	int num_families	- [in] 
	 *			调用ibase_get_Families 返回的列的总数
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI void ibase_families_free(char ** cfamilies, int num_families, int* errnum);
	typedef void (*Proc_ibase_families_free)(char ** cfamilies, int num_families, int* errnum);

	/** 
	 * @brief 	ibase_family_exist
	 *			判断指定分布式表中特定列族是否存在
	 * @return	bool 	- Return true in success, otherwise return false
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect返回的句柄
	 * @param	hbaseKeyInfo* hb_info	- [in] 
	 *			hb_info指定上传数据对应的具体表信息，包括目标表名称，目标行名称，目标表族名称，目标表限定名称，以及版本号
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI bool ibase_family_exist(table_handler handler, hbaseKeyInfo* hb_info, int* errnum);
	typedef bool (*Proc_ibase_family_exist)(table_handler handler, hbaseKeyInfo* hb_info, int* errnum);

	/** 
	 * @brief 	ibase_put
	 *			向分布式数据库上传数据
	 * @return	bool 	- Return true in success, otherwise return false
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect返回的句柄
	 * @param	hbaseKeyInfo* hb_info	- [in] 
	 *			hb_info指定上传数据对应的具体表信息，包括目标表名称，目标行名称，目标表族名称，目标表限定名称，以及版本号
	 * @param	void *buff	- [in] 
	 *			准备写入的内容缓存
	 * @param	int len 	- [in] 
	 *			准备写入的内容缓存长度
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI bool ibase_put(table_handler handler, hbaseKeyInfo* hb_info,void *buff,int len, int* errnum);
	typedef bool (*Proc_ibase_put)(table_handler handler, hbaseKeyInfo* hb_info,void *buff,int len, int* errnum);

	/** 
	 * @brief 	ibase_get
	 *			从分布式数据库读出数据
	 * @return	long 	- 返回真实读取的字节数，返回-1为失败
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect返回的句柄
	 * @param	hbaseKeyInfo* hb_info	- [in] 
	 *			hb_info指定读出数据对应的目标表信息，包括目标表名称，目标行名称，目标表族名称，目标表限定名称，以及版本号
	 * @param	void **value	- [out] 
	 *			返回动态开辟的存放读入数据的内存地址
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI long ibase_get(table_handler handler,hbaseKeyInfo* hb_info, void **value, int* errnum);
	typedef long (*Proc_ibase_get)(table_handler handler,hbaseKeyInfo* hb_info, void **value, int* errnum);

	/** 
	 * @brief 	ibase_get_free
	 *			删除ibase_get分配的用来存放读入数据的内存空间
	 * @return	No return value.
	 * @param	void* value	- [in] 
	 *			调用ibase_get所获得的内存地址
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI void ibase_get_free(void* value, int* errnum);
	typedef void (*Proc_ibase_get_free)(void* value, int* errnum);

	/** 
	 * @brief 	ibase_uninit
	 *			逆初始化的最后一个函数
	 * @return	bool 	- Return true in success, otherwise return false
	 * @param	base_handler handler	- [in] 
	 *			ibase_init返回的句柄
	 * @param	int* errnum					- [out]
	 *			该操作失败的错误码
	 */
	ICFAPI bool ibase_uninit(base_handler handler, int* errnum);
	typedef bool (*Proc_ibase_uninit)(base_handler handler, int* errnum);

#ifdef __cplusplus
}
#endif

#endif

