/** 
 * @file	thr_mngr.h
 * @brief	管理员线程
 * 
 *  所有测试线程的管理员，这个管理员调用服务的具体抽象：
 *  服务进程级全局实现svc_mngr、线程级服务实例实现svc_impl和测试单元实现run_unit。
 *  这三个单元组成了一个多线程测试的全部角色。
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2007-7-17
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2007-7-17	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __THR_MNGR_H__
#define __THR_MNGR_H__

#include <deque>
#include <fstream>
#include <signal.h>

#ifndef WIN32//linux yanxu2 add 2010-12-29
# include <string.h>
# include <stdlib.h>
#endif

#include "utils/sutils.h"
#include "thr_pool.h"
#include "proc_resp.h"
#include "log/dslog.h"

// 测试单元的过滤器类型
enum ru_filter_type
	{
	ru_filter_all,		// 所有文件
	ru_filter_scp,		// 仅脚本文件
	ru_filter_text,		// 仅文本文件
	ru_filter_wave		// 仅wave文件
	};

/** 
 * @class	run_unit
 * 
 * @brief	一个测试单元
 * 
 *  对应每一次测试的信息体
 * 
 * @author	jdyu
 * @date	2007-7-17
 */
template <unsigned int filter_t = ru_filter_all, class char_t = char>
class run_unit_t
	{
public:
	run_unit_t(const char * uri = 0)
		{
		if ( match_unit(uri) )
			{
			filter_t == ru_filter_scp
				? sp::strcopy(scp_file_,  uri)
				: sp::strcopy(test_file_, uri);
			}
		}
	operator bool(void) const
		{
		return filter_t == ru_filter_scp
			? !test_file_.empty() || !scp_file_.empty()
			: !test_file_.empty();
		}

	const char_t * test_file(void) const
		{
		return test_file_.c_str();
		}
	const char_t * scp_file(void) const
		{
		return scp_file_.c_str();
		}
	void scp_file(const char_t * scp)
		{
		if ( scp )
			scp_file_ = scp;
		}

public:
	// 获得过滤器类型
	unsigned int filter_type(void) const
		{
		return filter_t;
		}

	// 预处理函数
	int prepare(void)
		{
		return 0;
		}

protected:
	// 匹配一个单元
	bool match_unit(const char * uri)
		{
		if ( uri == 0 )
			return false;
		if ( filter_t == ru_filter_all )
			return true;

		// 扩展名，可以多个
		const char * filter_str[][8] =
			{
			{ 0, },
			{ "scp", "mrcp" },
			{ "txt" },
			{ "pcm", "wav", "alaw", "ulaw" }
			};
		const int filter_count[] =
			{
			0,
			2,
			1,
			4
			};

		std::string ext = sp::pathname_to_extname(uri);
		if ( filter_t < SP_COUNTOF(filter_str) )
			{
			return sp::search_strarr(ext.c_str(),
				filter_str[filter_t], filter_count[filter_t], false) >= 0;
			}

		return false;
		}

protected:
	// string type
	typedef std::basic_string<char_t>	string;
	// 测试单元携带的信息
	string		test_file_;
	string		scp_file_;
	};

// 宽窄字符的版本
typedef run_unit_t<ru_filter_all,  char>	run_unit;
typedef run_unit_t<ru_filter_all,  wchar_t>	wrun_unit;
typedef run_unit_t<ru_filter_scp,  char>	run_unit_scp;
typedef run_unit_t<ru_filter_scp,  wchar_t>	wrun_unit_scp;
typedef run_unit_t<ru_filter_text, char>	run_unit_txt;
typedef run_unit_t<ru_filter_text, wchar_t>	wrun_unit_txt;
typedef run_unit_t<ru_filter_wave, char>	run_unit_wav;
typedef run_unit_t<ru_filter_wave, wchar_t>	wrun_unit_wav;

/** 
 * @class	thr_svc
 * 
 * @brief	一个测试线程
 * 
 *  一个测试线程的过程
 *  这个线程假设用户使用的是脚本解析过程
 * 
 * @author	jdyu
 * @date	2007-7-17
 * 
 * @see		run_unit
 */
