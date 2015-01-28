/** 
 * @file	sulib.h
 * @brief	动态库加载的辅助类
 * 
 *  实现了Windows下和Linux下动态库的加载和释放，函数地址的获取功能
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2008/4/15
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2008/4/15	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __SULIB_H__
#define __SULIB_H__

#include "iFly_Assert.h"
#include "utils/sutils.h"
#include "log/srlog.h"

// Sun OS pack
#ifndef WIN32
#pragma pack(push)
#pragma pack(8)
#endif // sun

namespace sp {

/** 
 * @class	asc_loader
 * @brief	动态库加载辅助类
 * 
 *  实现Windows下的动态库加载
 * 
 * @author	jdyu
 * @date	2008/4/14
 * 
 * @see		
 */
#ifdef WIN32
class loader_win32
	{
public:
	typedef HMODULE handle;
	static int load(const char * mname, handle & hdl)
		{
		hdl = LoadLibraryA(mname);
		if ( hdl == NULL )
			return GetLastError();
		return ERROR_SUCCESS;
		}

	static int unload(handle hdl)
		{
		if ( hdl && !FreeLibrary(hdl) )
			return GetLastError();
		return 0;
		}

	static int proc_addr(handle hdl, const char * sym, void *& addr)
		{
		SP_ASSERT(hdl);
		SP_ASSERT(sym);
		addr = GetProcAddress(hdl, sym);
		if ( addr == NULL )
			return GetLastError();
		return 0;
		}
	};
typedef loader_win32 suloader;

#else  /* Non Win32 */
#include <dlfcn.h>
/** 
 * @class	loader_lnx
 * @brief	动态库加载辅助类
 * 
 *  实现Linux下的动态库加载
 * 
 * @author	jdyu
 * @date	2008/4/14
 * 
 * @see		
 */
class loader_lnx
	{
public:
	typedef void* handle;
	static int load(const char * mname, handle & hdl)
		{
		hdl = dlopen(mname,  /* RTLD_GLOBAL| */RTLD_LAZY);
		if ( hdl == NULL )
			return (int)dlerror();
		return 0;
		}

	static int unload(handle hdl)
		{
		if ( 0 != dlclose(hdl) )
			return (int)dlerror();
		return 0;
		}

	static int proc_addr(handle hdl, const char * sym, void *& addr)
		{
		addr = dlsym(hdl, (char*)(sym));
		if (addr == NULL)
			return (int)dlerror();
		return 0;
		}
	};
typedef loader_lnx suloader;
#endif /* end of lib loader */

/** 
 * @class	lib_loader
 * @brief	动态库加载器
 * 
 *  实现动态库加载功能
 * 
 * @author	jdyu
 * @date	2008/4/15
 * 
 * @see		loader_win32 loader_lnx
 */
template <class loader_os = suloader>
class lib_loader
	{
public:
	typedef suloader::handle handle;
	bool load(const char * mname)
		{
		err_ = loader_os::load(mname, hdl_);
		if ( err_ == 0 )
			++ ref_;
		if ( mname )
			lib_ = mname;
		nofree_ = false;
		return err_ == 0;
		}
	bool load(const wchar_t * mname)
		{
		return load(sp::wchar2char(mname).c_str());
		}
	bool load(handle hdl, bool no_free)
		{
		SP_ASSERT(hdl);
		hdl_ = hdl;
		nofree_ = no_free;
		return hdl != 0;
		}
	bool unload(void)
		{
		if ( hdl_ )
			{
			err_ = loader_os::unload(hdl_);
			if ( err_ == 0 )
				-- ref_;
			}
		hdl_ = 0;
		return err_ == 0;
		}

	template <typename fx_t>
	bool proc_addr(const char * sym, fx_t & addr)
		{
		addr = (fx_t) (proc_addr(sym));
		return ((void*)addr) != NULL;
		}

	template <typename fx_t>
	bool proc_addr(const wchar_t * sym, fx_t & addr)
		{
		addr = (fx_t) (proc_addr(sp::wchar2char(sym).c_str()));
		return ((void*)addr) != NULL;
		}

	void * proc_addr(const char* sym)
		{
		void * addr = NULL;
		if ( hdl_ == NULL || sym == NULL )
			{
			SP_ASSERT(0 && "Invalid handle, call load first.");
			return addr;
			}

		int ret = loader_os::proc_addr(hdl_, sym, addr);
		ret = ret;
		return addr;
		}

	bool loaded(void) const
		{
		return hdl_ != NULL;
		}
	handle mod_handle(void) const
		{
		return hdl_;
		}
	int error_code(void) const
		{
		return err_;
		}

	lib_loader(void)
		: hdl_		(0)
		, ref_		(0)
		, err_		(0)
		, nofree_	(false)
		{
		}
	explicit lib_loader(const char * mname)
		: ref_		(0)
		, err_		(0)
		, nofree_	(false)
		{
		hdl_ = loader_os::load(mname);
		}
	explicit lib_loader(handle hdl, bool no_free)
		: ref_		(0)
		, err_		(0)
		, hdl_		(hdl)
		, nofree_	(no_free)
		{
		}
	~lib_loader(void)
		{
		if ( hdl_ && !nofree_ )
			unload();
		SP_ASSERT(ref_ == 0);
		}

protected:
	handle			hdl_;
	std::string		lib_;
	unsigned int	ref_;
	int				err_;
	bool			nofree_;
	};

// loader
typedef lib_loader<> loader;

/** 
 * @class	su_proc
 * 
 * @brief	动态库的一个函数
 * 
 *  这个类提供了访问动态库的一个函数的实现
 * 
 * @author	jdyu
 * @date	2007-7-16
 * 
 * @see		
 */
