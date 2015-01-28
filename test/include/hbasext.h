/** 
 * @file	hbase.h
 * @brief	hbase��չ�ӿ�
 * 
 * ���ļ�������Ѷ�ɷֲ�ʽ���ݿ�ϵͳ��API����ԭ��
 * �����˸�API������������ͷ���ֵ���ͺ�ȡֵ��Χ
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
	 *			��ʼ��ϵͳ�ĵ�һ������������Ҫ����һ��
	 * @return	base_handler	- ����ɹ�����ϵͳ��������ʧ�ܷ���NULL
	 * @param	const char* zookeeper_url	- [in] 
	 *			zookeeper ����url
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI base_handler ibase_init(const char *zookeeper_url, int* errnum);
	typedef base_handler (*Proc_ibase_init)(const char *zookeeper_url, int* errnum);

	/** 
	 * @brief 	ibase_connect
	 *			����Ŀ������,�����̲߳��ɸ��������̵߳����Ӿ������Ӧ���иú����ĵ��ã���ȡ�¾��
	 * @return	table_handler	- ����ɹ�����ϵͳ��������ʧ�ܷ���NULL
	 * @param	base_handler base_handle	- [in] 
	 *			ibase_init���صľ��
	 * @param	const char* table	- [in] 
	 *			Ŀ�����
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI table_handler ibase_connect(base_handler base_handle, const char* table, int* errnum);
	typedef table_handler (*Proc_ibase_connect)(base_handler base_handle, const char* table, int* errnum);


	/** 
	 * @brief 	ibase_disconnect
	 *			����Ŀ������
	 * @return	bool 	- �ɹ�����ture�����ʧ��false
	 * @param	table_handler handle	- [in] 
	 *			ibase_connect���صľ��
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI bool ibase_disconnect(table_handler handle, int* errnum);
	typedef bool(*Proc_ibase_disconnect)(table_handler handle, int* errnum);

	/** 
	 * @brief 	ibase_table_exist
	 *			�ж�һ��ibase���Ƿ����
	 * @return	bool 	- Return true in success, otherwise return false
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect���صľ��
	 * @param	const char *table	- [in] 
	 *			��ѯ��Ŀ�������
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI int ibase_table_exist(table_handler handler,const char *table, int* errnum);
	typedef int (*Proc_ibase_table_exist)(table_handler handler,const char *table, int* errnum);

	/** 
	 * @brief 	ibase_get_Families
	 *			����Ŀ�������Ӧ�ĸ����е�����
	 * @return	char** 	- �ɹ����ض�Ӧ��������Ƶ�ַ�����ʧ�ܷ���NULL
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect���صľ��
	 * @param	const char *table	- [in] 
	 *			��ѯ��Ŀ�������
	 * @param	int *numfamilies	- [out] 
	 *			�ñ�����Ӧ���е�������������256��
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI char** ibase_get_families(table_handler handler,const char *table, int *numfamilies, int* errnum);
	typedef char** (*Proc_ibase_get_families)(table_handler handler,const char *table, int *numfamilies, int* errnum);

	/** 
	 * @brief 	ibase_Families_free
	 *			ɾ��ibase_get_Families�ļ���������������Ƶ��ڴ�ռ�
	 * @return	No return value.
	 * @param	char ** cfamilies	- [in] 
	 *			����ibase_get_Families�����ص��ڴ��ַ
	 * @param	int num_families	- [in] 
	 *			����ibase_get_Families ���ص��е�����
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI void ibase_families_free(char ** cfamilies, int num_families, int* errnum);
	typedef void (*Proc_ibase_families_free)(char ** cfamilies, int num_families, int* errnum);

	/** 
	 * @brief 	ibase_family_exist
	 *			�ж�ָ���ֲ�ʽ�����ض������Ƿ����
	 * @return	bool 	- Return true in success, otherwise return false
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect���صľ��
	 * @param	hbaseKeyInfo* hb_info	- [in] 
	 *			hb_infoָ���ϴ����ݶ�Ӧ�ľ������Ϣ������Ŀ������ƣ�Ŀ�������ƣ�Ŀ��������ƣ�Ŀ����޶����ƣ��Լ��汾��
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI bool ibase_family_exist(table_handler handler, hbaseKeyInfo* hb_info, int* errnum);
	typedef bool (*Proc_ibase_family_exist)(table_handler handler, hbaseKeyInfo* hb_info, int* errnum);

	/** 
	 * @brief 	ibase_put
	 *			��ֲ�ʽ���ݿ��ϴ�����
	 * @return	bool 	- Return true in success, otherwise return false
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect���صľ��
	 * @param	hbaseKeyInfo* hb_info	- [in] 
	 *			hb_infoָ���ϴ����ݶ�Ӧ�ľ������Ϣ������Ŀ������ƣ�Ŀ�������ƣ�Ŀ��������ƣ�Ŀ����޶����ƣ��Լ��汾��
	 * @param	void *buff	- [in] 
	 *			׼��д������ݻ���
	 * @param	int len 	- [in] 
	 *			׼��д������ݻ��泤��
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI bool ibase_put(table_handler handler, hbaseKeyInfo* hb_info,void *buff,int len, int* errnum);
	typedef bool (*Proc_ibase_put)(table_handler handler, hbaseKeyInfo* hb_info,void *buff,int len, int* errnum);

	/** 
	 * @brief 	ibase_get
	 *			�ӷֲ�ʽ���ݿ��������
	 * @return	long 	- ������ʵ��ȡ���ֽ���������-1Ϊʧ��
	 * @param	table_handler handler	- [in] 
	 *			ibase_connect���صľ��
	 * @param	hbaseKeyInfo* hb_info	- [in] 
	 *			hb_infoָ���������ݶ�Ӧ��Ŀ�����Ϣ������Ŀ������ƣ�Ŀ�������ƣ�Ŀ��������ƣ�Ŀ����޶����ƣ��Լ��汾��
	 * @param	void **value	- [out] 
	 *			���ض�̬���ٵĴ�Ŷ������ݵ��ڴ��ַ
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI long ibase_get(table_handler handler,hbaseKeyInfo* hb_info, void **value, int* errnum);
	typedef long (*Proc_ibase_get)(table_handler handler,hbaseKeyInfo* hb_info, void **value, int* errnum);

	/** 
	 * @brief 	ibase_get_free
	 *			ɾ��ibase_get�����������Ŷ������ݵ��ڴ�ռ�
	 * @return	No return value.
	 * @param	void* value	- [in] 
	 *			����ibase_get����õ��ڴ��ַ
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI void ibase_get_free(void* value, int* errnum);
	typedef void (*Proc_ibase_get_free)(void* value, int* errnum);

	/** 
	 * @brief 	ibase_uninit
	 *			���ʼ�������һ������
	 * @return	bool 	- Return true in success, otherwise return false
	 * @param	base_handler handler	- [in] 
	 *			ibase_init���صľ��
	 * @param	int* errnum					- [out]
	 *			�ò���ʧ�ܵĴ�����
	 */
	ICFAPI bool ibase_uninit(base_handler handler, int* errnum);
	typedef bool (*Proc_ibase_uninit)(base_handler handler, int* errnum);

#ifdef __cplusplus
}
#endif

#endif

