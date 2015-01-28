
#include <sstream>
#include "httpHelper.h"
#include "exception.h"
#include "error.h"

/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 */
#ifndef _WIN32
char* itoa(int value, char* result, int base) {
// check that the base if valid
	if (base < 2 || base > 36) { *result = '\0'; return result; }
    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );
             
// Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
#endif
					
namespace iflybox
{

bool getline(std::string& stream, std::string& line){
	if(stream.empty())
		return false;
#ifdef _WIN32
	const char* delimiter = "\r\n";
#else
	const char* delimiter = "\n";
#endif
	std::string::size_type pos = stream.find(delimiter);
	if(std::string::npos == pos){
		line = stream;
	}
	else{
		line = stream.substr(0, pos);
		stream = stream.substr(pos + strlen(delimiter), stream.length());
	}
	return true;
}
    
void HttpHeader::deserialize(std::string& headStream){
	//ÇÐ¸î»»ÐÐ
	std::string header;
	while(getline(headStream, header)){
		std::string::size_type pos = header.find(": ");
		if(pos != std::string::npos){
			headers_[header.substr(0, pos)] = header.substr(pos + strlen(": "), header.length());
		}
	}
}

HttpRequest::HttpRequest(const std::string& url, int timeout)
    : url_(url), timeout_milsecs_(timeout){
    init(); 
}

HttpRequest::HttpRequest(const std::string& url, const HttpHeader& header, int timeout)
    : url_(url), timeout_milsecs_(timeout), header_(header){
    init();
}

HttpRequest::~HttpRequest(){
    if(curl_handle_)
        curl_easy_cleanup(curl_handle_);
}

void HttpRequest::init(){
	curl_handle_ = curl_easy_init();
	if(curl_handle_)
	{
//		char *url = curl_easy_escape( curl_handle_ , url_.c_str() , url_.length());
		curl_easy_setopt(curl_handle_, CURLOPT_URL, url_.c_str());
	}
	else
		throw iflybox::iflyException(ERROR_LIBCUR_INIT, "curl_easy_init return NULL", __FILE__, __LINE__);
}

HttpHeader HttpRequest::setheader(const HttpHeader& header){
    HttpHeader original = header_;
    header_ = header;
    return original;
} 

CURLcode HttpRequest::getMethod(void* outputstream, write_data_ptr writeCallback, HttpResponse& resp){
	CURLcode ccode = CURLE_OK;
	std::string header_str;
	curl_easy_setopt(curl_handle_, CURLOPT_READFUNCTION, NULL);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, writeCallback);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, (void *)outputstream);
	curl_easy_setopt(curl_handle_, CURLOPT_HEADERFUNCTION, HttpRequest::writeHeader);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEHEADER, (void *)&header_str);
	curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, timeout_milsecs_);
	curl_easy_setopt(curl_handle_, CURLOPT_HTTPGET, 1);
	if(header_.getslist())
		curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, header_.getslist());
	ccode = curl_easy_perform(curl_handle_);
	if(CURLE_OK == ccode){
		curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &resp.status_);
		resp.header_.deserialize(header_str);
	}
	return ccode;
}

CURLcode HttpRequest::getMethod(HttpResponse& resp){
	CURLcode ccode = CURLE_OK;
	std::string header_str;
	curl_easy_setopt(curl_handle_, CURLOPT_READFUNCTION, NULL);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, writeContent);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, (void *)&resp.content_);
	curl_easy_setopt(curl_handle_, CURLOPT_HEADERFUNCTION, HttpRequest::writeHeader);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEHEADER, (void *)&header_str);
	curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, timeout_milsecs_);
	curl_easy_setopt(curl_handle_, CURLOPT_HTTPGET, 1);
	if(header_.getslist())
		curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, header_.getslist());
	ccode = curl_easy_perform(curl_handle_);
	if(CURLE_OK == ccode){
		curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &resp.status_);
		resp.header_.deserialize(header_str);
	}
	return ccode;
}

CURLcode HttpRequest::putMethod(const std::string& data, HttpResponse& resp){
	CURLcode ccode = CURLE_OK;
	std::stringstream stream(data);
	std::string header_str;
	curl_easy_setopt(curl_handle_, CURLOPT_READFUNCTION, HttpRequest::readContent);
	curl_easy_setopt(curl_handle_, CURLOPT_READDATA, (void *)&stream);
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, HttpRequest::writeContent);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, (void *)&resp.content_);
	curl_easy_setopt(curl_handle_, CURLOPT_HEADERFUNCTION, HttpRequest::writeHeader);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEHEADER, (void *)&header_str);
	curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, timeout_milsecs_);
	curl_easy_setopt(curl_handle_, CURLOPT_UPLOAD, 1);
	if(header_.getslist())
		curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, header_.getslist());
	ccode = curl_easy_perform(curl_handle_);
	if(CURLE_OK == ccode){
		curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &resp.status_);
		resp.header_.deserialize(header_str);
	}
	return ccode;
}

