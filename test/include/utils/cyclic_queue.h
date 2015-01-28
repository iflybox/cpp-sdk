/** 
 * @file	Cyclic_Queue.h
 * @brief	ѭ������ģ�����ʵ��
 * 
 * detail...
 * 
 * @author	hjye
 * @version	1.0
 * @date	2005��6��22��
 * 
 * @see		
 * 
 * @par �汾��¼��
 * <table border=1>
 *  <tr> <th>�汾	<th>����			<th>����	<th>��ע </tr>
 *  <tr> <td>1.0	<td>2005��6��22��	<td>hjye	<td>���� </tr>
 * </table>
 */

#ifndef CYCLIC_QUEUE_H
#define CYCLIC_QUEUE_H

/** 
 * @class	Cyclic_Queue
 * 
 * @brief	ѭ������ģ����
 * 
 * detail...
 * 
 * @author	hjye
 * @date	2005��6��22��
 * 
 * @see		
 * 
 * @par ��ע��
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
	/// ��׼ѭ�����в���

	/// ���
	bool en_queue (_Ty * elem);

	/// ����
	_Ty * dl_queue (void);

	/// ȡ����ͷ
	_Ty * gethead_queue (void);

	/// ȡ����β
	_Ty * getrear_queue (void);

	/// ������
	bool full_queue (void);

	/// ���п�
	bool empty_queue (void);

	/// ��Ϊ�ն���
	void clear_queue (void);

	/// ����Ԫ����
	unsigned long size_queue (void);

	//////////////////////////////////////////////////////////////////////////
	/// ��ǿѭ�����в���

	/// ȡ����Ԫ��
	_Ty * getelem_queue (unsigned long index)
		{
			// ȡ����Ԫ��
			if ( index < capacity_ ) return &queue_ [ index ];
			else return NULL;
		}

	/// ����ͷβλ��
	unsigned long front_pos (void)
		{ return front_; }
	unsigned long rear_pos (void)
		{ return rear_; }

	/// ȡ����λ�õ�ǰ/��һ��λ��
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

	/// ȡ����������
	unsigned long get_capacity (void)
		{ return capacity_; }

	/// ȡ����״̬������
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
	// ���
	if ( ( rear_ + 1 ) % capacity_ == front_ )
		return false;	// ������
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
	// ����
	if ( front_ == rear_ )
		return NULL;		// ���п�
	else
	{
		front_ = ( front_ + 1 ) % capacity_;
		return &queue_ [ front_ ];
	}
}

template<class _Ty>
_Ty * Cyclic_Queue<_Ty>::gethead_queue (void)
{
	// ȡ����ͷ
	if ( front_ == rear_ )
		return NULL;
	else
		return &queue_ [ ( front_ + 1 ) % capacity_ ];
}

template<class _Ty>
_Ty * Cyclic_Queue<_Ty>::getrear_queue (void)
{
	// ȡ����β
	if ( rear_ == front_ )
		return NULL;
	else
		return &queue_ [ rear_ ];
}

template<class _Ty>
bool Cyclic_Queue<_Ty>::full_queue (void)
{
	// ������?
	return ( rear_ + 1 ) % capacity_ == front_;
}

template<class _Ty>
bool Cyclic_Queue<_Ty>::empty_queue (void)
{
	// ���п�
	return front_ == rear_;
}

template<class _Ty>
void Cyclic_Queue<_Ty>::clear_queue (void)
{
	// ��Ϊ�ն���
	front_ = rear_;
}

template<class _Ty>
unsigned long Cyclic_Queue<_Ty>::size_queue (void)
{
	// ����Ԫ����
	return ( rear_ - front_ + capacity_ ) % capacity_;
}

#endif	// CYCLIC_QUEUE_H