template <typename proc_t, class char_t = char>
class su_proc
	{
public:
	template<class loader_t>
	int proc_addr(loader_t & loader, const char_t * proc_name)
		{
		loader.proc_addr(sp::tochar(proc_name).c_str(), proc_);
		name_ = proc_name;
		return proc_ == 0 ? -1 : 0;
		}
	const char_t * proc_name(void) const
		{
		return name_.c_str();
		}

	proc_t & operator () (void)
		{
		SP_ASSERTS(proc_, ("NULL function %s pointer", sp::tochar(name_.c_str()).c_str()));
		return proc_;
		}
	const proc_t & operator () (void) const
		{
		SP_ASSERTS(proc_, ("NULL function %s pointer", sp::tochar(name_.c_str()).c_str()));
		return proc_;
		}

	bool fail(void) const
		{
		return proc_ == 0;
		}
	operator bool (void) const
		{
		return proc_ != 0;
		}

	su_proc(void)
		: proc_ (0)
		{
		}

private:
	// m/w char type
	typedef std::basic_string<char_t> string;
	// procedure address
	proc_t		proc_;
	// procedure name
	string		name_;
	};

/** 
 * @class	su_lib
 * 
 * @brief	一个动态库加载器的基类
 * 
 *  为了简化代码而增加的一个动态库加载类
 * 
 * @author	jdyu
 * @date	2007-7-16
 * 
 * @see		
 */
template <class loader_t = loader, class char_t = char>
class su_lib
	{
public:
	typedef typename loader_t::handle handle;
	virtual int open(const char_t * lib)
		{
		if ( lib != 0 )
			lib_name_ = lib;
		SP_ASSERT(!lib_name_.empty());

		if ( !lib_loader_.load(sp::tochar(lib_name_.c_str()).c_str()) )
			{
			SP_ASSERTS(false,
				("su_lib::open | cannot load lib %s, errno %d.", sp::tochar(lib_name_.c_str()).c_str(), error_code()));
			return -1;
			}

		return load_procs();
		}
	virtual int open(handle hdl, bool no_free)
		{
		close();
		if ( !lib_loader_.load(hdl, no_free) )
			{
			SP_ASSERTS(false,
				("su_lib::open | cannot load lib 0x%x.", hdl));
			return -1;
			}
		return load_procs();
		}
	virtual int close(void)
		{
		bool ret = lib_loader_.unload();
		return ret ? 0 : sp::last_error();
		}

public:
	// query lib information
	const char_t * lib_name(void) const
		{
		return lib_name_.c_str();
		}
	void lib_name(const char_t * lib)
		{
		if ( lib )
			lib_name_ = lib;
		}
	loader_t & lib_loader(void)
		{
		return lib_loader_;
		}
	int error_code(void) const
		{
		return lib_loader_.error_code();
		}
	handle mod_handle(void) const
		{
		return lib_loader_.mod_handle();
		}
	bool loaded(void) const
		{
		return lib_loader_.loaded();
		}

	su_lib(const char_t * lib = 0)
		{
		if ( lib != 0 )
			lib_name_ = lib;
		}
	virtual ~su_lib(void)
		{
		close();
		}

protected:
	/// 加载每个函数地址，派生类必须实现这个函数
	virtual int load_procs(void) = 0;

protected:
	loader_t	lib_loader_;
	std::basic_string<char_t>	lib_name_;
	};

// null lib to load
template <class loader_t = loader, class char_t = char>
class lib_null
	{
public:
	typedef typename loader_t::handle handle;
	int open(const char_t * lib)
		{
		return 0;
		}
	int open(handle hdl, bool no_free)
		{
		return 0;
		}
	int close(void)
		{
		return 0;
		}
	lib_null(const char_t * lib = 0)
		{
		}
	};

// load procedure macros
#define su_proc_load_action(cmd_name, action)			\
	cmd_name##_.proc_addr(lib_loader_, #cmd_name);		\
	if ( cmd_name##_.fail() ) { action; }
#define su_proc_load_action_w(cmd_name, action)			\
	cmd_name##_.proc_addr(lib_loader_, L## #cmd_name);	\
	if ( cmd_name##_.fail() ) { action; }
#define su_proc_load_return(cmd_name)					\
	su_proc_load_action(cmd_name,	 srlog_error_assert(false, ("load proc %s failed.", #cmd_name)); return -1;)
#define su_proc_load(cmd_name)							\
	su_proc_load_action(cmd_name,	 srlog_warn( "load proc %s failed.", #cmd_name))
#define su_proc_load_w(cmd_name)						\
	su_proc_load_action_w(cmd_name,	 srlog_warn(L"load proc %s failed.", L## #cmd_name))

// procedure method
#define su_proc_addr_decl(cmd_name)						\
	sp::su_proc<Proc_##cmd_name, char>		cmd_name##_;
#define su_proc_addr_decl_w(cmd_name)					\
	sp::su_proc<Proc_##cmd_name, wchar_t>	cmd_name##_;
#define su_proc_decl(cmd_name)							\
	private:											\
		su_proc_addr_decl(cmd_name)						\
	public:												\
		Proc_##cmd_name cmd_name(void) { return cmd_name##_(); }
#define su_proc_decl_w(cmd_name)						\
	private:											\
		su_proc_addr_decl_w(cmd_name)					\
	public:												\
		Proc_##cmd_name cmd_name(void) { return cmd_name##_(); }

} /* namespace sp */

// Sun OS pack
#ifndef WIN32
#pragma pack(pop)
#endif // sun

#endif /* __SULIB_H__ */
