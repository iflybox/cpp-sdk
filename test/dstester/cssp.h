
#ifndef __CSSP__CSSP_H__
#define __CSSP__CSSP_H__

#include <stdint.h>

#define UPLOADID_LEN	32

//cssp操作句柄
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
// brief:	生成执行状态描述结构
// access:	public  
// returns:	IFLYAPI CSSPResult - 
// author:	zhengyao
// remarks:	CSSP的所有API接口中CSSPResult用于返回执行状态详细描述
//************************************
IFLYAPI CSSPResult cssp_init_result();
typedef CSSPResult (*Proc_cssp_init_result)();

//************************************
// method:	cssp_release_result
// brief:	释放执行状态结构体
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
// brief:	获取result结构中的statuscode
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
// brief:	判断状态码是否成功
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
// brief:	获取result中的详细信息
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
// brief:   创建cssp实例
// access:  public 
// returns: CSSPHandle  - 成功返回存储句柄，失败返回CSSP_INVALID_HANDLE.
// param:   const char* endpoint - [in]
//          cssp服务的endpoint
// param:   const char* accessKeyId - [in]
//          访问cssp的Access Key ID
// param:   const char* accessKeySecret	- [in]
//          访问cssp的Access Key Secret
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI CSSPHandle cssp_create_instance(const char* endpoint, const char* accessKeyId, const char* accessKeySecret, CSSPResult result);
typedef CSSPHandle (*Proc_cssp_create_instance)(const char* endpoint, const char* accessKeyId, const char* accessKeySecret, CSSPResult result);


//************************************
// method:  cssp_release_instance
// brief:   释放cssp实例
// access:  public 
// returns: int - return 0 in success, otherwise return error code.
// param:   CSSPHandle hdl  - [in] 
//          存储句柄
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI int cssp_release_instance(CSSPHandle hdl);
typedef int (*Proc_cssp_release_instance)(CSSPHandle hdl);

//************************************
// method:  cssp_set_timeout
// brief:   设置交互超时时间
// access:  public 
// returns: int - 返回之前的超时时间
// param:   CSSPHandle hdl  - [in] 
//          存储句柄
//			int	timeout		- [in]
//			超时时间（毫秒）
// author:  zhengyao
// remarks:   
//************************************
IFLYAPI int cssp_set_timeout(CSSPHandle hdl, int timeout);
typedef int (*Proc_cssp_set_timeout)(CSSPHandle hdl);




//************************************
// method:	cssp_list_object
// brief:	枚举container中object
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl    - [in]
//          存储句柄
// param:	ObjectMetaHandle * metahdls	- [in,out]
//          该指针指向的结构数组用于填充返回的ObejctMetaHandle
// param:	int * limit	- [in,out]
//          用户传入时填充为该次需要枚举的Object个数(该数值不能超过metahdls最大可存储个数)
//          函数执行后该字段填充为实际读取的ObjectMeta个数
//			**注意：limit最大不允许超过10000
// param:	const char * prefix	- [in]
//          Object名称前缀（枚举条件)
// param:	const char * delimiter	- [in]
//          Object名称分隔符（枚举条件，配合prefix使用）
// param:	const char * marker	- [in]
//          Object名称，用于分段枚举的分割点,函数将从该Object的下一个开始枚举
//          第一次枚举时传递NULL，以后每次均传递上次返回的metahdls的最后一项Object名称
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	1.该函数用于分段枚举container中object,当返回的
//          metahdlcnt填充的数值小于传入数值表示所有object读取完成
//          注意：metahdls中信息不再使用时，需要为每个ObjectMetaHandle调用cssp_objectmeta_release释放资源
//          2.prefix和delimiter配合使用时实现对指定“目录”进行枚举
//************************************
IFLYAPI int cssp_list_object(const CSSPHandle hdl, ObjectMetaHandle* metahdls, int* limit, const char* prefix, const char* delimiter, const char* marker, CSSPResult result);
typedef int (*Proc_cssp_list_object)(const CSSPHandle hdl, ObjectMetaHandle* metahdls, int* limit, const char* prefix, const char* delimiter, const char* marker, CSSPResult result);


