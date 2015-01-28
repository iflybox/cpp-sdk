/** 
 * @file	proc_resp.h
 * @brief	统计函数响应时间
 * 
 *  用于统计函数最大、最小、平均响应时间，
 *  也可以统计性能分布情况
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2007-7-20
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2007-7-20	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __PROC_RESP_H__
#define __PROC_RESP_H__

#include <vector>
#include <string>
#include <iomanip>
#ifndef WIN32//linux yanxu2 add 2010-12-29
# include <string.h>
#endif
#include "log/iFly_Log.h"

/** 
 * @class	proc_resp
 * 
 * @brief	函数响应时间统计类
 * 
 *  统计所有函数的响应时间，生成报表
 * 
 * @author	jdyu
 * @date	2007-7-20
 * 
 * @see		
 */
template <class mutex = Log_Mutex_Null, class char_t = char>
class proc_resp
	{
public:
	// string define
	typedef std::basic_string<char_t> string;

	// 接口
	proc_resp & add_proc(const char_t * proc, double scale = 1.)
		{
		LOG_GUARD(mutex, lock_);

		proc_unit_it pi = std::find(unit_arr_.begin(), unit_arr_.end(), proc);
		if ( pi == unit_arr_.end() )
			unit_arr_.push_back(proc_unit(proc, -1., scale));
		return *this;
		}
	proc_resp & operator () (const char_t * proc, double scale = 1.)
		{
		return add_proc(proc, scale);
		}
	proc_resp & add_proc(const char_t * proc, bool sub)
		{
		LOG_GUARD(mutex, lock_);

		proc_unit_it pi = std::find(unit_arr_.begin(), unit_arr_.end(), proc);
		if ( pi == unit_arr_.end() )
			unit_arr_.push_back(proc_unit(proc, -1., 1., sub));
		return *this;
		}
	proc_resp & operator () (const char_t * proc, bool sub)
		{
		return add_proc(proc, sub);
		}
	unsigned int add_msec(const char_t * proc, double msec)
		{
		LOG_GUARD(mutex, lock_);

		unsigned int ret = 1;
		proc_unit_it pi = std::find(unit_arr_.begin(), unit_arr_.end(), proc);
		if ( pi == unit_arr_.end() )
			unit_arr_.push_back(proc_unit(proc, msec));
		else
			ret = (*pi).add_msec(msec);
		return ret;
		}
	unsigned int add_sec(const char_t * proc, double sec)
		{
		return add_msec(proc, sec * 1000);
		}
	unsigned int add_usec(const char_t * proc, double usec)
		{
		return add_msec(proc, usec / 1000.);
		}
	unsigned int add_nsec(const char_t * proc, double nsec)
		{
		return add_msec(proc, nsec / (1000 * 1000));
		}

	// Add success and failure
	unsigned int add_success(void)
		{
		return add_msec("SUCCESS", 1.);
		}
	unsigned int add_failure(void)
		{
		return add_msec("FAILURE", 1.);
		}
	unsigned int add_result(bool success)
		{
		return success ? add_success() : add_failure();
		}
	unsigned int add_record(const char_t * str)
		{
		return add_msec(str, 1.);
		}

	void make_report(string & rpt_str, const char_t * sub = 0)
		{
		LOG_GUARD(mutex, lock_);
		if ( sub )
			subject_ = sub;

		rpt_str.reserve(unit_arr_.size() * 200);
		if ( !subject_.empty() )
			{
			std::basic_stringstream<char_t> stm;
			rpt_str.append(77, char_t('='));
			stm << std::endl
				<< subject_
				<< std::endl;
			rpt_str += stm.str();
			rpt_str.append(77, char_t('-'));
			rpt_str.append(1,  char_t('\n'));
			}

		for ( int i = 0; i < (int) unit_arr_.size(); i++ )
			unit_arr_[i].make_report(rpt_str, i == 0);

		rpt_str.append(77, char_t('='));
		rpt_str.append( 1, char_t('\n'));
		}

	void make_distrib_report(string & rpt_str, const char_t * sub = 0)
		{
		rpt_str.reserve(unit_arr_.size() * 200);
		if ( sub )
			{
			std::basic_stringstream<char_t> stm;
			rpt_str.append(77, char_t('='));
			stm << std::endl
				<< sub
				<< std::endl;
			rpt_str += stm.str();
			rpt_str.append(77, char_t('-'));
			rpt_str.append(1,  char_t('\n'));
			}

		LOG_GUARD(mutex, lock_);
		for ( int i = 0; i < (int) unit_arr_.size(); i++ )
			unit_arr_[i].make_distrib_report(rpt_str, i == 0);

		rpt_str.append(77, char_t('='));
		rpt_str.append( 1, char_t('\n'));
		}

	void make_prftime_report(string & rpt_str, const char_t * sub = 0)
	{
		rpt_str.reserve(unit_arr_.size() * 200);
		if ( sub )
		{
			std::basic_stringstream<char_t> stm;
			rpt_str.append(77, char_t('='));
			stm << std::endl
				<< sub
				<< std::endl;
			rpt_str += stm.str();
			rpt_str.append(77, char_t('-'));
			rpt_str.append(1,  char_t('\n'));
		}

		LOG_GUARD(mutex, lock_);
		for ( int i = 0; i < (int) unit_arr_.size(); i++ )
			unit_arr_[i].make_prftime_report(rpt_str, i == 0);

		rpt_str.append(77, char_t('='));
		rpt_str.append( 1, char_t('\n'));
	}

	// yanxu2 2011-10-25 命中率
	void make_hit_report(string & rpt_str, const char_t * sub = 0)
	{
		rpt_str.reserve(unit_hit_arr_.size() * 200);
		if ( sub )
		{
			std::basic_stringstream<char_t> stm;
			rpt_str.append(77, char_t('='));
			stm << std::endl
				<< sub
				<< std::endl;
			rpt_str += stm.str();
			rpt_str.append(77, char_t('-'));
			rpt_str.append(1,  char_t('\n'));
		}

		LOG_GUARD(mutex, lock_);
		for ( int i = 0; i < (int) unit_hit_arr_.size(); i++ )
			unit_hit_arr_[i].make_hit_report(rpt_str, i == 0);

		rpt_str.append(77, char_t('='));
		rpt_str.append( 1, char_t('\n'));
	}

	// yanxu2 2011-10-25 命中率
	void add_hit_percent(const char_t * proc)
	{
		LOG_GUARD(mutex, lock_);

		proc_unit_it pi = std::find(unit_hit_arr_.begin(), unit_hit_arr_.end(), proc);
		if ( pi == unit_hit_arr_.end() )
			unit_hit_arr_.push_back(proc_unit(proc));

		pi = std::find(unit_hit_arr_.begin(), unit_hit_arr_.end(), proc);
	}

	int svc_increase(void)
		{
		LOG_GUARD(mutex, lock_);
		return ++svc_active_;
		}
	int svc_decrease(void)
		{
		LOG_GUARD(mutex, lock_);
		return --svc_active_;
		}
	int svc_active_count(void)
		{
		LOG_GUARD(mutex, lock_);
		return svc_active_;
		}

	// constructor
	proc_resp(const char_t * subject = 0)
		{
		if ( subject )
			subject_ = subject;
		lock_.open();
		}
	virtual ~proc_resp(void)
		{
		lock_.close();
		}
	string & subject(void)
		{
		return subject_;
		}
	static proc_resp & instance(void)
		{
		static proc_resp the_pr;
		return the_pr;
		}

	/** 
	 * @class	proc_unit
	 * @brief	一个函数单元
	 * 
	 *  性能统计的一个函数单元
	 */
	/**
	 * yanxu2 add 2011-08-04
	 * @brief 新增接口调用时间统计分布功能
	 * 分布区间为(共5个时间区间)：0-100ms, 100-500ms, 500-1000ms, 1-3s, 3s more
	 */
	class proc_unit
		{
	public:
		const char_t * name(void) const
			{
			return proc_name_.c_str();
			}
		double min_sec(void) const
			{
			return min_msec_ / 1000.;
			}
		double min_msec(void) const
			{
			return min_msec_;
			}
		double max_sec(void) const
			{
			return max_msec_ / 1000.;
			}
		double max_msec(void) const
			{
			return max_msec_;
			}
		double average_msec(void) const
			{
			if ( call_times_ == 0 )
				return 0.;
			return all_msec_ / call_times_;
			}

		double hit_percent(void) const
		{
			if ( hit_miss_cnt_ == 0 )
				return 0.;

			double b = (double)hit_cnt_ / hit_miss_cnt_;
			return b * 100;
		}

		// 增加一条记录
		unsigned int add_msec(double msec)
			{
			if ( msec < min_msec_ )
				min_msec_ = msec;
			if ( msec > max_msec_ )
				max_msec_ = msec;
			all_msec_ += msec;

			// 分布情况
			size_t index = (size_t) std::min<double>(msec*scale_/distrib_graduation, distrib_max);
			if ( index >= distrib_.size() )
				distrib_.resize(index + 1, 0);
			distrib_[index] ++;

			// yanxu2 add 2011-08-04
			// 设置idss接口调用时间分布数组
			if (msec <= 100)
				prftime_[0] ++;
			else if(msec > 100 && msec <= 500)
				prftime_[1] ++;
			else if(msec > 500 && msec <= 1000)
				prftime_[2] ++;
			else if(msec > 1000 && msec <= 3000)
				prftime_[3] ++;
			else if(msec > 3000)
				prftime_[4] ++;

			return ++call_times_;
			}
		unsigned int add_sec(double sec)
			{
			return add_msec(sec * 1000);
			}
		unsigned int add_usec(double usec)
			{
			return add_msec(usec / 1000.);
			}
		unsigned int add_nsec(double nsec)
			{
			return add_msec(nsec / ( 1000 * 1000) );
			}

		// yanxu2 2011-10-25 命中率
		void add_hit()
		{
		}
		
		proc_unit(const char_t * proc_name, double msec = -1., double scale = 1., bool sub = false)
			: proc_name_	(proc_name)
			, max_msec_		(0.)
			, min_msec_		(1e9)
			, all_msec_		(0.)
			, call_times_	(0)
			, distrib_		(distrib_max + 1, 0)
			, scale_		(scale)
			, subject_		(sub)
			, prftime_		(5, 0)
			, hit_cnt_		(0)
			, miss_cnt_		(0)
			, hit_miss_cnt_		(0)
			{
			if ( msec >= 0 )
				add_msec(msec);
			}
		bool operator == (const char_t * name) const
			{
			return proc_name_ == name;
			}

		void make_report(string & rpt, bool gen_head = false)
			{
			if ( gen_head )
				{
				std::basic_stringstream<char_t> stm;
				const char_t proc_name[] = { 'P', 'r', 'o', 'c', '\0' };
				const char_t call_time[] = { 'C', 'a', 'l', 'l', '\0' };
				const char_t min_time [] = { 'M', 'i', 'n', '(', 'm', 's', ')', '\0' };
				const char_t max_time [] = { 'M', 'a', 'x', '(', 'm', 's', ')', '\0' };
				const char_t ave_time [] = { 'A', 'v', 'e', '(', 'm', 's', ')', '\0' };
				stm << std::setw(32) << proc_name
					<< std::setw(10)  << call_time
					<< std::setw(12) << min_time
					<< std::setw(12) << max_time
					<< std::setw(12) << ave_time
					<< std::endl;
				
				rpt += stm.str();
				rpt.append(77, char_t('-'));
				rpt.append( 1, char_t('\n'));
				}

			std::basic_stringstream<char_t> stm;
			if ( !subject_ )
				{
				stm.precision(4);
				stm.setf(std::ios::fixed);
				stm << std::setw(32) << proc_name_
					<< std::setw(10)  << call_times_
					<< std::setw(12) << ( min_msec_ > 1e8 ? 0. : min_msec_ )
					<< std::setw(12) << max_msec_
					<< std::setw(12) << average_msec()
					<< std::endl;
				}
			else
				{
				stm << std::setw(32) << proc_name_
					/* << std::string( 1, char_t(' ')) */
					/* << std::string(43, char_t('-')) */
					<< std::endl;
				}
			rpt += stm.str();
			}

		void make_distrib_report(string & rpt, bool gen_head = false)
			{
			if ( gen_head )
				{
				std::basic_stringstream<char_t> stm;
				const char_t proc_name[] = { 'P', 'r', 'o', 'c', '\0' };
				
				stm << std::setw(32) << proc_name;
				stm.precision(2);
				stm.setf(std::ios::fixed);
				for ( size_t i = 0; i < distrib_.size(); i++ )
					stm << std::setw(i == 0 ? 8 : 6) << i * 0.10;
				stm << std::endl;

				rpt += stm.str();
				rpt.append(77, char_t('-'));
				rpt.append( 1, char_t('\n'));
				}

			std::basic_stringstream<char_t> stm;
			stm.precision(2);
			stm.setf(std::ios::fixed);
			stm << std::setw(32) << proc_name_;
			if ( !subject_ )
				{
				for ( size_t i = 0; i < distrib_.size(); i++ )
					stm << std::setw(i == 0 ? 8 : 6) << distrib_[i];
				}
			stm << std::endl;
			rpt += stm.str();
			}

		void make_prftime_report(string & rpt, bool gen_head = false)
		{
			// 处理 proc_name_
			if (
				std::string::npos != proc_name_.find("_success")
				|| std::string::npos != proc_name_.find("(KB/s)")
				|| std::string::npos != proc_name_.find("TotalTime(sec)")
				|| std::string::npos != proc_name_.find("_failed")
				)
				return;

			if ( gen_head )
			{
				std::basic_stringstream<char_t> stm;
				const char_t proc_name[] = { 'P', 'r', 'o', 'c', '\0' };

				stm << std::setw(32) << proc_name;
				stm.precision(2);
				stm.setf(std::ios::fixed);

				stm << std::setw(13) << "0-100ms";
				stm << std::setw(13) << "100-500ms";
				stm << std::setw(13) << "500-1000ms";
				stm << std::setw(13) << "1-3s";
				stm << std::setw(13) << "3s more";
				
				stm << std::endl;

				rpt += stm.str();
				rpt.append(77, char_t('-'));
				rpt.append( 1, char_t('\n'));
			}

			std::basic_stringstream<char_t> stm;
			stm.precision(2);
			stm.setf(std::ios::fixed);
			stm << std::setw(32) << proc_name_;
			if ( !subject_ )
			{
				for ( size_t i = 0; i < prftime_.size(); i++ )
					stm << std::setw(13) << prftime_[i];
			}
			stm << std::endl;
			rpt += stm.str();
		}

		// yanxu2 2011-10-25 命中率
		void make_hit_report(string & rpt, bool gen_head = false)
		{
			if ( gen_head )
			{
				std::basic_stringstream<char_t> stm;
				const char_t proc_name[] = { 'P', 'r', 'o', 'c', '\0' };
				const char_t call_col[] = { 'C', 'a', 'l', 'l', '\0' };
				const char_t hit_col [] = { 'H', 'i', 't', '\0' };
				const char_t miss_col [] = { 'M', 'i', 's', 's', '\0' };
				const char_t percent_col [] = { 'P', 'e', 'r', 'c', 'e', 'n', 't', '\0' };
				stm << std::setw(32) << proc_name
					<< std::setw(10)  << call_col
					<< std::setw(12) << hit_col
					<< std::setw(12) << miss_col
					<< std::setw(12) << percent_col
					<< std::endl;

				rpt += stm.str();
				rpt.append(77, char_t('-'));
				rpt.append( 1, char_t('\n'));
			}

			std::basic_stringstream<char_t> stm;
			if ( !subject_ )
			{
				stm.precision(4);
				stm.setf(std::ios::fixed);
				stm << std::setw(32) << "hit percent"
					<< std::setw(10)  << hit_miss_cnt_
					<< std::setw(12) << hit_cnt_
					<< std::setw(12) << miss_cnt_
					<< std::setw(12) << hit_percent() << "%"
					<< std::endl;
			}
			else
			{
				stm << std::setw(32) << "hit percent"
					<< std::endl;
			}
			rpt += stm.str();
		}

		// 最大记录刻度
		enum { distrib_max = 30 };
		// 分布刻度（毫秒）
		enum { distrib_graduation = 100 };

	private:
		// 函数名
		string	proc_name_;
		// 时间计数(单位: 毫秒)
		double	min_msec_;
		double	max_msec_;
		double	all_msec_;
		// 调用次数
		unsigned int call_times_;
		// 统计分布情况的数组(0.1秒为一个区间)
		std::vector<int> distrib_;
		// 粒度(毫秒的倍数)
		double	scale_;
		// 是否是一个主题，不打印性能，打印空行
		bool	subject_;
		// 保存接口调用时间分布数组
		std::vector<int> prftime_;

		// yanxu2 2011-10-25
		// 命中率
		int hit_cnt_;
		int miss_cnt_;
		int hit_miss_cnt_;
		};

private:
	// 主题
	string			subject_;
	// 记录列表
	typedef std::vector<proc_unit>	proc_unit_arr;
	typedef typename proc_unit_arr::iterator	proc_unit_it;
	proc_unit_arr	unit_arr_;
	proc_unit_arr	unit_hit_arr_;// yanxu2 2011-10-25 命中率

	// 活动实例数
	int				svc_active_;

	// 锁定
	mutex			lock_;
	};

// 单体实例
#define wproc_resp_mngr proc_resp<Log_Thread_Mutex, wchar_t>::instance
#define proc_resp_mngr	proc_resp<Log_Thread_Mutex, char>::instance

#endif /* __PROC_RESP_H__ */
