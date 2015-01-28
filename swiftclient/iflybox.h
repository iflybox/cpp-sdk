
#ifndef __IFLYBOX__IFLYBOX_H__
#define __IFLYBOX__IFLYBOX_H__

#include <stdint.h>

#define UPLOADID_LEN	32

//iflybox�������
typedef void* iBoxHandle;
typedef void* MetadataHandle;
typedef void* iflyboxResult;
typedef MetadataHandle AccountMetaHandle;
typedef MetadataHandle BucketMetaHandle;
typedef MetadataHandle ObjectMetaHandle;
typedef char UploadId[UPLOADID_LEN + 1];

#define IFLYBOX_INVALID_HANDLE  NULL

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
// method:	iflybox_init_result
// brief:	����ִ��״̬�����ṹ
// access:	public  
// returns:	IFLYAPI iflyboxResult - 
// author:	zhengyao
// remarks:	iflybox������API�ӿ���iflyboxResult���ڷ���ִ��״̬��ϸ����
//************************************
IFLYAPI iflyboxResult iflybox_init_result();
typedef iflyboxResult (*Proc_iflybox_init_result)();

//************************************
// method:	iflybox_release_result
// brief:	�ͷ�ִ��״̬�ṹ��
// access:	public 
// returns:	IFLYAPI void - 
// param:	iflyboxResult	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI void iflybox_release_result(iflyboxResult);
typedef void (*Proc_iflybox_release_result)(iflyboxResult);


//************************************
// method:	iflybox_result_statuscode
// brief:	��ȡresult�ṹ�е�statuscode
// access:	public  
// returns:	IFLYAPI int - 
// param:	iflyboxResult	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_result_statuscode(iflyboxResult);
typedef int (*Proc_iflybox_result_statuscode)(iflyboxResult);

//************************************
// method:	iflybox_is_successful
// brief:	�ж�״̬���Ƿ�ɹ�
// access:	public  
// returns:	IFLYAPI bool - 
// param:	int	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_is_successful(int);
typedef int (*Proc_iflybox_is_successful)(int);

//************************************
// method:	iflybox_result_detail
// brief:	��ȡresult�е���ϸ��Ϣ
// access:	public  
// returns:	IFLYAPI const char* - 
// param:	iflyboxResult	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI const char* iflybox_result_detail(iflyboxResult);
typedef const char* (*Proc_iflybox_result_detail)(iflyboxResult);

//************************************
// method:  iflybox_create_instance
// brief:   ����iflyboxʵ��
// access:  public 
// returns: iBoxHandle  - �ɹ����ش洢�����ʧ�ܷ���IFLYBOX_INVALID_HANDLE.
// param:   const char* endpoint - [in]
//          iflybox�����endpoint
// param:   const char* accessKeyId - [in]
//          ����iflybox��Access Key ID
// param:   const char* accessKeySecret	- [in]
//          ����iflybox��Access Key Secret
// param:   int endpoint_type  - [in]
//			����iflybox��ʹ�õ�storage url���ͣ�
//				0 - ��ʾpublicURL - �������� 
//				1 - ��ʾinternalURL - ��������
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI iBoxHandle iflybox_create_instance(const char* endpoint, const char* accessKeyId, const char* accessKeySecret, int endpoint_type, iflyboxResult result);
typedef iBoxHandle (*Proc_iflybox_create_instance)(const char* endpoint, const char* accessKeyId, const char* accessKeySecret, int endpoint_type, iflyboxResult result);

//************************************
// method:  iflybox_reauth
// brief:   ������֤iflyboxʵ��
// access:  public 
// returns: int - return 0 in success, otherwise return error code.
// param:   iBoxHandle hdl  - [in] 
//          �洢���
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI int iflybox_reauth_account(iBoxHandle hdl, iflyboxResult result);
typedef int (*Proc_iflybox_reauth_account)(iBoxHandle hdl, iflyboxResult result);

//************************************
// method:  iflybox_release_instance
// brief:   �ͷ�iflyboxʵ��
// access:  public 
// returns: int - return 0 in success, otherwise return error code.
// param:   iBoxHandle hdl  - [in] 
//          �洢���
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI int iflybox_release_instance(iBoxHandle hdl);
typedef int (*Proc_iflybox_release_instance)(iBoxHandle hdl);

//************************************
// method:  iflybox_set_timeout
// brief:   ���ý�����ʱʱ��
// access:  public 
// returns: int - ����֮ǰ�ĳ�ʱʱ��
// param:   iBoxHandle hdl  - [in] 
//          �洢���
//			int	timeout		- [in]
//			��ʱʱ�䣨���룩
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI int iflybox_set_timeout(iBoxHandle hdl, int timeout);
typedef int (*Proc_iflybox_set_timeout)(iBoxHandle hdl);




