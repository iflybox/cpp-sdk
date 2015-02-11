
#include "sha1_request.h"
#include "openssl/hmac.h"
#include "openssl/pem.h"
#include <time.h>




namespace cssp{

	std::string Sha1Request::dataGMT(){
		static const char* weekday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri" ,"Sat"};
		static const char* month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
									  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		time_t t= time( NULL );
		static const int DATE_LENGTH = 64;
		char date[DATE_LENGTH]={0};
		tm* tm = gmtime(&t);
		sprintf_s(date, DATE_LENGTH - 1, "%s, %02d %s %d %02d:%02d:%02d GMT", 
			      weekday[tm->tm_wday], tm->tm_mday, month[tm->tm_mon], 
				  tm->tm_year + 1900, tm->tm_hour, tm->tm_min, tm->tm_sec);
		return date;
	}


	int Sha1Request::base64_encode(const char *src,int src_len,char *encode,int encode_len){
		BIO *b64 = BIO_new(BIO_f_base64());
		BIO *bio = BIO_new(BIO_s_mem());

		bio = BIO_push(b64, bio);
		BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
		BIO_write(bio, src, src_len);
		BIO_ctrl(bio, BIO_CTRL_FLUSH, 0, NULL);

		BUF_MEM *bptr = NULL;
		BIO_get_mem_ptr(bio, &bptr);

		size_t slen = bptr->length;
		if(encode_len < slen)
			return -1;
		memcpy(encode, bptr->data, slen);
		encode[slen] = '\0';

		BIO_free_all(bio);
		return slen;
	}


	std::string Sha1Request::hmac_sha1(const std::string& method, const std::string& date, const std::string& accessKeyId, const std::string& accessKeySecret){

		std::string strToSign = this->string_to_sign(method, date);
		unsigned char md[EVP_MAX_MD_SIZE];
		char base64[EVP_MAX_MD_SIZE];
		unsigned int length = EVP_MAX_MD_SIZE;
		HMAC(EVP_sha1(), accessKeySecret.c_str(), accessKeySecret.length(), (const unsigned char*)strToSign.data(), strToSign.length(), (unsigned char*)md, &length);
		std::string authorization= "CSSP " + accessKeyId + ":";
		int encode_len = base64_encode((const char*)md, length, base64, EVP_MAX_MD_SIZE);
		if(encode_len < 0)
			return "";
/*		char hex_str[EVP_MAX_MD_SIZE * 2] = {0};
		for(int i = 0; i < length; ++i){
#ifdef _WIN32
			printf("%02X", md[i]);
			sprintf_s(hex_str, EVP_MAX_MD_SIZE * 2 - 1, "%s%02X", hex_str, md[i]);
#else
			snprintf(hex_str, EVP_MAX_MD_SIZE * 2 - 1, "%s%02X", hex_str, md[i]);
#endif	
		}
		*/
		authorization += base64;
		return authorization;
	}




	std::string Sha1Request::string_to_sign(const std::string& method, const std::string& date){
		std::string str_to_sign = method + '\n';
		//1.Is there ETAG?
		std::string etag;
		if( this->header_.get("etag", etag) ){
			str_to_sign += etag;
		}
		str_to_sign += '\n';
		
		//2.Is there Content-Type?
		std::string content_type;
		if( this->header_.get("content-type", content_type)){
			str_to_sign += content_type;
		}
		str_to_sign += '\n';
		
		//3.Add Date
		str_to_sign += date + '\n';

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
			std::string::size_type param_pos = this->url_.find('?', pos);
			if( param_pos != std::string::npos)
				str_to_sign += this->url_.substr(pos, param_pos - pos);
			else
				str_to_sign += this->url_.substr(pos);
		}

		return str_to_sign;
	}

	CURLcode Sha1Request::getMethod(void* outputstream, write_data_ptr writeCallback, HttpResponse& resp){
		const std::string date = dataGMT();
		this->header_.append("date", date);
		std::string authorization = hmac_sha1("GET", date, this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::getMethod(outputstream, writeCallback, resp);
	}

	CURLcode Sha1Request::getMethod(HttpResponse& resp){
		const std::string date = dataGMT();
		this->header_.append("date", date);
		std::string authorization = hmac_sha1("GET", date, this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::getMethod(resp);
	}

	CURLcode Sha1Request::putMethod(void* inputstream, read_data_ptr readCallback, HttpResponse& resp){
		const std::string date = dataGMT();
		this->header_.append("date", date);
		std::string authorization = hmac_sha1("PUT", date, this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::putMethod(inputstream, readCallback, resp);
	}

	CURLcode Sha1Request::putMethod(const std::string& data, HttpResponse& resp){
		const std::string date = dataGMT();
		this->header_.append("date", date);
		std::string authorization = hmac_sha1("PUT", date, this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::putMethod(data, resp);
	}

	CURLcode Sha1Request::postMethod(const std::string& data, HttpResponse& resp){
		const std::string date = dataGMT();
		this->header_.append("date", date);
		std::string content_type;
		//libcurl中会默认为post操作添加Content-Type:application/x-www-form-urlencoded
		if(this->header_.get("Content-type", content_type) == false){
			this->header_.append("Content-Type", "application/x-www-form-urlencoded");
		}
		std::string authorization = hmac_sha1("POST", date, this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		
		return HttpRequest::postMethod(data, resp);
	}

	CURLcode Sha1Request::headMethod(HttpResponse& resp){
		const std::string date = dataGMT();
		this->header_.append("date", date);
		std::string authorization = hmac_sha1("HEAD", date, this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::headMethod(resp);
	}

	CURLcode Sha1Request::deleteMethod(HttpResponse& resp){
		const std::string date = dataGMT();
		this->header_.append("date", date);
		std::string authorization = hmac_sha1("DELETE", date, this->accessKeyId_, this->accessKeySecret_);
		this->header_.append("authorization", authorization);
		return HttpRequest::deleteMethod(resp);
	}
}