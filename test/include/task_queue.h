/** 
 * @file	Task_Queue.h
 * @brief	
 * 
 *  简单的任务队列,解决ACE_Message_Queue会先将消息的内容拷贝
 *  到一个ACE_Message_Block中再加入Queue的问题。
 * 
 * @author	xglu
 * @version	1.0
 * @date	2010/12/3
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2010/12/3	<td>xglu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __TASK_QUEUE_H__
#define __TASK_QUEUE_H__

#include <list>
template <class _MyTask>
class Task_Queue
	: public std::list<_MyTask *>
{
public:
	Task_Queue(size_t queue_size = 1024)
		: queue_size_ (queue_size)
		, not_empty_cond_(lock_)
		, not_full_cond_(lock_)
		, stop_ (false)
		{
		}
	~Task_Queue()
		{
		}

	int putq(_MyTask * task)
		{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, lock_, -1);
		while ( is_full() )
			{
			not_full_cond_.wait();
			if ( stop_ )
				return size();
			}
		push_back(task);
		not_empty_cond_.signal();
		return size();
		}

	_MyTask* getq()
		{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, lock_, 0);
		while ( empty() )
			{
			not_empty_cond_.wait();
			if ( stop_ )
				return 0;
			}

		_MyTask* task = front();
		pop_front();
		not_full_cond_.signal();
		return task;
		}

	size_t task_count() const
		{
		ACE_GUARD_RETURN(ACE_Thread_Mutex, guard, lock_, 0);
		return size();
		}

	void wake_up()
		{
		stop_ = true;
		not_empty_cond_.broadcast();
		not_full_cond_.broadcast();
		}

protected:
	bool is_full()
		{
		return queue_size_ <= size();
		}

private:
	ACE_Thread_Mutex				lock_;
	ACE_Condition_Thread_Mutex		not_empty_cond_;
	ACE_Condition_Thread_Mutex		not_full_cond_;
	size_t							queue_size_;
	bool							stop_;
};
#endif /* __MSS_TASK_QUEUE_H__ */