//************************************
// method:  cssp_count_object
// brief:   获取Container中object数量
// access:  public 
// returns: int - 返回HTTP状态码.
// param:	const CSSPHandle hdl    - [in]
//          存储句柄
// param:	uint64_t* count    - [in,out]
//          返回container中object数
// param:	CSSPResult result    - [in]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:  zhengyao
// remarks: objectCount为Container的system define metadata
//************************************
IFLYAPI int cssp_count_object(const CSSPHandle hdl, uint64_t* count, CSSPResult result);
typedef int (*Proc_cssp_count_object)(const CSSPHandle hdl, uint64_t* count, CSSPResult result);

//************************************
// method:  cssp_count_bytesused
// brief:   获取Container中object当前存储容量
// access:  public 
// returns: int - 返回HTTP状态码.
// param:	const CSSPHandle hdl    - [in]
//          存储句柄
// param:	uint64_t* bytesused    - [in,out]
//          返回container中字节数
// param:	CSSPResult result    - [in]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:  zhengyao
// remarks: byteUsed为Container的system define metadata
//************************************
IFLYAPI int cssp_count_bytesused(const CSSPHandle hdl, uint64_t* bytesused, CSSPResult result);
typedef int (*Proc_cssp_count_bytesused)(const CSSPHandle hdl, uint64_t* count, CSSPResult result);


/*************************************************************************/
/************************Storage Object Interfaces*********************/
/*************************************************************************/


//************************************
// method:  cssp_object_exist
// brief:   指定object是否存在
// access:  public 
// returns: int - 返回HTTP状态码.
// param:   const CSSPHandle hdl  - [in]
//          存储句柄  
// param:   const char * object - [in]
//          Object名称
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:  zhengyao
// remarks: 
//************************************
IFLYAPI int cssp_object_exist(const CSSPHandle hdl, const char* object, CSSPResult result);
typedef int (*Proc_cssp_object_exist)(const CSSPHandle hdl, const char* object, CSSPResult result);

//************************************
// method:	cssp_put_object_callback_ptr
// brief:	上传文件回调函数
// access:	public  
// returns:	size_t	- 返回值指代该次回调获取的上传数据字节数，为0则表示上传数据获取完毕
// param:	void* buffer	- [in]
//			上传系统缓冲区(回调函数中，需要将数据拷贝至该缓冲区)
// param:	size_t size	- [in]
//			
// param:	size_t nmemb	- [in]
//			和size配合（size*nmemb表示系统缓冲区buffer的大小）
// param:	void* inputstream	- [in]
//			调用cssp_put_object时传入的inputstream
//			inputstream必须具备如下特性：
//				1.通过inputstream可以获取待上传数据
//				2.通过inputstream可以获取待上传数据的边界(长度)
// author:	zhengyao
// remarks:	
//************************************
typedef size_t (*cssp_put_object_callback_ptr)(void* buffer, size_t size, size_t nmemb, void* inputstream);
IFLYAPI size_t cssp_put_object_fromdisk_callback(void* buffer, size_t size, size_t nmemb, void* inputstream);
typedef size_t (*Proc_cssp_put_object_fromdisk_callback)(void* buffer, size_t size, size_t nmemb, void* inputstream);

//************************************
// method:  cssp_put_object
// brief:   上传object
// access:  public 
// returns: int - 返回HTTP状态码.
// param:   const CSSPHandle hdl  - [in]
//          存储句柄
// param:   const char * object	- [in]
//          Object名称
// param:   cssp_put_object_callback putObjectCallback - [in]
//          上传回调函数(如果传递NULL，则会调用默认回调函数将inputstream和size指代数据上传)
// param:   void * inputstream  - [in]
//          Object数据（如果putObjectCallback回调传入，此参数会被传入putObjectCallback回调中）
// param:   uint64_t size - [in]
//          Object数据长度（如果putObjectCallback回调函数传入，该size将被忽略）
// param:   const char* md5 - [in]
//          Object数据的md5.若传递NULL则不进行数据校验;如果传递且校验值和该md5不符，则返回422状态码
// param:   ObjectMetaHandle outmetadata - [out]
//          返回上传后数据的metadata信息(注意Content-Length字段不可用)
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:  zhengyao
// remarks: 用户传递putObjectCallback回调后，cssp会通过调用该回调来获取上传数据，具体参见cssp_put_object_callback声明
//************************************
IFLYAPI int cssp_put_object(const CSSPHandle hdl, const char* object, cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, ObjectMetaHandle outmetadata, CSSPResult result);
typedef int (*Proc_cssp_put_object)(const CSSPHandle hdl, const char* object, cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, ObjectMetaHandle outmetadata, CSSPResult result);


