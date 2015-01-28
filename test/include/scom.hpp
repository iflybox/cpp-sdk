#ifndef __SCOM_INCLUDED
#define __SCOM_INCLUDED

/*
Usage:
	全局接口指针使用
		shptr<i_memmgnr>()->alloc(sizeof(int));
	或
		tts_allocator* pa = shptr<i_memmgnr>();
		if (ps != NULL)
			{
			pa->alloc(sizeof(int));
			}

		proc_create_engine pcr = shptr<proc_create_engine>();
		if (pce != NULL)
			{
			pce(NULL, &egn_ptr);
			}

Also:
	如果指针(如导出函数)多个模块都有定义，可以使用模块定位器来定位。
	struct mod_def
		{
		static const char* signature() { return "$mod_name"; }; // 注意这部分的写法。
		};
	proc_create_things pcr = shptr<proc_create_things, mod_def>();
	if (pcr != NULL)
		{
		pcr(....);
		}

	直接将一个singleton对象与接口指针关联起来。
	DEFINE_SHIMP(i_memmngr, tts_memmngr, unique);  // or multiple

	// unique, multiple, 某一个对象是否多份存在.
	// 是unique, 在对于指针进行赋值的时候, 如果已经存在的话, 则会出现assert.
	// 单体自动赋值的时候, 会自动保证单体的唯一性.

Requirements:
	1. 在头文件中命名接口API和接口对象。
	// assume: typedef int (*proc_create_engine)();
	DEFINE_SHPTR(proc_create_engine, "create_engine");
	DEFINE_SHPTR(i_memmngr, "memmngr");

	2. 在DLL实现cpp文件中添加如下语句
	IMPLEMENT_SHDLL("$module_name");
	// IMPLEMENT_SHDLL_1("$module_name", i_memmngr);	// 自动将singleton对象导出
	// IMPLEMENT_SHDLL_2("$module_name", i_memmngr, i_logger);

	3. DLL加载使用sd_loader
		sd_loader sdl("xxx.dll");
		sdl.load("xxx.dll");
		...
		sdl.unload();  卸载后，访问该模块的所有导出对象指针都会为空。

Platform:
	MSVC/Win32, gcc/Linux, Forte/SunOS
*/


// type 接口名或类名或api名, name 字符串型, 类型名称.
#define DEFINE_SHPTR(type, name) 									\
	namespace scom {												\
	template<> struct shptr_def<type> {								\
		static const char* signature() { return name; } };  }

#define DEFINE_SHIMP(ty, ty_impl, poly)									\
	namespace scom {													\
	template<> struct shptr_impl<ty> {									\
		static ty*	get_instance() { return &(ty_impl::instance()); };	\
		static const int objnum = poly::objnum; }; }

#include <cstddef>
#include <sstream>

#ifdef BOOST_ASSERT
# define SCOM_ASSERT BOOST_ASSERT
#else
#include <assert.h>
# define SCOM_ASSERT assert
#endif

#if defined _MSC_VER
#include <windows.h>

namespace scom {

class ld_proc_os
	{
public:
	typedef HMODULE handle;
	static int load(const char* mname, handle& h)
		{
		h = LoadLibraryA(mname);
		if (h == NULL)
			return GetLastError();
		return 0;
		}

	static int unload(handle h)
		{
		if (!FreeLibrary(h))
			return GetLastError();
		return 0;
		}

	static int proc_addr(handle h, const char*sym, void*& addr)
		{
		addr = GetProcAddress(h, (char*)sym);
		if (addr == NULL)
			return GetLastError();
		return 0;
		}

	static const char* errmsg(int errcode)
		{
		return NULL;
		}
	};
}	// end of scom namespace


#elif defined(__GNUC__) || defined(__SUNPRO_CC)
#include <dlfcn.h>
// Declare share object version, for every *.so library.
#define DEFINE_MOD_VERSION(x)	extern "C" {const char * ld_version_string = x;}
namespace scom {

class ld_proc_os
	{
public:
	typedef void* handle;
	static int load(const char* mname, handle& h)
		{
		h = dlopen(mname,  /* RTLD_GLOBAL| */RTLD_LAZY);
		if (h == NULL)
			return (int)dlerror();

		return 0;
		}

