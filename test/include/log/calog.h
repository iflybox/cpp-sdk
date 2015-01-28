/** 
 * @file	calog.h
 * @brief	呼叫日志接口定义
 * 
 *  Callog为每个实例提供中间结果的保存，便于效果调试和优化。
 *  其中实现了一个全局的calog_mngr_i用于初始化calog配置和其他信息；
 *  calog_i接口每一个实例都可以拥有一个实例记录信息。
 *  用法举例：
 *
 *		cl::rec_calog_i * clg = 0;
 *		scom::acquire_if(module_of_calog(), &clg);
 *		cl::calog_message(clg, L"chanid",
 *			cl::calog_attrs(L"attr1", L"val1")(L"attr2", 25)(L"attr3", 1e5),
 *			L"This is a %d test message.\n", argc);
 *		scom::dismiss_if(module_of_calog(), clg);
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
#ifndef __CALLLOG_H__
#define __CALLLOG_H__

#include "isr_types.h"
#include "isr_rec.h"
#include "isr_ep.h"
#include "base/srbase.h"
#include "iFly_Assert.h"
#include "utils/sutils.h"

#include <vector>
#include <list>
#include <sstream>
#include <stdarg.h>

/** 
 * @class	cl
 * 
 * @brief	call log的名字空间
 * 
 *  定义所有与callog相关的接口和实现
 * 
 * @author	jdyu
 * @date	2007-7-17
 * 
 * @see		
 */
