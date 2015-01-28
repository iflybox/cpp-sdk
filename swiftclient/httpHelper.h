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
	//�˴��������ؿ�������
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
    // brief:     ����header��
    // access:    public 
    // returns:   void
    // param:     const std::string & key               header��key������Range
    // param:     const std::string & value             header��value������10
    // author:    zhengyao
    // remarks:   ����Ĭ��ƴ��header��Ϊ��"key:value"
    //************************************
    void append(const std::string& key, const std::string& value){
		headers_[key] = value;
    }
    //************************************
    // method:    append
    // brief:     ����header��
    // access:    public 
    // returns:   void
    // param:     const std::string & key               
    // param:     int value
    // author:    zhengyao
    // remarks:   ����Ĭ�Ͻ�valueתΪ10���ƶ�Ӧ�ַ�����Ȼ��ƴ��
    //************************************
    void append(const std::string& key, int value){
        const int Int_Limit_Len = 20;
        char value_str[Int_Limit_Len];
        append( key, itoa(value, value_str, 10));
    }
	
	//************************************
	// method:	parse
	// brief:	�����ص�http header�ַ��������л�ΪHttpHeader
	// access:	public  
	// returns:	void - 
	// param:	std::string & headStream	- [in]
	// author:	zhengyao
	// remarks:	
	//************************************
	void deserialize(std::string& headStream);
	//************************************
	// method:	get
	// brief:	��http header���ͷ���ֶζ�Ӧ��ֵ
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
	// brief:	��ȡlibcurl��http header�ṹ
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
	//http ״̬��
    int			status_;
	//http ͷ��
	HttpHeader	header_;
	//http ����
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
    // brief:	���������ͷ��
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
	//			�������(�ò�����ֱ�Ӵ��ݸ�libcurl�Ļص�����)
	// param:	write_data_ptr writeCallback	- [in]
	//			���ع����еĻص�����
	// param:	HttpResponse & resp	- [in]
	//			���ʧ�ܣ����ص�Content��д��response��
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
	// remarks:	�÷�������������Ψһ��ͬ���ڸ÷����Ὣ����д��response��
	//************************************
	CURLcode getMethod(HttpResponse& resp);

    //************************************
    // method:	putMethod
    // brief:	HTTP PUT Method
    // access:	public  
    // returns:	CURLcode - 
    // param:	const std::string & data	- [in]
	//			���ϴ�����
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
	//			��inputstream��ϻ�ȡ���ϴ�����
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
	//			��POST������
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
	//libcurl���
    CURL*           curl_handle_;
	//�����Ӧ��url��ַ
    std::string     url_;
	//��ʱʱ��
    int             timeout_milsecs_;
    //����󶨵�httpͷ��
	HttpHeader      header_;
};



}




#endif // __IFLYBOX__HTTPHELPER_H__
