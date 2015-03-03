#ifndef __CSSP__SWIFTCLIENT_H__
#define __CSSP__SWIFTCLIENT_H__
#include "exception.h"
#include <string>
#include <vector>
#include "metadata.h"
#include "sha1_request.h"
#include "http_status_code.h"
#include <sstream>

namespace cssp
{

	class SwiftObject : public std::stringstream{};

	//用于libcurl中write的封装，用于分离正确数据和错误信息
	typedef struct libcurl_write_struct_tag{
		write_data_ptr	getCallback;
		void*			user_stream;
		std::string*	response_content;
		CURL*			curl_handle;
	}libcurl_write_struct;

	//cssp中返回信息
	class CSSPResult{
	public:
		void setStatus(int status){
			status_ = status;
		}
		int getStatus()const{
			return status_;
		}
		void setDetail(const std::string& detail){
			detail_ = detail;
		}
		const std::string& getDetail()const{
			return detail_;
		}
	private:
		int				status_;
		std::string		detail_;
	};


class SwiftClient{
public:
	SwiftClient(const std::string& containerUrl, const std::string& accessKeyId, const std::string& accessKeySecret, int timeout = 30000);
	int getTimeout();
	void setTimeout(int timeout);

	/*Container Interface*/

	//************************************
	// method:    containerExists
	// brief:	  判断container是否存在
	// access:    public 
	// returns:   cssp::CSSPResult		
	// author:    zhengyao
	// remarks:   函数在正常情况下由返回值表示是否存在，异常情况下函数抛出异常
	//************************************
	CSSPResult	containerExists();

	//************************************
	// method:    deleteContainerIfEmpty
	// brief:     如果Container为空，则删除该Container
	// access:    public 
	// returns:   cssp::CSSPResult
	// author:    zhengyao
	// remarks:   异常情况下由函数抛出错误码
	//************************************
	CSSPResult	deleteContainerIfEmpty();

	//************************************
	// method:	setContainerSysmeta
	// brief:	设置或移除container的系统数据
	// access:	public 
	// returns:	CSSPResult
	// param:	const std::string & sysKey	- []
	// param:	const std::string & sysValue	- []
	// author:	zhengyao
	// remarks:	该函数可同时提供设置或移除功能，通过sysKey来定义功能
	//			例如X-Container-* 和X-Remove-Container-* 分别表示设置和移除
	//************************************
	CSSPResult setContainerSysmeta(const std::string& sysKey, const std::string& sysValue);


	//************************************
	// method:    setContainerMetadata
	// brief：    设置container的metainfo
	// access:    public 
	// returns:   cssp::CSSPResult
	// param:     const std::map<std::string
	// param:     std::string> & containerMetadata
	// author:    zhengyao
	// remarks:   
	//************************************
	CSSPResult	setContainerMetadata(const ContainerMetadata& containerMetadata);

	//************************************
	// method:    getContainerMetadata
	// brief:     获取container的metainfo
	// access:    public 
	// returns:   cssp::CSSPResult
	// param:     ContainerMetadata& metadata
	// author:    zhengyao
	// remarks:   
	//************************************
	CSSPResult	getContainerMetadata(ContainerMetadata& metadata);

	//************************************
	// method:    deleteContainerMetadata
	// brief:     删除Container中指定的metadata K-V对
	// access:    public 
	// returns:   cssp::CSSPResult
	// param:     const ContainerMetadata& removeMetadata
	// author:    zhengyao
	// remarks:   异常情况下由函数抛出错误码
	//************************************
	CSSPResult	removeContainerMetadata(const ContainerMetadata& removeMetadata);
	
	//************************************
	// method:	listObjects
	// brief:	
	// access:	public  
	// returns:	cssp::CSSPResult 
	// param:	int limit	- [in]
	// param:	const std::string & prefix	- [in]
	// param:	const std::string & delimiter	- [in]
	// param:	const std::string & marker	- [in]
	// param:	std::vector<ObjectMetadata> - [out]
	// author:	zhengyao
	// remarks:	
	//************************************
	 CSSPResult listObjects(int limit, const std::string& prefix, const std::string& delimiter, const std::string& marker, std::vector<ObjectMetadata*>& metaVector);

	/****Object Interface****/

	//************************************
	// method:    objectExists
	// brief:     判断Object是否存在
	// access:    public 
	// returns:   cssp::CSSPResult
	// param:     const std::string & object
	// author:    zhengyao
	// remarks:   
	//************************************
	CSSPResult	objectExists(const std::string& object);


	//************************************
	// method:	putObject
	// brief:	上传object
	// access:	private 
	// returns:	cssp::CSSPResult
	// param:	const std::string & objectname	- [in]
	// param:	read_data_ptr putObjectCallback	- [in]
	// param:	void * inputstream	- [in]
	// param:	const char * md5	- [in]
	// param:	ObjectMetadata * outmetadata	- [out]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult	putObject(const std::string& objectname, read_data_ptr putObjectCallback, void* inputstream, const char* md5, ObjectMetadata* outmetadata);


