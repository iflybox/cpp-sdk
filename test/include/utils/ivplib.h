#ifndef __IVPLIB_H__
#define __IVPLIB_H__

#include "utils/sulib.h"
#include "qisv.h"

// Sun OS pack
#ifndef WIN32
#pragma pack(push)
#pragma pack(8)
#endif // sun

namespace sp {
		
const char qisv_lib_name[] = "vpsc"
	#ifdef _DEBUG
	"d"
	#endif /* _DEBUG */
	;

/** 
 * @class	qisv_lib
 * @brief	º”‘ÿQISVø‚
 * @author	
 * @date	
 * @see		
 */
class qisv_lib 
	: public sp::su_lib<sp::loader, char>
	{
public:
	qisv_lib(const char * lib = qisv_lib_name)
		: sp::su_lib<sp::loader, char>(lib)
		{
		}
	virtual int load_procs(void)
		{
		su_proc_load(QISVInit)
		su_proc_load(QISVSessionBegin)
		su_proc_load(QISVAudioWrite)
		su_proc_load(QISVGetResult)
		su_proc_load(QISVDownloadData)
		su_proc_load(QISVSessionEnd)

#ifdef MSP_WCHAR_SUPPORT
		su_proc_load(QISVInitW)
		su_proc_load(QISVSessionBeginW)
		su_proc_load(QISVAudioWriteW)
		su_proc_load(QISVGetResultW)
		su_proc_load(QISVSessionEndW)
#endif
		su_proc_load(QISVFini)
		return 0;
		}
private:
	su_proc_decl(QISVInit)
	su_proc_decl(QISVSessionBegin)
	su_proc_decl(QISVAudioWrite)
	su_proc_decl(QISVGetResult)
	su_proc_decl(QISVDownloadData)
	su_proc_decl(QISVSessionEnd)

#ifdef MSP_WCHAR_SUPPORT
	su_proc_decl(QISVInitW)
	su_proc_decl(QISVSessionBeginW)
	su_proc_decl(QISVAudioWriteW)
	su_proc_decl(QISVGetResultW)
	su_proc_decl(QISVSessionEndW)
#endif
	su_proc_decl(QISVFini)
	};
} /* namespace sp */

// Sun OS pack
#ifndef WIN32
#pragma pack(pop)
#endif // sun

#endif /* __ISRLIB_H__ */