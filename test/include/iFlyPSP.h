/** 
 * @file	iFlyPSP.h
 * @brief	讯飞个性化存储接口API
 * 
 * 本文件声明了讯飞个性化存储接口的API函数原型
 * 给出了各API函数语义参数和返回值类型和取值范围
 *
 * @author	danzhang
 * @version	1.0
 * @date	2012-04-13
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2010-12-9	<td>danzhang	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __IFLYPSP_H__
#define __IFLYPSP_H__

#include "TOL.h"
#include "hdfsext.h"

#if defined(WIN32)
# if defined(PSP_EXPORTS)
#  define PSPAPI __declspec(dllexport)
# elif defined(PSP_STATIC)
#  define PSPAPI
#else
#  define PSPAPI __declspec(dllimport)
# endif
#else /* defined(WIN32) */
# define PSPAPI
#endif /* defined(WIN32) */

#define PSP_MAX_PATH					MAX_PATH
#define PSP_USRID_LENGTH				64
#define PSP_APPID_LENGTH				32
#define PSP_DEVID_LENGTH				256
//
///* 服务类型 */
//const char* psp_svctype_mss				= "mss";
//const char* psp_svctype_ats				= "ats";
//const char* psp_svctype_nli				= "nli";
//
///* 用户个性化数据类型 */
//const char* psp_user_contact_org		= "contact_org";
//const char* psp_user_contact_nlp		= "contact_nlp";
//const char* psp_user_contact_hwb		= "contact_hwb";
//const char* psp_user_userword_org		= "userword_org";
//const char* psp_user_userword_hwb		= "userword_hwb";

	/* 文件信息 */
	typedef struct psp_file_info_t
	{
		char	filename[PSP_MAX_PATH];		/* 文件名			*/
		time_t	lastmodified;				/* 最后修改时间sec	*/
		uint64	size;						/* 文件长度 byte	*/
	} psp_file_info;

	/* 配额信息 */
	typedef struct psp_quota_info_t
	{
		char	dirpath[PSP_MAX_PATH];		/* 对应的idss目录	*/
		size_t	maxsize;					/* 分配的总存储空间 */
		size_t	usedsize;					/* 已用空间			*/
	} psp_quota_info;

	/* 个性化用户信息 */
	typedef struct psp_user_info_t
	{
		char usrid[PSP_USRID_LENGTH];
		char appid[PSP_APPID_LENGTH];
		char devid[PSP_DEVID_LENGTH];
	} psp_user_info;

