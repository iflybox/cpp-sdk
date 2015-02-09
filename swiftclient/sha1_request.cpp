
#include "sha1_request.h"
#include "openssl/hmac.h"
#include <time.h>


namespace cssp{

	std::string Sha1Request::hmac_sha1(const std::string& method, const std::string& accessKeyId, const std::string& accessKeySecret){

		std::string strToSign = this->string_to_sign(method);
		char md[EVP_MAX_MD_SIZE];
		const unsigned char* digest = HMAC(EVP_sha1(), accessKeySecret.c_str(), accessKeySecret.length(), (const unsigned char*)strToSign.data(), strToSign.length(), (unsigned char*)md, NULL);
		std::string authorization= "CSSP " + accessKeyId + ":";
		authorization += md;
		return authorization;
	}


	std::string Sha1Request::string_to_sign(const std::string& method){
		std::string str_to_sign = method + '\n';
		//1.Is there ETAG?
		std::string etag;
		if( this->header_.get("etag", etag) ){
			str_to_sign += etag + '\n';
		}
		
		//2.Is there Content-Type?
		std::string content_type;
		if( this->header_.get("content-type", content_type)){
			str_to_sign += content_type + '\n';
		}
		
		//3.Add Date
		time_t t= time( NULL );
		char szBuf[64]={0};
		strftime( szBuf ,63 ,"%A,%d-%b-%y %H:%M:%S\n" , gmtime(&t ) );
		str_to_sign += szBuf;

		//4.filter the header start with x-object-meta
		const char* META_PREFIX = "x-object-meta";
		std::string canonicalized_headers_str;
		//特别注意此处，是利用现版本STL的map实现是基于RBTree，故map的key本身就是字典序，以此特性完成header的排序
		for(HttpHeaderMap::const_iterator iter = this->header_.headers_.begin(); 
			iter != this->header_.headers_.end(); ++iter){
			if(iter->first.find(META_PREFIX, 0) ==0 ){
				canonicalized_headers_str += iter->first + ':' + iter->second + '\n';
			}
		}
		str_to_sign += canonicalized_headers_str;

		//5.add CanonicalizedResource;
		std::string::size_type pos = this->url_.find('/', strlen("http://"));
		if( pos != std::string::npos){
			str_to_sign += this->url_.substr(pos);
		}

		return str_to_sign;
	}

	CURLcode Sha1Request::getMethod(void* outputstream, write_data_ptr writeCallback, HttpResponse& resp){
		std::string authorization = hmac_sha1("GET", this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::getMethod(outputstream, writeCallback, resp);
	}

	CURLcode Sha1Request::getMethod(HttpResponse& resp){
		std::string authorization = hmac_sha1("GET", this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::getMethod(resp);
	}

	CURLcode Sha1Request::putMethod(void* inputstream, read_data_ptr readCallback, HttpResponse& resp){
		std::string authorization = hmac_sha1("PUT", this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::putMethod(inputstream, readCallback, resp);
	}

	CURLcode Sha1Request::putMethod(const std::string& data, HttpResponse& resp){
		std::string authorization = hmac_sha1("PUT", this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::putMethod(data, resp);
	}

	CURLcode Sha1Request::postMethod(const std::string& data, HttpResponse& resp){
		std::string authorization = hmac_sha1("POST", this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::postMethod(data, resp);
	}

	CURLcode Sha1Request::headMethod(HttpResponse& resp){
		std::string authorization = hmac_sha1("HEAD", this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::headMethod(resp);
	}

	CURLcode Sha1Request::deleteMethod(HttpResponse& resp){
		std::string authorization = hmac_sha1("DELETE", this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::deleteMethod(resp);
	}
}