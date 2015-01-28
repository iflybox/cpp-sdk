#include "common.h"
#include "swiftClient.h"
#include "exception.h"
#include "error.h"
#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"
#include "iflybox.h"
#include "jsoncpp/json.h"
#include <ctime>
#include <math.h> 
#ifndef UINT64_MAX
#define UINT64_MAX (18446744073709551615ULL)
#endif
namespace iflybox
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
std::string URLEncode(std::string pcsEncode)
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

SwiftClient::SwiftClient(const std::string& endpoint, const std::string& accessKeyId, const std::string& accessKeySecret, int endpoint_type, int timeout)
	: auth_endpoint_(endpoint), accessKeyId_(accessKeyId), accessKeySecret_(accessKeySecret), endpoint_type_(endpoint_type), timeout_ms_(timeout)
{
	srand((unsigned)time(NULL));
}

void SwiftClient::setTimeout(int timeout){
	timeout_ms_ = timeout;
}

int SwiftClient::getTimeout(){
	return timeout_ms_;
}

iflyboxResult SwiftClient::auth(){
	iflyboxResult result;
	//此处应该先进行keystone验证
	try{
		result = keystone(auth_endpoint_, accessKeyId_, accessKeySecret_, endpoint_type_);
	}catch(const iflyException& e){
		result.setStatus(e.error());
	}
	if(accessToken_.empty() || storageUrl_.empty()){
		result = tempauth(auth_endpoint_, accessKeyId_, accessKeySecret_);
	}
	return result;
}

