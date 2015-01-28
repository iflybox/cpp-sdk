#ifndef __MSP_ERROR_TO_STRING_H__
#define __MSP_ERROR_TO_STRING_H__

#include "msp_errors.h"
#include <list>

bool static error_to_string(int error_code , std::string &str_en , std::string &str_cn)
{
	if(error_code==MSP_SUCCESS)
	{
		str_cn="成功！";
		str_en="MSP_SUCCESS";
		return true;
	}
	switch(error_code)
	{
	case MSP_ERROR_FAIL:
		str_cn="失败！";
		str_en="MSP_ERROR_FAIL";
		break;
	case MSP_ERROR_EXCEPTION:
		str_cn="异常！";
		str_en="MSP_ERROR_EXCEPTION";
		break;
	case MSP_ERROR_GENERAL:
		str_cn="基码！";
		str_en="MSP_ERROR_GENERAL";
		break;
	case MSP_ERROR_OUT_OF_MEMORY:
		str_cn="内存越界！";
		str_en="MSP_ERROR_OUT_OF_MEMORY";
		break;
	case MSP_ERROR_FILE_NOT_FOUND:
		str_cn="没有找到文件！";
		str_en="MSP_ERROR_FILE_NOT_FOUND";
		break;
	case MSP_ERROR_NOT_SUPPORT:
		str_cn="不支持！";
		str_en="MSP_ERROR_NOT_SUPPORT";
		break;
	case MSP_ERROR_NOT_IMPLEMENT:
		str_cn="没有实现！";
		str_en="MSP_ERROR_NOT_IMPLEMENT";
		break;
	case MSP_ERROR_ACCESS:
		str_cn="没有权限！";
		str_en="MSP_ERROR_ACCESS";
		break;
	case MSP_ERROR_INVALID_PARA:
		str_cn="无效的参数！";
		str_en="MSP_ERROR_INVALID_PARA";
		break;
	case MSP_ERROR_INVALID_PARA_VALUE:
		str_cn="无效的参数值！";
		str_en="MSP_ERROR_INVALID_PARA_VALUE";
		break;
	case MSP_ERROR_INVALID_HANDLE:
		str_cn="无效的句柄！";
		str_en="MSP_ERROR_INVALID_HANDLE";
		break;
	case MSP_ERROR_INVALID_DATA:
		str_cn="无效的数据！";
		str_en="MSP_ERROR_INVALID_DATA";
		break;
	case MSP_ERROR_NO_LICENSE:
		str_cn="没有授权许可！";
		str_en="MSP_ERROR_NO_LICENSE";
		break;
	case MSP_ERROR_NOT_INIT:
		str_cn="没有初始化！";
		str_en="MSP_ERROR_NOT_INIT";
		break;
	case MSP_ERROR_NULL_HANDLE:
		str_cn="无效的Session ID！";
		str_en="MSP_ERROR_NULL_HANDLE";
		break;
	case MSP_ERROR_OVERFLOW:
		str_cn="溢出！";
		str_en="MSP_ERROR_OVERFLOW";
		break;
	case MSP_ERROR_TIME_OUT:
		str_cn="超时！";
		str_en="MSP_ERROR_TIME_OUT";
		break;
	case MSP_ERROR_OPEN_FILE:
		str_cn="打开文件出错！";
		str_en="MSP_ERROR_OPEN_FILE";
		break;
	case MSP_ERROR_NOT_FOUND:
		str_cn="没有发现！";
		str_en="MSP_ERROR_NOT_FOUND";
		break;
	case MSP_ERROR_NO_ENOUGH_BUFFER:
		str_cn="没有足够内存！";
		str_en="MSP_ERROR_NO_ENOUGH_BUFFER";
		break;
	case MSP_ERROR_NO_DATA:
		str_cn="没有数据！";
		str_en="MSP_ERROR_NO_DATA";
		break;
	case MSP_ERROR_NO_MORE_DATA:
		str_cn="没有更多数据！";
		str_en="MSP_ERROR_NO_MORE_DATA";
		break;
	case MSP_ERROR_NO_RESPONSE_DATA:
		str_cn="没有相应数据！";
		str_en="MSP_ERROR_NO_RESPONSE_DATA";
		break;
	case MSP_ERROR_ALREADY_EXIST:
		str_cn="已经存在！";
		str_en="MSP_ERROR_ALREADY_EXIST";
		break;
	case MSP_ERROR_LOAD_MODULE:
		str_cn="加载模块失败！";
		str_en="MSP_ERROR_LOAD_MODULE";
		break;
	case MSP_ERROR_BUSY:
		str_cn="忙碌！";
		str_en="MSP_ERROR_BUSY";
		break;
	case MSP_ERROR_INVALID_CONFIG:
		str_cn="无效的配置项！";
		str_en="MSP_ERROR_INVALID_CONFIG";
		break;
	case MSP_ERROR_VERSION_CHECK:
		str_cn="版本错误！";
		str_en="MSP_ERROR_VERSION_CHECK";
		break;
	case MSP_ERROR_CANCELED:
		str_cn="取消！";
		str_en="MSP_ERROR_CANCELED";
		break;
	case MSP_ERROR_INVALID_MEDIA_TYPE:
		str_cn="无效的媒体类型！";
		str_en="MSP_ERROR_INVALID_MEDIA_TYPE";
		break;
	case MSP_ERROR_CONFIG_INITIALIZE:
		str_cn="初始化失败！";
		str_en="MSP_ERROR_CONFIG_INITIALIZE";
		break;
	case MSP_ERROR_CREATE_HANDLE:
		str_cn="建立句柄错误！";
		str_en="MSP_ERROR_CREATE_HANDLE";
		break;
	case MSP_ERROR_CODING_LIB_NOT_LOAD:
		str_cn="编解码库未加载！";
		str_en="MSP_ERROR_CODING_LIB_NOT_LOAD";
		break;
//10200
	case MSP_ERROR_NET_GENERAL:
		str_cn="网络一般错误！";
		str_en="MSP_ERROR_NET_GENERAL";
		break;
	case MSP_ERROR_NET_OPENSOCK:
		str_cn="网络打开套接字错误！";
		str_en="MSP_ERROR_NET_OPENSOCK";
		break;
	case MSP_ERROR_NET_CONNECTSOCK:
		str_cn="网络套接字连接错误！";
		str_en="MSP_ERROR_NET_CONNECTSOCK";
		break;
	case MSP_ERROR_NET_ACCEPTSOCK:
		str_cn="网络套接字响应错误！";
		str_en="MSP_ERROR_NET_ACCEPTSOCK";
		break;
	case MSP_ERROR_NET_SENDSOCK:
		str_cn="网络套接字发送错误！";
		str_en="MSP_ERROR_NET_SENDSOCK";
		break;
	case MSP_ERROR_NET_RECVSOCK:
		str_cn="网络套接字接收错误！";
		str_en="MSP_ERROR_NET_RECVSOCK";
		break;
	case MSP_ERROR_NET_INVALIDSOCK:
		str_cn="网络无效的套接字！";
		str_en="MSP_ERROR_NET_INVALIDSOCK";
		break;
	case MSP_ERROR_NET_BADADDRESS:
		str_cn="网络无效的地址！";
		str_en="MSP_ERROR_NET_BADADDRESS";
		break;
	case MSP_ERROR_NET_BINDSEQUENCE:
		str_cn="网络绑定次序错误！";
		str_en="MSP_ERROR_NET_BINDSEQUENCE";
		break;
	case MSP_ERROR_NET_NOTOPENSOCK:
		str_cn="网络套接字没有打开！";
		str_en="MSP_ERROR_NET_NOTOPENSOCK";
		break;
	case MSP_ERROR_NET_NOTBIND:
		str_cn="网络没有绑定！";
		str_en="MSP_ERROR_NET_NOTBIND";
		break;
	case MSP_ERROR_NET_NOTLISTEN:
		str_cn="网络没有监听！";
		str_en="MSP_ERROR_NET_NOTLISTEN";
		break;
	case MSP_ERROR_NET_CONNECTCLOSE:
		str_cn="网络连接关闭！";
		str_en="MSP_ERROR_NET_CONNECTCLOSE";
		break;
	case MSP_ERROR_NET_NOTDGRAMSOCK:
		str_cn="网络非数据报套接字！";
		str_en="MSP_ERROR_NET_NOTDGRAMSOCK";
		break;
	case MSP_ERROR_NET_DNS:
		str_cn="网络DNS错误！";
		str_en="MSP_ERROR_NET_DNS";
		break;
//10300
	case MSP_ERROR_MSG_GENERAL:
		str_cn="消息一般错误！";
		str_en="MSP_ERROR_MSG_GENERAL";
		break;
	case MSP_ERROR_MSG_PARSE_ERROR:
		str_cn="消息解析错误！";
		str_en="MSP_ERROR_MSG_PARSE_ERROR";
		break;
	case MSP_ERROR_MSG_BUILD_ERROR:
		str_cn="消息构建错误！";
		str_en="MSP_ERROR_MSG_BUILD_ERROR";
		break;
	case MSP_ERROR_MSG_PARAM_ERROR:
		str_cn="消息参数错误！";
		str_en="MSP_ERROR_MSG_PARAM_ERROR";
		break;
	case MSP_ERROR_MSG_CONTENT_EMPTY:
		str_cn="消息CONTENT为空！";
		str_en="MSP_ERROR_MSG_CONTENT_EMPTY";
		break;
	case MSP_ERROR_MSG_INVALID_CONTENT_TYPE:
		str_cn="消息CONTENT类型无效！";
		str_en="MSP_ERROR_MSG_INVALID_CONTENT_TYPE";
		break;
	case MSP_ERROR_MSG_INVALID_CONTENT_LENGTH:
		str_cn="消息CONTENT长度无效！";
		str_en="MSP_ERROR_MSG_INVALID_CONTENT_LENGTH";
		break;
	case MSP_ERROR_MSG_INVALID_CONTENT_ENCODE:
		str_cn="消息CONTENT编码无效！";
		str_en="MSP_ERROR_MSG_INVALID_CONTENT_ENCODE";
		break;
	case MSP_ERROR_MSG_INVALID_KEY:
		str_cn="消息KEY无效！";
		str_en="MSP_ERROR_MSG_INVALID_KEY";
		break;
	case MSP_ERROR_MSG_KEY_EMPTY:
		str_cn="消息KEY为空！";
		str_en="MSP_ERROR_MSG_KEY_EMPTY";
		break;
	case MSP_ERROR_MSG_SESSION_ID_EMPTY:
		str_cn="消息会话ID为空！";
		str_en="MSP_ERROR_MSG_SESSION_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_LOGIN_ID_EMPTY:
		str_cn="消息登录ID为空！";
		str_en="MSP_ERROR_MSG_LOGIN_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_SYNC_ID_EMPTY:
		str_cn="消息同步ID为空！";
		str_en="MSP_ERROR_MSG_SYNC_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_APP_ID_EMPTY:
		str_cn="消息应用ID为空！";
		str_en="MSP_ERROR_MSG_APP_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_EXTERN_ID_EMPTY:
		str_cn="消息扩展ID为空！";
		str_en="MSP_ERROR_MSG_EXTERN_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_INVALID_CMD:
		str_cn="消息无效的命令！";
		str_en="MSP_ERROR_MSG_INVALID_CMD";
		break;
	case MSP_ERROR_MSG_INVALID_SUBJECT:
		str_cn="消息无效的主题！";
		str_en="MSP_ERROR_MSG_INVALID_SUBJECT";
		break;
	case MSP_ERROR_MSG_INVALID_VERSION:
		str_cn="消息无效的版本！";
		str_en="MSP_ERROR_MSG_INVALID_VERSION";
		break;
	case MSP_ERROR_MSG_NO_CMD:
		str_cn="消息没有命令！";
		str_en="MSP_ERROR_MSG_NO_CMD";
		break;
	case MSP_ERROR_MSG_NO_SUBJECT:
		str_cn="消息没有主题！";
		str_en="MSP_ERROR_MSG_NO_SUBJECT";
		break;
	case MSP_ERROR_MSG_NO_VERSION:
		str_cn="消息没有版本号！";
		str_en="MSP_ERROR_MSG_NO_VERSION";
		break;
	case MSP_ERROR_MSG_MSSP_EMPTY:
		str_cn="消息为空！";
		str_en="MSP_ERROR_MSG_MSSP_EMPTY";
		break;
	case MSP_ERROR_MSG_NEW_RESPONSE:
		str_cn="新建响应消息失败！";
		str_en="MSP_ERROR_MSG_NEW_RESPONSE";
		break;
	case MSP_ERROR_MSG_NEW_CONTENT:
		str_cn="消息新建Content失败！";
		str_en="MSP_ERROR_MSG_NEW_CONTENT";
		break;
	case MSP_ERROR_MSG_INVALID_SESSION_ID:
		str_cn="消息无效的会话ID！";
		str_en="MSP_ERROR_MSG_INVALID_SESSION_ID";
		break;
//10400
	case MSP_ERROR_DB_GENERAL:
		str_cn="数据库一般错误！";
		str_en="MSP_ERROR_DB_GENERAL";
		break;
	case MSP_ERROR_DB_EXCEPTION:
		str_cn="数据库异常！";
		str_en="MSP_ERROR_DB_EXCEPTION";
		break;
	case MSP_ERROR_DB_NO_RESULT:
		str_cn="数据库没有结果！";
		str_en="MSP_ERROR_DB_NO_RESULT";
		break;
	case MSP_ERROR_DB_INVALID_USER:
		str_cn="数据库无效的用户！";
		str_en="MSP_ERROR_DB_INVALID_USER";
		break;
	case MSP_ERROR_DB_INVALID_PWD:
		str_cn="数据库无效的密码！";
		str_en="MSP_ERROR_DB_INVALID_PWD";
		break;
	case MSP_ERROR_DB_CONNECT:
		str_cn="数据库连接错误！";
		str_en="MSP_ERROR_DB_CONNECT";
		break;
	case MSP_ERROR_DB_INVALID_SQL:
		str_cn="数据库无效的SQL！";
		str_en="MSP_ERROR_DB_INVALID_SQL";
		break;
	case MSP_ERROR_DB_INVALID_APPID:
		str_cn="数据库无效的APPID！";
		str_en="MSP_ERROR_DB_INVALID_APPID";
		break;
//10500
	case MSP_ERROR_RES_GENERAL:
		str_cn="资源一般错误！";
		str_en="MSP_ERROR_RES_GENERAL";
		break;
	case MSP_ERROR_RES_LOAD:
		str_cn="资源没有加载！";
		str_en="MSP_ERROR_RES_LOAD";
		break;
	case MSP_ERROR_RES_FREE:
		str_cn="资源空闲！";
		str_en="MSP_ERROR_RES_FREE";
		break;
	case MSP_ERROR_RES_MISSING:
		str_cn="资源缺失！";
		str_en="MSP_ERROR_RES_MISSING";
		break;
	case MSP_ERROR_RES_INVALID_NAME:
		str_cn="资源无效的名称！";
		str_en="MSP_ERROR_RES_INVALID_NAME";
		break;
	case MSP_ERROR_RES_INVALID_ID:
		str_cn="资源无效的ID！";
		str_en="MSP_ERROR_RES_INVALID_ID";
		break;
	case MSP_ERROR_RES_INVALID_IMG:
		str_cn="资源无效的映像！";
		str_en="MSP_ERROR_RES_INVALID_IMG";
		break;
	case MSP_ERROR_RES_WRITE:
		str_cn="资源写操作错误！";
		str_en="MSP_ERROR_RES_WRITE";
		break;
	case MSP_ERROR_RES_LEAK:
		str_cn="资源泄漏错误！";
		str_en="MSP_ERROR_RES_LEAK";
		break;
	case MSP_ERROR_RES_HEAD:
		str_cn="资源头部错误！";
		str_en="MSP_ERROR_RES_HEAD";
		break;
	case MSP_ERROR_RES_DATA:
		str_cn="资源数据错误！";
		str_en="MSP_ERROR_RES_DATA";
		break;
	case MSP_ERROR_RES_SKIP:
		str_cn="资源跳过！";
		str_en="MSP_ERROR_RES_SKIP";
		break;
//10600
	case MSP_ERROR_TTS_GENERAL:
		str_cn="合成错误基码！";
		str_en="MSP_ERROR_TTS_GENERAL";
		break;
	case MSP_ERROR_TTS_TEXTEND:
		str_cn="合成文本结束！";
		str_en="MSP_ERROR_TTS_TEXTEND";
		break;
	case MSP_ERROR_TTS_TEXT_EMPTY:
		str_cn="合成文本为空！";
		str_en="MSP_ERROR_TTS_TEXT_EMPTY";
		break;
//10700
	case MSP_ERROR_REC_GENERAL:
		str_cn="识别错误基码！";
		str_en="MSP_ERROR_REC_GENERAL";
		break;
	case MSP_ERROR_REC_INACTIVE:
		str_cn="识别处于不活跃状态！";
		str_en="MSP_ERROR_REC_INACTIVE";
		break;
	case MSP_ERROR_REC_GRAMMAR_ERROR:
		str_cn="识别语法错误！";
		str_en="MSP_ERROR_REC_GRAMMAR_ERROR";
		break;
	case MSP_ERROR_REC_NO_ACTIVE_GRAMMARS:
		str_cn="识别没有活跃的语法！";
		str_en="MSP_ERROR_REC_NO_ACTIVE_GRAMMARS";
		break;
	case MSP_ERROR_REC_DUPLICATE_GRAMMAR:
		str_cn="识别语法重复！";
		str_en="MSP_ERROR_REC_DUPLICATE_GRAMMAR";
		break;
	case MSP_ERROR_REC_INVALID_MEDIA_TYPE:
		str_cn="识别无效的媒体类型！";
		str_en="MSP_ERROR_REC_INVALID_MEDIA_TYPE";
		break;
	case MSP_ERROR_REC_INVALID_LANGUAGE:
		str_cn="识别无效的语言！";
		str_en="MSP_ERROR_REC_INVALID_LANGUAGE";
		break;
	case MSP_ERROR_REC_URI_NOT_FOUND:
		str_cn="识别没有对应的URI！";
		str_en="MSP_ERROR_REC_URI_NOT_FOUND";
		break;
	case MSP_ERROR_REC_URI_TIMEOUT:
		str_cn="识别获取URI超时！";
		str_en="MSP_ERROR_REC_URI_TIMEOUT";
		break;
	case MSP_ERROR_REC_URI_FETCH_ERROR:
		str_cn="识别获取URI内容时出错！";
		str_en="MSP_ERROR_REC_URI_FETCH_ERROR";
		break;
//10800
	case MSP_ERROR_EP_GENERAL:
		str_cn="EP一般错误！";
		str_en="MSP_ERROR_EP_GENERAL";
		break;
	case MSP_ERROR_EP_NO_SESSION_NAME:
		str_cn="EP连接没有名字！";
		str_en="MSP_ERROR_EP_NO_SESSION_NAME";
		break;
	case MSP_ERROR_EP_INACTIVE:
		str_cn="EP不活跃！";
		str_en="MSP_ERROR_EP_INACTIVE";
		break;
	case MSP_ERROR_EP_INITIALIZED:
		str_cn="EP初始化出错！";
		str_en="MSP_ERROR_EP_INITIALIZED";
		break;
//12000
	case MSP_ERROR_HTTP_BASE:
		str_cn="HTTP错误基码！";
		str_en="MSP_ERROR_HTTP_BASE";
		break;
//13000
	case MSP_ERROR_ISV_NO_USER:
		str_cn="声纹用户不存在！";
		str_en="MSP_ERROR_ISV_NO_USER";
		break;
//14000
	case MSP_ERROR_LUA_BASE:
		str_cn="LUA错误基码！";
		str_en="MSP_ERROR_LUA_BASE";
		break;
	case MSP_ERROR_LUA_YIELD:
		str_cn="LUA yield错误！";
		str_en="MSP_ERROR_LUA_YIELD";
		break;
	case MSP_ERROR_LUA_ERRRUN:
		str_cn="LUA运行错误！";
		str_en="MSP_ERROR_LUA_ERRRUN";
		break;
	case MSP_ERROR_LUA_ERRSYNTAX:
		str_cn="LUA语法错误！";
		str_en="MSP_ERROR_LUA_ERRSYNTAX";
		break;
	case MSP_ERROR_LUA_ERRMEM:
		str_cn="LUA内存错误！";
		str_en="MSP_ERROR_LUA_ERRMEM";
		break;
	case MSP_ERROR_LUA_ERRERR:
		str_cn="LUA错误！";
		str_en="MSP_ERROR_LUA_ERRERR";
		break;
//1000
	case 1000:
		str_cn="BATREC打开文件失败！";
		str_en="BATREC_ERROR_OPEN_FILE";
		break;
	case 1001:
		str_cn="LALR指标超过指定值！";
		str_en="BATREC_ERROR_LALR_TOO_LONG";
		break;
	case 1002:
		str_cn="结果对比不匹配！";
		str_en="BATREC_ERROR_COMPARE_RESULT_NOT_MATCH";
		break;
	case 1003:
		str_cn="结果长度对比不匹配！";
		str_en="BATREC_ERROR_COMPARE_RESULT_LEN_NOT_MATCH";
		break;
	case 1004:
		str_cn="结果长度对比时，脚本中的对比符号错误！";
		str_en="BATREC_ERROR_COMPARE_OPERATOR_ERROR";
		break;

	default:
		char buffer[20];
		sprintf(buffer,"错误码：%d",error_code);
		str_cn=buffer;
		sprintf(buffer,"errorcode：%d",error_code);
		str_en=buffer;
		break;
	}
	if(error_code>MSP_ERROR_HTTP_BASE && error_code<MSP_ERROR_ISV_NO_USER)
	{
		char buffer[20];
		sprintf(buffer,"HTTP %d 错误",error_code);
		str_cn=buffer;
		sprintf(buffer,"HTTP %d error",error_code);
		str_en=buffer;
	}
	return true;
}
#endif