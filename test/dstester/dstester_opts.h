/** 
 * @file	dstester_opts.h
 * @brief	讯飞分布式存储系统测试工具命令行解析类
 * 
 *
 *
 * @author	yanxu2
 * @version	1.0
 * @date	2010-12-17
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2010-12-17	<td>yanxu2	<td>Create this file</tr>
 * </table>
 * 
 */

#ifndef __IDSS_OPTS_H__
#define __IDSS_OPTS_H__

#include <fstream>

#ifndef WIN32//linux yanxu2 add 2010-12-29
# include <string.h>
# include <stdlib.h>
# include <netdb.h>
# include <sys/socket.h>
#endif

#include "log/dslog.h"
#include "utils/sutils.h"
#include "dstester_cfg.h"

#pragma warning( push )
#pragma warning( disable : 4819 )
#include "boost/program_options.hpp"
#pragma warning( pop )

namespace mr
{
	namespace po = boost::program_options;
}

/** 
 * @class	mrec_opts
 * @brief	命令行解析类
 * @author	jdyu
 * @date	2008/9/11
 */
class dstester_opts
{
public:	// list all options here
	dstester_opts(void)
		: log_lvl_			(lgl_normal)
		, log_file_			("dstester.log")
		, threads_			(1)
		, loops_			(1)
		, dir_				(".")
		, no_pool_			(false)
		, no_wait_			(false)
		, cfg_file_			("dstester.cfg")
		, scp_file_			("iflydss.scp")
		, thr_mode_			(thr_mode_single_file)
		, file_mode_		(false)
		, scp_mode_		    ("file")
		, run_mode_			("1")
		#ifndef WIN32
		, locale_			("zh_CN.UTF-8")
		#endif // Linux
		{
			using namespace boost::program_options;
		open_desc_.add_options()
			("cfg,f", value<std::string>(&cfg_file_),
				  "iflydss configuration file path")
			("scp,s", value<std::string>(&scp_file_),
				  "default script file")
			("subject", value<std::string>(&default_sub_),
				  "default subject to test")
			("threads,t", value<int>(&threads_),
				  "max threads count")
			("loops,c", value<int>(&loops_),
				  "all loops count")
			("dir,d", value<std::string>(&dir_),
				  "test root directory")
			("list,l", value<std::string>(&list_file_),
				  "test file list")
		    ("file-mode,m",
				  "to test in single wave file mode")
			("libs,b", value<std::string>(&libs_),
				  "api library files, \",\" to multi-libs")
			("server,v", value<std::string>(&server_addr_),
				"server address ip:port or host:port")
			("proxy-addr,x", value<std::string>(&proxy_addr_),
				"connect proxy address ip:port or host:port")
			("log-file", value<std::string>(&log_file_),
				  "iflydss logger path")
			("log-level", value<int>(&log_lvl_),
				  "iflydss logger level")
			("nowait",
				  "no wait mode, default is false")
			("thread-mode", value<int>(&thr_mode_),
				  "thread mode, default is 0")
			("locale", value<std::string>(&locale_),
				  "iflydss locale setting")
			("label", value<std::string>(&label_),
				  "the label string")
			("scp-mode,s", value<std::string>(&scp_mode_),
				  "default script file load mode")
			("user-id", value<std::string>(&uid_),
				  "user id")
			("keep-alive", value<int>(&keepalive_),
				  "keepalive")
			("run-mode", value<std::string>(&run_mode_),
				  "run mode")
			("help,h", "produce this help message")
			;

		// static options_description hidden_desc;
		entire_desc_.add(open_desc_);
		entire_desc_.add_options()
			("nopool",
				"not using thread pool, default is false.")
			("bat-file-list",
				value<std::vector<std::string> >(&bat_files_), "batch file list")
			;

		positon_desc_.add("bat-file-list", -1);
		}

public:
	// common settings
	size_t bat_files_count(void) const
		{
		return bat_files_.size();
		}
	std::string & bat_file_at(int idx)
		{
		return bat_files_[idx];
		}
	sp::str_arr & bat_files(int idx)
		{
		return bat_files_;
		}
	const std::string & cfg_file(void) const
		{
		return cfg_file_;
		}
	int log_level(void) const
		{
		return log_lvl_;
		}