iflyboxResult SwiftClient::tempauth(const std::string& endpoint, const std::string& accessKeyId, const std::string& accessKeySecret){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string urlRequst = endpoint;
	httpHeader.append(X_AUTH_USER, accessKeyId);
	httpHeader.append(X_AUTH_KEY, accessKeySecret);
	HttpRequest request(urlRequst, timeout_ms_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.getMethod(response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
		else{
			 response.getHeader().get(X_AUTH_TOKEN, accessToken_);
			 response.getHeader().get(X_STORAGE_URL, storageUrl_);
		}
	}
	else{
		throw iflyCurlException(ccode, "getMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::keystone(const std::string& endpoint, const std::string& accessKeyId, const std::string& accessKeySecret, int endpoint_type){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	Json::Value root_node, auth_node, passwordCredentials_node;
	std::string tenantName, username;
	//tenantName = accessKeyId.find(":", 0)
	std::string::size_type pos = accessKeyId.find(":", 0);
	if(pos == std::string::npos){
		throw iflyException(ERROR_ACCESSKEYID_FORMATERR, "format error", __FILE__, __LINE__);
	}
	tenantName = accessKeyId.substr(0, pos);
	username = accessKeyId.substr(pos + 1, accessKeyId.length());
	auth_node["tenantName"] = Json::Value(tenantName);
	passwordCredentials_node["username"] = Json::Value(username);
	passwordCredentials_node["password"] = Json::Value(accessKeySecret);
	auth_node["passwordCredentials"] = passwordCredentials_node;
	root_node["auth"] = auth_node;
	Json::FastWriter fast_writer;
	std::string json_data = fast_writer.write(root_node);
	HttpHeader httpHeader;
	std::string urlRequst = endpoint;
	httpHeader.append(X_OBJECT_CONTENT_TYPE, "application/json");
	HttpRequest request(urlRequst, timeout_ms_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.postMethod(json_data, response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
		else{
			Json::Reader reader;
			Json::Value json_object;
			if( reader.parse(response.content_, json_object) == false){
				throw iflyException(ERROR_JSON_PARSEFAILED, "json parse failed", __FILE__, __LINE__);
			}
			Json::Value access_node = json_object["access"];
			if(access_node != Json::Value::null){
				Json::Value token_node = access_node["token"];
				Json::Value serviceCatalog_node = access_node["serviceCatalog"];
				if(token_node != Json::Value::null){
					Json::Value id_node = token_node["id"];
					if(id_node != Json::Value::null){
						accessToken_ = id_node.asString();
					}
				}
				if(serviceCatalog_node != Json::Value::null){
					Json::Value endpoints_node = serviceCatalog_node[Json::Value::UInt(0)]["endpoints"];
					if(endpoints_node != Json::Value::null){
						Json::Value publicURL_node = (!endpoint_type) ? endpoints_node[Json::Value::UInt(0)]["publicURL"]:endpoints_node[Json::Value::UInt(0)]["internalURL"];
						if(publicURL_node != Json::Value::null){
							storageUrl_ = publicURL_node.asString();
						}
					}
				}
			}
		}
	}
	else{
		throw iflyCurlException(ccode, "getMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::setAccountMetadata(const AccountMetadata& accountMetadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string urlRequst = accountUrl();
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	for(std::map<std::string, std::string>::const_iterator iter = accountMetadata.account_meta_.begin(); iter != accountMetadata.account_meta_.end(); ++iter){
		httpHeader.append(AccountMetadata::account_prefix_ + iter->first, iter->second);
	}
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "postMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


 iflyboxResult SwiftClient::getAccountMetadata(AccountMetadata& metadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(accountUrl(), timeout_ms_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.headMethod(response);
	metadata.clearMeta();
	if(ccode == CURLE_OK){
		metadata.parse(response.getHeader().headers_);
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, "headMethod exception!", __FILE__, __LINE__);
	}
	return result;
}

iflyboxResult SwiftClient::removeAccountMetadata(const AccountMetadata& removeMetadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string urlRequst = accountUrl();
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	for(std::map<std::string, std::string>::const_iterator iter = removeMetadata.account_meta_.begin(); iter != removeMetadata.account_meta_.end(); ++iter){
		httpHeader.append(AccountMetadata::account_remove_prefix_ + iter->first, iter->second);
	}
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "postMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::listContainers(int limit, const std::string& marker, std::vector<ContainerMetadata*>& metaVector){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	
	std::string urlRequst = accountUrl() + ListFormatAndLimit;

	char limit_str[INT_STR_LEN_MAX];
	itoa(limit, limit_str, 10);
	urlRequst += limit_str;
	if(marker.empty() == false)
	{
		std::string markertmp;
		markertmp = URLEncode(marker);
		urlRequst += HttpParamMarker + markertmp;
	}
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "getMethod exception!", __FILE__, __LINE__);
	}
	XmlNodeVector nodeVt;
	parseXml(response.getContent(), nodeVt);
	for(XmlNodeVector::iterator vt_iter = nodeVt.begin(); vt_iter != nodeVt.end(); ++vt_iter){
		if((*vt_iter)["NodeType"] == "container"){
			ContainerMetadata* containerMeta = new ContainerMetadata;
			if(containerMeta)
			{
				containerMeta->setOwnerName((*vt_iter)["name"]);
				containerMeta->container_sysdef_[X_CONTAINER_OBJECT_COUNT] = (*vt_iter)["count"];
				containerMeta->container_sysdef_[X_CONTAINER_BYTES_USED] = (*vt_iter)["bytes"];
				metaVector.push_back(containerMeta);
			}
		}
	}
	return result;
}


iflyboxResult SwiftClient::containerExists(const std::string& containertmp){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container = URLEncode(containertmp);
	std::string urlRequst = containerUrl(container);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "headMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::createContainer(const std::string& containertmp){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container = URLEncode(containertmp);
	std::string urlRequst = containerUrl(container);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	//httpHeader.append(X_OBJECT_CONTENT_LENGTH, 0);
	HttpRequest request(urlRequst, timeout_ms_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.putMethod("", response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, "putMethod exception!", __FILE__, __LINE__);
	}
	return result;
}

iflyboxResult SwiftClient::createContainer(const std::string& containertmp, const ContainerMetadata& metadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container = URLEncode(containertmp);
	std::string urlRequst = containerUrl(container);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	//httpHeader.append(X_OBJECT_CONTENT_LENGTH, "0");
	for(MetadataMap::const_iterator iter = metadata.container_meta_.begin(); iter != metadata.container_meta_.end(); ++iter){
		httpHeader.append(ContainerMetadata::container_prefix_ + iter->first, iter->second);
	}
	HttpRequest request(urlRequst, timeout_ms_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.putMethod("", response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, "putMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::deleteContainerIfEmpty(const std::string& containertmp){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container = URLEncode(containertmp);
	std::string urlRequst = containerUrl(container);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "deleteMethod exception!", __FILE__, __LINE__);
	}
	return result;
}

iflyboxResult SwiftClient::setContainerMetadata(const std::string& containertmp, const ContainerMetadata& containerMetadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container = URLEncode(containertmp);
	std::string urlRequst = containerUrl(container);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	for(MetadataMap::const_iterator iter = containerMetadata.container_meta_.begin(); iter != containerMetadata.container_meta_.end(); ++iter){
		httpHeader.append(ContainerMetadata::container_prefix_ + iter->first, iter->second);
	}
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "postMethod exception!", __FILE__, __LINE__);
	}
	return result;
}

iflyboxResult SwiftClient::getContainerMetadata(const std::string& containertmp, ContainerMetadata& metadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container = URLEncode(containertmp);
	std::string urlRequst = containerUrl(container);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
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
			metadata.setOwnerName(container);
		}
	}
	else{
		throw iflyCurlException(ccode, "headMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::removeContainerMetadata(const std::string& containertmp, const ContainerMetadata& removeMetadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container = URLEncode(containertmp);
	std::string urlRequst = containerUrl(container);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	for(MetadataMap::const_iterator iter = removeMetadata.container_meta_.begin(); iter != removeMetadata.container_meta_.end(); ++iter){
		httpHeader.append(ContainerMetadata::container_remove_prefix_ + iter->first, iter->second);
	}
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "postMethod exception!", __FILE__, __LINE__);
	}
	return result;
}

iflyboxResult SwiftClient::listObjects(const std::string& containertmp, int limit, const std::string& prefixtmp, const std::string& delimitertmp, const std::string& markertmp, std::vector<ObjectMetadata*>& metaVector){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container = URLEncode(containertmp);
	std::string prefix= URLEncode(prefixtmp);
	std::string delimiter = URLEncode(delimitertmp);
	std::string marker = URLEncode(markertmp);

	std::string urlRequst = containerUrl(container) + ListFormatAndLimit;
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
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "getMethod exception!", __FILE__, __LINE__);
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




iflyboxResult SwiftClient::objectExists(const std::string& containertmp, const std::string& objecttmp){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container = URLEncode(containertmp);
	std::string object = URLEncode(objecttmp);
	std::string urlRequst = storageUrl_ + "/" + container + "/" + object;
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "headMethod exception!", __FILE__, __LINE__);
	}
	return result;
}

