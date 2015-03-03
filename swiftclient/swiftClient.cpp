#include "common.h"
#include "swiftClient.h"
#include "exception.h"
#include "error.h"
#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"
#include "cssp.h"
#include "jsoncpp/json.h"
#include <ctime>
#include <math.h> 
#include <clocale>
#ifndef UINT64_MAX
#define UINT64_MAX (18446744073709551615ULL)
#endif
namespace cssp
{

extern uint64_t atou64(const char* astr);

const char* u64toa(uint64_t value, char* str)
{
#ifdef _WIN32
		sprintf_s(str, 30, "%I64u", value);
#elif _M_X64
	snprintf(str, 30, "%lu", value);
#elif _M_IX86
	snprintf(str, 30, "%llu", value);
#else
#error "Not found _WIN32 or _M_X64 or _M_IX86!"
#endif
	return str;
}

extern bool getline(std::string& stream, std::string& line);

void parseXml(const std::string& xml, XmlNodeVector& objectVt)
{
	TiXmlBase::SetCondenseWhiteSpace(false);
	//根据文件路径创建一个XML的文档对象。
	TiXmlDocument *myDocument = new TiXmlDocument(xml.c_str());
	//利用parse方法load解析内存数据
	myDocument->Parse(xml.c_str());
	//获取根元素即container
	TiXmlElement *RootElement = myDocument->RootElement();
	TiXmlElement *pElement = NULL;
	//遍历XML文件
	if(RootElement == NULL){
		return ;
	}
	for(pElement=RootElement->FirstChildElement(); pElement; pElement = pElement->NextSiblingElement())
	{
		//向map中加入<name></name>中的内容
		XmlNodeDataMap node_map;
		TiXmlElement *pNode = pElement->FirstChildElement();
		while(pNode){	
			std::string node_text = (pNode->GetText()) ? pNode->GetText() : "";
			std::string node_value = (pNode->Value()) ? pNode->Value() : "";
			if (!node_text.empty() || !node_value.empty())
				node_map.insert(XmlNodeDataMap::value_type(node_value,node_text));
			pNode = pNode->NextSiblingElement();
		}
		//此处插入一个区别类别的预定义Key
		node_map.insert(XmlNodeDataMap::value_type("NodeType", pElement->Value()));
		objectVt.push_back(node_map);


	}
}

char hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
// UNSAFE String
std::string csUnsafeString= "% ";
std::string decToHex(char,int);
// PURPOSE OF THIS FUNCTION IS TO CONVERT A GIVEN CHAR TO URL HEX FORM
std::string convert(char val) 
{
	std::string csRet;
	csRet += "%";
	csRet += decToHex(val, 16); 
	return  csRet;
}

// THIS IS A HELPER FUNCTION.
// PURPOSE OF THIS FUNCTION IS TO GENERATE A HEX REPRESENTATION OF GIVEN CHARACTER
std::string decToHex(char num, int radix)
{ 
	int temp=0; 
	std::string csTmp;
	int num_char;
	num_char = (int) num;

	// ISO-8859-1 
	// IF THE IF LOOP IS COMMENTED, THE CODE WILL FAIL TO GENERATE A 
	// PROPER URL ENCODE FOR THE CHARACTERS WHOSE RANGE IN 127-255(DECIMAL)
	if (num_char < 0)
		num_char = 256 + num_char;

	while (num_char >= radix)
	{
		temp = num_char % radix;
		num_char = (int)floor((double)(num_char / radix));
		csTmp = hexVals[temp];
	}

	csTmp += hexVals[num_char];

	if(csTmp.length() < 2)
	{
		csTmp += '0';
	}

	std::string strdecToHex = "";
	// Reverse the String
	for (int i = csTmp.length() - 1 ; i >= 0; --i)
	{
		strdecToHex += csTmp[i]; 
	}

	return strdecToHex;
}

// PURPOSE OF THIS FUNCTION IS TO CHECK TO SEE IF A CHAR IS URL UNSAFE.
// TRUE = UNSAFE, FALSE = SAFE
bool isUnsafe(char compareChar)
{
	bool bcharfound = false;
	char tmpsafeChar;
	int m_strLen = 0;

	m_strLen = csUnsafeString.length();
	for(int ichar_pos = 0; ichar_pos < m_strLen ;ichar_pos++)
	{
		tmpsafeChar = csUnsafeString.at(ichar_pos); 
		if(tmpsafeChar == compareChar)
		{ 
			bcharfound = true;
			break;
		} 
	}
	int char_ascii_value = 0;
	//char_ascii_value = __toascii(compareChar);
	char_ascii_value = (int) compareChar;

	if(bcharfound == false &&  char_ascii_value > 32 && char_ascii_value < 123)
	{
		return false;
	}
	// found no unsafe chars, return false  
	else
	{
		return true;
	}

	return true;
}
// PURPOSE OF THIS FUNCTION IS TO CONVERT A STRING 
// TO URL ENCODE FORM.
std::string URLEncode(const std::string& pcsEncode)
{ 
	int ichar_pos;
	std::string csEncode;
	std::string csEncoded; 
	int m_length;
	int ascii_value;

	csEncode = pcsEncode;
	m_length = csEncode.length();

	for(ichar_pos = 0; ichar_pos < m_length; ichar_pos++)
	{
		char ch = csEncode.at(ichar_pos);
		if (ch < ' ') 
		{
			ch = ch;
		}  
		if(!isUnsafe(ch))
		{
			// Safe Character    
			csEncoded += ch;
		}
		else
		{
			// get Hex Value of the Character
			csEncoded += convert(ch);
		}
	}


	return csEncoded;

}

SwiftClient::SwiftClient(const std::string& containerUrl, const std::string& accessKeyId, const std::string& accessKeySecret, int timeout)
	: containerUrl_(containerUrl), accessKeyId_(accessKeyId), accessKeySecret_(accessKeySecret), timeout_ms_(timeout)
{
	srand((unsigned)time(NULL));
	setlocale(LC_TIME, "en_US.UTF-8");
	std::setlocale(LC_TIME, "en_US.UTF-8");
	
}

void SwiftClient::setTimeout(int timeout){
	timeout_ms_ = timeout;
}

int SwiftClient::getTimeout(){
	return timeout_ms_;
}


CSSPResult SwiftClient::containerExists(){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	Sha1Request request(containerUrl_, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.headMethod(response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("headMethod exception:" + containerUrl_).c_str(), __FILE__, __LINE__);
	}
	return result;
}


CSSPResult SwiftClient::deleteContainerIfEmpty(){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	Sha1Request request(containerUrl_, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.deleteMethod(response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("deleteMethod exception:" + containerUrl_).c_str(), __FILE__, __LINE__);
	}
	return result;
}

CSSPResult SwiftClient::setContainerSysmeta(const std::string& sysKey, const std::string& sysValue){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	httpHeader.append(sysKey, sysValue);
	Sha1Request request(containerUrl_, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.postMethod("", response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("postMethod exception:" + containerUrl_).c_str(), __FILE__, __LINE__);
	}
	return result;
}

CSSPResult SwiftClient::setContainerMetadata(const ContainerMetadata& containerMetadata){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	for(MetadataMap::const_iterator iter = containerMetadata.container_meta_.begin(); iter != containerMetadata.container_meta_.end(); ++iter){
		httpHeader.append(ContainerMetadata::container_prefix_ + iter->first, iter->second);
	}
	Sha1Request request(containerUrl_, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.postMethod("", response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("postMethod exception:" + containerUrl_).c_str(), __FILE__, __LINE__);
	}
	return result;
}

CSSPResult SwiftClient::getContainerMetadata(ContainerMetadata& metadata){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	Sha1Request request(containerUrl_, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.headMethod(response);
	metadata.clearMeta();
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
		else{
			metadata.parse(response.getHeader().headers_);
		}
	}
	else{
		throw iflyCurlException(ccode, ("headMethod exception:" + containerUrl_).c_str(), __FILE__, __LINE__);
	}
	return result;
}


CSSPResult SwiftClient::removeContainerMetadata(const ContainerMetadata& removeMetadata){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	for(MetadataMap::const_iterator iter = removeMetadata.container_meta_.begin(); iter != removeMetadata.container_meta_.end(); ++iter){
		httpHeader.append(ContainerMetadata::container_remove_prefix_ + iter->first, iter->second);
	}
	Sha1Request request(containerUrl_, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.postMethod("", response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("postMethod exception:" + containerUrl_).c_str(), __FILE__, __LINE__);
	}
	return result;
}

CSSPResult SwiftClient::listObjects(int limit, const std::string& prefixtmp, const std::string& delimitertmp, const std::string& markertmp, std::vector<ObjectMetadata*>& metaVector){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string prefix= URLEncode(prefixtmp);
	std::string delimiter = URLEncode(delimitertmp);
	std::string marker = URLEncode(markertmp);

	std::string urlRequst = containerUrl_ + ListFormatAndLimit;
	char limit_str[INT_STR_LEN_MAX];
	itoa(limit, limit_str, 10);
	urlRequst += limit_str;
	if(marker.empty() == false){
		urlRequst += HttpParamMarker + marker;
	}
	if(prefix.empty() == false){
		urlRequst += HttpParamPrefix + prefix;
	}
	if(delimiter.empty() == false){
		urlRequst += HttpParamDelimiter + delimiter;
	}
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.getMethod(response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("getMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	XmlNodeVector nodeVt;
	parseXml(response.getContent(), nodeVt);
	for(XmlNodeVector::iterator vt_iter = nodeVt.begin(); vt_iter != nodeVt.end(); ++vt_iter){
		if((*vt_iter)["NodeType"] == NodeTypeSubdir){
			ObjectMetadata* objectMeta = new ObjectMetadata;
			if(objectMeta)
			{
				objectMeta->setOwnerName((*vt_iter)["name"]);
				metaVector.push_back(objectMeta);
			}
		}
		else if((*vt_iter)["NodeType"] == NodeTypeObject){
			ObjectMetadata* objectMeta = new ObjectMetadata;
			if(objectMeta)
			{
				objectMeta->setOwnerName((*vt_iter)["name"]);
				objectMeta->object_sysdef_[X_OBJECT_ETAG] = (*vt_iter)["hash"];
				objectMeta->object_sysdef_[X_OBJECT_LAST_MODIFY] = (*vt_iter)["last_modified"];
				objectMeta->object_sysdef_[X_OBJECT_CONTENT_LENGTH] = (*vt_iter)["bytes"];
				objectMeta->object_sysdef_[X_OBJECT_CONTENT_TYPE] = (*vt_iter)["content_type"];
			}
			metaVector.push_back(objectMeta);
		}
	}
	return result;
}




CSSPResult SwiftClient::objectExists(const std::string& object){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string urlRequst = containerUrl_ + "/" + URLEncode(object);
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.headMethod(response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("headMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	return result;
}



CSSPResult SwiftClient::putObject(const std::string& objectname, read_data_ptr putObjectCallback, void* inputstream, const char* md5, ObjectMetadata* outmetadata){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string urlRequst = containerUrl_ + '/' + URLEncode(objectname);
	if(md5){
		httpHeader.append(X_OBJECT_ETAG, md5);
	}
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.putMethod(inputstream, putObjectCallback, response);
	
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果在2XX状态码内，需要填充metadata信息
		if(is_ok(response.getStatus()))
		{
			//如果用户关注metadata信息则需要将metadata信息返回
			if(outmetadata){
				outmetadata->parse(response.getHeader().headers_);
			}
		}
		else{
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("putMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	return result;
}


 CSSPResult SwiftClient::getObject(const std::string& objectname, SwiftObject& object){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string urlRequst = containerUrl_ + "/" + URLEncode(objectname);
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.getMethod(response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		if(HTTP_OK == response.getStatus() ){
			//200 status code
			object.write(response.getContent().data(), response.getContent().size());
		}
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("getMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	return result;
}


CSSPResult SwiftClient::getObject(const std::string& objectname, write_data_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t& size, ObjectMetadata* outmetadata){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string urlRequst = URLEncode(containerUrl_ + "/" + objectname);
	char range_begin[30];
	char range_end[30];
	std::string range = "bytes=";
	range += u64toa(offset, range_begin);
	range += "-";
	if(UINT64_MAX - offset < size){
		throw iflyParamException(ERROR_OVERRANGE, "offset OR size Over Range!", __FILE__, __LINE__);
	}
	if(size > 0){
		range += u64toa(offset + size - 1, range_end);
	}
	httpHeader.append(X_RANGE, range);
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	libcurl_write_struct libcurl_stream;
	libcurl_stream.curl_handle = request.getCurl();
	libcurl_stream.getCallback = getObjectCallback;
	libcurl_stream.response_content = &response.content_;
	libcurl_stream.user_stream = outputstream;
	ccode = request.getMethod(&libcurl_stream, libcurl_writedata_function, response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果在2XX状态码内，需要填充content-length信息
		if(is_ok(response.getStatus())){
			std::string content_length;
			response.getHeader().get(X_OBJECT_CONTENT_LENGTH, content_length);
			size = atou64(content_length.c_str());
		}
		else{
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("getMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	return result;
}

CSSPResult SwiftClient::removeObject(const std::string& object){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string path = containerUrl_ + '/' + URLEncode(object);
	std::string urlRequst = path + "?multipart-manifest=delete";
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.deleteMethod(response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("deleteMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	return result;
}

CSSPResult SwiftClient::copyObject(const std::string& srcObject, const std::string& dstContainer, const std::string& dstObject){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string urlRequst = containerUrl_ + "/" + URLEncode(srcObject);
	httpHeader.append(X_DESTINATION, dstContainer + "/" + URLEncode(dstObject));
	httpHeader.append(X_OBJECT_CONTENT_LENGTH, "0");
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.customMethod("COPY", response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("customMethod COPY exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	return result;
}


CSSPResult SwiftClient::setObjectMetadata(const std::string& object, const ObjectMetadata& userMetadata){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string urlRequst = containerUrl_ + "/" + URLEncode(object);
	for(std::map<std::string, std::string>::const_iterator iter = userMetadata.object_meta_.begin(); iter != userMetadata.object_meta_.end(); ++iter){
		httpHeader.append(ObjectMetadata::object_prefix_ + iter->first, iter->second);
	}
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.postMethod("", response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("postMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	return result;
}


CSSPResult SwiftClient::getObjectMetadata(const std::string& object, ObjectMetadata& metadata){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string urlRequst = containerUrl_ + "/" + URLEncode(object);
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.headMethod(response);
	metadata.clearMeta();
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
		else{
			metadata.parse(response.getHeader().headers_);
			metadata.setOwnerName(object);
		}
	}
	else{
		throw iflyCurlException(ccode, ("headMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	return result;
}


std::string SwiftClient::multipartUploadInit(const std::string& object){
	std::string upload_id = generateUploadId(object);
	if(upload_id.empty() == false){
		multi_uploads_[upload_id] = object;
	}
	return upload_id;
}

CSSPResult SwiftClient::multipartUploadPart(const std::string& upid, int partnumber, read_data_ptr putObjectCallback, void* inputstream, const char* md5){
	CURLcode ccode = CURLE_OK;
	HttpHeader httpHeader;
	std::string part_object;
	if(multi_uploads_.count(upid)){
		part_object = multi_uploads_[upid] + "/" + upid + "/";
	}
	else{
		throw iflyException(ERROR_UPLOADID_NOTEXIST, "upload id not exist.", __FILE__, __LINE__);
	}
	char szPartNumber[30];
	itoa(partnumber, szPartNumber, 10);
	part_object += szPartNumber;
	return putObject(part_object, putObjectCallback, inputstream, md5, NULL);
}


CSSPResult SwiftClient::multipartUploadListParts(const std::string& upid, int limit, const std::string& marker, std::vector<ObjectMetadata*>& metaVector){
	std::string prefix;
	if(multi_uploads_.count(upid)){
		prefix = multi_uploads_[upid] + "/" + upid + "/";
	}
	else{
		throw iflyException(ERROR_UPLOADID_NOTEXIST, "upload id not exist.", __FILE__, __LINE__);
	}
	if(prefix.empty())
		throw iflyException(ERROR_PARAM_WRONG, "prefix empty Or container empty.", __FILE__, __LINE__);
	return listObjects(limit, prefix, "/", marker, metaVector);
}


CSSPResult SwiftClient::multipartUploadComplete(const std::string& upid){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string urlRequst;
	if(multi_uploads_.count(upid)){
		urlRequst = containerUrl_ + '/' + URLEncode(multi_uploads_[upid]);
	}
	else{
		throw iflyException(ERROR_UPLOADID_NOTEXIST, "upload id not exist.", __FILE__, __LINE__);
	}
	//写入X-Object-Manifest
	std::string container = containerUrl_.substr(containerUrl_.find_last_of('/') + 1);
	httpHeader.append(X_OBJECT_MANIFEST, container + '/' + URLEncode(multi_uploads_[upid])  + '/' + upid + '/');
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	//Manifest文件必须为0字节
	ccode = request.putMethod("", response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("putMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	return result;
}

CSSPResult SwiftClient::multipartUploadAbort(const std::string& upid){
	CURLcode ccode = CURLE_OK;
	CSSPResult result;
	HttpHeader httpHeader;
	std::string urlRequst;
	std::string fixpart, container, object;
	if(multi_uploads_.count(upid)){
		urlRequst = containerUrl_ + "?path=" + multi_uploads_[upid] + "/" + upid;
		urlRequst = URLEncode(urlRequst);
	}
	else{
		throw iflyException(ERROR_UPLOADID_NOTEXIST, "upload id not exist.", __FILE__, __LINE__);
	}
	Sha1Request request(urlRequst, timeout_ms_, accessKeyId_, accessKeySecret_);
	request.setheader(httpHeader);
	HttpResponse response;
	//Manifest文件必须为0字节
	ccode = request.getMethod(response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, ("putMethod exception:" + urlRequst).c_str(), __FILE__, __LINE__);
	}
	//删除multipart
	std::string line, delurl;
	std::string content = response.getContent();
	while(getline(content, line)){
		if(line.empty() == false)
			removeObject(line);
	}
	return result;
}

std::string SwiftClient::generateUploadId(const std::string& object){
	time_t timer;
	struct tm *tblock;
	timer = time(NULL);
	tblock = localtime(&timer);
	UploadId upid;
#ifdef _WIN32
	sprintf_s(upid, UPLOADID_LEN + 1, "%04d%02d%02d%02d%02d%02d%06d%06d%06d", tblock->tm_year, tblock->tm_mon, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec, rand()%1000000, rand()%1000000, rand()%1000000);
#else
	snprintf(upid, UPLOADID_LEN + 1, "%04d%02d%02d%02d%02d%02d%06d%06d%06d", tblock->tm_year, tblock->tm_mon, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec, rand()%1000000, rand()%1000000, rand()%1000000);
#endif
	return upid;
}



std::string SwiftClient::containerUrl(const std::string& container){
	return containerUrl_;
}

std::string SwiftClient::objectUrl(const std::string& container, const std::string& object){
	return containerUrl_ + '/' + object;
}

size_t SwiftClient::libcurl_writedata_function(void* buffer, size_t size, size_t nmemb, void* userp){
	libcurl_write_struct* libcurl_write_stup = (libcurl_write_struct*)userp;
	int http_status = 0;
	CURLcode ccode = curl_easy_getinfo(libcurl_write_stup->curl_handle, CURLINFO_RESPONSE_CODE, &http_status);
	if(is_ok(http_status)){
		int a = libcurl_write_stup->getCallback(buffer, size, nmemb, libcurl_write_stup->user_stream);
		return a;
	}else{
		libcurl_write_stup->response_content->append((const char*)buffer, size * nmemb);
		return size * nmemb;
	}
	return size * nmemb;
}

size_t SwiftClient::readMemoryStream(void* buffer, size_t size, size_t nmemb, void* userp){
	std::stringstream* data = (std::stringstream*)userp;
	size_t avail = data->tellp() - data->tellg();
	size_t read_bytes = avail > (size * nmemb) ? (size * nmemb) : avail;
	data->read((char*)buffer, read_bytes);
	return read_bytes;
}

size_t SwiftClient::writeMemoryStream(void* buffer, size_t size, size_t nmemb, void* userp){
	SwiftObject* object = (SwiftObject*)userp;
	object->write((const char*)buffer, size * nmemb);
	return size * nmemb;
}


}

