/** 
 * @file	dum_svc.h
 * @brief	�յķ���ʵ��
 * 
 *  һ���յķ���ʵ�֣����ڷ���mtrec�����Ч�ʺ��ȶ��ԡ�
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
 * @brief	ÿ�����Ե�Ԫ
 * 
 *  ���ҽű��ļ�
 * 
 * @author	jdyu
 * @date	2008/10/9
 */
typedef run_unit_t<ru_filter_scp> dum_unit;

/** 
 * @class	dum_mngr
 * 
 * @brief	ȫ�ֵ�ʵ�ֿ�
 * 
 *  �Ե���ģʽʵ�֣����������ط��ĵ���
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
	// ����ʵ�ֵĳ�ʼ��
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

	// ����ȱʡ�ű��ļ���
	virtual const char * default_scp(void) const
		{
		return cfg_mngr().scp_file().c_str();
		}
	};

/** 
 * @class	dum_svc
 * @brief	dummy�Ĳ���
 * 
 *  ����dummy
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
// �߳̽ű�������
// Դ��batrec����Ľű�������
// ============================================================================
typedef sc::asc_session_t<sc::asc_parser, svc_sink_impl<dum_svc>, char,
			sc::asc_name_t<'d', 'u', 'm', 'm', 'y'> >
		dum_svc_parser;

// ============================================================================
// ����dummy�Ĺ�����
// ============================================================================
typedef	thr_admin_parser<dum_mngr, dum_svc_parser, dum_unit> dsdum_mngr;


#endif /* __DUM_SVC_H__ */