namespace cl
{

/// 没有创建session时的通道名
const wchar_t pre_session_chan[]= L"PRELOAD_SESSION";

/// 通用的calog事件名称
enum events_global
	{
	ISRevnt,		/* event */
	ISRtnat,		/* tenant */
	ISRwvfm,		/* waveform */
	ISRclnd,		/* call end */
	ISRclst,		/* call start */
	ISRendp,		/* end pointer */
	ISRgrld,		/* grammar load */
	ISRiffi,		/* fetched grammar file */
	ISRrcnd,		/* recognition end */
	ISRrcst,		/* recognition start */
	ISRrrcn,		/* rejected chunk */
	ISRrrst,		/* recognizer restart */
	ISRrslt,		/* recognizer result */
	ISRstop,		/* recognizer stop event */
	ISRrcpa,		/* rec parameters */
	ISReppa,		/* ep parameters */
	ISRacsv,		/* acoustic accumulator save event */
	ISRacum,		/* acoustic accumulation event */
	ISRaupd,		/* acoustic model update event */
	ISRcach,		/* caching event */
	ISRlise,		/* license end */
	ISRliss,		/* license start */
	ISRlock,		/* license lock */
	ISRunlo,		/* license unlock */
	ISRusre,		/* user defined event */
	ISRemax,		/* Max event */
	};
const wchar_t * const event_names_global[] =
	{
	L"ISRevnt",		/* event */
	L"ISRtnat",		/* tenant */
	L"ISRwvfm",		/* waveform */
	L"ISRclnd",		/* call end */
	L"ISRclst",		/* call start */
	L"ISRendp",		/* end pointer */
	L"ISRgrld",		/* grammar load */
	L"ISRiffi",		/* fetched grammar file */
	L"ISRrcnd",		/* recognition end */
	L"ISRrcst",		/* recognition start */
	L"ISRrrcn",		/* rejected chunk */
	L"ISRrrst",		/* recognizer restart */
	L"ISRrslt",		/* recognizer result */
	L"ISRstop",		/* recognizer stop event */
	L"ISRrcpa",		/* rec parameters */
	L"ISReppa",		/* ep parameters */
	L"ISRacsv",		/* acoustic accumulator save event */
	L"ISRacum",		/* acoustic accumulation event */
	L"ISRaupd",		/* acoustic model update event */
	L"ISRcach",		/* caching event */
	L"ISRlise",		/* license end */
	L"ISRliss",		/* license start */
	L"ISRlock",		/* license lock */
	L"ISRunlo",		/* license unlock */
	L"ISRusre",		/* user defined event */
	};

/// 获得事件字符串
inline
const wchar_t * get_event_str(events_global evnt)
	{
	SP_ASSERT(evnt < ISRemax);
	if ( evnt >= ISRemax )
		return 0;
	return event_names_global[evnt];
	}

/** 
 * @class	attr_val
 * 
 * @brief	calllog的属性对
 * 
 *  这些属性对记录在日志里面就形成类似
 *  attr1=val1|attr2=val2|attr3=val3的形式。
 * 
 * @author	jdyu
 * @date	2007-7-17
 * 
 * @see		
 */
struct attr_val
	{
	const wchar_t * attr;
	const wchar_t * val;
	};

/** 
 * @class	calog_i
 * 
 * @brief	一个callog实例
 * 
 *  每个实例附加在一个session
 * 
 * @author	jdyu
 * @date	2007-7-17
 * 
 * @see		
 */
class calog_i
	: public sr::if_base
	{
public:
	/** 
	 * @brief 	write_waveform
	 *  
	 *  记录一段wave数据
	 *  
	 * @author	jdyu
	 * @date	2007-7-20
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @param	const void * wave		- [in] Waveform data
	 * @param	unsigned int bytes		- [in] Size in bytes
	 * @param	int sample_status		- [in] Audio sample status ISR_AUDIO_SAMPLE_FIRST/ISR_AUDIO_SAMPLE_CONTINUE/ISR_AUDIO_SAMPLE_LAST
	 * @param	int adf					- [in] Audio format id
	 */
	virtual int log_waveform(const void * wave, unsigned int bytes, int sample_status, const wchar_t * audio_str = 0) = 0;

	/** 
	 * @brief 	log_session_message
	 *  
	 *  记录一条session的信息
	 *  
	 * @author	jdyu
	 * @date	2007-7-20
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @param	const wchar_t * event	- [in] 事件名称
	 * @param	const attr_val attrs[]	- [in] 属性值对的数组
	 * @param	int count				- [in] 属性对的数量
	 * @param	const wchar_t * msg		- [in] 信息内容
	 * @see		
	 */
	virtual int log_session_message(const wchar_t * event, const attr_val attrs[], int count, const wchar_t * msg) = 0;

	/** 
	 * @brief 	log_preload_message
	 *  
	 *  记录一条非session信息，例如还没有创建session的时候或者销毁session和其它情况
	 *  
	 * @author	jdyu
	 * @date	2007-7-25
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @see		log_session_message
	 */
	virtual int log_preload_message(const wchar_t * event, const attr_val attrs[], int count, const wchar_t * msg) = 0;

	/** 
	 * @brief 	wchar_t * cur_dir
	 *  
	 *  当前的calog目录
	 *  
	 * @author	jdyu
	 * @date	2007-7-20
	 * @return	const wchar_t * 当前目录路径
	 * @see		
	 */
	virtual const char * cur_dir(void) const = 0;

	/** 
	 * @brief 	wchar_t * base_path
	 *  
	 *   calog的根目录
	 *  
	 * @author	jdyu
	 * @date	2007-7-20
	 * @return	const wchar_t * 根目录路径
	 * @see		
	 */
	virtual const char * base_path(void) const = 0;

	/** 
	 * @fn		logging_enabled
	 * @brief	是否支持calog
	 * 
	 *  判断是否禁用calog
	 * 
	 * @author	jdyu
	 * @date	2007-9-14
	 * @see		
	 */
	virtual bool logging_enabled(void) const = 0;
	virtual void logging_enable(bool enable) = 0;

	/** 
	 * @fn		get_tenant_name
	 * @brief	设置和获取租用者名字
	 * @author	jdyu
	 * @date	2008/10/13
	 * @return	virtual const 
	 * @see		
	 */
	virtual const char * get_tenant_name(void) = 0;
	virtual void set_tenant_name(const char * name) = 0;

	/** 
	 * @fn		get_com_name
	 * @brief	设置和获取公司名字
	 * @author	jdyu
	 * @date	2008/10/13
	 * @return	virtual const 
	 * @see		
	 */
	virtual const char * get_com_name(void) = 0;
	virtual void set_com_name(const char * name) = 0;
	};

/** 
 * @class	rec_calog_i
 * @brief	识别器使用的calog接口
 * 
 *  符合识别流程的调用
 * 
 * @author	jdyu
 * @date	2007-8-3
 * 
 * @see		
 */
class rec_calog_i
	: public calog_i
	{
public:
	/** 
	 * @brief 	on_recognizer_create
	 *  
	 *  当识别器被创建和销毁的时候进行的调用
	 *  
	 * @author	jdyu
	 * @date	2007-7-25
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @param	ISR_REC_INST rec	- [in] 
	 * @see		
	 */
	virtual int on_recognizer_create(ISR_REC_INST rec) = 0;
	virtual int on_recognizer_destroy(ISR_REC_INST rec) = 0;

	/** 
	 * @brief 	on_session_begin
	 *  
	 *  会话开始和结束时对calog的调用
	 *  
	 * @author	jdyu
	 * @date	2007-7-20
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @param	const wchar_t * chan	- [in] Channel name, session name
	 * @see		
	 */
	virtual int on_session_begin(const wchar_t * chan) = 0;
	virtual int on_session_end(void) = 0;

	/** 
	 * @brief 	on_recognize_start
	 *  
	 *  识别开始和结束时的调用
	 *  
	 * @author	jdyu
	 * @date	2007-7-25
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @see		
	 */
	virtual int on_recognize_start(void) = 0;
	virtual int on_recognize_stop(void) = 0;
	};

/** 
 * @class	ep_calog_i
 * 
 * @brief	端点检测使用的calog
 * 
 *  为了端点检测接口实现了特别的接口
 * 
 * @author	jdyu
 * @date	2007-8-3
 * 
 * @see		
 */
class ep_calog_i
	: public calog_i
	{
public:
	/** 
	 * @brief 	on_recognizer_create
	 *  
	 *  当端点检测器被创建和销毁的时候进行的调用
	 *  
	 * @author	jdyu
	 * @date	2007-7-25
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @param	ISR_EP_INST ep	- [in] 
	 * @see		
	 */
	virtual int on_detector_create(ISR_EP_INST ep) = 0;
	virtual int on_detector_destroy(ISR_EP_INST ep) = 0;

	/** 
	 * @brief 	on_session_begin
	 *  
	 *  会话开始和结束时对calog的调用
	 *  
	 * @author	jdyu
	 * @date	2007-7-20
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @param	const wchar_t * chan	- [in] Channel name, session name
	 * @see		
	 */
	virtual int on_session_begin(const wchar_t * chan) = 0;
	virtual int on_session_end(void) = 0;

	/** 
	 * @brief 	on_ep_start
	 *  
	 *  端点检测开始和结束时的调用
	 *  
	 * @author	jdyu
	 * @date	2007-7-25
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @see		
	 */
	virtual int on_ep_start(void) = 0;
	virtual int on_ep_stop(void) = 0;

	/** 
	 * @fn		log_raw_waveform
	 * @brief	记录原始音频数据
	 * 
	 *  写入原始音频数据
	 * 
	 * @author	jdyu
	 * @date	2007-9-14
	 * @return	virtual int	- Return 0 in success, otherwise return error code.
	 * @see		
	 */
	virtual int log_raw_waveform(const void * wave, unsigned int bytes, int sample_status, const wchar_t * audio_str) = 0;
	};

/** 
 * @class	calog_mngr_i
 * @brief	calog的管理器
 * 
 *  全局的callog管理器，负责calog的初始化和逆初始化，
 *  配置读取和写入。
 * 
 * @author	jdyu
 * @date	2007-7-24
 * 
 * @see		calog_i
 * 
 */
class calog_mngr_i
	: public sr::if_base
	{
public:
	virtual int open(void * reserved = 0) = 0;
	virtual int close(void) = 0;
	virtual const char * base_path(void) const = 0;
	};

/* ==========================================================================*/
/* 方便使用的方法 */
/* ==========================================================================*/

class calog_attrs
	{
public:
	// add attr and val
	calog_attrs & operator () (const wchar_t * attr, const wchar_t * val)
		{
		attr_val av = { attr, val };
		attrs_.push_back(av);
		return *this;
		}
	calog_attrs & operator () (const wchar_t * attr, const std::wstring & val)
		{
		return operator ()(attr, val.c_str());
		}
	// add attr and val
	calog_attrs & operator () (const wchar_t * attr, const char * val)
		{
		str_bufs_.push_back(sp::char2wchar(val));
		attr_val av = { attr, str_bufs_.back().c_str() };
		attrs_.push_back(av);
		return *this;
		}
	calog_attrs & operator () (const wchar_t * attr, const std::string & val)
		{
		return operator ()(attr, val.c_str());
		}
	calog_attrs & operator () (const wchar_t * attr, double val, int prec = 6)
		{
		std::wstringstream sm;
		sm.setf(std::ios::fixed);
		sm.precision(prec);

		sm << val;
		str_bufs_.push_back(sm.str());

		attr_val av = { attr, str_bufs_.back().c_str() };
		attrs_.push_back(av);
		return *this;
		}
	calog_attrs & operator () (const wchar_t * attr, int val)
		{
		#if 0
		std::wstringstream sm;
		sm.imbue(std::locale("C"));
		sm << val;
		str_bufs_.push_back(sm.str());
		#else
		wchar_t tmp[64];
		swprintf(tmp, 64, L"%d", val);
		str_bufs_.push_back(tmp);
		#endif // 

		attr_val av = { attr, str_bufs_.back().c_str() };
		attrs_.push_back(av);
		return *this;
		}
	calog_attrs & operator () (const wchar_t * attr, unsigned int val)
		{
		wchar_t tmp[64];
		swprintf(tmp, 64, L"%d", val);
		str_bufs_.push_back(tmp);

		attr_val av = { attr, str_bufs_.back().c_str() };
		attrs_.push_back(av);
		return *this;
		}
	operator const attr_val * (void) const
		{
		if ( attrs_.empty() )
			return 0;
		return &attrs_.front();
		}
	int count(void) const
		{
		return (int)attrs_.size();
		}

