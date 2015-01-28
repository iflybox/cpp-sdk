/** 
 * @file	iFlyPSP.h
 * @brief	Ѷ�ɸ��Ի��洢�ӿ�API
 * 
 * ���ļ�������Ѷ�ɸ��Ի��洢�ӿڵ�API����ԭ��
 * �����˸�API������������ͷ���ֵ���ͺ�ȡֵ��Χ
 *
 * @author	danzhang
 * @version	1.0
 * @date	2012-04-13
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2010-12-9	<td>danzhang	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __IFLYPSP_H__
#define __IFLYPSP_H__

#include "TOL.h"
#include "hdfsext.h"

#if defined(WIN32)
# if defined(PSP_EXPORTS)
#  define PSPAPI __declspec(dllexport)
# elif defined(PSP_STATIC)
#  define PSPAPI
#else
#  define PSPAPI __declspec(dllimport)
# endif
#else /* defined(WIN32) */
# define PSPAPI
#endif /* defined(WIN32) */

#define PSP_MAX_PATH					MAX_PATH
#define PSP_USRID_LENGTH				64
#define PSP_APPID_LENGTH				32
#define PSP_DEVID_LENGTH				256
//
///* �������� */
//const char* psp_svctype_mss				= "mss";
//const char* psp_svctype_ats				= "ats";
//const char* psp_svctype_nli				= "nli";
//
///* �û����Ի��������� */
//const char* psp_user_contact_org		= "contact_org";
//const char* psp_user_contact_nlp		= "contact_nlp";
//const char* psp_user_contact_hwb		= "contact_hwb";
//const char* psp_user_userword_org		= "userword_org";
//const char* psp_user_userword_hwb		= "userword_hwb";

	/* �ļ���Ϣ */
	typedef struct psp_file_info_t
	{
		char	filename[PSP_MAX_PATH];		/* �ļ���			*/
		time_t	lastmodified;				/* ����޸�ʱ��sec	*/
		uint64	size;						/* �ļ����� byte	*/
	} psp_file_info;

	/* �����Ϣ */
	typedef struct psp_quota_info_t
	{
		char	dirpath[PSP_MAX_PATH];		/* ��Ӧ��idssĿ¼	*/
		size_t	maxsize;					/* ������ܴ洢�ռ� */
		size_t	usedsize;					/* ���ÿռ�			*/
	} psp_quota_info;

	/* ���Ի��û���Ϣ */
	typedef struct psp_user_info_t
	{
		char usrid[PSP_USRID_LENGTH];
		char appid[PSP_APPID_LENGTH];
		char devid[PSP_DEVID_LENGTH];
	} psp_user_info;