	static int unload(handle h)
		{
		if (0 != dlclose(h))
			return (int)dlerror();

		return 0;
		}

	static int proc_addr(handle h, const char*sym, void*& addr)
		{
		addr = dlsym(h, (char*)sym);
		if (addr == NULL)
			return (int)dlerror();
		return 0;
		}

	static const char* errmsg(int errcode)
		{
		return (const char*)errcode;
		}
	};
}	// end of scom namespace

#endif // end of compiler selection.


// the following section is the module implementation part.
#if defined(_MSC_VER)
	#define SHAPIIMPORT __declspec(dllimport)
	#define SHAPIEXPORT __declspec(dllexport)

#ifndef SHDLL_ENTRY
	#ifndef AFXAPI		// DLL used MFC already export DLLMain entry!!!
		#define SHDLL_ENTRY	\
			BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason, LPVOID lpReserved) {	\
				if (ul_reason == DLL_PROCESS_ATTACH) setlocale(LC_ALL, ".ACP");				\
				return TRUE; }
	#else
		#define SHDLL_ENTRY
	#endif
#endif
#else
	#define SHAPIIMPORT
	#define SHAPIEXPORT
	#define SHDLL_ENTRY
#endif // defined(_MSC_VER)

namespace scom {

struct null_def_type
	{
	static const char* signature() { return NULL; };
	};

template <typename Ty>
	struct shptr_def
	{
	static const char* signature();	// NOT IMPLEMENT THIS METHOD BY DESIGN.
	};

struct unique { static const int objnum = 1; };
struct multiple { static const int objnum = 10; };

struct dummy_shptr_type
	{
	};

template <typename Ty>
	struct shptr_impl
	{
	static Ty*	get_instance();	// NOT IMPLEMENT THIS METHOD BY DESIGN.
	static const int objnum = unique::objnum;
	};

template <typename sit>
	class singleton
	{
public:
	static sit& instance()
		{
		static sit sit_obj;
		return sit_obj;
		}
	};

class shdll_mod;
// assume: static variable of unique type is also unique even
//   if it's in a same template member method or a function.
template <typename Ty, typename modTy=null_def_type, typename shdll=shdll_mod>
	class shptr
	{
public:
	template <typename _Ty>
		struct ptr_type
		{
		typedef _Ty		Tyn;
		typedef _Ty*	Typ;
		static const bool query_api = false;
		};

	template <typename _Ty>
		struct ptr_type<const _Ty>
		{
		typedef _Ty		Tyn;
		typedef _Ty*	Typ;
		static const bool query_api = false;
		};

	template <typename _Ty>
		struct ptr_type<_Ty *>
		{
		typedef _Ty*	Tyn;
		typedef _Ty*	Typ;
		static const bool query_api = true;
		};

	typedef typename ptr_type<Ty>::Tyn	Tyn;	// non-const type
	typedef typename ptr_type<Ty>::Typ	Typ;
	static const bool query_api	= ptr_type<Ty>::query_api;

	typedef shptr_def<Tyn>				Tyd;
	typedef	shptr_impl<Tyn>				Tyl;

	template <typename _Pty, typename _Mty>
		class ptr_with_modtype
		{
	public:
		Typ ptr_;
		static ptr_with_modtype& ptr_inplace()
			{
			static ptr_with_modtype pwm;
			if (pwm.ptr_ == NULL)
				query_pointer((void**)(&pwm.ptr_));
			return pwm;
			}

		static void* query_pointer(void ** ppstr)
			{
			return shdll::instance().query_pointer(
				_Pty::signature(), _Mty::signature(), ppstr, query_api);
			}

		ptr_with_modtype()
			{
			ptr_ = NULL;
			}

		static void assign(const Typ& p0)
			{
			shdll::instance().insert_pointer(_Pty::signature(), p0, Tyl::objnum);
			query_pointer((void**)(&ptr_inplace().ptr_));
			}
		};

public:
	Typ operator->()
		{
		return ptr_ref_;
		}

	operator Typ() const
		{
		return ptr_ref_;
		}

	Typ operator=(const Typ& ptr)
		{
		ptr_with_modtype<Tyd, modTy>::ptr_inplace().assign(ptr);
		return ptr_ref_;
		}

