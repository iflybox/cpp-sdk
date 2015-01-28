/**
 * @file    libdatahub.h
 * @brief
 *          数据转发(同步)客户端接口
 * @author  lcdeng
 * @version 1.0
 * @date    2012.4.18
 *
 * @see
 *
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2012.4.18	<td>lcdeng	<td>Create this file</tr>
 * </table>
 * 
 */


#if defined(WIN32)
# if defined(DH_EXPORTS)
#  define DHAPI __declspec(dllexport)
# elif defined(DH_STATIC)
#  define DHAPI
#else
#  define DHAPI __declspec(dllimport)
# endif
#else /* defined(WIN32) */
# define DHAPI
#endif /* defined(WIN32) */


#ifdef __cplusplus
extern "C"
{
#endif

/** 
 * @brief 	libdatahub_init
 *			初始化函数
 * @return	执行成功返回0，失败返回错误代码
 * @see		
 */
DHAPI int libdatahub_init();
typedef int (*Proc_libdatahub_init)();

/** 
 * @brief 	libdatahub_fini
 *			逆初始化函数
 * @return	无返回值，调用后接口不可用
 * @see		
 */
DHAPI void libdatahub_fini();
typedef void (*Proc_libdatahub_fini)();


/** 
 * @brief 	libdatahub_sync
 *			执行文件同步的函数
 * @para    IN  path  --需要同步的文件路径
 * @return	执行成功返回0，失败返回错误代码
 * @see		
 */
DHAPI int libdatahub_sync(const char * path);
typedef int (*Proc_libdatahub_sync)(const char * path);


#ifdef __cplusplus
}
#endif