iflyboxResult SwiftClient::putObject(const std::string& container, const std::string& objectname, read_data_ptr putObjectCallback, void* inputstream, const char* md5, ObjectMetadata* outmetadata){
	return putObject(storageUrl_ + "/" + container + "/" + objectname, putObjectCallback, inputstream, md5, outmetadata);
}

iflyboxResult SwiftClient::putObject(const std::string& pathtmp, read_data_ptr putObjectCallback, void* inputstream, const char* md5, ObjectMetadata* outmetadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string path = URLEncode(pathtmp);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	if(md5){
		httpHeader.append(X_OBJECT_ETAG, md5);
	}
	HttpRequest request(path, timeout_ms_);
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
		throw iflyCurlException(ccode, "putMethod exception!", __FILE__, __LINE__);
	}
	return result;
}

// iflyboxResult SwiftClient::putObject(const std::string& path, const SwiftObject& objectdata){
// 	CURLcode ccode = CURLE_OK;
// 	iflyboxResult result;
// 	HttpHeader httpHeader;
// 	httpHeader.append(X_AUTH_TOKEN, accessToken_);
// 	HttpRequest request(path);
// 	request.setheader(httpHeader);
// 	HttpResponse response;
// 	ccode = request.putMethod(objectdata, response);
// 	if(ccode == CURLE_OK){
// 		result.setStatus(response.getStatus());
// 		//如果不在2XX状态码内，需要填充content信息
// 		if(response.getStatus() < 200 || response.getStatus() >= 300){
// 			result.setDetail(response.getContent());
// 		}
// 	}
// 	else{
// 		throw iflyCurlException(ccode, "putMethod exception!", __FILE__, __LINE__);
// 	}
// 	return result;
// }

 iflyboxResult SwiftClient::getObject(const std::string& containertmp, const std::string& objectnametmp, SwiftObject& object){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string container= URLEncode(containertmp);
	std::string objectname= URLEncode(objectnametmp);

	std::string urlRequst = storageUrl_ + "/" + container + "/" + objectname;
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "getMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::getObject(const std::string& container, const std::string& objectname, write_data_ptr getObjectCallback, void* outputstream, uint64_t offset, int64_t& size, ObjectMetadata* outmetadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string urlRequst = URLEncode(storageUrl_ + "/" + container + "/" + objectname);
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
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
	request.setheader(httpHeader);
	HttpResponse response;
	libcurl_write_struct libcurl_stream;
	libcurl_stream.curl_handle = request.getCurl();
	libcurl_stream.getCallback = getObjectCallback;
	libcurl_stream.response_content = &response.content_;
	libcurl_stream.user_stream = outputstream;
	ccode = request.getMethod(&libcurl_stream, libcurl_writedata_function, response);
//	ccode = request.getMethod(response);
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
		throw iflyCurlException(ccode, "getMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::removeObject(const std::string& container, const std::string& object){
	return removeObject(storageUrl_ + "/" + container + "/" + object);
}

iflyboxResult SwiftClient::removeObject(const std::string& pathtmp){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string path = URLEncode(pathtmp);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	std::string request_str = path + "?multipart-manifest=delete";
	HttpRequest request(request_str, timeout_ms_);
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
		throw iflyCurlException(ccode, "deleteMethod exception!", __FILE__, __LINE__);
	}
	return result;
}

