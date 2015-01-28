/** 
 * @file	thr_pool.h
 * @brief	线程池类
 * 
 *  实现一个线程池，用于优化线程分配的效率
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2007-7-12
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2007-7-12	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __THR_POOL_H__
#define __THR_POOL_H__

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

#include "iFly_Assert.h"
#include <vector>
#ifndef WIN32
#include "ace/Thread_Semaphore.h"
#endif // WIN32

#pragma warning(push)
#pragma warning(disable : 4819)
#include "boost/thread.hpp"
#pragma warning(pop)

/** 
 * @class	thr_sem
 * 
 * @brief	自己封装的信号量
 * 
 *  目前在使用boost的condition遇到问题，只能用这个类顶着先。
 * 
 * @author	jdyu
 * @date	2007-7-19
 * 
 * @see		
 */
class thr_sem
	{
public:
	thr_sem(void)
		#ifndef WIN32
		: sem_	(0)
		#endif // LINUX
		{
		#ifdef WIN32
		sem_ = CreateSemaphore(0, 0, INT_MAX, 0);
		SP_ASSERT(sem_);
		#endif // WIN32
		}
	~thr_sem(void)
		{
		#ifdef WIN32
		if ( sem_ )
			CloseHandle(sem_), sem_ = 0;
		#endif // WIN32
		}

	void notify_one(void)
		{
		#ifdef WIN32
		ReleaseSemaphore(sem_, 1, 0);
		#else // not win32
		sem_.release(1);
		#endif // WIN32
		}

	template<class LOCK>
	bool timed_wait(LOCK & lock, int msec)
		{
		#ifdef WIN32
		lock.unlock();
		DWORD ret = WaitForSingleObject(sem_, msec);
		lock.lock();
		return ret == WAIT_OBJECT_0;
		#else
		lock.unlock();
		ACE_Time_Value tv = ACE_OS::gettimeofday();
		tv.msec((long)tv.msec() + msec);
		int ret = sem_.acquire(tv);
		lock.lock();
		return ret == 0;
		#endif
		}
	
	template<class LOCK>
	bool wait(LOCK & lock)
		{
		#ifdef WIN32
		lock.unlock();
		DWORD ret = WaitForSingleObject(sem_, INFINITE);
		lock.lock();
		return ret == WAIT_OBJECT_0;
		#else // Linux
		lock.unlock();
		int ret = sem_.acquire();
		lock.lock();
		return ret == 0;
		#endif // OS
		}

private:
	#ifdef WIN32
	HANDLE				sem_;
	#else // not win32
	ACE_Thread_Semaphore sem_;
	#endif // 
	};

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
 * @class	thr_ctx
 * 
 * @brief	线程需要的上下文
 * 
 *  在线程最初创建和销毁的时候上下文可以进行创建和销毁。
 *  程序可以用来把自己认为消耗较大的对象也缓存起来，达到性能优化的目的。
 * 
 * @author	jdyu
 * @date	2007-7-17
 * 
 * @see		dstester_thr thr_pool
 */
class thr_ctx_n
	{
public:
	thr_ctx_n(void)
		{
		}
	};

/** 
 * @class	dstester_thr
 * 
 * @brief	一个线程实例基类
 * 
 *  基本的线程操作，建议使用线程池的线程可以从这里派生。
 * 
 * @author	jdyu
 * @date	2007-7-17
 * 
 * @see		thr_ctx thr_pool
 */
