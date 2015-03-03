
#ifndef __CSSP__CSSP_H__
#define __CSSP__CSSP_H__

#include <stdint.h>

#define UPLOADID_LEN	32

//cssp�������
typedef void* CSSPHandle;
typedef void* MetadataHandle;
typedef void* CSSPResult;
typedef MetadataHandle ContainerMetaHandle;
typedef MetadataHandle ObjectMetaHandle;
typedef char UploadId[UPLOADID_LEN + 1];

#define CSSP_INVALID_HANDLE  NULL

#if defined(WIN32)
# if defined(IFLY_EXPORTS)
#  define IFLYAPI __declspec(dllexport)
# elif defined(IFLY_STATIC)
#  define IFLYAPI
#else
#  define IFLYAPI __declspec(dllimport)
# endif
#else /* defined(WIN32) */
# define IFLYAPI
#endif /* defined(WIN32) */

#ifdef __cplusplus
extern  "C" {
#endif

//************************************
// method:	cssp_init_result
// brief:	����ִ��״̬�����ṹ
// access:	public  
// returns:	IFLYAPI CSSPResult - 
// author:	zhengyao
// remarks:	CSSP������API�ӿ���CSSPResult���ڷ���ִ��״̬��ϸ����
//************************************
IFLYAPI CSSPResult cssp_init_result();
typedef CSSPResult (*Proc_cssp_init_result)();

//************************************
// method:	cssp_release_result
// brief:	�ͷ�ִ��״̬�ṹ��
// access:	public 
// returns:	IFLYAPI void - 
// param:	CSSPResult	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI void cssp_release_result(CSSPResult);
typedef void (*Proc_cssp_release_result)(CSSPResult);


//************************************
// method:	cssp_result_statuscode
// brief:	��ȡresult�ṹ�е�statuscode
// access:	public  
// returns:	IFLYAPI int - 
// param:	CSSPResult	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_result_statuscode(CSSPResult);
typedef int (*Proc_cssp_result_statuscode)(CSSPResult);

//************************************
// method:	cssp_is_successful
// brief:	�ж�״̬���Ƿ�ɹ�
// access:	public  
// returns:	IFLYAPI bool - 
// param:	int	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_is_successful(int);
typedef int (*Proc_cssp_is_successful)(int);

//************************************
// method:	cssp_result_detail
// brief:	��ȡresult�е���ϸ��Ϣ
// access:	public  
// returns:	IFLYAPI const char* - 
// param:	CSSPResult	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI const char* cssp_result_detail(CSSPResult);
typedef const char* (*Proc_cssp_result_detail)(CSSPResult);

//************************************
// method:  cssp_create_instance
// brief:   ����csspʵ��
// access:  public 
// returns: CSSPHandle  - �ɹ����ش洢�����ʧ�ܷ���CSSP_INVALID_HANDLE.
// param:   const char* endpoint - [in]
//          cssp�����endpoint
// param:   const char* accessKeyId - [in]
//          ����cssp��Access Key ID
// param:   const char* accessKeySecret	- [in]
//          ����cssp��Access Key Secret
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI CSSPHandle cssp_create_instance(const char* endpoint, const char* accessKeyId, const char* accessKeySecret, CSSPResult result);
typedef CSSPHandle (*Proc_cssp_create_instance)(const char* endpoint, const char* accessKeyId, const char* accessKeySecret, CSSPResult result);


//************************************
// method:  cssp_release_instance
// brief:   �ͷ�csspʵ��
// access:  public 
// returns: int - return 0 in success, otherwise return error code.
// param:   CSSPHandle hdl  - [in] 
//          �洢���
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI int cssp_release_instance(CSSPHandle hdl);
typedef int (*Proc_cssp_release_instance)(CSSPHandle hdl);

//************************************
// method:  cssp_set_timeout
// brief:   ���ý�����ʱʱ��
// access:  public 
// returns: int - ����֮ǰ�ĳ�ʱʱ��
// param:   CSSPHandle hdl  - [in] 
//          �洢���
//			int	timeout		- [in]
//			��ʱʱ�䣨���룩
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI int cssp_set_timeout(CSSPHandle hdl, int timeout);
typedef int (*Proc_cssp_set_timeout)(CSSPHandle hdl);




//************************************
// method:	cssp_list_object
// brief:	ö��container��object
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl    - [in]
//          �洢���
// param:	ObjectMetaHandle * metahdls	- [in,out]
//          ��ָ��ָ��Ľṹ����������䷵�ص�ObejctMetaHandle
// param:	int * limit	- [in,out]
//          �û�����ʱ���Ϊ�ô���Ҫö�ٵ�Object����(����ֵ���ܳ���metahdls���ɴ洢����)
//          ����ִ�к���ֶ����Ϊʵ�ʶ�ȡ��ObjectMeta����
//			**ע�⣺limit���������10000
// param:	const char * prefix	- [in]
//          Object����ǰ׺��ö������)
// param:	const char * delimiter	- [in]
//          Object���Ʒָ�����ö�����������prefixʹ�ã�
// param:	const char * marker	- [in]
//          Object���ƣ����ڷֶ�ö�ٵķָ��,�������Ӹ�Object����һ����ʼö��
//          ��һ��ö��ʱ����NULL���Ժ�ÿ�ξ������ϴη��ص�metahdls�����һ��Object����
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	1.�ú������ڷֶ�ö��container��object,�����ص�
//          metahdlcnt������ֵС�ڴ�����ֵ��ʾ����object��ȡ���
//          ע�⣺metahdls����Ϣ����ʹ��ʱ����ҪΪÿ��ObjectMetaHandle����cssp_objectmeta_release�ͷ���Դ
//          2.prefix��delimiter���ʹ��ʱʵ�ֶ�ָ����Ŀ¼������ö��
//************************************
IFLYAPI int cssp_list_object(const CSSPHandle hdl, ObjectMetaHandle* metahdls, int* limit, const char* prefix, const char* delimiter, const char* marker, CSSPResult result);
typedef int (*Proc_cssp_list_object)(const CSSPHandle hdl, ObjectMetaHandle* metahdls, int* limit, const char* prefix, const char* delimiter, const char* marker, CSSPResult result);


//************************************
// method:  cssp_count_object
// brief:   ��ȡContainer��object����
// access:  public 
// returns: int - ����HTTP״̬��.
// param:	const CSSPHandle hdl    - [in]
//          �洢���
// param:	uint64_t* count    - [in,out]
//          ����container��object��
// param:	CSSPResult result    - [in]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks: objectCountΪContainer��system define metadata
//************************************
IFLYAPI int cssp_count_object(const CSSPHandle hdl, uint64_t* count, CSSPResult result);
typedef int (*Proc_cssp_count_object)(const CSSPHandle hdl, uint64_t* count, CSSPResult result);

//************************************
// method:  cssp_count_bytesused
// brief:   ��ȡContainer��object��ǰ�洢����
// access:  public 
// returns: int - ����HTTP״̬��.
// param:	const CSSPHandle hdl    - [in]
//          �洢���
// param:	uint64_t* bytesused    - [in,out]
//          ����container���ֽ���
// param:	CSSPResult result    - [in]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks: byteUsedΪContainer��system define metadata
//************************************
IFLYAPI int cssp_count_bytesused(const CSSPHandle hdl, uint64_t* bytesused, CSSPResult result);
typedef int (*Proc_cssp_count_bytesused)(const CSSPHandle hdl, uint64_t* count, CSSPResult result);


/*************************************************************************/
/************************Storage Object Interfaces*********************/
/*************************************************************************/


//************************************
// method:  cssp_object_exist
// brief:   ָ��object�Ƿ����
// access:  public 
// returns: int - ����HTTP״̬��.
// param:   const CSSPHandle hdl  - [in]
//          �洢���  
// param:   const char * object - [in]
//          Object����
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks: 
//************************************
IFLYAPI int cssp_object_exist(const CSSPHandle hdl, const char* object, CSSPResult result);
typedef int (*Proc_cssp_object_exist)(const CSSPHandle hdl, const char* object, CSSPResult result);

//************************************
// method:	cssp_put_object_callback_ptr
// brief:	�ϴ��ļ��ص�����
// access:	public  
// returns:	size_t	- ����ֵָ���ôλص���ȡ���ϴ������ֽ�����Ϊ0���ʾ�ϴ����ݻ�ȡ���
// param:	void* buffer	- [in]
//			�ϴ�ϵͳ������(�ص������У���Ҫ�����ݿ������û�����)
// param:	size_t size	- [in]
//			
// param:	size_t nmemb	- [in]
//			��size��ϣ�size*nmemb��ʾϵͳ������buffer�Ĵ�С��
// param:	void* inputstream	- [in]
//			����cssp_put_objectʱ�����inputstream
//			inputstream����߱��������ԣ�
//				1.ͨ��inputstream���Ի�ȡ���ϴ�����
//				2.ͨ��inputstream���Ի�ȡ���ϴ����ݵı߽�(����)
// author:	zhengyao
// remarks:	
//************************************
typedef size_t (*cssp_put_object_callback_ptr)(void* buffer, size_t size, size_t nmemb, void* inputstream);
IFLYAPI size_t cssp_put_object_fromdisk_callback(void* buffer, size_t size, size_t nmemb, void* inputstream);
typedef size_t (*Proc_cssp_put_object_fromdisk_callback)(void* buffer, size_t size, size_t nmemb, void* inputstream);

//************************************
// method:  cssp_put_object
// brief:   �ϴ�object
// access:  public 
// returns: int - ����HTTP״̬��.
// param:   const CSSPHandle hdl  - [in]
//          �洢���
// param:   const char * object	- [in]
//          Object����
// param:   cssp_put_object_callback putObjectCallback - [in]
//          �ϴ��ص�����(�������NULL��������Ĭ�ϻص�������inputstream��sizeָ�������ϴ�)
// param:   void * inputstream  - [in]
//          Object���ݣ����putObjectCallback�ص����룬�˲����ᱻ����putObjectCallback�ص��У�
// param:   uint64_t size - [in]
//          Object���ݳ��ȣ����putObjectCallback�ص��������룬��size�������ԣ�
// param:   const char* md5 - [in]
//          Object���ݵ�md5.������NULL�򲻽�������У��;���������У��ֵ�͸�md5�������򷵻�422״̬��
// param:   ObjectMetaHandle outmetadata - [out]
//          �����ϴ������ݵ�metadata��Ϣ(ע��Content-Length�ֶβ�����)
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks: �û�����putObjectCallback�ص���cssp��ͨ�����øûص�����ȡ�ϴ����ݣ�����μ�cssp_put_object_callback����
//************************************
IFLYAPI int cssp_put_object(const CSSPHandle hdl, const char* object, cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, ObjectMetaHandle outmetadata, CSSPResult result);
typedef int (*Proc_cssp_put_object)(const CSSPHandle hdl, const char* object, cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, ObjectMetaHandle outmetadata, CSSPResult result);


//************************************
// method:	cssp_get_object_callback_ptr
// brief:	�����ļ��ص�����
// access:	public  
// returns:	size_t	- ָ���ôλص���ȡ�����������ֽ������������ֵ������size*nmemb���ʾ�������ؽ�����ֹ
// param:	void* buffer	- [in]
//			����ϵͳ������(�ص������У���Ҫ�û����������ݿ������û�������)
// param:	size_t size	- [in]
//			
// param:	size_t nmemb	- [in]
//			��size��ϣ�size*nmemb��ʾ�ôλص������ֽ�����
// param:	void* outputstream	- [in]
//			����cssp_get_objectʱ�����outputstream
// author:	zhengyao
// remarks:	
//************************************
typedef size_t (*cssp_get_object_callback_ptr)(void* buffer, size_t size, size_t nmemb, void* outputstream);

//************************************
// method:	cssp_get_object_todisk_callback
// brief:	�ṩ�������صĻص�����
// access:	public  
// returns:	size_t - 
// param:	void * buffer	- [in]
// param:	size_t size	- [in]
// param:	size_t nmemb	- [in]
// param:	void * outputstream	- [in]
//			FILE*�ļ���������û�ͨ��cssp_get_object����(����ʱ��cssp����������ص���)
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI size_t cssp_get_object_todisk_callback(void* buffer, size_t size, size_t nmemb, void* outputstream);
typedef size_t (*Proc_cssp_get_object_todisk_callback)(void* buffer, size_t size, size_t nmemb, void* outputstream);

//************************************
// method:  cssp_get_object
// brief:   ����object
// access:  public 
// returns: int - ����HTTP״̬��.
// param:   const CSSPHandle hdl    - [in]
//          �洢���
// param:   const char * object - [in]
//          Object����
// param:   cssp_get_object_callback getObjectCallback - [in]
//          ���ػص�����(�������NULL��������Ĭ�ϻص��������������غ�д��outputstream��sizeָ����������)
// param:   void * outputstream  - [in,out]
//          �û����ڽ����������ݵ��Զ���ṹ(���getObjectCallback����ΪNULL����ò�������Ϊ�ڴ�buffer)
// param:   uint64_t offset - [in]
//          ������ʼƫ��
// param:   uint64_t * size - [in,out]
//          ����Ϊ�������صĳ���
//          ����Ϊ��ʵ���صĳ���
//			ע�⣺�˴�*size < 0 ���ʾ�û���һ������ȫ���ļ������յ�buffer��С���û���֤
//				���ǽ���ֻ�е��û�����getObjectCallback����ʱ�Ŵ���*size < 0��ȫ�����ز���
// param:   ObjectMetaHandle outmetadata - [out]
//          �����ϴ������ݵ�metadata��Ϣ
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks: �ýӿ������û��ֶ�����
//************************************
IFLYAPI int cssp_get_object(const CSSPHandle hdl, const char* object, cssp_get_object_callback_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t* size, ObjectMetaHandle outmetadata,CSSPResult result);
typedef int (*Proc_cssp_get_object)(const CSSPHandle hdl, const char* object, cssp_get_object_callback_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t* size, ObjectMetaHandle outmetadata,CSSPResult result);

//************************************
// method:	cssp_remove_object
// brief:	�Ƴ�object
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl	- [in]
//          �洢���
// param:	const char * object	- [in]
//          Object����
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	 
//************************************
IFLYAPI int cssp_remove_object(const CSSPHandle hdl, const char* object, CSSPResult result);
typedef int (*Proc_cssp_remove_object)(const CSSPHandle hdl, const char* object, CSSPResult result);

//************************************
// method:	cssp_copy_object
// brief:	����object
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl	- [in]
//			�洢���
// param:	const char * srcObject	- [in]
//          Դobject����
// param:	const char * dstContainer	- [in]
//          Ŀ��container����
// param:	const char * dstObject	- [in]
//          Ŀ��object����
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_copy_object(const CSSPHandle hdl, const char* srcObject, const char* dstContainer, const char* dstObject, CSSPResult result);
typedef int (*Proc_cssp_copy_object)(const CSSPHandle hdl, const char* srcObject, const char* dstContainer, const char* dstObject, CSSPResult result);


//************************************
// method:	cssp_move_object
// brief:	�ƶ�object
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl	- [in]
//			�洢���
// param:	const char * srcObject	- [in]
//          Դobject����
// param:	const char * dstContainer	- [in]
//          Ŀ��container����
// param:	const char * dstObject	- [in]
//          Ŀ��object����
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_move_object(const CSSPHandle hdl, const char* srcObject, const char* dstContainer, const char* dstObject, CSSPResult result);
typedef int (*Proc_cssp_move_object)(const CSSPHandle hdl, const char* srcObject, const char* dstContainer, const char* dstObject, CSSPResult result);


//************************************
// method:	cssp_set_objectmeta
// brief:	����object metadata
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl	- [in]
//          �洢���
// param:	const char * object	- [in]
//          Object����
// param:	const ObjectMetaHandle metadata	- [in]
//          ������Object metadata��Ϣ
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_set_objectmeta(const CSSPHandle hdl, const char* object, const ObjectMetaHandle metadata, CSSPResult result);
typedef int (*Proc_cssp_set_objectmeta)(const CSSPHandle hdl, const char* object, const ObjectMetaHandle metadata, CSSPResult result);

//************************************
// method:	cssp_get_objectmeta
// brief:	��ȡobject metadata
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl	- [in]
//          �洢���
// param:	const char * object	- [in]
//          Object����
// param:	ObjectMetaHandle  metadata	- [in,out]
//          ����ObjectMeta
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_get_objectmeta(const CSSPHandle hdl, const char* object, ObjectMetaHandle metadata, CSSPResult result);
typedef int (*Proc_cssp_get_objectmeta)(const CSSPHandle hdl, const char* object, ObjectMetaHandle metadata, CSSPResult result);


//************************************
// method:	cssp_objectmeta_is_pseudodirectory
// brief:	�Ƿ�Ϊ����Ŀ¼
// access:	public 
// returns:	bool	- return true if owner is a Pseudo-Directory, otherwise return false(owner is a object).
// param:	const ObjectMetaHandle metahandle	- [in]
//          ObjectMeta���
// author:	zhengyao
// remarks:	���Ϊ����Ŀ¼����Ӧ�ü�����ȡ����Metadata��Ϣ,����Ŀ¼û��metadata��Ϣ
//************************************
IFLYAPI bool cssp_objectmeta_is_pseudodirectory(const ObjectMetaHandle metahandle);
typedef bool (*Proc_cssp_objectmeta_is_pseudodirectory)(const ObjectMetaHandle metahandle);

//************************************
// method:	cssp_objectmeta_contentlength
// brief:	Object����
// access:	public 
// returns:	uint64_t    - return object content length.
// param:	const ObjectMetaHandle metahandle	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI uint64_t cssp_objectmeta_contentlength(const ObjectMetaHandle metahandle);
typedef uint64_t (*Proc_cssp_objectmeta_contentlength)(const ObjectMetaHandle metahandle);

//************************************
// method:	cssp_objectmeta_etag
// brief:	object��Etagֵ
// access:	public 
// returns:	const char*	- return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const ObjectMetaHandle metahandle	- [in]
// author:	zhengyao
// remarks:	EtagĬ��Ϊmd5
//************************************
IFLYAPI const char* cssp_objectmeta_etag(const ObjectMetaHandle metahandle);
typedef const char* (*Proc_cssp_objectmeta_etag)(const ObjectMetaHandle metahandle);

//************************************
// method:	cssp_objectmeta_lastmodify
// brief:	object Last Modifyʱ��
// access:	public  
// returns:	const char* - return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const ObjectMetaHandle metaHandle	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI const char* cssp_objectmeta_lastmodify(const ObjectMetaHandle metahandle);
typedef const char* (*Proc_cssp_objectmeta_lastmodify)(const ObjectMetaHandle metahandle);

/*****************************************************************************/
/*************************Multipart Upload interfaces*************************/
/*****************************************************************************/

//************************************
// method:	cssp_multipart_upload_init
// brief:	��ʼ���ֶ��ϴ�
// access:	public 
// returns:	int	- return 200 in success, otherwise return 1.
// param:	const CSSPHandle hdl	- [in]
//          �洢���
// param:	const char * object	- [in]
//          object����
// param:	UploadId upid	- [in,out]
//          ����һ������Ϊ32λ��upload id(�û����봫��һ��33�ֽڵ�����)
// author:	zhengyao
// remarks:	�����ֶ��ϴ�����ͨ��UploadId����
//************************************
IFLYAPI int cssp_multipart_upload_init(const CSSPHandle hdl, const char* object, UploadId upid);
typedef int (*Proc_cssp_multipart_upload_init)(const CSSPHandle hdl, const char* object, UploadId upid);

//************************************
// method:	cssp_multipart_upload_part
// brief:	�ϴ��ֶ�
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl	- [in]
//          �洢���
// param:	const UploadId upid	- [in]
//          �ֶ��ϴ�id
// param:	int partnumber	- [in]
//          �ֶα��
// param:   cssp_put_object_callback putObjectCallback - [in]
//          �ϴ��ص�����(�������NULL��������Ĭ�ϻص�������inputstream��sizeָ�������ϴ�)
// param:   void * inputstream  - [in]
//          Object���ݣ����putObjectCallback�ص����룬�˲����ᱻ����putObjectCallback�ص��У�
// param:   uint64_t size - [in]
//          Object���ݳ��ȣ����putObjectCallback�ص��������룬��size�������ԣ�
// param:	const char* md5	- [in]
//          �ֶ�md5,����NULL�򲻽��зֶ�����У��,��������У�����ݲ����ڸ�md5���򷵻�422״̬��
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	������ͬ��partnumber������֮ǰ�ĸ÷ֶ�
//************************************
IFLYAPI int cssp_multipart_upload_part(const CSSPHandle hdl, const UploadId upid, int partnumber,  cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, CSSPResult result);
typedef int (*Proc_cssp_multipart_upload_part)(const CSSPHandle hdl, const UploadId upid, int partnumber,  cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, CSSPResult result);

//************************************
// method:	cssp_multipart_upload_list_parts
// brief:	ö�ٵ�ǰ���ϴ��ֶ�
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl    - [in]
//          �洢���
// param:	const UploadId upid	- [in]
//          �ֶ��ϴ�id
// param:	ObjectMetaHandle * metahdls	- [in,out]
//          ��ָ��ָ��Ľṹ����������䷵�ص�ObjectMetaHandle
// param:	int * limit	- [in,out]
//          �û�����ʱ���Ϊ�ô���Ҫö�ٵķֶθ���(����ֵ���ܳ���metahdls���ɴ洢����)
//          ����ִ�к���ֶ����Ϊʵ�ʶ�ȡ�ķֶθ���
// param:	const char * marker	- [in]
//          �ֶ����ƣ����ڷֶ�ö�ٵķָ��,�������Ӹ÷ֶε���һ����ʼö��
//          ��һ��ö��ʱ����NULL���Ժ�ÿ�ξ������ϴη��ص�metahdls�����һ��ֶ�����
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	1.�ú�������ö��UploadId��Ӧ�����ϴ��ķֶ�,�����ص�
//          metahdlcnt������ֵС�ڴ�����ֵ��ʾ���зֶ�ö�����
//          ע�⣺metahdls����Ϣ����ʹ��ʱ����ҪΪÿ��ObjectMetaHandle����cssp_objectmeta_release�ͷ���Դ
//          2.�ú��������ѳ�ʼ������δ����complete��abort��UploadId��Ч
//			3.�ֶ��ϴ����ص�ObjectMetaHandleָ��ķֶξ���һ��ʵ�ʵ�Object
//************************************
IFLYAPI int cssp_multipart_upload_list_parts(const CSSPHandle hdl, const UploadId upid, ObjectMetaHandle* metahdls, int* limit, const char* marker, CSSPResult result);
typedef int (*Proc_cssp_multipart_upload_list_parts)(const CSSPHandle hdl, const UploadId upid, ObjectMetaHandle* metahdls, int* limit, const char* marker, CSSPResult result);

//************************************
// method:	cssp_multipart_upload_complete
// brief:	�ֶ��ϴ����
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl	- [in]
//          �洢���
// param:	const UploadId upid	- [in]
//			�ֶ��ϴ����
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	1.�ú�����������ϴ��ķֶδ���manifest�ļ������߼��鲢
//			2.ִ�иú�����UploadIdʧЧ
//			3.��δִ�иú���ǰ����������������ǲ����õ�
//************************************
IFLYAPI int cssp_multipart_upload_complete(const CSSPHandle hdl, const UploadId upid, CSSPResult result);
typedef int (*Proc_cssp_multipart_upload_complete)(const CSSPHandle hdl, const UploadId upid, CSSPResult result);

//************************************
// method:	cssp_multipart_upload_abort
// brief:	��ֹ�ֶ��ϴ�
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const CSSPHandle hdl	- [in]
//          �洢���
// param:	const UploadId upid	- [in]
//			�ֶ��ϴ����
// param:   CSSPResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	1.�ú��������Ƴ������ϴ��ķֶ�
//			2.ִ�иú�����UploadIdʧЧ
//************************************
IFLYAPI int cssp_multipart_upload_abort(const CSSPHandle hdl, const UploadId upid, CSSPResult result);
typedef int (*Proc_cssp_multipart_upload_abort)(const CSSPHandle hdl, const UploadId upid, CSSPResult result);


/*****************************************************************************/
/***********************metadata specialize interfaces************************/
/*****************************************************************************/


/*Container Metadata Interface*/
//************************************
// method:  cssp_containermeta_create
// brief:   ����ContainerMeta�ṹ
// access:  public 
// returns: ContainerMetaHandle - return a handle of ContainerMetadata, otherwise return CSSP_INVALID_HANDLE.
// author:  zhengyao
// remarks: ����ContainerMeta�ṹ����������
//************************************
IFLYAPI ContainerMetaHandle cssp_containermeta_create();
typedef ContainerMetaHandle (*Proc_cssp_containermeta_create)();

//************************************
// method:  cssp_containermeta_release
// brief:   �ͷ�ContainerMeta�ṹ
// access:  public 
// returns: void
// param:   const ContainerMetaHandle - [in]
//          ContainerMeta���
// author:  zhengyao
// remarks: �����κβ���ʹ�õ�ContainerMeta��������ʽ���øýӿ��ͷ���Դ
//************************************
IFLYAPI void cssp_containermeta_release(const ContainerMetaHandle);
typedef void (*Proc_cssp_containermeta_release)(const ContainerMetaHandle);


/*Object Metadata Interface*/
//************************************
// method:	cssp_objectmeta_create
// brief:	����ObjectMeta
// access:	public 
// returns:	ObjectMetaHandle - return a handle of ObjectMetadata, otherwise return CSSP_INVALID_HANDLE.
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI ObjectMetaHandle cssp_objectmeta_create();
typedef ObjectMetaHandle (*Proc_cssp_objectmeta_create)();

//************************************
// method:	cssp_objectmeta_release
// brief:	�ͷ�ObjectMeta��Ϣ
// access:	public 
// returns:	void	- 
// param:	const ObjectMetaHandle	- [in]
//          ObjectMeta���
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI void cssp_objectmeta_release(const ObjectMetaHandle);
typedef void (*Proc_cssp_objectmeta_release)(const ObjectMetaHandle);



/*****************************************************************************/
/*************************metadata common interfaces**************************/


//************************************
// method:	cssp_metadata_ownername
// brief:	��ȡmetadata��������(container name��object name)
// access:	public 
// returns:	const char*	- return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const MetadataHandle metadhandle    - [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI const char* cssp_metadata_ownername(const MetadataHandle metadhandle);
typedef const char* (*Proc_cssp_metadata_ownername)(const MetadataHandle metadhandle);

//************************************
// method:	cssp_metadata_get
// brief:	��ȡ�Զ���metadata��Ϣ
// access:	public 
// returns:	const char*	- return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const MetadataHandle metahandle	- [in]
//          Metadata���(���Դ���ContainerMetaHandle��ObjectMetaHandle)
// param:	const char * metaname	- [in]
//          metadata����
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI const char* cssp_metadata_get(const MetadataHandle metahandle, const char* metaname);
typedef const char* (*Proc_cssp_metadata_get)(const MetadataHandle metahandle, const char* metaname);

//************************************
// method:	cssp_metadata_set
// brief:	����ָ����metadata��Ϣ
// access:	public 
// returns:	int	- return 0 in success, otherwise return error code.
// param:	const MetadataHandle metahandle	- [in]
//          Metadata���(���Դ���ContainerMetaHandle��ObjectMetaHandle)
// param:	const char * metaname	- [in]
//          metadata����
// param:	const char * metavalue	- [in]
//          metadataֵ
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_metadata_set(const MetadataHandle metahandle, const char* metaname, const char* metavalue);
typedef int (*Proc_cssp_metadata_set)(const MetadataHandle metahandle, const char* metaname, const char* metavalue);

//************************************
// method:	cssp_metadata_count
// brief:	metadata�����
// access:	public 
// returns:	int	- return the count of items in Metadata.
// param:	const MetadataHandle metahandle	- [in]
//          Metadata���(���Դ���ContainerMetaHandle��ObjectMetaHandle)
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_metadata_count(const MetadataHandle metahandle);
typedef int (*Proc_cssp_metadata_count)(const MetadataHandle metahandle);




#ifdef __cplusplus

}
#endif


#endif // __CSSP__CSSP_H__