template <class svc_impl, class svc_context = thr_ctx_n>
class thr_svc
	: public dstester_thr<svc_context>
	{
public:
	template <class ru_impl>
	void set_run_unit(const ru_impl & ru, unsigned int id)
		{
		svc_impl_.sink().set_run_unit(ru, id, svc_sn_++);
		}

	thr_svc(void)
		: svc_sn_	(0)
		{
		svc_impl_.sink().bat_mode(true);
		}

	unsigned int svc_sn(void) const
		{
		return svc_sn_;
		}

protected:
	// thread implement
	typedef dstester_thr<svc_context> thr_impl;

	// 线程执行函数
	virtual int svc(void)
		{
		// run batch command
		svc_impl_.parse_file(svc_impl_.sink().get_run_unit().scp_file());
		svc_impl_.batch_exec();
		return 0;
		}

	virtual int stop_thr(void)
		{
		svc_impl_.batch_stop();
		thr_impl::stop_thr();
		return 0;
		}

protected:
	// service implement
	svc_impl	svc_impl_;
	// 当前服务的流水号
	unsigned int svc_sn_;
	};

/** 
 * @class	bind_unit
 * 
 * @brief	把服务和unit绑定在一起
 * 
 *  增加与测试单元的绑定，在测试服务中可以获取本次测试的信息
 *  注意：在服务里测试单元是const的，不允许服务单元修改测试单元
 * 
 * @author	jdyu
 * @date	2007-7-19
 * 
 * @see		run_unit thr_svc
 */
template <class unit_impl>
class svc_bind_unit
	{
public:
	void set_run_unit(const unit_impl & ru, unsigned int tid, unsigned int trd)
		{
		run_unit_	= &ru;
		test_id_	= tid;
		test_round_	= trd;
		}
	const unit_impl & get_run_unit(void) const
		{
		return *run_unit_;
		}
	unsigned int test_id(void) const
		{
		return test_id_;
		}
	unsigned int test_round(void) const
		{
		return test_round_;
		}

protected:
	svc_bind_unit(void)
		: run_unit_	(0)
		, test_id_	(0)
		, test_round_(0)
		{
		}

private:
	// 实际的测试单元
	const unit_impl *	run_unit_;
	// 当前的测试ID，全局递增
	unsigned int		test_id_;
	// 当前单元的测试数量
	unsigned int		test_round_;
	};

/** 
 * @class	cfg_mngr_null
 * @brief	一个空的配置类
 * @author	jdyu
 * @date	2008/12/23
 */
class cfg_mngr_null
	{
public:
	static cfg_mngr_null & instance(void)
		{
		static cfg_mngr_null the_cm;
		return the_cm;
		}
	const std::string & label(void) const
		{
		static const std::string empty;
		return empty;
		}
	};

/** 
 * @class	svc_sink_impl
 * @brief	缺省实现了获取流水号的功能
 * 
 *  流水号是解析器需要的功能，建议该类从svc_bind_unit派生
 * 
 * @author	jdyu
 * @date	2008/11/26
 */
template <class svc_impl_t, class cfg_mngr_t = cfg_mngr_null>
class svc_sink_impl
	: public svc_impl_t
	{
public:
	// 获得当前的测试ID
	virtual unsigned int svc_id(void) const
		{
		return test_id();
		}
	virtual unsigned int svc_round(void) const
		{
		return test_round();
		}
	virtual const std::string & label(void) const
		{
		return cfg_mngr_t::instance().label();
		}
	};

/** 
 * @class	pos_mngr_t
 * @brief	向后移动游标的类
 * @author	jdyu
 * @date	2009/4/16
 */
template <typename mutex_t>
class pos_mngr_t
	{
protected:
	// guard_t
	struct guard_t
		{
		guard_t(mutex_t & lock)
			{
			lock_ = &lock;
			lock.acquire();
			}
		~guard_t(void)
			{
			if ( lock_ )
				lock_->release();
			}
		mutex_t * lock_;
		};

	// next item
	template <class POS, class CNT>
	const std::string & next_item(POS & pos, const CNT & con, const char * filter, const std::string & root)
		{
		guard_t mon(pos_lock_);

		size_t old_pos = pos;
		if ( filter && *filter )
			{
			std::string flt = sp::is_abs_path(filter)
				? sp::normalize_path(filter)
				: sp::cat_path(root.c_str(), filter);
			for ( size_t i = 0; i < con.size(); i++ )
				{
				const std::string & item = next_item(pos, con);
				if ( sp::match_string(flt.c_str(), item.c_str()) )
					return item;
				}
			pos = old_pos;
			}
		return next_item(pos, con);
		}
	template <class POS, class CNT>
	const std::string & next_item(POS & pos, const CNT & con)
		{
		static const std::string estr("");
		if ( con.empty() )
			return estr;
		if ( pos >= (u_int) con.size() )
			pos = 0;
		return con[pos++];
		}

protected:
	mutex_t		pos_lock_;
	};

