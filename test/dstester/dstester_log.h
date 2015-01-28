/** 
 * @file	mtr_log.h
 * @brief	mtrec��������ʹ�õļ�����־
 * 
 *  ��־������������־
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

// ��־����
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, prflog);
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, rsplog);
LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Thread_Mutex, Log_Cfg_Heap_Reg, errlog);

/**
 * @brief	����ʹ�õĺ�
 *	��ʹ�õ�ʵ��ģʽ��ʱ���⼸������Ը�����ʹ����־����
 */
#define prflog_inst						prflog::instance()
#define prflog_crit						if ( prflog_inst ) prflog_inst->log_crit
#define prflog_wmsg						if ( prflog_inst ) prflog_inst->write_msg
#define prflog_flush					if ( prflog_inst ) prflog_inst->flush

// ��ʼ�������ʼ��
#define prflog_open						prflog::open
#define prflog_close					prflog::close
#define prflog_open_ac					Log_Auto_Close<prflog> __pflac__; prflog::open

// ������
typedef prflog_Cfg						prflog_cfg;

/**
 * @brief	����ʹ�õĺ�
 *	��ʹ�õ�ʵ��ģʽ��ʱ���⼸������Ը�����ʹ����־����
 */
#define rsplog_inst						rsplog::instance()
#define rsplog_crit						if ( rsplog_inst ) rsplog_inst->log_crit
#define rsplog_wmsg						if ( rsplog_inst ) rsplog_inst->write_msg
#define rsplog_flush					if ( rsplog_inst ) rsplog_inst->flush

// ��ʼ�������ʼ��
#define rsplog_open						rsplog::open
#define rsplog_close					rsplog::close
#define rsplog_open_ac					Log_Auto_Close<rsplog> __rsplac__; rsplog::open

// ������
typedef rsplog_Cfg						rsplog_cfg;

/**
 * @brief	����ʹ�õĺ�
 *	��ʹ�õ�ʵ��ģʽ��ʱ���⼸������Ը�����ʹ����־����
 */
#define errlog_inst						errlog::instance()
#define errlog_crit						if ( errlog_inst ) errlog_inst->log_crit

// ��ʼ�������ʼ��
#define errlog_open						errlog::open
#define errlog_close					errlog::close
#define errlog_open_ac					Log_Auto_Close<errlog> __errlac__; errlog::open

// ������
typedef errlog_Cfg						errlog_cfg;

#endif /* __IDSS_LOG_H__ */
