#ifndef __MTR_CFG_H__
#define __MTR_CFG_H__

#include "asc_parser.h"
#include "utils/suconf.h"

#define MILLION_LEVEL 1000000
#define HUNDRED_MILLION_LEVEL 100000000
#define MILLION_MODE 16
#define HUNDRED_MILLION_MODE 64

// 识别的线程模式
enum ds_thr_mode
	{
	thr_mode_single_file,
	thr_mode_single_record,
	thr_mode_dual,
	};

const wchar_t * const thr_mode_str[] =
	{
	L"thread_mode_single_file",
	L"thread_mode_single_record",
	L"thread_mode_dual",
	};

// yanxu2 2011-07-07
enum connect_mode
{
	CONNECT_MODE_Undefined = 0,
	CONNECT_OUT_THREAD,
	CONNECT_IN_THREAD,
	CONNECT_MODE_Counter
};

static const struct {
	connect_mode	type_;
	const char*		mode_no_;
} g_connect_mode_info [] ={
	{ CONNECT_MODE_Undefined,	"0"},
	{ CONNECT_OUT_THREAD,	"1"},
	{ CONNECT_IN_THREAD,	"2"},
	{ CONNECT_MODE_Counter,		"3"}
};

static connect_mode connect_mode_type(std::string str)
{
	const char* no = str.c_str();
	for (int i = 0; i < CONNECT_MODE_Counter; ++i)
	{
		if (!sp::strcasecmp(no, g_connect_mode_info[i].mode_no_))
			return g_connect_mode_info[i].type_;
	}
	return CONNECT_MODE_Undefined;
}

/** 
 * @class	dstester_cfg
 * 
 * @brief	配置管理
 * 
 *  负责mtr的配置管理功能
 * 
 * @author	jdyu
 * @date	2007-7-13
 */
