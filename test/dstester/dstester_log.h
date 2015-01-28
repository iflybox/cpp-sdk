/** 
 * @file	mtr_log.h
 * @brief	mtrec程序里面使用的几个日志
 * 
 *  日志仅包含性能日志
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
#ifndef __IDSS_LOG_H__
#define __IDSS_LOG_H__

#ifndef WIN32//linux yanxu2 add 2010-12-29
# include <string.h>
#endif
#include "log/iFly_Log.h"
#include "iFly_Assert.h"

// 日志单体
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, prflog);
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, rsplog);
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, errlog);

/**
 * @brief	方便使用的宏
 *	在使用单实例模式的时候，这几个宏可以更简洁的使用日志功能
 */
#define prflog_inst						prflog::instance()
#define prflog_crit						if ( prflog_inst ) prflog_inst->log_crit
#define prflog_wmsg						if ( prflog_inst ) prflog_inst->write_msg
#define prflog_flush					if ( prflog_inst ) prflog_inst->flush

// 初始化和逆初始化
#define prflog_open						prflog::open
#define prflog_close					prflog::close
#define prflog_open_ac					Log_Auto_Close<prflog> __pflac__; prflog::open

// 配置类
typedef prflog_Cfg						prflog_cfg;

/**
 * @brief	方便使用的宏
 *	在使用单实例模式的时候，这几个宏可以更简洁的使用日志功能
 */
#define rsplog_inst						rsplog::instance()
#define rsplog_crit						if ( rsplog_inst ) rsplog_inst->log_crit
#define rsplog_wmsg						if ( rsplog_inst ) rsplog_inst->write_msg
#define rsplog_flush					if ( rsplog_inst ) rsplog_inst->flush

// 初始化和逆初始化
#define rsplog_open						rsplog::open
#define rsplog_close					rsplog::close
#define rsplog_open_ac					Log_Auto_Close<rsplog> __rsplac__; rsplog::open

// 配置类
typedef rsplog_Cfg						rsplog_cfg;

/**
 * @brief	方便使用的宏
 *	在使用单实例模式的时候，这几个宏可以更简洁的使用日志功能
 */
#define errlog_inst						errlog::instance()
#define errlog_crit						if ( errlog_inst ) errlog_inst->log_crit

// 初始化和逆初始化
#define errlog_open						errlog::open
#define errlog_close					errlog::close
#define errlog_open_ac					Log_Auto_Close<errlog> __errlac__; errlog::open

// 配置类
typedef errlog_Cfg						errlog_cfg;

#endif /* __IDSS_LOG_H__ */
