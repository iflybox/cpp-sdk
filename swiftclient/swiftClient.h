#ifndef __IFLYBOX__SWIFTCLIENT_H__
#define __IFLYBOX__SWIFTCLIENT_H__
#include "exception.h"
#include <string>
#include <vector>
#include "metadata.h"
#include "httpHelper.h"
#include "http_status_code.h"
#include <sstream>

namespace iflybox
{

	class SwiftObject : public std::stringstream{};

	//用于libcurl中write的封装，用于分离正确数据和错误信息
	typedef struct libcurl_write_struct_tag{
		write_data_ptr	getCallback;
		void*			user_stream;
		std::string*	response_content;
		CURL*			curl_handle;
	}libcurl_write_struct;

	//iflybox中返回信息
	class iflyboxResult{
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
	SwiftClient(const std::string& endpoint, const std::string& accessKeyId, const std::string& accessKeySecret, int endpoint_type, int timeout = 30000);
	int getTimeout();
	void setTimeout(int timeout);
	iflyboxResult auth();
	/*Account Interface*/
	
	//************************************
	// method:	setAccountMetadata
	// brief:	设置account的metadata
	// access:	public  
	// returns:	void - 
	// param:	const AccountMetadata & accountMetadata	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	iflyboxResult setAccountMetadata(const AccountMetadata& accountMetadata);
	
	//************************************
	// method:	getAccountMetadata
	// brief:	
	// access:	public  
	// returns:	iflyboxResult - 
	// author:	zhengyao
	// remarks:	
	//************************************
	iflyboxResult	getAccountMetadata(AccountMetadata& accountMetadata);

	//************************************
	// method:	removeAccountMetadata
	// brief:	移除账号metadata
	// access:	public  
	// returns:	void
	// param:	const AccountMetadata & removeMetadata	- [in]
	//			待移除metadata
	// author:	zhengyao
	// remarks:	
	//************************************
	iflyboxResult	removeAccountMetadata(const AccountMetadata& removeMetadata);
	
	
	//************************************
	// method:	listContainers
	// brief:	枚举containers
	// access:	public  
	// returns:	std::vector<ContainerMetadata> - 返回所有container的metadata
	// param:	int limit	- [in]
	// param:	const std::string & prefix	- [in]
	// param:	const std::string & delimiter	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	iflyboxResult	listContainers(int limit, const std::string& marker, std::vector<ContainerMetadata*>& metaVector);

	/*Container Interface*/

	//************************************
	// method:    containerExists
	// brief:	  判断container是否存在
	// access:    public 
	// returns:   bool		
	// param:     const std::string & container
	// author:    zhengyao
	// remarks:   函数在正常情况下由返回值表示是否存在，异常情况下函数抛出异常
	//************************************
	iflyboxResult	containerExists(const std::string& container);

	//************************************
	// method:    createContainer
	// brief：    创建Container
	// access:    public 
	// returns:   void
	// param:     const std::string & container
	// author:    zhengyao
	// remarks:   异常情况下由函数抛出错误码
	//************************************
	iflyboxResult	createContainer(const std::string& container);

	//************************************
	// method:  createContainer
	// brief:   创建Container
	// access:  public 
	// returns: void
	// param:   const std::string & container
	// param:   const ContainerMetadata& metadata
	//			附加metadata信息
	// author:  zhengyao
	// remarks:   
	//************************************
	iflyboxResult	createContainer(const std::string& container, const ContainerMetadata& metadata);

	//************************************
	// method:    deleteContainerIfEmpty
	// brief:     如果Container为空，则删除该Container
	// access:    public 
	// returns:   bool
	// param:     const std::string & container
	// author:    zhengyao
	// remarks:   异常情况下由函数抛出错误码
	//************************************
	iflyboxResult	deleteContainerIfEmpty(const std::string& container);

	//************************************
	// method:    setContainerMetadata
	// brief：    设置container的metainfo
	// access:    public 
	// returns:   void
	// param:     const std::string & container
	// param:     const std::map<std::string
	// param:     std::string> & containerMetadata
	// author:    zhengyao
	// remarks:   
	//************************************
	iflyboxResult	setContainerMetadata(const std::string& container, const ContainerMetadata& containerMetadata);

	//************************************
	// method:    getContainerMetadata
	// brief:     获取container的metainfo
	// access:    public 
	// returns:   ContainerMetadata
	// param:     const std::string & container
	// author:    zhengyao
	// remarks:   
	//************************************
	iflyboxResult	getContainerMetadata(const std::string& container, ContainerMetadata& metadata);

	//************************************
	// method:    deleteContainerMetadata
	// brief:     删除Container中指定的metadata K-V对
	// access:    public 
	// returns:   bool
	// param:     const std::string & container
	// param:     const std::vector<string> & metadataKeys
	// author:    zhengyao
	// remarks:   异常情况下由函数抛出错误码
	//************************************
	iflyboxResult	removeContainerMetadata(const std::string& container, const ContainerMetadata& removeMetadata);
	
	//************************************
	// method:	listObjects
	// brief:	
	// access:	public  
	// returns:	std::vector<ObjectMetadata> - 
	// param:	int limit	- [in]
	// param:	const std::string & prefix	- [in]
	// param:	const std::string & delimiter	- [in]
	// param:	const std::string & marker	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	 iflyboxResult listObjects(const std::string& container, int limit, const std::string& prefix, const std::string& delimiter, const std::string& marker, std::vector<ObjectMetadata*>& metaVector);

