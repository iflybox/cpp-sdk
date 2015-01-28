/** 
 * @file	idsslib.h
 * @brief	分布式文件系统客户端接口库加载器
 * 
 * 实现了加载iFlyDSS.dll的加载
 * 
 * @author	jzwu
 * @version	1.0
 * @date	2011-5-5
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2011-5-5	<td>jzwu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __IDSSLIB_H__
#define __IDSSLIB_H__

#include "utils/sulib.h"
#include "../iFlyDSS.h"

// Sun OS pack
#ifndef WIN32
#pragma pack(push)
#pragma pack(8)
#endif // sun

namespace sp{

// default idss_lib name
const char idss_lib_name[] = "iFlyDSS"
#ifdef _DEBUG
		"d"
#endif /* _DEBUG */
		;
	
/** 
 * @class	idss_lib
 * @brief	加载iFlyDSS.dll库
 * @author	jzwu
 * @date	2011/05/05
 * @see		
 */
class idss_lib : public sp::su_lib<sp::loader, char>
{
public:
	idss_lib(const char * lib = idss_lib_name)
		: sp::su_lib<sp::loader, char>(lib)
	{
	}

	virtual int load_procs(void)
	{
		su_proc_load(ds_initialize)
		su_proc_load(ds_connect)
		su_proc_load(ds_disconnect)
		su_proc_load(ds_fopen)
		su_proc_load(ds_fclose)
		su_proc_load(ds_read)
		su_proc_load(ds_write)
		su_proc_load(ds_flush)
		su_proc_load(ds_seek)
		su_proc_load(ds_tell)
		su_proc_load(ds_create_dir)
		su_proc_load(ds_exists)
		su_proc_load(ds_delete)
		su_proc_load(ds_rename)
		su_proc_load(ds_getworkingdir)
		su_proc_load(ds_setworkingdir)
		su_proc_load(ds_list_dir)
		su_proc_load(ds_path_info)
		su_proc_load(ds_free_file_info)
		su_proc_load(ds_find_change_notification)
		su_proc_load(ds_wait)
		su_proc_load(ds_copyfile)
		su_proc_load(ds_movefile)
		su_proc_load(ds_from_local)
		su_proc_load(ds_to_local)
		su_proc_load(ds_get_hosts)
		su_proc_load(ds_free_hosts)
		su_proc_load(ds_uninitialize)
		su_proc_load(ds_get_dsproxy)
		su_proc_load(ds_get_dsproxies)
		return 0;
	}

private:
	su_proc_decl(ds_initialize)
	su_proc_decl(ds_connect)
	su_proc_decl(ds_disconnect)
	su_proc_decl(ds_fopen)
	su_proc_decl(ds_fclose)
	su_proc_decl(ds_read)
	su_proc_decl(ds_write)
	su_proc_decl(ds_flush)
	su_proc_decl(ds_seek)
	su_proc_decl(ds_tell)
	su_proc_decl(ds_create_dir)
	su_proc_decl(ds_exists)
	su_proc_decl(ds_delete)
	su_proc_decl(ds_rename)
	su_proc_decl(ds_getworkingdir)
	su_proc_decl(ds_setworkingdir)
	su_proc_decl(ds_list_dir)
	su_proc_decl(ds_path_info)
	su_proc_decl(ds_free_file_info)
	su_proc_decl(ds_find_change_notification)
	su_proc_decl(ds_wait)
	su_proc_decl(ds_copyfile)
	su_proc_decl(ds_movefile)
	su_proc_decl(ds_from_local)
	su_proc_decl(ds_to_local)
	su_proc_decl(ds_get_hosts)
	su_proc_decl(ds_free_hosts)
	su_proc_decl(ds_uninitialize)
	su_proc_decl(ds_get_dsproxy)
	su_proc_decl(ds_get_dsproxies)
};

} /* namespace sp */

// Sun OS pack
#ifndef WIN32
#pragma pack(pop)
#endif // sun


#endif /* __IDSSLIB_H__ */