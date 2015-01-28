/** 
 * @file	ISP_Utils.cpp
 * @brief	编码中常用的函数库
 * 
 *  这个文件中集中了在编码中经常用到的一些小功能
 *  非常实用。
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2004-6-22
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2004-6-22	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#include <sys/stat.h>

// OS relative headers
#ifdef WIN32
#  include <windows.h>
#  include <IPHlpApi.h>
#  include <Sensapi.h>
#  if _MSC_VER >= 1400
#    pragma warning(push)
#    pragma warning(disable : 4996)
#  endif /* VC8 */
#  pragma comment(lib, "version.lib")
#  pragma comment(lib, "ws2_32.lib")
#else /* LINUX */
#  include <unistd.h>
#  include <signal.h>
#  include <sys/param.h>
#  include <sys/types.h>
#  include <sys/stat.h>
#endif /* WIN32 */

#include "utils/sutils.h"

// using ACE or not
#if defined(ACE_ACE_H)
#  define USE_ACE		1
#  pragma message("ISP_Utils: using ace to implement isp utilities.")
#else
#  pragma message("ISP_Utils: not used ace, some function may not supported.")
#endif /* defined(ACE_ACE_H) */

#ifdef USE_ACE
#  include "ace/OS_NS_arpa_inet.h"
#endif /* #ifdef USE_ACE */

#ifdef WIN32
   struct ACE_utsname;
#elif defined (USE_ACE)
#  include "ace/OS_NS_sys_utsname.h"
#endif /* USE_ACE */

// assert
#ifndef IU_ASSERT
#  ifdef SP_ASSERT
#    define IU_ASSERT		SP_ASSERT
#  elif defined(ASSERT)
#    define IU_ASSERT		ASSERT
#  else // null
#    define IU_ASSERT(x)
#  endif
#endif /* IU_ASSERT */


/*=============================================================================
 * 通用操作
 *=============================================================================*/

// namespace sp
namespace sp
{
// debugging mode
static bool debug_mode_ = false;
}

bool sp::debugging_mode(bool mode)
{
	bool prev_mode = debug_mode_;
	debug_mode_ = mode;
	return prev_mode;
}

bool sp::debugging_mode(void)
{
	return debug_mode_;
}

char* sp::get_module_cfg(char path[MAX_PATH], const char* def_cfg, IU_HANDLE hmod = 0)
{
	char mod_path[MAX_PATH], mod_name[MAX_PATH];
	mod_path[0] = 0; mod_name[0] = 0;
	get_bin_path(mod_path, hmod);

	char dcfg[MAX_PATH]; dcfg[0] = 0;
	if ( def_cfg )
		strcpy(dcfg, def_cfg);

	// 直接查看缺省的配置绝对路径文件是否存在
	if ( is_abs_path(dcfg)
		&& is_file_exist(dcfg) )
		return strcpy(path, dcfg);

	// 看看和模块相同路径下有否？
	strcpy(path, mod_path);
	if ( is_file_exist(cat_path(path, dcfg)) )
		return path;

	// 检查当前路径上级的cfg目录下有否
	strcpy(path, mod_path);
	cat_path(path, "../cfg");
	if ( is_file_exist(cat_path(path, dcfg)) )
		return path;

	// 没有路径的缺省配置文件是否存在
	if ( is_file_exist(dcfg) )
		return strcpy(path, dcfg);

	// 替换后缀为cfg，查看对应的cfg文件是否存在
	get_bin_pathname(mod_name, hmod);
	char * pos = strrchr(mod_name, '.');
	if ( 0 != pos )
	{
		*pos = 0;
		sprintf(path, "%s.cfg", mod_name);
	}
	else if ( def_cfg )
		strcpy(path, dcfg);

	if ( is_file_exist(path) )
		return path;

	if ( def_cfg )
		strcpy(path, dcfg);
	return path;
}

std::string sp::get_module_cfg(const char* def_cfg, IU_HANDLE hmod /* = 0 */)
{
	char mod_path[MAX_PATH];
	mod_path[0] = 0;
	return get_module_cfg(mod_path, def_cfg, hmod);
}

const char* sp::install_path(const char* path /* = 0 */)
{
	static char ins_path[MAX_PATH] = "\0";
	if ( path )
		strsncpy(ins_path, path, MAX_PATH);
	return ins_path;
}

