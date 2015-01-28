/** 
 * @file	thr_mngr.h
 * @brief	����Ա�߳�
 * 
 *  ���в����̵߳Ĺ���Ա���������Ա���÷���ľ������
 *  ������̼�ȫ��ʵ��svc_mngr���̼߳�����ʵ��ʵ��svc_impl�Ͳ��Ե�Ԫʵ��run_unit��
 *  ��������Ԫ�����һ�����̲߳��Ե�ȫ����ɫ��
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

// ���Ե�Ԫ�Ĺ���������
enum ru_filter_type
	{
	ru_filter_all,		// �����ļ�
	ru_filter_scp,		// ���ű��ļ�
	ru_filter_text,		// ���ı��ļ�
	ru_filter_wave		// ��wave�ļ�
	};

/** 
 * @class	run_unit
 * 
 * @brief	һ�����Ե�Ԫ
 * 
 *  ��Ӧÿһ�β��Ե���Ϣ��
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
	// ��ù���������
	unsigned int filter_type(void) const
		{
		return filter_t;
		}

	// Ԥ������
	int prepare(void)
		{
		return 0;
		}

protected:
	// ƥ��һ����Ԫ
	bool match_unit(const char * uri)
		{
		if ( uri == 0 )
			return false;
		if ( filter_t == ru_filter_all )
			return true;

		// ��չ�������Զ��
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
	// ���Ե�ԪЯ������Ϣ
	string		test_file_;
	string		scp_file_;
	};

// ��խ�ַ��İ汾
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
 * @brief	һ�������߳�
 * 
 *  һ�������̵߳Ĺ���
 *  ����̼߳����û�ʹ�õ��ǽű���������
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

	// �߳�ִ�к���
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
	// ��ǰ�������ˮ��
	unsigned int svc_sn_;
	};

/** 
 * @class	bind_unit
 * 
 * @brief	�ѷ����unit����һ��
 * 
 *  ��������Ե�Ԫ�İ󶨣��ڲ��Է����п��Ի�ȡ���β��Ե���Ϣ
 *  ע�⣺�ڷ�������Ե�Ԫ��const�ģ����������Ԫ�޸Ĳ��Ե�Ԫ
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
	// ʵ�ʵĲ��Ե�Ԫ
	const unit_impl *	run_unit_;
	// ��ǰ�Ĳ���ID��ȫ�ֵ���
	unsigned int		test_id_;
	// ��ǰ��Ԫ�Ĳ�������
	unsigned int		test_round_;
	};

/** 
 * @class	cfg_mngr_null
 * @brief	һ���յ�������
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
 * @brief	ȱʡʵ���˻�ȡ��ˮ�ŵĹ���
 * 
 *  ��ˮ���ǽ�������Ҫ�Ĺ��ܣ���������svc_bind_unit����
 * 
 * @author	jdyu
 * @date	2008/11/26
 */
template <class svc_impl_t, class cfg_mngr_t = cfg_mngr_null>
class svc_sink_impl
	: public svc_impl_t
	{
public:
	// ��õ�ǰ�Ĳ���ID
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
 * @brief	����ƶ��α����
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
 * @brief	ȫ�ֵķ���ʵ�ֿ�
 * 
 *  �Ե���ģʽʵ�֣����������ط��ĵ���
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
	// �ַ�����������
	typedef std::vector<std::basic_string<char_t> > str_arr;

	// ���������ʵ�ֵĺ���
	virtual int on_initialize  (const str_arr & libs)
		{ return 0; }
	virtual int on_uninitialize(void)
		{ return 0; }
	
public:
	// ����ȱʡ�ű��ļ���
	virtual const char_t * default_scp(void) const
		{ return 0; }

	// �Ƿ�Ŀ¼ģʽ
	virtual bool dir_mode(void) const
		{ return false; }
	
	// ���û�л�ȡ��ִ�е�Ԫ�Ƿ��ܼ���
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

	// ��ʼ��
	int initialize(const char * lib)
		{
		str_arr libs;
		if ( lib != 0 )
			{
			const char_t split[] = { ';', ',', '\0' };
			sp::split_str(lib, libs, split, true, true, true);
			}
		int ret = on_initialize(libs);
		// ��ʼ��ʱ
		start_tick_ = dstester_thr<>::tick_count();
		return ret;
		}
	// ���ʼ��
	int uninitialize(void)
		{
		total_caps_ = calc_real_caps(total_svcs_);
		int ret = on_uninitialize();
		return ret;
		}

	// ������в��Ե�Ԫ
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
	// �߳̿�ʼ��֪ͨ
	unsigned int on_svc_begin(void)
		{
		boost::mutex::scoped_lock lock(lock_);
		return ++total_svcs_;
		}
	double caps_global(void) const
		{
		return total_caps_;
		}

	// ����ʵ��
	static svc_mngr_impl & instance(void)
		{
		static svc_mngr_impl the_svc_mngr;
		return the_svc_mngr;
		}
	static str_arr & parse_lib(const char_t * lib, str_arr & libs)
		{
		return libs;
		}

	// �ڲ�ʹ�õĺ���
protected:
	// ����ʵ��caps
	double calc_real_caps(size_t count) const
		{
		return count * 1000. / (dstester_thr<>::tick_count() - start_tick_);
		}
	// CAPS������
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
	// ��ʼ��ʱ��
	unsigned int	start_tick_;
	// �����ʵ��CAPS
	double			total_caps_;
	unsigned int	total_svcs_;
	// ͬ������
	boost::mutex	lock_;
	};