iflyboxResult SwiftClient::copyObject(const std::string& sourceContainertmp, const std::string& sourceObjecttmp, const std::string& destinationContainertmp, const std::string& destinationObjecttmp){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string sourceContainer= URLEncode(sourceContainertmp);
	std::string sourceObject= URLEncode(sourceObjecttmp);
	std::string destinationContainer= URLEncode(destinationContainertmp);
	std::string destinationObject= URLEncode(destinationObjecttmp);

	std::string urlRequst = storageUrl_ + "/" + destinationContainer + "/" + destinationObject;
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	httpHeader.append(X_COPY_FROM, "/" + sourceContainer + "/" + sourceObject);
	httpHeader.append(X_OBJECT_CONTENT_LENGTH, "0");
	HttpRequest request(urlRequst, timeout_ms_);
	request.setheader(httpHeader);
	HttpResponse response;
	ccode = request.putMethod("", response);
	if(ccode == CURLE_OK){
		result.setStatus(response.getStatus());
		//如果不在2XX状态码内，需要填充content信息
		if(!is_ok(response.getStatus())){
			result.setDetail(response.getContent());
		}
	}
	else{
		throw iflyCurlException(ccode, "putMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::setObjectMetadata(const std::string& container, const std::string& object, const ObjectMetadata& userMetadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string urlRequst = URLEncode(storageUrl_ + "/" + container + "/" + object);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	for(std::map<std::string, std::string>::const_iterator iter = userMetadata.object_meta_.begin(); iter != userMetadata.object_meta_.end(); ++iter){
		httpHeader.append(ObjectMetadata::object_prefix_ + iter->first, iter->second);
	}
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "postMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::getObjectMetadata(const std::string& container, const std::string& object, ObjectMetadata& metadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string urlRequst = URLEncode(storageUrl_ + "/" + container + "/" + object);
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
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
			metadata.setOwnerName(container + "/" + object);
		}
	}
	else{
		throw iflyCurlException(ccode, "headMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


iflyboxResult SwiftClient::removeObjectMetadata(const std::string& container, const std::string& object, const ObjectMetadata& removeMetadata){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string urlRequst = URLEncode(objectUrl(container, object));
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	for(std::map<std::string, std::string>::const_iterator iter = removeMetadata.object_meta_.begin(); iter != removeMetadata.object_meta_.end(); ++iter){
		httpHeader.append(ObjectMetadata::object_remove_prefix_ + iter->first, iter->second);
	}
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "postMethod exception!", __FILE__, __LINE__);
	}
	return result;
}


std::string SwiftClient::multipartUploadInit(const std::string& container, const std::string& object){
	std::string upload_id = generateUploadId(container, object);
	if(upload_id.empty() == false){
		multi_uploads_[upload_id] = container + "/" + object;
	}
	return upload_id;
}

