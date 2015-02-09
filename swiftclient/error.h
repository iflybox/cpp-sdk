#ifndef __CSSP__ERROR_H__
#define __CSSP__ERROR_H__

#define IFLYBOX_SUCCESS         200
#define SERVICE_PREFIX          0
#define IFLYBOX_ERROR_UNKNOW	SERVICE_PREFIX + 1

//����http״̬���ƫ��ͷ(�����������������http״̬��)
#define HTTP_PREFIX             SERVICE_PREFIX + 0
//libcurl����ƫ��ͷ���ض���libcurl�Ŀ������
#define ERROR_PREFIX_LIBCURL    SERVICE_PREFIX + 1000

//�Զ��������ƫ��ͷ
#define ERROR_PREFIX_COMMON				SERVICE_PREFIX + 2000
//curl_easy_initʧ��
#define ERROR_LIBCUR_INIT				ERROR_PREFIX_COMMON + 1
//multipart uploadid������
#define ERROR_UPLOADID_NOTEXIST			ERROR_PREFIX_COMMON + 2
//��������
#define	ERROR_PARAM_WRONG				ERROR_PREFIX_COMMON + 3		
//json����ʧ��
#define ERROR_JSON_PARSEFAILED			ERROR_PREFIX_COMMON + 4
//accessKeyId��ʽ����
#define ERROR_ACCESSKEYID_FORMATERR		ERROR_PREFIX_COMMON + 5
//����������Χ
#define ERROR_OVERRANGE					ERROR_PREFIX_COMMON + 6
//�����ڴ����
#define ERROR_MALLOC_BAD				ERROR_PREFIX_COMMON + 7

#endif // __CSSP__ERROR_H__