/*************************************************************************/
/************************Storage Account Interfaces***********************/
/*************************************************************************/





//************************************
// method:	iflybox_list_buckets
// brief:	ö��account����bucket
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	BucketMetaHandle * metahdls	- [in,out]
//          ��ָ��ָ��Ľṹ����������䷵�ص�BucketMetaHandle
// param:	int * metahdlcnt	- [in,out]
//          �û�����ʱ���Ϊ�ô���Ҫ��ȡ��Bucket����(����ֵ���ܳ���metahdls���ɴ洢����)
//          ����ִ�к���ֶ����Ϊʵ�ʶ�ȡ��BucketMeta����
//			**ע�⣺metahdlcnt���������10000
// param:	const char * marker	- [in]
//          Bucket���ƣ����ڷֶ�ö�ٵķָ��,�������Ӹ�Bucket����һ����ʼö��
//          ��һ��ö��ʱ����NULL���Ժ�ÿ�ξ������ϴη��ص�metahdls�����һ��Bucket����
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	�ú������ڷֶ�ö��account����bucket,�����ص�
//          metahdlcnt������ֵС�ڴ�����ֵ��ʾ����bucket��ȡ���
//          ע�⣺metahdls����Ϣ����ʹ��ʱ����ҪΪÿ��BucketMetaHandle����iflybox_bucketmeta_release�ͷ���Դ
//************************************
IFLYAPI int iflybox_list_buckets(const iBoxHandle hdl, BucketMetaHandle* metahdls, int* metahdlcnt, const char* marker, iflyboxResult result);
typedef int (*Proc_iflybox_list_buckets)(const iBoxHandle hdl, BucketMetaHandle* metahdls, int* metahdlcnt, const char* marker, iflyboxResult result);

//************************************
// method:	iflybox_set_accountmeta
// brief:	����account metadata
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl    - [in]
//          �洢���
// param:	const AccountMetaHandle metadata	- [in]
//          ������Account Metadata��Ϣ
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_set_accountmeta(const iBoxHandle hdl, const AccountMetaHandle metadata, iflyboxResult result);
typedef int (*Proc_iflybox_set_accountmeta)(const iBoxHandle hdl, const AccountMetaHandle metadata, iflyboxResult result);


//************************************
// method:	iflybox_get_accountmeta
// brief:	��ȡaccount metadata
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	AccountMetaHandle  metadata	- [in,out]
//          ����AccountMeta
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_get_accountmeta(const iBoxHandle hdl, AccountMetaHandle metadata, iflyboxResult result);
typedef int (*Proc_iflybox_get_accountmeta)(const iBoxHandle hdl, AccountMetaHandle metadata, iflyboxResult result);


//************************************
// method:	iflybox_remove_accountmeta
// brief:	�Ƴ�account metadata
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const AccountMetaHandle metadata	- [in]
//          ���Ƴ�Account Metadata��Ϣ
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_remove_accountmeta(const iBoxHandle hdl, const AccountMetaHandle metadata, iflyboxResult result);
typedef int (*Proc_iflybox_remove_accountmeta)(const iBoxHandle hdl, const AccountMetaHandle metadata, iflyboxResult result);

//************************************
// method:  iflybox_accountmeta_bucketcount
// brief:   ��ȡAccount��Bucket����
// access:  public 
// returns: uint64_t - return the count of bucket in Account.
// param:   const AccountMetaHandle metahandle    - [in]
//          AccountMeta���
// author:  zhengyao
// remarks: bucketCountΪAccount��system define metadata
//************************************
IFLYAPI uint64_t iflybox_accountmeta_bucketcount(const AccountMetaHandle metahandle);
typedef uint64_t (*Proc_iflybox_accountmeta_bucketcount)(const AccountMetaHandle metahandle);

//************************************
// method:  iflybox_accountmeta_bytesused
// brief:   ��ȡaccount�е�ǰ�洢����
// access:  public 
// returns: uint64_t - return the total bytes stored in Object Storage for the account.
// param:   const AccountMetaHandle metahandle    - [in]
//          AccountMeta���
// author:  zhengyao
// remarks: byteUsedΪAccount��system define metadata
//************************************
IFLYAPI uint64_t iflybox_accountmeta_bytesused(const AccountMetaHandle metahandle);
typedef uint64_t (*Proc_iflybox_accountmeta_bytesused)(const AccountMetaHandle metahandle);


/*************************************************************************/
/************************Storage Bucket Interfaces*********************/
/*************************************************************************/