/** 
 * @class	svc_mngr
 * 
 * @brief	全局的服务实现库
 * 
 *  以单体模式实现，方便其他地方的调用
 * 
 * @author	jdyu
 * @date	2007-7-18
 * 
 * @see		
 */
template <class svc_mngr_impl, class char_t = char>
class svc_mngr
	{
protected:
	// 字符串数组类型
	typedef std::vector<std::basic_string<char_t> > str_arr;

	// 派生类必须实现的函数
	virtual int on_initialize  (const str_arr & libs)
		{ return 0; }
	virtual int on_uninitialize(void)
		{ return 0; }
	
public:
	// 返回缺省脚本文件名
	virtual const char_t * default_scp(void) const
		{ return 0; }

	// 是否目录模式
	virtual bool dir_mode(void) const
		{ return false; }
	
	// 如果没有获取到执行单元是否还能继续
	virtual bool has_units(void) const
		{ return false; }

public:
	// Constructor
	svc_mngr(void)
		: start_tick_	(0)
		, total_caps_	(0.)
		, total_svcs_	(0)
		{
		}

	// 初始化
	int initialize(const char * lib)
		{
		str_arr libs;
		if ( lib != 0 )
			{
			const char_t split[] = { ';', ',', '\0' };
			sp::split_str(lib, libs, split, true, true, true);
			}
		int ret = on_initialize(libs);
		// 开始计时
		start_tick_ = dstester_thr<>::tick_count();
		return ret;
		}
	// 逆初始化
	int uninitialize(void)
		{
		total_caps_ = calc_real_caps(total_svcs_);
		int ret = on_uninitialize();
		return ret;
		}

	// 获得所有测试单元
	template <class ru_impl>
	int get_all_unit(const char * dir, std::vector<ru_impl> & rua, const char * listf)
		{
		typedef std::deque<std::string> strdeq;

		strdeq drs;
		if ( listf != 0 && *listf != 0 )
			{
			int ret = read_list_file(listf, drs, dir);
			if ( ret != 0 )
				{
				dslog_error_assert(false, ("read_list_file cannot get files in list file %s.", listf));
				return ret;
				}
			dslog_crit("svc_mngr::get_all_unit using list file to get all units.");
			}
		else
			{
			int ret = dir_mode() ? sp::get_dir_list(dir, drs) : sp::get_file_list(dir, drs);
			if ( ret != 0 )
				{
				dslog_error_assert(false, ("get_all_unit cannot get dir list in dir %s.", dir));
				return ret;
				}
			}

		rua.reserve(drs.size());
		typename strdeq::iterator pi = drs.begin();
		for ( ; pi != drs.end(); pi++ )
			{
			ru_impl ru(pi->c_str());
			if ( default_scp() )
				{
				if ( ru.scp_file() == 0 || *ru.scp_file() == 0 )
					ru.scp_file(default_scp());
				}

			if ( ru )
				rua.push_back(ru);
			}
		dslog_crit("svc_mngr::get_all_unit done, all %d units.", rua.size());
		return 0;
		}

	template <class cont_t>
	int read_list_file(const char * listf, cont_t & files, const char * root = 0)
		{
		std::ifstream stm(listf);
		if ( stm.fail() )
			return -1;

		char line[1024];
		while ( !stm.eof() )
			{
			stm.getline(line, sizeof line);
			if ( sp::trim_str(line) <= 0 )
				continue ;
			if ( line[0] == '#'
				|| line[0] == ';'
				|| ( line[0] == '/' && line[1] == '/' )
				)
				continue ;

			if ( sp::is_abs_path(line) || root == 0 )
				files.push_back(line);
			else if ( root != 0 )
				files.push_back(sp::cat_path(root, line));
			}

		return 0;
		}
	// 线程开始的通知
	unsigned int on_svc_begin(void)
		{
		boost::mutex::scoped_lock lock(lock_);
		return ++total_svcs_;
		}
	double caps_global(void) const
		{
		return total_caps_;
		}

	// 单体实现
	static svc_mngr_impl & instance(void)
		{
		static svc_mngr_impl the_svc_mngr;
		return the_svc_mngr;
		}
	static str_arr & parse_lib(const char_t * lib, str_arr & libs)
		{
		return libs;
		}

	// 内部使用的函数
protected:
	// 计算实际caps
	double calc_real_caps(size_t count) const
		{
		return count * 1000. / (dstester_thr<>::tick_count() - start_tick_);
		}
	// CAPS的限制
	template <typename count_t>
	void on_caps_begin(count_t & count, unsigned int caps_limit)
		{
		// add count
			{
			boost::mutex::scoped_lock lock(lock_);
			count ++;
			}
		while ( caps_limit > 0 )
			{
			double real_caps = calc_real_caps(count);
			if ( real_caps - caps_limit < 1e-5 )
				break;
			dstester_thr<>::sleep(1);
			}
		}
	template <typename count_t>
	void on_caps_end(count_t & count)
		{
		boost::mutex::scoped_lock lock(lock_);
		count --;
		}

private:
	// 开始的时间
	unsigned int	start_tick_;
	// 计算的实际CAPS
	double			total_caps_;
	unsigned int	total_svcs_;
	// 同步锁定
	boost::mutex	lock_;
	};