bool sp::is_register_key(const char* key)
{
	static const char* REG_KEY_ROOT[] =
	{
		"HKEY_CLASSES_ROOT",
		"HKEY_CURRENT_USER",
		"HKEY_LOCAL_MACHINE",
		"HKEY_USERS",
		"HKEY_CURRENT_CONFIG",
	};

	if ( key == 0 )
		return false;

	if ( is_file_exist(key) ||
		is_dir_exist(key) )
		return false;

	for ( unsigned int i = 0; i < sizeof REG_KEY_ROOT / sizeof REG_KEY_ROOT[0]; i++ )
	{
		#ifdef WIN32
		if ( strnicmp(REG_KEY_ROOT[i], key, strlen(REG_KEY_ROOT[i])) == 0 )
			return true;
		#else
		if ( strncasecmp(REG_KEY_ROOT[i], key, strlen(REG_KEY_ROOT[i])) == 0 )
			return true;
		#endif
	}

	return false;
}

const char* sp::get_common_path(void)
{
	static char path[MAX_PATH] = "\0";

	if ( path[0] != 0 )
		return path;

#ifdef WIN32
	typedef	HRESULT (WINAPI *Proc_SHGetFolderPath)(HWND hwndOwner,
		int		nFolder,
		HANDLE	hToken,
		DWORD	dwFlags,
		LPTSTR	pszPath
	);

	HMODULE hLib = LoadLibrary("shell32.dll");
	if ( hLib )
	{
		HRESULT ret;
		Proc_SHGetFolderPath pfnGFP = (Proc_SHGetFolderPath)
			GetProcAddress(hLib, "SHGetFolderPathA");
		if ( pfnGFP )
		{
			ret = pfnGFP(0, 0x002b/* CSIDL_PROGRAM_FILES_COMMON */, 0, 0, path);
			if ( !SUCCEEDED(ret) )
				printf("sp::get_common_path::SHGetFolderPath failed with %d!\n", ret);
		}
		else
			printf("sp::get_common_path::function SHGetFolderPath not found with %d!\n", GetLastError());
		FreeLibrary(hLib);
	}
#else	// linux
	strcpy(path, "/opt/iFLYTEK");
#endif  // linux

	return path;
}

std::string sp::error_desc(void)
{
# if defined (ACE_HAS_SVR4_DYNAMIC_LINKING)
#if defined(_M_UNIX)
	ACE_OSCALL_RETURN ((char *)::_dlerror (), char *, 0);
#else /* _M_UNIX */
	ACE_OSCALL_RETURN ((char *)::dlerror (), char *, 0);
#endif /* _M_UNIX */
# elif defined (__hpux) || defined (VXWORKS)
	ACE_OSCALL_RETURN (::strerror(errno), char *, 0);
# elif defined (WIN32)
	char buf[256];
#   if defined (ACE_HAS_PHARLAP)
	sprintf (buf, "error code %d", GetLastError());
#   else
	FormatMessage (FORMAT_MESSAGE_FROM_SYSTEM,
				0,
				last_error(),
				0,
				buf,
				sizeof buf / sizeof buf[0],
				0);
#   endif /* ACE_HAS_PHARLAP */
	return buf;
# else
	ACE_NOTSUP_RETURN (0);
# endif /* ACE_HAS_SVR4_DYNAMIC_LINKING */
}

/*=============================================================================
 * 网络相关的实现
 *=============================================================================*/
/** 
 * @brief 	local_hostname
 * 
 *  返回当前主机名称，如果有完全限定域名（fully qualified domain name，FQDN）
 *  则返回FQDN，否则返回简单主机名，第三个参数指明了是简单主机名，还是FQDN
 *  
 * @author	jingzhang
 * @date	2006-10-19
 * @return	int	- Return 0 in success, otherwise return error code.
 * @param	char*	hostname	- [out] 
 * @param	int		namelen		- [in] 
 * @param	bool	is_fqdn		- [out]
 * @see		
 */