	//************************************
	// method:	getObject
	// brief:	获取object
	// access:	public 
	// returns:	cssp::CSSPResult
	// param:	const std::string & objectname	- [in]
	// param:	SwiftObject & object	- [out]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult	getObject(const std::string& objectname, SwiftObject& object);

	
	//************************************
	// method:	getObject
	// brief:	获取object
	// access:	public 
	// returns:	cssp::CSSPResult
	// param:	const std::string & objectname	- [in]
	// param:	write_data_ptr getObjectCallback	- [in]
	// param:	void * outputstream	- [in,out]
	// param:	uint64_t offset	- [in]
	// param:	int64_t & size	- [in,out]
	// param:	ObjectMetadata * outmetadata	- [out]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult getObject(const std::string& objectname, write_data_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t& size, ObjectMetadata* outmetadata);


	//************************************
	// method:	removeObject
	// brief:	移除object
	// access:	private 
	// returns:	cssp::CSSPResult
	// param:	const std::string & object	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult	removeObject(const std::string& object);


	//************************************
	// method:	copyObject
	// brief:	cssp::SwiftClient::copyObject
	// access:	public 
	// returns:	CSSPResult
	// param:	const std::string & srcObject	- []
	// param:	const std::string & dstContainer	- []
	// param:	const std::string & dstObject	- []
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult	copyObject(const std::string& srcObject, const std::string& dstContainer, const std::string& dstObject);
	

	//************************************
	// method:	setObjectMetadata
	// brief:	设置object Metadata
	// access:	public 
	// returns:	cssp::CSSPResult
	// param:	const std::string & object	- [in]
	// param:	const ObjectMetadata & userMetadata	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult	setObjectMetadata(const std::string& object, const ObjectMetadata& userMetadata);
	
	
	//************************************
	// method:	getObjectMetadata
	// brief:	获取object Metadata
	// access:	public 
	// returns:	cssp::CSSPResult
	// param:	const std::string & object	- [in]
	// param:	ObjectMetadata & metadata	- [out]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult	getObjectMetadata(const std::string& object, ObjectMetadata& metadata);

	
	//************************************
	// method:	multipartUploadInit
	// brief:	初始化分片上传
	// access:	public 
	// returns:	std::string
	// param:	const std::string & object	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	std::string multipartUploadInit(const std::string& object);


	//************************************
	// method:	multipartUploadPart
	// brief:	上传分片
	// access:	public 
	// returns:	cssp::CSSPResult
	// param:	const std::string & upid	- [in]
	// param:	int partnumber	- [in]
	// param:	read_data_ptr putObjectCallback	- [in]
	// param:	void * inputstream	- [in,out]
	// param:	const char * md5	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult multipartUploadPart(const std::string& upid, int partnumber, read_data_ptr putObjectCallback, void* inputstream, const char* md5);


	//************************************
	// method:	multipartUploadListParts
	// brief:	枚举已上传分片
	// access:	public 
	// returns:	cssp::CSSPResult
	// param:	const std::string & upid	- [in]
	// param:	int limit	- [in]
	// param:	const std::string & marker	- [in]
	// param:	std::vector<ObjectMetadata * > & metaVector	- [out]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult multipartUploadListParts(const std::string& upid, int limit, const std::string& marker, std::vector<ObjectMetadata*>& metaVector);


	//************************************
	// method:	multipartUploadComplete
	// brief:	完成本次分片上传
	// access:	public 
	// returns:	cssp::CSSPResult
	// param:	const std::string & upid	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult multipartUploadComplete(const std::string& upid);


	//************************************
	// method:	multipartUploadAbort
	// brief:	取消本次分片上传
	// access:	public 
	// returns:	CSSPResult
	// param:	const std::string & upid	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult multipartUploadAbort(const std::string& upid);
private:
	std::string generateUploadId(const std::string& object);
	inline std::string containerUrl(const std::string& container);
	inline std::string objectUrl(const std::string& container, const std::string& object);
public:
	static size_t libcurl_writedata_function(void* buffer, size_t size, size_t nmemb, void* userp);
	static size_t readMemoryStream(void* buffer, size_t size, size_t nmemb, void* userp);
	static size_t writeMemoryStream(void* buffer, size_t size, size_t nmemb, void* userp);
private:
	int				timeout_ms_;
	std::string		accessKeyId_;
	std::string		accessKeySecret_;
	std::string		containerUrl_;
private:
	//key为UploadId，value为 "container/object"
	std::map<std::string, std::string>	multi_uploads_;
};

}

#endif // __CSSP__SWIFTCLIENT_H__