iflyboxResult SwiftClient::multipartUploadPart(const std::string& upid, int partnumber, read_data_ptr putObjectCallback, void* inputstream, const char* md5){
	CURLcode ccode = CURLE_OK;
	HttpHeader httpHeader;
	std::string urlRequst = storageUrl_;
	if(multi_uploads_.count(upid)){
		urlRequst += "/" + multi_uploads_[upid] + "/" + upid + "/";
	}
	else{
		throw iflyException(ERROR_UPLOADID_NOTEXIST, "upload id not exist.", __FILE__, __LINE__);
	}
	char szPartNumber[30];
	itoa(partnumber, szPartNumber, 10);
	urlRequst += szPartNumber;
	return putObject(urlRequst, putObjectCallback, inputstream, md5, NULL);
}


iflyboxResult SwiftClient::multipartUploadListParts(const std::string& upid, int limit, const std::string& marker, std::vector<ObjectMetadata*>& metaVector){
	CURLcode ccode = CURLE_OK;
	HttpHeader httpHeader;
	std::string fixpart, container, prefix;
	if(multi_uploads_.count(upid)){
		fixpart = multi_uploads_[upid];
		container = fixpart.substr(0, fixpart.find('/', 0));
		prefix = fixpart.substr(fixpart.find('/', 0) + 1, fixpart.length()) + "/" + upid + "/";
	}
	else{
		throw iflyException(ERROR_UPLOADID_NOTEXIST, "upload id not exist.", __FILE__, __LINE__);
	}
	if(prefix.empty() || container.empty())
		throw iflyException(ERROR_PARAM_WRONG, "prefix empty Or container empty.", __FILE__, __LINE__);
	return listObjects(container, limit, prefix, "/", marker, metaVector);
}


iflyboxResult SwiftClient::multipartUploadComplete(const std::string& upid){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string urlRequst = storageUrl_;
	if(multi_uploads_.count(upid)){
		urlRequst += "/" + multi_uploads_[upid];
		urlRequst = URLEncode(urlRequst);
	}
	else{
		throw iflyException(ERROR_UPLOADID_NOTEXIST, "upload id not exist.", __FILE__, __LINE__);
	}
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	//写入X-Object-Manifest
	httpHeader.append(X_OBJECT_MANIFEST, URLEncode(multi_uploads_[upid] + "/" + upid + "/"));
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "putMethod exception!", __FILE__, __LINE__);
	}
	return result;
}

iflyboxResult SwiftClient::multipartUploadAbort(const std::string& upid){
	CURLcode ccode = CURLE_OK;
	iflyboxResult result;
	HttpHeader httpHeader;
	std::string urlRequst = storageUrl_;
	std::string fixpart, container, object;
	if(multi_uploads_.count(upid)){
		fixpart = multi_uploads_[upid];
		container = fixpart.substr(0, fixpart.find('/', 0));
		object = fixpart.substr(fixpart.find('/', 0) + 1, fixpart.length());
		urlRequst += "/" + container + "?path=" + object + "/" + upid;
		urlRequst = URLEncode(urlRequst);
	}
	else{
		throw iflyException(ERROR_UPLOADID_NOTEXIST, "upload id not exist.", __FILE__, __LINE__);
	}
	httpHeader.append(X_AUTH_TOKEN, accessToken_);
	HttpRequest request(urlRequst, timeout_ms_);
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
		throw iflyCurlException(ccode, "putMethod exception!", __FILE__, __LINE__);
	}
	//删除multipart
	std::string line, delurl;
	std::string content = response.getContent();
	while(getline(content, line)){
		if(line.empty() == false)
			removeObject(storageUrl_ + "/" + container + "/" + line);
	}
	return result;
}

std::string SwiftClient::generateUploadId(const std::string& container, const std::string& object){
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

std::string SwiftClient::accountUrl(){
	return storageUrl_;
}

std::string SwiftClient::containerUrl(const std::string& container){
	return storageUrl_ + "/" + container;
}

std::string SwiftClient::objectUrl(const std::string& container, const std::string& object){
	return storageUrl_ + "/" + container + "/" + object;
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