int sp::local_hostname(char * hostname, int namelen, bool & is_fqdn)
{
	is_fqdn = false;
	memset(hostname, 0, namelen);
	if ( gethostname(hostname, namelen) != 0 )
		return SOCKET_ERROR;

	hostent* host = gethostbyname(hostname);
	if ( host == NULL )
		return SOCKET_ERROR;

	if ( host->h_name )
	{
		if ( ( strcmp(hostname, host->h_name) != 0 ) &&
			 ( strstr(host->h_name, ".") != NULL )   &&
			 ( strlen(host->h_name) < size_t(namelen) ) )
		{
			is_fqdn = true;
			strcpy(hostname, host->h_name);
			return 0;
		}
	}
	return 0;
}

namespace sp
{

int get_local_ip_list(str_arr & lips)
{
#ifdef WIN32
	// 获取网卡数量的接口函数
	typedef DWORD (WINAPI *Proc_GetAdaptersInfo)(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen);
	Proc_GetAdaptersInfo pfnGetAdaptersInfo = 0;

	HMODULE mod = LoadLibrary("iphlpapi.dll");
	if ( mod == 0 )
		return -1;

	pfnGetAdaptersInfo = (Proc_GetAdaptersInfo)GetProcAddress(mod, "GetAdaptersInfo");
	if ( pfnGetAdaptersInfo == 0 )
	{
		FreeLibrary(mod);
		return -1;
	}

	IP_ADAPTER_INFO* ia_info = 0;
	ULONG  dwSize = 0;
	int count = 0;

	// 获取网络适配器信息
	DWORD  ret = pfnGetAdaptersInfo(ia_info, &dwSize);
	// 重新分配内存
	if ( ret == ERROR_BUFFER_OVERFLOW )
	{
		count = dwSize / sizeof(IP_ADAPTER_INFO);
		ia_info = new IP_ADAPTER_INFO[count + 1];
		IU_ASSERT(ia_info);
		
		// 重新获取信息
		ret = pfnGetAdaptersInfo(ia_info, &dwSize);
		if ( ret != 0 )
		{
			if ( debug_mode_ )
				printf("get_local_ip_list pfnGetAdaptersInfo failed with %d, last error %d.\n", ret, last_error());
		}

		// 得到每个网卡的信息
		IP_ADAPTER_INFO * aip = ia_info;
		while ( aip )
		{
			IP_ADDR_STRING * addrs = &aip->IpAddressList;
			while(addrs)
			{
				const char * ip = addrs->IpAddress.String;
				if ( strcmp(ip, "0.0.0.0") != 0
					&& is_valid_ip_addr(ip) )
					lips.push_back(ip);
				addrs = addrs->Next;
			}
			aip = aip->Next;
		}
		delete [] ia_info;
	}
	else
	{
		if ( debug_mode_ )
			printf("get_local_ip_list pfnGetAdaptersInfo failed with %d, last error %d.\n", ret, last_error());
	}

	FreeLibrary(mod);
	return ret;
#else
	return -1;
#endif /* WIN32 */
}

}

int sp::local_ips(str_arr & lips)
{
	int ret = 0;
	static str_arr ip_list;

	lips.clear();

#ifdef USE_ACE
	ACE_Thread_Mutex lock("ISP_LOCAL_IPS_LOCK");
	ACE_GUARD_RETURN(ACE_Thread_Mutex, mon, lock, -1);
#endif /* USE_ACE */

	// 如果已经找过了，直接返回
	if ( !ip_list.empty() )
	{
		lips = ip_list;
		return 0;
	}

	// 先使用API获取，然后再使用原来的方法
	ret = get_local_ip_list(ip_list);
	if ( ret != 0 )
	{
		char hname[32];
		ret = gethostname(hname, ISP_MAXCOMPUTERNAME + 1);
		if ( -1 == ret )
		{
			ret = last_error();
			if ( debug_mode_ )
				printf("local_ips gethostname failed, last error %d.\n", ret);
			if ( ret == 0 )
				ret = -1;
			return ret;
		}

		// 获取本机地址
		hostent* pHost = gethostbyname(hname);
		for ( int i = 0; pHost != 0 && pHost->h_addr_list[i] != 0; i++ ) 
		{ 
			char ip[16]; ip[0] = 0;
			for ( int j = 0; j < pHost->h_length; j++ ) 
			{ 
				if( j > 0 )
					strcat(ip, ".");
				char tmp[16];
				sprintf( tmp, "%u",
					(unsigned int)((unsigned char*)pHost->h_addr_list[i])[j]); 
				strcat(ip, tmp); 
			}
			ip_list.push_back(ip);
		}

		// 如果gethostbyname失败，打印错误码
		if ( pHost == 0 )
		{
			printf("gethostbyname(%s) FAILED, code = %d.\n", hname, last_error());
			return -1;
		}
	}

	lips = ip_list;
	return ip_list.empty() ? -1 : 0;
}

