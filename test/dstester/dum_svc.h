/** 
 * @file	dum_svc.h
 * @brief	空的服务实现
 * 
 *  一个空的服务实现，用于分析mtrec本身的效率和稳定性。
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2008/11/24
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2008/11/24	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __DUM_SVC_H__
#define __DUM_SVC_H__

/** 
 * @class	dum_unit
 * @brief	每个测试单元
 * 
 *  查找脚本文件
 * 
 * @author	jdyu
 * @date	2008/10/9
 */
typedef run_unit_t<ru_filter_scp> dum_unit;

/** 
 * @class	dum_mngr
 * 
 * @brief	全局的实现库
 * 
 *  以单体模式实现，方便其他地方的调用
 * 
 * @author	jdyu
 * @date	2008-10-08
 * 
 * @see		
 */
class dum_mngr
	: public svc_mngr<dum_mngr>
	{
protected:
	// 必须实现的初始化
	int on_initialize(const sp::str_arr & libs)
		{
		return 0;
		}

	int on_uninitialize(void)
		{
		proc_resp_mngr().add_msec("CAPS", caps_global());
		return 0;
		}

public:
	dum_mngr(void)
		{
		}

	// 返回缺省脚本文件名
	virtual const char * default_scp(void) const
		{
		return cfg_mngr().scp_file().c_str();
		}
	};

/** 
 * @class	dum_svc
 * @brief	dummy的测试
 * 
 *  测试dummy
 * 
 * @author	jdyu
 * @date	2008/10/8
 */
class dum_svc
	: public sc::asc_cmd_t<dum_svc>
	, public svc_bind_unit<dum_unit>
	{
public:
	dum_svc(void)
		{
		}
	virtual ~dum_svc(void)
		{
		}
	};

// ============================================================================
// 线程脚本解析器
// 源于batrec里面的脚本解析器
// ============================================================================
typedef sc::asc_session_t<sc::asc_parser, svc_sink_impl<dum_svc>, char,
			sc::asc_name_t<'d', 'u', 'm', 'm', 'y'> >
		dum_svc_parser;

// ============================================================================
// 测试dummy的管理器
// ============================================================================
typedef	thr_admin_parser<dum_mngr, dum_svc_parser, dum_unit> dsdum_mngr;


#endif /* __DUM_SVC_H__ */
