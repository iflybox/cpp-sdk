/** 
 * @file	log_asyn.h
 * @brief	�첽Log_IO���ʵ��
 * 
 *  Ϊ�˽���Logд�����������Ӱ�죬ʵ���첽���ļ�д��IO�ӿڣ���Log��ܵ��á�
 *
 *  ��ʹ���첽��־֮ǰ������Ҫ�ȶ������������ࣺ
 *  1. �¼��࣬����֪ͨ�̳߳ع������μ����������thr_event��
 *  2. �߳��࣬���ڲ���ϵͳ�Ĺ�ϵ����ͬ�Ĳ���ϵͳ��Ҫʵ�ֲ�ͬ���߳����䣨�μ����������as_thr_adpt)
 *
 *  ����ǣ�浽�첽�̳߳ص�������ֹͣ�����ù��̱�ͬ����־��΢����һЩ���������£�
 *	// 1. ʹ���첽��־֮ǰ��Ҫע���ȴ����̳߳�
 *	log_pools::instance().open(2);
 *
 *	// 2. Ȼ�����־�࣬�򿪵�ʱ����Զ�ע�ᵽ�̳߳���
 *	asLogger::open(asLogger_Cfg("as.log", "aslogging"), "aslog.cfg");
 *
 *	// 3. ���Ĵ󵨼�¼��־��
 *	asLogger::instance()->log_crit("Hello world!");
 *
 *	// 4. �ر���־��ע������Ĺر����첽�ģ��رպ���־�ļ����ڼ���д��
 *	asLogger::close();
 *
 *	// 5. ��������־�رպ����ֹͣ�̳߳�
 *	// ���ڿ��ܻ�������־û��д�꣬����̳߳ر���ȴ���־д��
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
 * @brief	�첽�ļ��ӿ�
 * 
 *  �����̳߳�ʵ���첽IO�����������Ŀǰ��������������������������ļ��������̳߳ص���
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
	 *  �ı�������Ʒ�ʽд��
	 */
	// խ�ַ���¼�������ݽ���ƴ���Ż�
	int write(const char* msg);
	int write(const wchar_t * msg);
	int write(const void * data, int size);

	/** 
	 *  ����ļ�
	 */
	int empty(void)
		{
		// ���
		sp::sguard<sp::smutex> gd(acts_lock_);
		pending_acts_.push(act_empty);
		return 0;
		}

	/** 
	 *  �ӻ���ˢ����־���첽��־�������ȡ��
	 */
	void flush(void)
		{
		// ˢ���ļ�����ʱ��ˢ��
		// sp::sguard<sp::smutex> gd(acts_lock_);
		// pending_acts_.push(act_flush);
		}

	/** 
	 *  ��õ�ǰ�ߴ�
	 */
	size_t size(void)
		{
		sp::sguard<sp::smutex> gd(acts_lock_);
		return log_size_;
		}

	/** 
	 *  ���ݾɵ��ļ�
	 */
	int backup(const char* bname /* = 0 */)
		{
		// ����/ɾ�����ļ�
		sp::sguard<sp::smutex> gd(acts_lock_);
		pending_acts_.push(act_backup);
		log_size_ = 0;
		if ( bname )
			backup_bname_ = bname;
		return 0;
		}

public:
	// ���̳߳ص��õ�ʵ�ʸɻ�ĺ���
	int do_work(int & acts_left, bool & all_done);
	
	// ����ȫ��Log��delete����
	bool donot_delete(void)
		{
		return true;
		}

	// �Ƿ�������
	bool work_done(void)
		{
		sp::sguard<sp::smutex> gd(acts_lock_);
		return pending_acts_.empty();
		}

