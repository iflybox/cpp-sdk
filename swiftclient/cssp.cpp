
#define IFLY_EXPORTS
#include "common.h"
#include "cssp.h"
#include "swiftClient.h"
#include "error.h"
#include "http_status_code.h"
using cssp::SwiftClient;
using cssp::AccountMetadata;
using cssp::Metadata;
using cssp::ContainerMetadata;
using cssp::ObjectMetadata;
using cssp::SwiftObject;

//复制cssp_result
inline void CSSP_ASSIGN_RESULT(CSSPResult result, const cssp::CSSPResult& iflyResult){
	if(result){
		reinterpret_cast<cssp::CSSPResult*>(result)->operator = (iflyResult);
	}
}

//从iflyException提取CSSPResult
inline void IFLYBOX_EXTRACT_RESULT(CSSPResult result, const cssp::iflyException& exception){
	if(result){
		reinterpret_cast<cssp::CSSPResult*>(result)->setStatus(exception.error());
		reinterpret_cast<cssp::CSSPResult*>(result)->setDetail(exception.detail());
	}
}



CSSPResult cssp_init_result(){
	return new cssp::CSSPResult;
}

void cssp_release_result(CSSPResult result){
	delete reinterpret_cast<cssp::CSSPResult*>(result);
}

int cssp_result_statuscode(CSSPResult result){
	IFLY_ASSERT(result);
	return reinterpret_cast<cssp::CSSPResult*>(result)->getStatus();
}

int cssp_is_successful(int statuscode){
	return is_ok(statuscode) || statuscode == 0;
}

const char* cssp_result_detail(CSSPResult result){
	IFLY_ASSERT(result);
	return reinterpret_cast<cssp::CSSPResult*>(result)->getDetail().c_str();
}

CSSPHandle cssp_create_instance(const char* endpoint, const char* accessKeyId, const char* accessKeySecret, CSSPResult result){
	IFLY_ASSERT(endpoint);
	IFLY_ASSERT(accessKeyId);
	IFLY_ASSERT(accessKeySecret);
	SwiftClient* swiftClient = new SwiftClient(endpoint, accessKeyId, accessKeySecret);
	if(swiftClient == NULL){
		cssp::CSSPResult iflyResult;
		iflyResult.setStatus(ERROR_MALLOC_BAD);
		iflyResult.setDetail("new SwiftClient return NULL");
		CSSP_ASSIGN_RESULT(result, iflyResult);
	}
	return swiftClient;
}


int cssp_release_instance(CSSPHandle hdl){
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	delete swiftClient;
	return 0;
}


int cssp_set_timeout(CSSPHandle hdl, int timeout){
	IFLY_ASSERT(hdl);
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	int pre_timeout = swiftClient->getTimeout();
	if(timeout > 0){
		swiftClient->setTimeout(timeout);
	}
	return pre_timeout;
}