	shptr(const Typ& ptr)
		: ptr_ref_(ptr_with_modtype<Tyd, modTy>::ptr_inplace().ptr_)
		{
		ptr_with_modtype<Tyd, modTy>::ptr_inplace().assign(ptr);
		}

	shptr()
		: ptr_ref_(ptr_with_modtype<Tyd, modTy>::ptr_inplace().ptr_)
		{
		}

	static void set(const Typ& ptr)
		{
		ptr_with_modtype<Tyd, modTy>::ptr_inplace().assign(ptr);
		}

protected:
	Typ& ptr_ref_;
	};

template <typename Ty, typename modTy=null_def_type, typename shdll=shdll_mod>
	class const_shptr
		: public shptr<const Ty, modTy, shdll>
		{
		};

template<typename t0>
struct init_singleton_h
	{
	init_singleton_h()
		{
		shptr<t0> pt = shptr_impl<t0>::get_instance();
		}
	};

template<>
struct init_singleton_h<dummy_shptr_type>
	{
	init_singleton_h()
		{
		}
	};

class i_shdll_mod
	{
public:
	virtual const char*		name() const = 0;
	virtual const char*		domain() const = 0;
	virtual i_shdll_mod*	prev_mod() const = 0;
	virtual i_shdll_mod*	next_mod() const = 0;
	virtual void*			find_pointer(const char* pname, bool query_api) const = 0;

	virtual bool			on_unload() = 0;

	// set/unset pointer to pointer which is be referenced by other module.
	virtual bool			set_refptr(i_shdll_mod* pmod, void** pptr) = 0;
	virtual bool			unset_refptr(i_shdll_mod* pmod, void** pptr= NULL) = 0;

	// set current module successor as sd_mod;
	virtual void 			prev_mod(i_shdll_mod* sd_mod) = 0;
	virtual void 			next_mod(i_shdll_mod* sd_mod) = 0;
	virtual	~i_shdll_mod() {};
	};

char const shdll_mod_env_name[] = "SHDLL_MOD_ENV_NAME";
char const pname_get_shdll_mod[] = "shdll_get_mod";
typedef i_shdll_mod* (*proc_shdll_get_mod)(i_shdll_mod* ld_mod, ld_proc_os::handle);

template <typename ld_proc=ld_proc_os, typename shdll=shdll_mod>
class sd_loader
	{
public:
	typedef typename ld_proc::handle handle;
	enum ld_stage
		{
		vacant = 0,
		ext_handle,		// external handle, which assigned in constructor.
		int_handle,		// internal handle, which assigned by loading image.
		joined,			// It's me who join this sh_mod in the ring.
		full			// I have done everything.
		};
	ld_stage	stage_tag_;

	bool load(const char* name)
		{
		unload();
		shdll& sdm = shdll::instance();

		int ret = ld_proc::load(name, module_h_);
		if (ret != 0)
			{
			err_msg_ = ld_proc::errmsg(ret);
			return false;
			}

		stage_tag_ = int_handle;
		// has been loaded before ?
		if (sdm.lookup((void*)module_h_))
			{
			return true;
			}

		stage_tag_ = joined;
		proc_shdll_get_mod pgsm = NULL;
		proc_addr(pname_get_shdll_mod, pgsm);
		if (pgsm == NULL)
			{
			stage_tag_ = full;
			sdm.create(module_h_, name);
			}
		else
			{
			pgsm(&sdm, module_h_);
			}
		return true;
		}

	bool unload()
		{
		if (stage_tag_ >= joined)
			{
			SCOM_ASSERT(module_h_ != NULL);
			i_shdll_mod* smp = shdll::instance().lookup((void*)module_h_);
#ifdef WIN32
			SCOM_ASSERT(smp != NULL);
			if (smp != NULL)
				smp->on_unload();
#else
			if (smp != NULL)
				smp->on_unload();
#endif
			if (stage_tag_ == full)
				shdll::instance().destroy(smp);
			stage_tag_ = int_handle;
			}

		if (stage_tag_ == int_handle)
			{
			int ret = ld_proc::unload(module_h_);
			if (ret != 0)
				{
				err_msg_ = ld_proc::errmsg(ret);
				return false;
				}
			}

		module_h_ = NULL;
		stage_tag_ = vacant;
		return true;
		}