#ifdef __cplusplus
	extern  "C" {
#endif

	/** 
	 * @brief 	PSP_DATA_MIGRATE_CB
	 *			�û�����Ǩ���첽��ʽ�µĻص�����,����֪ͨ�û�Ǩ���Ƿ�ɹ�
	 * @return	int	- �ɹ�����0�����򷵻�ʧ�ܴ���
	 * @param	const char* src_uid	- [in]
	 *          Ǩ��ǰ�û�id
	 * @param	const char* dst_uid	- [in]
	 *          Ǩ�ƺ��û�id
	 * @param	int result	- [in]
	 *          ��ʾǨ���Ƿ�ɹ���0ʱ�ɹ�������ʧ��
	 * @see		
	 */
	typedef int (*PSP_DATA_MIGRATE_CB)(void* cbdata, const char* src_uid, const char* dst_uid, int result);

	/** 
	 * @brief 	psp_initialize
	 *			��ʼ��ϵͳ�ĵ�һ������������Ҫ����һ��
	 * @return	int	- �ɹ�����0�����򷵻�ʧ�ܴ���
	 * @param	void* reserv	- [in]
	 *          �����Ժ�ʹ��
	 * @see		
	 */
	PSPAPI int psp_initialize(void* reserv);
	typedef int (*Proc_psp_initialize)(void* reserv);

	/** 
	 * @brief 	psp_uninitialize
	 *			���ʼ�������һ������
	 * @return	�޷���ֵ�����ú��ٿ���.
	 * @see		
	 */
	PSPAPI void psp_uninitialize(void);
	typedef void (*Proc_psp_uninitialize)(void);

	/** 
	 * @brief 	psp_read_comdata
	 *			��ȡds(�洢ϵͳ)��ĳһҵ���ļ���������
	 *			���ȡats�����wfst.SMS20110728.20111225.wbi2011.bin��������
	 * @return	int	- Return 0 in success, otherwise return error code.
	 *			�����ݳ�����󳤶ȣ����ض�Ӧ������
	 * @param	const char* svctype		- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds�ļ�������wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	void* outbuf			- [out] 
	 *			��ȡ������
	 * @param	size_t* bytes			- [in/out]
	 *			����������,������ȡ���������е����ݳ��� 
	 */
	PSPAPI int psp_read_comdata(const char* svctype, const char* dsfile, void* outbuf, size_t* bytes);
	typedef int (*Proc_psp_read_comdata)(const char* svctype, const char* dsfile, void* outbuf, size_t* bytes);

	/** 
	 * @brief 	psp_write_comdata
	 *			��Ż����������ݵ�ds��ĳҵ���£����ŵ�ats��
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds�ļ�������wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	const void* inbuf		- [in] 
	 *			׼��д������ݻ���
	 * @param	size_t bytes			- [in]
	 *			����������
	 */
	PSPAPI int psp_write_comdata(const char* svctype, const char* dsfile, const void* inbuf, size_t bytes);
	typedef int (*Proc_psp_write_comdata)(const char* svctype, const char* dsfile, const void* inbuf, size_t bytes);

	/** 
	 * @brief 	psp_readcomdata_lock
	 *			��ȡds��ĳһҵ���ļ���������,�����ڲ������ض������ܶԱ������ļ�����
	 *			���ȡats�����wfst.SMS20110728.20111225.wbi2011.bin��������
	 * @return	int	- Return 0 in success, otherwise return error code.
	 *			�����ݳ�����󳤶���ʧ�ܣ����ض�Ӧ������
	 * @param	const char* svctype		- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds�ļ�������wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	void* outbuf			- [out] 
	 *			��ȡ������
	 * @param	size_t* bytes			- [in/out]
	 *			���������ȣ�������ȡ���������е����ݳ��� 
	 */
	PSPAPI int psp_readcomdata_lock(const char* svctype, const char* dsfile, void* outbuf, size_t* bytes);
	typedef int (*Proc_psp_readcomdata_lock)(const char* svctype, const char* dsfile, void* outbuf, size_t* bytes);

	/** 
	 * @brief 	psp_writecomdata_lock
	 *			��Ż����������ݵ�ds��ĳҵ���£������ڲ������ض������ܶԱ������ļ�����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	const void* dsfile		- [in] 
	 *			ds�ļ�������wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	const void* inbuf		- [in] 
	 *			׼��д������ݻ���
	 * @param	size_t bytes			- [in]
	 *			����������
	 */
	PSPAPI int psp_writecomdata_lock(const char* svctype, const char* dsfile, const void* inbuf, size_t bytes);
	typedef int (*Proc_psp_writecomdata_lock)(const char* svctype, const char* dsfile, const void* inbuf, size_t bytes);

	/** 
	 * @brief 	psp_read_userdata
	 *			��ȡds���û����ݵ������������ȡ�û�a01003395��/wav/00000001.wav�ļ���������
	 * @return	int	- Return 0 in success, otherwise return error code.
	 *			�����ݳ�����󳤶���ʧ�ܣ����ض�Ӧ������
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id
	 *			·������˳��Ϊuid/appid/devid,�����û�id��Ӧ��id������һ����Ϊ��
	 * @param	const char* dsfile				- [in] 
	 *			ds�ļ���,��/wav/00000001.wav
	 * @param	void* outbuf					- [out] 
	 *			��ȡ������
	 * @param	size_t* bytes					- [in/out]
	 *			���������ȣ�������ȡ���������е����ݳ��� 
	 */
	PSPAPI int psp_read_userdata(const psp_user_info* userinfo, const char* dsfile, void* outbuf, size_t* bytes);
	typedef int (*Proc_psp_read_userdata)(const psp_user_info* userinfo, const char* dsfile, void* outbuf, size_t* bytes);

	/** 
	 * @brief 	psp_write_userdata
	 *			��Ż������е����ݵ�ds�϶�Ӧ�û��£����ŵ��û�a01003395�µ�/wav/00000001.wav�ļ�
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id
	 *			·������˳��Ϊuid/appid/devid,�����û�id��Ӧ��id������һ����Ϊ��
	 * @param	const char* dsfile				- [in] 
	 *			ds�ļ���,��/wav/00000001.wav
	 * @param	const void* inbuf				- [in] 
	 *			׼��д������ݻ���
	 * @param	size_t bytes					- [in]
	 *			����������
	 * @param	bool append						- [in]
	 *			�Ƿ���׷�ӷ�ʽд�ļ�
	 */
	PSPAPI int psp_write_userdata(const psp_user_info* userinfo, const char* dsfile, const void* inbuf, size_t bytes, bool append);
	typedef int (*Proc_psp_write_userdata)(const psp_user_info* userinfo, const char* dsfile, const void* inbuf, size_t bytes, bool append);

	/** 
	 * @brief 	psp_read_persondata
	 *			��ȡds���û��ĸ��Ի����ݵ������������ȡ�û�a01003395����ϵ�˴ʱ�Դ�ļ���������
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id
	 *			·������˳��Ϊuid/devid/appid,�����û�id��Ӧ��id������һ����Ϊ��
	 * @param	const void* datatype			- [in] 
	 *			�ļ������ͣ���PSP_USER_CONTACT_ORG
	 * @param	char* outbuf					- [out] 
	 *			��ȡ������
	 * @param	size_t* bytes					- [in/out]
	 *			���������ȣ�������ȡ���������е����ݳ���
	 */
	PSPAPI int psp_read_persondata(const psp_user_info* userinfo, const char* datatype, void* outbuf, size_t* bytes);
	typedef int (*Proc_psp_read_persondata)(const psp_user_info* userinfo, const char* datatype, void* outbuf, size_t* bytes);

	/** 
	 * @brief 	psp_write_persondata
	 *			��Ż��������û��ĸ��Ի����ݵ�ds���罫���������û�a01003395����ϵ�˴ʱ�Դ�ļ����ݴ�ŵ�ds
	 * @return	int	- Return 0 in success, otherwise return error code.
	 *			�����ݳ�����󳤶���ʧ�ܣ����ض�Ӧ������
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id
	 *			·������˳��Ϊuid/appid/devid,�����û�id��Ӧ��id������һ����Ϊ��
	 * @param	const char* datatype			- [in] 
	 *			�ļ������ͣ���PSP_USER_CONTACT_ORG
	 * @param	const void* inbuf				- [in] 
	 *			׼��д������ݻ���
	 * @param	size_t bytes					- [in]
	 *			����������
	 */
	PSPAPI int psp_write_persondata(const psp_user_info* userinfo, const char* datatype, const void* outbuf, size_t bytes);
	typedef int (*Proc_psp_write_persondata)(const psp_user_info* userinfo, const char* datatype, const void* outbuf, size_t bytes);

	/** 
	 * @brief 	psp_list_comfile
	 *			��ȡds��ҵ���ļ��б���Ϣ�����ȡds�ϴ�ŵ�ats���ļ��б�
	 * @return	psp_file_info �����ļ���Ϣ�����飬ʧ�ܷ���NULL
	 *			��������ϵͳ��̬���䣬�û���Ҫ�����ͷź����ͷ�
	 * @param	const char* svctype		- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	int* filecount			- [out] 
	 *			psp_file_info����
	 * @param	int* errnum				- [out] 
	 *			�����룬0��ʾ�ɹ�
	 */
	PSPAPI psp_file_info* psp_list_comfile(const char* svctype, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_comfile)(const char* svctype, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_list_userfile
	 *			��ȡds��ĳ�û��ļ��б���Ϣ�������������ļ� 
	 * @return	psp_file_info �����ļ���Ϣ�����飬ʧ�ܷ���NULL
	 *			��������ϵͳ��̬���䣬�û���Ҫ�����ͷź����ͷ�
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id�������û�id��Ӧ��id������һ����Ϊ��
	 * @param	int* filecount			- [out] 
	 *			psp_file_info����
	 * @param	int* errnum				- [out] 
	 *			�����룬0��ʾ�ɹ�
	 */
	PSPAPI psp_file_info* psp_list_userfile(const psp_user_info* userinfo, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_userfile)(const psp_user_info* userinfo, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_list_userbakfile
	 *			��ȡds��ĳ�û������ļ��б���Ϣ
	 * @return	psp_file_info �����ļ���Ϣ�����飬ʧ�ܷ���NULL
	 *			��������ϵͳ��̬���䣬�û���Ҫ�����ͷź����ͷ�
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id�������û�id��Ӧ��id������һ����Ϊ��
	 * @param	int* filecount					- [out] 
	 *			psp_file_info����
	 * @param	int* errnum						- [out] 
	 *			�����룬0��ʾ�ɹ�
	 */
	PSPAPI psp_file_info* psp_list_userbakfile(const psp_user_info* userinfo, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_userbakfile)(const psp_user_info* userinfo, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_list_personfile
	 *			��ȡ�û����Ի������ļ��б���Ϣ�������������ļ������ȡ�û�a01003395���ȴ�Դ�ļ���Ϣ
	 * @return	psp_file_info �����ļ���Ϣ�����飬ʧ�ܷ���NULL
	 *			��������ϵͳ��̬���䣬�û���Ҫ�����ͷź����ͷ�
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id�������û�id��Ӧ��id������һ����Ϊ��
	 * @param	const char* datatype			- [in] 
	 *			���Ի��������ͣ���PSP_USER_USRWORD_ORG
	 * @param	int* filecount					- [out] 
	 *			psp_file_info����
	 * @param	int* errnum						- [out] 
	 *			�����룬0��ʾ�ɹ�
	 */
	PSPAPI psp_file_info* psp_list_personfile(const psp_user_info* userinfo, const char* datatype, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_personfile)(const psp_user_info* userinfo, const char* datatype, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_list_personbakfile
	 *			��ȡ�û����Ի����ݵı����ļ��б���Ϣ,���ȡ�û�a01003395���ȴ�Դ�ļ��ı����ļ��б�
	 * @return	psp_file_info �����ļ���Ϣ�����飬ʧ�ܷ���NULL
	 *			��������ϵͳ��̬���䣬�û���Ҫ�����ͷź����ͷ�
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id�������û�id��Ӧ��id������һ����Ϊ��
	 * @param	const char* datatype			- [in] 
	 *			���Ի��������ͣ���PSP_USER_CONTACT_ORG
	 * @param	int* filecount					- [out] 
	 *			psp_file_info����
	 * @param	int* errnum						- [out] 
	 *			�����룬0��ʾ�ɹ�
	 */
	PSPAPI psp_file_info* psp_list_personbakfile(const psp_user_info* userinfo, const char* datatype, int* filecount, int* errcode);
	typedef psp_file_info* (*Proc_psp_list_personbakfile)(const psp_user_info* userinfo, const char* datatype, int* filecount, int* errcode);

	/** 
	 * @brief 	psp_free_file_info
	 *			�ͷ�psp_file_info��Ϣ
	 * @return	No return value.
	 * @param	const psp_file_info *fileinfo	- [in] 
	 *			psp_file_info������
	 */
	PSPAPI void psp_free_file_info(const psp_file_info *fileinfo);
	typedef void (*Proc_psp_free_file_info)(const psp_file_info *fileinfo);

	/** 
	 * @brief 	psp_com_quotainfo
	 *			��ȡҵ�������Ϣ
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype			- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	psp_quota_info* quotinfo	- [in/out] 
	 *			�����Ϣ
	 */
	PSPAPI int psp_com_quotainfo(const char* svctype, psp_quota_info* quotinfo);
	typedef int (*Proc_psp_com_quotainfo)(const char* svctype, psp_quota_info* quotinfo);

	/** 
	 * @brief 	psp_user_quotainfo
	 *			��ȡ�û������Ϣ
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* uid		- [in] 
	 *			�û�id 
	 * @param	psp_quota_info* quotinfo	- [in/out] 
	 *			�����Ϣ
	 */
	PSPAPI int psp_user_quotainfo(const char* uid, psp_quota_info* quotinfo);
	typedef int (*Proc_psp_user_quotainfo)(const char* uid, psp_quota_info* quotinfo);

	/** 
	 * @brief 	psp_migrate_userdata
	 *			��ע���û���ע���û�������Ǩ�� 
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* src_uid		- [in] 
	 *			ע��ǰ�û�id
	 * @param	const char* dst_uid		- [in] 
	 *			ע����û�id
	 * @param	PSP_DATA_MIGRATE fn		- [in] 
	 *			�û�����Ǩ���첽��ʽ�µĻص�����
	 */
	PSPAPI int psp_migrate_userdata(const char* src_uid, const char* dst_uid, PSP_DATA_MIGRATE_CB fn, void* cbdata);
	typedef int (*Proc_psp_migrate_userdata)(const char* src_uid, const char* dst_uid, PSP_DATA_MIGRATE_CB fn, void* cbdata);

	/** 
	 * @brief 	psp_get_comfile
	 *			��ȡds(�洢ϵͳ)��ĳһҵ���ļ�������
	 *			�縴��ats�����wfst.SMS20110728.20111225.wbi2011.bin�������ļ�
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds�ļ�������wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	const char* localfile	- [in] 
	 *			����·��
	 */
	PSPAPI int psp_get_comfile(const char* svctype, const char* dsfile, const char* localfile);
	typedef int (*Proc_psp_get_comfile)(const char* svctype, const char* dsfile, const char* localfile);

	/** 
	 * @brief 	psp_put_comfile
	 *			��ű����ļ���ds��ĳҵ����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	const char* localfile	- [in] 
	 *			�����ļ�·��
	 * @param	const char* dsfile		- [in] 
	 *			ds�ļ�������wfst.SMS20110728.20111225.wbi2011.bin
	 */
	PSPAPI int psp_put_comfile(const char* svctype, const char* localfile, const char* dsfile);
	typedef int (*Proc_psp_put_comfile)(const char* svctype, const char* localfile, const char* dsfile);

	/** 
	 * @brief 	psp_getcomfile_lock
	 *			��ȡds��ĳһҵ���ļ�������,�����ڲ������ض������ܶԱ������ļ�����
	 *			�縴��ats�����wfst.SMS20110728.20111225.wbi2011.bin�������ļ�
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	const char* dsfile		- [in] 
	 *			ds�ļ�������wfst.SMS20110728.20111225.wbi2011.bin
	 * @param	const char* localfile	- [in] 
	 *			����·��
	 */
	PSPAPI int psp_getcomfile_lock(const char* svctype, const char* dsfile, const char* localfile);
	typedef int (*Proc_psp_getcomfile_lock)(const char* svctype, const char* dsfile, const char* localfile);

	/** 
	 * @brief 	psp_putcomfile_lock
	 *			��ű����ļ���ds��ĳҵ���£������ڲ������ض������ܶԱ������ļ�����
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char* svctype		- [in] 
	 *			ҵ�����ͣ���PSP_SVCTYPE_ATS
	 * @param	const char* localfile	- [in] 
	 *			�����ļ�·��
	 * @param	const char* dsfile		- [in] 
	 *			ds�ļ�������wfst.SMS20110728.20111225.wbi2011.bin
	 */
	PSPAPI int psp_putcomfile_lock(const char* svctype, const char* localfile, const char* dsfile);
	typedef int (*Proc_psp_putcomfile_lock)(const char* svctype, const char* localfile, const char* dsfile);

	/** 
	 * @brief 	psp_get_userfile
	 *			��ȡds���û��ļ������أ��縴���û�a01003395��/wav/00000001.wav�ļ�������
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id
	 *			·������˳��Ϊuid/appid/devid,�����û�id��Ӧ��id������һ����Ϊ��
	 * @param	const char* dsfile				- [in] 
	 *			ds�ļ���,��/wav/00000001.wav
	 * @param	const char* localfile			- [in] 
	 *			����·��
	 */
	PSPAPI int psp_get_userfile(const psp_user_info* userinfo, const char* dsfile, const char* localfile);
	typedef int (*Proc_psp_get_userfile)(const psp_user_info* userinfo, const char* dsfile, const char* localfile);

	/** 
	 * @brief 	psp_put_userfile
	 *			��ű����ļ���ds�϶�Ӧ�û��£��縴�Ƶ��û�a01003395�µ�/wav/00000001.wav�ļ�
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id
	 *			·������˳��Ϊuid/appid/devid,�����û�id��Ӧ��id������һ����Ϊ��
	 * @param	const char* localfile			- [in] 
	 *			����·��
	 * @param	const char* dsfile				- [in] 
	 *			ds�ļ���,��/wav/00000001.wav
	 */
	PSPAPI int psp_put_userfile(const psp_user_info* userinfo, const char* localfile, const char* dsfile);
	typedef int (*Proc_psp_put_userfile)(const psp_user_info* userinfo, const char* localfile, const char* dsfile);

	/** 
	 * @brief 	psp_get_personfile
	 *			��ȡds���û��ĸ��Ի����ݵ������ļ� �����ȡ�û�a01003395����ϵ�˴ʱ�Դ�ļ��������ļ�
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id
	 *			·������˳��Ϊuid/appid/devid,�����û�id��Ӧ��id������һ����Ϊ��
	 * @param	const char* datatype			- [in] 
	 *			�ļ������ͣ���PSP_USER_CONTACT_ORG
	 * @param	const char* localfile			- [in] 
	 *			����·��
	 */
	PSPAPI int psp_get_personfile(const psp_user_info* userinfo, const char* datatype, const char* localfile);
	typedef int (*Proc_psp_get_personfile)(const psp_user_info* userinfo, const char* datatype, const char* localfile);

	/** 
	 * @brief 	psp_put_personfile
	 *			���û��ĸ��Ի����ݴӱ��ظ��Ƶ�ds���罫�û�a01003395����ϵ�˴ʱ�Դ�ļ����Ƶ�ds
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const psp_user_info* userinfo	- [in] 
	 *			�û���Ϣ�������û�id��Ӧ��id���豸id
	 *			·������˳��Ϊuid/appid/devid,�����û�id��Ӧ��id������һ����Ϊ��
	 * @param	const char* datatype			- [in] 
	 *			�ļ������ͣ���PSP_USER_CONTACT_ORG
	 * @param	const char* localfile			- [in] 
	 *			����·��
	 */
	PSPAPI int psp_put_personfile(const psp_user_info* userinfo, const char* datatype, const char* localfile);
	typedef int (*Proc_psp_put_personfile)(const psp_user_info* userinfo, const char* datatype, const char* localfile);

#ifdef __cplusplus

}
#endif
#endif	 //__IFLYPSP_H__