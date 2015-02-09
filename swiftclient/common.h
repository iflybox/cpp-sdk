

#ifndef __CSSP__COMMON_H__
#define __CSSP__COMMON_H__
#include <assert.h>
#include <string>
#include <map>
#include <vector>
#include <cctype>
#include <algorithm>

#define		X_AUTH_USER					"X-Auth-User"
#define		X_AUTH_KEY					"X-Auth-Key"
#define		X_AUTH_TOKEN				"X-Auth-Token"
#define		X_STORAGE_URL				"X-Storage-Url"
#define		X_ACCOUNT_META				"X-Account-Meta-"
#define		X_REMOVE_ACCOUNT_META		"X-Remove-Account-Meta-"
#define		X_ACCOUNT_CONTAINER_COUNT	"X-Account-Container-Count"
#define		X_ACCOUNT_OBJECT_COUNT		"X-Account-Object-Count"
#define		X_ACCOUNT_BYTES_USED		"X-Account-Bytes-Used"
#define		X_CONTAINER_META			"X-Container-Meta-"
#define		X_REMOVE_CONTAINER_META		"X-Remove-Container-Meta-"
#define		X_CONTAINER_OBJECT_COUNT	"X-Container-Object-Count"
#define		X_CONTAINER_BYTES_USED		"X-Container-Bytes-Used"
#define		X_OBJECT_META				"X-Object-Meta-"
#define		X_REMOVE_OBJECT_META		"X-Remove-Object-Meta-"
#define		X_OBJECT_ETAG				"Etag"
#define		X_OBJECT_LAST_MODIFY		"Last-Modified"
#define		X_OBJECT_CONTENT_LENGTH		"Content-Length"
#define		X_OBJECT_CONTENT_TYPE		"Content-Type"
#define		X_COPY_FROM					"X-Copy-From"
#define		X_RANGE						"Range"
#define		X_OBJECT_MANIFEST			"X-Object-Manifest"

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