//************************************
// method:  iflybox_bucket_exist
// brief:   �ж�ָ��Bucket�Ƿ����
// access:  public 
// returns: int - ����HTTP״̬��.
// param:   const iBoxHandle hdl    - [in]
//          �洢���
// param:   const char * bucket  - [in]
//          Bucket����
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI int iflybox_bucket_exist(const iBoxHandle hdl, const char* bucket, iflyboxResult result);
typedef int (*Proc_iflybox_bucket_exist)(const iBoxHandle hdl, const char* bucket, iflyboxResult result);

//************************************
// method:  iflybox_create_bucket
// brief:   ����BucketMetadata��Ϣ����bucket
// access:  public 
// returns: int - ����HTTP״̬��.
// param:   const iBoxHandle hdl  - [in]
//          �洢���
// param:   const char * bucket    - [in]
//          Bucket����
// param:   const BucketMetaHandle metahandle  - [in]
//          ��д��BucketMeta��Ϣ(ΪNULL���ʾ��д���Զ���metadata)
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI int iflybox_create_bucket(const iBoxHandle hdl, const char* bucket, const BucketMetaHandle metahandle, iflyboxResult result);
typedef int (*Proc_iflybox_create_bucket)(const iBoxHandle hdl, const char* bucket, const BucketMetaHandle metahandle, iflyboxResult result);

//************************************
// method:  iflybox_delete_bucket_ifempty
// brief:   ɾ��ָ���Ŀ�Bucket
// access:  public 
// returns: int - ����HTTP״̬��.
// param:   const iBoxHandle hdl    - [in]
//          �洢���
// param:   const char * bucket  - [in]
//          Bucket����
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks: ���Bucket�д���object��������ɾ��
//************************************
IFLYAPI int iflybox_delete_bucket_ifempty(const iBoxHandle hdl, const char* bucket, iflyboxResult result);
typedef int (*Proc_iflybox_delete_bucket_ifempty)(const iBoxHandle hdl, const char* bucket, iflyboxResult result);

//************************************
// method:	iflybox_list_object
// brief:	ö��bucket��object
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl    - [in]
//          �洢���
// param:	const char * bucket	- [in]
//          Bucket����
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
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	1.�ú������ڷֶ�ö��bucket��object,�����ص�
//          metahdlcnt������ֵС�ڴ�����ֵ��ʾ����object��ȡ���
//          ע�⣺metahdls����Ϣ����ʹ��ʱ����ҪΪÿ��ObjectMetaHandle����iflybox_objectmeta_release�ͷ���Դ
//          2.prefix��delimiter���ʹ��ʱʵ�ֶ�ָ����Ŀ¼������ö��
//************************************
IFLYAPI int iflybox_list_object(const iBoxHandle hdl, const char* bucket, ObjectMetaHandle* metahdls, int* limit, const char* prefix, const char* delimiter, const char* marker, iflyboxResult result);
typedef int (*Proc_iflybox_list_object)(const iBoxHandle hdl, const char* bucket, ObjectMetaHandle* metahdls, int* limit, const char* prefix, const char* delimiter, const char* marker, iflyboxResult result);

//************************************
// method:	iflybox_set_bucketmeta
// brief:	����bucket metadata
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const char * bucket	- [in]
//          Bucket����
// param:	const BucketMetaHandle metadata	- [in]
//          ������Bucket Metadata��Ϣ
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_set_bucketmeta(const iBoxHandle hdl, const char* bucket, const BucketMetaHandle metadata, iflyboxResult result);
typedef int (*Proc_iflybox_set_bucketmeta)(const iBoxHandle hdl, const char* bucket, const BucketMetaHandle metadata, iflyboxResult result);

// method:	iflybox_get_bucketmeta
// brief:	��ȡBucketMeta
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const char * bucket	- [in]
//          Bucket����
// param:	BucketMetaHandle  metadata	- [in,out]
//          ����BucketMeta
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_get_bucketmeta(const iBoxHandle hdl, const char* bucket, BucketMetaHandle metadata, iflyboxResult result);
typedef int (*Proc_iflybox_get_bucketmeta)(const iBoxHandle hdl, const char* bucket, BucketMetaHandle metadata, iflyboxResult result);

//************************************
// method:	iflybox_remove_bucketmeta
// brief:	�Ƴ�ָ��BucketMetadata
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const char * bucket	- [in]
//          ������bucket����
// param:	const BucketMetaHandle removeMeta	- [in]
//          ���Ƴ���metadata��
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_remove_bucketmeta(const iBoxHandle hdl, const char* bucket, const BucketMetaHandle removeMeta, iflyboxResult result);
typedef int (*Proc_iflybox_remove_bucketmeta)(const iBoxHandle hdl, const char* bucket, const BucketMetaHandle removeMeta, iflyboxResult result);