//************************************
// method:	cssp_get_object_callback_ptr
// brief:	下载文件回调函数
// access:	public  
// returns:	size_t	- 指代该次回调获取的下载数据字节数，如果返回值不等于size*nmemb则表示出错，下载将会终止
// param:	void* buffer	- [in]
//			下载系统缓冲区(回调函数中，需要该缓冲区的数据拷贝到用户数据中)
// param:	size_t size	- [in]
//			
// param:	size_t nmemb	- [in]
//			和size配合（size*nmemb表示该次回调下载字节数）
// param:	void* outputstream	- [in]
//			调用cssp_get_object时传入的outputstream
// author:	zhengyao
// remarks:	
//************************************
typedef size_t (*cssp_get_object_callback_ptr)(void* buffer, size_t size, size_t nmemb, void* outputstream);

//************************************
// method:	cssp_get_object_todisk_callback
// brief:	提供磁盘下载的回调函数
// access:	public  
// returns:	size_t - 
// param:	void * buffer	- [in]
// param:	size_t size	- [in]
// param:	size_t nmemb	- [in]
// param:	void * outputstream	- [in]
//			FILE*文件句柄，由用户通过cssp_get_object传入(下载时由cssp将参数传入回调中)
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI size_t cssp_get_object_todisk_callback(void* buffer, size_t size, size_t nmemb, void* outputstream);
typedef size_t (*Proc_cssp_get_object_todisk_callback)(void* buffer, size_t size, size_t nmemb, void* outputstream);

//************************************
// method:  cssp_get_object
// brief:   下载object
// access:  public 
// returns: int - 返回HTTP状态码.
// param:   const CSSPHandle hdl    - [in]
//          存储句柄
// param:   const char * object - [in]
//          Object名称
// param:   cssp_get_object_callback getObjectCallback - [in]
//          下载回调函数(如果传递NULL，则会调用默认回调函数将数据下载后写入outputstream和size指代的数据流)
// param:   void * outputstream  - [in,out]
//          用户用于接收下载数据的自定义结构(如果getObjectCallback传入为NULL，则该参数必须为内存buffer)
// param:   uint64_t offset - [in]
//          下载起始偏移
// param:   uint64_t * size - [in,out]
//          传入为期望下载的长度
//          传出为真实下载的长度
//			注意：此处*size < 0 则表示用户想一次下载全部文件，接收的buffer大小由用户保证
//				我们建议只有当用户传递getObjectCallback函数时才传递*size < 0的全量下载操作
// param:   ObjectMetaHandle outmetadata - [out]
//          返回上传后数据的metadata信息
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:  zhengyao
// remarks: 该接口允许用户分段下载
//************************************
IFLYAPI int cssp_get_object(const CSSPHandle hdl, const char* object, cssp_get_object_callback_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t* size, ObjectMetaHandle outmetadata,CSSPResult result);
typedef int (*Proc_cssp_get_object)(const CSSPHandle hdl, const char* object, cssp_get_object_callback_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t* size, ObjectMetaHandle outmetadata,CSSPResult result);

//************************************
// method:	cssp_remove_object
// brief:	移除object
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl	- [in]
//          存储句柄
// param:	const char * object	- [in]
//          Object名称
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	 
//************************************
IFLYAPI int cssp_remove_object(const CSSPHandle hdl, const char* object, CSSPResult result);
typedef int (*Proc_cssp_remove_object)(const CSSPHandle hdl, const char* object, CSSPResult result);

