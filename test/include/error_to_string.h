#ifndef __MSP_ERROR_TO_STRING_H__
#define __MSP_ERROR_TO_STRING_H__

#include "msp_errors.h"
#include <list>

bool static error_to_string(int error_code , std::string &str_en , std::string &str_cn)
{
	if(error_code==MSP_SUCCESS)
	{
		str_cn="�ɹ���";
		str_en="MSP_SUCCESS";
		return true;
	}
	switch(error_code)
	{
	case MSP_ERROR_FAIL:
		str_cn="ʧ�ܣ�";
		str_en="MSP_ERROR_FAIL";
		break;
	case MSP_ERROR_EXCEPTION:
		str_cn="�쳣��";
		str_en="MSP_ERROR_EXCEPTION";
		break;
	case MSP_ERROR_GENERAL:
		str_cn="���룡";
		str_en="MSP_ERROR_GENERAL";
		break;
	case MSP_ERROR_OUT_OF_MEMORY:
		str_cn="�ڴ�Խ�磡";
		str_en="MSP_ERROR_OUT_OF_MEMORY";
		break;
	case MSP_ERROR_FILE_NOT_FOUND:
		str_cn="û���ҵ��ļ���";
		str_en="MSP_ERROR_FILE_NOT_FOUND";
		break;
	case MSP_ERROR_NOT_SUPPORT:
		str_cn="��֧�֣�";
		str_en="MSP_ERROR_NOT_SUPPORT";
		break;
	case MSP_ERROR_NOT_IMPLEMENT:
		str_cn="û��ʵ�֣�";
		str_en="MSP_ERROR_NOT_IMPLEMENT";
		break;
	case MSP_ERROR_ACCESS:
		str_cn="û��Ȩ�ޣ�";
		str_en="MSP_ERROR_ACCESS";
		break;
	case MSP_ERROR_INVALID_PARA:
		str_cn="��Ч�Ĳ�����";
		str_en="MSP_ERROR_INVALID_PARA";
		break;
	case MSP_ERROR_INVALID_PARA_VALUE:
		str_cn="��Ч�Ĳ���ֵ��";
		str_en="MSP_ERROR_INVALID_PARA_VALUE";
		break;
	case MSP_ERROR_INVALID_HANDLE:
		str_cn="��Ч�ľ����";
		str_en="MSP_ERROR_INVALID_HANDLE";
		break;
	case MSP_ERROR_INVALID_DATA:
		str_cn="��Ч�����ݣ�";
		str_en="MSP_ERROR_INVALID_DATA";
		break;
	case MSP_ERROR_NO_LICENSE:
		str_cn="û����Ȩ��ɣ�";
		str_en="MSP_ERROR_NO_LICENSE";
		break;
	case MSP_ERROR_NOT_INIT:
		str_cn="û�г�ʼ����";
		str_en="MSP_ERROR_NOT_INIT";
		break;
	case MSP_ERROR_NULL_HANDLE:
		str_cn="��Ч��Session ID��";
		str_en="MSP_ERROR_NULL_HANDLE";
		break;
	case MSP_ERROR_OVERFLOW:
		str_cn="�����";
		str_en="MSP_ERROR_OVERFLOW";
		break;
	case MSP_ERROR_TIME_OUT:
		str_cn="��ʱ��";
		str_en="MSP_ERROR_TIME_OUT";
		break;
	case MSP_ERROR_OPEN_FILE:
		str_cn="���ļ�����";
		str_en="MSP_ERROR_OPEN_FILE";
		break;
	case MSP_ERROR_NOT_FOUND:
		str_cn="û�з��֣�";
		str_en="MSP_ERROR_NOT_FOUND";
		break;
	case MSP_ERROR_NO_ENOUGH_BUFFER:
		str_cn="û���㹻�ڴ棡";
		str_en="MSP_ERROR_NO_ENOUGH_BUFFER";
		break;
	case MSP_ERROR_NO_DATA:
		str_cn="û�����ݣ�";
		str_en="MSP_ERROR_NO_DATA";
		break;
	case MSP_ERROR_NO_MORE_DATA:
		str_cn="û�и������ݣ�";
		str_en="MSP_ERROR_NO_MORE_DATA";
		break;
	case MSP_ERROR_NO_RESPONSE_DATA:
		str_cn="û����Ӧ���ݣ�";
		str_en="MSP_ERROR_NO_RESPONSE_DATA";
		break;
	case MSP_ERROR_ALREADY_EXIST:
		str_cn="�Ѿ����ڣ�";
		str_en="MSP_ERROR_ALREADY_EXIST";
		break;
	case MSP_ERROR_LOAD_MODULE:
		str_cn="����ģ��ʧ�ܣ�";
		str_en="MSP_ERROR_LOAD_MODULE";
		break;
	case MSP_ERROR_BUSY:
		str_cn="æµ��";
		str_en="MSP_ERROR_BUSY";
		break;
	case MSP_ERROR_INVALID_CONFIG:
		str_cn="��Ч�������";
		str_en="MSP_ERROR_INVALID_CONFIG";
		break;
	case MSP_ERROR_VERSION_CHECK:
		str_cn="�汾����";
		str_en="MSP_ERROR_VERSION_CHECK";
		break;
	case MSP_ERROR_CANCELED:
		str_cn="ȡ����";
		str_en="MSP_ERROR_CANCELED";
		break;
	case MSP_ERROR_INVALID_MEDIA_TYPE:
		str_cn="��Ч��ý�����ͣ�";
		str_en="MSP_ERROR_INVALID_MEDIA_TYPE";
		break;
	case MSP_ERROR_CONFIG_INITIALIZE:
		str_cn="��ʼ��ʧ�ܣ�";
		str_en="MSP_ERROR_CONFIG_INITIALIZE";
		break;
	case MSP_ERROR_CREATE_HANDLE:
		str_cn="�����������";
		str_en="MSP_ERROR_CREATE_HANDLE";
		break;
	case MSP_ERROR_CODING_LIB_NOT_LOAD:
		str_cn="������δ���أ�";
		str_en="MSP_ERROR_CODING_LIB_NOT_LOAD";
		break;
//10200
	case MSP_ERROR_NET_GENERAL:
		str_cn="����һ�����";
		str_en="MSP_ERROR_NET_GENERAL";
		break;
	case MSP_ERROR_NET_OPENSOCK:
		str_cn="������׽��ִ���";
		str_en="MSP_ERROR_NET_OPENSOCK";
		break;
	case MSP_ERROR_NET_CONNECTSOCK:
		str_cn="�����׽������Ӵ���";
		str_en="MSP_ERROR_NET_CONNECTSOCK";
		break;
	case MSP_ERROR_NET_ACCEPTSOCK:
		str_cn="�����׽�����Ӧ����";
		str_en="MSP_ERROR_NET_ACCEPTSOCK";
		break;
	case MSP_ERROR_NET_SENDSOCK:
		str_cn="�����׽��ַ��ʹ���";
		str_en="MSP_ERROR_NET_SENDSOCK";
		break;
	case MSP_ERROR_NET_RECVSOCK:
		str_cn="�����׽��ֽ��մ���";
		str_en="MSP_ERROR_NET_RECVSOCK";
		break;
	case MSP_ERROR_NET_INVALIDSOCK:
		str_cn="������Ч���׽��֣�";
		str_en="MSP_ERROR_NET_INVALIDSOCK";
		break;
	case MSP_ERROR_NET_BADADDRESS:
		str_cn="������Ч�ĵ�ַ��";
		str_en="MSP_ERROR_NET_BADADDRESS";
		break;
	case MSP_ERROR_NET_BINDSEQUENCE:
		str_cn="����󶨴������";
		str_en="MSP_ERROR_NET_BINDSEQUENCE";
		break;
	case MSP_ERROR_NET_NOTOPENSOCK:
		str_cn="�����׽���û�д򿪣�";
		str_en="MSP_ERROR_NET_NOTOPENSOCK";
		break;
	case MSP_ERROR_NET_NOTBIND:
		str_cn="����û�а󶨣�";
		str_en="MSP_ERROR_NET_NOTBIND";
		break;
	case MSP_ERROR_NET_NOTLISTEN:
		str_cn="����û�м�����";
		str_en="MSP_ERROR_NET_NOTLISTEN";
		break;
	case MSP_ERROR_NET_CONNECTCLOSE:
		str_cn="�������ӹرգ�";
		str_en="MSP_ERROR_NET_CONNECTCLOSE";
		break;
	case MSP_ERROR_NET_NOTDGRAMSOCK:
		str_cn="��������ݱ��׽��֣�";
		str_en="MSP_ERROR_NET_NOTDGRAMSOCK";
		break;
	case MSP_ERROR_NET_DNS:
		str_cn="����DNS����";
		str_en="MSP_ERROR_NET_DNS";
		break;
//10300
	case MSP_ERROR_MSG_GENERAL:
		str_cn="��Ϣһ�����";
		str_en="MSP_ERROR_MSG_GENERAL";
		break;
	case MSP_ERROR_MSG_PARSE_ERROR:
		str_cn="��Ϣ��������";
		str_en="MSP_ERROR_MSG_PARSE_ERROR";
		break;
	case MSP_ERROR_MSG_BUILD_ERROR:
		str_cn="��Ϣ��������";
		str_en="MSP_ERROR_MSG_BUILD_ERROR";
		break;
	case MSP_ERROR_MSG_PARAM_ERROR:
		str_cn="��Ϣ��������";
		str_en="MSP_ERROR_MSG_PARAM_ERROR";
		break;
	case MSP_ERROR_MSG_CONTENT_EMPTY:
		str_cn="��ϢCONTENTΪ�գ�";
		str_en="MSP_ERROR_MSG_CONTENT_EMPTY";
		break;
	case MSP_ERROR_MSG_INVALID_CONTENT_TYPE:
		str_cn="��ϢCONTENT������Ч��";
		str_en="MSP_ERROR_MSG_INVALID_CONTENT_TYPE";
		break;
	case MSP_ERROR_MSG_INVALID_CONTENT_LENGTH:
		str_cn="��ϢCONTENT������Ч��";
		str_en="MSP_ERROR_MSG_INVALID_CONTENT_LENGTH";
		break;
	case MSP_ERROR_MSG_INVALID_CONTENT_ENCODE:
		str_cn="��ϢCONTENT������Ч��";
		str_en="MSP_ERROR_MSG_INVALID_CONTENT_ENCODE";
		break;
	case MSP_ERROR_MSG_INVALID_KEY:
		str_cn="��ϢKEY��Ч��";
		str_en="MSP_ERROR_MSG_INVALID_KEY";
		break;
	case MSP_ERROR_MSG_KEY_EMPTY:
		str_cn="��ϢKEYΪ�գ�";
		str_en="MSP_ERROR_MSG_KEY_EMPTY";
		break;
	case MSP_ERROR_MSG_SESSION_ID_EMPTY:
		str_cn="��Ϣ�ỰIDΪ�գ�";
		str_en="MSP_ERROR_MSG_SESSION_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_LOGIN_ID_EMPTY:
		str_cn="��Ϣ��¼IDΪ�գ�";
		str_en="MSP_ERROR_MSG_LOGIN_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_SYNC_ID_EMPTY:
		str_cn="��Ϣͬ��IDΪ�գ�";
		str_en="MSP_ERROR_MSG_SYNC_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_APP_ID_EMPTY:
		str_cn="��ϢӦ��IDΪ�գ�";
		str_en="MSP_ERROR_MSG_APP_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_EXTERN_ID_EMPTY:
		str_cn="��Ϣ��չIDΪ�գ�";
		str_en="MSP_ERROR_MSG_EXTERN_ID_EMPTY";
		break;
	case MSP_ERROR_MSG_INVALID_CMD:
		str_cn="��Ϣ��Ч�����";
		str_en="MSP_ERROR_MSG_INVALID_CMD";
		break;
	case MSP_ERROR_MSG_INVALID_SUBJECT:
		str_cn="��Ϣ��Ч�����⣡";
		str_en="MSP_ERROR_MSG_INVALID_SUBJECT";
		break;
	case MSP_ERROR_MSG_INVALID_VERSION:
		str_cn="��Ϣ��Ч�İ汾��";
		str_en="MSP_ERROR_MSG_INVALID_VERSION";
		break;
	case MSP_ERROR_MSG_NO_CMD:
		str_cn="��Ϣû�����";
		str_en="MSP_ERROR_MSG_NO_CMD";
		break;
	case MSP_ERROR_MSG_NO_SUBJECT:
		str_cn="��Ϣû�����⣡";
		str_en="MSP_ERROR_MSG_NO_SUBJECT";
		break;
	case MSP_ERROR_MSG_NO_VERSION:
		str_cn="��Ϣû�а汾�ţ�";
		str_en="MSP_ERROR_MSG_NO_VERSION";
		break;
	case MSP_ERROR_MSG_MSSP_EMPTY:
		str_cn="��ϢΪ�գ�";
		str_en="MSP_ERROR_MSG_MSSP_EMPTY";
		break;
	case MSP_ERROR_MSG_NEW_RESPONSE:
		str_cn="�½���Ӧ��Ϣʧ�ܣ�";
		str_en="MSP_ERROR_MSG_NEW_RESPONSE";
		break;
	case MSP_ERROR_MSG_NEW_CONTENT:
		str_cn="��Ϣ�½�Contentʧ�ܣ�";
		str_en="MSP_ERROR_MSG_NEW_CONTENT";
		break;
	case MSP_ERROR_MSG_INVALID_SESSION_ID:
		str_cn="��Ϣ��Ч�ĻỰID��";
		str_en="MSP_ERROR_MSG_INVALID_SESSION_ID";
		break;
//10400
	case MSP_ERROR_DB_GENERAL:
		str_cn="���ݿ�һ�����";
		str_en="MSP_ERROR_DB_GENERAL";
		break;
	case MSP_ERROR_DB_EXCEPTION:
		str_cn="���ݿ��쳣��";
		str_en="MSP_ERROR_DB_EXCEPTION";
		break;
	case MSP_ERROR_DB_NO_RESULT:
		str_cn="���ݿ�û�н����";
		str_en="MSP_ERROR_DB_NO_RESULT";
		break;
	case MSP_ERROR_DB_INVALID_USER:
		str_cn="���ݿ���Ч���û���";
		str_en="MSP_ERROR_DB_INVALID_USER";
		break;
	case MSP_ERROR_DB_INVALID_PWD:
		str_cn="���ݿ���Ч�����룡";
		str_en="MSP_ERROR_DB_INVALID_PWD";
		break;
	case MSP_ERROR_DB_CONNECT:
		str_cn="���ݿ����Ӵ���";
		str_en="MSP_ERROR_DB_CONNECT";
		break;
	case MSP_ERROR_DB_INVALID_SQL:
		str_cn="���ݿ���Ч��SQL��";
		str_en="MSP_ERROR_DB_INVALID_SQL";
		break;
	case MSP_ERROR_DB_INVALID_APPID:
		str_cn="���ݿ���Ч��APPID��";
		str_en="MSP_ERROR_DB_INVALID_APPID";
		break;
//10500
	case MSP_ERROR_RES_GENERAL:
		str_cn="��Դһ�����";
		str_en="MSP_ERROR_RES_GENERAL";
		break;
	case MSP_ERROR_RES_LOAD:
		str_cn="��Դû�м��أ�";
		str_en="MSP_ERROR_RES_LOAD";
		break;
	case MSP_ERROR_RES_FREE:
		str_cn="��Դ���У�";
		str_en="MSP_ERROR_RES_FREE";
		break;
	case MSP_ERROR_RES_MISSING:
		str_cn="��Դȱʧ��";
		str_en="MSP_ERROR_RES_MISSING";
		break;
	case MSP_ERROR_RES_INVALID_NAME:
		str_cn="��Դ��Ч�����ƣ�";
		str_en="MSP_ERROR_RES_INVALID_NAME";
		break;
	case MSP_ERROR_RES_INVALID_ID:
		str_cn="��Դ��Ч��ID��";
		str_en="MSP_ERROR_RES_INVALID_ID";
		break;
	case MSP_ERROR_RES_INVALID_IMG:
		str_cn="��Դ��Ч��ӳ��";
		str_en="MSP_ERROR_RES_INVALID_IMG";
		break;
	case MSP_ERROR_RES_WRITE:
		str_cn="��Դд��������";
		str_en="MSP_ERROR_RES_WRITE";
		break;
	case MSP_ERROR_RES_LEAK:
		str_cn="��Դй©����";
		str_en="MSP_ERROR_RES_LEAK";
		break;
	case MSP_ERROR_RES_HEAD:
		str_cn="��Դͷ������";
		str_en="MSP_ERROR_RES_HEAD";
		break;
	case MSP_ERROR_RES_DATA:
		str_cn="��Դ���ݴ���";
		str_en="MSP_ERROR_RES_DATA";
		break;
	case MSP_ERROR_RES_SKIP:
		str_cn="��Դ������";
		str_en="MSP_ERROR_RES_SKIP";
		break;
//10600
	case MSP_ERROR_TTS_GENERAL:
		str_cn="�ϳɴ�����룡";
		str_en="MSP_ERROR_TTS_GENERAL";
		break;
	case MSP_ERROR_TTS_TEXTEND:
		str_cn="�ϳ��ı�������";
		str_en="MSP_ERROR_TTS_TEXTEND";
		break;
	case MSP_ERROR_TTS_TEXT_EMPTY:
		str_cn="�ϳ��ı�Ϊ�գ�";
		str_en="MSP_ERROR_TTS_TEXT_EMPTY";
		break;
//10700
	case MSP_ERROR_REC_GENERAL:
		str_cn="ʶ�������룡";
		str_en="MSP_ERROR_REC_GENERAL";
		break;
	case MSP_ERROR_REC_INACTIVE:
		str_cn="ʶ���ڲ���Ծ״̬��";
		str_en="MSP_ERROR_REC_INACTIVE";
		break;
	case MSP_ERROR_REC_GRAMMAR_ERROR:
		str_cn="ʶ���﷨����";
		str_en="MSP_ERROR_REC_GRAMMAR_ERROR";
		break;
	case MSP_ERROR_REC_NO_ACTIVE_GRAMMARS:
		str_cn="ʶ��û�л�Ծ���﷨��";
		str_en="MSP_ERROR_REC_NO_ACTIVE_GRAMMARS";
		break;
	case MSP_ERROR_REC_DUPLICATE_GRAMMAR:
		str_cn="ʶ���﷨�ظ���";
		str_en="MSP_ERROR_REC_DUPLICATE_GRAMMAR";
		break;
	case MSP_ERROR_REC_INVALID_MEDIA_TYPE:
		str_cn="ʶ����Ч��ý�����ͣ�";
		str_en="MSP_ERROR_REC_INVALID_MEDIA_TYPE";
		break;
	case MSP_ERROR_REC_INVALID_LANGUAGE:
		str_cn="ʶ����Ч�����ԣ�";
		str_en="MSP_ERROR_REC_INVALID_LANGUAGE";
		break;
	case MSP_ERROR_REC_URI_NOT_FOUND:
		str_cn="ʶ��û�ж�Ӧ��URI��";
		str_en="MSP_ERROR_REC_URI_NOT_FOUND";
		break;
	case MSP_ERROR_REC_URI_TIMEOUT:
		str_cn="ʶ���ȡURI��ʱ��";
		str_en="MSP_ERROR_REC_URI_TIMEOUT";
		break;
	case MSP_ERROR_REC_URI_FETCH_ERROR:
		str_cn="ʶ���ȡURI����ʱ����";
		str_en="MSP_ERROR_REC_URI_FETCH_ERROR";
		break;
//10800
	case MSP_ERROR_EP_GENERAL:
		str_cn="EPһ�����";
		str_en="MSP_ERROR_EP_GENERAL";
		break;
	case MSP_ERROR_EP_NO_SESSION_NAME:
		str_cn="EP����û�����֣�";
		str_en="MSP_ERROR_EP_NO_SESSION_NAME";
		break;
	case MSP_ERROR_EP_INACTIVE:
		str_cn="EP����Ծ��";
		str_en="MSP_ERROR_EP_INACTIVE";
		break;
	case MSP_ERROR_EP_INITIALIZED:
		str_cn="EP��ʼ������";
		str_en="MSP_ERROR_EP_INITIALIZED";
		break;
//12000
	case MSP_ERROR_HTTP_BASE:
		str_cn="HTTP������룡";
		str_en="MSP_ERROR_HTTP_BASE";
		break;
//13000
	case MSP_ERROR_ISV_NO_USER:
		str_cn="�����û������ڣ�";
		str_en="MSP_ERROR_ISV_NO_USER";
		break;
//14000
	case MSP_ERROR_LUA_BASE:
		str_cn="LUA������룡";
		str_en="MSP_ERROR_LUA_BASE";
		break;
	case MSP_ERROR_LUA_YIELD:
		str_cn="LUA yield����";
		str_en="MSP_ERROR_LUA_YIELD";
		break;
	case MSP_ERROR_LUA_ERRRUN:
		str_cn="LUA���д���";
		str_en="MSP_ERROR_LUA_ERRRUN";
		break;
	case MSP_ERROR_LUA_ERRSYNTAX:
		str_cn="LUA�﷨����";
		str_en="MSP_ERROR_LUA_ERRSYNTAX";
		break;
	case MSP_ERROR_LUA_ERRMEM:
		str_cn="LUA�ڴ����";
		str_en="MSP_ERROR_LUA_ERRMEM";
		break;
	case MSP_ERROR_LUA_ERRERR:
		str_cn="LUA����";
		str_en="MSP_ERROR_LUA_ERRERR";
		break;
//1000
	case 1000:
		str_cn="BATREC���ļ�ʧ�ܣ�";
		str_en="BATREC_ERROR_OPEN_FILE";
		break;
	case 1001:
		str_cn="LALRָ�곬��ָ��ֵ��";
		str_en="BATREC_ERROR_LALR_TOO_LONG";
		break;
	case 1002:
		str_cn="����ԱȲ�ƥ�䣡";
		str_en="BATREC_ERROR_COMPARE_RESULT_NOT_MATCH";
		break;
	case 1003:
		str_cn="������ȶԱȲ�ƥ�䣡";
		str_en="BATREC_ERROR_COMPARE_RESULT_LEN_NOT_MATCH";
		break;
	case 1004:
		str_cn="������ȶԱ�ʱ���ű��еĶԱȷ��Ŵ���";
		str_en="BATREC_ERROR_COMPARE_OPERATOR_ERROR";
		break;

	default:
		char buffer[20];
		sprintf(buffer,"�����룺%d",error_code);
		str_cn=buffer;
		sprintf(buffer,"errorcode��%d",error_code);
		str_en=buffer;
		break;
	}
	if(error_code>MSP_ERROR_HTTP_BASE && error_code<MSP_ERROR_ISV_NO_USER)
	{
		char buffer[20];
		sprintf(buffer,"HTTP %d ����",error_code);
		str_cn=buffer;
		sprintf(buffer,"HTTP %d error",error_code);
		str_en=buffer;
	}
	return true;
}
#endif