int sp::local_ips(unsigned long ipa[], int& count)
{
	str_arr lips;
	local_ips(lips);

	int ret = 0;
	if ( count < (int)lips.size() )
		ret = -1;

	int nmax = ret ? count : (int)lips.size();
	for ( int i = 0; i < nmax; i++ )
	{
		#ifdef USE_ACE
		ipa[i] = ACE_INET_Addr(lips[i].c_str()).get_ip_address();
		#else
		ipa[i] = ::ntohl(inet_addr(lips[i].c_str()));
		#endif // ifdef WIN32
	}
	count = (int)lips.size();

	return ret;
}

int sp::ip_count(void)
{
	str_arr ips;
	local_ips(ips);
	return (int)ips.size();
}

int sp::net_adpt_count(void)
{
#ifdef WIN32
	static int netadp_num = 0;

	// 获取网卡数量的接口函数
	typedef DWORD (WINAPI *Proc_GetAdaptersInfo)(PIP_ADAPTER_INFO pAdapterInfo, PULONG pOutBufLen);
	Proc_GetAdaptersInfo pfnGetAdaptersInfo = 0;

#ifdef USE_ACE
	ACE_Thread_Mutex lock("ISP_LOCAL_NAN_LOCK");
	ACE_GUARD_RETURN(ACE_Thread_Mutex, mon, lock, -1);
#endif /* USE_ACE */

	if ( netadp_num != 0 )
		return netadp_num;

	HMODULE mod = LoadLibrary("iphlpapi.dll");
	if ( mod == 0 )
		return ip_count();

	pfnGetAdaptersInfo = (Proc_GetAdaptersInfo)GetProcAddress(mod, "GetAdaptersInfo");
	if ( pfnGetAdaptersInfo == 0 )
	{
		FreeLibrary(mod);
		return ip_count();
	}

	IP_ADAPTER_INFO* nic_info = 0;
	ULONG  dwSize = 0;
	int count = 0;

	// 获取网络适配器信息
	DWORD  ret = pfnGetAdaptersInfo(nic_info, &dwSize);

	// 重新分配内存
	if ( ret == ERROR_BUFFER_OVERFLOW )
	{
		count = dwSize / sizeof(IP_ADAPTER_INFO);
		nic_info = new IP_ADAPTER_INFO[count + 1];
		IU_ASSERT(nic_info);
		// 重新获取信息
		ret = pfnGetAdaptersInfo(nic_info, &dwSize);
	}

	delete [] nic_info;

	FreeLibrary(mod);
	netadp_num = count;
	return count;
#else /* WIN32 */
	return ip_count();
#endif /* WIN32 */
}

int sp::local_ip(unsigned long & ip)
{
	char ipsz[16];
	int ret = local_ip(ipsz);
	if ( ret == -1 )
		return -1;

	ip = inet_addr(ipsz);
	ip = ntohl(ip);
	IU_ASSERT(ip != INADDR_NONE);
	return 0;
}

int sp::local_ip(char* pszip)
{
	str_arr ips;
	int ret = local_ips(ips);
	if ( ips.size() > 0 )
		strcpy(pszip, ips[0].c_str());
	return ret;
}

bool sp::is_local_host(unsigned long ip_addr)
{
	int count = 16;
	unsigned long ipa[16];
	if ( local_ips(ipa, count) != 0 )
		return false;
	for ( int i = 0; i < count; i++ )
	{
		if ( ipa[i] == ip_addr )
			return true;
	}
	return ip_addr == INADDR_LOOPBACK;
}

bool sp::is_local_host(const char* ip_addr)
{
	char lip[16];
	if ( ! strcmp(ip_addr, "127.0.0.1" ) )
		return true;
	local_ip(lip);
	return ( ! strcmp(ip_addr, lip) );
}