template <class thr_context = thr_ctx_n>
class dstester_thr
	{
public:
	virtual int svc(void)
		{
		while ( !to_stop_thr() )
			{
			sleep(100);
			printf("[%d] Hello.\n", thr_self());
			}
		printf("[%d] Quitting!\n", thr_self());
		return 0;
		}

	virtual int stop_thr(void)
		{
		stop_thr_ = true;
		return 0;
		}
	virtual bool to_stop_thr(void) const
		{
		return stop_thr_;
		}

	// 创建和销毁上下文的回调函数
	virtual int context_create(thr_context & ctx)
		{
		return 0;
		}
	virtual int context_destroy(thr_context & ctx)
		{
		return 0;
		}

	dstester_thr(void)
		: stop_thr_	(false)
		{
		}
	virtual ~dstester_thr(void)
		{
		}

// 供派生类使用的方法
public:
	static void sleep(int msec)
		{
		boost::xtime xt;
		boost::xtime_get(&xt, boost::TIME_UTC);
		// boost::xtime_get(&xt, boost::xtime_clock_types::TIME_UTC_); //yanxu2
		xt.nsec += msec * 1000 * 1000;
		boost::thread::sleep(xt);
		}
	static int thr_self(void)
		{
		#if defined (WIN32)
			return ::GetCurrentThreadId();
		#else
			return ::pthread_self();
		#endif /* WIN32 */
		}
	static unsigned int tick_count(void)
		{
		#if defined (WIN32)
			return ::GetTickCount();
		#else
			boost::xtime xt;
			boost::xtime_get(&xt, boost::TIME_UTC);
			unsigned int tick = xt.sec * 1000 + xt.nsec * 1e-6;
			return tick;
		#endif /* WIN32 */
		}

	unsigned int thr_sn(void) const
		{
		return thr_sn_;
		}
	void thr_sn(unsigned int sn)
		{
		thr_sn_ = sn;
		}

protected:
	// stop flag
	bool			stop_thr_;
	// serial no
	unsigned int	thr_sn_;
	};

/** 
 * @class	thr_sink
 * 
 * @brief	线程信息接收器
 * 
 *  用于线程的启动、停止信息回调
 * 
 * @author	jdyu
 * @date	2007-7-18
 * 
 * @see		
 */
template <class thr_impl>
class thr_sink_t
	{
public:
	virtual void on_svc_begin(thr_impl * svc) = 0;
	virtual void on_svc_end  (thr_impl * svc) = 0;
	};

/** 
 * @class	thr_pool
 * 
 * @brief	线程池
 * 
 *  线程管理，创建固定大小的线程池，用户从这里分配线程，
 *  如果超过了线程池的容量，那么会直接从外部创建新的线程。
 * 
 * @author	jdyu
 * @date	2007-7-16
 */
