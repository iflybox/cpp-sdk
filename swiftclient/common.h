

#ifndef __CSSP__COMMON_H__
#define __CSSP__COMMON_H__
#include <assert.h>
#include <string>
#include <map>
#include <vector>
#include <cctype>
#include <algorithm>

#define		X_AUTH_USER					"x-auth-user"
#define		X_AUTH_KEY					"x-auth-key"
#define		X_AUTH_TOKEN				"x-auth-token"
#define		X_STORAGE_URL				"x-storage-url"
#define		X_ACCOUNT_META				"x-account-meta-"
#define		X_REMOVE_ACCOUNT_META		"x-remove-account-meta-"
#define		X_ACCOUNT_CONTAINER_COUNT	"x-account-container-count"
#define		X_ACCOUNT_OBJECT_COUNT		"x-account-object-count"
#define		X_ACCOUNT_BYTES_USED		"x-account-bytes-used"
#define		X_CONTAINER_META			"x-container-meta-"
#define		X_REMOVE_CONTAINER_META		"x-remove-container-meta-"
#define		X_CONTAINER_OBJECT_COUNT	"x-container-object-count"
#define		X_CONTAINER_BYTES_USED		"x-container-bytes-used"
#define		X_OBJECT_META				"x-object-meta-"
#define		X_REMOVE_OBJECT_META		"x-remove-object-meta-"
#define		X_OBJECT_ETAG				"etag"
#define		X_OBJECT_LAST_MODIFY		"last-modified"
#define		X_OBJECT_CONTENT_LENGTH		"content-length"
#define		X_OBJECT_CONTENT_TYPE		"content-type"
#define		X_COPY_FROM					"x-copy-from"
#define     X_DESTINATION               "destination"
#define		X_RANGE						"range"
#define		X_OBJECT_MANIFEST			"x-object-manifest"
#define		X_CONTAINER_READ			"x-container-read"
#define		X_REMOVE_CONTAINER_READ		"x-remove-container-read"

//int类型字符最大长度
#define		INT_STR_LEN_MAX				12
#define		ListFormatAndLimit			"?format=xml&limit=";
#define		HttpParamMarker					"&marker="
#define		HttpParamPrefix					"&prefix="
#define		HttpParamDelimiter				"&delimiter="

#define		NodeTypeSubdir				"subdir"
#define		NodeTypeObject				"object"

typedef		std::map<std::string, std::string>	HttpHeaderMap;
typedef		std::map<std::string, std::string>	MetadataMap;
typedef		std::map<std::string, std::string>	XmlNodeDataMap;
typedef		std::vector<XmlNodeDataMap>			XmlNodeVector;


//debug assert macro
#if	(DEBUG || _DEBUG)
	#define IFLY_ASSERT		assert
#else
	#define IFLY_ASSERT		 
#endif



#endif // __CSSP__COMMON_H__