	// try to use shptr<...>() instead of this method
	// if import/export function is unique.
	template <typename Fx>
	bool proc_addr(const char* sym, Fx& addr)
		{
		addr = (Fx)(proc_addr(sym));
		return ((void*)addr) != NULL;
		}

	void* proc_addr(const char* sym)
		{
		void* addr = NULL;
		if (module_h_ == NULL || sym == NULL)
			{
			SCOM_ASSERT(0 && "no image handle found. forget loading?");
			return addr;
			}

		int ret = ld_proc::proc_addr(module_h_, sym, addr);
		if (ret != 0)
			{
			err_msg_ = ld_proc_os::errmsg(ret);
			}
		return addr;
		}

	const char* last_error() const
		{
		return err_msg_;
		}

public:
	sd_loader(void* hm = NULL)
		{
		stage_tag_ = vacant;
		if (hm != NULL)
			{
			module_h_ = (handle)hm;
			stage_tag_ = ext_handle;
			}
		err_msg_ = NULL;
		}

	~sd_loader()
		{
		unload();
		}

protected:
	handle		module_h_;
	const char*	err_msg_;
	};

typedef sd_loader<> loader;
}	// end of scom namespace

#include <string>
#include <map>
#include <list>

namespace scom {
class shdll_mod
	: public i_shdll_mod
	{
public:
	void* query_pointer(const char* sname, const char* mname, void** pptr, bool query_api)
		{
		SCOM_ASSERT(sname != NULL && "unexpected parameter.");
		void* ptr = NULL;
		if (sname == NULL)
			{
			return ptr;
			}

		i_shdll_mod* mod = this;
		if (mname != NULL && *mname != '\x0')
			{
			// locate the specified module.
			std::string str_name = mname;
			while (str_name != mod->name())
				{
				mod = mod->next_mod();
				}
			}

		do {
			ptr = mod->find_pointer(sname, query_api);
			if (ptr != NULL)
				{
				if (pptr != NULL)
					{
					*pptr = ptr;
					mod->set_refptr(this, pptr);
					}
				return ptr;
				}

			if (mname != NULL && *mname != '\x0')
				return NULL;
			mod = mod->next_mod();
		}while(mod != this);

		return ptr;
		}

	void insert_pointer(const char* sname, void* ptr, int objnum)
		{
		SCOM_ASSERT(ptr != NULL && sname != NULL && "unexpected parameter.");
		if (ptr == NULL || sname == NULL)
			return;

		if (objnum == 1
			&& mod_arrptr_.find(sname) != mod_arrptr_.end())
			{
			SCOM_ASSERT(0 && "try to assign a singleton pointer more than once!");
			return;
			}
		mod_arrptr_.insert(ptrmap::value_type(sname, ptr));
		}

	i_shdll_mod* lookup(void* handle)
		{
		void* ptr = NULL;
		i_shdll_mod* mod = this;

		do {
			ptr = mod->find_pointer(NULL, false);
			if (handle == ptr)
				return mod;

			mod = mod->next_mod();
		}while(mod != this);

		return NULL;
		}

public:
	static shdll_mod& instance(i_shdll_mod* ld_mod = NULL,
		ld_proc_os::handle h_mod = NULL, const char* domain_name = NULL, const char* name = NULL)
		{
		static shdll_mod shmod;
		if (!shmod.joined())
			{
			SCOM_ASSERT(domain_name != NULL);
			// check environment variable.
			std::string str_env_name = shdll_mod_env_name;
			str_env_name += "_";
			str_env_name += domain_name;
#ifdef _MSC_VER
			char sm_temp[256];
			const char* sm_i = NULL;
			if (GetEnvironmentVariableA(
				str_env_name.c_str(), sm_temp, 256) > 0)
				{
				sm_i = sm_temp;
				}
#else
			const char* sm_i = getenv(str_env_name.c_str());
#endif // _MSC_VER
			if (sm_i == NULL || *sm_i == 0)
				{
				std::ostringstream oss;
				oss << std::hex << static_cast<const void*>(&shmod);
#ifdef _MSC_VER
				::SetEnvironmentVariableA(str_env_name.c_str(), oss.str().c_str());
#else
				setenv(str_env_name.c_str(), oss.str().c_str(), 1);
#endif  // _MSC_VER
				}
			if (ld_mod == NULL && ((sm_i != NULL)&&(*sm_i)))
				{
				std::istringstream iss(sm_i);
				void* up = NULL;
				iss >> std::hex >> up;
				SCOM_ASSERT(up != NULL);
				if (up != NULL)
					{
					ld_mod = (i_shdll_mod*)(up);
					}
				}
			shmod.init(ld_mod, h_mod, domain_name, name);
			}
		return shmod;
		}

	i_shdll_mod* create(ld_proc_os::handle h_mod, const char* mname)
		{
		shdll_mod* mp = new shdll_mod();
		mp->init(this, h_mod, domain(), mname);
		return mp;
		}

	bool destroy(i_shdll_mod*& smp)
		{
		if (smp == NULL)
			return false;
		delete smp;
		smp = NULL;
		return true;
		}

	bool joined() const
		{
		return joined_;
		}

	virtual const char* name() const
		{
		return mod_name_.c_str();
		}

	virtual const char* domain() const
		{
		return domain_name_.c_str();
		}

	virtual i_shdll_mod* prev_mod() const
		{
		return prev_mod_;
		}

	virtual i_shdll_mod* next_mod() const
		{
		return next_mod_;
		}

	virtual void* find_pointer(const char* pname, bool query_api) const
		{
		// is os_handle is valid, try search for it.
		if (pname == NULL)
			return os_handle_;

		void* ret_p = NULL;
		if (query_api && os_handle_ != NULL)
			{
			loader ldr(os_handle_);
			void* ret_p = ldr.proc_addr(pname);
			if (ret_p != NULL)
				return ret_p;
			}

		ptrmap::const_iterator it = mod_arrptr_.find(pname);
		if (it != mod_arrptr_.end())
			{
			ret_p = it->second;
			}
		return ret_p;
		}

	virtual bool on_unload()
		{
		if (!joined_)
			return true;

		// first, unset all ref_ptr;
		i_shdll_mod* pshmod = this;
		do {
			pshmod->unset_refptr(this);
			pshmod = pshmod->next_mod();
		} while(pshmod != this);

		// second, disjoin from the ring.
		prev_mod_->next_mod(next_mod_);
		next_mod_->prev_mod(prev_mod_);
		prev_mod_ = this;
		next_mod_ = this;

		// set all referenced pointer as NULL;
		ptrlist::iterator it = mod_refptr_.begin();
		for (; it != mod_refptr_.end(); it ++)
			{
			*(it->refptr) = NULL;
			}
		mod_refptr_.clear();

		os_handle_ = NULL;
		joined_ =  false;
		return true;
		}

	// set pointer to pointer which is be referenced by other module.
	virtual bool set_refptr(i_shdll_mod* pm, void** pptr)
		{
		mod_refptr_.push_back(refptr_mod(pm, pptr));
		return true;
		}

	virtual bool unset_refptr(i_shdll_mod* pm, void** pptr=NULL)
		{
		ptrlist::iterator it = mod_refptr_.begin();
		while(it != mod_refptr_.end())
			{
			ptrlist::iterator itn = it;
			it ++;
			if (itn->modptr != pm)
				continue;
			if (pptr != NULL && pptr != itn->refptr)
				continue;
			mod_refptr_.erase(itn);
			}
		return true;
		}

	virtual void prev_mod(i_shdll_mod* sd_mod)
		{
		prev_mod_ = sd_mod;
		}

	virtual void next_mod(i_shdll_mod* sd_mod)
		{
		next_mod_ = sd_mod;
		}

protected:
	typedef std::map<std::string, void*> ptrmap;
	struct refptr_mod
		{
		i_shdll_mod*	modptr;
		void**			refptr;
		refptr_mod(
			i_shdll_mod* p1,void** pp2)
			: modptr(p1), refptr(pp2)
			{
			}
		};
	typedef			std::list<refptr_mod>				ptrlist;

	ptrmap			mod_arrptr_;
	ptrlist			mod_refptr_;
	std::string		mod_name_;
	std::string		domain_name_;
	i_shdll_mod* 	next_mod_;
	i_shdll_mod*	prev_mod_;
	void*			os_handle_;
	bool			joined_;

	shdll_mod()
		{
		os_handle_ = NULL;
		next_mod_ = this;
		prev_mod_ = this;
		joined_ = false;
		}

	~shdll_mod()
		{
		on_unload();
		}

	bool init(i_shdll_mod* ld_mod,
		ld_proc_os::handle hmod, const char* domain_name, const char* name)
		{
		if (hmod != NULL)
			os_handle_ = hmod;

		if (joined())
			{
			// disjoin from the ring.
			prev_mod_->next_mod(next_mod_);
			next_mod_->prev_mod(prev_mod_);
			prev_mod_ = this;
			next_mod_ = this;
			}

		if (domain_name)
			domain_name_ = domain_name;
		if (name)
			mod_name_ = name;

		if (ld_mod != NULL && ld_mod != this)
			{
			next_mod_ = ld_mod;
			prev_mod_ = ld_mod->prev_mod();
			prev_mod_->next_mod(this);
			next_mod_->prev_mod(this);
			}

		joined_ = true;
		return true;
		}
	};

template   <typename t0 = dummy_shptr_type, typename t1 = dummy_shptr_type,
			typename t2 = dummy_shptr_type, typename t3 = dummy_shptr_type,
			typename t4 = dummy_shptr_type, typename t5 = dummy_shptr_type,
			typename t6 = dummy_shptr_type, typename t7 = dummy_shptr_type,
			typename t8 = dummy_shptr_type, typename t9 = dummy_shptr_type>
struct initialize
	{
	initialize(i_shdll_mod* ld_mod,
		ld_proc_os::handle h_mod, const char* dname, const char* mname)
		{
		if (dname == NULL && ld_mod != NULL)
			dname = ld_mod->domain();
		scom::shdll_mod::instance(ld_mod, h_mod, dname, mname);

		init_singleton_h<t0>();
		init_singleton_h<t1>();
		init_singleton_h<t2>();
		init_singleton_h<t3>();
		init_singleton_h<t4>();
		init_singleton_h<t5>();
		init_singleton_h<t6>();
		init_singleton_h<t7>();
		init_singleton_h<t8>();
		init_singleton_h<t9>();
		}
	};

inline const char* get_dname()
	{
	return shdll_mod::instance().domain();
	}

inline const char* get_mname()
	{
	return shdll_mod::instance().name();
	}
}	// end of scom namespace

