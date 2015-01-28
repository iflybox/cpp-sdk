/** 
* @file	cachelib
* @brief	
* 
*  detail...
* 
* @author	zhli2
* @version	1.0
* @date	2008/5/22
* 
* @see		
* 
* <b>History:</b><br>
* <table>
*  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
*  <tr> <td>1.0		<td>2008/5/22	<td>zhli2	<td>Create this file</tr>
* </table>
* 
*/
#ifndef __CACHE_LIB_H__
#define __CACHE_LIB_H__

#include "utils/sulib.h"
#include "icache.h"

namespace ic{

// default cachelib name
const char cachelib_name[] = "ifly_cache"
	#ifdef _DEBUG
	"d"
	#endif /* _DEBUG */
	;

/** 
 * @class	cachelib
 * 
 * @brief	Cache动态库加载器
 * 
 *  完成加载卸载Cache动态库，并定位其API地址
 * 
 * @author	zhli2
 * @date	2007-7-16
 * 
 * @see		
 */
class cache_lib
	: public sp::su_lib<sp::loader, char>
	{
public:
	cache_lib(const char * lib = cachelib_name)
		: sp::su_lib<sp::loader, char>(lib)
		{
		}

protected:
	virtual int load_procs(void)
		{
		su_proc_load(IFLYCacheInit)
		su_proc_load(IFLYCacheFini)
		su_proc_load(IFLYCacheOpenUrl)
		su_proc_load(IFLYCacheAddUrl)
		su_proc_load(IFLYCacheFetch)
		su_proc_load(IFLYCacheCloseUrl)
		su_proc_load(IFLYCacheQuery)
		return 0;
		}
	private:
		su_proc_decl(IFLYCacheInit)
		su_proc_decl(IFLYCacheFini)
		su_proc_decl(IFLYCacheOpenUrl)
		su_proc_decl(IFLYCacheAddUrl)
		su_proc_decl(IFLYCacheFetch)
		su_proc_decl(IFLYCacheCloseUrl)
		su_proc_decl(IFLYCacheQuery)
	};
} /* namespace ic */

#endif /* __IFlYCACHE_H__ */