bool sp::is_valid_ip_addr(const char* ip_addr)
{
	if ( ip_addr == 0 )
		return false;
	if ( strcmp(ip_addr, "0.0.0.0") == 0
		|| strcmp(ip_addr, "127.0.0.1") == 0 )
		return true;

	str_arr fds;
	split_str(ip_addr, fds, ".", true, true);
	if ( fds.size() != 4 )
		return false;

	int first = 0;
	for ( int i = 0; i < (int)fds.size(); i++ )
	{
		long fdl = strtol(fds[i].c_str(), 0, 0);
		if ( i == 0 )
			first = fdl;
		// 169.254.x.x是保留地址
		else if ( i == 1
			&& first == 169
			&& fdl == 254 )
			return false;

		if ( fdl == 255 )
			return false;
		if ( fdl == 0 && i == 0 )
			return false;
		if ( fdl == 0 && i == 3 )
			return false;
	}
	return true;
}

const char * sp::get_first_valid_ip_addr(const str_arr & ips)
{
	if ( ips.size() == 0 )
		return 0;

	for ( int i = 0; i < (int)ips.size(); i++ )
	{
		const std::string & ip_addr = ips[i];
		if ( is_valid_ip_addr(ip_addr.c_str()) )
			return ip_addr.c_str();
	}

	return ips.front().c_str();
}

int sp::get_sub_net_addr(const char * ip_addr, std::string & sub_net)
{
	sp::str_arr	fds;
	sp::split_str(ip_addr, fds, ".", true, true);
	
	sub_net.clear();
	if ( fds.size() != 4 )
		return -1;

	char type = 'd';

	// 这里还需要考虑地址类型A类、B类、C类
	for ( int i = 0; i < 4; i++ )
	{
		std::string & fd = fds[i];
		int fdl = strtol(fd.c_str(), 0, 0);
		if ( i == 0 )
		{
			if ( fdl < 128 )
				type = 'a';
			else if ( fdl < 192 )
				type = 'b';
			else if ( fdl < 224 )
				type = 'c';
		}
		
		sub_net += fd;
		sub_net += ".";

		if ( i == 0 && type == 'a' )
		{
			sub_net += "0.0.0";
			break;
		}
		else if ( i == 1 && type == 'b' )
		{
			sub_net += "0.0";
			break;
		}
		else if ( i == 2 && type == 'c' )
		{
			sub_net += "0";
			break;
		}
	}

	return 0;
}

bool sp::is_network_alive(const char * intf)
{
#ifdef WIN32
	// 获取网络状态的函数
	typedef BOOL (WINAPI *Proc_IsNetworkAlive)(LPDWORD lpdwFlags);
	Proc_IsNetworkAlive pfnIsNetworkAlive = 0;

#ifdef USE_ACE
	ACE_Thread_Mutex lock("ISP_LOCAL_NAN_LOCK");
	ACE_GUARD_RETURN(ACE_Thread_Mutex, mon, lock, true);
#endif /* USE_ACE */

	HMODULE mod = LoadLibrary("Sensapi.dll");
	if ( mod == 0 )
		return true;

	pfnIsNetworkAlive = (Proc_IsNetworkAlive)GetProcAddress(mod, "IsNetworkAlive");
	if ( pfnIsNetworkAlive == 0 )
	{
		FreeLibrary(mod);
		return true;
	}

	// 获取网络适配器信息
	DWORD net_flag = 0;
	BOOL  ret = pfnIsNetworkAlive(&net_flag);

	FreeLibrary(mod);
	return ret != FALSE
		&& net_flag == NETWORK_ALIVE_LAN;

#else /* Linux */
	#if 0
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <sys/ioctl.h>
	#include <net/if.h>
	#include <linux/sockios.h>
	#include <linux/types.h>

	struct mii_data { 
		__u16   phy_id; 
		__u16   reg_num; 
		__u16   val_in; 
		__u16   val_out; 
	}; 

	int skfd; 
	struct ifreq ifr; 
	struct mii_data *mii; 

	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
		perror("socket"); 
		exit(1); 
	} 

	strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1); 
	ifr.ifr_name[IFNAMSIZ-1] = 0; 

	if (ioctl(skfd, SIOCGMIIPHY, &ifr) < 0) { 
		perror("ioctl"); 
		exit(1); 
	} 

	mii = (struct mii_data *)&ifr.ifr_data; 
	mii->reg_num = 0x01; 

	if (ioctl(skfd, SIOCGMIIREG, &ifr) < 0) { 
		perror("ioctl"); 
		exit(1); 
	} 
	if (mii->val_out & 0x0004) 
		puts("link ok"); 
	else 
		puts("no link"); 

	close(skfd); 

	return 0; 
	#endif
	return true;
