/** 
 * @file	iFlyCFDef.h
 * @brief	Ѷ���Ƽ��������Cloud Foundation������
 * 
 * Ϊ�����β�ͬ����ϵͳ�汾���춨���һЩ����
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
 *  <tr> <td>2.0		<td>2011-4-8	<td>jingzhang	<td>Second Edition</tr>
 * </table>
 * 
 */

#ifndef __IFLYCFDEF_H__
#define __IFLYCFDEF_H__

#if defined(WIN32)
# if defined(DS_EXPORTS)
#  define DSAPI __declspec(dllexport)
#  define ICFAPI __declspec(dllexport)
# elif defined(DS_STATIC)
#  define DSAPI
#  define ICFAPI
#else
#  define DSAPI __declspec(dllimport)
#  define ICFAPI __declspec(dllexport)
# endif
#else /* defined(WIN32) */
# define DSAPI
# define ICFAPI
#endif /* defined(WIN32) */

#ifdef __cplusplus
extern  "C" {
#endif

#if _MSC_VER >= 1400
	typedef signed   __int64 int64;
	typedef unsigned __int64 uint64;
#else
	typedef long long          int64;
	typedef unsigned long long uint64;
#endif

	typedef void*		cf_handle;
	typedef cf_handle	ds_handle;
	typedef cf_handle	dc_handle;

#ifdef _WIN32
#if defined	_M_X64
	#define cf_invalid_handle	(int64)(-1)
	#define ptr_to_uint(x)		(unsigned long long)((void*)(x))
	#define cf_max_size_t		0xffffffffffffffff
	#define ds_localfs_handle	(int64)(0x108)
	typedef unsigned long long  ptr_uint_t;
#else
	#define cf_invalid_handle	(int)(-1)
	#define ptr_to_uint(x)		(unsigned)((void*)(x))
	#define cf_max_size_t		0xffffffff
	#define ds_localfs_handle	(int)(0x108)
	typedef unsigned			ptr_uint_t;
#endif

#else // for Linux, we only support 64bit
	
	#define cf_invalid_handle	(int64)(-1)
	#define ptr_to_uint(x)		(unsigned long long)((void*)(x))
	#define cf_max_size_t		0xffffffffffffffff
	#define ds_localfs_handle	(int64)(0x108)
	typedef unsigned long long  ptr_uint_t;

#endif

	#define ds_invalid_handle cf_invalid_handle

#ifdef __cplusplus

}
#endif

#endif /* __IFLYDSSDEF_H__ */
