/** 
 * @file	Cyclic_Queue.h
 * @brief	循环队列模板类的实现
 * 
 * detail...
 * 
 * @author	hjye
 * @version	1.0
 * @date	2005年6月22日
 * 
 * @see		
 * 
 * @par 版本记录：
 * <table border=1>
 *  <tr> <th>版本	<th>日期			<th>作者	<th>备注 </tr>
 *  <tr> <td>1.0	<td>2005年6月22日	<td>hjye	<td>创建 </tr>
 * </table>
 */

#ifndef CYCLIC_QUEUE_H
#define CYCLIC_QUEUE_H

/** 
 * @class	Cyclic_Queue
 * 
 * @brief	循环队列模板类
 * 
 * detail...
 * 
 * @author	hjye
 * @date	2005年6月22日
 * 
 * @see		
 * 
 * @par 备注：
 * 
 */
template<class _Ty>
class Cyclic_Queue
{
public:
	/// Constructor the circular queue.
	Cyclic_Queue (unsigned long capacity);

	/// Destructor the circular queue.
	~Cyclic_Queue (void);

	//////////////////////////////////////////////////////////////////////////
	/// 标准循环队列操作

	/// 入队
	bool en_queue (_Ty * elem);

	/// 出队
	_Ty * dl_queue (void);

	/// 取队列头
	_Ty * gethead_queue (void);

	/// 取队列尾
	_Ty * getrear_queue (void);

	/// 队列满
	bool full_queue (void);

	/// 队列空
	bool empty_queue (void);

	/// 置为空队列
	void clear_queue (void);

	/// 队列元素数
	unsigned long size_queue (void);

	//////////////////////////////////////////////////////////////////////////
	/// 增强循环队列操作

	/// 取队列元素
	_Ty * getelem_queue (unsigned long index)
		{
			// 取队列元素
			if ( index < capacity_ ) return &queue_ [ index ];
			else return NULL;
		}

	/// 队列头尾位置
	unsigned long front_pos (void)
		{ return front_; }
	unsigned long rear_pos (void)
		{ return rear_; }

	/// 取给定位置的前/后一个位置
	unsigned long prev_pos (unsigned long pos)
		{
			if ( pos > 0 ) return pos - 1;
			else return capacity_ - 1;
		}
	unsigned long post_pos (unsigned long pos)
		{
			if ( pos >= capacity_ - 1 ) return 0;
			else return pos + 1;
		}

	/// 取他队列容量
	unsigned long get_capacity (void)
		{ return capacity_; }

	/// 取队列状态描述串
	const char * status_desc (void)
		{
			static char buf [ 256 ];
			sprintf( buf, "size:%5d [front:%5d->rear:%5d]", size_queue(), front_, rear_ );
			return buf;
		}

protected:
	_Ty				*queue_;
	unsigned long	capacity_;
	unsigned long	front_;
	unsigned long	rear_;
};

template<class _Ty>
Cyclic_Queue<_Ty>::Cyclic_Queue (unsigned long capacity)
	: capacity_ (capacity)
	, front_( 0 )
	, rear_( 0 )
{
	queue_ = new _Ty [ capacity ];
}

template<class _Ty>
Cyclic_Queue<_Ty>::~Cyclic_Queue (void)
{
	delete [ ] queue_;
}

template<class _Ty>
bool Cyclic_Queue<_Ty>::en_queue (_Ty * elem)
{
	// 入队
	if ( ( rear_ + 1 ) % capacity_ == front_ )
		return false;	// 队列满
	else
	{
		rear_ = ( rear_ + 1 ) % capacity_;
		if ( elem )
			queue_ [ rear_ ] = *elem;
		else
		{
			int nSize = sizeof (queue_ [ rear_ ]);
			memset ( &queue_ [ rear_ ], 0, sizeof (queue_ [ rear_ ]) );
		}
		return true;
	}
}

template<class _Ty>
_Ty * Cyclic_Queue<_Ty>::dl_queue (void)
{
	// 出队
	if ( front_ == rear_ )
		return NULL;		// 队列空
	else
	{
		front_ = ( front_ + 1 ) % capacity_;
		return &queue_ [ front_ ];
	}
}

template<class _Ty>
_Ty * Cyclic_Queue<_Ty>::gethead_queue (void)
{
	// 取队列头
	if ( front_ == rear_ )
		return NULL;
	else
		return &queue_ [ ( front_ + 1 ) % capacity_ ];
}

template<class _Ty>
_Ty * Cyclic_Queue<_Ty>::getrear_queue (void)
{
	// 取队列尾
	if ( rear_ == front_ )
		return NULL;
	else
		return &queue_ [ rear_ ];
}

template<class _Ty>
bool Cyclic_Queue<_Ty>::full_queue (void)
{
	// 队列满?
	return ( rear_ + 1 ) % capacity_ == front_;
}

template<class _Ty>
bool Cyclic_Queue<_Ty>::empty_queue (void)
{
	// 队列空
	return front_ == rear_;
}

template<class _Ty>
void Cyclic_Queue<_Ty>::clear_queue (void)
{
	// 置为空队列
	front_ = rear_;
}

template<class _Ty>
unsigned long Cyclic_Queue<_Ty>::size_queue (void)
{
	// 队列元素数
	return ( rear_ - front_ + capacity_ ) % capacity_;
}

#endif	// CYCLIC_QUEUE_H
