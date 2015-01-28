#ifndef __MSP_CONFIG_H__
#define __MSP_CONFIG_H__

#ifdef WIN32
#include <Windows.h>
#else
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#endif

#include <vector>
#include <iostream>

/*
Tips:
	WINDOWS�£���Ҫʹ�ò�ѯ����IP�ĺ�������ǰ��
		Ӧ�ó�����Ҫ�Լ�����WSAStartup����ʼ��������Դ
	Linux����ʱδ����
*/

namespace msp_config
{

//------------------------------------//
//	��־Ŀ¼						  //
//------------------------------------//
enum
{
	LOG_RUNTIME,
	LOG_CALOG,
	LOG_COLOR,

	LOG_COUNT
};

#ifdef WIN32
static const char *base_log_dir[] = {
	"E:\\log\\",
	"F:\\calog\\",
	"F:\\color\\",
};
#else /* Linux */
static const char *base_log_dir[] = {
	"/log/",
	"/calog/",
	"/color/",
};
#endif

inline const char *get_log_dir(int log_type = LOG_RUNTIME)
{
	if( log_type < 0 || log_type >= LOG_COUNT )
		return NULL;
	
	return base_log_dir[log_type];
}

//------------------------------------//
//	������������					  //
//------------------------------------//
enum
{
	AREA_GUANGZHOU,		// ����
	AREA_BEJING,		// ����
	AREA_HEFEI,			// �Ϸ�
	AREA_SHANGHAI,		// �Ϻ�
	AREA_XIAN,			// ����
	AREA_JAPAN,			// С�ձ�

	AREA_COUNT
};

// �������������б�
static const char *service_area[] = {
	"gz",
	"ch",
	"hf",
	"sh",
	"xa",
	"jp",
};

// �������������б�(����)
static const char *service_area_cn[] = {
	"����",
	"����",
	"�Ϸ�",
	"�Ϻ�",
	"����",
	"�ձ�",
};

// ����IP��
static const char *net_seg_guangzhou[] = {
	"192.168.70.",
	"192.168.60.",
	"192.168.50.",
};

// ����IP��
static const char *net_seg_beijing[] = {
	"192.168.71.",
	"192.168.61.",
	"192.168.51.",
};

// �Ϸ�IP��
static const char *net_seg_hefei[] = {
	"192.168.72.",
	"192.168.62.",
	"192.168.52.",
	"192.168.42.",
};

// �Ϻ�IP��
static const char *net_seg_shanghai[] = {
	""
};

// ����IP��
static const char *net_seg_xian[] = {
	""
};

// С�ձ�IP��
static const char *net_seg_japan[] = {
	""
};

const struct
{
	const char			**net_seg; // IP���׵�ַ
	int					seg_count; // IP�θ���
}
net_seg_list[] = {
	{ net_seg_guangzhou, 3},
	{ net_seg_beijing,	 3},
	{ net_seg_hefei,	 3},
	{ net_seg_shanghai,	 0},
	{ net_seg_xian,		 0},
	{ net_seg_japan,	 0},
};

// ��ȡ����IP�б�
inline int local_ip_list(std::vector<std::string> &ip_list)
{
	char host_name[255];
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR) 
		return -1;


	struct hostent *phe = gethostbyname(host_name);
	if (phe == 0) 
		return -1;

	struct in_addr addr;
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) 
	{
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		ip_list.push_back(inet_ntoa(addr));
	}

	return 0;
}

// ���ݵ�ǰ����IP��ȡ��������ID
inline int current_area_id()
{
	std::vector<std::string> lips;
	int ret = local_ip_list(lips);
	if( ret == -1 )
		return -1;

	for( size_t i = 0; i < lips.size(); ++i )
	{
		for( int j = 0; j < AREA_COUNT; ++j )
		{
			for( int l = 0; l < net_seg_list[j].seg_count; ++l )
			{
				if( strncmp(lips[i].c_str(), net_seg_list[j].net_seg[l], strlen(net_seg_list[j].net_seg[l])) == 0 )
					return j; // find it
			}
		}
	}

	return -1; // not found
}

// ���ݵ�ǰ����IP��ȡ����������
inline const char *current_area_name()
{
	int area_id = current_area_id();
	if( area_id != -1 )
		return service_area[area_id];

	return "";
}

// ���ݵ�ǰ����IP��ȡ����������(����)
inline const char *current_area_name_cn()
{
	int area_id = current_area_id();
	if( area_id != -1 )
		return service_area_cn[area_id];

	return "";
}

// ���ݴ���IP������������ID
inline int get_area_id(const char *ip)
{
	for( int j = 0; j < AREA_COUNT; ++j )
	{
		for( int l = 0; l < net_seg_list[j].seg_count; ++l )
		{
			if( strncmp(ip, net_seg_list[j].net_seg[l], strlen(net_seg_list[j].net_seg[l])) == 0 )
				return j; // find it
		}
	}

	return -1; // not found
}

// ���ݴ�������ID��ȡ������
inline const char *get_area_name(int area_id)
{
	if( area_id < 0 || area_id >= AREA_COUNT )
		return NULL;
	return service_area[area_id];
}

};

#endif /* __MSP_CONFIG_H__ */