template <class thr_context = thr_ctx_n, class thr_service = dstester_thr<thr_context> >
class thr_pool
	{
public:
	// thread begin/end message sink
	typedef thr_sink_t<thr_service>	thr_sink;
	// max threads
	enum { thr_max_count = 2000 };

	/** 
	 * @brief 	open
	 *  
	 *   初始化，构造线程池空间
	 *  
	 * @author	jdyu
	 * @date	2007-7-19
	 * @return	int	- Return 0 in success, otherwise return error code.
	 * @param	unsigned int max_thrs	- [in] 缓冲的最大线程
	 * @see		
	 */
	int open(unsigned int max_thrs)
		{
		boost::mutex::scoped_lock lock(thr_lock_);

		thr_arr_.reserve(thr_max_count);
		max_thrs_ = max_thrs;
		for ( unsigned int i = 0; i < max_thrs; i++ )
			thr_arr_.push_back(new thr_unit(this, i, true));

		return 0;
		}
	int close(void)
		{
		boost::mutex::scoped_lock lock(thr_lock_);

		thr_stop_ = true;
		for ( int i = 0; i < (int)thr_arr_.size(); i++ )
			{
			thr_unit * tu = thr_arr_[i];
			tu->end();
			delete tu;
			thr_arr_[i] = 0;
			}

		thr_stop_ = false;
		return 0;
		}
	void stop(void)
		{
		boost::mutex::scoped_lock lock(thr_lock_);
		thr_stop_ = true;
		for ( int i = 0; i < (int)thr_arr_.size(); i++ )
			thr_arr_[i]->stop();
		}

	// 分配和释放线程
	int alloc_thr(thr_service * svc)
		{
		boost::mutex::scoped_lock lock(thr_lock_);

		if ( last_free_ != -1
			&& thr_arr_[last_free_]->alloc(svc) == 0 )
			return 0;

		for ( int i = 0; i < (int)thr_arr_.size(); i++ )
			{
			thr_unit & thu = *thr_arr_[i];
			if ( thu.alloc(svc) == 0 )
				return 0;
			}

		if ( thr_arr_.size() >= thr_max_count )
			{
			SP_ASSERT(0 && "Thread cannot alloc!");
			return -1;
			}

		// 现有的池没有空间，创建一个新的
		thr_arr_.push_back(new thr_unit(this, (int)thr_arr_.size()));
		return thr_arr_.back()->alloc(svc);
		}

	int free_thr(thr_service * svc)
		{
		boost::mutex::scoped_lock slock(thr_lock_);

		thr_it pi = thr_arr_.begin();
		for ( ; pi != thr_arr_.end(); pi++)
			{
			thr_unit * tu = *pi;
			if ( *tu == svc )
				break;
			}
		if ( pi != thr_arr_.end() )
			{
			thr_unit * thu = *pi;
			thu->free(svc);
			if ( !thu->pool() )
				{
				thr_arr_.erase(pi);
				delete thu;
				}
			else
				last_free_ = thu->idx();
			return 0;
			}
		SP_ASSERT(0);
		return -1;
		}

	// 添加和删除信息接收器
	int add_sink(thr_sink * sink)
		{
		SP_ASSERT(std::find(sink_list_.begin(), sink_list_.end(), sink) == sink_list_.end());
		sink_list_.push_back(sink);
		return (int)sink_list_.size();
		}
	int remove_sink(thr_sink * sink)
		{
		sink_it pi = std::find(sink_list_.begin(), sink_list_.end(), sink);
		SP_ASSERT(pi != sink_list_.end());
		if ( pi != sink_list_.end() )
			sink_list_.erase(pi);
		return (int)sink_list_.size();
		}

	int on_thr_begin(thr_service * svc)
		{
		sink_it pi = sink_list_.begin();
		for ( ; pi != sink_list_.end(); pi++ )
			(*pi)->on_svc_begin(svc);

		return 0;
		}
	int on_thr_end(thr_service * svc)
		{
		sink_it pi = sink_list_.begin();
		for ( ; pi != sink_list_.end(); pi++ )
			(*pi)->on_svc_end(svc);

		return 0;
		}

	thr_pool(void)
		: max_thrs_		(0)
		, thr_stop_		(false)
		, last_free_	(-1)
		, thr_sn_		(0)
		{
		}

public:
	// 线程状态
	enum thr_state
		{
		thr_init,
		thr_idle,
		thr_ending,
		thr_busy,
		};

	/** 
	 * @class	thr_unit
	 * 
	 * @brief	内部使用的线程对象
	 * 
	 *  代表着线程的一个单位，内部工作单元
	 * 
	 * @author	jdyu
	 * @date	2007-7-19
	 * 
	 * @see		
	 */
	class thr_unit
		{
	public:
		thr_unit(thr_pool * thp, int idx, bool pool = false)
			: thp_	(thp)
			, svc_	(0)
			, sta_	(thr_init)
			, inst_	(0)
			, pool_	(pool)
			, idx_	(idx)
			, ac_	(0)
			, fc_	(0)
			{
			}
		~thr_unit(void)
			{
			}

		// 线程函数对象
		struct thr_inst
			{
			thr_inst(thr_unit * thu)
				: thu_	(thu) { }
			void operator() (void)
				{ thu_->svc(); }
			thr_unit * thu_;
			};

		bool operator == (const thr_service * svc)
			{
			return svc_ == svc;
			}

		thr_state sta(void) const
			{
			boost::mutex::scoped_lock lock(lock_);
			return sta_;
			}
		int idx(void) const
			{
			boost::mutex::scoped_lock lock(lock_);
			return idx_;
			}
		bool pool(void) const
			{
			return pool_;
			}

		// 实际的启动和停止线程
		int begin(thr_service * svc)
			{
			boost::mutex::scoped_lock lock(lock_);
			int ret = do_begin(svc);
			return ret;
			}
		int end(void)
			{
			boost::mutex::scoped_lock lock(lock_);
			if ( inst_ )
				{
				svc_->stop_thr();
				cond_.wait(lock);
				do_end();			
				}
			return 0;
			}

		// 分配和释放池里面的线程
		int alloc(thr_service * svc)
			{
			boost::mutex::scoped_lock lock(lock_);

			// 如果线程没有启动，启动之
			if ( sta_ == thr_init )
				{
				if ( !pool_ )
					do_end();
				do_begin(svc);
				}

			// 如果空闲，唤起
			if ( sta_ == thr_idle )
				{
				sta_ = thr_busy;
				svc_ = svc;
				ac_++;
				svc_->thr_sn(thp_->thr_sn_++);

				lock.unlock();
				cond_.notify_one();
				return 0;
				}
			else if ( sta_ == thr_busy )
				{
				return 1;
				}

			SP_ASSERT(0);
			return -1;
			}
		void free(thr_service * svc)
			{
			boost::mutex::scoped_lock lock(lock_);

			svc_->stop_thr();
			sta_ = thr_idle;
			}
		void stop(void)
			{
			boost::mutex::scoped_lock lock(lock_);
			if ( svc_ )
				svc_->stop_thr();
			}

		void join(void)
			{
			if ( inst_ )
				{
				inst_->join();
				boost::mutex::scoped_lock lock(lock_);
				cond_.wait(lock);
				}
			}

	protected:
		int do_begin(thr_service * svc)
			{
			SP_ASSERT(inst_ == 0);
			svc_ = svc;
			sta_ = thr_idle;
			svc->context_create(ctx_);

			inst_ = new boost::thread(thr_inst(this));
			return 0;
			}
		int do_end(bool wait = false)
			{
			if ( wait )
				join();
			if ( inst_ )
				{
				delete inst_, inst_ = 0;
				svc_ = 0;
				sta_ = thr_init;
				}
			return 0;
			}

		// 内部工作线程
		int svc(void)
			{
			boost::mutex::scoped_lock lock(lock_);

			while ( !thp_->thr_stop_ )
				{
				if ( !cond_.timed_wait(lock, 1000) )
					{
					continue ;
					}

				if ( sta_ == thr_busy )
					{
					thp_->on_thr_begin(svc_);
					lock.unlock();
					svc_->svc();
					lock.lock();
					thp_->on_thr_end(svc_);

					// change state
					sta_ = ( pool_ ? thr_idle : thr_init );

					fc_++;
					if ( !pool_ )
						break ;
					}
				else
					{
					SP_ASSERT(0);
					}
				}

			// 通知线程结束
			if ( svc_ )
				svc_->context_destroy(ctx_);
			cond_.notify_one();
			return 0;
			}

		// 不允许拷贝
	private:
		thr_unit(const thr_unit & thu)
			{
			}
		thr_unit & operator = (const thr_unit & thu)
			{
			thp_  = thu.thp_;
			svc_  = thu.svc_;
			sta_  = thu.sta_;
			pool_ = thu.pool_;
			idx_  = thu.idx_;
			inst_ = thu.inst_;
			ac_   = thu.ac_;
			fc_	  = thu.fc_;
			return *this;
			}

	private:
		// thread context
		thr_pool *			thp_;
		thr_service *		svc_;
		thr_context			ctx_;
		thr_state			sta_;
		bool				pool_;
		int					idx_;

		// boost thread object
		boost::thread *		inst_;
		// 同步锁定
		boost::mutex		lock_;
		// 等待条件事件
		thr_sem				cond_;
		int					ac_;
		int					fc_;
		};
	friend class thr_unit;

private:
	// 同步锁定
	boost::mutex		thr_lock_;
	bool				thr_stop_;

	/// 线程池容量
	unsigned int		max_thrs_;

	/// 线程组
	typedef std::vector<thr_unit*>		thr_arr;
	typedef typename thr_arr::iterator	thr_it;
	thr_arr				thr_arr_;
	int					last_free_;

	// 线程回调信息接收器
	typedef std::list<thr_sink*>		sink_list;
	typedef typename sink_list::iterator sink_it;
	sink_list			sink_list_;

	/// 计数
	unsigned int		thr_sn_;
	};

#endif /* __THR_POOL_H__ */