//************************************
// method:  iflybox_bucketmeta_objectcount
// brief:   ��ȡBucket��object����
// access:  public 
// returns: int - return the count of object in Bucket.
// param:   const BucketMetaHandle metahandle    - [in]
//          BucketMeta���
// author:  zhengyao
// remarks: objectCountΪBucket��system define metadata
//************************************
IFLYAPI uint64_t iflybox_bucketmeta_objectcount(const BucketMetaHandle metahandle);
typedef uint64_t (*Proc_iflybox_bucketmeta_objectcount)(const BucketMetaHandle metahandle);

//************************************
// method:  iflybox_bucketmeta_bytesused
// brief:   ��ȡBucket��object��ǰ�洢����
// access:  public 
// returns: uint64_t - return the total bytes stored in Object Storage for the bucket.
// param:   const BucketMetaHandle metahandle    - [in]
//          BucketMeta���
// author:  zhengyao
// remarks: byteUsedΪBucket��system define metadata
//************************************
IFLYAPI uint64_t iflybox_bucketmeta_bytesused(const BucketMetaHandle metahandle);
typedef uint64_t (*Proc_iflybox_bucketmeta_bytesused)(const BucketMetaHandle metahandle);


/*************************************************************************/
/************************Storage Object Interfaces*********************/
/*************************************************************************/


//************************************
// method:  iflybox_object_exist
// brief:   ָ��object�Ƿ����
// access:  public 
// returns: int - ����HTTP״̬��.
// param:   const iBoxHandle hdl  - [in]
//          �洢���  
// param:   const char * bucket  - [in]
//          Bucket����
// param:   const char * object - [in]
//          Object����
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks: 
//************************************
IFLYAPI int iflybox_object_exist(const iBoxHandle hdl, const char* bucket, const char* object, iflyboxResult result);
typedef int (*Proc_iflybox_object_exist)(const iBoxHandle hdl, const char* bucket, const char* object, iflyboxResult result);

//************************************
// method:	iflybox_put_object_callback_ptr
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
//			����iflybox_put_objectʱ�����inputstream
//			inputstream����߱��������ԣ�
//				1.ͨ��inputstream���Ի�ȡ���ϴ�����
//				2.ͨ��inputstream���Ի�ȡ���ϴ����ݵı߽�(����)
// author:	zhengyao
// remarks:	
//************************************
typedef size_t (*iflybox_put_object_callback_ptr)(void* buffer, size_t size, size_t nmemb, void* inputstream);
IFLYAPI size_t iflybox_put_object_fromdisk_callback(void* buffer, size_t size, size_t nmemb, void* inputstream);
typedef size_t (*Proc_iflybox_put_object_fromdisk_callback)(void* buffer, size_t size, size_t nmemb, void* inputstream);

//************************************
// method:  iflybox_put_object
// brief:   �ϴ�object
// access:  public 
// returns: int - ����HTTP״̬��.
// param:   const iBoxHandle hdl  - [in]
//          �洢���
// param:   const char * bucket    - [in]
//          Bucket����
// param:   const char * object	- [in]
//          Object����
// param:   iflybox_put_object_callback putObjectCallback - [in]
//          �ϴ��ص�����(�������NULL��������Ĭ�ϻص�������inputstream��sizeָ�������ϴ�)
// param:   void * inputstream  - [in]
//          Object���ݣ����putObjectCallback�ص����룬�˲����ᱻ����putObjectCallback�ص��У�
// param:   uint64_t size - [in]
//          Object���ݳ��ȣ����putObjectCallback�ص��������룬��size�������ԣ�
// param:   const char* md5 - [in]
//          Object���ݵ�md5.������NULL�򲻽�������У��;���������У��ֵ�͸�md5�������򷵻�422״̬��
// param:   ObjectMetaHandle outmetadata - [out]
//          �����ϴ������ݵ�metadata��Ϣ(ע��Content-Length�ֶβ�����)
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks: �û�����putObjectCallback�ص���iflybox��ͨ�����øûص�����ȡ�ϴ����ݣ�����μ�iflybox_put_object_callback����
//************************************
IFLYAPI int iflybox_put_object(const iBoxHandle hdl, const char* bucket, const char* object, iflybox_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, ObjectMetaHandle outmetadata, iflyboxResult result);
typedef int (*Proc_iflybox_put_object)(const iBoxHandle hdl, const char* bucket, const char* object, iflybox_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, ObjectMetaHandle outmetadata, iflyboxResult result);