#ifdef __cplusplus
	extern  "C" {
#endif

	/** 
	 * @brief 	PSP_DATA_MIGRATE_CB
	 *			用户数据迁移异步方式下的回调函数,用于通知用户迁移是否成功
	 * @return	int	- 成功返回0，否则返回失败代码
	 * @param	const char* src_uid	- [in]
	 *          迁移前用户id
	 * @param	const char* dst_uid	- [in]
	 *          迁移后用户id
	 * @param	int result	- [in]
	 *          标示迁移是否成功，0时成功，否则失败
	 * @see		
	 */
	typedef int (*PSP_DATA_MIGRATE_CB)(void* cbdata, const char* src_uid, const char* dst_uid, int result);

	/** 
	 * @brief 	psp_initialize
	 *			初始化系统的第一个函数，仅需要调用一次
	 * @return	int	- 成功返回0，否则返回失败代码
	 * @param	void* reserv	- [in]
	 *          保留以后使用
	 * @see		
	 */
	PSPAPI int psp_initialize(void* reserv);
	typedef int (*Proc_psp_initialize)(void* reserv);

	/** 
	 * @brief 	psp_uninitialize
	 *			逆初始化的最后一个函数
	 * @return	无返回值，调用后不再可用.
	 * @see		
	 */
	PSPAPI void psp_uninitialize(void);
	typedef void (*Proc_psp_uninitialize)(void);

	/** 
	 * @brief 	psp_read_comdata
	 *			读取ds(存储系统)上某一业务文件到缓冲区
	 *			如获取ats所需的wfst.SMS20110728.20111225.wbi2011.bin到缓冲区
	 * @return	int	- Return 0 in success, otherwise return error code.
	 *			若数据超过最大长度，返回对应错误码
	 * @param	const char* svctype		- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds文件名，如wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	void* outbuf			- [out] 
	 *			读取缓冲区
	 * @param	size_t* bytes			- [in/out]
	 *			缓冲区长度,传出读取到缓冲区中的数据长度 
	 */
	PSPAPI int psp_read_comdata(const char* svctype, const char* dsfile, void* outbuf, size_t* bytes);
	typedef int (*Proc_psp_read_comdata)(const char* svctype, const char* dsfile, void* outbuf, size_t* bytes);

	/** 
	 * @brief 	psp_write_comdata
	 *			存放缓冲区中数据到ds上某业务下，如存放到ats下
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds文件名，如wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	const void* inbuf		- [in] 
	 *			准备写入的内容缓存
	 * @param	size_t bytes			- [in]
	 *			缓冲区长度
	 */
	PSPAPI int psp_write_comdata(const char* svctype, const char* dsfile, const void* inbuf, size_t bytes);
	typedef int (*Proc_psp_write_comdata)(const char* svctype, const char* dsfile, const void* inbuf, size_t bytes);

	/** 
	 * @brief 	psp_readcomdata_lock
	 *			读取ds上某一业务文件到缓冲区,函数内部调用特定锁功能对被访问文件加锁
	 *			如获取ats所需的wfst.SMS20110728.20111225.wbi2011.bin到缓冲区
	 * @return	int	- Return 0 in success, otherwise return error code.
	 *			若数据超过最大长度则失败，返回对应错误码
	 * @param	const char* svctype		- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds文件名，如wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	void* outbuf			- [out] 
	 *			读取缓冲区
	 * @param	size_t* bytes			- [in/out]
	 *			缓冲区长度，传出读取到缓冲区中的数据长度 
	 */
	PSPAPI int psp_readcomdata_lock(const char* svctype, const char* dsfile, void* outbuf, size_t* bytes);
	typedef int (*Proc_psp_readcomdata_lock)(const char* svctype, const char* dsfile, void* outbuf, size_t* bytes);

	/** 
	 * @brief 	psp_writecomdata_lock
	 *			存放缓冲区中数据到ds上某业务下，函数内部调用特定锁功能对被访问文件加锁
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	const void* dsfile		- [in] 
	 *			ds文件名，如wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	const void* inbuf		- [in] 
	 *			准备写入的内容缓存
	 * @param	size_t bytes			- [in]
	 *			缓冲区长度
	 */
	PSPAPI int psp_writecomdata_lock(const char* svctype, const char* dsfile, const void* inbuf, size_t bytes);
	typedef int (*Proc_psp_writecomdata_lock)(const char* svctype, const char* dsfile, const void* inbuf, size_t bytes);

	/** 
	 * @brief 	psp_read_userdata
	 *			读取ds上用户数据到缓冲区，如获取用户a01003395的/wav/00000001.wav文件到缓冲区
	 * @return	int	- Return 0 in success, otherwise return error code.
	 *			若数据超过最大长度则失败，返回对应错误码
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id
	 *			路径生成顺序为uid/appid/devid,其中用户id和应用id至少有一个不为空
	 * @param	const char* dsfile				- [in] 
	 *			ds文件名,如/wav/00000001.wav
	 * @param	void* outbuf					- [out] 
	 *			读取缓冲区
	 * @param	size_t* bytes					- [in/out]
	 *			缓冲区长度，传出读取到缓冲区中的数据长度 
	 */
	PSPAPI int psp_read_userdata(const psp_user_info* userinfo, const char* dsfile, void* outbuf, size_t* bytes);
	typedef int (*Proc_psp_read_userdata)(const psp_user_info* userinfo, const char* dsfile, void* outbuf, size_t* bytes);

	/** 
	 * @brief 	psp_write_userdata
	 *			存放缓冲区中的数据到ds上对应用户下，如存放到用户a01003395下的/wav/00000001.wav文件
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id
	 *			路径生成顺序为uid/appid/devid,其中用户id和应用id至少有一个不为空
	 * @param	const char* dsfile				- [in] 
	 *			ds文件名,如/wav/00000001.wav
	 * @param	const void* inbuf				- [in] 
	 *			准备写入的内容缓存
	 * @param	size_t bytes					- [in]
	 *			缓冲区长度
	 * @param	bool append						- [in]
	 *			是否以追加方式写文件
	 */
	PSPAPI int psp_write_userdata(const psp_user_info* userinfo, const char* dsfile, const void* inbuf, size_t bytes, bool append);
	typedef int (*Proc_psp_write_userdata)(const psp_user_info* userinfo, const char* dsfile, const void* inbuf, size_t bytes, bool append);

	/** 
	 * @brief 	psp_read_persondata
	 *			读取ds上用户的个性化数据到缓冲区，如获取用户a01003395的联系人词表源文件到缓冲区
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id
	 *			路径生成顺序为uid/devid/appid,其中用户id和应用id至少有一个不为空
	 * @param	const void* datatype			- [in] 
	 *			文件的类型，如PSP_USER_CONTACT_ORG
	 * @param	char* outbuf					- [out] 
	 *			读取缓冲区
	 * @param	size_t* bytes					- [in/out]
	 *			缓冲区长度，传出读取到缓冲区中的数据长度
	 */
	PSPAPI int psp_read_persondata(const psp_user_info* userinfo, const char* datatype, void* outbuf, size_t* bytes);
	typedef int (*Proc_psp_read_persondata)(const psp_user_info* userinfo, const char* datatype, void* outbuf, size_t* bytes);

	/** 
	 * @brief 	psp_write_persondata
	 *			存放缓冲区中用户的个性化数据到ds，如将缓冲区中用户a01003395的联系人词表源文件数据存放到ds
	 * @return	int	- Return 0 in success, otherwise return error code.
	 *			若数据超过最大长度则失败，返回对应错误码
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id
	 *			路径生成顺序为uid/appid/devid,其中用户id和应用id至少有一个不为空
	 * @param	const char* datatype			- [in] 
	 *			文件的类型，如PSP_USER_CONTACT_ORG
	 * @param	const void* inbuf				- [in] 
	 *			准备写入的内容缓存
	 * @param	size_t bytes					- [in]
	 *			缓冲区长度
	 */
	PSPAPI int psp_write_persondata(const psp_user_info* userinfo, const char* datatype, const void* outbuf, size_t bytes);
	typedef int (*Proc_psp_write_persondata)(const psp_user_info* userinfo, const char* datatype, const void* outbuf, size_t bytes);

	/** 
	 * @brief 	psp_list_comfile
	 *			获取ds上业务文件列表信息，如获取ds上存放的ats的文件列表
	 * @return	psp_file_info 返回文件信息的数组，失败返回NULL
	 *			该数组由系统动态分配，用户需要调用释放函数释放
	 * @param	const char* svctype		- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	int* filecount			- [out] 
	 *			psp_file_info长度
	 * @param	int* errnum				- [out] 
	 *			错误码，0表示成功
	 */
	PSPAPI psp_file_info* psp_list_comfile(const char* svctype, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_comfile)(const char* svctype, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_list_userfile
	 *			获取ds上某用户文件列表信息，不包括备份文件 
	 * @return	psp_file_info 返回文件信息的数组，失败返回NULL
	 *			该数组由系统动态分配，用户需要调用释放函数释放
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id，其中用户id和应用id至少有一个不为空
	 * @param	int* filecount			- [out] 
	 *			psp_file_info长度
	 * @param	int* errnum				- [out] 
	 *			错误码，0表示成功
	 */
	PSPAPI psp_file_info* psp_list_userfile(const psp_user_info* userinfo, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_userfile)(const psp_user_info* userinfo, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_list_userbakfile
	 *			获取ds上某用户备份文件列表信息
	 * @return	psp_file_info 返回文件信息的数组，失败返回NULL
	 *			该数组由系统动态分配，用户需要调用释放函数释放
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id，其中用户id和应用id至少有一个不为空
	 * @param	int* filecount					- [out] 
	 *			psp_file_info长度
	 * @param	int* errnum						- [out] 
	 *			错误码，0表示成功
	 */
	PSPAPI psp_file_info* psp_list_userbakfile(const psp_user_info* userinfo, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_userbakfile)(const psp_user_info* userinfo, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_list_personfile
	 *			获取用户个性化数据文件列表信息，不包括备份文件，如获取用户a01003395的热词源文件信息
	 * @return	psp_file_info 返回文件信息的数组，失败返回NULL
	 *			该数组由系统动态分配，用户需要调用释放函数释放
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id，其中用户id和应用id至少有一个不为空
	 * @param	const char* datatype			- [in] 
	 *			个性化数据类型，如PSP_USER_USRWORD_ORG
	 * @param	int* filecount					- [out] 
	 *			psp_file_info长度
	 * @param	int* errnum						- [out] 
	 *			错误码，0表示成功
	 */
	PSPAPI psp_file_info* psp_list_personfile(const psp_user_info* userinfo, const char* datatype, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_personfile)(const psp_user_info* userinfo, const char* datatype, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_list_personbakfile
	 *			获取用户个性化数据的备份文件列表信息,如获取用户a01003395的热词源文件的备份文件列表
	 * @return	psp_file_info 返回文件信息的数组，失败返回NULL
	 *			该数组由系统动态分配，用户需要调用释放函数释放
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id，其中用户id和应用id至少有一个不为空
	 * @param	const char* datatype			- [in] 
	 *			个性化数据类型，如PSP_USER_CONTACT_ORG
	 * @param	int* filecount					- [out] 
	 *			psp_file_info长度
	 * @param	int* errnum						- [out] 
	 *			错误码，0表示成功
	 */
	PSPAPI psp_file_info* psp_list_personbakfile(const psp_user_info* userinfo, const char* datatype, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_personbakfile)(const psp_user_info* userinfo, const char* datatype, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_free_file_info
	 *			释放psp_file_info信息
	 * @return	No return value.
	 * @param	const psp_file_info *fileinfo	- [in] 
	 *			psp_file_info的数组
	 */
	PSPAPI void psp_free_file_info(const psp_file_info *fileinfo);
	typedef void (*Proc_psp_free_file_info)(const psp_file_info *fileinfo);

	/** 
	 * @brief 	psp_com_quotainfo
	 *			获取业务配额信息
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype			- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	psp_quota_info* quotinfo	- [in/out] 
	 *			配额信息
	 */
	PSPAPI int psp_com_quotainfo(const char* svctype, psp_quota_info* quotinfo);
	typedef int (*Proc_psp_com_quotainfo)(const char* svctype, psp_quota_info* quotinfo);

	/** 
	 * @brief 	psp_user_quotainfo
	 *			获取用户配额信息
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* uid		- [in] 
	 *			用户id 
	 * @param	psp_quota_info* quotinfo	- [in/out] 
	 *			配额信息
	 */
	PSPAPI int psp_user_quotainfo(const char* uid, psp_quota_info* quotinfo);
	typedef int (*Proc_psp_user_quotainfo)(const char* uid, psp_quota_info* quotinfo);

	/** 
	 * @brief 	psp_migrate_userdata
	 *			非注册用户到注册用户的数据迁移 
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* src_uid		- [in] 
	 *			注册前用户id
	 * @param	const char* dst_uid		- [in] 
	 *			注册后用户id
	 * @param	PSP_DATA_MIGRATE fn		- [in] 
	 *			用户数据迁移异步方式下的回调函数
	 */
	PSPAPI int psp_migrate_userdata(const char* src_uid, const char* dst_uid, PSP_DATA_MIGRATE_CB fn, void* cbdata);
	typedef int (*Proc_psp_migrate_userdata)(const char* src_uid, const char* dst_uid, PSP_DATA_MIGRATE_CB fn, void* cbdata);

	/** 
	 * @brief 	psp_get_comfile
	 *			获取ds(存储系统)上某一业务文件到本地
	 *			如复制ats所需的wfst.SMS20110728.20111225.wbi2011.bin到本地文件
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds文件名，如wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	const char* localfile	- [in] 
	 *			本地路径
	 */
	PSPAPI int psp_get_comfile(const char* svctype, const char* dsfile, const char* localfile);
	typedef int (*Proc_psp_get_comfile)(const char* svctype, const char* dsfile, const char* localfile);

	/** 
	 * @brief 	psp_put_comfile
	 *			存放本地文件到ds上某业务下
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	const char* localfile	- [in] 
	 *			本地文件路径
	 * @param	const char* dsfile		- [in] 
	 *			ds文件名，如wfst.SMS20110728.20111225.wbi2011.bin
	 */
	PSPAPI int psp_put_comfile(const char* svctype, const char* localfile, const char* dsfile);
	typedef int (*Proc_psp_put_comfile)(const char* svctype, const char* localfile, const char* dsfile);

	/** 
	 * @brief 	psp_getcomfile_lock
	 *			获取ds上某一业务文件到本地,函数内部调用特定锁功能对被访问文件加锁
	 *			如复制ats所需的wfst.SMS20110728.20111225.wbi2011.bin到本地文件
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds文件名，如wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	const char* localfile	- [in] 
	 *			本地路径
	 */
	PSPAPI int psp_getcomfile_lock(const char* svctype, const char* dsfile, const char* localfile);
	typedef int (*Proc_psp_getcomfile_lock)(const char* svctype, const char* dsfile, const char* localfile);

	/** 
	 * @brief 	psp_putcomfile_lock
	 *			存放本地文件到ds上某业务下，函数内部调用特定锁功能对被访问文件加锁
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			业务类型，如PSP_SVCTYPE_ATS
	 * @param	const char* localfile	- [in] 
	 *			本地文件路径
	 * @param	const char* dsfile		- [in] 
	 *			ds文件名，如wfst.SMS20110728.20111225.wbi2011.bin
	 */
	PSPAPI int psp_putcomfile_lock(const char* svctype, const char* localfile, const char* dsfile);
	typedef int (*Proc_psp_putcomfile_lock)(const char* svctype, const char* localfile, const char* dsfile);

	/** 
	 * @brief 	psp_get_userfile
	 *			获取ds上用户文件到本地，如复制用户a01003395的/wav/00000001.wav文件到本地
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id
	 *			路径生成顺序为uid/appid/devid,其中用户id和应用id至少有一个不为空
	 * @param	const char* dsfile				- [in] 
	 *			ds文件名,如/wav/00000001.wav
	 * @param	const char* localfile			- [in] 
	 *			本地路径
	 */
	PSPAPI int psp_get_userfile(const psp_user_info* userinfo, const char* dsfile, const char* localfile);
	typedef int (*Proc_psp_get_userfile)(const psp_user_info* userinfo, const char* dsfile, const char* localfile);

	/** 
	 * @brief 	psp_put_userfile
	 *			存放本地文件到ds上对应用户下，如复制到用户a01003395下的/wav/00000001.wav文件
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id
	 *			路径生成顺序为uid/appid/devid,其中用户id和应用id至少有一个不为空
	 * @param	const char* localfile			- [in] 
	 *			本地路径
	 * @param	const char* dsfile				- [in] 
	 *			ds文件名,如/wav/00000001.wav
	 */
	PSPAPI int psp_put_userfile(const psp_user_info* userinfo, const char* localfile, const char* dsfile);
	typedef int (*Proc_psp_put_userfile)(const psp_user_info* userinfo, const char* localfile, const char* dsfile);

	/** 
	 * @brief 	psp_get_personfile
	 *			获取ds上用户的个性化数据到本地文件 ，如获取用户a01003395的联系人词表源文件到本地文件
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id
	 *			路径生成顺序为uid/appid/devid,其中用户id和应用id至少有一个不为空
	 * @param	const char* datatype			- [in] 
	 *			文件的类型，如PSP_USER_CONTACT_ORG
	 * @param	const char* localfile			- [in] 
	 *			本地路径
	 */
	PSPAPI int psp_get_personfile(const psp_user_info* userinfo, const char* datatype, const char* localfile);
	typedef int (*Proc_psp_get_personfile)(const psp_user_info* userinfo, const char* datatype, const char* localfile);

	/** 
	 * @brief 	psp_put_personfile
	 *			将用户的个性化数据从本地复制到ds，如将用户a01003395的联系人词表源文件复制到ds
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			用户信息，包括用户id、应用id、设备id
	 *			路径生成顺序为uid/appid/devid,其中用户id和应用id至少有一个不为空
	 * @param	const char* datatype			- [in] 
	 *			文件的类型，如PSP_USER_CONTACT_ORG
	 * @param	const char* localfile			- [in] 
	 *			本地路径
	 */
	PSPAPI int psp_put_personfile(const psp_user_info* userinfo, const char* datatype, const char* localfile);
	typedef int (*Proc_psp_put_personfile)(const psp_user_info* userinfo, const char* datatype, const char* localfile);

#ifdef __cplusplus

}
#endif
#endif	 //__IFLYPSP_H__