//************************************
// method:	cssp_copy_object
// brief:	拷贝object
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl	- [in]
//			存储句柄
// param:	const char * srcObject	- [in]
//          源object名称
// param:	const char * dstContainer	- [in]
//          目标container名称
// param:	const char * dstObject	- [in]
//          目标object名称
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_copy_object(const CSSPHandle hdl, const char* srcObject, const char* dstContainer, const char* dstObject, CSSPResult result);
typedef int (*Proc_cssp_copy_object)(const CSSPHandle hdl, const char* srcObject, const char* dstContainer, const char* dstObject, CSSPResult result);


//************************************
// method:	cssp_move_object
// brief:	移动object
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl	- [in]
//			存储句柄
// param:	const char * srcObject	- [in]
//          源object名称
// param:	const char * dstContainer	- [in]
//          目标container名称
// param:	const char * dstObject	- [in]
//          目标object名称
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_move_object(const CSSPHandle hdl, const char* srcObject, const char* dstContainer, const char* dstObject, CSSPResult result);
typedef int (*Proc_cssp_move_object)(const CSSPHandle hdl, const char* srcObject, const char* dstContainer, const char* dstObject, CSSPResult result);


//************************************
// method:	cssp_set_objectmeta
// brief:	设置object metadata
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl	- [in]
//          存储句柄
// param:	const char * object	- [in]
//          Object名称
// param:	const ObjectMetaHandle metadata	- [in]
//          待设置Object metadata信息
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_set_objectmeta(const CSSPHandle hdl, const char* object, const ObjectMetaHandle metadata, CSSPResult result);
typedef int (*Proc_cssp_set_objectmeta)(const CSSPHandle hdl, const char* object, const ObjectMetaHandle metadata, CSSPResult result);

//************************************
// method:	cssp_get_objectmeta
// brief:	获取object metadata
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl	- [in]
//          存储句柄
// param:	const char * object	- [in]
//          Object名称
// param:	ObjectMetaHandle  metadata	- [in,out]
//          返回ObjectMeta
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_get_objectmeta(const CSSPHandle hdl, const char* object, ObjectMetaHandle metadata, CSSPResult result);
typedef int (*Proc_cssp_get_objectmeta)(const CSSPHandle hdl, const char* object, ObjectMetaHandle metadata, CSSPResult result);


//************************************
// method:	cssp_objectmeta_is_pseudodirectory
// brief:	是否为虚拟目录
// access:	public 
// returns:	bool	- return true if owner is a Pseudo-Directory, otherwise return false(owner is a object).
// param:	const ObjectMetaHandle metahandle	- [in]
//          ObjectMeta句柄
// author:	zhengyao
// remarks:	如果为虚拟目录，则不应该继续获取其他Metadata信息,虚拟目录没有metadata信息
//************************************
IFLYAPI bool cssp_objectmeta_is_pseudodirectory(const ObjectMetaHandle metahandle);
typedef bool (*Proc_cssp_objectmeta_is_pseudodirectory)(const ObjectMetaHandle metahandle);

//************************************
// method:	cssp_objectmeta_contentlength
// brief:	Object长度
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
// brief:	object的Etag值
// access:	public 
// returns:	const char*	- return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const ObjectMetaHandle metahandle	- [in]
// author:	zhengyao
// remarks:	Etag默认为md5
//************************************
IFLYAPI const char* cssp_objectmeta_etag(const ObjectMetaHandle metahandle);
typedef const char* (*Proc_cssp_objectmeta_etag)(const ObjectMetaHandle metahandle);

//************************************
// method:	cssp_objectmeta_lastmodify
// brief:	object Last Modify时间
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
// brief:	初始化分段上传
// access:	public 
// returns:	int	- return 200 in success, otherwise return 1.
// param:	const CSSPHandle hdl	- [in]
//          存储句柄
// param:	const char * object	- [in]
//          object名称
// param:	UploadId upid	- [in,out]
//          返回一个长度为32位的upload id(用户必须传入一个33字节的数组)
// author:	zhengyao
// remarks:	后续分段上传必须通过UploadId操作
//************************************
IFLYAPI int cssp_multipart_upload_init(const CSSPHandle hdl, const char* object, UploadId upid);
typedef int (*Proc_cssp_multipart_upload_init)(const CSSPHandle hdl, const char* object, UploadId upid);