private:
	// ��������
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
	// �ڲ��������Ϣ�������ڱ��̳߳ص���
	std::queue<std::string>		logging_strs_;
	std::queue<std::wstring>	logging_wstrs_;
	std::queue<std::string>		logging_bins_;
	std::queue<action_type>		pending_acts_;
	// ���������������
	sp::smutex					acts_lock_;
	// ���ݵĲ���
	std::string					backup_bname_;
	// log size
	size_t						log_size_;
	}; // log_io_fasyn

/** 
 * @class	log_event
 * @brief	�¼����࣬�û���ʹ��ʱ��Ҫ����һ����ͬ�ӿڵ���
 * 
 *  ��־�߳�֮����Ҫ����֪ͨ�������Ҫ����¼���ͬ��
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
 * @brief	IO������
 * 
 *  ������������е��첽IO�࣬���̳߳ص���
 *  IO���ڴ򿪵�ʱ����Լ�ע�ᵽ�˹������У��ر�ʱ�Ӵ˹�������ע��
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
	// �Ե�����ʵ��
	static log_io_mngr & instance(void)
		{
		static log_io_mngr the_iom_;
		return the_iom_;
		}

	// ע��һ���첽IO�ൽ������
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
	// IO��ر�ǰ����Ҫ�ӹ�������ע������Ҫ��֤IO��δд������ȫ��д��
	int io_unbind(log_io_fasyn* io_asyn)
		{
		sp::sguard<sp::smutex> gd(log_lock_);
		
		SP_ASSERT(std::find(log_ios_.begin(), log_ios_.end(), io_asyn) != log_ios_.end());
		log_pi pi = std::find(log_ios_.begin(), log_ios_.end(), io_asyn);
		if ( pi != log_ios_.end() )
			{
			// �������д�ֱ꣬���ͷţ�������Ϊ���ͷ�
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

	// ����IOʵ��������д����������̳߳ص���
	int io_work(int id, bool & all_done)
		{
		int acts_left = 0;
		all_done = false;

		// ִ�о�����ļ�����
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

		// �Ƿ�����ͷ���
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
	 * �ڲ����ݷ���
	 */

	// ע���IO����
	int size(void)
		{
		return (int) log_ios_.size();
		}
	// ����ָ��λ�õ�IO��
	log_io_fasyn * io(int id)
		{
		return log_ios_[id].io_;
		}

	// ����ȫ�������¼�
	void work_event(std::vector<log_event*> & evts)
		{
		thr_evts_ = evts;
		}
	// ֪ͨ�̳߳ظɻ�
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
	// ��־���״̬��æ������
	enum log_status
		{
		log_invalid,
		log_idle,
		log_busy,
		log_unbinding,
		};
	// ÿ��IO���ڵ�״̬
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

		// ֪ͨ�̳߳ظɻ�
		void work_signal(void)
			{
			if ( evt_ != 0 )
				evt_->signal();
			}
		};

	// �첽�ļ���־����
	typedef std::vector<log_info>	log_list;
	typedef log_list::iterator		log_pi;
	log_list	log_ios_;
	sp::smutex	log_lock_;

	// ���еĹ����¼�
	std::vector<log_event*>			thr_evts_;
	};

/*
 * log_io_fasyn�ļ�����������
 */

// �첽IO��Ĵ򿪺�������ȷ��첽������ע��Ĺ���
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

// �첽iO�رգ��ӹ�������ע����ע���������첽��
inline int log_io_fasyn::close(void)
	{
	if ( parent::handle() != NULL )
		{
		sp::sguard<sp::smutex> gd(acts_lock_);
		pending_acts_.push(act_close);
		}
	return 0;
	}

// խ�ַ���¼�������ݽ���ƴ���Ż�
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
// ���ַ���¼�������ݽ���ƴ���Ż�
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
// �����ư汾
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

