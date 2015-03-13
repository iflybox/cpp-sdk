
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
					
namespace cssp
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
			std::string lower_key = header.substr(0, pos);
			std::transform(lower_key.begin(), lower_key.end(), lower_key.begin(), std::tolower);
			headers_[lower_key] = header.substr(pos + strlen(": "), header.length());
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
		curl_easy_setopt(curl_handle_, CURLOPT_URL, URLEncode(url_).c_str());
	}
	else
		throw cssp::iflyException(ERROR_LIBCUR_INIT, "curl_easy_init return NULL", __FILE__, __LINE__);
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
	curl_easy_setopt(curl_handle_, CURLOPT_FOLLOWLOCATION, 1);
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
	curl_easy_setopt(curl_handle_, CURLOPT_FOLLOWLOCATION, 1);
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
	curl_easy_setopt(curl_handle_, CURLOPT_FOLLOWLOCATION, 1);
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
	return customMethod("DELETE", resp);
}

CURLcode HttpRequest::customMethod(const std::string& method, HttpResponse& resp){
	CURLcode ccode = CURLE_OK;
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEFUNCTION, HttpRequest::writeContent);  
	curl_easy_setopt(curl_handle_, CURLOPT_WRITEDATA, (void *)&resp.content_);
	curl_easy_setopt(curl_handle_, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl_handle_, CURLOPT_TIMEOUT_MS, timeout_milsecs_);
	curl_easy_setopt(curl_handle_, CURLOPT_CUSTOMREQUEST, method.c_str());  
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

