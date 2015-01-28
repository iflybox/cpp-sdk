/** 
 * @file	srlog.h
 * @brief	isrȫ��ʹ�õ���־��
 * 
 *  ����isr����ģ�����Ŀ��ʹ�������־���¼������־��Ϣ
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
#ifndef __SRLOG_H__
#define __SRLOG_H__

#include "log/iFly_Log.h"
#include "iFly_Assert.h"

// ��־����
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, sr_log);

/**
 * @brief	����ʹ�õĺ�
 *	��ʹ�õ�ʵ��ģʽ��ʱ���⼸������Ը�����ʹ����־����
 */
#define srlog_inst						sr_log::instance()
#define srlog_crit						if ( srlog_inst ) srlog_inst->log_crit
#define srlog_error						if ( srlog_inst ) srlog_inst->log_error
#define srlog_warn						if ( srlog_inst ) srlog_inst->log_warn
#define srlog_info						if ( srlog_inst ) srlog_inst->log_info
#define srlog_verbose					if ( srlog_inst ) srlog_inst->log_verbose
#define srlog_dbg						if ( srlog_inst ) srlog_inst->log_debug
#define srlog_perf						if ( srlog_inst ) srlog_inst->log_perf
#define srlog_trace						if ( srlog_inst ) srlog_inst->log_trace
#define srlog_wmsg						if ( srlog_inst ) srlog_inst->write_msg
#define srlog_spec						if ( srlog_inst ) srlog_inst->log_spec
#define srlog_flush						if ( srlog_inst ) srlog_inst->flush

// �ܹ��Զ�ͳ�����ܲ���ӡ��
#define srlog_perf_n(name)				Log_Perf_Helper<Log_Timer, sr_log>		__ph__(#name);
#define srlog_perf_s(name,fmt)			Log_Perf_Helper<Log_Timer, sr_log>		__ph__(#name); __ph__.log fmt;
#define srlog_perf_nsec(name,nsec)		Log_Perf_Helper<Log_Timer, sr_log>		__ph__(#name, nsec);
#define srlog_perf_stop()														__ph__.stop()

// ���ٺ������õķ���
#define srlog_tracer(sub)				Log_Func_Tracer<sr_log>					__lt__(#sub);
#define srlog_tracer_perf(sub)			srlog_perf_n(sub); srlog_tracer(sub);

// ��ʼ�������ʼ��
#define srlog_open						sr_log::open
#define srlog_close						sr_log::close
#define srlog_open_ac					Log_Auto_Close<sr_log> __lac__; sr_log::open

// ������
typedef sr_log_Cfg						srlog_cfg;

// ============================================================================
// ����ĺ����
// ============================================================================

// ASSERTȻ���ӡ��־
#define srlog_error_assert(exp, msg)	if (!(exp)) { srlog_error msg; SP_ASSERTS(exp, msg); }
#define srlog_error_assert_return(exp, msg, ret)	\
										if (!(exp)) { srlog_error msg; SP_ASSERTS(exp, msg); return ret; }
#define srlog_error_assert_return_no(exp, msg)	\
										if (!(exp)) { srlog_error msg; SP_ASSERTS(exp, msg); return ; }
#define srlog_warn_assert(exp, msg)		if (!(exp)) { srlog_warn msg;  SP_ASSERTS(exp, msg); }
#define srlog_warn_assert_return(exp, msg, ret)	\
										if (!(exp)) { srlog_warn msg; SP_ASSERTS(exp, msg); return ret; }
#define srlog_warn_assert_return_no(exp, msg)	\
										if (!(exp)) { srlog_warn msg; SP_ASSERTS(exp, msg); return ; }

#define srlog_error_noassert(exp, msg)	if (!(exp)) { srlog_error msg; }
#define srlog_error_return(exp, msg, ret)	\
										if (!(exp)) { srlog_error msg; return ret; }
#define srlog_error_return_no(exp, msg)		\
										if (!(exp)) { srlog_error msg; return ; }
#define srlog_warn_noassert(exp, msg)	if (!(exp)) { srlog_warn msg; }
#define srlog_warn_return(exp, msg, ret)	\
										if (!(exp)) { srlog_warn msg; return ret; }
#define srlog_warn_return_no(exp, msg)	\
										if (!(exp)) { srlog_warn msg; return ; }

// ���ݷ���ֵ��ӡ��ͬ����Ϣ
#define srlog_func_ret(ret, msg)		if ( ret != 0 ) { srlog_error msg; } else { srlog_verbose msg; }
#define srlog_func_ret_info(ret, msg)	if ( ret != 0 ) { srlog_error msg; } else { srlog_info msg; }

// ���ݺ�������ֵ��ӡ��ͬ����Ϣ
#define srlog_func_verb(ret, succ_msg, fail_msg)	\
			if ( ret == 0 ) { srlog_verbose succ_msg; }	else { srlog_error fail_msg; }
#define srlog_func_info(ret, succ_msg, fail_msg)	\
			if ( ret == 0 ) { srlog_info succ_msg; } else { srlog_error fail_msg; }
#define srlog_func_verb_return(ret, succ_msg, fail_msg)	\
			if ( ret == 0 ) { srlog_verbose succ_msg; }	else { srlog_error fail_msg; return ret; }
#define srlog_func_info_return(ret, succ_msg, fail_msg)	\
			if ( ret == 0 ) { srlog_info succ_msg; } else { srlog_error fail_msg; return ret; }

// �жϱ��ʽ����ӡ��ͬ����Ϣ
#define srlog_func_expr(expr, succ_msg, fail_msg)	\
			if ( expr ) { srlog_info succ_msg; } else { srlog_error fail_msg; }

// У����������ӡ��־
#define srlog_verify_inst(func, inst)				\
			if ( !(inst) ) { srlog_error("%s | %s handle is NULL.", #func, #inst); return ISR_ERROR_NULL_HANDLE; }
#define srlog_verify_para(func, para)				\
			if ( !(para) ) { srlog_error("%s | para %s is NULL.", #func, #para); return ISR_ERROR_INVALID_PARA; }
#define srlog_verify_para_ex(func, para, ret, rtn)	\
	if ( !(para) ) { srlog_error("%s | para %s is NULL.", #func, #para); \
	*ret = ISR_ERROR_INVALID_PARA;  return rtn;}
#define srlog_verify_return(func, inst, ret, msg)	\
			if ( !(inst) ) { srlog_error("%s | %s is NULL, %s.", #func, #inst, msg); return ret; }

#define srlog_verify_rec(func)			srlog_verify_inst(func, rec)
#define srlog_verify_ep(func)			srlog_verify_inst(func, ep)

#define sr_str_print(x)		x ? sp::tochar(x).c_str() : "null"
#define sr_wstr_print(x)	x ? sp::towchar(x).c_str() : L"null"

#endif /* __SRLOG_H__ */