//************************************
// method:	cssp_multipart_upload_part
// brief:	上传分段
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl	- [in]
//          存储句柄
// param:	const UploadId upid	- [in]
//          分段上传id
// param:	int partnumber	- [in]
//          分段编号
// param:   cssp_put_object_callback putObjectCallback - [in]
//          上传回调函数(如果传递NULL，则会调用默认回调函数将inputstream和size指代数据上传)
// param:   void * inputstream  - [in]
//          Object数据（如果putObjectCallback回调传入，此参数会被传入putObjectCallback回调中）
// param:   uint64_t size - [in]
//          Object数据长度（如果putObjectCallback回调函数传入，该size将被忽略）
// param:	const char* md5	- [in]
//          分段md5,传递NULL则不进行分段数据校验,若传递且校验数据不等于该md5，则返回422状态码
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	传递相同的partnumber将覆盖之前的该分段
//************************************
IFLYAPI int cssp_multipart_upload_part(const CSSPHandle hdl, const UploadId upid, int partnumber,  cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, CSSPResult result);
typedef int (*Proc_cssp_multipart_upload_part)(const CSSPHandle hdl, const UploadId upid, int partnumber,  cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, CSSPResult result);

//************************************
// method:	cssp_multipart_upload_list_parts
// brief:	枚举当前已上传分段
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl    - [in]
//          存储句柄
// param:	const UploadId upid	- [in]
//          分段上传id
// param:	ObjectMetaHandle * metahdls	- [in,out]
//          该指针指向的结构数组用于填充返回的ObjectMetaHandle
// param:	int * limit	- [in,out]
//          用户传入时填充为该次需要枚举的分段个数(该数值不能超过metahdls最大可存储个数)
//          函数执行后该字段填充为实际读取的分段个数
// param:	const char * marker	- [in]
//          分段名称，用于分段枚举的分割点,函数将从该分段的下一个开始枚举
//          第一次枚举时传递NULL，以后每次均传递上次返回的metahdls的最后一项分段名称
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	1.该函数用于枚举UploadId对应的已上传的分段,当返回的
//          metahdlcnt填充的数值小于传入数值表示所有分段枚举完成
//          注意：metahdls中信息不再使用时，需要为每个ObjectMetaHandle调用cssp_objectmeta_release释放资源
//          2.该函数仅对已初始化但是未调用complete或abort的UploadId有效
//			3.分段上传返回的ObjectMetaHandle指向的分段就是一个实际的Object
//************************************
IFLYAPI int cssp_multipart_upload_list_parts(const CSSPHandle hdl, const UploadId upid, ObjectMetaHandle* metahdls, int* limit, const char* marker, CSSPResult result);
typedef int (*Proc_cssp_multipart_upload_list_parts)(const CSSPHandle hdl, const UploadId upid, ObjectMetaHandle* metahdls, int* limit, const char* marker, CSSPResult result);

//************************************
// method:	cssp_multipart_upload_complete
// brief:	分段上传完成
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl	- [in]
//          存储句柄
// param:	const UploadId upid	- [in]
//			分段上传句柄
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	1.该函数将会对已上传的分段创建manifest文件进行逻辑归并
//			2.执行该函数后UploadId失效
//			3.在未执行该函数前，完整对象的下载是不可用的
//************************************
IFLYAPI int cssp_multipart_upload_complete(const CSSPHandle hdl, const UploadId upid, CSSPResult result);
typedef int (*Proc_cssp_multipart_upload_complete)(const CSSPHandle hdl, const UploadId upid, CSSPResult result);

//************************************
// method:	cssp_multipart_upload_abort
// brief:	终止分段上传
// access:	public 
// returns:	int	- 返回HTTP状态码.
// param:	const CSSPHandle hdl	- [in]
//          存储句柄
// param:	const UploadId upid	- [in]
//			分段上传句柄
// param:   CSSPResult result	- [in,out]
//          用于返回执行状态，如果为NULL则不返回执行状态
// author:	zhengyao
// remarks:	1.该函数将会移除对已上传的分段
//			2.执行该函数后UploadId失效
//************************************
IFLYAPI int cssp_multipart_upload_abort(const CSSPHandle hdl, const UploadId upid, CSSPResult result);
typedef int (*Proc_cssp_multipart_upload_abort)(const CSSPHandle hdl, const UploadId upid, CSSPResult result);


