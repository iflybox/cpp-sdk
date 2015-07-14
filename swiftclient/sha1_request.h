#ifndef __CSSP__SHA1_REQUEST_H__
#define __CSSP__SHA1_REQUEST_H__
#include <string>
#include "httpHelper.h"

#ifdef _WIN32
#include <regex>
using std::tr1::regex;
using std::tr1::cmatch;
using std::tr1::regex_search;
#else
#include <boost/regex.hpp>
using boost::regex;
using boost::cmatch;
using boost::regex_search;
#endif

namespace cssp{

	class Sha1Request : public HttpRequest{
	public:
		Sha1Request(const std::string& url, int timeout, const std::string& accessKeyId, const std::string& accessKeySecret)
			:HttpRequest(url, timeout), accessKeyId_(accessKeyId), accessKeySecret_(accessKeySecret){
			//empty
		}
		CURLcode getMethod(void* outputstream, write_data_ptr writeCallback, HttpResponse& resp);

		CURLcode getMethod(HttpResponse& resp);

		CURLcode putMethod(const std::string& data, HttpResponse& resp);
	
		CURLcode putMethod(void* inputstream, read_data_ptr readCallback, HttpResponse& resp);
	
		CURLcode postMethod(const std::string& data, HttpResponse& resp);

	    CURLcode headMethod(HttpResponse& resp);

	    CURLcode deleteMethod(HttpResponse& resp);

		CURLcode customMethod(const std::string& method, HttpResponse& resp);
	private:
		std::string hmac_sha1(const std::string& method, const std::string& date, const std::string& accessKeyId, const std::string& accessKeySecret);
		std::string string_to_sign(const std::string& method, const std::string& date);
		bool parse_internal(const std::string& url, std::string& retStr);
		std::string parse_url(const std::string& url);
		std::string dataGMT();
		int base64_encode(const char *str,int str_len,char *encode,int encode_len);
	private:
		std::string accessKeyId_;
		std::string accessKeySecret_;
	};

}

#endif // __CSSP__SHA1_REQUEST_H__