#define IMPLEMENT_SHDLL_BASE(modname, inis)								\
	extern "C" SHAPIEXPORT scom::i_shdll_mod* shdll_get_mod(			\
		scom::i_shdll_mod* ld_mod, scom::ld_proc_os::handle h_mod) {	\
		static bool inited = false;										\
		if (!inited) {													\
			inited = true;												\
			inis(ld_mod, h_mod, NULL, modname);	}						\
		return &scom::shdll_mod::instance(); }							\
	SHDLL_ENTRY;

#define IMPLEMENT_SHDLL(modname)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<>)

#define IMPLEMENT_SHDLL_1(modname, t0)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0>)

#define IMPLEMENT_SHDLL_2(modname, t0, t1)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0, t1>)

#define IMPLEMENT_SHDLL_3(modname, t0, t1, t2)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0, t1, t2>)

#define IMPLEMENT_SHDLL_4(modname, t0, t1, t2, t3)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0, t1, t2, t3>)

#define IMPLEMENT_SHDLL_5(modname, t0, t1, t2, t3, t4)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0, t1, t2, t3, t4>)

#define IMPLEMENT_SHDLL_6(modname, t0, t1, t2, t3, t4, t5)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0, t1, t2, t3, t4, t5>)

#define IMPLEMENT_SHDLL_7(modname, t0, t1, t2, t3, t4, t5, t6)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0, t1, t2, t3, t4, t5, t6>)

#define IMPLEMENT_SHDLL_8(modname, t0, t1, t2, t3, t4, t5, t6, t7)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0, t1, t2, t3, t4, t5, t6, t7>)

#define IMPLEMENT_SHDLL_9(modname, t0, t1, t2, t3, t4, t5, t6, t7, t8)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0, t1, t2, t3, t4, t5, t6, t7, t8>)

#define IMPLEMENT_SHDLL_10(modname, t0, t1, t2, t3, t4, t5, t6, t7, t8, t9)	\
	IMPLEMENT_SHDLL_BASE(modname, scom::initialize<t0, t1, t2, t3, t4, t5, t6, t7, t8, t9>)

#endif // __SCOM_INCLUDED