/** 
 * @class	thr_admin
 * 
 * @brief	管理员线程
 * 
 *  负责管理所有测试线程的启动、停止和统计
 *  和主流程的执行。
 * 
 * @author	jdyu
 * @date	2007-7-17
 * 
 * @see		thr_pool
 */
template<class svc_mngr, class svc_impl, class run_unit, class svc_ctx = thr_ctx_n>
class thr_admin
	: public thr_sink_t	<dstester_thr<svc_ctx> >
	{
public:
	// service implement
	typedef dstester_thr<svc_ctx>				thr_impl;
	typedef thr_sink_t	<dstester_thr<svc_ctx> >	thr_sink_impl;

	/** 
	 * @brief 	open
	 *  
	 *   初始化
	 *  
	 * @author	jdyu
	 * @date	2007-7-18
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char * root	- [in] 测试的根目录
	 * @param	int loop_count		- [in] 总的循环数量
	 * @param	int thr_count		- [in] 并发线程数量
	 * @param	bool start = true	- [in] 是否启动所有线程
	 * @param	bool thpool = true	- [in] 是否使用线程池
	 * @param	bool ctrlc_stop = true	- [in] 是否支持Ctrl+C停止
	 * @see		
	 */
	int open(const char * root, const char * libs, int loop_count, int thr_count,
		bool start = true, bool thrpool = true, bool ctrlc_stop = true, const char * listf = 0)
		{
		close();

		// 获得所有待测试单元
		int ret = 0;
		if ( !svc_mngr::instance().has_units() )
			{
			ret = svc_mngr::instance().get_all_unit(root, all_units_, listf);
			if ( ret != 0 )
				{
				dslog_error("thr_admin::open get_all_unit failed, ret %d.", ret);
				return ret;
				}
			}
		else
			{
			// 可以增加一个配置文件配置的unit进去
			run_unit ru;
			if ( svc_mngr::instance().default_scp() )
				ru.scp_file(svc_mngr::instance().default_scp());
			if ( ru )
				{
				all_units_.push_back(ru);
				dslog_crit("thr_admin | skip getting units from %s.", root);
				}
			}

		if ( all_units_.empty() )
			{
			dslog_error_assert(false,
				("thr_admin | cannot get any test units in dir %s, %s mode, quit!",
					root, svc_mngr::instance().dir_mode() ? "dir" : "file"));
			return 3 /* ERROR_PATH_NOT_FOUND */;
			}

		// 预处理每个单元
		std::for_each(all_units_.begin(), all_units_.end(),
			std::mem_fun_ref(&run_unit::prepare));

		// 初始化线程池
		thr_max_ = thr_count;
		ret = thr_pool_.open(thrpool ? thr_count : 0);
		if ( ret != 0 )
			{
			dslog_error("thr_admin::open init thr_pool failed, ret %d.", ret);
			return ret;
			}

		// 实现库初始化
		dslog_info("Initialize svc library...");
		ret = svc_mngr::instance().initialize(libs);
		if ( ret != 0 )
			{
			dslog_error("thr_admin::open init svc_lib %s failed, ret %d.",
				libs, ret);
			return ret;
			}

		loop_count_ = loop_count;
		/*if ( analyze_mode )
			loop_count_ = (unsigned int)all_units_.size();*/

		svc_arr_.resize(thr_max_, svc_impl());
		svc_sta_.resize(thr_max_, thr_init);

		// 设置信号处理函数
		signal(SIGINT, signal_handler);
		signal(SIGTERM, signal_handler);
		#ifdef WIN32
		SetConsoleCtrlHandler(ctrl_handler, TRUE);
		#endif // WIN32

		// 启动管理员线程
		if ( start )
			this->start();
		init_ = true;

		return ret;
		}

	/** 
	 * @brief 	close
	 *  
	 *   逆初始化
	 *  
	 * @author	jdyu
	 * @date	2007-7-19
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	bool wait_all = false	- [in] 是否等待所有测试线程退出
	 * @see		
	 */
	int close(bool wait_all = false)
		{
		if ( !init_ )
			return 0;

		stop();
		thr_pool_.close();

		dstester_thr<>::sleep(200);
		all_units_.clear();

		// 逆初始化实现库
		int ret = svc_mngr::instance().uninitialize();
		dslog_error_assert(ret == 0,
			("thr_admin::close uninit svc_mngr failed, ret %d.", ret));
		init_ = false;
		return ret;
		}

	int start(void)
		{
		SP_ASSERT(thr_inst_==0);
		thr_inst_ = new boost::thread(thr_proc(this));
		return 0;
		}
	int stop(void)
		{
		if ( thr_inst_ )
			{
			thr_stop_ = true;
			thr_inst_->join();
			delete thr_inst_, thr_inst_ = 0;
			}
		return 0;
		}

	int wait(void)
		{
		admin_cond_.wait();
		return 0;
		}

	thr_admin(void)
		: thr_actv_		(0)
		, loop_count_	(0)
		, thr_max_		(10)
		, thr_free_		(0)
		, thr_inst_		(0)
		, thr_stop_		(false)
		, init_			(false)
		, complete_count_(0)
		{
		thr_pool_.add_sink(this);
		}

protected:
	// 线程执行函数
	virtual int svc(void)
		{
		unsigned int count = 0;
		while ( !thr_stop_ )
			{
			boost::mutex::scoped_lock lock(thr_lock_);
			
			// 是否需要创建线程
			while ( thr_actv_ < thr_max_
				&& count < loop_count_ )
				{
				// 获取空闲线程位置
				int idle_idx = -1;
				if ( svc_sta_[thr_free_] == thr_busy )
					{
					for ( int i = 0; i < (int)svc_arr_.size(); i++ )
						{
						if ( svc_sta_[i] != thr_busy )
							{
							idle_idx = i;
							break ;
							}
						}
					}
				else
					idle_idx = thr_free_;

				// 如果存在空闲的，创建新的
				if ( idle_idx != -1 )
					{
					svc_impl & si = svc_arr_[idle_idx];
					const run_unit & ru = all_units_.at(count % all_units_.size());
					si.set_run_unit(ru, count);

					lock.unlock();
					int ret = thr_pool_.alloc_thr(&si);
					lock.lock();
					dslog_info("thr_admin::create new thread, count %4d of %d.", count, loop_count_);
					
					// yanxu2 2011-04-02
					printf("thr_admin::create new thread, count %4d of %d.\r", count, loop_count_);

					if ( count % 1000 == 0 && ( dslog_inst->config().level() & lgl_info ) == 0 )
						dslog_crit("thr_admin::create new thread, count %4d of %d.", count, loop_count_);
					dslog_error_assert(ret == 0,
						("thr_admin::alloc_thr thread failed, ret = %d.", ret));

					if ( ret == 0 )
						{
						svc_sta_[idle_idx] = thr_busy;
						thr_actv_ ++;
						count ++;
						svc_mngr::instance().on_svc_begin();
						}
					}
				else
					break;

				if ( stopping() )
					thr_stop_ = true;
				if ( thr_stop_ )
					break ;
				}

			if ( stopping() )
				thr_stop_ = true;

			// 等待消息
			boost::xtime xt;
			boost::xtime_get(&xt, boost::TIME_UTC);
			//boost::xtime_get(&xt, boost::xtime_clock_types::TIME_UTC_); // yanxu2
			xt.sec += 1;
			if ( !thr_cond_.timed_wait(lock, xt) )
				{
				if ( count >= loop_count_ && thr_actv_ == 0 )
					break;
				continue ;
				}

			if ( count >= loop_count_ )
				break;
			}

		// 停止未完的线程
		if ( stopping() )
			thr_pool_.stop();

		dslog_info("thr_admin::all %d threads created.", count);
		while ( !stopping() )
			{
				#ifdef WIN32
				if ( complete_cond_.wait(1000) != WAIT_TIMEOUT )
				#else // LINUX
				if ( complete_cond_.wait(1000) != -1 )
				#endif // OS
				break;
			}
		admin_cond_.signal();
		dslog_crit("Administrator thread quitting!");
		return 0;
		}

	virtual void on_svc_begin(thr_impl * svc)
		{
		}

	virtual void on_svc_end(thr_impl * svc)
		{
		boost::mutex::scoped_lock lock(thr_lock_);

		thr_free_ = (unsigned int)(dynamic_cast<svc_impl*>(svc) - &svc_arr_.front());
		dslog_error_assert(thr_free_ < svc_arr_.size(),
			("Fatal: on_svc_end invalid svc 0x%x, index %d.", svc, thr_free_));

		if ( thr_free_ < svc_arr_.size() )
			{
			svc_sta_[thr_free_] = thr_idle;
			thr_actv_ --;
			thr_cond_.notify_one();
			SP_ASSERT(thr_actv_>=0);
			dslog_info("thr_admin svc %2d ended, active thrs %2d.", thr_free_, thr_actv_);
			// yanxu2 2011-04-02
			// printf("thr_admin svc %2d ended, active thrs %2d.\r", thr_free_, thr_actv_);
			}
		if ( ++complete_count_ >= loop_count_ )
			{
			dslog_info("thr_admin all %d tests finish.", complete_count_);
			complete_cond_.signal();
			}
		}

	static void signal_handler(int signal)
		{
		stopping() = true;
		if ( signal == SIGINT )
			dslog_crit("\nCtrl + C to stopping...\n");
		if ( signal == SIGTERM )
			printf("\nSoftware terminate to stopping...\n");
#ifdef WIN32
		if ( signal == SIGBREAK )
			dslog_crit("\nCtrl + Break to stopping...\n");
#endif // WIN32
		}
	static bool & stopping(void)
		{
		static bool to_stop = false;
		return to_stop;
		}

	#ifdef WIN32
	static BOOL WINAPI ctrl_handler(DWORD fdwCtrlType)
		{
		switch (fdwCtrlType) 
			{ 
			// Handle the CTRL+C signal. 
			case CTRL_C_EVENT:
				signal_handler(SIGINT);
				return TRUE; 

			// CTRL+CLOSE: confirm that the user wants to exit. 
			case CTRL_CLOSE_EVENT: 
				dslog_crit("admin | user close the console.");
				signal_handler(SIGTERM);
				Beep(1000, 1000);
				Sleep(1000);
				return TRUE; 

			// Pass other signals to the next handler. 
			case CTRL_BREAK_EVENT:
				signal_handler(SIGBREAK);
				return FALSE;
			case CTRL_LOGOFF_EVENT:
			case CTRL_SHUTDOWN_EVENT:
				stopping() = true;
				dslog_crit("admin | LOGOFF or SHUTDOWN handled.");
			default: 
				return FALSE; 
			}
		return FALSE;
		}
	#endif // WIN32

private:
	// 所有的测试单元
	typedef std::vector<run_unit> ru_arr;
	ru_arr					all_units_;

	// 携带一个线程池
	typedef thr_pool<svc_ctx> thr_pool_impl;
	thr_pool_impl			thr_pool_;

	// 检测线程启动和停止信号
	boost::mutex			thr_lock_;
	// boost::condition		thr_cond_;
	boost::condition_variable_any		thr_cond_;
	thr_event				admin_cond_;
	thr_event				complete_cond_;

	// 活动线程数量
	unsigned int			thr_actv_;
	unsigned int			thr_max_;
	unsigned int			thr_free_;
	unsigned int			loop_count_;
	unsigned int			complete_count_;

	// 线程状态
	enum thr_state
		{
		thr_init,
		thr_idle,
		thr_busy,
		};
	/// 线程实例数组
	typedef std::vector<svc_impl>	svc_arr;
	typedef std::vector<thr_state>	svc_sta;
	svc_arr					svc_arr_;
	svc_sta					svc_sta_;

private:
	// boost thread object
	boost::thread *			thr_inst_;
	// thread object
	struct thr_proc
		{
		thr_proc(thr_admin * ta)
			: ta_(ta) { }
		void operator() (void)
			{ ta_->svc(); }
		thr_admin * ta_;
		};
	// stop flag
	bool					thr_stop_;
	// init_flag
	bool					init_;
	};

// 使用自带解析线程
template<class svc_mngr, class svc_impl, class run_unit, class svc_ctx = thr_ctx_n>
class thr_admin_parser
	: public thr_admin<svc_mngr, thr_svc<svc_impl>, run_unit, svc_ctx>
	{
	};

#endif /* __THR_MNGR_H__ */
