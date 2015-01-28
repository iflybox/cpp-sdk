
#define IFLY_EXPORTS
#include "common.h"
#include "iflybox.h"
#include "swiftClient.h"
#include "error.h"
#include "http_status_code.h"
using iflybox::SwiftClient;
using iflybox::AccountMetadata;
using iflybox::Metadata;
using iflybox::ContainerMetadata;
using iflybox::ObjectMetadata;
using iflybox::SwiftObject;

//复制iflybox_result
inline void ILFYBOX_ASSIGN_RESULT(iflyboxResult result, const iflybox::iflyboxResult& iflyResult){
	if(result){
		reinterpret_cast<iflybox::iflyboxResult*>(result)->operator = (iflyResult);
	}
}

//从iflyException提取iflyboxResult
inline void IFLYBOX_EXTRACT_RESULT(iflyboxResult result, const iflybox::iflyException& exception){
	if(result){
		reinterpret_cast<iflybox::iflyboxResult*>(result)->setStatus(exception.error());
		reinterpret_cast<iflybox::iflyboxResult*>(result)->setDetail(exception.detail());
	}
}



iflyboxResult iflybox_init_result(){
	return new iflybox::iflyboxResult;
}

void iflybox_release_result(iflyboxResult result){
	delete reinterpret_cast<iflybox::iflyboxResult*>(result);
}

int iflybox_result_statuscode(iflyboxResult result){
	IFLY_ASSERT(result);
	return reinterpret_cast<iflybox::iflyboxResult*>(result)->getStatus();
}

int iflybox_is_successful(int statuscode){
	return is_ok(statuscode) || statuscode == 0;
}

const char* iflybox_result_detail(iflyboxResult result){
	IFLY_ASSERT(result);
	return reinterpret_cast<iflybox::iflyboxResult*>(result)->getDetail().c_str();
}

iBoxHandle iflybox_create_instance(const char* endpoint, const char* accessKeyId, const char* accessKeySecret, int endpoint_type, iflyboxResult result){
	IFLY_ASSERT(endpoint);
	IFLY_ASSERT(accessKeyId);
	IFLY_ASSERT(accessKeySecret);
	SwiftClient* swiftClient = new SwiftClient(endpoint, accessKeyId, accessKeySecret, endpoint_type);
	if(swiftClient){
		try{
			iflybox::iflyboxResult iflyResult = swiftClient->auth();
			ILFYBOX_ASSIGN_RESULT(result, iflyResult);
			if(iflyResult.getStatus() < HTTP_OK || iflyResult.getStatus() >= HTTP_MULTIPLE_CHOICES){
				delete swiftClient;
				return NULL;
			}
		}catch(const iflybox::iflyException& e){
			IFLYBOX_EXTRACT_RESULT(result, e);
			delete swiftClient;
			return NULL;
		}
	}
	else{
		iflybox::iflyboxResult iflyResult;
		iflyResult.setStatus(ERROR_MALLOC_BAD);
		iflyResult.setDetail("new SwiftClient return NULL");
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
	}
	return swiftClient;
}
int iflybox_reauth_account(iBoxHandle hdl, iflyboxResult result){
	int ret = -1;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	if(swiftClient){
		try{
			iflybox::iflyboxResult iflyResult = swiftClient->auth();
			ILFYBOX_ASSIGN_RESULT(result, iflyResult);
			ret = iflyResult.getStatus();
			if(iflyResult.getStatus() < HTTP_OK || iflyResult.getStatus() >= HTTP_MULTIPLE_CHOICES){
				return ret;
			}
		}catch(const iflybox::iflyException& e){
			IFLYBOX_EXTRACT_RESULT(result, e);
			return ret;
		}
	}
	else{
		iflybox::iflyboxResult iflyResult;
		iflyResult.setStatus(ERROR_PARAM_WRONG);
		iflyResult.setDetail("SwiftClient is NULL");
		ret = iflyResult.getStatus();
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
	}
	return ret;
}