// ���̳߳ص��õĺ���
inline int log_io_fasyn::do_work(int & acts_left, bool & all_done)
	{
	int ret = 0;
	action_type act = act_none;
	std::string str;
	std::wstring wstr;

	// ���ظ��ⲿ������
	acts_left = 0;
	all_done = false;

	// ��ȡ���ݣ�ʹ��������
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

	// ִ���ļ�������ʹ���ļ���
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
 * @brief	IO��־�̳߳���
 * 
 *  �ṩһ�����Խ����̺߳��¼���������̳߳أ������̵߳�ʵ��û�б�׼֧�֣�����û���ʹ��ʱ����ʹ�ò�ͬ���̺߳��¼���
 *  ������Բο�����ע�����������
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
	// ����ģʽ
	static log_thr_pool & instance(void)
		{
		static log_thr_pool the_thrp_;
		return the_thrp_;
		}
	// ��ʼ���������������������߳�
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
	// �̺߳�����
	virtual int svc(int idx)
		{
		int ret = 0;
		log_io_mngr & iom = log_io_mngr::instance();
		bool all_done = true;

		// �¼�
		log_event * work_event = thr_evts_[idx];
		while ( 1 )
			{
			bool all_done = true;
			int pending_work = 0;
			for ( int i = idx; i < iom.size(); i += (int)thr_evts_.size() )
				{
				// ����ʹ��һ����ִ��
				bool io_done = false;
				pending_work += iom.io_work(i, io_done);
				if ( !io_done )
					all_done = false;
				}

			// �Ƿ�ֹͣ
			if ( thr_stopping_ )
				{
				if ( all_done )
					break;
				}

			// ���û�����ݣ���Ҫ���ߵȴ�����
			if ( pending_work == 0 )
				work_event->wait(1000);
			}
		return 0;
		}

private:
	// stop flag
	bool					thr_stopping_;
	// ���еĹ����¼�
	std::vector<log_event*>	thr_evts_;
	// �߳�����
	int						thr_count_;
	};

// **********************************************
// �ṩ�¼����߳�������������룬�û����Բο�
// **********************************************

#if 0
/** 
 * @class	thr_event
 * 
 * @brief	�¼���
 * 
 *  Windows��ʹ��Eventʵ�֣�linux�½���ace
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
 * @brief	�߳�������
 * 
 *  �߳������࣬�˴�ʹ��boostʵ�֣�ace������ͬ������
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

	// �̺߳�������
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

// �����첽��־�̳߳�
typedef log_thr_pool<as_thr_adpt, thr_event> log_pools;

// ʹ���첽IO����־�ඨ��
typedef Log_Impl_T<log_io_fasyn, Log_Thread_Mutex> log_impl;
// ���߿�������
LOG_DECL_SINGLETON_EX(log_io_fasyn, Log_Thread_Mutex, Log_Cfg_Heap_Reg, asLogger)

// ����ʹ���첽��־ʱ����������д����
/*
int main(int argc, char* argv[])
	{
	// 1. ʹ���첽��־֮ǰ��Ҫע���ȴ����̳߳�
	log_pools::instance().open(2);

	// 2. Ȼ�����־�࣬�򿪵�ʱ����Զ�ע�ᵽ�̳߳���
	asLogger::open(asLogger_Cfg("as.log", "aslogging"), "aslog.cfg");
	log_impl log1, log2, log3, log4;
	log_cfg  cfg1("a1.log");
	log_cfg  cfg2("a2.log");
	log_cfg  cfg3("a3.log");
	log_cfg  cfg4("a4.log");

	log1.open(&cfg1);
	log2.open(&cfg2);
	log3.open(&cfg3);

	// 3. ���Ĵ󵨼�¼��־��
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

	// 4. �ر���־��ע������Ĺر����첽�ģ��رպ���־�ļ����ڼ���д��
	log1.close();
	log2.close();
	log3.close();
	asLogger::close();

	// 5. ��������־�رպ����ֹͣ�̳߳�
	// ���ڿ��ܻ�������־û��д�꣬����̳߳ر���ȴ���־д��
	log_pools::instance().close();
	}
 */

#endif // 0

#endif /* __LOG_ASYN_H__ */