class dstester_cfg
	: public sc::asc_singleton<dstester_cfg>
	{
public:
	dstester_cfg(void)
		: root_dir_			(".")
		, threads_			(1)
		, loops_			(1)
		, no_pool_			(false)
		, no_pause_			(false)
		, file_mode_		(true)
		, run_mode_			("1")
		, dstester_log_		("dstester.log")
		, dsperf_log_		("dsperf.log")
		, dsresp_log_		("dsresp.log")
		{
		}
	virtual ~dstester_cfg(void)
		{
		}

	int open(const char * cfg_file)
		{
		cfg_file_ = cfg_file;
		
		// 按配置文件中的[common][run_logger]为单元读取

		// 读取配置信息
		sp::inisec com_sec(cfg_file, "common");
		com_sec.get_str_value	("root_dir",		root_dir_);
		com_sec.get_str_value	("list_file",		list_file_);
		com_sec.get_int_value	("threads_count",	threads_,	threads_,	1, 1000);
		com_sec.get_int_value	("loops_count",		loops_,		loops_,		1, INT_MAX);
		com_sec.get_str_value	("server_addr",		server_addr_);
		com_sec.get_str_value	("svc_libs",		api_libs_);
		com_sec.get_str_value	("scp_file",		scp_file_);
		com_sec.get_str_value	("subject",			default_sub_);
		com_sec.get_str_value	("label",			label_);
		com_sec.get_bool_value	("no_pool",			no_pool_);
		com_sec.get_bool_value	("no_pause",		no_pause_);
		
		com_sec.get_str_value	("scp_mode",		scp_mode_);
		com_sec.get_str_value	("resource_dir",	resource_dir_);
		com_sec.get_bool_value	("file_mode",		file_mode_);
		
		com_sec.get_str_value	("uid",				uid_);
		com_sec.get_int_value	("keepalive",	keepalive_,	keepalive_);
		com_sec.get_str_value	("proxy_addr",		proxy_addr_);
		com_sec.get_str_value	("reserv",		reserv_);
		
		com_sec.get_str_value	("run_mode",		run_mode_);
		//hbasext
		sp::inisec hbs_sec(cfg_file, "hbasext");
		hbs_sec.get_str_value	("zookeeper_host",	zookeeper_host_);
		hbs_sec.get_int_value	("zookeeper_num",	zookeeper_num_);
		hbs_sec.get_str_value	("table_name",		table_name_);
		// loggers
		sp::inisec rl_sec(cfg_file, "run_logger");
		rl_sec.get_str_value	("file",			dstester_log_);
		
		rl_sec.get_str_value	("level",			log_lvl_);

		sp::inisec pl_sec(cfg_file, "perf_logger");
		pl_sec.get_str_value	("file",			dsperf_log_);
		sp::inisec rp_sec(cfg_file, "resp_logger");
		rp_sec.get_str_value	("file",			dsresp_log_);
		
		return 0;
		}

	int close(void)
		{
		return 0;
		}

	// 通用选项
public:
	const std::string & cfg_file(void) const
		{
		return cfg_file_;
		}

	const sp::str_arr & bat_files(void) const
		{
		return bat_files_;
		}
	sp::str_arr & bat_files(void)
		{
		return bat_files_;
		}

	const std::string & root_dir(void) const
		{
		return root_dir_;
		}
	std::string & root_dir(void)
		{
		return root_dir_;
		}

	const std::string & libs(void) const
	{
		return api_libs_;
	}
	std::string & libs(void)
		{
		return api_libs_;
		}
	
	const std::string & zookeeper_host(void) const
	{
		return zookeeper_host_;
	}
	std::string & zookeeper_host(void)
	{
		return zookeeper_host_;
	}

	int zookeeper_num(void) const
	{
		return zookeeper_num_;
	}
	int & zookeeper_num(void)
	{
		return zookeeper_num_;
	}

	const std::string & table_name(void) const
	{
		return table_name_;
	}
	std::string & table_name(void)
	{
		return table_name_;
	}

	const std::string & list_file(void) const
		{
		return list_file_;
		}
	std::string & list_file(void)
		{
		return list_file_;
		}

	int loops_count(void) const
		{
		return loops_;
		}
	int & loops_count(void)
		{
		return loops_;
		}

	int threads_count(void) const
		{
		return threads_;
		}
	int & threads_count(void)
		{
		return threads_;
		}

	bool no_pool(void) const
		{
		return no_pool_;
		}
	bool & no_pool(void)
		{
		return no_pool_;
		}
	bool no_pause(void) const
		{
		return no_pause_;
		}
	bool & no_pause(void)
		{
		return no_pause_;
		}

	const std::string & dstester_log(void) const
		{
		return dstester_log_;
		}
	std::string & dstester_log(void)
		{
		return dstester_log_;
		}
	const std::string & dsperf_log(void) const
	{
		return dsperf_log_;
	}
	std::string & dsperf_log(void)
	{
		return dsperf_log_;
	}
	std::string & dsresp_log(void)
	{
		return dsresp_log_;
	}

	bool file_mode(void) const
		{
		return file_mode_;
		}
	bool & file_mode(void)
		{
		return file_mode_;
		}

	const std::string & server_addr(void) const
		{
		return server_addr_;
		}
	std::string & server_addr(void)
		{
		return server_addr_;
		}

	const std::string & run_mode(void) const
	{
		return run_mode_;
	}
	std::string & run_mode(void)
	{
		return run_mode_;
	}

	const std::string & scp_file(void) const
		{
		return scp_file_;
		}
	std::string & scp_file(void)
		{
		return scp_file_;
		}

	const std::string & scp_mode(void) const
	{
		return scp_mode_;
	}
	std::string & scp_mode(void)
	{
		return scp_mode_;
	}

	const std::string & resource_dir(void) const
	{
		return resource_dir_;
	}
	std::string & resource_dir(void)
	{
		return resource_dir_;
	}

	const std::string & subject(void) const
		{
		return default_sub_;
		}
	std::string & subject(void)
		{
		return default_sub_;
		}

	const std::string & label(void) const
		{
		return label_;
		}
	std::string & label(void)
		{
		return label_;
		}

	const std::string & log_lvl(void) const
	{
		return log_lvl_;
	}
	std::string & log_lvl(void)
	{
		return log_lvl_;
	}

	const std::string & user_id(void) const
	{
		return uid_;
	}
	std::string & user_id(void)
	{
		return uid_;
	}
	int keep_alive(void) const
	{
		return keepalive_;
	}
	int & keep_alive(void)
	{
		return keepalive_;
	}
	const std::string & proxy_addr(void) const
	{
		return proxy_addr_;
	}
	std::string & proxy_addr(void)
	{
		return proxy_addr_;
	}

	const std::string & cfg_reserv(void) const
	{
		return reserv_;
	}
	std::string & cfg_reserv(void)
	{
		return reserv_;
	}

	const std::map<int, std::string> & simugen_files_map(void) const
	{
		return files_map_;
	}
	std::map<int, std::string> & simugen_files_map(void)
	{
		return files_map_;
	}
	
private:
	// 配置文件
	std::string		cfg_file_;

	// 测试根目录
	sp::str_arr		bat_files_;
	std::string		root_dir_;
	std::string		scp_file_;
	std::string		api_libs_;
	std::string		server_addr_;
	std::string		list_file_;
	std::string		default_sub_;
	std::string		label_;
	int				threads_;
	int				loops_;
	bool			no_pool_;
	bool			no_pause_;
	bool			file_mode_;

	std::string		scp_mode_;
	std::string		resource_dir_;

	//hbase
	std::string		zookeeper_host_;
	int				zookeeper_num_;
	std::string		table_name_;

	// log
	std::string		dstester_log_;
	std::string		dsperf_log_;
	std::string		dsresp_log_;

	std::string     log_lvl_;

	std::string     uid_;
	int             keepalive_;
	std::string     proxy_addr_;
	std::string     reserv_;

	std::string		run_mode_;

	std::map<int, std::string> files_map_;

	};

// the configuration manager
#define cfg_mngr()	dstester_cfg::instance()

#endif /* __MTR_CFG_H__ */