CURLcode HttpRequest::putMethod(void* inputstream, read_data_ptr readCallback, HttpResponse& resp){
	CURLcode ccode = CURLE_OK;
	std::string header_str;
	curl_easy_setopt(curl_handle_, CURLOPT_READFUNCTION, readCallback);
	curl_easy_setopt(curl_handle_, CURLOPT_READDATA, inputstream);
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, HttpRequest::writeContent);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, (void *)&resp.content_);
	curl_easy_setopt(curl_handle_, CURLOPT_HEADERFUNCTION, HttpRequest::writeHeader);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEHEADER, (void *)&header_str);
	curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, timeout_milsecs_);
	curl_easy_setopt(curl_handle_, CURLOPT_UPLOAD, 1);
	if(header_.getslist())
		curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, header_.getslist());
	ccode = curl_easy_perform(curl_handle_);
	if(CURLE_OK == ccode){
		curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &resp.status_);
		resp.header_.deserialize(header_str);
	}
	return ccode;
}

CURLcode HttpRequest::postMethod(const std::string& data, HttpResponse& resp){
	CURLcode ccode = CURLE_OK;
	std::string header_str;
	curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDS, data.data()); 
	curl_easy_setopt(curl_handle_, CURLOPT_POSTFIELDSIZE, data.size()); 
	curl_easy_setopt(curl_handle_, CURLOPT_HEADERFUNCTION, HttpRequest::writeHeader);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEHEADER, (void *)&header_str);
 	curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, HttpRequest::writeContent);  
 	curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, (void *)&resp.content_);
	curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, timeout_milsecs_);
	curl_easy_setopt(curl_handle_, CURLOPT_POST, 1);
	if(header_.getslist())
		curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, header_.getslist());
	ccode = curl_easy_perform(curl_handle_);
	if(CURLE_OK == ccode){
		curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &resp.status_);
		resp.header_.deserialize(header_str);
	}
	return ccode;
}

CURLcode HttpRequest::headMethod(HttpResponse& resp){
//	CURLOPT_NOBODY
	CURLcode ccode = CURLE_OK;
	std::string header_str;
	curl_easy_setopt(curl_handle_, CURLOPT_READFUNCTION, NULL);  
	curl_easy_setopt(curl_handle_, CURLOPT_HEADERFUNCTION, HttpRequest::writeHeader);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEHEADER, (void *)&header_str);
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, HttpRequest::writeContent);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, (void *)&resp.content_);
	curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, timeout_milsecs_);
	curl_easy_setopt(curl_handle_, CURLOPT_NOBODY, 1);
	if(header_.getslist())
		curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, header_.getslist());
	ccode = curl_easy_perform(curl_handle_);
	if(CURLE_OK == ccode){
		curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &resp.status_);
		resp.header_.deserialize(header_str);
	}
	return ccode;
}

CURLcode HttpRequest::deleteMethod(HttpResponse& resp){
	CURLcode ccode = CURLE_OK;
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, HttpRequest::writeContent);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, (void *)&resp.content_);
	curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, timeout_milsecs_);
	curl_easy_setopt(curl_handle_, CURLOPT_CUSTOMREQUEST, "DELETE");  
	if(header_.getslist())
		curl_easy_setopt(curl_handle_, CURLOPT_HTTPHEADER, header_.getslist());
	ccode = curl_easy_perform(curl_handle_);
	if(CURLE_OK == ccode)
		curl_easy_getinfo(curl_handle_, CURLINFO_RESPONSE_CODE, &resp.status_);
	return ccode;
}





size_t HttpRequest::writeHeader(void* buffer, size_t size, size_t nmemb, void* userp){
// 	HttpHeader* header = (HttpHeader*)userp;
// 	std::string header_str;
// 	
// 	header_str.append((const char*)buffer, size * nmemb);
// 	header->deserialize(header_str);
	std::string* header_str = (std::string*)userp;
	header_str->append((const char*)buffer, size * nmemb);
	return size * nmemb;
}

size_t HttpRequest::writeContent(void* buffer, size_t size, size_t nmemb, void* userp){
	std::string* content = (std::string*)userp;
	content->append((const char*)buffer, size * nmemb);
	return size * nmemb;
}


size_t HttpRequest::readContent(void* buffer, size_t size, size_t nmemb, void* userp){
	std::stringstream * data = (std::stringstream*)userp;
	size_t avail = data->tellp() - data->tellg();
	size_t read_bytes = avail > (size * nmemb) ? (size * nmemb) : avail;
	if(read_bytes > 0)
		data->read((char*)buffer, read_bytes);
	return read_bytes;
}

}

