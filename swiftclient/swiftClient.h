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

	//����libcurl��write�ķ�װ�����ڷ�����ȷ���ݺʹ�����Ϣ
	typedef struct libcurl_write_struct_tag{
		write_data_ptr	getCallback;
		void*			user_stream;
		std::string*	response_content;
		CURL*			curl_handle;
	}libcurl_write_struct;

	//iflybox�з�����Ϣ
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
	// brief:	����account��metadata
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
	// brief:	�Ƴ��˺�metadata
	// access:	public  
	// returns:	void
	// param:	const AccountMetadata & removeMetadata	- [in]
	//			���Ƴ�metadata
	// author:	zhengyao
	// remarks:	
	//************************************
	iflyboxResult	removeAccountMetadata(const AccountMetadata& removeMetadata);
	
	
	//************************************
	// method:	listContainers
	// brief:	ö��containers
	// access:	public  
	// returns:	std::vector<ContainerMetadata> - ��������container��metadata
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
	// brief:	  �ж�container�Ƿ����
	// access:    public 
	// returns:   bool		
	// param:     const std::string & container
	// author:    zhengyao
	// remarks:   ����������������ɷ���ֵ��ʾ�Ƿ���ڣ��쳣����º����׳��쳣
	//************************************
	iflyboxResult	containerExists(const std::string& container);

	//************************************
	// method:    createContainer
	// brief��    ����Container
	// access:    public 
	// returns:   void
	// param:     const std::string & container
	// author:    zhengyao
	// remarks:   �쳣������ɺ����׳�������
	//************************************
	iflyboxResult	createContainer(const std::string& container);

	//************************************
	// method:  createContainer
	// brief:   ����Container
	// access:  public 
	// returns: void
	// param:   const std::string & container
	// param:   const ContainerMetadata& metadata
	//			����metadata��Ϣ
	// author:  zhengyao
	// remarks:   
	//************************************
	iflyboxResult	createContainer(const std::string& container, const ContainerMetadata& metadata);

	//************************************
	// method:    deleteContainerIfEmpty
	// brief:     ���ContainerΪ�գ���ɾ����Container
	// access:    public 
	// returns:   bool
	// param:     const std::string & container
	// author:    zhengyao
	// remarks:   �쳣������ɺ����׳�������
	//************************************
	iflyboxResult	deleteContainerIfEmpty(const std::string& container);

	//************************************
	// method:    setContainerMetadata
	// brief��    ����container��metainfo
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
	// brief:     ��ȡcontainer��metainfo
	// access:    public 
	// returns:   ContainerMetadata
	// param:     const std::string & container
	// author:    zhengyao
	// remarks:   
	//************************************
	iflyboxResult	getContainerMetadata(const std::string& container, ContainerMetadata& metadata);

	//************************************
	// method:    deleteContainerMetadata
	// brief:     ɾ��Container��ָ����metadata K-V��
	// access:    public 
	// returns:   bool
	// param:     const std::string & container
	// param:     const std::vector<string> & metadataKeys
	// author:    zhengyao
	// remarks:   �쳣������ɺ����׳�������
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
	// brief:     �ж�Object�Ƿ����
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
	// brief:     ��ȡָ��Object
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
	// brief:     �Ƴ�ָ��Object
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
	// brief:     ����Object
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
	// brief:     ����Object metainfo
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
	// brief:     ��ȡObject metainfo
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
	// brief:     ɾ��Object��ָ����metadata K-V��
	// access:    public 
	// returns:   bool
	// param:     const std::string & container
	// param:	  const std::string& object
	// param:     const ObjectMetadata& removeMetadata
	// author:    zhengyao
	// remarks:   �쳣������ɺ����׳�������
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
	//keyΪUploadId��valueΪ "container/object"
	std::map<std::string, std::string>	multi_uploads_;
};

}

#endif // __IFLYBOX__SWIFTCLIENT_H__