#ifndef __CSSP__ERROR_H__
#define __CSSP__ERROR_H__

#define IFLYBOX_SUCCESS         200
#define SERVICE_PREFIX          0
#define IFLYBOX_ERROR_UNKNOW	SERVICE_PREFIX + 1

//用于http状态码的偏移头(用于区分其他服务的http状态码)
#define HTTP_PREFIX             SERVICE_PREFIX + 0
//libcurl错误偏移头，重定向libcurl的库错误码
#define ERROR_PREFIX_LIBCURL    SERVICE_PREFIX + 1000

//自定义错误码偏移头
#define ERROR_PREFIX_COMMON				SERVICE_PREFIX + 2000
//curl_easy_init失败
#define ERROR_LIBCUR_INIT				ERROR_PREFIX_COMMON + 1
//multipart uploadid不存在
#define ERROR_UPLOADID_NOTEXIST			ERROR_PREFIX_COMMON + 2
//参数错误
#define	ERROR_PARAM_WRONG				ERROR_PREFIX_COMMON + 3		
//json解析失败
#define ERROR_JSON_PARSEFAILED			ERROR_PREFIX_COMMON + 4
//accessKeyId格式错误
#define ERROR_ACCESSKEYID_FORMATERR		ERROR_PREFIX_COMMON + 5
//参数超出范围
#define ERROR_OVERRANGE					ERROR_PREFIX_COMMON + 6
//申请内存错误
#define ERROR_MALLOC_BAD				ERROR_PREFIX_COMMON + 7

#endif // __CSSP__ERROR_H__