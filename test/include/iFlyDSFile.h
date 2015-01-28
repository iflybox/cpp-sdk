/** 
 * @file	iFlyDSFile.h
 * @brief	�洢ϵͳ�����ļ�����
 * 
 * detail...
 * 
 * @author	jingzhang
 * @version	1.0
 * @date	2012/2/23
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2012/2/23	<td>jingzhang	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __IFLYDSFILE_H__
#define __IFLYDSFILE_H__

#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#include "iFlyCFDef.h"
#include "iFlyCFError.h"

#ifdef __cplusplus
extern  "C" {
#endif

		/* ʹ�ñ����ļ�ϵͳ */
	#define USE_LOCALFILE_SYSTEM "localfilesystem"

	/* �ļ��Ĵ򿪷�ʽ */
	#define DS_RDONLY O_RDONLY		// 0x0000
	#define DS_WRONLY O_WRONLY		// 0x0001
	#define DS_APPEND O_APPEND		// 0x0008
		
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
		event_dsprx_connect_down,   /* dsproxy�����ж� */
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


#ifdef __cplusplus

}
#endif

#endif /* __IFLYDSFILE_H__ */