/** 
 * @class	thr_admin
 * 
 * @brief	����Ա�߳�
 * 
 *  ����������в����̵߳�������ֹͣ��ͳ��
 *  �������̵�ִ�С�
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
	 *   ��ʼ��
	 *  
	 * @author	jdyu
	 * @date	2007-7-18
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	const char * root	- [in] ���Եĸ�Ŀ¼
	 * @param	int loop_count		- [in] �ܵ�ѭ������
	 * @param	int thr_count		- [in] �����߳�����
	 * @param	bool start = true	- [in] �Ƿ����������߳�
	 * @param	bool thpool = true	- [in] �Ƿ�ʹ���̳߳�
	 * @param	bool ctrlc_stop = true	- [in] �Ƿ�֧��Ctrl+Cֹͣ
	 * @see		
	 */
	int open(const char * root, const char * libs, int loop_count, int thr_count,
		bool start = true, bool thrpool = true, bool ctrlc_stop = true, const char * listf = 0)
		{
		close();

		// ������д����Ե�Ԫ
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
			// ��������һ�������ļ����õ�unit��ȥ
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

		// Ԥ����ÿ����Ԫ
		std::for_each(all_units_.begin(), all_units_.end(),
			std::mem_fun_ref(&run_unit::prepare));

		// ��ʼ���̳߳�
		thr_max_ = thr_count;
		ret = thr_pool_.open(thrpool ? thr_count : 0);
		if ( ret != 0 )
			{
			dslog_error("thr_admin::open init thr_pool failed, ret %d.", ret);
			return ret;
			}

		// ʵ�ֿ��ʼ��
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

		// �����źŴ�����
		signal(SIGINT, signal_handler);
		signal(SIGTERM, signal_handler);
		#ifdef WIN32
		SetConsoleCtrlHandler(ctrl_handler, TRUE);
		#endif // WIN32

		// ��������Ա�߳�
		if ( start )
			this->start();
		init_ = true;

		return ret;
		}

	/** 
	 * @brief 	close
	 *  
	 *   ���ʼ��
	 *  
	 * @author	jdyu
	 * @date	2007-7-19
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	bool wait_all = false	- [in] �Ƿ�ȴ����в����߳��˳�
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

		// ���ʼ��ʵ�ֿ�
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
	// �߳�ִ�к���
	virtual int svc(void)
		{
		unsigned int count = 0;
		while ( !thr_stop_ )
			{
			boost::mutex::scoped_lock lock(thr_lock_);
			
			// �Ƿ���Ҫ�����߳�
			while ( thr_actv_ < thr_max_
				&& count < loop_count_ )
				{
				// ��ȡ�����߳�λ��
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

				// ������ڿ��еģ������µ�
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

			// �ȴ���Ϣ
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

		// ֹͣδ����߳�
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
	// ���еĲ��Ե�Ԫ
	typedef std::vector<run_unit> ru_arr;
	ru_arr					all_units_;

	// Я��һ���̳߳�
	typedef thr_pool<svc_ctx> thr_pool_impl;
	thr_pool_impl			thr_pool_;

	// ����߳�������ֹͣ�ź�
	boost::mutex			thr_lock_;
	// boost::condition		thr_cond_;
	boost::condition_variable_any		thr_cond_;
	thr_event				admin_cond_;
	thr_event				complete_cond_;

	// ��߳�����
	unsigned int			thr_actv_;
	unsigned int			thr_max_;
	unsigned int			thr_free_;
	unsigned int			loop_count_;
	unsigned int			complete_count_;

	// �߳�״̬
	enum thr_state
		{
		thr_init,
		thr_idle,
		thr_busy,
		};
	/// �߳�ʵ������
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

// ʹ���Դ������߳�
template<class svc_mngr, class svc_impl, class run_unit, class svc_ctx = thr_ctx_n>
class thr_admin_parser
	: public thr_admin<svc_mngr, thr_svc<svc_impl>, run_unit, svc_ctx>
	{
	};

#endif /* __THR_MNGR_H__ */