//************************************
// method:	iflybox_get_object_callback_ptr
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
//			����iflybox_get_objectʱ�����outputstream
// author:	zhengyao
// remarks:	
//************************************
typedef size_t (*iflybox_get_object_callback_ptr)(void* buffer, size_t size, size_t nmemb, void* outputstream);

//************************************
// method:	iflybox_get_object_todisk_callback
// brief:	�ṩ�������صĻص�����
// access:	public  
// returns:	size_t - 
// param:	void * buffer	- [in]
// param:	size_t size	- [in]
// param:	size_t nmemb	- [in]
// param:	void * outputstream	- [in]
//			FILE*�ļ���������û�ͨ��iflybox_get_object����(����ʱ��iflybox����������ص���)
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI size_t iflybox_get_object_todisk_callback(void* buffer, size_t size, size_t nmemb, void* outputstream);
typedef size_t (*Proc_iflybox_get_object_todisk_callback)(void* buffer, size_t size, size_t nmemb, void* outputstream);

//************************************
// method:  iflybox_get_object
// brief:   ����object
// access:  public 
// returns: int - ����HTTP״̬��.
// param:   const iBoxHandle hdl    - [in]
//          �洢���
// param:   const char * bucket  - [in]
//          Bucket����
// param:   const char * object - [in]
//          Object����
// param:   iflybox_get_object_callback getObjectCallback - [in]
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
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:  zhengyao
// remarks: �ýӿ������û��ֶ�����
//************************************
IFLYAPI int iflybox_get_object(const iBoxHandle hdl, const char* bucket, const char* object, iflybox_get_object_callback_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t* size, ObjectMetaHandle outmetadata,iflyboxResult result);
typedef int (*Proc_iflybox_get_object)(const iBoxHandle hdl, const char* bucket, const char* object, iflybox_get_object_callback_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t* size, ObjectMetaHandle outmetadata,iflyboxResult result);

//************************************
// method:	iflybox_remove_object
// brief:	�Ƴ�object
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const char * bucket	- [in]
//          Bucket����
// param:	const char * object	- [in]
//          Object����
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	 
//************************************
IFLYAPI int iflybox_remove_object(const iBoxHandle hdl, const char* bucket, const char* object, iflyboxResult result);
typedef int (*Proc_iflybox_remove_object)(const iBoxHandle hdl, const char* bucket, const char* object, iflyboxResult result);

//************************************
// method:	iflybox_copy_object
// brief:	����object
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
// param:	const char * srcBucket	- [in]
//          Դbucket����
// param:	const char * srcObject	- [in]
//          Դobject����
// param:	const char * dstBucket	- [in]
//          Ŀ��bucket����(��bucket�����Ѿ�����)
// param:	const char * dstObject	- [in]
//          Ŀ��object����
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_copy_object(const iBoxHandle hdl, const char* srcBucket, const char* srcObject, const char* dstBucket, const char* dstObject, iflyboxResult result);
typedef int (*Proc_iflybox_copy_object)(const iBoxHandle hdl, const char* srcBucket, const char* srcObject, const char* dstBucket, const char* dstObject, iflyboxResult result);


//************************************
// method:	iflybox_move_object
// brief:	�ƶ�object
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
// param:	const char * srcBucket	- [in]
//          Դbucket����
// param:	const char * srcObject	- [in]
//          Դobject����
// param:	const char * dstBucket	- [in]
//          Ŀ��bucket����
// param:	const char * dstObject	- [in]
//          Ŀ��object����
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_move_object(const iBoxHandle hdl, const char* srcBucket, const char* srcObject, const char* dstBucket, const char* dstObject, iflyboxResult result);
typedef int (*Proc_iflybox_move_object)(const iBoxHandle hdl, const char* srcBucket, const char* srcObject, const char* dstBucket, const char* dstObject, iflyboxResult result);


//************************************
// method:	iflybox_set_objectmeta
// brief:	����object metadata
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const char * bucket	- [in]
//          Bucket����
// param:	const char * object	- [in]
//          Object����
// param:	const ObjectMetaHandle metadata	- [in]
//          ������Object metadata��Ϣ
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_set_objectmeta(const iBoxHandle hdl, const char* bucket, const char* object, const ObjectMetaHandle metadata, iflyboxResult result);
typedef int (*Proc_iflybox_set_objectmeta)(const iBoxHandle hdl, const char* bucket, const char* object, const ObjectMetaHandle metadata, iflyboxResult result);

//************************************
// method:	iflybox_get_objectmeta
// brief:	��ȡobject metadata
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]

