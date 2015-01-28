/** 
 * @file	log_asyn.h
 * @brief	异步Log_IO类的实现
 * 
 *  为了降低Log写入带来的性能影响，实现异步的文件写入IO接口，供Log框架调用。
 *
 *  在使用异步日志之前，你需要先定义两个辅助类：
 *  1. 事件类，用于通知线程池工作（参见后面的样例thr_event）
 *  2. 线程类，由于操作系统的关系，不同的操作系统需要实现不同的线程适配（参见后面的样例as_thr_adpt)
 *
 *  由于牵涉到异步线程池的启动和停止，调用过程比同步日志稍微复杂一些，步骤如下：
 *	// 1. 使用异步日志之前，要注意先创建线程池
 *	log_pools::instance().open(2);
 *
 *	// 2. 然后打开日志类，打开的时候会自动注册到线程池中
 *	asLogger::open(asLogger_Cfg("as.log", "aslogging"), "aslog.cfg");
 *
 *	// 3. 放心大胆记录日志吧
 *	asLogger::instance()->log_crit("Hello world!");
 *
 *	// 4. 关闭日志，注意这里的关闭是异步的，关闭后日志文件还在继续写入
 *	asLogger::close();
 *
 *	// 5. 在所有日志关闭后，最后停止线程池
 *	// 由于可能还存在日志没有写完，因此线程池必须等待日志写完
 *	log_pools::instance().close();
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2011/8/30
 * 
 * @see		iFly_Log.h
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2011/8/30	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __LOG_ASYN_H__
#define __LOG_ASYN_H__

#include "log/iFly_Log.h"
#include "utils/smutex.h"
#include "iFly_Assert.h"
#include "utils/sutils.h"
#include <string>
#include <queue>
#include <vector>

/** 
 * @class	log_io_fasyn
 * @brief	异步文件接口
 * 
 *  利用线程池实现异步IO操作，这个类目前仅负责把数据排起来，真正的文件操作供线程池调用
 * 
 * @author	jdyu
 * @date	2011/8/30
 */
class log_io_fasyn
	: public Log_IO_FILE
	{
public:
	typedef Log_IO_FILE parent;
	enum { max_msg_size = 400 * 1024 };

public:
	log_io_fasyn(void)
		: log_size_ (0)
		{
		}
	virtual ~log_io_fasyn(void)
		{
		close();
		}

	int open(const char * fn, Log_Mutex * lock /* = 0 */, int mode /* = 0 */, void * rev /* = 0 */);
	int close(void);

	/** 
	 *  文本或二进制方式写入
	 */
	// 窄字符记录，对数据进行拼接优化
	int write(const char* msg);
	int write(const wchar_t * msg);
	int write(const void * data, int size);

	/** 
	 *  清空文件
	 */
	int empty(void)
		{
		// 清空
		sp::sguard<sp::smutex> gd(acts_lock_);
		pending_acts_.push(act_empty);
		return 0;
		}

	/** 
	 *  从缓存刷新日志，异步日志这个功能取消
	 */
	void flush(void)
		{
		// 刷新文件，暂时不刷新
		// sp::sguard<sp::smutex> gd(acts_lock_);
		// pending_acts_.push(act_flush);
		}

	/** 
	 *  获得当前尺寸
	 */
	size_t size(void)
		{
		sp::sguard<sp::smutex> gd(acts_lock_);
		return log_size_;
		}

	/** 
	 *  备份旧的文件
	 */
	int backup(const char* bname /* = 0 */)
		{
		// 备份/删除旧文件
		sp::sguard<sp::smutex> gd(acts_lock_);
		pending_acts_.push(act_backup);
		log_size_ = 0;
		if ( bname )
			backup_bname_ = bname;
		return 0;
		}

public:
	// 供线程池调用的实际干活的函数
	int do_work(int & acts_left, bool & all_done);
	
	// 不让全局Log来delete对象
	bool donot_delete(void)
		{
		return true;
		}

	// 是否还有数据
	bool work_done(void)
		{
		sp::sguard<sp::smutex> gd(acts_lock_);
		return pending_acts_.empty();
		}

private:
	// 数据类型
	enum action_type
		{
		act_none,
		act_string,
		act_wstring,
		act_binary,
		act_open,
		act_empty,
		act_backup,
		act_close,
		act_flush,
		};
	// 内部保存的信息链表，用于被线程池调用
	std::queue<std::string>		logging_strs_;
	std::queue<std::wstring>	logging_wstrs_;
	std::queue<std::string>		logging_bins_;
	std::queue<action_type>		pending_acts_;
	// 保护数据链表的锁
	sp::smutex					acts_lock_;
	// 备份的参数
	std::string					backup_bname_;
	// log size
	size_t						log_size_;
	}; // log_io_fasyn