	const std::string & root_dir(void) const
		{
		return dir_;
		}
	int loops_count(void) const
		{
		return loops_;
		}
	int threads_count(void) const
		{
		return threads_;
		}
	bool no_pool(void) const
		{
		return no_pool_;
		}
	bool no_wait(void) const
		{
		return no_wait_;
		}
	bool file_mode(void) const
	{
		return file_mode_;
	}
	const std::string & lib(void) const
		{
		return libs_;
		}
	const std::string & locale(void) const
		{
		return locale_;
		}
	const std::string & subject(void) const
		{
		return default_sub_;
		}

public:
#ifdef WIN32
	int parse(int argc, wchar_t ** argv, std::wstring & msg, bool & quit)
#else
	int parse(int argc, char ** argv,    std::wstring & msg, bool & quit)
#endif
	{
		using namespace boost::program_options;
		int ret = 0;
		std::ostringstream oss;
		try 
		{
			variables_map vm;
#ifdef WIN32
			store(wcommand_line_parser(argc, argv)
				.options(entire_desc_)
				.positional(positon_desc_)
				.run(), vm);
			store(parse_config_file(
				std::ifstream(L".mtrc"), entire_desc_), vm);
			notify(vm);
#elif !defined(__sun)
			store(command_line_parser(argc, argv)
				.options(entire_desc_)
				.positional(positon_desc_)
				.run(), vm);
			std::ifstream fs(".mtrc");
			store(parse_config_file(fs, entire_desc_), vm);
			notify(vm);
#endif

			if ( vm.count("help") )
			{
				oss << "Multi-Threads Recognize Tool. Version ";
				oss << sp::get_file_version(sp::get_bin_pathname().c_str()) << ".\n";
				oss << "Copyright (C) 1999 - 2009 ANHUI USTC iFLYTEK Co,LTD.\n\n";
				oss << "Usage: \niflydss [options] recfile ...\n";
				oss << open_desc_;
				quit = true;
			}

			// 设置处理文件列表
			if ( !bat_files_.empty() )
				 cfg_mngr().bat_files() = bat_files_;

			// 读取配置文件
			int ret = cfg_mngr().open(cfg_file_.c_str());
			dslog_error_assert(ret==0,
				("cfg_mngr.open failed, ret = %d.", ret));

			// 用选项覆盖配置文件里面的内容
			if ( vm.count("threads") )
				cfg_mngr().threads_count() = threads_;
			if ( vm.count("loops") )
				cfg_mngr().loops_count() = loops_;
			if ( vm.count("dir") )
				cfg_mngr().root_dir() = dir_;
			if ( vm.count("list") )
				cfg_mngr().list_file() = list_file_;
			if ( vm.count("nopool") )
				cfg_mngr().no_pool() = no_pool_ = true;
			if ( vm.count("nowait") )
				cfg_mngr().no_pause() = no_wait_ = true;
			if ( vm.count("file-mode") )
				cfg_mngr().file_mode() = file_mode_ = true;
			if ( vm.count("libs") )
				cfg_mngr().libs() = libs_;
			if ( vm.count("server") )
				cfg_mngr().server_addr() = server_addr_;
			if ( vm.count("scp") )
				cfg_mngr().scp_file() = scp_file_;
			if ( vm.count("subject") )
				cfg_mngr().subject() = default_sub_;
			if ( vm.count("log-file") )
				cfg_mngr().dstester_log() = log_file_;
			if ( vm.count("label") )
				cfg_mngr().label() = label_;
			if ( vm.count("scp-mode") )
				cfg_mngr().scp_mode() = scp_mode_;
			if ( vm.count("user-id") )
				cfg_mngr().user_id() = uid_;
			if ( vm.count("keep-alive") )
				cfg_mngr().keep_alive() = keepalive_;
			if ( vm.count("proxy-addr") )
				cfg_mngr().proxy_addr() = proxy_addr_;
			if ( vm.count("run-mode") )
				cfg_mngr().run_mode() = run_mode_;

		}
		catch (std::exception& e)
		{
			oss << e.what() << std::endl;
			quit = true;
			ret = 1;
		}

		msg = boost::from_local_8_bit(oss.str());
		return 0;
		}

private:
	mr::po::options_description				open_desc_;
	mr::po::options_description				entire_desc_;
	mr::po::positional_options_description	positon_desc_;

	// batch files
	std::vector<std::string>	bat_files_;

	// log setting
	int							log_lvl_;
	std::string					log_file_;

	// common
	std::string					cfg_file_;
	std::string					scp_file_;
	std::string					dir_;
	std::string					libs_;
	std::string					server_addr_;
	std::string					list_file_;
	std::string					locale_;
	std::string					default_sub_;
	std::string					label_;
	// yanxu2 2011-01-05
	std::string					scp_mode_;

	int							threads_;
	int							loops_;
	int							thr_mode_;
	bool						no_pool_;
	bool						no_wait_;
	bool						file_mode_;

	// yanxu2 2011-04-29
	std::string					uid_;
	int							keepalive_;
	std::string					proxy_addr_;

	// yanxu2 2011-07-07
	std::string					run_mode_;
};

#endif /* __IDSS_OPTS_H__ */