//          �洢���
// param:	const char * bucket	- [in]
//          Bucket����
// param:	const char * object	- [in]
//          Object����
// param:	ObjectMetaHandle  metadata	- [in,out]
//          ����ObjectMeta
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_get_objectmeta(const iBoxHandle hdl, const char* bucket, const char* object, ObjectMetaHandle metadata, iflyboxResult result);
typedef int (*Proc_iflybox_get_objectmeta)(const iBoxHandle hdl, const char* bucket, const char* object, ObjectMetaHandle metadata, iflyboxResult result);

//************************************
// method:	iflybox_remove_objectmeta
// brief:	�Ƴ�ָ��ObjectMetadata��
// access:	public 
// returns:	int	- return 0 in success, otherwise return error code.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const char * bucket	- [in]
//          Bucket����
// param:	const char * object	- [in]
//          object����
// param:	const ObjectMetaHandle removeMeta	- [in]
//          ���Ƴ���metadata��
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	
//************************************
//IFLYAPI int iflybox_remove_objectmeta(const iBoxHandle hdl, const char* bucket, const char* object, const ObjectMetaHandle removeMeta, iflyboxResult result);
//typedef int (*Proc_iflybox_remove_objectmeta)(const iBoxHandle hdl, const char* bucket, const char* object, const ObjectMetaHandle removeMeta, iflyboxResult result);

//************************************
// method:	iflybox_objectmeta_is_pseudodirectory
// brief:	�Ƿ�Ϊ����Ŀ¼
// access:	public 
// returns:	bool	- return true if owner is a Pseudo-Directory, otherwise return false(owner is a object).
// param:	const ObjectMetaHandle metahandle	- [in]
//          ObjectMeta���
// author:	zhengyao
// remarks:	���Ϊ����Ŀ¼����Ӧ�ü�����ȡ����Metadata��Ϣ,����Ŀ¼û��metadata��Ϣ
//************************************
IFLYAPI bool iflybox_objectmeta_is_pseudodirectory(const ObjectMetaHandle metahandle);
typedef bool (*Proc_iflybox_objectmeta_is_pseudodirectory)(const ObjectMetaHandle metahandle);

//************************************
// method:	iflybox_objectmeta_contentlength
// brief:	Object����
// access:	public 
// returns:	uint64_t    - return object content length.
// param:	const ObjectMetaHandle metahandle	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI uint64_t iflybox_objectmeta_contentlength(const ObjectMetaHandle metahandle);
typedef uint64_t (*Proc_iflybox_objectmeta_contentlength)(const ObjectMetaHandle metahandle);

//************************************
// method:	iflybox_objectmeta_etag
// brief:	object��Etagֵ
// access:	public 
// returns:	const char*	- return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const ObjectMetaHandle metahandle	- [in]
// author:	zhengyao
// remarks:	EtagĬ��Ϊmd5
//************************************
IFLYAPI const char* iflybox_objectmeta_etag(const ObjectMetaHandle metahandle);
typedef const char* (*Proc_iflybox_objectmeta_etag)(const ObjectMetaHandle metahandle);

//************************************
// method:	iflybox_objectmeta_lastmodify
// brief:	object Last Modifyʱ��
// access:	public  
// returns:	const char* - return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const ObjectMetaHandle metaHandle	- [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI const char* iflybox_objectmeta_lastmodify(const ObjectMetaHandle metahandle);
typedef const char* (*Proc_iflybox_objectmeta_lastmodify)(const ObjectMetaHandle metahandle);

/*****************************************************************************/
/*************************Multipart Upload interfaces*************************/
/*****************************************************************************/

//************************************
// method:	iflybox_multipart_upload_init
// brief:	��ʼ���ֶ��ϴ�
// access:	public 
// returns:	int	- return 200 in success, otherwise return 1.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const char * bucket	- [in]
//          Bucket����
// param:	const char * object	- [in]
//          object����
// param:	UploadId upid	- [in,out]
//          ����һ������Ϊ32λ��upload id(�û����봫��һ��33�ֽڵ�����)
// author:	zhengyao
// remarks:	�����ֶ��ϴ�����ͨ��UploadId����
//************************************
IFLYAPI int iflybox_multipart_upload_init(const iBoxHandle hdl, const char* bucket, const char* object, UploadId upid);
typedef int (*Proc_iflybox_multipart_upload_init)(const iBoxHandle hdl, const char* bucket, const char* object, UploadId upid);

