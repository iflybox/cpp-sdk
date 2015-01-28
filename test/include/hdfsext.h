/** 
 * @file	hdfsext.h
 * @brief	HDFS��չ�ӿ�
 * 
 * ���ļ���������չ��HDFS�ӿ�
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
	 *          ���ص�ǰ������룬�̰߳�ȫ
	 * @return	DSAPI int	- Return 0 in success, otherwise return error code.
	 */
	DSAPI int hdfs_lasterror(void);
	typedef int (*Proc_hdfs_lasterror)(void);

	/** 
	 * @brief 	hdfs_initialize_ex
	 *			��ʼ��ϵͳ�ĵ�һ������������Ҫ����һ��
	 * @return	DSAPI int	- �ɹ�����0�����򷵻�ʧ�ܴ���
	 * @param	int enable_dcache	- [in] - �Ƿ�ʹ�÷ֲ�ʽcache
	 * @param	DS_EVENT_HOOK fn	- [in] - ��������ϵͳ�¼���Callback����
	 * @param	void* reserv	- [in]
	 *          �����Ժ�ʹ��
	 * @see		
	 */
	ICFAPI int hdfs_initialize_ex(HDFS_EVENT_HOOK fn, void* cbdata, void* reserv);
	typedef int (*Proc_hdfs_initialize_ex)(HDFS_EVENT_HOOK fn, void* cbdata, void* reserv);

	/** 
	 * @brief 	hdfs_uninitialize
	 *			���ʼ�������һ������
	 * @return	�޷���ֵ�����ú��ڿ���.
	 * @see		
	 */
	ICFAPI void hdfs_uninitialize(void);
	typedef void (*Proc_hdfs_uninitialize)(void);
	
	/** 
	 * @brief 	hdfs_connect
	 *          ���ӷֲ�ʽ�洢ϵͳ
	 * @return	hdfs_handle ����ɹ����ش洢ϵͳ��������ʧ�ܷ���hdfs_invalid_handle��errnoָ��ʧ��ԭ��
	 * @param	const char* dsURL	- [in] �ֲ�ʽ�洢URL
	 *			dsURLSӦ����������ʽ scheme://host:port/dir
	 *				scheme -Э�飬����Э����hdfs��Hadoop�ֲ�ʽ�ļ�ϵͳ��
	 *						���������û����������Ŀ¼��grammar
	 *				dir    -��Ŀ¼�������Ŀ¼���ڵ�ǰĿ¼����Ϊ��Ŀ¼
	 *                      ��������Ϊ��Ŀ¼����������Ŀ¼��Ŀ¼����ָӳ��
	 *                      ��������Ŀ¼��DSSĿ¼
	 * @param	const char* uid	- [in]
	 *          ָ��һ�����Ա����û���ݵ�id
	 * @param	void* reserv	- [in]
	 *          �����Ժ�ʹ��
	 * @Comments: ��ʹ��dsproxyʱһ��������dsURL�е�host��port��uid��ͬ��ʶ��Ҳ����˵���
	 *            host��port��uid����ͬ����ú���Ӧ�÷�����ͬ�ľ��
	 *            ʹ��dsproxyʱ���ӳ���������ʶ�⻹Ӧ�ü���dsproxy�ĵ�ַ��Ҳ����˵�ͻ���ͨ��
	 *            ��ͬ��dsproxy��ͬһ�û�������ͬһ��dsURL�����ز�ͬ�ľ��
	 */
	ICFAPI hdfs_handle hdfs_connect(const char* dsURL, const char* uid, void* reserv);
	typedef hdfs_handle (*Proc_hdfs_connect)(const char* dsURL, const char* uid, void* reserv);

	/** 
	 * @brief 	hdfs_disconnect
	 *          �Ͽ��ֲ�ʽ�ļ�ϵͳ
	 * @return	int - Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *          hdfs_connect�������صľ�� 
	 */
	ICFAPI int hdfs_disconnect(hdfs_handle dsh);
	typedef int (*Proc_hdfs_disconnect)(hdfs_handle dsh);

	/** 
	 * @brief 	hdfs_fopen
	 *			���ļ�
	 * @return	HDFSFILE �ɹ������ļ���������ʧ�ܷ���NULL��errnoָ��ʧ��ԭ��
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			�򿪵��ļ�·����֧������·���ϵ��ļ�
	 * @param	int flags	- [in] 
	 *			�򿪷�ʽ	DS_RDONLYֻ����ʽ��
	 *					DS_WRONLYд��ʽ��
	 *					DS_APPEND׷�ӷ�ʽ��
	 * @param	int buffersize	- [in] range [0, 0x7FFFFFFF]
	 *			ָ�����ļ�ʱ�������Ĵ�С��0��ʹ��Ĭ��ֵ��4096
	 * @param	short replication	- [in] range [0, 0x7FFF]
	 *			ָ���ļ���������Ŀ��0��ʹ��Ĭ��ֵ��3
	 * @param	int blocksize	- [in] range [0, 0x7FFF]
	 *			ָ���ļ���Ĵ�С��0��ʹ��Ĭ��ֵ��64*1024*1024
	 * @Comments: buffersize�Ǵ򿪵����Ļ�������С����ֵ�����մ���java��BufferedInputStream��BufferedOutputStream
	 *            ������Ĺ��캯������Java SE��ʵ���������ָ�����ֵ����Ĭ��Ϊ8192�ֽ�
	 *            hadoop.configuration ��io.file.buffer.size���ʹ����ж���ĸ�ֵΪ4096.
	 */
	ICFAPI HDFSFILE hdfs_fopen(hdfs_handle dsh, const char* path, int flags,
		int buffersize, short replication, int blocksize);
	typedef HDFSFILE (*Proc_hdfs_fopen)(hdfs_handle dsh, const char* path, int flags,
		int buffersize, short replication, int blocksize);

	/** 
	 * @brief 	hdfs_fclose
	 *			�ر��ļ�
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	HDFSFILE file	- [in] 
	 *			׼���رյ��ļ�ָ��	
	 */
    ICFAPI int hdfs_fclose(hdfs_handle dsh, HDFSFILE file);
	typedef int (*Proc_hdfs_fclose)(hdfs_handle dsh, HDFSFILE file);

	/** 
	 * @brief 	hdfs_read
	 *			��ȡ�ļ�
	 * @return	int ������ʵ��ȡ���ֽ���������-1Ϊʧ�ܣ�errnoָ��ʧ��ԭ��
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	HDFSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	void* buffer	- [in] 
	 *			��ȡ������
	 * @param	int length	- [in] range:(0, 0x7FFFFFFF]
	 *			����������
	 */
    ICFAPI int hdfs_read(hdfs_handle dsh,  HDFSFILE file, void* buffer, int length);
	typedef int (*Proc_hdfs_read)(hdfs_handle dsh,  HDFSFILE file, void* buffer, int length);

	/** 
	 * @brief 	hdfs_write
	 *			д���ļ�
	 * @return	int ������ʵд����ֽ���������-1Ϊʧ�ܣ�errnoָ��ʧ��ԭ��
	 *			����0Ϊд�뻺���������������0�������Ե�Ƭ�̺��ٴ�д��
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	HDFSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	const void* buffer	- [in] 
	 *			׼��д������ݻ���
	 * @param	int length	- [in] 
	 *			׼��д������ݻ��泤��
	 */
    ICFAPI int hdfs_write(hdfs_handle dsh, HDFSFILE file, const void* buffer, int length);
	typedef int (*Proc_hdfs_write)(hdfs_handle dsh, HDFSFILE file, const void* buffer, int length);
	 
	/**
	 * @brief 	hdfs_flush
	 *			��д�뻺���е�����flush���洢ϵͳ��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	HDFSFILE file	- [in] 
	 *			�ļ�ָ��
	 */
    ICFAPI int hdfs_flush(hdfs_handle dsh, HDFSFILE file);
	typedef int (*Proc_hdfs_flush)(hdfs_handle dsh, HDFSFILE file);

	/** 
	 * @brief 	hdfs_seek
	 *			���ļ���ȡָ���Ƶ��ļ���ĳһλ��
	 *			�ú���ֻ��read-onlyģʽ����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in]
	 *			���ӵĴ洢ϵͳ���
	 * @param	HDFSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	size_t offset	- [in] 
	 *			λ�Ƴ���
	 */
    ICFAPI int hdfs_seek(hdfs_handle dsh, HDFSFILE file, size_t offset); 
	typedef int (*Proc_hdfs_seek)(hdfs_handle dsh, HDFSFILE file, size_t offset); 

	/** 
	 * @brief 	hdfs_tell
	 *			���ص�ǰ�ļ���offsetֵ����bytesΪ��λ
	 * @return	size_t ���ص�ǰ�ļ���offsetֵ������Ϊ-1��errnoָ������ԭ��
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	HDFSFILE file	- [in] 
	 *			�ļ�ָ��
	 */
    ICFAPI size_t hdfs_tell(hdfs_handle dsh, HDFSFILE file);
	typedef size_t (*Proc_hdfs_tell)(hdfs_handle dsh, HDFSFILE file);

	/** 
	 * @brief 	hdfs_create_dir
	 *			����Ŀ¼������Ŀ¼��
				hadoop�������֧�ֵ�Ŀ¼���Ϊ121��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			������[����]Ŀ¼·����֧�־���·�������·��	
	 * @param	const char* orignal_path	- [in] 
	 *			������Ŀ¼ʱ�ò�������NULL����������Ŀ¼ʱ�ò�����������Ŀ¼ӳ���ԴĿ¼	
	 */
    ICFAPI int hdfs_create_dir(hdfs_handle dsh, const char* path, const char* orignal_path);
	typedef int (*Proc_hdfs_create_dir)(hdfs_handle dsh, const char* path, const char* orignal_path);

	/** 
	 * @brief 	hdfs_exists
	 * 			�ж�һ���ļ�/Ŀ¼����������Ŀ¼���Ƿ����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char *path	- [in] 
	 *			��֤��·������
	 */
    ICFAPI int hdfs_exists(hdfs_handle dsh, const char *path);
	typedef int (*Proc_hdfs_exists)(hdfs_handle dsh, const char *path);

	/** 
	 * @brief 	hdfs_delete
	 *			ɾ��һ���ļ�/Ŀ¼����������Ŀ¼��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			ɾ����·������
	 * @param	int recursive	- [in] 
	 *			���ɾ��Ŀ¼�ò����ķ�0ֵָ���ݹ�ɾ��Ŀ¼�е�����
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			�Ƿ���cache
	 * @Comments: ���ɾ����path����ʵpath����ɾ��������ʵpath����ͨ����������У������path
	 *            ��һ��Ŀ¼���ж�Ӧ������Ŀ¼������������ʵĿ¼��ɾ�������Ӧ������Ŀ¼Ҳ��ɾ��
	 *            ���ɾ����path������path�����recursive=1��ɾ������path�Ͷ�Ӧ����ʵpath��
	 *                                     ���recursive=0��ֻɾ������Ŀ¼
	 */
    ICFAPI int hdfs_delete(hdfs_handle dsh, const char* path, int recursive);
	typedef int (*Proc_hdfs_delete)(hdfs_handle dsh, const char* path, int recursive);

	/** 
	 * @brief 	hdfs_rename
	 *			������һ���ļ���Ŀ¼����������Ŀ¼��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* old_path	- [in] 
	 *			ԭ·������
	 * @param	const char* new_path	- [in] 
	 *			��·������
	 */
    ICFAPI int hdfs_rename(hdfs_handle dsh, const char* old_path, const char* new_path);
	typedef int (*Proc_hdfs_rename)(hdfs_handle dsh, const char* old_path, const char* new_path);

	/** 
	 * @brief 	hdfs_setreplication
	 *			�����ļ��ĸ�����Ŀ ����������·����
				�ļ��ĸ�����Ŀ��ΧΪ��1��512
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			·������
	 * @param	short replication	- [in] 
	 *			�µĸ�����Ŀ
	 */
    ICFAPI int hdfs_setreplication(hdfs_handle dsh, const char* path, short replication);
	typedef int (*Proc_hdfs_setreplication)(hdfs_handle dsh, const char* path, short replication);

	/** 
	 * @brief 	hdfs_chown
	 *			�����ļ�����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in]
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in]
	 *			·������
	 * @param	const char* owner - [in]
	 *			�ļ������������NULL��ֻ����group
	 * @param	const char* group - [in] 
	 *			�ļ��飬�������NULL��ֻ����owner
	 */
    ICFAPI int hdfs_chown(hdfs_handle dsh, const char* path, const char *owner, const char *group);
	typedef int (*Proc_hdfs_chown)(hdfs_handle dsh, const char* path, const char *owner, const char *group);

	/** 
	 * @brief 	hdfs_chmod
	 *			�����ļ�Ȩ��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in]
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in]
	 *			·������
	 * @param	short mode - [in]
				�ļ�Ȩ��
				hadoop���ļ������û������������Ķ�дִ��Ȩ�ޣ���ʽΪrwx��xͳһ����Ϊ0���������ļ�Ϊ�û���д�������������Ȩ�ޣ���Ӧ����Ϊrw_r_____��
				�˴�����modeΪ��110100000����2���ƣ�����Ӧ��10����������416��
	 *			
	 */
    ICFAPI int hdfs_chmod(hdfs_handle dsh, const char* path, short mode);
	typedef int (*Proc_hdfs_chmod)(hdfs_handle dsh, const char* path, short mode);

	/** 
	 * @brief 	hdfs_utime
	 *			�����ļ���ʱ����Ϣ
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in]
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in]
	 *			·������
	 * @param	time_t mtime - [in]
	 *			�ļ����޸�ʱ�䣬�������0����ֻ���÷���ʱ��
	 * @param	time_t atime - [in]
	 *			�ļ��ķ���ʱ�䣬�������0����ֻ�����޸�ʱ��
	 */
	ICFAPI int hdfs_utime(hdfs_handle dsh, const char* path, time_t mtime, time_t atime);
	typedef int (*Proc_hdfs_utime)(hdfs_handle dsh, const char* path, time_t mtime, time_t atime);

	/** 
	 * @brief 	hdfs_getworkingdir
	 *			���ǰ����Ŀ¼
	 * @return	char* ����ɹ�����ָ��buffer��ָ�룬���򷵻�NULL��errnoָ������ԭ��
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	char *buffer	- [in] 
	 *			������ŵ�ǰĿ¼�Ļ���
	 * @param	size_t length	- [in] 
	 *			���泤��	
	 */
    ICFAPI char* hdfs_getworkingdir(hdfs_handle dsh, char *buffer, size_t length);
	typedef char* (*Proc_hdfs_getworkingdir)(hdfs_handle dsh, char *buffer, size_t length);

	/** 
	 * @brief 	hdfs_setworkingdir
	 *			���õ�ǰ����Ŀ¼
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			���õ�Ŀ¼·�������·��������ڵ�ǰ·������������·��ҲҪ����		
	 */
    ICFAPI int hdfs_setworkingdir(hdfs_handle dsh, const char* path);
	typedef int (*Proc_hdfs_setworkingdir)(hdfs_handle dsh, const char* path);

	/** 
	 * @brief 	*hdfs_list_dir
	 *			�г�Ŀ¼��������Ŀ¼���е��ļ���Ϣ
	 * @return	hdfs_file_info �����ļ���Ϣ�����飬ʧ�ܷ���NULL��errnoָ������ԭ��
	 *			��������ϵͳ��̬�����裬�û���Ҫ�����ͷź����ͷ�
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			Ŀ¼��������Ŀ¼��·��
	 * @param	int *num	- [in] 
	 *			hdfs_file_info����
	 */
    ICFAPI hdfs_file_info *hdfs_list_dir(hdfs_handle dsh, const char* path, int *num);
	typedef hdfs_file_info* (*Proc_hdfs_list_dir)(hdfs_handle dsh, const char* path, int *num);

	/** 
	 * @brief 	*hdfs_path_info
	 *			�г��ļ�/Ŀ¼��������Ŀ¼�������ļ���Ϣ
	 * @return	hdfs_file_info �����ļ���Ϣ��ʧ�ܷ���NULL��errnoָ������ԭ��
	 *			����Ϣ��ϵͳ��̬�����裬�û���Ҫ�����ͷź����ͷ�
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			Ŀ¼��������Ŀ¼��·��
	 */
    ICFAPI hdfs_file_info *hdfs_path_info(hdfs_handle dsh, const char* path);
	typedef hdfs_file_info* (*Proc_hdfs_path_info)(hdfs_handle dsh, const char* path);

	/** 
	 * @brief 	hdfs_free_file_info
	 *			�ͷ�hdfs_file_info��Ϣ
	 * @return	No return value.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	hdfs_file_info *fileinfo	- [in] 
	 *			hdfs_file_info������
	 * @param	int num	- [in] 	
	 *			����Ԫ�صĸ���
	 */
	ICFAPI void hdfs_free_file_info(hdfs_handle dsh, hdfs_file_info *fileinfo, int num);
	typedef void (*Proc_hdfs_free_file_info)(hdfs_handle dsh, hdfs_file_info *fileinfo, int num);

	/** 
	 * @brief 	hdfs_find_change_notification
	 *			�����ļ��������Ѽ���ļ��޸�
	 * @return	hdfs_handle �ɹ�����һ�����Ѿ����ʧ�ܷ���hdfs_invalid_handle��errnoָ��ʧ��ԭ��
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			��Ҫ���ӵ��ļ�·�������·��Ŀǰֻ֧���ļ�
	 * @param	int watchsubtree	- [in] 
	 *			�Ƿ������Ŀ¼���ļ����������Ŀǰֻ��Ϊ�˺�Windows API����һ��
	 * @param	int filter	- [in] 
	 *			��ع��������������Ŀǰֻ��Ϊ�˺�Windows API����һ��
	 */
	ICFAPI hdfs_handle hdfs_find_change_notification(hdfs_handle dsh, const char* path, int watchsubtree, int filter);
	typedef hdfs_handle (*Proc_hdfs_find_change_notification)(hdfs_handle dsh, const char* path, int watchsubtree, int filter);

	/** 
	 * @brief 	hdfs_wait
	 *			�ȴ����
	 * @return	DSAPI int	- Return 0 in success, -1��ʱ������Ϊ�������.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	hdfs_handle handle	- [in] 
	 *			Ҫ�ȴ��ľ��
	 * @param	time_t ms	- [in]
	 *			��ʱʱ��
	 * @see		
	 */
	ICFAPI int hdfs_wait(hdfs_handle dsh, hdfs_handle handle, time_t ms);
	typedef int (*Proc_hdfs_wait)(hdfs_handle dsh, hdfs_handle handle, time_t ms);

	/** 
	 * @brief 	hdfs_copyfile
	 *			�����ļ���֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle sdsh	- [in] 
	 *			���ӵ�Դ�洢ϵͳ���
	 * @param	const char* src	- [in] 
	 *			Դ·��
	 * @param	hdfs_handle ddsh	- [in] 
	 *			���ӵ�Ŀ��洢ϵͳ���
	 * @param	const char* dst	- [in] 
	 *			Ŀ��·��
	 */
    ICFAPI int hdfs_copyfile(hdfs_handle sdsh, const char* src, hdfs_handle ddsh, const char* dst);
	typedef int (*Proc_hdfs_copyfile)(hdfs_handle sdsh, const char* src, hdfs_handle ddsh, const char* dst);

	/** 
	 * @brief 	hdfs_movefile
	 *			�ƶ��ļ���֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle sdsh	- [in] 
	 *			���ӵ�Դ�洢ϵͳ���
	 * @param	const char* src	- [in] 
	 *			Դ·��
	 * @param	hdfs_handle ddsh	- [in] 
	 *			���ӵ�Ŀ��洢ϵͳ���
	 * @param	const char* dst	- [in] 	
	 *			Ŀ��·��
	 */
    ICFAPI int hdfs_movefile(hdfs_handle sdsh, const char* src, hdfs_handle ddsh, const char* dst);
	typedef int (*Proc_hdfs_movefile)(hdfs_handle sdsh, const char* src, hdfs_handle ddsh, const char* dst);

	/** 
	 * @brief 	hdfs_from_local
	 *			�ӱ��ؿ����ļ���DSSϵͳ�ϣ�֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* localpath	- [in] 
	 *			����·��
	 * @param	const char* dst	- [in] 	
	 *			Զ��·��
	 */
	ICFAPI int hdfs_from_local(hdfs_handle dsh, const char* localpath, const char* dst);
	typedef int (*Proc_hdfs_from_local)(hdfs_handle dsh, const char* localpath, const char* dst);

	/** 
	 * @brief 	hdfs_to_local
	 *			��DSSϵͳ�Ͽ����ļ������أ�֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* dst	- [in] 
	 *			Զ��·��
	 * @param	const char* localpath	- [in] 
	 *			����·��
	 */
	ICFAPI int hdfs_to_local(hdfs_handle dsh, const char* dst, const char* localpath);
	typedef int (*Proc_hdfs_to_local)(hdfs_handle dsh, const char* dst, const char* localpath);

	/** 
	* @brief 	hdfs_get_hosts
	*			��DSSϵͳ�ϲ�ѯ�ļ����ֲ�������
	* @return	char*** 
	*			������Щ������������ַ�����û�з��ؿգ�errnoָ���������
	* @param	hdfs_handle dsh	- [in] 
	*			���ӵĴ洢ϵͳ���
	* @param	const char* path	- [in] 
	*			�ļ�·��	
	* @param   size_t start - [in] block�Ŀ�ʼλ��
	* @param   size_t length - [in] block����
	*/
	ICFAPI char*** hdfs_get_hosts(hdfs_handle dsh, const char* path, size_t start, size_t length);
	typedef char*** (*Proc_hdfs_get_hosts)(hdfs_handle dsh, const char* path, size_t start, size_t length);

	/** 
	 * @brief 	hdfs_free_hosts
	 *			ɾ��hdfs_get_hosts�ļ�������������������ַ���ڴ�
	 * @return	No return value.
	 * @param	char ***hosts	- [in] 
	 *	        hdfs_get_hosts�ļ�������������������ַ���ڴ�
	 */
    ICFAPI void hdfs_free_hosts(hdfs_handle dsh, char ***hosts);
	typedef void (*Proc_hdfs_free_hosts)(hdfs_handle dsh, char ***hosts);
	
	/** 
	 * @brief 	hdfs_default_blocksize
	 *			��ѯ�ֲ�ʽ�洢ϵͳĬ�Ͽ��С
	 * @return	size_t 
	 *			����Ĭ�Ͽ��С���������-1��ʾ����errnoָ���������
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 */
	ICFAPI size_t hdfs_default_blocksize(hdfs_handle dsh);
	typedef size_t (*Proc_hdfs_default_blocksize)(hdfs_handle dsh);

   /** 
	* @brief 	hdfs_capacity
	*			��ѯ�ֲ�ʽ�洢ϵͳ����
	* @return	size_t 
	*			���طֲ�ʽϵͳ�������������-1��ʾ����errnoָ���������
	* @param	hdfs_handle dsh	- [in] 
	*			���ӵĴ洢ϵͳ���
	*/
	ICFAPI size_t hdfs_capacity(hdfs_handle dsh);
	typedef size_t (*Proc_hdfs_capacity)(hdfs_handle dsh);

    /** 
	 * @brief 	hdfs_used
	 *			��ѯ�ֲ�ʽ�洢ϵͳ�洢�����ļ���С
	 * @return	size_t 
	 *			���طֲ�ʽ�洢��ϵͳ�洢�ļ����ܴ�С���������-1��ʾ����errnoָ���������
	 * @param	hdfs_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 */
    ICFAPI size_t hdfs_used(hdfs_handle dsh);
	typedef size_t (*Proc_hdfs_used)(hdfs_handle dsh);

	/** 
	 * @brief 	hdfs_get_dsproxy
	 *			���һ��dsproxy�ĵ�ַ
	 * @return	int  ����0��ʾ�ɹ�������Ϊ�������
	 * @param	char* inbuf	- [in]	   ������������dsproxy��ַ��buf 
	 *			size_t inbuflen - [in] ��������������
	 *			int* num - [out]	   ��ʾ��dsproxy���ڷ���Ŀͻ�����Ŀ
	 */
    ICFAPI int hdfs_get_dsproxy(char* inbuf, size_t inbuflen, int* num);
	typedef int (*Proc_hdfs_get_dsproxy)(char* inbuf, size_t inbuflen, int* num);

	 /** 
	 * @brief 	hdfs_get_dsproxies
	 *			�������DSProxy��ַ���õ�ַ�е����е�ַ���յ�ǰ�����ʵ�����������򣬼��������������ǰ��
	 * @return	int  ����0��ʾ�ɹ�������Ϊ�������
	 * @param	char* inbuf	- [in]	   ������������dsproxy��ַ��buf 
	 *			size_t inbuflen - [in] ��������������
	 *			int* num - [out]	   ��ʾ���ڷ���Ŀͻ�����Ŀ
	  *			size_t innumlen - [in]	num�����ĳ���
	 *          size_t* counter - [out] ���е�dsproxy�ĸ���
	 * @Comments: ��ַ��,�ָ���dss://192.168.72.71:50180,dss://192.168.72.73:50180
	 */
    ICFAPI int hdfs_get_dsproxies(char* inbuf, size_t inbuflen, int* num, size_t innumlen, size_t* counter);
	typedef int (*Proc_hdfs_get_dsproxies)(char* inbuf, size_t inbuflen, int* num, size_t innumlen, size_t* counter);


#ifdef __cplusplus
}
#endif

#endif /* __IFLY_DSS_H__ */
