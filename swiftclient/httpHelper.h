#ifndef __IFLYBOX__HTTPHELPER_H__
#define __IFLYBOX__HTTPHELPER_H__

#include "common.h"
#include "curl/curl.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* itoa(int value, char* result, int base);

namespace iflybox
{
	typedef size_t (*read_data_ptr)(void* buffer, size_t size, size_t nmemb, void* inputstream);
	typedef size_t (*write_data_ptr)(void* buffer, size_t size, size_t nmemb, void* inputstream);
    class HttpHeader{
public:
    HttpHeader()
        : slist_(NULL){
        //
    }
	//此处必须重载拷贝构造
	HttpHeader(const HttpHeader& header){
		*this = header;
	}

	HttpHeader& operator = (const HttpHeader& header){
		this->slist_ = NULL;
		this->headers_ = header.headers_;
		return *this;
	}
    ~HttpHeader(){
        if(slist_){
            curl_slist_free_all(slist_);
            slist_ = NULL;
        }
    }
// 	void append(const std::string& headstring){
// 		slist_ = curl_slist_append(slist_, headstring.c_str());
// 	}
    //************************************
    // method:    append
    // brief:     增加header项
    // access:    public 
    // returns:   void
    // param:     const std::string & key               header项key，例如Range
    // param:     const std::string & value             header项value，例如10
    // author:    zhengyao
    // remarks:   函数默认拼接header项为："key:value"
    //************************************
    void append(const std::string& key, const std::string& value){
		headers_[key] = value;
    }
    //************************************
    // method:    append
    // brief:     增加header项
    // access:    public 
    // returns:   void
    // param:     const std::string & key               
    // param:     int value
    // author:    zhengyao
    // remarks:   函数默认将value转为10进制对应字符串，然后拼接
    //************************************
    void append(const std::string& key, int value){
        const int Int_Limit_Len = 20;
        char value_str[Int_Limit_Len];
        append( key, itoa(value, value_str, 10));
    }
	
	//************************************
	// method:	parse
	// brief:	将返回的http header字符串反序列化为HttpHeader
	// access:	public  
	// returns:	void - 
	// param:	std::string & headStream	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	void deserialize(std::string& headStream);
	//************************************
	// method:	get
	// brief:	从http header获得头域字段对应的值
	// access:	public  
	// returns:	bool - 
	// param:	const std::string & key	- [in]
	// param:	std::string & value	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	bool get(const std::string& key, std::string& value)const{
		HttpHeaderMap::const_iterator iter = headers_.find(key);
		if(iter != headers_.end()){
			value = iter->second;
			return true;
		}
		return false;
	}

	//************************************
	// method:	getslist
	// brief:	获取libcurl中http header结构
	// access:	public  
	// returns:	struct curl_slist* - 
	// author:	zhengyao
	// remarks:	
	//************************************
	struct curl_slist* getslist(){
		if(slist_)
			return slist_;
		for(HttpHeaderMap::const_iterator iter = headers_.begin(); iter != headers_.end(); ++iter){
			slist_ = curl_slist_append(slist_, (iter->first + ": " + iter->second).c_str());
		}
		return slist_;
	}
	
private:
	struct curl_slist* slist_;
public:
	HttpHeaderMap	headers_;
};


class HttpResponse{
public:
	HttpResponse(){}
	int getStatus(){
		return status_;
	}
	const std::string getContent(){
		return content_;
	}
	const HttpHeader& getHeader(){
		return header_;
	}
public:
	//http 状态码
    int			status_;
	//http 头域
	HttpHeader	header_;
	//http 正文
    std::string	content_;
};

class HttpRequest{
public:
    HttpRequest(const std::string& url, int timeout);
    HttpRequest(const std::string& url, const HttpHeader& header, int timeout);
	~HttpRequest();
	CURL* getCurl()const{
		return curl_handle_;
	}
	void setTimeout(int milsec){
		timeout_milsecs_ = milsec;
	}
	int getTimeout(){
		return timeout_milsecs_;
	}

    //************************************
    // method:	setheader
    // brief:	设置请求的头域
    // access:	public  
    // returns:	iflybox::HttpHeader - 
    // param:	const HttpHeader & header	- [in]
    // author:	zhengyao
    // remarks:	
    //************************************
    HttpHeader setheader(const HttpHeader& header);

	//************************************
	// method:	getMethod
	// brief:	HTTP GET Method
	// access:	public  
	// returns:	CURLcode - 
	// param:	void * outputstream	- [in]			
	//			输出参数(该参数会直接传递给libcurl的回调函数)
	// param:	write_data_ptr writeCallback	- [in]
	//			下载过程中的回调函数
	// param:	HttpResponse & resp	- [in]
	//			如果失败，返回的Content会写入response中
	// author:	zhengyao
	// remarks:	
	//************************************
	CURLcode getMethod(void* outputstream, write_data_ptr writeCallback, HttpResponse& resp);
	//************************************
	// method:	getMethod
	// brief:	HTTP GET Method
	// access:	public  
	// returns:	CURLcode - 
	// param:	HttpResponse & resp	- [in]
	// author:	zhengyao
	// remarks:	该方法和上面重载唯一不同在于该方法会将数据写入response中
	//************************************
	CURLcode getMethod(HttpResponse& resp);

    //************************************
    // method:	putMethod
    // brief:	HTTP PUT Method
    // access:	public  
    // returns:	CURLcode - 
    // param:	const std::string & data	- [in]
	//			待上传数据
    // param:	HttpResponse & resp	- [in]
    // author:	zhengyao
    // remarks:	
    //************************************
    CURLcode putMethod(const std::string& data, HttpResponse& resp);

	//************************************
	// method:	putMethod
	// brief:	HTTP PUT Method
	// access:	public  
	// returns:	CURLcode - 
	// param:	void * inputstream	- [in]
	// param:	read_data_ptr readCallback	- [in]
	//			和inputstream配合获取待上传数据
	// param:	HttpResponse & resp	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	CURLcode putMethod(void* inputstream, read_data_ptr readCallback, HttpResponse& resp);
	
	//************************************
	// method:	postMethod
	// brief:	HTTP POST Method
	// access:	public  
	// returns:	CURLcode - 
	// param:	const std::string & data	- [in]
	//			待POST的数据
	// param:	HttpResponse & resp	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	CURLcode postMethod(const std::string& data, HttpResponse& resp);

    //************************************
    // method:	headMethod
    // brief:	HTTP HEAD Method
    // access:	public  
    // returns:	CURLcode - 
    // param:	HttpResponse & resp	- [in]
    // author:	zhengyao
    // remarks:	
    //************************************
    CURLcode headMethod(HttpResponse& resp);

    //************************************
    // method:	deleteMethod
    // brief:	HTTP DELETE Method
    // access:	public  
    // returns:	CURLcode - 
    // param:	HttpResponse & resp	- [in]
    // author:	zhengyao
    // remarks:	
    //************************************
    CURLcode deleteMethod(HttpResponse& resp);
public:
	static size_t writeHeader(void* buffer, size_t size, size_t nmemb, void* userp);
	static size_t writeContent(void* buffer, size_t size, size_t nmemb, void* userp);
	static size_t readContent(void* buffer, size_t size, size_t nmemb, void* userp);
protected:
    void init();
private:
	//libcurl句柄
    CURL*           curl_handle_;
	//请求对应的url地址
    std::string     url_;
	//超时时间
    int             timeout_milsecs_;
    //请求绑定的http头域
	HttpHeader      header_;
};



}




#endif // __IFLYBOX__HTTPHELPER_H__