/** 
 * @class	log_event
 * @brief	事件基类，用户在使用时需要适配一个相同接口的类
 * 
 *  日志线程之间需要互相通知，因此需要这个事件类同步
 * 
 * @author	jdyu
 * @date	2011/9/5
 */
class log_event
	{
	public:
	log_event(void)
		{
		}
	virtual ~log_event(void)
		{
		}
	virtual int wait (int time_out = -1)
		{
		return 0;
		}
	virtual int signal (void)
		{
		return 0;
		}
	virtual int reset (void)
		{
		return 0;
		}
	};

/** 
 * @class	log_io_mngr
 * @brief	IO管理器
 * 
 *  管理进程中所有的异步IO类，供线程池调用
 *  IO类在打开的时候把自己注册到此管理器中，关闭时从此管理器中注销
 * 
 * @author	jdyu
 * @date	2011/9/5
 */
class log_io_mngr
	{
private:
	log_io_mngr(void)
		{
		}
	~log_io_mngr(void)
		{
		log_ios_.clear();
		}
public:
	// 以单体类实现
	static log_io_mngr & instance(void)
		{
		static log_io_mngr the_iom_;
		return the_iom_;
		}

	// 注册一个异步IO类到管理器
	int io_bind(log_io_fasyn* io_asyn)
		{
		sp::sguard<sp::smutex> gd(log_lock_);

		SP_ASSERT(std::find(log_ios_.begin(), log_ios_.end(), io_asyn) == log_ios_.end());
		for ( size_t i = 0; i < log_ios_.size(); i++ )
			{
			if ( log_ios_[i].sta_ == log_invalid )
				{
				log_ios_[i].io_ = io_asyn;
				log_ios_[i].sta_ = log_idle;
				log_ios_[i].evt_ = thr_evts_[i % thr_evts_.size()];
				return 0;
				}
			}
		log_ios_.push_back(log_info(log_idle, io_asyn, thr_evts_[log_ios_.size() % thr_evts_.size()]));
		return 0;
		}
	// IO类关闭前，需要从管理器中注销，需要保证IO中未写入数据全部写入
	int io_unbind(log_io_fasyn* io_asyn)
		{
		sp::sguard<sp::smutex> gd(log_lock_);
		
		SP_ASSERT(std::find(log_ios_.begin(), log_ios_.end(), io_asyn) != log_ios_.end());
		log_pi pi = std::find(log_ios_.begin(), log_ios_.end(), io_asyn);
		if ( pi != log_ios_.end() )
			{
			// 如果数据写完，直接释放，否则标记为待释放
			if ( pi->io_->work_done() )
				{
				pi->sta_ = log_invalid;
				pi->io_ = 0;
				}
			else
				pi->sta_ = log_unbinding;

			return 0;
			}
		return -1;
		}

	// 调用IO实现真正的写入操作，供线程池调用
	int io_work(int id, bool & all_done)
		{
		int acts_left = 0;
		all_done = false;

		// 执行具体的文件操作
		sp::sguard<sp::smutex> gd(log_lock_);
		log_info & li = log_ios_[id];
		if ( li.sta_ == log_idle )
			{
			li.sta_ = log_busy;
			log_lock_.release();
			li.io_->do_work(acts_left, all_done);
			log_lock_.acquire();
			li.sta_ = log_idle;
			}

		// 是否可以释放了
		if ( all_done )
			{
			delete li.io_; li.io_ = 0;
			li.sta_ = log_invalid;
			}
		if ( li.sta_ == log_invalid )
			all_done = true;

		return acts_left;
		}

	/*
	 * 内部数据访问
	 */

	// 注册的IO数量
	int size(void)
		{
		return (int) log_ios_.size();
		}
	// 返回指定位置的IO类
	log_io_fasyn * io(int id)
		{
		return log_ios_[id].io_;
		}

	// 设置全局任务事件
	void work_event(std::vector<log_event*> & evts)
		{
		thr_evts_ = evts;
		}
	// 通知线程池干活
	void work_signal(log_io_fasyn * io)
		{
		log_pi pi = std::find(log_ios_.begin(), log_ios_.end(), io);
		if ( pi != log_ios_.end() )
			{
			if ( pi->evt_ )
				pi->evt_->signal();
			}
		}

private:
	// 日志类的状态，忙还是闲
	enum log_status
		{
		log_invalid,
		log_idle,
		log_busy,
		log_unbinding,
		};
	// 每个IO现在的状态
	class log_info
		{
		public:
		log_status		sta_;
		log_io_fasyn *	io_;
		log_event *		evt_;

		log_info(log_status sta = log_invalid, log_io_fasyn* io = 0, log_event* evt = 0)
			: sta_	(sta)
			, io_	(io)
			, evt_	(evt)
			{
			}
		bool operator () (const log_io_fasyn* io) const
			{
			SP_ASSERT(io);
			return io != 0 && io == io_;
			}
		bool operator == (const log_io_fasyn* io) const
			{
			SP_ASSERT(io);
			return io != 0 && io == io_;
			}

		// 通知线程池干活
		void work_signal(void)
			{
			if ( evt_ != 0 )
				evt_->signal();
			}
		};

	// 异步文件日志队列
	typedef std::vector<log_info>	log_list;
	typedef log_list::iterator		log_pi;
	log_list	log_ios_;
	sp::smutex	log_lock_;

	// 所有的工作事件
	std::vector<log_event*>			thr_evts_;
	};

