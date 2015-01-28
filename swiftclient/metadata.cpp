#include "metadata.h"
#include "common.h"


namespace iflybox
{
uint64_t atou64(const char* astr)
{
	uint64_t value = 0;
#ifdef _WIN32
	sscanf(astr, "%I64u", &value);
#elif _M_X64
	sscanf(astr, "%lu", &value);
#elif _M_IX86
	sscanf(astr, "%llu", &value);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	return value;
}

std::string		AccountMetadata::account_prefix_			= X_ACCOUNT_META;
std::string		AccountMetadata::account_remove_prefix_		= X_REMOVE_ACCOUNT_META;

std::string		ContainerMetadata::container_prefix_		= X_CONTAINER_META;
std::string		ContainerMetadata::container_remove_prefix_ = X_REMOVE_CONTAINER_META;

std::string		ObjectMetadata::object_prefix_				= X_OBJECT_META;
std::string		ObjectMetadata::object_remove_prefix_		= X_REMOVE_OBJECT_META;

const char* AccountMetadata::getMeta(const std::string& metaname){
	if(account_meta_.count(metaname)){
		return account_meta_[metaname].c_str();
	}
	return NULL;
}

void AccountMetadata::setMeta(const std::string& metaname, const std::string& metavalue){
	account_meta_[metaname] = metavalue;
}

int AccountMetadata::countMeta(){
	return account_meta_.size();
}

void AccountMetadata::clearMeta(){
	account_meta_.clear();
	account_sysdef_.clear();
}

void AccountMetadata::parse(const HttpHeaderMap& header){
	for(HttpHeaderMap::const_iterator iter = header.begin(); iter != header.end(); ++iter){
		if(iter->first.find(X_ACCOUNT_META) == 0){
			std::string metaname = iter->first.substr(strlen(X_ACCOUNT_META), iter->first.length());
			transform(metaname.begin(),metaname.end(),metaname.begin(),tolower);
			account_meta_[metaname] = iter->second;
		}
		else{
			account_sysdef_[iter->first] = iter->second;
		}
	}
}


uint64_t AccountMetadata::countContainer(){
	if(account_sysdef_.count(X_ACCOUNT_CONTAINER_COUNT)){
		std::string conCnt = account_sysdef_[X_ACCOUNT_CONTAINER_COUNT];
		return atou64(conCnt.c_str());
	}
	return 0;
}

uint64_t AccountMetadata::countObject(){
	if(account_sysdef_.count(X_ACCOUNT_OBJECT_COUNT)){
		std::string objCnt = account_sysdef_[X_ACCOUNT_OBJECT_COUNT];
		return atou64(objCnt.c_str());
	}
	return 0;
}

uint64_t AccountMetadata::bytesUsed(){
	if(account_sysdef_.count(X_ACCOUNT_BYTES_USED)){
		std::string bytesUsed = account_sysdef_[X_ACCOUNT_BYTES_USED];
		return atou64(bytesUsed.c_str());
	}
	return 0;
}



const char* ContainerMetadata::getMeta(const std::string& metaname){
	if(container_meta_.count(metaname)){
		return container_meta_[metaname].c_str();
	}
	return NULL;
}

void ContainerMetadata::setMeta(const std::string& metaname, const std::string& metavalue){
	container_meta_[metaname] = metavalue;
}


int ContainerMetadata::countMeta(){
	return container_meta_.size();
}

void ContainerMetadata::clearMeta(){
	container_meta_.clear();
	container_sysdef_.clear();
}

void ContainerMetadata::parse(const HttpHeaderMap& header){
	for(HttpHeaderMap::const_iterator iter = header.begin(); iter != header.end(); ++iter){
		if(iter->first.find(X_CONTAINER_META) == 0){
			std::string metaname = iter->first.substr(strlen(X_CONTAINER_META), iter->first.length());
			transform(metaname.begin(),metaname.end(),metaname.begin(),tolower);
			container_meta_[metaname] = iter->second;
		}
		else{
			container_sysdef_[iter->first] = iter->second;
		}
	}
}

uint64_t ContainerMetadata::countObject(){
	if(container_sysdef_.count(X_CONTAINER_OBJECT_COUNT)){
		std::string objCnt = container_sysdef_[X_CONTAINER_OBJECT_COUNT];
		return atou64(objCnt.c_str());
	}
	return 0;
}

uint64_t ContainerMetadata::bytesUsed(){
	if(container_sysdef_.count(X_CONTAINER_BYTES_USED)){
		std::string bytesUsed = container_sysdef_[X_CONTAINER_BYTES_USED];
		return atou64(bytesUsed.c_str());
	}
	return 0;
}


const char* ObjectMetadata::getMeta(const std::string& metaname){
	if(object_meta_.count(metaname)){
		return object_meta_[metaname].c_str();
	}
	return NULL;
}

void ObjectMetadata::setMeta(const std::string& metaname, const std::string& metavalue){
	object_meta_[metaname] = metavalue;
}

int ObjectMetadata::countMeta(){
	return object_meta_.size();
}

void ObjectMetadata::clearMeta(){
	object_meta_.clear();
	object_sysdef_.clear();
}

void ObjectMetadata::parse(const HttpHeaderMap& header){
	for(HttpHeaderMap::const_iterator iter = header.begin(); iter != header.end(); ++iter){
		if(iter->first.find(X_OBJECT_META) == 0){
			std::string metaname = iter->first.substr(strlen(X_OBJECT_META), iter->first.length());
			transform(metaname.begin(),metaname.end(),metaname.begin(),tolower);
			object_meta_[metaname] = iter->second;
		}
		else{
			object_sysdef_[iter->first] = iter->second;
		}
	}
}

uint64_t ObjectMetadata::contentLength(){
	if(object_sysdef_.count(X_OBJECT_CONTENT_LENGTH)){
		std::string bytesUsed = object_sysdef_[X_OBJECT_CONTENT_LENGTH];
		return atou64(bytesUsed.c_str());
	}
	return 0;
}

bool ObjectMetadata::isPseudoDirectory(){
	return pseudo_dir_;
}

const char*	ObjectMetadata::lastModified(){
	if(object_sysdef_.count(X_OBJECT_LAST_MODIFY)){
		return object_sysdef_[X_OBJECT_LAST_MODIFY].c_str();
	}
	return NULL;
}


const char*	ObjectMetadata::Etag(){
	if(object_sysdef_.count(X_OBJECT_ETAG)){
		return object_sysdef_[X_OBJECT_ETAG].c_str();
	}
	return NULL;
}

}