	/****Object Interface****/

	//************************************
	// method:    objectExists
	// brief:     判断Object是否存在
	// access:    public 
	// returns:   bool
	// param:     const std::string & container
	// param:     const std::string & object
	// author:    zhengyao
	// remarks:   
	//************************************
	iflyboxResult	objectExists(const std::string& container, const std::string& object);


	iflyboxResult	putObject(const std::string& container, const std::string& objectname, read_data_ptr putObjectCallback, void* inputstream, const char* md5, ObjectMetadata* outmetadata);

	//************************************
	// method:    getObject
	// brief:     获取指定Object
	// access:    public 
	// returns:   SwiftObject
	// param:     const std::string & container
	// param:     const std::string & object
	// param:     GetOptions... options
	// author:    zhengyao
	// remarks:   
	//************************************
	iflyboxResult	getObject(const std::string& container, const std::string& objectname, SwiftObject& object);

	
	iflyboxResult getObject(const std::string& container, const std::string& objectname, write_data_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t& size, ObjectMetadata* outmetadata);

	//************************************
	// method:    removeObject
	// brief:     移除指定Object
	// access:    public 
	// returns:   void
	// param:     const std::string & container
	// param:     const std::string & object
	// author:    zhengyao
	// remarks:   
	//************************************
	iflyboxResult	removeObject(const std::string& container, const std::string& object);

	

	//************************************
	// method:    copyObject
	// brief:     拷贝Object
	// access:    public 
	// returns:   bool
	// param:     const std::string & sourceContainer
	// param:     const std::string & sourceObject
	// param:     const std::string & destinationContainer
	// param:     const std::string & destinationObject
	// author:    zhengyao
	// remarks:   destination container must exist
	//************************************
	iflyboxResult	copyObject(const std::string& sourceContainer, const std::string& sourceObject, const std::string& destinationContainer, const std::string& destinationObject);
	
	//************************************
	// method:    setObjectInfo
	// brief:     设置Object metainfo
	// access:    public 
	// returns:   bool
	// param:     const std::string & container
	// param:     const string & name
	// param:     const std::map<std::string
	// param:     std::string> & userMetadata
	// author:    zhengyao
	// remarks:   
	//************************************
	iflyboxResult	setObjectMetadata(const std::string& container, const std::string& object, const ObjectMetadata& userMetadata);
	
	
	//************************************
	// method:    getObjectInfo
	// brief:     获取Object metainfo
	// access:    public 
	// returns:   ObjectMetadata
	// param:     const std::string & container
	// param:     const std::string & object
	// author:    zhengyao
	// remarks:   
	//************************************
	iflyboxResult	getObjectMetadata(const std::string& container, const std::string& object, ObjectMetadata& metadata);

	//************************************
	// method:    removeObjectMetadata
	// brief:     删除Object中指定的metadata K-V对
	// access:    public 
	// returns:   bool
	// param:     const std::string & container
	// param:	  const std::string& object
	// param:     const ObjectMetadata& removeMetadata
	// author:    zhengyao
	// remarks:   异常情况下由函数抛出错误码
	//************************************
	iflyboxResult	removeObjectMetadata(const std::string& container, const std::string& object, const ObjectMetadata& removeMetadata);
	
	std::string multipartUploadInit(const std::string& container, const std::string& object);

	iflyboxResult multipartUploadPart(const std::string& upid, int partnumber, read_data_ptr putObjectCallback, void* inputstream, const char* md5);

	iflyboxResult multipartUploadListParts(const std::string& upid, int limit, const std::string& marker, std::vector<ObjectMetadata*>& metaVector);

	iflyboxResult multipartUploadComplete(const std::string& upid);

	iflyboxResult multipartUploadAbort(const std::string& upid);
private:
	iflyboxResult tempauth(const std::string& endpoint, const std::string& accessKeyId, const std::string& accessKeySecret);
	iflyboxResult keystone(const std::string& endpoint, const std::string& accessKeyId, const std::string& accessKeySecret, int endpoint_type);
	std::string generateUploadId(const std::string& container, const std::string& object);
	iflyboxResult	removeObject(const std::string& path);
	iflyboxResult	putObject(const std::string& path, read_data_ptr putObjectCallback, void* inputstream, const char* md5, ObjectMetadata* outmetadata);
	inline std::string accountUrl();
	inline std::string containerUrl(const std::string& container);
	inline std::string objectUrl(const std::string& container, const std::string& object);
public:
	static size_t libcurl_writedata_function(void* buffer, size_t size, size_t nmemb, void* userp);
	static size_t readMemoryStream(void* buffer, size_t size, size_t nmemb, void* userp);
	static size_t writeMemoryStream(void* buffer, size_t size, size_t nmemb, void* userp);
private:
	int				timeout_ms_;
	std::string		auth_endpoint_;
	std::string		accessKeyId_;
	std::string		accessKeySecret_;
	std::string		accessToken_;
	std::string		storageUrl_;
	int				endpoint_type_;
private:
	//key为UploadId，value为 "container/object"
	std::map<std::string, std::string>	multi_uploads_;
};

}

#endif // __IFLYBOX__SWIFTCLIENT_H__