/*
 * log_io_fasyn的几个内联函数
 */

// 异步IO类的打开函数，相比非异步，多了注册的过程
inline int log_io_fasyn::open(const char * fn, Log_Mutex * lock /* = 0 */, int mode /* = 0 */, void * rev /* = 0 */)
	{
	int ret = 0;
	if ( parent::handle() == NULL )
		{
		sp::sguard<sp::smutex> gd(acts_lock_);
		ret = parent::open(fn, lock, mode, rev);
		if ( ret == 0 )
			{
			log_io_mngr::instance().io_bind(this);
			log_size_ = parent::size();
			}
		}
	return ret;
	}

// 异步iO关闭，从管理器中注销，注意这里是异步的
inline int log_io_fasyn::close(void)
	{
	if ( parent::handle() != NULL )
		{
		sp::sguard<sp::smutex> gd(acts_lock_);
		pending_acts_.push(act_close);
		}
	return 0;
	}

// 窄字符记录，对数据进行拼接优化
inline int log_io_fasyn::write(const char* msg)
		{
		if ( msg != NULL )
			{
			sp::sguard<sp::smutex> gd(acts_lock_);
			if ( !pending_acts_.empty() && pending_acts_.back() == act_string )
				{
				log_size_ += sp::strlen(msg);
				logging_strs_.back().append(msg);
				if ( logging_strs_.back().size() > max_msg_size - 10 * 1024 )
					log_io_mngr::instance().work_signal(this);
				}
			else
				{
				std::string mb;
				mb.reserve(max_msg_size);
				mb.assign(msg);
				logging_strs_.push(mb);
				pending_acts_.push(act_string);
				log_size_ += mb.size();
				}
			}
		return 0;
		}
// 宽字符记录，对数据进行拼接优化
inline int log_io_fasyn::write(const wchar_t * msg)
		{
		if ( msg != NULL )
			{
			sp::sguard<sp::smutex> gd(acts_lock_);
			if ( !pending_acts_.empty() && pending_acts_.back() == act_wstring )
				{
				log_size_ += sp::strlen(msg) * 2;
				logging_wstrs_.back().append(msg);
				if ( logging_wstrs_.back().size() > max_msg_size - 10 * 1024 )
					log_io_mngr::instance().work_signal(this);
				}
			else
				{
				std::wstring mb;
				mb.reserve(max_msg_size);
				mb.assign(msg);
				logging_wstrs_.push(mb);
				pending_acts_.push(act_wstring);
				log_size_ += mb.size() * 2;
				}
			}
		return 0;
		}
// 二进制版本
inline int log_io_fasyn::write(const void * data, int size)
		{
		if ( data != NULL )
			{
			sp::sguard<sp::smutex> gd(acts_lock_);
			if ( !pending_acts_.empty() && pending_acts_.back() == act_binary )
				{
				logging_bins_.back().append((const char*)data, (const char*)data + size);
				if ( logging_bins_.back().size() > max_msg_size - 10 * 1024 )
					log_io_mngr::instance().work_signal(this);
				}
			else
				{
				std::string mb;
				mb.reserve(max_msg_size);
				mb.assign((const char*)data, (const char*)data + size);
				logging_bins_.push(mb);
				pending_acts_.push(act_binary);
				}
			log_size_ += size;
			}
		return 0;
		}