#endif /* WIN32 */
}

/*=============================================================================
 * 操作系统相关的实现
 *=============================================================================*/

bool sp::EnableDebugPrivNT()
{
#ifdef WIN32
	HANDLE hToken;
	LUID DebugValue;
	TOKEN_PRIVILEGES tkp;

	// Retrieve a handle of the access token
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken))
	{
		printf("OpenProcessToken failed with %d\n", GetLastError());
		return false;
	}

	// Enable the SE_DEBUG_NAME privilege
	if (!LookupPrivilegeValue((LPSTR)0, SE_DEBUG_NAME, &DebugValue))
	{
		printf("LookupPrivilegeValue failed with %d\n", GetLastError());
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = DebugValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken,
		false,
		&tkp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES) 0,
		(PDWORD) 0);

	// The return value of AdjustTokenPrivileges can't be tested
	if (GetLastError() != ERROR_SUCCESS)
	{
		printf("AdjustTokenPrivileges failed with %d.\n", GetLastError());
		return false;
	}
#endif
	return true;
}

#ifdef WIN32
class IWU_Context
{
public:
	IWU_Context(HWINSTA hsta, HDESK hdes, HWINSTA hsta_old, HDESK hdes_old)
		: hwinsta_save_(hsta_old),
		hdesk_save_(hdes_old),
		hwinsta_user_(hsta),
		hdesk_user_(hdes) { }
		HWINSTA	hwinsta_save_;
		HDESK	hdesk_save_;
		HWINSTA	hwinsta_user_;
		HDESK	hdesk_user_;
};
#endif // ifdef WIN32

IU_HANDLE sp::interact_with_user_open(const char* stat /* = "WinSta0") */, const char* desk /* = "Default") */)
{
#ifdef WIN32
	DWORD dwThreadId; 
	HWINSTA hwinstaSave; 
	HDESK hdeskSave; 
	HWINSTA hwinstaUser; 
	HDESK hdeskUser; 

	// Ensure connection to service window station and desktop, and 
	// save their handles. 

	GetDesktopWindow(); 
	hwinstaSave = GetProcessWindowStation(); 
	dwThreadId = GetCurrentThreadId(); 
	hdeskSave = GetThreadDesktop(dwThreadId); 

	// Impersonate the client and connect to the User's 
	// window station and desktop. 

	hwinstaUser = OpenWindowStation(stat, FALSE, MAXIMUM_ALLOWED); 
	if ( hwinstaUser == 0 ) 
		return 0; 

	SetProcessWindowStation(hwinstaUser); 
	hdeskUser = OpenDesktop(desk, 0, FALSE, MAXIMUM_ALLOWED); 
	if ( hdeskUser == 0 )
	{ 
		SetProcessWindowStation(hwinstaSave); 
		CloseWindowStation(hwinstaUser); 
		return 0; 
	} 
	SetThreadDesktop(hdeskUser);

	return new IWU_Context
		(hwinstaUser, hdeskUser, hwinstaSave, hdeskSave);
#else
	return (IU_HANDLE)1;
#endif
}

bool sp::interact_with_user_close(IU_HANDLE hiwu)
{
#ifdef WIN32
	if ( hiwu == 0 || IsBadWritePtr(hiwu, sizeof IWU_Context) )
		return false;

	// Restore window station and desktop. 
	IWU_Context* pcx = (IWU_Context*) hiwu;
	SetThreadDesktop(pcx->hdesk_save_);
	SetProcessWindowStation(pcx->hwinsta_save_);
	CloseDesktop(pcx->hdesk_user_);
	CloseWindowStation(pcx->hwinsta_user_);

	delete pcx;
	return true;
#else
	return true;
#endif
}

