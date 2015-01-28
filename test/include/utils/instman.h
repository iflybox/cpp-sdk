/** 
 * @file	instman.h
 * @brief	实例管理器
 * 
 *  通用的句柄管理器，主要实现：
 *	1. 实例的缓存功能，也支持句柄流水号的增加；
 *  2. 支持锁定策略的定制；
 *  3. 句柄管理效率高。
 *  4. 用法举例：typedef sp::inst_mngr<rec_inst, ACE_Recursive_Thread_Mutex, ISR_REC_INST> rc_inst_mngr;
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2008/5/4
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2008/5/4	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __INSTMAN_H__
#define __INSTMAN_H__

#include <map>
#include "autobuf.h"
#include "iFly_Assert.h"

namespace sp {


/** 
 * @class	inst_sink_t
 * @brief	分派句柄回调消息接收类
 * 
 *  用于接收实例管理器的一些消息的对象
 * 
 * @author	jdyu
 * @date	2008/9/10
 */
template<typename INST>
class inst_sink_t
	{
public:
	virtual int on_open(int inst_max, int inst_start)
		{
		return 0;
		}
	virtual int on_close(void)
		{
		return 0;
		}

	virtual int on_inst_alloc(int act_insts)
		{
		return 0;
		}
	virtual int on_inst_release(int act_insts)
		{
		return 0;
		}

	virtual bool on_inst_checkbusy(INST * inst)
		{
		return false;
		}
	};

/** 
 * @class	inst_sink_null
 * @brief	空的消息接收类
 * 
 *  用于接收实例管理器的一些消息的对象
 * 
 * @author	jdyu
 * @date	2008/9/10
 */
template<typename INST>
class inst_sink_null
	{
public:
	int on_open(int inst_max, int inst_start)
		{
		return 0;
		}
	int on_close(void)
		{
		return 0;
		}

	int on_inst_alloc(int act_insts)
		{
		return 0;
		}
	int on_inst_release(int act_insts)
		{
		return 0;
		}

	bool on_check_busy(INST * inst)
		{
		return false;
		}
	};


/** 
 * @class	inst_handle
 * @brief	内置的句柄类型
 * 
 *  提供类型安全保证
 * 
 * @author	jdyu
 * @date	2008/11/5
 */
class inst_handle_t
	{
	void * reserved;
	};
// handle type
typedef inst_handle_t * inst_handle;

/** 
 * @class	inst_mngr
 * @brief	实例管理器
 * 
 *  实例管理器的实现
 *  模板参数解释如下：
 *		INST	分配的实例对象类型，例如srinst
 *		LOCK	实例管理器使用的同步锁定类，必须支持acquire和release接口
 *		HAND	实际返回给用户的句柄类型，例如：ISR_REC_INST
 *		SINK	实例分配和释放的回调类
 *		handle_max 本实例管理对象支持的最大实例数
 *		handle_start 实例的起始流水号，避免加载的几个实例管理返回句柄可能重复的情况
 * 
 * @author	jdyu
 * @date	2008/9/10
 */
template <typename INST, typename LOCK, typename HAND = inst_handle,
	int handle_max = 1024, int handle_start = 0, typename SINK = inst_sink_null<INST> >
