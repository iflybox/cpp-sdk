/**
 * @file	hcrlib.h
 * @brief	���ڼ�����дʶ���dll
 * 
 * 
 * @author	mingzhang2
 * @version	1.0
 * @date	2010��9��24��
 *
 * @see
 *
 * @par	�汾��¼
 * <table border=1>
 *  <tr> <th>�汾	<th>����            <th>����		<th>��ע </tr>
 *  <tr> <th>1.0	<th>2010��8��4��	<td>mingzhang2	<td>���� <tr>
 * </table>
 */

#ifndef __HCRLIB_H__
#define __HCRLIB_H__

#if _MSC_VER > 1000
# pragma once
#endif

#include "utils/sulib.h"
#include "iFlyHcr.h"
#include "qhcr.h"

const wchar_t hcr_lib_name[] = L"iFlyHcr";

class hcr_lib
	: public sp::su_lib<sp::loader, wchar_t>
	{
public:
	hcr_lib(const wchar_t * lib = hcr_lib_name)
		: sp::su_lib<sp::loader, wchar_t>(lib)
		{
		}
protected:
	virtual int load_procs(void)
		{
		su_proc_load_w(QHCRInit)
		su_proc_load_w(iFlyHcrModuleCreate)
		su_proc_load_w(iFlyHcrModuleDestroy)
		su_proc_load_w(iFlyHcrModuleStart)
		su_proc_load_w(iFlyHcrModuleAppendStrokes)
		su_proc_load_w(iFlyHcrModuleGetResult)
		su_proc_load_w(iFlyHcrSetPredictData)
		su_proc_load_w(iFlyHcrGetPredictResult)
		su_proc_load_w(QHCRFini)
		return 0;
		}
private:
	su_proc_decl_w(QHCRInit)
	su_proc_decl_w(iFlyHcrModuleCreate)
	su_proc_decl_w(iFlyHcrModuleDestroy)
	su_proc_decl_w(iFlyHcrModuleStart)
	su_proc_decl_w(iFlyHcrModuleAppendStrokes)
	su_proc_decl_w(iFlyHcrModuleGetResult)
	su_proc_decl_w(iFlyHcrSetPredictData)
	su_proc_decl_w(iFlyHcrGetPredictResult)
	su_proc_decl_w(QHCRFini)
	};

#endif	/* __HCRLIB_H__ */
