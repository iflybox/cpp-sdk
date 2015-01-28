/**
 * @file    libdatahub.h
 * @brief
 *          ����ת��(ͬ��)�ͻ��˽ӿ�
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
 *			��ʼ������
 * @return	ִ�гɹ�����0��ʧ�ܷ��ش������
 * @see		
 */
DHAPI int libdatahub_init();
typedef int (*Proc_libdatahub_init)();

/** 
 * @brief 	libdatahub_fini
 *			���ʼ������
 * @return	�޷���ֵ�����ú�ӿڲ�����
 * @see		
 */
DHAPI void libdatahub_fini();
typedef void (*Proc_libdatahub_fini)();


/** 
 * @brief 	libdatahub_sync
 *			ִ���ļ�ͬ���ĺ���
 * @para    IN  path  --��Ҫͬ�����ļ�·��
 * @return	ִ�гɹ�����0��ʧ�ܷ��ش������
 * @see		
 */
DHAPI int libdatahub_sync(const char * path);
typedef int (*Proc_libdatahub_sync)(const char * path);


#ifdef __cplusplus
}
#endif