// 供线程池调用的函数
inline int log_io_fasyn::do_work(int & acts_left, bool & all_done)
	{
	int ret = 0;
	action_type act = act_none;
	std::string str;
	std::wstring wstr;

	// 返回给外部调用者
	acts_left = 0;
	all_done = false;

	// 获取数据，使用数据锁
		{
		sp::sguard<sp::smutex> gd(acts_lock_);
		if ( pending_acts_.empty() )
			return 0;

		act = pending_acts_.front();
		switch ( act )
			{
		case act_string:
			str = logging_strs_.front();
			logging_strs_.pop();
			break;
		case act_wstring:
			wstr = logging_wstrs_.front();
			logging_wstrs_.pop();
			break;
		case act_binary:
			str = logging_bins_.front();
			logging_bins_.pop();
			break;
		default:
			break;
			}

		pending_acts_.pop();
		acts_left = (int) pending_acts_.size();
		}

	// 执行文件操作，使用文件锁
	switch ( act )
		{
		case act_string:
			ret = parent::write(str.c_str());
			// printf("written %6d bytes.\r", str.size());
			break;
		case act_wstring:
			ret = parent::write(wstr.c_str());
			break;
		case act_binary:
			ret = parent::write(&str[0], (int) str.size());
			break;
		case act_backup:
			parent::flush();
			ret = parent::backup(backup_bname_.empty() ? 0 : backup_bname_.c_str());
				{
				sp::sguard<sp::smutex> gd(acts_lock_);
				log_size_ += parent::size();
				}
			break;
		case act_empty:
			ret = parent::empty();
			break;
		case act_open:
			break;
		case act_flush:
			parent::flush();
			break;
		case act_close:
			all_done = true;
			ret = parent::close();
			break;
		default:
			SP_ASSERT(0 && "Invalid logging data type!");
			break;
		}

	return ret;
	}

/** 
 * @class	log_thr_pool
 * @brief	IO日志线程池类
 * 
 *  提供一个可以进行线程和事件类适配的线程池，由于线程的实现没有标准支持，因此用户在使用时必须使用不同的线程和事件类
 *  具体可以参考下面注释里的适配类
 * 
 * @author	jdyu
 * @date	2011/9/5
 */
template <typename thr_adpt_t, typename event_t>
class log_thr_pool
	: public thr_adpt_t
	{
private:
	log_thr_pool(void)
		{
		}
	virtual ~log_thr_pool(void)
		{
		close();
		}
public:
	// 单体模式
	static log_thr_pool & instance(void)
		{
		static log_thr_pool the_thrp_;
		return the_thrp_;
		}
	// 初始化，可以设置启动几个线程
	virtual int open(int thr_count)
		{
		int ret = 0;

		thr_count_ = thr_count;
		for ( int i = 0; i < thr_count; i++ )
			thr_evts_.push_back(new event_t);

		log_io_mngr::instance().work_event(thr_evts_);

		ret = thr_adpt_t::open(thr_count);
		return ret;
		}
	virtual int close(void)
		{
		int ret = 0;

		thr_stopping_ = true;
		ret = thr_adpt_t::close();

		for ( int i = 0; i < (int) thr_evts_.size(); i++ )
			delete thr_evts_[i];
		thr_evts_.clear();
		return 0;
		}

protected:
	// 线程函数体
	virtual int svc(int idx)
		{
		int ret = 0;
		log_io_mngr & iom = log_io_mngr::instance();
		bool all_done = true;

		// 事件
		log_event * work_event = thr_evts_[idx];
		while ( 1 )
			{
			bool all_done = true;
			int pending_work = 0;
			for ( int i = idx; i < iom.size(); i += (int)thr_evts_.size() )
				{
				// 现在使用一揽子执行
				bool io_done = false;
				pending_work += iom.io_work(i, io_done);
				if ( !io_done )
					all_done = false;
				}

			// 是否停止
			if ( thr_stopping_ )
				{
				if ( all_done )
					break;
				}

			// 如果没有内容，需要休眠等待任务
			if ( pending_work == 0 )
				work_event->wait(1000);
			}
		return 0;
		}

private:
	// stop flag
	bool					thr_stopping_;
	// 所有的工作事件
	std::vector<log_event*>	thr_evts_;
	// 线程数量
	int						thr_count_;
	};

// **********************************************
// 提供事件和线程适配的样例代码，用户可以参考
// **********************************************

#if 0
/** 
 * @class	thr_event
 * 
 * @brief	事件类
 * 
 *  Windows下使用Event实现，linux下借用ace
 * 
 * @author	jdyu
 * @date	2008/8/22
 */