//************************************
// method:	iflybox_multipart_upload_part
// brief:	�ϴ��ֶ�
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const UploadId upid	- [in]
//          �ֶ��ϴ�id
// param:	int partnumber	- [in]
//          �ֶα��
// param:   iflybox_put_object_callback putObjectCallback - [in]
//          �ϴ��ص�����(�������NULL��������Ĭ�ϻص�������inputstream��sizeָ�������ϴ�)
// param:   void * inputstream  - [in]
//          Object���ݣ����putObjectCallback�ص����룬�˲����ᱻ����putObjectCallback�ص��У�
// param:   uint64_t size - [in]
//          Object���ݳ��ȣ����putObjectCallback�ص��������룬��size�������ԣ�
// param:	const char* md5	- [in]
//          �ֶ�md5,����NULL�򲻽��зֶ�����У��,��������У�����ݲ����ڸ�md5���򷵻�422״̬��
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	������ͬ��partnumber������֮ǰ�ĸ÷ֶ�
//************************************
/*IFLYAPI int iflybox_multipart_upload_part(const iBoxHandle hdl, const UploadId upid, int partnumber, const char* buf, uint64_t size, const char* md5, iflyboxResult result);
typedef int (*Proc_iflybox_multipart_upload_part)(const iBoxHandle hdl, const UploadId upid, int partnumber, const char* buf, uint64_t size, const char* md5, iflyboxResult result);
*/
IFLYAPI int iflybox_multipart_upload_part(const iBoxHandle hdl, const UploadId upid, int partnumber,  iflybox_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, iflyboxResult result);
typedef int (*Proc_iflybox_multipart_upload_part)(const iBoxHandle hdl, const UploadId upid, int partnumber,  iflybox_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, iflyboxResult result);

//************************************
// method:	iflybox_multipart_upload_list_parts
// brief:	ö�ٵ�ǰ���ϴ��ֶ�
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl    - [in]
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
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	1.�ú�������ö��UploadId��Ӧ�����ϴ��ķֶ�,�����ص�
//          metahdlcnt������ֵС�ڴ�����ֵ��ʾ���зֶ�ö�����
//          ע�⣺metahdls����Ϣ����ʹ��ʱ����ҪΪÿ��ObjectMetaHandle����iflybox_objectmeta_release�ͷ���Դ
//          2.�ú��������ѳ�ʼ������δ����complete��abort��UploadId��Ч
//			3.�ֶ��ϴ����ص�ObjectMetaHandleָ��ķֶξ���һ��ʵ�ʵ�Object
//************************************
IFLYAPI int iflybox_multipart_upload_list_parts(const iBoxHandle hdl, const UploadId upid, ObjectMetaHandle* metahdls, int* limit, const char* marker, iflyboxResult result);
typedef int (*Proc_iflybox_multipart_upload_list_parts)(const iBoxHandle hdl, const UploadId upid, ObjectMetaHandle* metahdls, int* limit, const char* marker, iflyboxResult result);

//************************************
// method:	iflybox_multipart_upload_complete
// brief:	�ֶ��ϴ����
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const UploadId upid	- [in]
//			�ֶ��ϴ����
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	1.�ú�����������ϴ��ķֶδ���manifest�ļ������߼��鲢
//			2.ִ�иú�����UploadIdʧЧ
//			3.��δִ�иú���ǰ����������������ǲ����õ�
//************************************
IFLYAPI int iflybox_multipart_upload_complete(const iBoxHandle hdl, const UploadId upid, iflyboxResult result);
typedef int (*Proc_iflybox_multipart_upload_complete)(const iBoxHandle hdl, const UploadId upid, iflyboxResult result);

//************************************
// method:	iflybox_multipart_upload_abort
// brief:	��ֹ�ֶ��ϴ�
// access:	public 
// returns:	int	- ����HTTP״̬��.
// param:	const iBoxHandle hdl	- [in]
//          �洢���
// param:	const UploadId upid	- [in]
//			�ֶ��ϴ����
// param:   iflyboxResult result	- [in,out]
//          ���ڷ���ִ��״̬�����ΪNULL�򲻷���ִ��״̬
// author:	zhengyao
// remarks:	1.�ú��������Ƴ������ϴ��ķֶ�
//			2.ִ�иú�����UploadIdʧЧ
//************************************
IFLYAPI int iflybox_multipart_upload_abort(const iBoxHandle hdl, const UploadId upid, iflyboxResult result);
typedef int (*Proc_iflybox_multipart_upload_abort)(const iBoxHandle hdl, const UploadId upid, iflyboxResult result);


/*****************************************************************************/
/***********************metadata specialize interfaces************************/
/*****************************************************************************/



/*Account Metadata Interface*/
//************************************
// method:	iflybox_accountmeta_create
// brief:	����AccountMeta�ṹ
// access:	public 
// returns:	AccountMetaHandle	- return a handle of AccountMetadata, otherwise return IFLYBOX_INVALID_HANDLE.
// author:	zhengyao
// remarks:	����AccountMeta�ṹ����������
//************************************
IFLYAPI AccountMetaHandle iflybox_accountmeta_create();
typedef AccountMetaHandle (*Proc_iflybox_accountmeta_create)();

