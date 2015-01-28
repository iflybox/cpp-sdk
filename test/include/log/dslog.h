/** 
 * @file	dslog.h
 * @brief	全局使用的日志类
 * 
 *  请在DSS核心模块的项目中使用这个日志类记录运行日志信息
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2007-5-11
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2007-5-11	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __DSLOG_H__
#define __DSLOG_H__

#include "log/iFly_Log.h"
#include "iFly_Assert.h"

// 日志单体
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, ds_log);

/**
 * @brief	方便使用的宏
 *	在使用单实例模式的时候，这几个宏可以更简洁的使用日志功能
 */
#define dslog_inst						ds_log::instance()
#define dslog_crit						if ( dslog_inst ) dslog_inst->log_crit
#define dslog_error						if ( dslog_inst ) dslog_inst->log_error
#define dslog_warn						if ( dslog_inst ) dslog_inst->log_warn
#define dslog_info						if ( dslog_inst ) dslog_inst->log_info
#define dslog_verbose					if ( dslog_inst ) dslog_inst->log_verbose
#define dslog_dbg						if ( dslog_inst ) dslog_inst->log_debug
#define dslog_perf						if ( dslog_inst ) dslog_inst->log_perf
#define dslog_trace						if ( dslog_inst ) dslog_inst->log_trace
#define dslog_wmsg						if ( dslog_inst ) dslog_inst->write_msg
#define dslog_flush						if ( dslog_inst ) dslog_inst->flush

// 能够自动统计性能并打印的
#define dslog_perf_n(name)				Log_Perf_Helper<Log_Timer, ds_log>		__ph__(#name);
#define dslog_perf_s(name,fmt)			Log_Perf_Helper<Log_Timer, ds_log>		__ph__(#name); __ph__.log fmt;
#define dslog_perf_nsec(name,nsec)		Log_Perf_Helper<Log_Timer, ds_log>		__ph__(#name, nsec);
#define dslog_perf_stop()														__ph__.stop()

// 跟踪函数调用的方法
#define dslog_tracer(sub)				Log_Func_Tracer<ds_log>					__lt__(#sub);
#define dslog_tracer_perf(sub)			dslog_perf_n(sub); dslog_tracer(sub);

// 初始化和逆初始化
#define dslog_open						ds_log::open
#define dslog_close						ds_log::close
#define dslog_open_ac					Log_Auto_Close<ds_log> __lac__; ds_log::open

// 配置类
typedef ds_log_Cfg						dslog_cfg;

// ============================================================================
// 方便的宏组合
// ============================================================================

// ASSERT然后打印日志
#define dslog_error_assert(exp, msg)	if (!(exp)) { dslog_error msg; SP_ASSERTS(exp, msg); }
#define dslog_error_assert_return(exp, msg, ret)	\
										if (!(exp)) { dslog_error msg; SP_ASSERTS(exp, msg); return ret; }
#define dslog_error_assert_return_no(exp, msg)	\
										if (!(exp)) { dslog_error msg; SP_ASSERTS(exp, msg); return ; }
#define dslog_warn_assert(exp, msg)		if (!(exp)) { dslog_warn msg;  SP_ASSERTS(exp, msg); }
#define dslog_warn_assert_return(exp, msg, ret)	\
										if (!(exp)) { dslog_warn msg; SP_ASSERTS(exp, msg); return ret; }
#define dslog_warn_assert_return_no(exp, msg)	\
										if (!(exp)) { dslog_warn msg; SP_ASSERTS(exp, msg); return ; }

#define dslog_error_noassert(exp, msg)	if (!(exp)) { dslog_error msg; }
#define dslog_error_return(exp, msg, ret)	\
										if (!(exp)) { dslog_error msg; return ret; }
#define dslog_error_return_no(exp, msg)		\
										if (!(exp)) { dslog_error msg; return ; }
#define dslog_warn_noassert(exp, msg)	if (!(exp)) { dslog_warn msg; }
#define dslog_warn_return(exp, msg, ret)	\
										if (!(exp)) { dslog_warn msg; return ret; }
#define dslog_warn_return_no(exp, msg)	\
										if (!(exp)) { dslog_warn msg; return ; }

// 根据返回值打印相同的信息
#define dslog_func_ret(ret, msg)		if ( ret != 0 ) { dslog_error msg; } else { dslog_verbose msg; }
#define dslog_func_ret_info(ret, msg)	if ( ret != 0 ) { dslog_error msg; } else { dslog_info msg; }

// 根据函数返回值打印不同的信息
#define dslog_func_verb(ret, succ_msg, fail_msg)	\
			if ( ret == 0 ) { dslog_verbose succ_msg; }	else { dslog_error fail_msg; }
#define dslog_func_info(ret, succ_msg, fail_msg)	\
			if ( ret == 0 ) { dslog_info succ_msg; } else { dslog_error fail_msg; }
#define dslog_func_verb_return(ret, succ_msg, fail_msg)	\
			if ( ret == 0 ) { dslog_verbose succ_msg; }	else { dslog_error fail_msg; return ret; }
#define dslog_func_info_return(ret, succ_msg, fail_msg)	\
			if ( ret == 0 ) { dslog_info succ_msg; } else { dslog_error fail_msg; return ret; }

// 判断表达式，打印不同的信息
#define dslog_func_expr(expr, succ_msg, fail_msg)	\
			if ( expr ) { dslog_info succ_msg; } else { dslog_error fail_msg; }

// 校验句柄，并打印日志
#define dslog_verify_inst(func, inst)				\
			if ( !(inst) ) { dslog_error("%s | %s handle is NULL.", #func, #inst); return ISR_ERROR_NULL_HANDLE; }
#define dslog_verify_para(func, para)				\
			if ( !(para) ) { dslog_error("%s | para %s is NULL.", #func, #para); return ISR_ERROR_INVALID_PARA; }
#define dslog_verify_return(func, inst, ret, msg)	\
			if ( !(inst) ) { dslog_error("%s | %s is NULL, %s.", #func, #inst, msg); return ret; }

#define dslog_verify_rec(func)			dslog_verify_inst(func, rec)
#define dslog_verify_ep(func)			dslog_verify_inst(func, ep)

#define sr_str_print(x)		x ? sp::tochar(x).c_str() : "null"
#define sr_wstr_print(x)	x ? sp::towchar(x).c_str() : L"null"

#endif /* __SRLOG_H__ */
