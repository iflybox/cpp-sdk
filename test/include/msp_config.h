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
	WINDOWS下，需要使用查询本地IP的函数操作前，
		应用程序需要自己调用WSAStartup来初始化网络资源
	Linux下暂时未测试
*/

namespace msp_config
{

//------------------------------------//
//	日志目录						  //
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
//	服务区域及网段					  //
//------------------------------------//
enum
{
	AREA_GUANGZHOU,		// 广州
	AREA_BEJING,		// 北京
	AREA_HEFEI,			// 合肥
	AREA_SHANGHAI,		// 上海
	AREA_XIAN,			// 西安
	AREA_JAPAN,			// 小日本

	AREA_COUNT
};

// 服务区域名称列表
static const char *service_area[] = {
	"gz",
	"ch",
	"hf",
	"sh",
	"xa",
	"jp",
};

// 服务区域名称列表(中文)
static const char *service_area_cn[] = {
	"广州",
	"长话",
	"合肥",
	"上海",
	"西安",
	"日本",
};

// 广州IP段
static const char *net_seg_guangzhou[] = {
	"192.168.70.",
	"192.168.60.",
	"192.168.50.",
};

// 北京IP段
static const char *net_seg_beijing[] = {
	"192.168.71.",
	"192.168.61.",
	"192.168.51.",
};

// 合肥IP段
static const char *net_seg_hefei[] = {
	"192.168.72.",
	"192.168.62.",
	"192.168.52.",
	"192.168.42.",
};

// 上海IP段
static const char *net_seg_shanghai[] = {
	""
};

// 西安IP段
static const char *net_seg_xian[] = {
	""
};

// 小日本IP段
static const char *net_seg_japan[] = {
	""
};

const struct
{
	const char			**net_seg; // IP段首地址
	int					seg_count; // IP段个数
}
net_seg_list[] = {
	{ net_seg_guangzhou, 3},
	{ net_seg_beijing,	 3},
	{ net_seg_hefei,	 3},
	{ net_seg_shanghai,	 0},
	{ net_seg_xian,		 0},
	{ net_seg_japan,	 0},
};

// 获取本地IP列表
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

// 根据当前机器IP获取所在区域ID
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

// 根据当前机器IP获取所在区域名
inline const char *current_area_name()
{
	int area_id = current_area_id();
	if( area_id != -1 )
		return service_area[area_id];

	return "";
}

// 根据当前机器IP获取所在区域名(中文)
inline const char *current_area_name_cn()
{
	int area_id = current_area_id();
	if( area_id != -1 )
		return service_area_cn[area_id];

	return "";
}

// 根据传入IP查找所在区域ID
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

// 根据传入区域ID获取区域名
inline const char *get_area_name(int area_id)
{
	if( area_id < 0 || area_id >= AREA_COUNT )
		return NULL;
	return service_area[area_id];
}

};

#endif /* __MSP_CONFIG_H__ */