int cssp_container_exist(const CSSPHandle hdl, CSSPResult result){
	IFLY_ASSERT(hdl);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult  iflyResult = swiftClient->containerExists();
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


int cssp_delete_container_ifempty(const CSSPHandle hdl, CSSPResult result){
	IFLY_ASSERT(hdl);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->deleteContainerIfEmpty();
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_list_object(const CSSPHandle hdl, ObjectMetaHandle* metahdls, int* limit, const char* prefix, const char* delimiter, const char* marker, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(metahdls);
	IFLY_ASSERT(limit);
	if (*limit < 0 || *limit > 10000)
	{
		int ret = HTTP_PRECONDITION_FAILED;
		cssp::CSSPResult iflyResult;
		iflyResult.setStatus(HTTP_PRECONDITION_FAILED);
		iflyResult.setDetail("Parameter Error");
		CSSP_ASSIGN_RESULT(result, iflyResult);
		return ret;
	}
	int ret = IFLYBOX_SUCCESS;
	std::string prefix_str = prefix ? prefix : "";
	std::string delimiter_str = delimiter ? delimiter : "";
	std::string marker_str = marker ? marker : "";
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	std::vector<ObjectMetadata*> metaVector;
	try{
		cssp::CSSPResult iflyResult = swiftClient->listObjects(*limit, prefix_str, delimiter_str, marker_str, metaVector);
		*limit = metaVector.size();
		if(*limit)
			memcpy(metahdls, &metaVector[0], metaVector.size() * sizeof(ObjectMetadata*));
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		*limit = 0;
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_set_containermeta(const CSSPHandle hdl, const ContainerMetaHandle metadata, CSSPResult result){
	IFLY_ASSERT(hdl);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->setContainerMetadata(*reinterpret_cast<ContainerMetadata*>(metadata));
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_get_containermeta(const CSSPHandle hdl, ContainerMetaHandle metadata, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(metadata);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->getContainerMetadata(*reinterpret_cast<ContainerMetadata*>(metadata));
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_remove_containermeta(const CSSPHandle hdl, const ContainerMetaHandle removeMeta, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(removeMeta);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->removeContainerMetadata(*reinterpret_cast<ContainerMetadata*>(removeMeta));
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_count_object(const CSSPHandle hdl, uint64_t* count, CSSPResult result){
	ContainerMetadata metadata;
	int status = cssp_get_containermeta(hdl, &metadata, result);
	if(cssp_is_successful(status)){
		*count = metadata.countObject();
	}
	return status;
}

int cssp_count_bytesused(const CSSPHandle hdl, uint64_t* bytesused, CSSPResult result){
	ContainerMetadata metadata;
	int status = cssp_get_containermeta(hdl, &metadata, result);
	if(cssp_is_successful(status)){
		*bytesused = metadata.bytesUsed();
	}
	return status;
}


int cssp_object_exist(const CSSPHandle hdl, const char* object, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(object);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->objectExists(object);
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

size_t cssp_put_object_fromdisk_callback(void* buffer, size_t size, size_t nmemb, void* inputstream){
	FILE* fd = reinterpret_cast<FILE*>(inputstream);
	return fread(buffer, size, nmemb, fd) * size;
}

int cssp_put_object(const CSSPHandle hdl, const char* object, cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, ObjectMetaHandle outmetadata, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(object);
	int ret = IFLYBOX_SUCCESS;
	cssp::CSSPResult iflyResult;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		if(putObjectCallback == NULL){
			SwiftObject swiftObject;
			if(inputstream){
				swiftObject.write(reinterpret_cast<const char*>(inputstream),  size);
				iflyResult = swiftClient->putObject(object, cssp::SwiftClient::readMemoryStream, (void*)&swiftObject, md5, reinterpret_cast<cssp::ObjectMetadata*>(outmetadata));
			}
		}
		else{
			iflyResult = swiftClient->putObject(object, putObjectCallback, inputstream, md5, reinterpret_cast<cssp::ObjectMetadata*>(outmetadata));
		}
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

size_t cssp_get_object_todisk_callback(void* buffer, size_t size, size_t nmemb, void* outputstream){
	FILE* fd = (FILE*)outputstream;
	return fwrite(buffer, size, nmemb, fd) * size;
}

int cssp_get_object(const CSSPHandle hdl, const char* object, cssp_get_object_callback_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t* size, ObjectMetaHandle outmetadata, CSSPResult result){
	IFLY_ASSERT(hdl);
	int ret = IFLYBOX_SUCCESS;
	SwiftObject swiftObject;
	cssp::CSSPResult iflyResult;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		if(getObjectCallback == NULL){
			SwiftObject swiftObject;
			iflyResult = swiftClient->getObject(object, cssp::SwiftClient::writeMemoryStream, &swiftObject, offset, *size, reinterpret_cast<cssp::ObjectMetadata*>(outmetadata));
			*size = swiftObject.str().size();
			swiftObject.read(reinterpret_cast<char*>(outputstream), *size);
		}
		else{
			iflyResult = swiftClient->getObject(object, getObjectCallback, outputstream, offset, *size, reinterpret_cast<cssp::ObjectMetadata*>(outmetadata));
		}
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_remove_object(const CSSPHandle hdl, const char* object, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(object);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->removeObject(object);
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_copy_object(const CSSPHandle hdl, const char* srcObject, const char* dstObject, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(srcObject);
	IFLY_ASSERT(dstObject);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->copyObject(srcObject, dstObject);
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_move_object(const CSSPHandle hdl, const char* srcObject, const char* dstObject, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(srcObject);
	IFLY_ASSERT(dstObject);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->copyObject(srcObject, dstObject);
		if(iflyResult.getStatus() >= HTTP_OK && iflyResult.getStatus() < HTTP_MULTIPLE_CHOICES){
			iflyResult = swiftClient->removeObject(srcObject);
		}
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_set_objectmeta(const CSSPHandle hdl, const char* object, const ObjectMetaHandle metadata, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(object);
	IFLY_ASSERT(metadata);
	int ret = IFLYBOX_SUCCESS;

	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->setObjectMetadata(object, *reinterpret_cast<ObjectMetadata*>(metadata));
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_get_objectmeta(const CSSPHandle hdl, const char* object, ObjectMetaHandle metadata, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(object);
	IFLY_ASSERT(metadata);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->getObjectMetadata(object, *reinterpret_cast<ObjectMetadata*>(metadata));
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


bool cssp_objectmeta_is_pseudodirectory(const ObjectMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ObjectMetadata* metadata = reinterpret_cast<ObjectMetadata*>(metahandle);
	return metadata->isPseudoDirectory();
}


uint64_t cssp_objectmeta_contentlength(const ObjectMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ObjectMetadata* metadata = reinterpret_cast<ObjectMetadata*>(metahandle);
	return metadata->contentLength();
}


const char* cssp_objectmeta_etag(const ObjectMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ObjectMetadata* metadata = reinterpret_cast<ObjectMetadata*>(metahandle);
	return metadata->Etag();
}


const char* cssp_objectmeta_lastmodify(const ObjectMetaHandle metahandle){
	IFLY_ASSERT(metahandle);
	ObjectMetadata* metadata = reinterpret_cast<ObjectMetadata*>(metahandle);
	return metadata->lastModified();
}


int cssp_multipart_upload_init(const CSSPHandle hdl, const char* object, UploadId upid){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(object);
	IFLY_ASSERT(upid);

	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	std::string uploadId = swiftClient->multipartUploadInit(object);
	if(uploadId.empty())
		return IFLYBOX_ERROR_UNKNOW;
	memset(upid, 0, UPLOADID_LEN + 1);
	uploadId.copy(upid, UPLOADID_LEN, 0);
	return IFLYBOX_SUCCESS;
}

int cssp_multipart_upload_part(const CSSPHandle hdl, const UploadId upid, int partnumber, cssp_put_object_callback_ptr putObjectCallback, void* inputstream, uint64_t size, const char* md5, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(upid);

	int ret = IFLYBOX_SUCCESS;
	cssp::CSSPResult iflyResult;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		if(putObjectCallback == NULL){
			SwiftObject swiftObject;
			if(inputstream){
				swiftObject.write(reinterpret_cast<const char*>(inputstream),  size);
				iflyResult = swiftClient->multipartUploadPart(upid, partnumber, cssp::SwiftClient::readMemoryStream, (void*)&swiftObject, md5);
			}
		}
		else{
			iflyResult = swiftClient->multipartUploadPart(upid, partnumber, putObjectCallback, inputstream, md5);
		}
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;	
}

int cssp_multipart_upload_list_parts(const CSSPHandle hdl, const UploadId upid, ObjectMetaHandle* metahdls, int* limit, const char* marker, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(upid);
	if (*limit < 0)
	{
		int ret = HTTP_PRECONDITION_FAILED;
		cssp::CSSPResult iflyResult;
		iflyResult.setStatus(HTTP_PRECONDITION_FAILED);
		iflyResult.setDetail("Parameter Error");
		CSSP_ASSIGN_RESULT(result, iflyResult);
		return ret;
	}
	int ret = IFLYBOX_SUCCESS;
	std::string marker_str = marker ? marker : "";
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	std::vector<ObjectMetadata*> metaVector;
	try{
		cssp::CSSPResult iflyResult = swiftClient->multipartUploadListParts(upid, *limit, marker_str, metaVector);
		*limit = metaVector.size();
		if(*limit)
			memcpy(metahdls, &metaVector[0], metaVector.size() * sizeof(ObjectMetadata*));
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		*limit = 0;
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


int cssp_multipart_upload_complete(const CSSPHandle hdl, const UploadId upid, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(upid);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->multipartUploadComplete(upid);
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}

int cssp_multipart_upload_abort(const CSSPHandle hdl, const UploadId upid, CSSPResult result){
	IFLY_ASSERT(hdl);
	IFLY_ASSERT(upid);
	int ret = IFLYBOX_SUCCESS;
	SwiftClient* swiftClient = reinterpret_cast<SwiftClient*>(hdl);
	try{
		cssp::CSSPResult iflyResult = swiftClient->multipartUploadAbort(upid);
		CSSP_ASSIGN_RESULT(result, iflyResult);
		ret = iflyResult.getStatus();
	}catch(const cssp::iflyException& e){
		IFLYBOX_EXTRACT_RESULT(result, e);
		ret = e.error();
	}
	return ret;
}


ContainerMetaHandle cssp_containermeta_create(){
	return new ContainerMetadata;
}

void cssp_containermeta_release(const ContainerMetaHandle metahandle){
	delete reinterpret_cast<ContainerMetadata*>(metahandle);
}

ObjectMetaHandle cssp_objectmeta_create(){
	return new ObjectMetadata;
}

void cssp_objectmeta_release(const ObjectMetaHandle metahandle){
	delete reinterpret_cast<ObjectMetadata*>(metahandle);
}

const char* cssp_metadata_ownername(const MetadataHandle metadhandle){
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

const char* cssp_metadata_get(const MetadataHandle metahandle, const char* metaname){
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

int cssp_metadata_set(const MetadataHandle metahandle, const char* metaname, const char* metavalue){
	IFLY_ASSERT(metahandle);
	IFLY_ASSERT(metaname);	
	IFLY_ASSERT(metavalue);
	if (!string_is_all_right(metaname) || !string_is_all_right(metavalue) || !strlen(metavalue) || !strlen(metaname))
		return -1;
	Metadata* metadata = reinterpret_cast<Metadata*>(metahandle);
	metadata->setMeta(metaname, metavalue);
	return 0;
}

int cssp_metadata_count(const MetadataHandle metahandle){
	IFLY_ASSERT(metahandle);
	Metadata* metadata = reinterpret_cast<Metadata*>(metahandle);
	return metadata->countMeta();
}
