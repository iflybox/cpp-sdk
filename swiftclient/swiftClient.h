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

	//����libcurl��write�ķ�װ�����ڷ�����ȷ���ݺʹ�����Ϣ
	typedef struct libcurl_write_struct_tag{
		write_data_ptr	getCallback;
		void*			user_stream;
		std::string*	response_content;
		CURL*			curl_handle;
	}libcurl_write_struct;

	//cssp�з�����Ϣ
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
	// brief:	  �ж�container�Ƿ����
	// access:    public 
	// returns:   cssp::CSSPResult		
	// author:    zhengyao
	// remarks:   ����������������ɷ���ֵ��ʾ�Ƿ���ڣ��쳣����º����׳��쳣
	//************************************
	CSSPResult	containerExists();

	//************************************
	// method:    deleteContainerIfEmpty
	// brief:     ���ContainerΪ�գ���ɾ����Container
	// access:    public 
	// returns:   cssp::CSSPResult
	// author:    zhengyao
	// remarks:   �쳣������ɺ����׳�������
	//************************************
	CSSPResult	deleteContainerIfEmpty();

	//************************************
	// method:	setContainerSysmeta
	// brief:	���û��Ƴ�container��ϵͳ����
	// access:	public 
	// returns:	CSSPResult
	// param:	const std::string & sysKey	- []
	// param:	const std::string & sysValue	- []
	// author:	zhengyao
	// remarks:	�ú�����ͬʱ�ṩ���û��Ƴ����ܣ�ͨ��sysKey�����幦��
	//			����X-Container-* ��X-Remove-Container-* �ֱ��ʾ���ú��Ƴ�
	//************************************
	CSSPResult setContainerSysmeta(const std::string& sysKey, const std::string& sysValue);


	//************************************
	// method:    setContainerMetadata
	// brief��    ����container��metainfo
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
	// brief:     ��ȡcontainer��metainfo
	// access:    public 
	// returns:   cssp::CSSPResult
	// param:     ContainerMetadata& metadata
	// author:    zhengyao
	// remarks:   
	//************************************
	CSSPResult	getContainerMetadata(ContainerMetadata& metadata);

	//************************************
	// method:    deleteContainerMetadata
	// brief:     ɾ��Container��ָ����metadata K-V��
	// access:    public 
	// returns:   cssp::CSSPResult
	// param:     const ContainerMetadata& removeMetadata
	// author:    zhengyao
	// remarks:   �쳣������ɺ����׳�������
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
	// brief:     �ж�Object�Ƿ����
	// access:    public 
	// returns:   cssp::CSSPResult
	// param:     const std::string & object
	// author:    zhengyao
	// remarks:   
	//************************************
	CSSPResult	objectExists(const std::string& object);


	//************************************
	// method:	putObject
	// brief:	�ϴ�object
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
	// brief:	��ȡobject
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
	// brief:	��ȡobject
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
	// brief:	�Ƴ�object
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
	// brief:	����object Metadata
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
	// brief:	��ȡobject Metadata
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
	// brief:	��ʼ����Ƭ�ϴ�
	// access:	public 
	// returns:	std::string
	// param:	const std::string & object	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	std::string multipartUploadInit(const std::string& object);


	//************************************
	// method:	multipartUploadPart
	// brief:	�ϴ���Ƭ
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
	// brief:	ö�����ϴ���Ƭ
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
	// brief:	��ɱ��η�Ƭ�ϴ�
	// access:	public 
	// returns:	cssp::CSSPResult
	// param:	const std::string & upid	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	CSSPResult multipartUploadComplete(const std::string& upid);


	//************************************
	// method:	multipartUploadAbort
	// brief:	ȡ�����η�Ƭ�ϴ�
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
	//keyΪUploadId��valueΪ "container/object"
	std::map<std::string, std::string>	multi_uploads_;
};

}

#endif // __CSSP__SWIFTCLIENT_H__