int sp::uname(sp::utsname * name)
{
#ifdef USE_ACE
	ACE_utsname au;
	int ret = ACE_OS::uname(au);
	if ( ret != 0 )
		return ret;

	// copy data
	sp::strsncpy(name->sysname,  au.sysname,  sizeof name->sysname);
	sp::strsncpy(name->nodename, au.nodename, sizeof name->nodename);
	sp::strsncpy(name->release,  au.release,  sizeof name->release);
	sp::strsncpy(name->version,  au.version,  sizeof name->version);
	sp::strsncpy(name->machine,  au.machine,  sizeof name->machine);

#  if defined (ACE_WIN32)
	ISP_BEGIN_REGREAD("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
	ISP_READ_REGSTR("ProductName", name->release);
	ISP_END_READREG;
	trim_str(name->release);

	DWORD dwMHz = 0;
	char szMHz[32];
	ISP_BEGIN_REGREAD("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");
	ISP_READ_REGDWORD("~MHz", dwMHz);
	if ( dwMHz != 0 )
	{
		sprintf(szMHz, " CPU %dMHz", dwMHz);
		strcat(name->machine, szMHz);
	}
	ISP_READ_REGSTR("ProcessorNameString", name->machine);
	ISP_END_READREG;
	trim_str(name->machine);
#  endif /* ACE_WIN32 */
	
	// 格式化
	int pos;
	std::string str(name->machine);
	char buf[16];
	size_t cpu_num = ACE_OS::num_processors();
	if ( cpu_num > 1 ) 
		sprintf(buf, ", %d*", cpu_num);
	else
		sprintf(buf, ", ");

	for ( pos = (int)str.find("(R)"); pos != -1; pos = (int) str.find("(R)") )
		str.replace(pos, 3, "", 0);
	for ( pos = (int)str.find("(TM)"); pos != -1; pos = (int) str.find("(TM)") )
		str.replace(pos, 4, "", 0);
	for ( pos = (int)str.find("  "); pos != -1; pos = (int) str.find("  ") )
		str.replace(pos, 2, " ", 1);
	for ( pos = (int)str.find(" CPU "); pos != -1; pos = (int) str.find(" CPU ") )
		str.replace(pos, 5, buf, strlen(buf));
    
	strcpy(name->machine, str.c_str());
#endif

	return 0;
}

int sp::make_daemon(void)
{
#ifndef WIN32
	pid_t pid;
	FILE* lockfd;
	sigset_t sighup;
	int i;

	extern pid_t getsid(pid_t);
	pid = fork();//第一个子进程生成
	if (pid < 0) {
		return -1;
	} else if (pid > 0) {
		printf("Starting daemon\n");
		exit(0);//退出父进程，摆脱shell的控制
	}
	setsid();//第一子进程成为新的会话组长和进程组长 
	if ( pid = fork() ) {//再次生成子进程，这时候是孙子进程
		exit(0);//退出上一代进程
	} else if ( pid < 0 ) {
		return -1;
	}

	close(1);
	close(2);
	//umask(0);//改变文件权限
	signal(SIGCHLD,SIG_IGN);
	return 0;
#endif
	return 0;
}

long sp::get_daemon_pid(const char* svc_name)
{
#ifndef WIN32
	FILE* fp = popen("/bin/ps -A", "r");
	if ( fp == 0 )
		fp = popen("/usr/bin/ps -A", "r");
	if ( fp != 0 )
	{
		str_arr arr;
		char szLine[1000];
		while ( !feof(fp) )
		{
			szLine[0] = 0;
			fgets(szLine, 1000, fp);

			arr.clear();
			split_str(szLine, arr, "\t ", true, true);

			// 大于等于COL列才读
			const int COL = 4;
			if ( arr.size() < COL )
				continue;

			// 第一列是pid，第四列是exe
			if ( strcmp(arr[COL-1].c_str(), svc_name)  == 0
				&& strcmp(arr[1].c_str(), "?") == 0 )
			{
				return strtol(arr[0].c_str(), 0, 10);
			}
		}
		pclose(fp);
	}
	return 0;
#endif
	return 0;
}

#if _MSC_VER >= 1400
#  pragma warning(pop)
#endif /* VC8 */