/*****************************************************************************/
/***********************metadata specialize interfaces************************/
/*****************************************************************************/


/*Container Metadata Interface*/
//************************************
// method:  cssp_containermeta_create
// brief:   创建ContainerMeta结构
// access:  public 
// returns: ContainerMetaHandle - return a handle of ContainerMetadata, otherwise return CSSP_INVALID_HANDLE.
// author:  zhengyao
// remarks: 创建ContainerMeta结构供后续操作
//************************************
IFLYAPI ContainerMetaHandle cssp_containermeta_create();
typedef ContainerMetaHandle (*Proc_cssp_containermeta_create)();

//************************************
// method:  cssp_containermeta_release
// brief:   释放ContainerMeta结构
// access:  public 
// returns: void
// param:   const ContainerMetaHandle - [in]
//          ContainerMeta句柄
// author:  zhengyao
// remarks: 对于任何不再使用的ContainerMeta，必须显式调用该接口释放资源
//************************************
IFLYAPI void cssp_containermeta_release(const ContainerMetaHandle);
typedef void (*Proc_cssp_containermeta_release)(const ContainerMetaHandle);


/*Object Metadata Interface*/
//************************************
// method:	cssp_objectmeta_create
// brief:	生成ObjectMeta
// access:	public 
// returns:	ObjectMetaHandle - return a handle of ObjectMetadata, otherwise return CSSP_INVALID_HANDLE.
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI ObjectMetaHandle cssp_objectmeta_create();
typedef ObjectMetaHandle (*Proc_cssp_objectmeta_create)();

//************************************
// method:	cssp_objectmeta_release
// brief:	释放ObjectMeta信息
// access:	public 
// returns:	void	- 
// param:	const ObjectMetaHandle	- [in]
//          ObjectMeta句柄
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI void cssp_objectmeta_release(const ObjectMetaHandle);
typedef void (*Proc_cssp_objectmeta_release)(const ObjectMetaHandle);



/*****************************************************************************/
/*************************metadata common interfaces**************************/


//************************************
// method:	cssp_metadata_ownername
// brief:	获取metadata属主名称(container name、object name)
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
// brief:	获取自定义metadata信息
// access:	public 
// returns:	const char*	- return a pointer to string terminated with '\0' in success, otherwise return NULL.
// param:	const MetadataHandle metahandle	- [in]
//          Metadata句柄(可以传递ContainerMetaHandle、ObjectMetaHandle)
// param:	const char * metaname	- [in]
//          metadata名称
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI const char* cssp_metadata_get(const MetadataHandle metahandle, const char* metaname);
typedef const char* (*Proc_cssp_metadata_get)(const MetadataHandle metahandle, const char* metaname);

//************************************
// method:	cssp_metadata_set
// brief:	设置指定的metadata信息
// access:	public 
// returns:	int	- return 0 in success, otherwise return error code.
// param:	const MetadataHandle metahandle	- [in]
//          Metadata句柄(可以传递ContainerMetaHandle、ObjectMetaHandle)
// param:	const char * metaname	- [in]
//          metadata名称
// param:	const char * metavalue	- [in]
//          metadata值
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_metadata_set(const MetadataHandle metahandle, const char* metaname, const char* metavalue);
typedef int (*Proc_cssp_metadata_set)(const MetadataHandle metahandle, const char* metaname, const char* metavalue);

//************************************
// method:	cssp_metadata_count
// brief:	metadata项个数
// access:	public 
// returns:	int	- return the count of items in Metadata.
// param:	const MetadataHandle metahandle	- [in]
//          Metadata句柄(可以传递ContainerMetaHandle、ObjectMetaHandle)
// author:	zhengyao
// remarks:	
//************************************
IFLYAPI int cssp_metadata_count(const MetadataHandle metahandle);
typedef int (*Proc_cssp_metadata_count)(const MetadataHandle metahandle);




#ifdef __cplusplus

}
#endif


#endif // __CSSP__CSSP_H__