int iflybox_release_instance(iBoxHandle hdl){
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	delete swiftClient;
	return 0;
}


int iflybox_set_timeout(iBoxHandle hdl, int timeout){
	IFLY_ASSERT(hdl);
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	int pre_timeout = swiftClient->getTimeout();
	if(timeout > 0){
		swiftClient->setTimeout(timeout);
	}
	return pre_timeout;
}


int iflybox_list_buckets(const iBoxHandle hdl, BucketMetaHandle* metahdls, int* metahdlcnt, const char* marker, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(metahdlcnt);
	IFLY_ASSERT(metahdls);
	if (*metahdlcnt < 0 || !metahdls || *metahdlcnt > 10000)
	{
		int ret = HTTP_PRECONDITION_FAILED;
		iflybox::iflyboxResult iflyResult;
		iflyResult.setStatus(HTTP_PRECONDITION_FAILED);
		iflyResult.setDetail("Parameter Error");
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		return ret;
	}
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	std::string marker_str;
	if(marker != NULL)
		marker_str = marker;
	std::vector<ContainerMetadata*> metaVector;
	try
	{
		iflybox::iflyboxResult  iflyResult = swiftClient->listContainers(*metahdlcnt, marker_str, metaVector);
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		*metahdlcnt = metaVector.size();
		if(*metahdlcnt)
			memcpy(metahdls, &metaVector[0], metaVector.size() * sizeof(ContainerMetadata*));
		
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		*metahdlcnt = 0;
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	
	return ret;
}

int iflybox_set_accountmeta(const iBoxHandle hdl, const AccountMetaHandle metadata, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(metadata);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult  iflyResult = swiftClient->setAccountMetadata(*reinterpret_cast<AccountMetadata*>(metadata));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	
	return ret;
}

int iflybox_get_accountmeta(const iBoxHandle hdl, AccountMetaHandle metadata, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(metadata);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult  iflyResult = swiftClient->getAccountMetadata(*reinterpret_cast<AccountMetadata*>(metadata));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


int iflybox_remove_accountmeta(const iBoxHandle hdl, const AccountMetaHandle metadata, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(metadata);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult  iflyResult = swiftClient->removeAccountMetadata(*reinterpret_cast<AccountMetadata*>(metadata));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

uint64_t iflybox_accountmeta_bucketcount(const AccountMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	AccountMetadata* metadata = reinterpret_cast<AccountMetadata*>(metahandle);
	return metadata->countContainer();
}

uint64_t iflybox_accountmeta_bytesused(const AccountMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	AccountMetadata* metadata = reinterpret_cast<AccountMetadata*>(metahandle);
	return metadata->bytesUsed();
} 

int iflybox_bucket_exist(const iBoxHandle hdl, const char* bucket, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult  iflyResult = swiftClient->containerExists(bucket);
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_create_bucket(const iBoxHandle hdl, const char* bucket, const BucketMetaHandle metahandle, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult;
		if(metahandle)
			iflyResult = swiftClient->createContainer(bucket, *reinterpret_cast<ContainerMetadata*>(metahandle));
		else
			iflyResult = swiftClient->createContainer(bucket);
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


int iflybox_delete_bucket_ifempty(const iBoxHandle hdl, const char* bucket, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->deleteContainerIfEmpty(bucket);
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_list_object(const iBoxHandle hdl, const char* bucket, ObjectMetaHandle* metahdls, int* limit, const char* prefix, const char* delimiter, const char* marker, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(metahdls);
	IFLY_ASSERT(limit);
	if (*limit < 0 || !bucket || *limit > 10000)
	{
		int ret = HTTP_PRECONDITION_FAILED;
		iflybox::iflyboxResult iflyResult;
		iflyResult.setStatus(HTTP_PRECONDITION_FAILED);
		iflyResult.setDetail("Parameter Error");
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		return ret;
	}
	int ret = IFLYBOX_SUCCESS;
	std::string prefix_str = prefix ? prefix : "";
	std::string delimiter_str = delimiter ? delimiter : "";
	std::string marker_str = marker ? marker : "";
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	std::vector<ObjectMetadata*> metaVector;
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->listObjects(bucket, *limit, prefix_str, delimiter_str, marker_str, metaVector);
		*limit = metaVector.size();
		if(*limit)
			memcpy(metahdls, &metaVector[0], metaVector.size() * sizeof(ObjectMetadata*));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		*limit = 0;
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_set_bucketmeta(const iBoxHandle hdl, const char* bucket, const BucketMetaHandle metadata, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->setContainerMetadata(bucket, *reinterpret_cast<ContainerMetadata*>(metadata));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_get_bucketmeta(const iBoxHandle hdl, const char* bucket, BucketMetaHandle metadata, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(metadata);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->getContainerMetadata(bucket, *reinterpret_cast<ContainerMetadata*>(metadata));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_remove_bucketmeta(const iBoxHandle hdl, const char* bucket, const BucketMetaHandle removeMeta, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(removeMeta);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->removeContainerMetadata(bucket, *reinterpret_cast<ContainerMetadata*>(removeMeta));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

uint64_t iflybox_bucketmeta_objectcount(const BucketMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ContainerMetadata* metadata = reinterpret_cast<ContainerMetadata*>(metahandle);
	return metadata->countObject();
}

uint64_t iflybox_bucketmeta_bytesused(const BucketMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ContainerMetadata* metadata = reinterpret_cast<ContainerMetadata*>(metahandle);
	return metadata->bytesUsed();
}


int iflybox_object_exist(const iBoxHandle hdl, const char* bucket, const char* object, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(object);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->objectExists(bucket, object);
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

size_t iflybox_put_object_fromdisk_callback(void* buffer, size_t size, size_t nmemb, void* inputstream){
	FILE* fd = reinterpret_cast<FILE*>(inputstream);
	return fread(buffer, size, nmemb, fd) * size;
}

int iflybox_put_object(const iBoxHandle hdl, const char* bucket, const char* object, iflybox_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, ObjectMetaHandle outmetadata, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(object);
	int ret = IFLYBOX_SUCCESS;
	iflybox::iflyboxResult iflyResult;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		if(putObjectCallback == NULL){
			SwiftObject swiftObject;
			if(inputstream){
				swiftObject.write(reinterpret_cast<const char*>(inputstream),  size);
				iflyResult = swiftClient->putObject(bucket, object, iflybox::SwiftClient::readMemoryStream, (void*)&swiftObject, md5, reinterpret_cast<iflybox::ObjectMetadata*>(outmetadata));
			}
		}
		else{
			iflyResult = swiftClient->putObject(bucket, object, putObjectCallback, inputstream, md5, reinterpret_cast<iflybox::ObjectMetadata*>(outmetadata));
		}
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

size_t iflybox_get_object_todisk_callback(void* buffer, size_t size, size_t nmemb, void* outputstream){
	FILE* fd = (FILE*)outputstream;
	return fwrite(buffer, size, nmemb, fd) * size;
}

int iflybox_get_object(const iBoxHandle hdl, const char* bucket, const char* object, iflybox_get_object_callback_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t* size, ObjectMetaHandle outmetadata, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	int ret = IFLYBOX_SUCCESS;
	SwiftObject swiftObject;
	iflybox::iflyboxResult iflyResult;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		if(getObjectCallback == NULL){
			SwiftObject swiftObject;
			iflyResult = swiftClient->getObject(bucket, object, iflybox::SwiftClient::writeMemoryStream, &swiftObject, offset, *size, reinterpret_cast<iflybox::ObjectMetadata*>(outmetadata));
			*size = swiftObject.str().size();
			swiftObject.read(reinterpret_cast<char*>(outputstream), *size);
		}
		else{
			iflyResult = swiftClient->getObject(bucket, object, getObjectCallback, outputstream, offset, *size, reinterpret_cast<iflybox::ObjectMetadata*>(outmetadata));
		}
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_remove_object(const iBoxHandle hdl, const char* bucket, const char* object, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(object);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->removeObject(bucket, object);
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_copy_object(const iBoxHandle hdl, const char* srcBucket, const char* srcObject, const char* dstBucket, const char* dstObject, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(srcBucket);
	IFLY_ASSERT(srcObject);
	IFLY_ASSERT(dstBucket);
	IFLY_ASSERT(dstObject);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->copyObject(srcBucket, srcObject, dstBucket, dstObject);
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_move_object(const iBoxHandle hdl, const char* srcBucket, const char* srcObject, const char* dstBucket, const char* dstObject, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(srcBucket);
	IFLY_ASSERT(srcObject);
	IFLY_ASSERT(dstBucket);
	IFLY_ASSERT(dstObject);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->copyObject(srcBucket, srcObject, dstBucket, dstObject);
		if(iflyResult.getStatus() >= HTTP_OK && iflyResult.getStatus() < HTTP_MULTIPLE_CHOICES){
			iflyResult = swiftClient->removeObject(srcBucket, srcObject);
		}
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_set_objectmeta(const iBoxHandle hdl, const char* bucket, const char* object, const ObjectMetaHandle metadata, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(object);
	IFLY_ASSERT(metadata);
	int ret = IFLYBOX_SUCCESS;

	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->setObjectMetadata(bucket, object, *reinterpret_cast<ObjectMetadata*>(metadata));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_get_objectmeta(const iBoxHandle hdl, const char* bucket, const char* object, ObjectMetaHandle metadata, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(object);
	IFLY_ASSERT(metadata);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->getObjectMetadata(bucket, object, *reinterpret_cast<ObjectMetadata*>(metadata));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


int iflybox_remove_objectmeta(const iBoxHandle hdl, const char* bucket, const char* object, const ObjectMetaHandle removeMeta, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(object);
	IFLY_ASSERT(removeMeta);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->removeObjectMetadata(bucket, object, *reinterpret_cast<ObjectMetadata*>(removeMeta));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


bool iflybox_objectmeta_is_pseudodirectory(const ObjectMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ObjectMetadata* metadata = reinterpret_cast<ObjectMetadata*>(metahandle);
	return metadata->isPseudoDirectory();
}


uint64_t iflybox_objectmeta_contentlength(const ObjectMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ObjectMetadata* metadata = reinterpret_cast<ObjectMetadata*>(metahandle);
	return metadata->contentLength();
}


const char* iflybox_objectmeta_etag(const ObjectMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ObjectMetadata* metadata = reinterpret_cast<ObjectMetadata*>(metahandle);
	return metadata->Etag();
}


const char* iflybox_objectmeta_lastmodify(const ObjectMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ObjectMetadata* metadata = reinterpret_cast<ObjectMetadata*>(metahandle);
	return metadata->lastModified();
}


int iflybox_multipart_upload_init(const iBoxHandle hdl, const char* bucket, const char* object, UploadId upid){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(bucket);
	IFLY_ASSERT(object);
	IFLY_ASSERT(upid);

	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	std::string uploadId = swiftClient->multipartUploadInit(bucket, object);
	if(uploadId.empty())
		return IFLYBOX_ERROR_UNKNOW;
	memset(upid, 0, UPLOADID_LEN + 1);
	uploadId.copy(upid, UPLOADID_LEN, 0);
	return IFLYBOX_SUCCESS;
}

int iflybox_multipart_upload_part(const iBoxHandle hdl, const UploadId upid, int partnumber, iflybox_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(upid);

	int ret = IFLYBOX_SUCCESS;
	iflybox::iflyboxResult iflyResult;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		if(putObjectCallback == NULL){
			SwiftObject swiftObject;
			if(inputstream){
				swiftObject.write(reinterpret_cast<const char*>(inputstream),  size);
				iflyResult = swiftClient->multipartUploadPart(upid, partnumber, iflybox::SwiftClient::readMemoryStream, (void*)&swiftObject, md5);
			}
		}
		else{
			iflyResult = swiftClient->multipartUploadPart(upid, partnumber, putObjectCallback, inputstream, md5);
		}
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;	
}

int iflybox_multipart_upload_list_parts(const iBoxHandle hdl, const UploadId upid, ObjectMetaHandle* metahdls, int* limit, const char* marker, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(upid);
	if (*limit < 0)
	{
		int ret = HTTP_PRECONDITION_FAILED;
		iflybox::iflyboxResult iflyResult;
		iflyResult.setStatus(HTTP_PRECONDITION_FAILED);
		iflyResult.setDetail("Parameter Error");
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		return ret;
	}
	int ret = IFLYBOX_SUCCESS;
	std::string marker_str = marker ? marker : "";
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	std::vector<ObjectMetadata*> metaVector;
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->multipartUploadListParts(upid, *limit, marker_str, metaVector);
		*limit = metaVector.size();
		if(*limit)
			memcpy(metahdls, &metaVector[0], metaVector.size() * sizeof(ObjectMetadata*));
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		*limit = 0;
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


int iflybox_multipart_upload_complete(const iBoxHandle hdl, const UploadId upid, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(upid);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->multipartUploadComplete(upid);
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int iflybox_multipart_upload_abort(const iBoxHandle hdl, const UploadId upid, iflyboxResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(upid);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		iflybox::iflyboxResult iflyResult = swiftClient->multipartUploadAbort(upid);
		ILFYBOX_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const iflybox::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


AccountMetaHandle iflybox_accountmeta_create(){
	return new AccountMetadata;
}

void iflybox_accountmeta_release(const AccountMetaHandle metahandle){
	delete reinterpret_cast<AccountMetadata*>(metahandle);
}


BucketMetaHandle iflybox_bucketmeta_create(){
	return new ContainerMetadata;
}

void iflybox_bucketmeta_release(const BucketMetaHandle metahandle){
	delete reinterpret_cast<ContainerMetadata*>(metahandle);
}

ObjectMetaHandle iflybox_objectmeta_create(){
	return new ObjectMetadata;
}

void iflybox_objectmeta_release(const ObjectMetaHandle metahandle){
	delete reinterpret_cast<ObjectMetadata*>(metahandle);
}

const char* iflybox_metadata_ownername(const MetadataHandle metadhandle){
	IFLY_ASSERT(metadhandle);
	Metadata* metadata = reinterpret_cast<Metadata*>(metadhandle);
	return metadata->ownerName().c_str();
}

bool string_is_all_right(const char* s)
{
	for (int i = 0; i < strlen(s); ++i)
		if (!((s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')))
			return false;
	return true;
}

const char* iflybox_metadata_get(const MetadataHandle metahandle, const char* metaname){
	IFLY_ASSERT(metahandle);
	IFLY_ASSERT(metaname);	
	if (!string_is_all_right(metaname))
		return NULL;
	Metadata* metadata = reinterpret_cast<Metadata*>(metahandle);
	char metanamelow[300];
	strcpy(metanamelow, metaname);
	for (int i = 0; i < strlen(metanamelow); ++i)
	{
		if (metanamelow[i] >= 'A' && metanamelow[i] <= 'Z')
			metanamelow[i] = 'a' + (metaname[i] - 'A');
	}
	return metadata->getMeta(metanamelow);
}

int iflybox_metadata_set(const MetadataHandle metahandle, const char* metaname, const char* metavalue){
	IFLY_ASSERT(metahandle);
	IFLY_ASSERT(metaname);	
	IFLY_ASSERT(metavalue);
	if (!string_is_all_right(metaname) || !string_is_all_right(metavalue) || !strlen(metavalue) || !strlen(metaname))
		return -1;
	Metadata* metadata = reinterpret_cast<Metadata*>(metahandle);
	metadata->setMeta(metaname, metavalue);
	return 0;
}

int iflybox_metadata_count(const MetadataHandle metahandle){
	IFLY_ASSERT(metahandle);
	Metadata* metadata = reinterpret_cast<Metadata*>(metahandle);
	return metadata->countMeta();
}
