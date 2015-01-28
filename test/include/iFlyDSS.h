/** 
 * @file	iFlyDSS.h
 * @brief	Ѷ�ɷֲ�ʽ�洢ϵͳAPI
 * 
 * ���ļ�������Ѷ�ɷֲ�ʽ�洢ϵͳ��API����ԭ��
 * �����˸�API������������ͷ���ֵ���ͺ�ȡֵ��Χ
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
 * </table>
 * 
 */

#ifndef __IFLY_DSS_H__
#define __IFLY_DSS_H__

#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#include "iFlyCFDef.h"
#include "iFlyCFError.h"

/* ʹ�ñ����ļ�ϵͳ */
#define USE_LOCALFILE_SYSTEM "localfilesystem"

/* �ļ��Ĵ򿪷�ʽ */
#define DS_RDONLY O_RDONLY		// 0x0000
#define DS_WRONLY O_WRONLY		// 0x0001
#define DS_APPEND O_APPEND		// 0x0008
#define DS_RDNOCACHE				3
#define DS_WRNOCACHE				4
#define DS_APNOCACHE				5

#ifdef __cplusplus
extern  "C" {
#endif
	
	/* �ļ��������� */
	typedef enum stream_type
	{
		undefined = 0,
		input,
		output
	}stream_type;

	/* �ļ�ָ�� */
	typedef struct ds_file_t
	{
		ds_handle file;
		stream_type stream;
		int cache_open;	/* ���ļ���Ӧ��cache�Ƿ��Ѿ��� */
		int dss_open; /* ���ļ���Ӧ��DSS�ļ��Ƿ��Ѿ��� */
	} * DSFILE;
	 
	/* �ļ����� */
	typedef enum ds_file_kind {
        kind_file = 'F',
        kind_dir = 'D',
    }ds_file_kind;

	/* �ļ���Ϣ */
	typedef struct ds_file_info_t
	{
		ds_file_kind kind;		/* �����ļ���Ŀ¼	*/
        char   *name;			/* �ļ���			*/
		char   *owner;			/* �ļ���			*/
		char   *group;			/* �������ļ�����	*/
		short  permissions;		/* �ļ����������	*/
		short  replication;		/* ��������			*/
        time_t last_modified;	/* ����޸�ʱ��sec	*/
		time_t last_access;		/* ������ʱ��sec	*/
        uint64 size;			/* �ļ����� byte		*/
        uint64 blocksize;		/* ���С			*/
	} ds_file_info;

	typedef enum ds_event_t 
	{
		event_hdfs_connect_down,	/* hdfs�����ж� */
		event_hdfs_resume,			/* hdfs���ӻظ� */
		event_dsprx_connect_down,  /* dsproxy�����ж� */
		event_file_connect_down		/* �ļ����������ж� */
	} ds_event;

	/* �ص������������� */
	typedef struct edata_t
	{
		ds_handle old_hd; /* ԭ�ȵľ���� */
		ds_handle new_hd; /* �µľ���ţ����û��������Ϊ-1 */
		DSFILE    old_fd; /* ԭ�ȵ��ļ������� */
	} edata;

	/* ���������������������жϻ��߷���˱������쳣������ͻ��˽ӿڻὫ��Щ���ͨ���ص�����
	   ֪ͨ�����ó��򣬵��ó���������Щ�쳣���д���Ŀǰϵͳ�������쳣�ʹ���������:
	   (1) event_hdfs_connect_down 
	       ˵����HDFS NameNode������
		   �ͻ������������ͻ������յ���һ֪ͨ��Ӧ�ñ���data.old_hd����ָ�����û�����
		   ʹ�õ�ĳ��session�ľ����������һ�¼�˵����session���ٿ��ã�����û�����ʹ�øþ��
		   ���е��ýӿڻ᷵�ش�����ʱ��Ӧ�ó���Ӧ����ͣʹ��iDSS���ܣ�ֱ������event_hdfs_resume�¼�
	   (2) event_hdfs_resume
	       ˵����HDFS NameNode����ʹ��
		   �ͻ������������ͻ��������յ���һ֪ͨ��˵��HDFS�Ѿ�����ʹ�ã��ͻ�����Ӧ�����ȵ���
		   ds_disconnect()�����ͷ���Щ��(1)�м�¼�ľ������ΪNameNode����������ԭ�о���Ѿ���Ч��
		   Ȼ�����µ���ds_connect����iDSS��
	   (3) event_dsprx_connect_down
	       ˵�������ӵ�dsproxy������
		   �ͻ������������ͻ������յ���һ֪ͨ��Ӧ�ñ���data.old_hd����ָ�����û�����
		   ʹ�õ�ĳ��session�ľ����������һ�¼�˵����session���ٿ��ã�����û�����ʹ�øþ��
		   ���е��ýӿڻ᷵�ش�����ʱ����ʵ��������ȵ���ds_disconnect���ͷŵ�������������ӣ�Ȼ��
		   ����ds_get_dsproxy()�������һ���Ϻõ�dsproxy�����µ������µ���ds_connect����iDSS��
	   (4) event_file_connect_down
		   ˵�����ļ�����ͨ������
		   �ͻ��������������˳����յ���һ֪ͨ��Ӧ�ñ���data.old_fd,��ָ���˳�����ļ���������
		   data.old_hdָ�������Ӿ����������һ�¼�˵�����ļ�����ͨ��ʧЧ������ds_fclose()�ر���һ�ļ���
		   ͬʱ�û�Ӧ�õ���ds_fopen���´�Ҫ������ļ���
		*/
	typedef int (*DS_EVENT_HOOK) (ds_event e, edata* data, void* cbdata);

	/** 
	 * @brief 	ds_initialize_ex
	 *			��ʼ��ϵͳ�ĵ�һ������������Ҫ����һ��
	 * @return	DSAPI int	- �ɹ�����0�����򷵻�ʧ�ܴ���
	 * @param	int enable_dcache	- [in] - �Ƿ�ʹ�÷ֲ�ʽcache
	 * @param	DS_EVENT_HOOK fn	- [in] - ��������ϵͳ�¼���Callback����
	 * @param	void* reserv	- [in]
	 *          �����Ժ�ʹ��
	 * @see		
	 */
	DSAPI int ds_initialize_ex(int enable_dcache, DS_EVENT_HOOK fn, void* cbdata, void* reserv);
	typedef int (*Proc_ds_initialize_ex)(int enable_dcache, DS_EVENT_HOOK fn, void* cbdata, void* reserv);

	/** 
	 * @brief 	ds_initialize
	 *			��ʼ��ϵͳ�ĵ�һ������������Ҫ����һ��
	 * @return	DSAPI int	- �ɹ�����0�����򷵻�ʧ�ܴ���
	 * @param	int enable_dcache	- [in] - �Ƿ�ʹ�÷ֲ�ʽcache
	 * @param	void* reserv	- [in]
	 *          �����Ժ�ʹ��
	 * @see		
	 */
	DSAPI int ds_initialize(int enable_dcache, void* reserv);
	typedef int (*Proc_ds_initialize)(int enable_dcache, void* reserv);

	/** 
	 * @brief 	ds_uninitialize
	 *			���ʼ�������һ������
	 * @return	�޷���ֵ�����ú��ڿ���.
	 * @see		
	 */
	DSAPI void ds_uninitialize(void);
	typedef void (*Proc_ds_uninitialize)(void);
	
	/** 
	 * @brief 	ds_connect
	 *          ���ӷֲ�ʽ�洢ϵͳ
	 * @return	ds_handle ����ɹ����ش洢ϵͳ��������ʧ�ܷ���ds_invalid_handle��errnoָ��ʧ��ԭ��
	 * @param	const char* dsURL	- [in] �ֲ�ʽ�洢URL
	 *			dsURLSӦ����������ʽ scheme://host:port/dir
	 *				scheme -Э�飬����Э����hdfs��Hadoop�ֲ�ʽ�ļ�ϵͳ��
	 *						���������û����������Ŀ¼��grammar
	 *				dir    -��Ŀ¼�������Ŀ¼���ڵ�ǰĿ¼����Ϊ��Ŀ¼
	 *                      ��������Ϊ��Ŀ¼����������Ŀ¼��Ŀ¼����ָӳ��
	 *                      ��������Ŀ¼��DSSĿ¼
	 * @param	const char* uid	- [in]
	 *          ָ��һ�����Ա����û���ݵ�id
	 * @param	const char* proxyAddr	- [in] 
	 *			ʹ��dsproxy�ĵ�ַ��Linux��ΪNULL
	 * @param	void* reserv	- [in]
	 *          �����Ժ�ʹ��
	 * @Comments: ��ʹ��dsproxyʱһ��������dsURL�е�host��port��uid��ͬ��ʶ��Ҳ����˵���
	 *            host��port��uid����ͬ����ú���Ӧ�÷�����ͬ�ľ��
	 *            ʹ��dsproxyʱ���ӳ���������ʶ�⻹Ӧ�ü���dsproxy�ĵ�ַ��Ҳ����˵�ͻ���ͨ��
	 *            ��ͬ��dsproxy��ͬһ�û�������ͬһ��dsURL�����ز�ͬ�ľ��
	 */
	DSAPI ds_handle ds_connect(const char* dsURL, const char* uid, const char* proxyAddr, void* reserv);
	typedef ds_handle (*Proc_ds_connect)(const char* dsURL, const char* uid, const char* proxyAddr, void* reserv);

	/** 
	 * @brief 	ds_disconnect
	 *          �Ͽ��ֲ�ʽ�ļ�ϵͳ
	 * @return	int - Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *          ds_connect�������صľ�� 
	 */
	DSAPI int ds_disconnect(ds_handle dsh);
	typedef int (*Proc_ds_disconnect)(ds_handle dsh);

	/** 
	 * @brief 	ds_fopen
	 *			���ļ�
	 * @return	DSFILE �ɹ������ļ���������ʧ�ܷ���NULL��errnoָ��ʧ��ԭ��
	 * @param	ds_handle dsh	- [in] 
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
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			ָ�����ļ��Ƿ�ʹ��cache������ǣ������0�����cache�е��ļ�������ֱ�Ӵ�idss�е��ļ�
	 * @Comments: buffersize�Ǵ򿪵����Ļ�������С����ֵ�����մ���java��BufferedInputStream��BufferedOutputStream
	 *            ������Ĺ��캯������Java SE��ʵ���������ָ�����ֵ����Ĭ��Ϊ8192�ֽ�
	 *            hadoop.configuration ��io.file.buffer.size���ʹ����ж���ĸ�ֵΪ4096.
	 */
	DSAPI DSFILE ds_fopen(ds_handle dsh, const char* path, int flags,
		int buffersize, short replication, int blocksize, int cache_flag);
	typedef DSFILE (*Proc_ds_fopen)(ds_handle dsh, const char* path, int flags,
		int buffersize, short replication, int blocksize, int cache_flag);

	/** 
	 * @brief 	ds_fclose
	 *			�ر��ļ�
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			׼���رյ��ļ�ָ��	
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			�Ƿ���cache
	 */
    DSAPI int ds_fclose(ds_handle dsh, DSFILE file, int cache_flag);
	typedef int (*Proc_ds_fclose)(ds_handle dsh, DSFILE file, int cache_flag);

	/** 
	 * @brief 	ds_sync_fclose
	 *			�ر��ļ�����ǰ���ṩ������ͬ��ʹ��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			׼���رյ��ļ�ָ��	
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			�Ƿ���cache
	 */
    DSAPI int ds_sync_fclose(ds_handle dsh, DSFILE file, int cache_flag);
	typedef int (*Proc_ds_sync_fclose)(ds_handle dsh, DSFILE file, int cache_flag);

	/** 
	 * @brief 	ds_read
	 *			��ȡ�ļ�
	 * @return	int ������ʵ��ȡ���ֽ���������-1Ϊʧ�ܣ�errnoָ��ʧ��ԭ��
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	void* buffer	- [in] 
	 *			��ȡ������
	 * @param	int length	- [in] range:(0, 0x7FFFFFFF]
	 *			����������
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			�Ƿ���cache
	 */
    DSAPI int ds_read(ds_handle dsh,  DSFILE file, void* buffer, int length, int cache_flag);
	typedef int (*Proc_ds_read)(ds_handle dsh,  DSFILE file, void* buffer, int length, int cache_flag);

	/** 
	 * @brief 	ds_write
	 *			д���ļ�
	 * @return	int ������ʵд����ֽ���������-1Ϊʧ�ܣ�errnoָ��ʧ��ԭ��
	 *			����0Ϊд�뻺���������������0�������Ե�Ƭ�̺��ٴ�д��
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	const void* buffer	- [in] 
	 *			׼��д������ݻ���
	 * @param	int length	- [in] 
	 *			׼��д������ݻ��泤��
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			�Ƿ���cache
	 */
    DSAPI int ds_write(ds_handle dsh, DSFILE file, const void* buffer, int length, int cache_flag);
	typedef int (*Proc_ds_write)(ds_handle dsh, DSFILE file, const void* buffer, int length, int cache_flag);
	 
	/**
	 * @brief 	ds_flush
	 *			��д�뻺���е�����flush���洢ϵͳ��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			�Ƿ���cache
	 */
    DSAPI int ds_flush(ds_handle dsh, DSFILE file, int cache_flag);
	typedef int (*Proc_ds_flush)(ds_handle dsh, DSFILE file, int cache_flag);

	/** 
	 * @brief 	ds_seek
	 *			���ļ���ȡָ���Ƶ��ļ���ĳһλ��
	 *			�ú���ֻ��read-onlyģʽ����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in]
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	size_t offset	- [in] 
	 *			λ�Ƴ���
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			�Ƿ���cache
	 */
    DSAPI int ds_seek(ds_handle dsh, DSFILE file, size_t offset, int cache_flag); 
	typedef int (*Proc_ds_seek)(ds_handle dsh, DSFILE file, size_t offset, int cache_flag); 

	/** 
	 * @brief 	ds_tell
	 *			���ص�ǰ�ļ���offsetֵ����bytesΪ��λ
	 * @return	size_t ���ص�ǰ�ļ���offsetֵ������Ϊ-1��errnoָ������ԭ��
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	int cache_flag	- [in] range [0, 1]
	 *			�Ƿ���cache
	 */
    DSAPI size_t ds_tell(ds_handle dsh, DSFILE file, int cache_flag);
	typedef size_t (*Proc_ds_tell)(ds_handle dsh, DSFILE file, int cache_flag);

	/** 
	 * @brief 	ds_create_dir
	 *			����Ŀ¼������Ŀ¼��
				hadoop�������֧�ֵ�Ŀ¼���Ϊ121��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			������[����]Ŀ¼·����֧�־���·�������·��	
	 * @param	const char* orignal_path	- [in] 
	 *			������Ŀ¼ʱ�ò�������NULL����������Ŀ¼ʱ�ò�����������Ŀ¼ӳ���ԴĿ¼	
	 */
    DSAPI int ds_create_dir(ds_handle dsh, const char* path, const char* orignal_path);
	typedef int (*Proc_ds_create_dir)(ds_handle dsh, const char* path, const char* orignal_path);

	/** 
	 * @brief 	ds_exists
	 * 			�ж�һ���ļ�/Ŀ¼����������Ŀ¼���Ƿ����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char *path	- [in] 
	 *			��֤��·������
	 */
    DSAPI int ds_exists(ds_handle dsh, const char *path);
	typedef int (*Proc_ds_exists)(ds_handle dsh, const char *path);

	/** 
	 * @brief 	ds_delete
	 *			ɾ��һ���ļ�/Ŀ¼����������Ŀ¼��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
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
    DSAPI int ds_delete(ds_handle dsh, const char* path, int recursive, int cache_flag);
	typedef int (*Proc_ds_delete)(ds_handle dsh, const char* path, int recursive, int cache_flag);

	/** 
	 * @brief 	ds_rename
	 *			������һ���ļ���Ŀ¼����������Ŀ¼��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* old_path	- [in] 
	 *			ԭ·������
	 * @param	const char* new_path	- [in] 
	 *			��·������
	 */
    DSAPI int ds_rename(ds_handle dsh, const char* old_path, const char* new_path);
	typedef int (*Proc_ds_rename)(ds_handle dsh, const char* old_path, const char* new_path);

	/** 
	 * @brief 	ds_setreplication
	 *			�����ļ��ĸ�����Ŀ ����������·����
				�ļ��ĸ�����Ŀ��ΧΪ��1��512
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			·������
	 * @param	short replication	- [in] 
	 *			�µĸ�����Ŀ
	 */
    DSAPI int ds_setreplication(ds_handle dsh, const char* path, short replication);
	typedef int (*Proc_ds_setreplication)(ds_handle dsh, const char* path, short replication);

	/** 
	 * @brief 	ds_chown
	 *			�����ļ�����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in]
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in]
	 *			·������
	 * @param	const char* owner - [in]
	 *			�ļ������������NULL��ֻ����group
	 * @param	const char* group - [in] 
	 *			�ļ��飬�������NULL��ֻ����owner
	 */
    DSAPI int ds_chown(ds_handle dsh, const char* path, const char *owner, const char *group);
	typedef int (*Proc_ds_chown)(ds_handle dsh, const char* path, const char *owner, const char *group);

	/** 
	 * @brief 	ds_chmod
	 *			�����ļ�Ȩ��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in]
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in]
	 *			·������
	 * @param	short mode - [in]
				�ļ�Ȩ��
				hadoop���ļ������û������������Ķ�дִ��Ȩ�ޣ���ʽΪrwx��xͳһ����Ϊ0���������ļ�Ϊ�û���д�������������Ȩ�ޣ���Ӧ����Ϊrw_r_____��
				�˴�����modeΪ��110100000����2���ƣ�����Ӧ��10����������416��
	 *			
	 */
    DSAPI int ds_chmod(ds_handle dsh, const char* path, short mode);
	typedef int (*Proc_ds_chmod)(ds_handle dsh, const char* path, short mode);

	/** 
	 * @brief 	ds_utime
	 *			�����ļ���ʱ����Ϣ
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in]
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in]
	 *			·������
	 * @param	time_t mtime - [in]
	 *			�ļ����޸�ʱ�䣬�������0����ֻ���÷���ʱ��
	 * @param	time_t atime - [in]
	 *			�ļ��ķ���ʱ�䣬�������0����ֻ�����޸�ʱ��
	 */
	DSAPI int ds_utime(ds_handle dsh, const char* path, time_t mtime, time_t atime);
	typedef int (*Proc_ds_utime)(ds_handle dsh, const char* path, time_t mtime, time_t atime);

	/** 
	 * @brief 	ds_getworkingdir
	 *			���ǰ����Ŀ¼
	 * @return	char* ����ɹ�����ָ��buffer��ָ�룬���򷵻�NULL��errnoָ������ԭ��
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	char *buffer	- [in] 
	 *			������ŵ�ǰĿ¼�Ļ���
	 * @param	size_t length	- [in] 
	 *			���泤��	
	 */
    DSAPI char* ds_getworkingdir(ds_handle dsh, char *buffer, size_t length);
	typedef char* (*Proc_ds_getworkingdir)(ds_handle dsh, char *buffer, size_t length);

	/** 
	 * @brief 	ds_setworkingdir
	 *			���õ�ǰ����Ŀ¼
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			���õ�Ŀ¼·�������·��������ڵ�ǰ·������������·��ҲҪ����		
	 */
    DSAPI int ds_setworkingdir(ds_handle dsh, const char* path);
	typedef int (*Proc_ds_setworkingdir)(ds_handle dsh, const char* path);

	/** 
	 * @brief 	*ds_list_dir
	 *			�г�Ŀ¼��������Ŀ¼���е��ļ���Ϣ
	 * @return	ds_file_info �����ļ���Ϣ�����飬ʧ�ܷ���NULL��errnoָ������ԭ��
	 *			��������ϵͳ��̬�����裬�û���Ҫ�����ͷź����ͷ�
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			Ŀ¼��������Ŀ¼��·��
	 * @param	int *num	- [in] 
	 *			ds_file_info����
	 */
    DSAPI ds_file_info *ds_list_dir(ds_handle dsh, const char* path, int *num);
	typedef ds_file_info* (*Proc_ds_list_dir)(ds_handle dsh, const char* path, int *num);

	/** 
	 * @brief 	*ds_path_info
	 *			�г��ļ�/Ŀ¼��������Ŀ¼�������ļ���Ϣ
	 * @return	ds_file_info �����ļ���Ϣ��ʧ�ܷ���NULL��errnoָ������ԭ��
	 *			����Ϣ��ϵͳ��̬�����裬�û���Ҫ�����ͷź����ͷ�
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			Ŀ¼��������Ŀ¼��·��
	 */
    DSAPI ds_file_info *ds_path_info(ds_handle dsh, const char* path);
	typedef ds_file_info* (*Proc_ds_path_info)(ds_handle dsh, const char* path);

	/** 
	 * @brief 	ds_free_file_info
	 *			�ͷ�ds_file_info��Ϣ
	 * @return	No return value.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	ds_file_info *fileinfo	- [in] 
	 *			ds_file_info������
	 * @param	int num	- [in] 	
	 *			����Ԫ�صĸ���
	 */
	DSAPI void ds_free_file_info(ds_handle dsh, ds_file_info *fileinfo, int num);
	typedef void (*Proc_ds_free_file_info)(ds_handle dsh, ds_file_info *fileinfo, int num);

	/** 
	 * @brief 	ds_find_change_notification
	 *			�����ļ��������Ѽ���ļ��޸�
	 * @return	ds_handle �ɹ�����һ�����Ѿ����ʧ�ܷ���ds_invalid_handle��errnoָ��ʧ��ԭ��
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			��Ҫ���ӵ��ļ�·�������·��Ŀǰֻ֧���ļ�
	 * @param	int watchsubtree	- [in] 
	 *			�Ƿ������Ŀ¼���ļ����������Ŀǰֻ��Ϊ�˺�Windows API����һ��
	 * @param	int filter	- [in] 
	 *			��ع��������������Ŀǰֻ��Ϊ�˺�Windows API����һ��
	 */
	DSAPI ds_handle ds_find_change_notification(ds_handle dsh, const char* path, int watchsubtree, int filter);
	typedef ds_handle (*Proc_ds_find_change_notification)(ds_handle dsh, const char* path, int watchsubtree, int filter);

	/** 
	 * @brief 	ds_wait
	 *			�ȴ����
	 * @return	DSAPI int	- Return 0 in success, -1��ʱ������Ϊ�������.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	ds_handle handle	- [in] 
	 *			Ҫ�ȴ��ľ��
	 * @param	time_t ms	- [in]
	 *			��ʱʱ��
	 * @see		
	 */
	DSAPI int ds_wait(ds_handle dsh, ds_handle handle, time_t ms);
	typedef int (*Proc_ds_wait)(ds_handle dsh, ds_handle handle, time_t ms);

	/** 
	 * @brief 	ds_copyfile
	 *			�����ļ���֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle sdsh	- [in] 
	 *			���ӵ�Դ�洢ϵͳ���
	 * @param	const char* src	- [in] 
	 *			Դ·��
	 * @param	ds_handle ddsh	- [in] 
	 *			���ӵ�Ŀ��洢ϵͳ���
	 * @param	const char* dst	- [in] 
	 *			Ŀ��·��
	 */
    DSAPI int ds_copyfile(ds_handle sdsh, const char* src, ds_handle ddsh, const char* dst);
	typedef int (*Proc_ds_copyfile)(ds_handle sdsh, const char* src, ds_handle ddsh, const char* dst);

	/** 
	 * @brief 	ds_movefile
	 *			�ƶ��ļ���֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle sdsh	- [in] 
	 *			���ӵ�Դ�洢ϵͳ���
	 * @param	const char* src	- [in] 
	 *			Դ·��
	 * @param	ds_handle ddsh	- [in] 
	 *			���ӵ�Ŀ��洢ϵͳ���
	 * @param	const char* dst	- [in] 	
	 *			Ŀ��·��
	 */
    DSAPI int ds_movefile(ds_handle sdsh, const char* src, ds_handle ddsh, const char* dst);
	typedef int (*Proc_ds_movefile)(ds_handle sdsh, const char* src, ds_handle ddsh, const char* dst);

	/** 
	 * @brief 	ds_from_local
	 *			�ӱ��ؿ����ļ���DSSϵͳ�ϣ�֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* localpath	- [in] 
	 *			����·��
	 * @param	const char* dst	- [in] 	
	 *			Զ��·��
	 */
	DSAPI int ds_from_local(ds_handle dsh, const char* localpath, const char* dst);
	typedef int (*Proc_ds_from_local)(ds_handle dsh, const char* localpath, const char* dst);

	/** 
	 * @brief 	ds_sync_from_local
	 *			�ӱ��ؿ����ļ���DSSϵͳ�ϣ�֧������·����,��ǰ���ṩ������ͬ��ʹ��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* localpath	- [in] 
	 *			����·��
	 * @param	const char* dst	- [in] 	
	 *			Զ��·��
	 */
	DSAPI int ds_sync_from_local(ds_handle dsh, const char* localpath, const char* dst);
	typedef int (*Proc_ds_sync_from_local)(ds_handle dsh, const char* localpath, const char* dst);

	/** 
	 * @brief 	ds_to_local
	 *			��DSSϵͳ�Ͽ����ļ������أ�֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* dst	- [in] 
	 *			Զ��·��
	 * @param	const char* localpath	- [in] 
	 *			����·��
	 */
	DSAPI int ds_to_local(ds_handle dsh, const char* dst, const char* localpath);
	typedef int (*Proc_ds_to_local)(ds_handle dsh, const char* dst, const char* localpath);

	/** 
	* @brief 	ds_get_hosts
	*			��DSSϵͳ�ϲ�ѯ�ļ����ֲ�������
	* @return	char*** 
	*			������Щ������������ַ�����û�з��ؿգ�errnoָ���������
	* @param	ds_handle dsh	- [in] 
	*			���ӵĴ洢ϵͳ���
	* @param	const char* path	- [in] 
	*			�ļ�·��	
	* @param   size_t start - [in] block�Ŀ�ʼλ��
	* @param   size_t length - [in] block����
	*/
	DSAPI char*** ds_get_hosts(ds_handle dsh, const char* path, size_t start, size_t length);
	typedef char*** (*Proc_ds_get_hosts)(ds_handle dsh, const char* path, size_t start, size_t length);

	/** 
	 * @brief 	ds_free_hosts
	 *			ɾ��ds_get_hosts�ļ�������������������ַ���ڴ�
	 * @return	No return value.
	 * @param	char ***hosts	- [in] 
	 *	        ds_get_hosts�ļ�������������������ַ���ڴ�
	 */
    DSAPI void ds_free_hosts(ds_handle dsh, char ***hosts);
	typedef void (*Proc_ds_free_hosts)(ds_handle dsh, char ***hosts);
	
	/** 
	 * @brief 	ds_default_blocksize
	 *			��ѯ�ֲ�ʽ�洢ϵͳĬ�Ͽ��С
	 * @return	size_t 
	 *			����Ĭ�Ͽ��С���������-1��ʾ����errnoָ���������
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 */
	DSAPI size_t ds_default_blocksize(ds_handle dsh);
	typedef size_t (*Proc_ds_default_blocksize)(ds_handle dsh);

   /** 
	* @brief 	ds_capacity
	*			��ѯ�ֲ�ʽ�洢ϵͳ����
	* @return	size_t 
	*			���طֲ�ʽϵͳ�������������-1��ʾ����errnoָ���������
	* @param	ds_handle dsh	- [in] 
	*			���ӵĴ洢ϵͳ���
	*/
	DSAPI size_t ds_capacity(ds_handle dsh);
	typedef size_t (*Proc_ds_capacity)(ds_handle dsh);

    /** 
	 * @brief 	ds_used
	 *			��ѯ�ֲ�ʽ�洢ϵͳ�洢�����ļ���С
	 * @return	size_t 
	 *			���طֲ�ʽ�洢��ϵͳ�洢�ļ����ܴ�С���������-1��ʾ����errnoָ���������
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 */
    DSAPI size_t ds_used(ds_handle dsh);
	typedef size_t (*Proc_ds_used)(ds_handle dsh);

	/////////////////////////////////////////////////////////////////////////////////////

	 /** 
	 * @brief 	ds_get_dsproxy
	 *			���һ��dsproxy�ĵ�ַ
	 * @return	int  ����0��ʾ�ɹ�������Ϊ�������
	 * @param	char* inbuf	- [in]	   ������������dsproxy��ַ��buf 
	 *			size_t inbuflen - [in] ��������������
	 *			int* num - [out]	   ��ʾ��dsproxy���ڷ���Ŀͻ�����Ŀ
	 */
    DSAPI int ds_get_dsproxy(char* inbuf, size_t inbuflen, int* num);
	typedef int (*Proc_ds_get_dsproxy)(char* inbuf, size_t inbuflen, int* num);

	 /** 
	 * @brief 	ds_get_dsproxies
	 *			�������DSProxy��ַ���õ�ַ�е����е�ַ���յ�ǰ�����ʵ�����������򣬼��������������ǰ��
	 * @return	int  ����0��ʾ�ɹ�������Ϊ�������
	 * @param	char* inbuf	- [in]	   ������������dsproxy��ַ��buf 
	 *			size_t inbuflen - [in] ��������������
	 *			int* num - [out]	   ��ʾ���ڷ���Ŀͻ�����Ŀ
	  *			size_t innumlen - [in]	num�����ĳ���
	 *          size_t* counter - [out] ���е�dsproxy�ĸ���
	 * @Comments: ��ַ��,�ָ���dss://192.168.72.71:50180,dss://192.168.72.73:50180
	 */
    DSAPI int ds_get_dsproxies(char* inbuf, size_t inbuflen, int* num, size_t innumlen, size_t* counter);
	typedef int (*Proc_ds_get_dsproxies)(char* inbuf, size_t inbuflen, int* num, size_t innumlen, size_t* counter);

#ifdef WIN32
	 /** 
	 * @brief 	ds_get_current_dsproxy
	 *			��õ�ǰ����ʹ�õ�dsproxy��ַ
	 * @return	int  ����0��ʾ�ɹ�������Ϊ�������
	 * @param	ds_handle dsh	- [in] ���Ӿ�� 
	 */
	DSAPI const char* ds_get_current_dsproxy(ds_handle dsh);
#endif

#ifdef __cplusplus
}
#endif

#endif /* __IFLY_DSS_H__ */
