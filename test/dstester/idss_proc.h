//#include "iFlyDSS.h"

#ifdef __cplusplus
extern  "C" {
#endif

	/** 
	 * @brief 	ds_connect
	 *          ���ӷֲ�ʽ�洢ϵͳ
	 * @return	ds_handle ����ɹ����ش洢ϵͳ��������ʧ�ܷ���ds_invalid_handle
	 * @param	const char* url	- [in]
	 *			urlӦ����������ʽ scheme://host:port/dir
	 *				scheme -Э�飬����Э����dss���ֲ�ʽ�洢ϵͳ��
	 *						���������û����������Ŀ¼��grammar
	 *				dir    -��Ŀ¼�������Ŀ¼���ڵ�ǰĿ¼����Ϊ��Ŀ¼
	 *                      ��������Ϊ��Ŀ¼����������Ŀ¼��Ŀ¼����ָӳ��
	 *                      ��������Ŀ¼��DSSĿ¼
	 * @param	const char* uid	- [in]
	 *          ָ��һ�����Ա����û���ݵ�id
	 * @param	bool keepalive	- [in]
	 *          ָ���������Ƿ���һ��ʹ��keepalive�ĳ����ӣ�Ĭ��Ϊtrue
	 */
	//DSAPI ds_handle ds_connect(const char* url, const char* uid, bool keepalive);
	typedef ds_handle (DSAPI *Proc_ds_connect)(const char* url, const char* uid, bool keepalive);

	/** 
	 * @brief 	ds_disconnect
	 *          �Ͽ��ֲ�ʽ�ļ�ϵͳ
	 * @return	int - Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *          ds_connect�������صľ�� 
	 */
	//DSAPI int ds_disconnect(ds_handle dsh);
	typedef int (DSAPI *Proc_ds_disconnect)(ds_handle dsh);

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
	 * @param	int bufferSize	- [in] 
	 *			ָ�����ļ�ʱ�������Ĵ�С��0��ʹ��Ĭ��ֵ
	 * @param	short replication	- [in] 
	 *			ָ���ļ���������Ŀ��0��ʹ��Ĭ��ֵ
	 * @param	size_t blocksize	- [in] 
	 *			ָ���ļ���Ĵ�С��0��ʹ��Ĭ��ֵ
	 */
	/*DSAPI DSFILE ds_fopen(ds_handle dsh, const char* path, int flags,
		int bufferSize, short replication, size_t blocksize);*/
	typedef DSFILE (DSAPI *Proc_ds_fopen)(ds_handle dsh, const char* path, int flags, int bufferSize, short replication, size_t blocksize);

	/** 
	 * @brief 	ds_fclose
	 *			�ر��ļ�
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			׼���رյ��ļ�ָ��	
	 */
    //DSAPI int ds_fclose(ds_handle dsh, DSFILE file);
	typedef int (DSAPI *Proc_ds_fclose)(ds_handle dsh, DSFILE file);

	/** 
	 * @brief 	ds_read
	 *			��ȡ�ļ�
	 * @return	size_t ������ʵ��ȡ���ֽ���������-1Ϊʧ�� 
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	void* buffer	- [in] 
	 *			��ȡ������
	 * @param	size_t length	- [in] 
	 *			����������
	 */
    //DSAPI size_t ds_read(ds_handle dsh,  DSFILE file, void* buffer, size_t length);
	typedef size_t (DSAPI *Proc_ds_read)(ds_handle dsh,  DSFILE file, void* buffer, size_t length);
	
	/** 
	 * @brief 	ds_write
	 *			д���ļ�
	 * @return	size_t ������ʵд����ֽ���������-1Ϊʧ��
	 *			����0Ϊд�뻺���������������0�������Ե�Ƭ�̺��ٴ�д��
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			�ļ�ָ��
	 * @param	const void* buffer	- [in] 
	 *			׼��д������ݻ���
	 * @param	size_t length	- [in] 
	 *			׼��д������ݻ��泤��
	 */
    //DSAPI size_t ds_write(ds_handle dsh, DSFILE file, const void* buffer, size_t length);
	typedef size_t (DSAPI *Proc_ds_write)(ds_handle dsh, DSFILE file, const void* buffer, size_t length);
	
	/**
	 * @brief 	ds_flush
	 *			��д�뻺���е�����flush���洢ϵͳ��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			�ļ�ָ��
	 */
    //DSAPI int ds_flush(ds_handle dsh, DSFILE file);
	typedef int (DSAPI *Proc_ds_flush)(ds_handle dsh, DSFILE file);

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
	 */
    //DSAPI int ds_seek(ds_handle dsh, DSFILE file, size_t offset);
	typedef int (DSAPI *Proc_ds_seek)(ds_handle dsh, DSFILE file, size_t offset);

	/** 
	 * @brief 	ds_tell
	 *			���ص�ǰ�ļ���offsetֵ����bytesΪ��λ
	 * @return	size_t ���ص�ǰ�ļ���offsetֵ������Ϊ-1
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	DSFILE file	- [in] 
	 *			�ļ�ָ��
	 */
    //DSAPI size_t ds_tell(ds_handle dsh, DSFILE file);
	typedef size_t (DSAPI *Proc_ds_tell)(ds_handle dsh, DSFILE file);

	/** 
	 * @brief 	ds_create_dir
	 *			����Ŀ¼������Ŀ¼��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			������[����]Ŀ¼·����֧�־���·�������·��	
	 * @param	const char* orignal_path	- [in] 
	 *			������Ŀ¼ʱ�ò�������NULL����������Ŀ¼ʱ�ò�����������Ŀ¼ӳ���ԴĿ¼	
	 */
    //DSAPI int ds_create_dir(ds_handle dsh, const char* path, const char* orignal_path);
	typedef int (DSAPI *Proc_ds_create_dir)(ds_handle dsh, const char* path, const char* orignal_path);

	/** 
	 * @brief 	ds_exists
	 * 			�ж�һ���ļ�/Ŀ¼����������Ŀ¼���Ƿ����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char *path	- [in] 
	 *			��֤��·������
	 */
    //DSAPI int ds_exists(ds_handle dsh, const char *path);
	typedef int (DSAPI *Proc_ds_exists)(ds_handle dsh, const char *path);

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
	 */
    //DSAPI int ds_delete(ds_handle dsh, const char* path, int recursive);
	typedef int (DSAPI *Proc_ds_delete)(ds_handle dsh, const char* path, int recursive);

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
    //DSAPI int ds_rename(ds_handle dsh, const char* old_path, const char* new_path);
	typedef int (DSAPI *Proc_ds_rename)(ds_handle dsh, const char* old_path, const char* new_path);

	/** 
	 * @brief 	ds_getworkingdir
	 *			���ǰ����Ŀ¼
	 * @return	char* ����ɹ�����ָ��buffer��ָ�룬���򷵻�NULL
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	char *buffer	- [in] 
	 *			������ŵ�ǰĿ¼�Ļ���
	 * @param	size_t length	- [in] 
	 *			���泤��	
	 */
    //DSAPI char* ds_getworkingdir(ds_handle dsh, char *buffer, size_t length);
	typedef char* (DSAPI *Proc_ds_getworkingdir)(ds_handle dsh, char *buffer, size_t length);

	/** 
	 * @brief 	ds_setworkingdir
	 *			���õ�ǰ����Ŀ¼
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			���õ�Ŀ¼·�������·��������ڵ�ǰ·������������·��ҲҪ����		
	 */
    //DSAPI int ds_setworkingdir(ds_handle dsh, const char* path);
	typedef int (DSAPI *Proc_ds_setworkingdir)(ds_handle dsh, const char* path);

	/** 
	 * @brief 	*ds_list_dir
	 *			�г�Ŀ¼��������Ŀ¼���е��ļ���Ϣ
	 * @return	ds_file_info �����ļ���Ϣ�����飬ʧ�ܷ���NULL
	 *			��������ϵͳ��̬�����裬�û���Ҫ�����ͷź����ͷ�
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			Ŀ¼��������Ŀ¼��·��
	 * @param	int *num	- [in] 
	 *			ds_file_info����
	 */
    //DSAPI ds_file_info *ds_list_dir(ds_handle dsh, const char* path, int *num);
	typedef ds_file_info (DSAPI **Proc_ds_list_dir)(ds_handle dsh, const char* path, int *num);

	/** 
	 * @brief 	*ds_path_info
	 *			�г��ļ�/Ŀ¼��������Ŀ¼�������ļ���Ϣ
	 * @return	ds_file_info �����ļ���Ϣ��ʧ�ܷ���NULL
	 *			����Ϣ��ϵͳ��̬�����裬�û���Ҫ�����ͷź����ͷ�
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			Ŀ¼��������Ŀ¼��·��
	 */
    //DSAPI ds_file_info *ds_path_info(ds_handle dsh, const char* path);
	typedef ds_file_info (DSAPI **Proc_ds_path_info)(ds_handle dsh, const char* path);

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
	//DSAPI void ds_free_file_info(ds_handle dsh, ds_file_info *fileinfo, int num);
	typedef void (DSAPI *Proc_ds_free_file_info)(ds_handle dsh, ds_file_info *fileinfo, int num);

	/** 
	 * @brief 	ds_find_change_notification
	 *			�����ļ��������Ѽ���ļ��޸�
	 * @return	ds_handle �ɹ�����һ�����Ѿ��
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			��Ҫ���ӵ��ļ�·�������·��Ŀǰֻ֧���ļ�
	 * @param	int watchsubtree	- [in] 
	 *			�Ƿ������Ŀ¼���ļ����������Ŀǰֻ��Ϊ�˺�Windows API����һ��
	 * @param	int filter	- [in] 
	 *			��ع��������������Ŀǰֻ��Ϊ�˺�Windows API����һ��
	 */
	//DSAPI ds_handle ds_find_change_notification(ds_handle dsh, const char* path, int watchsubtree, int filter);
	typedef ds_handle (DSAPI *Proc_ds_find_change_notification)(ds_handle dsh, const char* path, int watchsubtree, int filter);

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
	//DSAPI int ds_wait(ds_handle dsh, ds_handle handle, time_t ms);
	typedef int (DSAPI *Proc_ds_wait)(ds_handle dsh, ds_handle handle, time_t ms);

	/** 
	 * @brief 	ds_copyfile
	 *			�����ļ���֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* src	- [in] 
	 *			Դ·��
	 * @param	const char* dst	- [in] 
	 *			Ŀ��·��
	 */
    //DSAPI int ds_copyfile(ds_handle dsh, const char* src, const char* dst);
	typedef int (DSAPI *Proc_ds_copyfile)(ds_handle dsh, const char* src, const char* dst);

	/** 
	 * @brief 	ds_movefile
	 *			�ƶ��ļ���֧������·����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* src	- [in] 
	 *			Դ·��
	 * @param	const char* dst	- [in] 	
	 *			Ŀ��·��
	 */
    //DSAPI int ds_movefile(ds_handle dsh, const char* src, const char* dst);
	typedef int (DSAPI *Proc_ds_movefile)(ds_handle dsh, const char* src, const char* dst);

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
	//DSAPI int ds_from_local(ds_handle dsh, const char* localpath, const char* dst);
	typedef int (DSAPI *Proc_ds_from_local)(ds_handle dsh, const char* localpath, const char* dst);

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
	//DSAPI int ds_to_local(ds_handle dsh, const char* dst, const char* localpath);
	typedef int (DSAPI *Proc_ds_to_local)(ds_handle dsh, const char* dst, const char* localpath);

	/** 
	 * @brief 	ds_get_hosts
	 *			��DSSϵͳ�ϲ�ѯ�ļ����ֲ�������
	 * @return	char*** 
	 *			������Щ������������ַ�����û�з��ؿ�
	 * @param	ds_handle dsh	- [in] 
	 *			���ӵĴ洢ϵͳ���
	 * @param	const char* path	- [in] 
	 *			�ļ�·��	
	 */
    //char*** ds_get_hosts(ds_handle dsh, const char* path);
	typedef char*** (*Proc_ds_get_hosts)(ds_handle dsh, const char* path);

	/** 
	 * @brief 	ds_free_hosts
	 *			ɾ��ds_get_hosts�ļ�������������������ַ���ڴ�
	 * @return	No return value.
	 * @param	char ***hosts	- [in] 
	 *	        ds_get_hosts�ļ�������������������ַ���ڴ�
	 */
    //void ds_free_hosts(char ***hosts);
	typedef void (*Proc_ds_free_hosts)(char ***hosts);

#ifdef __cplusplus
}
#endif