class inst_mngr
	{
public:
	int alloc_inst(INST *& inst, HAND & hdl)
		{
		int ret = 0;
		inst = 0;
		hdl  = 0;

		mutex_guard<LOCK> mon(lock_);

		// call sink
		if ( inst_sink_ )
			{
			ret = inst_sink_->on_inst_alloc(busy_count_);
			if ( ret != 0 )
				return ret;
			}

		if ( busy_count_ >= max_license_ )
			return 10010; /* ISR_ERROR_NO_LICENSE */

		// find idle inst
		bool new_inst = false;
		for ( int i = 0; i < handle_max; i++ )
			{
			if ( !insts_[i].busy() )
				{
				ret = insts_[i].alloc(inst, hdl, new_inst);
				if ( ret == 0 )
					busy_count_ ++;

				if ( new_inst )
					insts_set_.insert(std::make_pair(inst, hdl));
				else
					insts_set_[inst] = hdl;

				SP_ASSERT(find_inst_i(hdl)->inst() == inst);
				return ret;
				}
			}

		return 10010; /* ISR_ERROR_NO_LICENSE */
		}
	int release_inst(HAND hdl, bool free_it = false)
		{
		mutex_guard<LOCK> mon(lock_);
		return release_inst_i(hdl, free_it);
		}
	int release_inst(INST * inst, bool free_it = false)
		{
		mutex_guard<LOCK> mon(lock_);
		inst_iter pi = insts_set_.find(inst);
		if ( pi != insts_set_.end() )
			return release_inst_i(pi->second, free_it);
		return 10008; /* ISR_ERROR_INVALID_HANDLE */
		}

	INST * verify_inst(HAND hdl)
		{
		mutex_guard<LOCK> mon(lock_);

		inst_info * ii = find_inst_i(hdl);
        if ( ii )
			{
			return ii->inst();
			}
        return 0;
		}

	INST * check_inst(INST * inst)
		{
		mutex_guard<LOCK> mon(lock_);
		if ( insts_set_.find(inst) != insts_set_.end() )
			return inst;
		return 0;
		}

	HAND find_inst(INST * inst)
		{
		mutex_guard<LOCK> mon(lock_);
		inst_iter pi = insts_set_.find(inst);
		if ( pi != insts_set_.end() )
			return pi->second;
		return 0;
		}

	// 活动数量
	unsigned int active_insts(void)
		{
		mutex_guard<LOCK> mon(lock_);
		return busy_count_;
		}

	// 每个实例需要实现一个查找实例的方法，用于用其他的token来寻找实例
	template <typename T>
	INST * find_inst(const T & token)
		{
		mutex_guard<LOCK> mon(lock_);

		for ( int i = 0; i < handle_max; i++ )
			{
			inst_info & it = insts_[i];
			if ( it.inst() == 0 )
				continue ;
			if ( it.inst()->find_inst(token) )
				return it.inst();
			}
		return NULL;
		}

	// 初始化和逆初始化
	int open(unsigned int max_license = handle_max, bool inst_cache = true, SINK * alloc_sink = 0)
		{
		SP_ASSERT(handle_max  <= inst_info::max_count);
		SP_ASSERT(handle_max  >= 1);
		SP_ASSERT(max_license <= handle_max);
		SP_ASSERT(handle_start >= 0);

		if ( handle_max > inst_info::max_count )
			return -1;
		if ( handle_max < 1 )
			return -1;
		if ( handle_start < 0 )
			return -1;

		if ( max_license < handle_max )
			max_license_ = max_license;
		inst_nocache_ = !inst_cache;
		inst_sink_ = alloc_sink;

		int ret = 0;
		if ( inst_sink_ )
			ret = inst_sink_->on_open(handle_max, handle_start);
		return ret;
		}
	int close(void)
		{
		int ret = 0;
		if ( inst_sink_ )
			ret = inst_sink_->on_close();
		return ret;
		}

	LOCK & mutex(void)
		{
		return lock_;
		}
	SINK * sink(void)
		{
		return inst_sink_;
		}

	// 设置指定的实例指针，用于外部分配
	int set_inst(int index, INST * inst)
		{
		SP_ASSERT(index >= 0 && index < handle_max);
		mutex_guard<LOCK> mon(lock_);
		if ( index >= 0 && index < handle_max )
			{
			insts_[index].inst(inst);
			return 0;
			}
		return -1;
		}

public:
	// singleton access enable
	static inst_mngr & instance(void)
		{
		static inst_mngr the_iman;
		return the_iman;
		}

	inst_mngr(void)
		: busy_count_	(0)
		, max_license_	(handle_max)
		, inst_nocache_	(false)
		, inst_sink_	(0)
		{
		for ( int i = 0; i < handle_max; i++ )
			{
			insts_[i].index(i);
			insts_[i].sn(handle_start);
			}
		}
	virtual ~inst_mngr(void)
		{
		SP_ASSERT(busy_count_ == 0 && "Some insts not released!");
		}

protected:
	// inst info
	template <typename INST_T, typename HAND_T>
	class inst_info_t
		{
	public:
		inst_info_t (int index = 0)
			: inst_	(0)
			, busy_	(false)
			{
			u_.s_.idx_ = index;
			u_.s_.sn_  = handle_start;
			}
		~inst_info_t(void)
			{
			if ( inst_ )
				delete inst_;
			}

		int alloc(INST_T *& inst, HAND_T & id, bool & new_inst)
			{
			new_inst = false;
			if ( inst_ == 0 )
				{
				#ifdef WIN32
				inst_ = new __declspec(align(16)) INST_T();
				#else // LINUX
				inst_ = new INST_T();
				#endif // OS
				new_inst = true;
				}
			if ( inst_ == 0 )
				return -1;

			// serial no
			u_.s_.sn_++;
			if ( u_.s_.sn_ == 0 )
				u_.s_.sn_++;

			id = u_.id_;
			inst = inst_;

			busy_ = true;
			return 0;
			}
		int release(bool free_inst)
			{
			if ( free_inst )
				{
				delete inst_;
				inst_ = 0;
				}
			busy_ = false;
			return 0;
			}

		bool busy(void) const
			{
			return busy_;
			}
		INST * inst(void) const
			{
			return inst_;
			}
		void inst(INST * inst)
			{
			SP_ASSERT(inst_ == 0);
			inst_ = inst;
			}
		
		HAND_T id(void) const
			{
			return u_.id_;
			}
		void id(HAND_T hdl)
			{
			u_.id_ = hdl;
			}

		int index(void) const
			{
			return u_.s_.idx_;
			}
		void index(int idx)
			{
			u_.s_.idx_ = idx;
			}

		int sn(void) const
			{
			return u_.s_.sn_;
			}
		void sn(int nsn)
			{
			u_.s_.sn_ = nsn;
			}

		// handle to index and sn
		#ifdef WIN32
		typedef __int64 int64;
		#else // LINUX
		typedef int int64;
		#endif // OS

		static int to_index(HAND_T id)
			{
			int idx = (unsigned int)(((unsigned int)(int64)id) >> 20);
			return idx;
			}
		static int to_sn(HAND_T id)
			{
			int sn = (unsigned int)((unsigned int)(int64)(id) & 0x000FFFFF);
			SP_ASSERT(sn >= 0);
			return sn;
			}

		// 最大允许的句柄数量
		enum { max_count = 4096 };

	private:
		// inst handle
		INST_T *	inst_;
		bool		busy_;
		// id info
		union
			{
			HAND_T			 id_;
			struct
				{
				#if defined (__sun) || defined(_AIX)
				unsigned int idx_ : 12;
				unsigned int sn_  : 20;
				#else //os
				unsigned int sn_  : 20;
				unsigned int idx_ : 12;	
				#endif // sun OS or aix os
				} s_;
			} u_;
		};

	// 所有实例列表
	typedef inst_info_t<INST, HAND>		inst_info;
	typedef std::map<INST*, HAND>		inst_set;
	typedef typename inst_set::iterator	inst_iter;
	inst_info			insts_[handle_max];
	inst_set			insts_set_;
	LOCK				lock_;

	// 计数
	unsigned int		busy_count_;
	unsigned int		max_license_;
	bool				inst_nocache_;

	// 消息接收器
	SINK *				inst_sink_;

protected:
	// 内部的查找实例函数
	inst_info * find_inst_i(HAND hdl)
		{
		int index = inst_info::to_index(hdl);
		if ( index >= 0 && index < inst_info::max_count )
			{
			inst_info & ii = insts_[index];
			if ( ii.id() == hdl )
				return &ii;
			}
		SP_ASSERT("Instance not found!" && 0);
		return 0;
		}
	// 实际的释放
	int release_inst_i(HAND hdl, bool free_it = false)
		{
		int ret = 0;

		// call sink
		if ( inst_sink_ )
			inst_sink_->on_inst_release(busy_count_);

		// find inst to free
		inst_info * ii = find_inst_i(hdl);
		if ( ii && ii->busy() )
			{
			bool free_inst = inst_nocache_;
			if ( free_it )
				free_inst = true;
			ret = ii->release(free_inst);
			SP_ASSERT(busy_count_ > 0);
			if ( ret == 0 )
				busy_count_ --;
			if ( inst_nocache_ )
				insts_set_.erase(ii->inst());
			return ret;
			}

		return 10008; /* ISR_ERROR_INVALID_HANDLE */
		}
	};

} /* sp */

#endif /* __INSTMAN_H__ */