	explicit calog_attrs(const wchar_t * attr, const wchar_t * val)
		{
		this->operator()(attr, val);
		}
	explicit calog_attrs(const wchar_t * attr, const std::wstring & val)
		{
		this->operator()(attr, val);
		}
	explicit calog_attrs(const wchar_t * attr, const char * val)
		{
		this->operator()(attr, val);
		}
	explicit calog_attrs(const wchar_t * attr, const std::string & val)
		{
		this->operator()(attr, val);
		}
	calog_attrs(const wchar_t * attr, double val, int prec = 6)
		{
		this->operator()(attr, val, prec);
		}
	calog_attrs(const wchar_t * attr, int val)
		{
		this->operator()(attr, val);
		}
	calog_attrs(const wchar_t * attr, unsigned int val)
		{
		this->operator()(attr, val);
		}
private:
	// attributes array
	typedef std::vector<attr_val> attr_arr;
	// attr_val array
	attr_arr	attrs_;

	// 用于保存临时转换的内容
	typedef std::list<std::wstring> str_list;
	str_list	str_bufs_;
	};

#if _MSC_VER >= 1400
# define cl_vpars(msg)			\
	va_list lst;				\
	va_start(lst, fmt);			\
	vswprintf_s(msg, sizeof msg / sizeof msg[0], fmt, lst);	\
	va_end(lst);
#else // vc 7 and before and linux
# define cl_vpars(msg)			\
	va_list lst;				\
	va_start(lst, fmt);			\
	vswprintf(msg, sizeof msg / sizeof msg[0], fmt, lst);	\
	va_end(lst);
#endif // cl_vpars

/** 
 * @brief 	calog_message
 *  
 *  支持变参的记录信息，简化调用过程，举例如下：
 *  	cl::calog_message(clg, L"chanid",
 *			cl::calog_attrs(L"attr1", L"val1")(L"attr2", val2)(L"attr3", val3),
 *				L"This is a %d test message.\n", __LINE__);
 *
 * @author	jdyu
 * @date	2007-7-20
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	calog_i * clg	- [in] 当前的calog实例
 */
inline
int calog_message(calog_i * clg, const wchar_t * events, const calog_attrs & ca, const wchar_t * fmt, ...)
	{
	if ( clg == 0 )
		return -1;
	if ( !clg->logging_enabled() )
		return 0;

	wchar_t msg[4096];
	cl_vpars(msg);
	return clg->log_session_message(events, ca, ca.count(), msg);
	}
inline
int calog_message(calog_i * clg, const wchar_t * events, const calog_attrs & ca)
	{
	if ( clg == 0 )
		return -1;
	if ( !clg->logging_enabled() )
		return 0;
	return clg->log_session_message(events, ca, ca.count(), L"");
	}
inline
int calog_message(calog_i * clg, events_global event, const calog_attrs & ca)
	{
	return calog_message(clg, get_event_str(event), ca);
	}
inline
int calog_preload_message(calog_i * clg, const wchar_t * events, const calog_attrs & ca, const wchar_t * fmt, ...)
	{
	if ( clg == 0 )
		return -1;
	if ( !clg->logging_enabled() )
		return 0;

	wchar_t msg[4096];
	cl_vpars(msg);
	return clg->log_preload_message(events, ca, ca.count(), msg);
	}
inline
int calog_preload_message(calog_i * clg, const wchar_t * events, const calog_attrs & ca)
	{
	if ( clg == 0 )
		return -1;
	if ( !clg->logging_enabled() )
		return 0;
	return clg->log_preload_message(events, ca, ca.count(), L"");
	}
inline
int calog_preload_message(calog_i * clg, events_global event, const calog_attrs & ca)
	{
	return calog_preload_message(clg, get_event_str(event), ca);
	}


/** 
 * @brief 	calog_message
 *  
 *  支持变参的记录信息(没有属性对)
 *  
 * @author	jdyu
 * @date	2007-7-20
 * @return	int	- Return 0 in success, otherwise return error code.
 * @see		
 */
inline
int calog_message(calog_i * clg, const wchar_t * events, const wchar_t * fmt, ...)
	{
	if ( clg == 0 )
		return -1;
	if ( !clg->logging_enabled() )
		return 0;

	wchar_t msg[4096];
	cl_vpars(msg);
	return clg->log_session_message(events, 0, 0, msg);
	}
inline
int calog_preload_message(calog_i * clg, const wchar_t * events, const wchar_t * fmt, ...)
	{
	if ( clg == 0 )
		return -1;
	if ( !clg->logging_enabled() )
		return 0;

	wchar_t msg[4096];
	cl_vpars(msg);
	return clg->log_preload_message(events, 0, 0, msg);
	}
#undef cl_vpars

} /* namespace cl */

DEFINE_SHMOD(calog);
DEFINE_SHPTR(cl::rec_calog_i,  "rec_calog_i");
DEFINE_SHPTR(cl::ep_calog_i,   "ep_calog_i");
DEFINE_SHPTR(cl::calog_mngr_i, "calog_mngr_i");

#endif /* __CALLLOG_H__ */