//************************************
// method:	iflybox_accountmeta_release
// brief:	�ͷ�AccountMeta�ṹ
// access:	public 
// returns:	void	- 
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI void iflybox_accountmeta_release(const AccountMetaHandle metahandle);
typedef void (*Proc_iflybox_accountmeta_release)(const AccountMetaHandle metahandle);


/*Bucket Metadata Interface*/
//************************************
// method:  iflybox_bucketmeta_create
// brief:   ����BucketMeta�ṹ
// access:  public 
// returns: BucketMetaHandle - return a handle of BucketMetadata, otherwise return IFLYBOX_INVALID_HANDLE.
// author:  zhengyao
// remarks: ����BucketMeta�ṹ����������
//************************************
IFLYAPI BucketMetaHandle iflybox_bucketmeta_create();
typedef BucketMetaHandle (*Proc_iflybox_bucketmeta_create)();

//************************************
// method:  iflybox_bucketmeta_release
// brief:   �ͷ�BucketMeta�ṹ
// access:  public 
// returns: void
// param:   const BucketMetaHandle - [in]
//          BucketMeta���
// author:  zhengyao
// remarks: �����κβ���ʹ�õ�BucketMeta��������ʽ���øýӿ��ͷ���Դ
//************************************
IFLYAPI void iflybox_bucketmeta_release(const BucketMetaHandle);
typedef void (*Proc_iflybox_bucketmeta_release)(const BucketMetaHandle);


/*Object Metadata Interface*/
//************************************
// method:	iflybox_objectmeta_create
// brief:	����ObjectMeta
// access:	public 
// returns:	ObjectMetaHandle - return a handle of ObjectMetadata, otherwise return IFLYBOX_INVALID_HANDLE.
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI ObjectMetaHandle iflybox_objectmeta_create();
typedef ObjectMetaHandle (*Proc_iflybox_objectmeta_create)();

//************************************
// method:	iflybox_objectmeta_release
// brief:	�ͷ�ObjectMeta��Ϣ
// access:	public 
// returns:	void	- 
// param:	const ObjectMetaHandle	- [in]
//          ObjectMeta���
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI void iflybox_objectmeta_release(const ObjectMetaHandle);
typedef void (*Proc_iflybox_objectmeta_release)(const ObjectMetaHandle);



/*****************************************************************************/
/*************************metadata common interfaces**************************/


//************************************
// method:	iflybox_metadata_ownername
// brief:	��ȡmetadata��������(account name��bucket name��object name)
// access:	public 
// returns:	const char*	- return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const MetadataHandle metadhandle    - [in]
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI const char* iflybox_metadata_ownername(const MetadataHandle metadhandle);
typedef const char* (*Proc_iflybox_metadata_ownername)(const MetadataHandle metadhandle);

//************************************
// method:	iflybox_metadata_get
// brief:	��ȡ�Զ���metadata��Ϣ
// access:	public 
// returns:	const char*	- return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const MetadataHandle metahandle	- [in]
//          Metadata���(���Դ���AccountMetaHandle��BucketMetaHandle��ObjectMetaHandle)
// param:	const char * metaname	- [in]
//          metadata����
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI const char* iflybox_metadata_get(const MetadataHandle metahandle, const char* metaname);
typedef const char* (*Proc_iflybox_metadata_get)(const MetadataHandle metahandle, const char* metaname);

//************************************
// method:	iflybox_metadata_set
// brief:	����ָ����metadata��Ϣ
// access:	public 
// returns:	int	- return 0 in success, otherwise return error code.
// param:	const MetadataHandle metahandle	- [in]
//          Metadata���(���Դ���AccountMetaHandle��BucketMetaHandle��ObjectMetaHandle)
// param:	const char * metaname	- [in]
//          metadata����
// param:	const char * metavalue	- [in]
//          metadataֵ
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_metadata_set(const MetadataHandle metahandle, const char* metaname, const char* metavalue);
typedef int (*Proc_iflybox_metadata_set)(const MetadataHandle metahandle, const char* metaname, const char* metavalue);

//************************************
// method:	iflybox_metadata_count
// brief:	metadata�����
// access:	public 
// returns:	int	- return the count of items in Metadata.
// param:	const MetadataHandle metahandle	- [in]
//          Metadata���(���Դ���AccountMetaHandle��BucketMetaHandle��ObjectMetaHandle)
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int iflybox_metadata_count(const MetadataHandle metahandle);
typedef int (*Proc_iflybox_metadata_count)(const MetadataHandle metahandle);




#ifdef __cplusplus

}
#endif


#endif // __IFLYBOX__IFLYBOX_H__