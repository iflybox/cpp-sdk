// dstester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <locale.h>
#include "dstester_opts.h"
#include "thr_pool.h"
#include "thr_mngr.h"
#include "dum_svc.h"
#include "dstester_log.h"
#include "iflybox_svc.h"

#ifdef WIN32
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // CRTDBG

void print_help();

// 执行测试函数
template <class admin_thr, class cfg_man>
int run_test(admin_thr & ta, const cfg_man & cm)
{
	unsigned int tick = dstester_thr<>::tick_count();

	// 调用了thr_mngr.h的open
	int ret = ta.open(cm.root_dir().c_str(), cm.libs().c_str(), cm.loops_count(), cm.threads_count(),
		true, !cm.no_pool(), true, cm.list_file().c_str());
	if ( ret != 0 )
		return ret;
	ta.wait();
	ta.close(true);

	double secs = (dstester_thr<>::tick_count() - tick) / 1000.;
	proc_resp_mngr().add_msec("TotalTime(sec)", secs);

	printf("%sUsing thread pool threads %d, loops %d, cost %6.3f sec (%s).\n",
		cm.no_pool() ? "NOT " : " ", cm.threads_count(), cm.loops_count(), secs, sp::sec_to_dhms(secs).c_str());
	return 0;
}

// 根据测试主题执行相应的测试
#define IDSS_BEGIN()									\
	if ( sp::strcasecmp(scp_sub, "dummy") == 0 )		\
{													\
	dsdum_mngr ta_dum;								\
	dslog_crit("\nTest dummy begin...");			\
	ret = run_test(ta_dum, cfg_mngr());				\
}
#define IDSS_RUNTEST(sub)								\
	else if ( sp::strcasecmp(scp_sub, #sub) == 0 )		\
{													\
	sub##_mngr ta_##sub;						\
	dslog_crit("\nTest " #sub " begin...");			\
	ret = run_test(ta_##sub, cfg_mngr());			\
}
#define IDSS_END()										\
	else												\
	{													\
		dslog_error_assert(false,						\
		("iflydss | cannot find the implement of scp file %s, subject %s.",	\
		cfg_mngr().scp_file().c_str(), scp_sub.c_str()));				\
		ret = -1;							\
	}

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	//print_help();

#ifdef WIN32
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef WIN32
	std::locale::global(std::locale(""));
	_setmaxstdio(2048);
#else // LNX
	::setlocale(LC_ALL, "zh_CN.UTF-8");
#endif // WIN32
	
	dstester_opts opts;
	std::wstring msg;
	bool quit = false;
	int ret = opts.parse(argc, argv, msg, quit);
	std::wcout << msg;
	if ( quit )
		return ret;

#ifdef WIN32
	if ( !opts.locale().empty() )
		std::locale::global(std::locale(opts.locale().c_str()));
#else // LNX
	::setlocale(LC_ALL, opts.locale().c_str());
#endif // WIN32

	// yanxu2 2011-04-09
	const char* lvl = cfg_mngr().log_lvl().c_str();
	int log_lvl = 0;
	if (!strcmp(lvl,"all"))
		log_lvl = lgl_all;
	else if (!strcmp(lvl,"detail"))
		log_lvl = lgl_detail;
	else if (!strcmp(lvl,"normal"))
		log_lvl = lgl_normal;
	else if (!strcmp(lvl,"low"))
		log_lvl = lgl_low;
	else if (!strcmp(lvl,"none"))
		log_lvl = lgl_none;
	else
		log_lvl = opts.log_level();

	// Open logger
	dslog_cfg srcfg(cfg_mngr().dstester_log().c_str(), "MT Recognizer Running Logging");
	// srcfg.level(opts.log_level());
	srcfg.level(log_lvl);
	srcfg.output(lgo_console | lgo_file);
	srcfg.style(lgs_long);
	srcfg.synch_time(true);
	dslog_open_ac(
		srcfg,
		opts.cfg_file().c_str(), "run_logger");

	// performance logging
	prflog_cfg pfcfg(cfg_mngr().dsperf_log().c_str(), "MT Recognizer Performance Logging");
	pfcfg.style(lgs_crlf);
	pfcfg.level(lgl_all);
	pfcfg.has_head(false);
	pfcfg.synch_time(true);
	prflog_open_ac(
		pfcfg,
		opts.cfg_file().c_str(), "perf_logger");

	// response time logging
	rsplog_cfg rspcfg(cfg_mngr().dsresp_log().c_str(), "MT Recognizer Response Time Logging");
	rspcfg.style(lgs_crlf);
	rspcfg.level(lgl_all);
	rspcfg.has_head(false);
	rsplog_open_ac(
		rspcfg,
		opts.cfg_file().c_str(), "resp_logger");

	// test begin
	dslog_crit("Create thread manager...");

	// 根据脚本文件的主题确定实现
	std::string scp_sub = cfg_mngr().subject();
	std::string s = cfg_mngr().scp_file();
	bool b = cfg_mngr().scp_file().empty();
	if ( !cfg_mngr().scp_file().empty() )
		scp_sub = sc::get_scp_subject(cfg_mngr().scp_file().c_str());
	
#ifdef WIN32
	SP_ASSERT(_CrtCheckMemory());
#endif

	// 结尾不需要分号
	IDSS_BEGIN()
		IDSS_RUNTEST(iflybox)
	IDSS_END()

#ifdef WIN32
	SP_ASSERT(_CrtCheckMemory());
#endif

	// report function response data
	std::string resp_rpt;
	proc_resp_mngr().make_report(resp_rpt, "  Function response time statistics:");
	dslog_crit("\n%s", resp_rpt.c_str());
	std::string dist_rpt;
	proc_resp_mngr().make_distrib_report(dist_rpt, "  Function response time distributions:");

	// yanxu2 2011-10-25 命中率
	std::string scp_s = scp_sub;
	sp::trim_str(scp_s);
	if (scp_s == "simugen")
	{
		std::string hit_rpt;
		proc_resp_mngr().make_hit_report(hit_rpt, "  Function hit percent:");
		dslog_crit("\n%s", hit_rpt.c_str());
	}

	// yanxu2 add 2011-08-04
	// 接口调用时间分布
	std::string prftime_rpt;
	proc_resp_mngr().make_prftime_report(prftime_rpt, "  Function response time distributions:");

	// keep report output
	if ( ( dslog_inst->config().output() & lgo_console ) == 0 )
		printf("%s\n", resp_rpt.c_str());

	rsplog_crit("%sUsing thread pool threads %d, loops %d.",
		cfg_mngr().no_pool() ? "NOT " : " ", cfg_mngr().threads_count(), cfg_mngr().loops_count());
	rsplog_crit("\n%s", resp_rpt.c_str());
	rsplog_crit("\n%s", dist_rpt.c_str());
	rsplog_crit("\n%s", prftime_rpt.c_str());// 接口调用时间分布

	dslog_crit("Test complete.");

#ifdef WIN32
	if ( !cfg_mngr().no_pause() )
		system("pause");
#endif // WIN32

	// error report log
	errlog_close();

#ifdef WIN32
	// _CrtDumpMemoryLeaks();
#endif

	return ret;
}

void print_help()
{
	printf("Usage: dstester command [command_options]\n");
	printf("comamnds and options:\n");
	printf("\tcreatefile\n");
	printf("\t\t-r, --random\t\t:create file with random name\n");
	printf("\t\t-t, --thrnum num\t:create num thread to create file\n");
	printf("\t\t-s, --size size\t\t:size of created file\n");
	printf("\t\t-n, --namelog logf\t:log file names into file logf\n");
	printf("\treadfile\n");
	printf("\t\t-t, --thrnum num\t:create num thread to read file\n");
	printf("\t\t-n, --namelog logf\t:logf contains file names to read\n");
	printf("\t\t-d, --dir path\t\t:file to be stored in local directory\n");
	printf("\thelp\n");
	printf("iFly Distributed Storage System MT test tool 1.0 \n");
	printf("Copyright (C) 1999 - 2011 ANHUI USTC iFLYTEK Co,LTD.\n\n");
}