#ifdef WIN32
class thr_event
	: public log_event
	{
public:
	thr_event(int manual_reset = 0, int init_state = 0, const char * name = 0)
		: event_	(0)
		{
		event_ = CreateEventA(0, manual_reset, init_state, name);
		SP_ASSERT(event_);
		}
	~thr_event(void)
		{
		if ( event_ )
			CloseHandle(event_);
		}

	int wait (int time_out = -1)
		{
		return WaitForSingleObject(event_, time_out == -1 ? INFINITE : time_out);
		}

	int signal (void)
		{
		if ( !SetEvent(event_) )
			return GetLastError();
		return 0;
		}
	int reset (void)
		{
		if ( !ResetEvent(event_) )
			return GetLastError();
		return 0;
		}

private:
	HANDLE		event_;
	};
#else // Non WIN32
#include "ace/Event.h"
class thr_event
	{
public:
	thr_event(int manual_reset = 0, int init_state = 0, const char * name = 0)
		: event_(manual_reset, init_state, 0, name)
		{
		}
	~thr_event(void)
		{
		}

	int wait (int time_out = -1)
		{
		if ( time_out == -1 )
			return event_.wait();
		ACE_Time_Value tv;
		tv.msec(time_out);
		return event_.wait(&tv, 0);
		}

	int signal (void)
		{
		return event_.signal();
		}
	int reset (void)
		{
		return event_.reset();
		}

private:
	ACE_Event	event_;
	};
#endif // Non WIN32

/** 
 * @class	as_thr_adpt
 * @brief	线程适配类
 * 
 *  线程适配类，此处使用boost实现，ace或其他同样可以
 * 
 * @author	jdyu
 * @date	2011/9/5
 */
class as_thr_adpt
	{
public:
	virtual int open(unsigned int thr_count)
		{
		int ret = 0;
		for ( unsigned int i = 0; i < thr_count; i++ )
			{
			thrs_.push_back(new boost::thread(thr_inst(this, i)));
			}
		return ret;
		}
	virtual int close(void)
		{
		int ret = 0;
		for ( unsigned int i = 0; i < (int) thrs_.size(); i++ )
			{
			thrs_[i]->join();
			delete thrs_[i];
			}
		thrs_.clear();
		return ret;
		}
	virtual int svc(int idx)
		{
		return 0;
		}

protected:
	// boost thread object
	std::vector<boost::thread*>	thrs_;

	// 线程函数对象
	struct thr_inst
		{
		thr_inst(as_thr_adpt * thu, int idx)
			: thu_	(thu)
			, idx_	(idx)
			{ }
		void operator() (void)
			{ thu_->svc(idx_); }

		as_thr_adpt * thu_;
		int	idx_;
		};
	};

// 定义异步日志线程池
typedef log_thr_pool<as_thr_adpt, thr_event> log_pools;

// 使用异步IO的日志类定义
typedef Log_Impl_T<log_io_fasyn, Log_Thread_Mutex> log_impl;
// 或者可以如下
LOG_DECL_SINGLETON_EX(log_io_fasyn, Log_Thread_Mutex, Log_Cfg_Heap_Reg, asLogger)

// 真正使用异步日志时，可以如下写代码
/*
int main(int argc, char* argv[])
	{
	// 1. 使用异步日志之前，要注意先创建线程池
	log_pools::instance().open(2);

	// 2. 然后打开日志类，打开的时候会自动注册到线程池中
	asLogger::open(asLogger_Cfg("as.log", "aslogging"), "aslog.cfg");
	log_impl log1, log2, log3, log4;
	log_cfg  cfg1("a1.log");
	log_cfg  cfg2("a2.log");
	log_cfg  cfg3("a3.log");
	log_cfg  cfg4("a4.log");

	log1.open(&cfg1);
	log2.open(&cfg2);
	log3.open(&cfg3);

	// 3. 放心大胆记录日志吧
	tick_total = GetTickCount();
	tick_max = 0;
	for ( int i = 0; i < max_count; i++ )
		{
		tb = GetTickCount();
		asLogger::instance()->log_crit("Hello hello world! this is Mr %d.", i);
		te = GetTickCount();
		if ( te - tb > tick_max )
			tick_max = te - tb;
		if ( i % 10000 == 0 )
			printf("*");
		
		log1.log_crit("Hello log1 %d.", i);
		log2.log_crit("Hello log2 %d.", i);
		log3.log_crit("Hello log3 %d.", i);
		}
	printf("\nAsyn Log OK, time %.3f, max %.3f.\n", (GetTickCount() - tick_total) * 1e-3, tick_max * 1e-3);

	// 4. 关闭日志，注意这里的关闭是异步的，关闭后日志文件还在继续写入
	log1.close();
	log2.close();
	log3.close();
	asLogger::close();

	// 5. 在所有日志关闭后，最后停止线程池
	// 由于可能还存在日志没有写完，因此线程池必须等待日志写完
	log_pools::instance().close();
	}
 */

#endif // 0

#endif /* __LOG_ASYN_H__ */
