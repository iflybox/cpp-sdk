/** 
 * @file	REM_Spec.h
 * @brief   iFLY SEM SDK inner header file
 * 
 * This file contains the inner SEM application programming interface(API) declarations
 * of iFLYTEK. Advanced SEM developer can include this file in your project to build applications.
 * For more information, please read the developer guide.
 * Copyright (C)    1999 - 2004 by ANHUI USTC IFLYTEK. Co.,LTD.
 *                  All rights reserved.
 * 
 * @author    Speech Dept. iFLYTEK.
 * @version   1.0
 * @date	  2008/11/21
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2008/11/21	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __REM_SPEC_H__
#define __REM_SPEC_H__

#include "iFly_REM.h"
#include "iFly_Assert.h"

#ifdef __cplusplus
extern "C" {
#endif

const char rem_inst_status_str[][16] =
{
	"Unknown",
	"Create",
	"Destroy",
	"Recv",
	"Send",
	"Recog",
	"Idle",
};

inline const char * rem_get_inst_status_str(int status)
{
	SP_ASSERT(status >= REM_INST_STATUS_UNKN);
	SP_ASSERT(status <= REM_INST_STATUS_IDLE);
	if ( status < 0 || status > REM_INST_STATUS_IDLE )
		return 0;
	return rem_inst_status_str[status];
}

#ifdef __cplusplus
}    /* extern "C" */
#endif

#endif /* __REM_SPEC_H__ */
