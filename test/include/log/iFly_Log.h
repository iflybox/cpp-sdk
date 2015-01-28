/*=============================================================================================================*/
/** 
 * @file	iFly_Log.h
 * @brief	��־��������ļ�
 * 
 * �����־����һ�������õĵ���־�࣬�û����Ե�ʹ���ھ������C/C++Ӧ�ó����С�Ϊ�û��ṩ�˷ǳ�����Ĳ���
 * ���䷽�����û�����ʹ�������ṩ�Ķ��ֲ���ģʽ��Ҳ�����Լ���չ���ԣ�����IO���ԡ����ö�ȡ���ԡ��������Եȡ�
 * ��Ҫ���ܣ�
 *		1.  �ֵȼ���¼��Ϣ���ؼ�����Ϣ�����桢���󡢵�����Ϣ��������Ϣ��
 *		2.  ���������ʽ���ļ�������̨����������MessageBox������ڽ��������ʹ�ÿ���̨��������Զ�����һ������̨��
 *		3.  �ṩ�߳̽��̼���ȫ��֧�ֶ��̡߳������ʹ��һ����־��
 *		4.  ֧�ִ�ע���������ļ���ȡ������Ϣ��֧��ָ���Զ����section����
 *		5.  ֧�ָ߾��ȵ�����ͳ�ƹ��ܣ�֧�ֺ�������ʱ����Զ�ͳ�ƣ�
 *		6.  ֧��Windows��Linux��Solaris����ϵͳ��֧��VC6/7/8��GCC��������
 *		7.  ֧�������������˹��ܣ�Ϊ��ʵ��������ܣ���Ҫ�������ಢ����subject()������
 *		8.  Ӧ����singletonģʽ�������Ե�ʵ��ʹ�ã�ͬʱ֧�ַǵ����ʹ�ã�
 *		9.  ֧���û��Զ���Ļ����������������������߳�����������������Windows���ȣ�
 *		10. ֧���û��Զ������ö�ȡ������Ŀǰ֧��ע����ini��ʽ�������ļ���
 *		11. ֧���û�����ʵ����־��IO������Ŀǰ�ṩ�Ľ��л����ļ���IOʵ�֣��û����Կ���TCP��ʽ���ܵ���ʽ�ȣ�
 *		12. ֧�ֵ������ļ������ı�ʱ�����¶�ȡ���õĹ��ܣ�����ʵʱ������¼���������ʽ��
 *		14. �ṩ�˶�����ʽ�ɹ�ѡ�񣬰���ʱ�䡢���⡢�ȼ����߳�ID������ID���Զ��س���������ϣ�
 *		15. ������������ͷ�ļ���ֱ�ӿ���Ȼ��include�Ϳ���ʹ�ã�
 *		16. ���ڽӿڵ�ģ����Žӻ��ƣ�ʹ����־�Ĵ�����²���Ҫ���±���ǣ����ģ�飬���ģ��֮�����
 *			ʵ�ְ�ȫ����־��������
 *		17. ֧����־�Զ����ݻ��ƣ�֧����־��С�����������ƣ�����־�����ﵽһ��������ȱʡ20��ʱ��
 *			���Զ�ɾ�����籸�ݵ��ļ���
 *
 * �÷�������
 *      1. �����Ҫһ������Ҫ���������ù��ܵ��ļ���־�࣬���������÷���
 *			// ��Ҫ����ģʽ
 *			typedef Log_Singleton<Log_IO_FILE, Log_Mutex_Null, Log_Cfg_T<Log_Cfg_Heap> > XXX_Logger;
 *			// ��Ҫ����ģʽ��
 *			typedef Log_Impl_T<Log_IO_FILE, Log_Win32_Mutex, Log_Cfg_T<Log_Cfg_Heap> > XXX_Logger;
 *
 *		2. �����Ҫһ����Win32�½���������ע���ini�ļ����ù��ܵ��ļ���־�࣬�������£�
 *			typedef Log_Singleton<Log_IO_FILE, Log_Win32_Process_Mutex, Log_Cfg_T<Log_Cfg_Heap_Reg> > XXX_Logger;
 *
 *		3. �û����Դ�Log_Impl_T������ʵ���Լ���Ҫ�Ĺ��ܣ�
 *			class XXX_Log : public Log_Impl_T<IO, MUTEX, CFG> { ... }
 *			��������ʵ�ֵĺ�����Log_Impl_T���У�
 *
 *		4. ��������ʹ�õ�ʱ���ж��open����
 *			open(const CFG & cfg, const char * cfg_file = 0, const char * key = 0);
 *			open(const char* cfg_file, const char* key = 0,  const CFG* cfg_p = 0, LOG_HANDLE hmod = 0, void* rev = 0);
 *			open(const CFG & cfg, Log_IO * io_impl = 0, void * rev = 0);
 *			open(const char* io_lib,   const char* cfg_file, const char* key, const CFG* cfg_p, LOG_HANDLE hmod, void* rev);
 *			���е�һ����������õķ�������ʱ�û���Ҫ����һ�������ļ���·����
 *
 *		5. �򻯵ĵ���������ʽ��
 *		   (1) LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Mutex_Null, Log_Cfg_Heap_Reg, XXX_Logger)
 *             �����������һ��ʹ���ļ�IO��ʹ�ý���������ʹ��ע���ini��ȡ����־����
 *		   (2) LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Mutex_Process, ISP_Log_Cfg_Heap, ISP_Logger)
 *			   �����������һ��������־��ʹ���ļ���ʽ�����������ܹ���ȡcfg�ļ��������࣬����־�������ΪISP_Logger
 *		   (3) LOG_DECL_SIMPLE(Log_IO_FILE, Log_Mutex_Null, IFLY_Logger)
 *			   ����һ�����Ӽ򻯵�������ʽ
 *
 *		6. ��һ��ʵ�ʵ����ӣ��û���Ҫһ������Ϊ My_Logger �ĵ�����־���������£�
 *			// ��һ�����õ�ͷ�ļ�������������
 *			LOG_DECL_SINGLETON_EX(Log_IO_FILE, Log_Win32_Process_Mutex, Log_Cfg_Heap_Reg, My_Logger);
 *			...
 *			// �ڳ����ʼ��ʱ��ʼ����־
 *			My_Logger_Cfg default_cfg("my.log", "My logging information");
 *			My_Logger::open("my.cfg", "logger", &default_cfg);
 *			...
 *			// ����Ϳ�������ʹ����
 *			My_Logger::instance()->log_error("This is a log msg, line %d.", __LINE__);
 *			...
 *			// �������˳�ʱ�����˹ر���־
 *			My_Logger::close();
 *
 *		7. ����û���ʱʹ��һ��(�����ȡ�����ļ���ע���Ҳ����ʹ��ͬ��)�Ļ�����ʵ����һ���ļ����������·�����
 *			// ����������
 *			typedef Log_Impl_T<Log_IO_FILE> My_Logger;
 *			// ��Ҫʹ�õ�ʱ��
 *			My_Logger log("c:\\test.log", "Test Log");
 *			log.log_error("This is a error line.");
 *
 *		8. ����û���ʱʹ�ã���Ҫ��ȡ�����ļ���ע����������·�����
 *			// ���������ͣ�ע��������õ����Ϳ���֧��ini�ļ���ע���
 *          typedef Log_Cfg_T<Log_Cfg_Heap_Reg> My_Logger_Cfg;
 *			typedef Log_Impl_T<Log_IO_FILE, Log_Thread_Mutex, My_Logger_Cfg> My_Logger;
 *			// ��Ҫʹ�õ�ʱ��
 *			My_Logger_Cfg cfg("c:\\test.log", "Test Log");
 *			cfg.read_config("logger", "my_logger.cfg");
 *			My_Logger log;
 *			log.open(&cfg);
 *			log.log_error("This is a error line.");
 *			log.close();
 *
 *		9. ��򵥵�ʹ�÷������ǰѱ�ͷ�ļ�������Щ�궨�忽�����Լ���ͷ�ļ��У��ĳ��Լ�����
 *		   �֣�ֱ�ӾͿ���ʹ���ˡ�
 *
 * ע�����
 *		1.  �ڵ��巽ʽʹ�ñ���־��ʱ�������һ����������д��ڶ����̬�⹤�̣���ע��Ϊ���ÿ����̬��
 *			�е���־����ָ��һ��Ψһ�����֣���̬��֮��ͨ��log_io�ӿڽ������ӣ�
 *			�ٸ����ӣ�
 *			LOG_DECL_SINGLETON_EX(io, lock, cfg, name) ���е�name��������Ҫȡ�����֡�
 *		2.  �ڷ���bug���߶���־�Ĺ����кý����ʱ���벻Ҫ�Լ��޸Ĵ��룬�������ϵ���Ա�ͳһά�����ʼ�
 *			jdyu@iflytek.com��лл��
 *		3.  Just enjoy it!
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2004-05-13
 * 
 * <b>�汾��¼��</b><br>
 * <table>
 *  <tr> <th>�汾	<th>����			<th>����	<th>��ע </tr>
 *  <tr> <td>1.0	<td>2002-08-31		<td>jdyu	<td>�����ļ� </tr>
 *  <tr> <td>1.0	<td>2005-10-12		<td>jdyu	<td>��־���ķ�װ��</tr>
 *  <tr> <td>1.0	<td>2008-04-08		<td>jdyu	<td>����˵������</tr>
 * </table>
 */

#ifndef __IFLYLOGGER_H__
#define __IFLYLOGGER_H__

// disable vc6 warning
#if _MSC_VER < 1300
# pragma warning(disable:4786)
#endif /* _MSC_VER < 1300 */

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#ifdef WIN32
#  if !defined (_WIN32_WINNT)
#   define _WIN32_WINNT 0x0400
#  endif // _WIN32_WINNT
# include <io.h>
# include <WinSock2.h>
# include <Windows.h>
#else /* Linux */
# include <sys/time.h>
# include <stdarg.h>
# include <pthread.h>
#endif /* WIN32 */

#if _MSC_VER > 1000
# pragma once
#endif /* _MSC_VER > 1000 */

/* cdecl */
#ifdef WIN32
# define LOG_CDECL __cdecl
#else
# define LOG_CDECL
#endif

/* ������͵Ķ��� */
typedef void *					LOG_HANDLE;
typedef std::pair				<std::string, std::string> LOG_STR_PAIR;
typedef std::vector				<LOG_STR_PAIR> LOG_STRPAIR_ARR;

/* Invalid handle */
#define LOG_INVALID_HANDLE		((LOG_HANDLE)-1)

/* size_t */
#ifndef _SIZE_T_DEFINED
# ifdef  _WIN64
   typedef unsigned __int64		size_t;
# else
   typedef unsigned int			size_t;
# endif
# define _SIZE_T_DEFINED
#endif /* _SIZE_T_DEFINED */

/* time_t */
#ifndef _TIME_T_DEFINED
# ifdef  _WIN64
   typedef __int64 time_t;     /* time value */
# else
#  ifndef _AIX
    typedef long    time_t;     /* time value */
#  endif
# endif
# define _TIME_T_DEFINED
#endif /* _TIME_T_DEFINED */

/* �Ƿ����־�ĺ� */
#define IFLY_LOG_ENABLE			1

/* ���д����Ϣ���� */
#ifdef _AIX
 const int LOG_MAXMSGLEN		= 4 * 1024;
#else
 const int LOG_MAXMSGLEN		= 80 * 1024;
#endif

/* �����־�ļ����� */
const int LOG_MAXFILELEN		= 10 * 1024 * 1024;
/* �����־���� */
const int LOG_MAXCOUNT			= 20;
/* ȱʡ��־�ļ� */
#define LOG_DEFFILE				"ifly.log"
/* ȱʡ���� */
#define LOG_DEFTITLE			"Running Information"
/* ������ñ仯�ļ��ʱ��(��) */
const int LOG_CFGCHECK_TIME		= 10;
/* �����ļ�ȱʡ�ļ����� */
#define LOG_DEFKEY				"logger"


/* ��־��¼�ļ��� */
enum LOG_LEVEL
{
	lgl_none		= 0,		/* ����� */
	lgl_crit		= 1,		/* �ؼ���Ϣ */
	lgl_error		= 2,		/* ������Ϣ */
	lgl_warning		= 4,		/* ������Ϣ */
	lgl_low			= lgl_warning | lgl_error | lgl_crit,	/* ������� - ������Ϣ */
	lgl_info		= 8,		/* һ����Ϣ */
	lgl_normal		= lgl_low | lgl_info,					/* ������� - һ����Ϣ */
	lgl_verbose		= 16,		/* ����Ҫ����Ϣ */
	lgl_debug		= 32,		/* ������Ϣ����Ϣ����� */
	lgl_detail		= lgl_normal | lgl_verbose | lgl_debug,	/* ������� - ������Ϣ */
	lgl_perf		= 64,		/* ������Ϣ����Ϣ���� */
	lgl_trace		= 128,		/* ������Ϣ */
	lgl_spec		= 256,		/* ���⼶�������ر���; */
	lgl_all			= -1,		/* ������Ϣ */
};

/* ��־��¼�������ʽ */
enum LOG_OUTPUT
{
	lgo_none		= 0,		/* ����� */
	lgo_file		= 1,		/* �ļ���ʽ��� */
	lgo_console		= 2,		/* ���������̨ */
	lgo_debugger	= 4,		/* ����������� */
	lgo_msgbox		= 8,		/* �������Ϣ�� */
};

/* ��¼��־����ʽ */
enum LOG_STYLE
{
	lgs_default		= 0x00,		/* ȱʡ��ʽ */
	lgs_time_hms	= 0x01,		/* �򵥵�ʱ�� */
	lgs_time		= 0x02,		/* ����ʱ�� */
	lgs_level		= 0x04,		/* ��������ȼ� */
	lgs_threadid	= 0x08,		/* �����߳�ID */
	lgs_crlf		= 0x10,		/* �����س� */
	lgs_subject		= 0x20,		/* �������� */
	lgs_precessid	= 0x40,		/* ��������ID */
	lgs_none		= 0x100,	/* ��������Ϣ */
	lgs_all			= 0x0FF,	/* ������Ϣ */

	lgs_short		= lgs_time_hms | lgs_level | lgs_threadid | lgs_crlf | lgs_subject,
	lgs_long		= lgs_time | lgs_level | lgs_threadid | lgs_precessid | lgs_crlf | lgs_subject,
};

/* ��־�Ĵ�ģʽ */
enum LOG_MODE
{
	lgm_text		= 0,		/* �ı�׷��ģʽ */
	lgm_binary		= 1,		/* ������׷��ģʽ */
	lgm_text_ow		= 2,		/* �ı�����ģʽ */
	lgm_binary_ow	= 3,		/* �����Ƹ���ģʽ */
};

/* ��־�Ŀ�խ�ַ����� */
enum LOG_CHARSET
{
	lgc_auto		= 0,		/* �Զ� */
	lgc_mbcs		= 1,		/* խ�ַ� */
	lgc_wchar		= 2,		/* ���ַ� */
};

/** 
 * @class	Log_Cfg_Heap
 * 
 * @brief	���ö�ȡ��
 * 
 *  ���ﶨ����һ�����ö�ȡ��д��ķ�װ���
 *  ��ʵ������ʱ���û�����ָ��һ����ȡ��ʵ�ַ�ʽ
 * 
 * @author	jdyu
 * @date	2005-10-28
 * 
 */
struct Log_Cfg_Heap
{
	int open(const char* /* section */, const char* /* file */ = 0)
		{ return 0; }
	int close(void)
		{ return 0; }
	int get_string_value(const char* /* key */, std::string & /* value */, const char* /* sub */ = 0)
		{ return 0; }
	int set_string_value(const char* /* key */, const char* /* value */, const char* /* sub */ = 0)
		{ return 0; }
	int get_string_arr  (const char* /* key */, std::vector<std::string> & /* arr */)
		{ return 0; }
};

/** 
 * @class	 Log_Cfg
 * 
 * @brief	���ýӿ�
 * 
 *  ���ﶨ����һ�����ýӿڣ�����ģ���Ĵ���
 * 
 * @author	jdyu
 * @date	2005-10-28
 * 
 */
struct Log_Cfg
{
	virtual const char * file_name		(void) const = 0;
	virtual const char * title			(void) const = 0;
	virtual const char * cfg_file		(void) const = 0;
	virtual const char * locale			(void) const = 0;
	virtual size_t		 max_size		(void) const = 0;
	virtual bool		 overwrite		(void) const = 0;
	virtual int			 output			(void) const = 0;
	virtual int			 level			(void) const = 0;
	virtual int			 style			(void) const = 0;
	virtual int			 filter			(void) const = 0;
	virtual bool		 flush_always	(void) const = 0;
	virtual int			 max_count		(void) const = 0;
	virtual int			 mode			(void) const = 0;
	virtual int          perf_thres     (void) const = 0;
	virtual bool		 has_head		(void) const = 0;
	virtual void *		 module			(void) const = 0;
	virtual int			 console_level	(void) const = 0;
	virtual LOG_CHARSET	 charset		(void) const = 0;
	virtual time_t		 start_time		(void) const = 0;
	virtual unsigned int start_tick		(void) const = 0;
	virtual const char * log_server		(void) const = 0;
};

/* ��־��������Ϣ */
template <class CFG_HEAP = Log_Cfg_Heap, typename BASE = Log_Cfg>
class Log_Cfg_T
	: public BASE
{
public:
	Log_Cfg_T(const char* file = LOG_DEFFILE, const char* ttl = LOG_DEFTITLE, int md = lgm_text)
		: max_size_		(LOG_MAXFILELEN)
		, max_count_	(20)
		, overwrite_	(0)
		, output_		(lgo_file)
		, level_		(lgl_low)
		, style_		(lgs_all)
		, filter_		(-1)
		, flush_always_	(false)
		, has_head_		(true)
		, synch_time_	(false)
		, module_		(0)
		, cfg_mtime_	(0)
		, section_		("logger")
		, mode_			(md)
		, encrypt_		(0)
		, locale_		(".ACP")
		, console_level_(level_)
		, charset_		(lgc_auto)
		, start_time_	(0)
		, start_tick_	(0)
		, perf_thres_	(0)
		{
		file_name(file);
		title(ttl);
		if ( md == lgm_binary || md == lgm_binary_ow )
			style_ = lgs_none;
		}
	Log_Cfg_T(const BASE * cfg)
		{
		this->set(cfg);
		}

	Log_Cfg_T & operator = (const Log_Cfg_T & cfg)
		{
		max_size_	  = cfg.max_size_;
		max_count_	  = cfg.max_count_;
		overwrite_	  = cfg.overwrite_;
		output_		  = cfg.output_;
		level_		  = cfg.level_;
		style_		  = cfg.style_;
		filter_		  = cfg.filter_;
		module_		  = cfg.module_;
		file_name_	  = cfg.file_name_;
		title_		  = cfg.title_;
		flush_always_ = cfg.flush_always_;
		has_head_	  = cfg.has_head_;
		synch_time_	  = cfg.synch_time_;
		cfg_file_	  = cfg.cfg_file_;
		section_	  = cfg.section_;
		cfg_mtime_	  = cfg.cfg_mtime_;
		mode_		  = cfg.mode_;
		encrypt_	  = cfg.encrypt_;
		locale_		  = cfg.locale_;
		console_level_= cfg.console_level_;
		charset_	  = cfg.charset_;
		start_time_	  = cfg.start_time_;
		start_tick_	  = cfg.start_tick_;
		perf_thres_	  = cfg.perf_thres_;
		log_server_	  = cfg.log_server_;
		includes_.assign(cfg.includes_.begin(), cfg.includes_.end());
		excludes_.assign(cfg.excludes_.begin(), cfg.excludes_.end());
		return *this;
		}

	const char* file_name(void) const
		{ return file_name_.c_str(); }
	void file_name(const char* fn)
		{ if ( fn != 0 ) file_name_ = fn; }

	const char* title(void) const
		{ return title_.c_str(); }
	void title(const char* tl)
		{ if ( tl ) title_ = tl; }

	size_t max_size(void) const
		{ return max_size_; }
	void max_size(size_t size)
		{ max_size_ = size; }

	bool overwrite(void) const
		{ return overwrite_; }
	void overwrite(bool ow)
		{ overwrite_ = ow; }

	int output(void) const
		{ return output_; }
	void output(int op)
		{ output_ = op; }

	int level(void) const
		{ return level_; }
	void level(int lvl)
		{ console_level_ = level_ = lvl; }

	int style(void) const
		{ return style_; }
	void style(int stl)
		{ style_ = stl; }

	int filter(void) const
		{ return filter_; }
	void filter(int flt)
		{ filter_ = flt; }

	bool flush_always(void) const
		{ return flush_always_; }
	void flush_always(bool fa)
		{ flush_always_ = fa; }

	bool synch_time(void) const
		{ return synch_time_; }
	void synch_time(bool st)
		{ synch_time_ = st; }

	void * module(void) const
		{ return module_; }
	void module(void* mod)
		{ module_ = mod; }

	int max_count(void) const
		{ return max_count_; }
	void max_count(int maxc)
		{ if ( maxc > 0 ) max_count_ = maxc; }

	int mode(void) const
		{ return mode_; }
	void mode(int md)
		{ mode_ = md; }

	bool has_head(void) const
		{ return has_head_;	}
	void has_head(bool hhd)
		{ has_head_ = hhd; }

	const char * locale(void) const
		{ return locale_.c_str(); }
	void locale(const char * lc)
		{ if ( lc ) locale_ = lc; }

	int encrypt(void) const
		{ return encrypt_;	}
	void encrypt(int enc)
		{ encrypt_ = enc;	}

	int console_level(void) const
		{ return console_level_; }
	void console_level(int cl)
		{ console_level_ = cl; }

	LOG_CHARSET charset(void) const
		{ return charset_; }
	void charset(LOG_CHARSET cs)
		{ charset_ = cs; }

	time_t start_time(void) const
		{ return start_time_; }
	void start_time(time_t tm)
		{ start_time_ = tm; }
	unsigned int start_tick(void) const
		{ return start_tick_; }
	void start_tick(unsigned int tick)
		{ start_tick_ = tick; }

	const char* cfg_file(void) const
		{ return cfg_file_.c_str(); }

	int perf_thres(void) const
		{ return perf_thres_; }
	void perf_thres(int pth)
		{ perf_thres_ = pth; }

	const char * log_server(void) const
		{ return log_server_.c_str();}

	void log_server(const char *svr)
		{ log_server_ = svr;}

	// �ж������Ƿ�Ӧ�����
	bool is_output(const char* msg, const char * sub) const
		{
		// �ų�������
		std::vector<std::string>::const_iterator pi = excludes_.begin();
		for ( ; pi != excludes_.end(); ++pi )
			{
			if ( sub && (*pi).compare(sub) == 0 )
				return false;
			if ( strstr(msg, (*pi).c_str()) )
				return false;
			}

		// ���жϹ��˵�
		pi = includes_.begin();
		for ( ; pi != includes_.end(); ++pi )
			{
			if ( sub && (*pi).compare(sub) == 0 )
				return true;
			if ( strstr(msg, (*pi).c_str()) )
				return true;
			}

		return includes_.empty() ? true : false;
		}

	// ��һ���ӿڻ�ýӿڵ�����
	void set(const BASE * bs)
		{
		if ( bs == 0 )
			return ;
		max_size_		= bs->max_size();
		max_count_		= bs->max_count();
		overwrite_		= bs->overwrite();
		output_			= bs->output();
		level_			= bs->level();
		style_			= bs->style();
		filter_			= bs->filter();
		module_			= bs->module();
		flush_always_	= bs->flush_always();
		mode_			= bs->mode();
		has_head_		= bs->has_head();
		locale_			= bs->locale();
		console_level_  = bs->console_level();
		charset_		= bs->charset();
		file_name		 (bs->file_name());
		title			 (bs->title());
		encrypt_		= 0;
		cfg_mtime_		= 0;
		start_time_		= bs->start_time();
		start_tick_		= bs->start_tick();
		}

	// ��ӻ��߼���level
	int level_add(int lvl)
		{
		return level_ |= lvl;
		}
	int level_remove(int lvl)
		{
		return level_ &= ~lvl;
		}
	// ��ӻ�������
	int output_add(int out)
		{
		return output_ |= out;
		}
	int output_remove(int out)
		{
		return output_ &= ~out;
		}

	// ��ȡ������Ϣ
	int read_config (const char* key = 0, const char* cfg_file = 0);
	// д��������Ϣ
	int write_config(const char* key = 0, const char* cfg_file = 0);
	// �ж�������Ϣ�Ƿ�ı���
	bool config_changed(void);

private:
	/// ��־�ļ�����
	std::string file_name_;
	/// ����
	std::string title_;
	/// ��־���ߴ�
	size_t		max_size_;
	/// ��־�ļ����������
	int			max_count_;
	/// ��־�����ʽ
	int			output_;
	/// ��־��¼������ 
	int			level_;
	/// ��־����ʽ
	int			style_;
	/// ģ�����������
	int			filter_;
	/// �Ƿ񸲸ǣ����ݷ񣩾ɵ��ļ�
	bool		overwrite_;
	/// �Ƿ�ÿ����Ϣ��flush
	bool		flush_always_;
	/// �Ƿ����ͷ
	bool		has_head_;
	/// �Ƿ���Ҫ��ʱ
	bool		synch_time_;
	/// ģ����
	void *		module_;
	/// �������ַ���
	std::vector<std::string> includes_;
	/// �ų����ַ���
	std::vector<std::string> excludes_;
	/// �����ļ���
	std::string cfg_file_;
	/// section��
	std::string section_;
	/// ��־�����ļ�������޸�����
	time_t		cfg_mtime_;
	/// ��־�Ĵ�ģʽ
	int			mode_;
	/// �����Ϣ�Ƿ����
	int			encrypt_;
	/// λ����Ϣ
	std::string locale_;
	/// ����̨���ʱ�ĵȼ�
	int			console_level_;
	/// ��խ�ַ�
	LOG_CHARSET charset_;
	/// ��ʼʱ��
	time_t		start_time_;
	unsigned int start_tick_;
	// ��������
	int			perf_thres_;
	// log��������ַ
	std::string	log_server_;
};


/** 
 * @class	 Log_Mutex
 * 
 * @brief	��־�������ӿ�
 * 
 *  ���̷߳�ʽ��ʹ�õ���־��Ҫ�û��Զ���������ʽ
 * 
 * @author	jdyu
 * @date	2005-10-28
 * 
 * @see		
 * 
 * @par ��ע��
 * 
 */
struct Log_Mutex
{
	virtual int open(const char* name = 0) = 0;
	virtual int close(bool call_delelte = false) = 0;
	virtual int acquire(int time_out = -1) = 0;
	virtual int release(void) = 0;
	virtual const char* name(void) = 0;
};

/** 
 * @class	 Log_IO
 * 
 * @brief	��־��I/O��ȡ�ӿ�
 * 
 *  ����ӿڶ�������־�������ʽ��
 *  ��������ʵ��Ϊ���ַ�ʽ�������ļ���ʽ��socket��ʽ���ڴ淽ʽ�ȵ�
 * 
 * @author	jdyu
 * @date	2005-10-28
 * 
 * @see		Log_IO_File
 */
struct Log_IO
{
	// ��ʼ��/���ʼ��
	virtual int open(const char * fn, Log_Mutex * lock = 0, int mode = 0, void * rev = 0) = 0;
	virtual int close(void) = 0;

	// дһ����¼���ı���ʽ
	virtual int write(const char * msg) = 0;
	virtual int write(const wchar_t * msg) = 0;
	// �����Ʒ�ʽд��
	virtual int write(const void * data, int size) = 0;

	// ����ļ�
	virtual int empty(void) = 0;
	// �ӻ���ˢ����־
	virtual void flush(void) = 0;
	// ��õ�ǰ�ߴ�
	virtual size_t size(void) = 0;

	/// ���ݾɵ��ļ�
	virtual int backup(const char* bname = 0) = 0;
	/// ��־�ľ��
	virtual LOG_HANDLE handle(void) = 0;

	/* ��־�� */
	virtual Log_Mutex * mutex(void) = 0;
	virtual void mutex(Log_Mutex * lock) = 0;
	/* ��־���� */
	virtual Log_Cfg * config(void) = 0;
	virtual void config(Log_Cfg * cfg) = 0;
	/* �Ƿ������ͷ� */
	virtual bool donot_delete(void) = 0;
};

/** 
 * @class	 Log_Mutex_Null
 * 
 * @brief	һ���յ���־��
 * 
 *  �������Ҫ��������ʹ�������
 * 
 * @author	jdyu
 * @date	2005-10-11
 * 
 */
class Log_Mutex_Null
	: public Log_Mutex
{
public:
	int open(const char* /* name */ = 0)
		{ return 0; }
	int close(bool call_delete = false)
		{ if ( call_delete ) delete this; return 0; }
	int acquire(int /* time_out */ = -1)
		{ return 0; }
	int release(void)
		{ return 0; }
	const char* name(void)
		{ return 0; }
	Log_Mutex_Null(const char* /* name */ = 0)
		{ }
};

/*
 * ��Win32��ʵ�ֵĽ�����
 */
class Log_Win32_Process_Mutex
	: public Log_Mutex
{
public:
	int open(const char* name = 0);
	int close(bool call_delete = false);
	int acquire(int time_out = -1);
	int release(void);
	const char* name(void);

	Log_Win32_Process_Mutex(const char* name = 0, bool create = false);
	~Log_Win32_Process_Mutex(void);

private:
	// ���������
	LOG_HANDLE mutex_;
	// ����
	char name_[252];
};

/*
 * ��Win32��Linux��Solaris��ʵ�ֵ��߳���
 */
class Log_Thread_Mutex
	: public Log_Mutex
{
public:
	int open(const char* name = 0);
	int close(bool call_delete = false);
	int acquire(int time_out = -1);
	int release(void);

	const char* name(void)
		{ return 0; }

	Log_Thread_Mutex(bool create = false);
	~Log_Thread_Mutex(void);

private:
	// ���������
	LOG_HANDLE mutex_;
};

/*
 * ��Linux��Solaris��ʵ�ֵĽ�����
 */
class Log_Unix_Process_Mutex
	: public Log_Thread_Mutex
{
public:
	int open(const char* name = 0);
	int close(bool call_delete = false);
	int acquire(int time_out = -1);
	int release(void);

	const char* name(void)
		{ return 0; }

	Log_Unix_Process_Mutex(const char* name = 0, bool create = false);
	~Log_Unix_Process_Mutex(void);
};

/*
 * ��Win32��Linux��Solaris��ʵ�ֵĽ�����
 */
#if defined(WIN32) || defined(_WINDOWS)
  typedef Log_Win32_Process_Mutex Log_Process_Mutex;
#else // Non Windows
  typedef Log_Unix_Process_Mutex Log_Process_Mutex;
#endif // defined(WIN32) || defined(_WINDOWS)

// ͬ������ʹ��������
template <typename T>
class Log_Mutex_Guard
{
public:
	Log_Mutex_Guard(T * lock)
		: lock_(lock)
		{
		if ( lock_ )
			lock_->acquire();
		}
	Log_Mutex_Guard(T & lock)
		: lock_(&lock)
		{
		if ( lock_ )
			lock_->acquire();
		}
	~Log_Mutex_Guard(void)
		{
		if ( lock_ )
			lock_->release();
		}
private:
	T * lock_;
};

// ����ʹ�õĺ�
#define LOG_GUARD(mutex, lock)	Log_Mutex_Guard<mutex> _mon(lock);
#define LOG_GUARD_PROCESS(name)	Log_Process_Mutex _lock(name, true); Log_Mutex_Guard<Log_Process_Mutex> _mon(_lock);

/// ʹ���ļ�ʵ�ֵ�IO��
class Log_IO_FILE
	: public Log_IO
{
public:
	/* ��ʼ��/���ʼ�� */
	virtual int open(const char * fn, Log_Mutex * lock = 0, int mode = 0, void * rev = 0);
	virtual int close(void);

	/* дһ����¼���ı���ʽ */
	virtual int write(const char * msg);
	virtual int write(const wchar_t * msg);
	/* �����Ʒ�ʽд�� */
	virtual int write(const void * data, int size);

	/* ����ļ� */
	virtual int empty(void);
	/* �ӻ���ˢ����־ */
	virtual void flush(void);
	/* ��õ�ǰ�ߴ� */
	virtual size_t size(void);

	/* ���ݾɵ��ļ� */
	virtual int backup(const char * bname = 0);
	/* ��־�ľ�� */
	virtual LOG_HANDLE handle(void);
	/* ��־�� */
	virtual Log_Mutex * mutex(void);
	virtual void mutex(Log_Mutex * lock);
	/* ��־���� */
	virtual Log_Cfg * config(void);
	virtual void config(Log_Cfg * cfg);
	virtual bool donot_delete(void);

	/* constructor */
	Log_IO_FILE(void);
	virtual ~Log_IO_FILE(void);

private:
	// �ļ�ָ��
	FILE*			file_;
	// �ļ�����
	std::string		fn_;
	// ��־��
	Log_Mutex *		lock_;
	// ����
	Log_Cfg *		cfg_;
};

/// ����ģ����
template <typename T>
class iFly_Singleton_T
{
public:
	// �õ���־ȫ��ʵ��
	// �õ���־ȫ��ʵ���������ʵ����û�д������򴴽�֮
	// �û��������close_singleton���ٴ�����ʵ��
	static T* open_singleton(void)
		{
		if ( instance() == 0 )
			{
			instance() = new T;
			need_delelte() = true;
			}
		ref_count()++;
		return instance();
		}
	static T* instance(T* tm)
		{
		T* t = instance();
		close_singleton();
		// �ⲿ����Ĳ���Ҫdelete
		need_delelte() = false;
		instance() = tm;
		return t;
		}
	// ����û�������ȫ��ʵ������������������
	static void close_singleton(void)
		{
		if ( --ref_count() > 0 )
			return ;
		if ( instance() && need_delelte() )
			{
			delete instance();
			instance() = 0;
			need_delelte() = false;
			}
		}
	static T* & instance(void)
		{
		static T* the_inst = 0;
		return the_inst;
		}
	static bool & need_delelte(void)
		{
		/// ��ʵ����صĳ�Ա
		static bool	delete_instance_ = false;
		return delete_instance_;
		}
	static int & ref_count(void)
		{
		static int reference_count_ = 0;
		return reference_count_;
		}
protected:
	iFly_Singleton_T(void)
		{ }
};

/** 
 * @brief	��������һ��Ψһ������
 * 
 *  ��������ڽ��Log_Singletonģ�����������ʱ����������������ȫһ����
 *  �ͻ�ʹ��ͬһ������ָ�������
 * 
 * @author	jdyu
 * @date	2005-12-23
 */
template <typename T, typename E /* , E nontype_param */>
class Log_Uni_Type
	: public T
{
	operator T & (void)
		{ return *this; }
};

// ������������һ��Ψһ������
# define LOG_UT_DECL(cls, name)										\
	class __LOG_TOK_##cls##_##name##__ { };							\
	typedef Log_Uni_Type<cls, __LOG_TOK_##cls##_##name##__> name;

/**
 * һ��CFG_HEAP����
 *	Windows���ܹ���ȡע����ini�ļ�
 *	Linux���ܹ���ȡ�����ļ�
 */
class Log_Cfg_Heap_Reg
	: public Log_Cfg_Heap
{
public:
	// open / close
	int open(const char* section, const char* file = 0);
	int close(void);
	
	// value get /set
	int get_string_value(const char* key, std::string & value, const char* sub = 0);
	int enum_values(LOG_STRPAIR_ARR & arr, const char* sub = 0);
	int get_string_arr  (const char* key, std::vector<std::string> & arr);

	// construct
	Log_Cfg_Heap_Reg(void);
	virtual ~Log_Cfg_Heap_Reg(void);

protected:
	// root
	std::string root_;
	// file
	std::string file_;
	// handle
	LOG_HANDLE	handle_;
	// is ini file?
	bool		ini_;
};

//====================================================================================
// �ܹ�����������������Ե���־ģ����
//====================================================================================
template <typename IO = Log_IO_FILE,
	typename LOCK = Log_Mutex_Null, typename CFG = Log_Cfg_T<> >
class Log_Impl_T
{
public:
	typedef IO		io_impl;
	typedef CFG		cfg_impl;
	typedef LOCK	lock_impl;

	Log_Impl_T(void);
	Log_Impl_T(const char * file, const char * title = 0, bool has_head = false, io_impl * io = 0);
	virtual ~Log_Impl_T(void);

public:	
	// ��ʼ��/���ʼ��
	virtual int open (const cfg_impl * cfg = 0, io_impl * io = 0, void * rev = 0);
	virtual int close(void);

	// ������õĺ���
	void LOG_CDECL write_msg	(const char * fmt, ...);

	// ������ģ����Ϣ����־����
	void LOG_CDECL log_crit		(const char * fmt, ...);
	void LOG_CDECL log_error	(const char * fmt, ...);
	void LOG_CDECL log_warn		(const char * fmt, ...);
	void LOG_CDECL log_info		(const char * fmt, ...);
	void LOG_CDECL log_verbose	(const char * fmt, ...);
	void LOG_CDECL log_debug	(const char * fmt, ...);
	void LOG_CDECL log_perf		(const char * fmt, ...);
	void LOG_CDECL log_trace	(const char * fmt, ...);
	void LOG_CDECL log_spec		(const char * fmt, ...);

	// ��������Ϣ����־����
	void LOG_CDECL log_crit		(int sub, const char * fmt, ...);
	void LOG_CDECL log_error	(int sub, const char * fmt, ...);
	void LOG_CDECL log_warn		(int sub, const char * fmt, ...);
	void LOG_CDECL log_info		(int sub, const char * fmt, ...);
	void LOG_CDECL log_verbose	(int sub, const char * fmt, ...);
	void LOG_CDECL log_debug	(int sub, const char * fmt, ...);
	void LOG_CDECL log_perf		(int sub, const char * fmt, ...);
	void LOG_CDECL log_trace	(int sub, const char * fmt, ...);
	void LOG_CDECL log_spec		(int sub, const char * fmt, ...);

	// ����������Ϣ����־����
	void LOG_CDECL log_crits	(const char * sub, const char * fmt, ...);
	void LOG_CDECL log_errors	(const char * sub, const char * fmt, ...);
	void LOG_CDECL log_warns	(const char * sub, const char * fmt, ...);
	void LOG_CDECL log_infos	(const char * sub, const char * fmt, ...);
	void LOG_CDECL log_verboses	(const char * sub, const char * fmt, ...);
	void LOG_CDECL log_debugs	(const char * sub, const char * fmt, ...);
	void LOG_CDECL log_perfs	(const char * sub, const char * fmt, ...);
	void LOG_CDECL log_traces	(const char * sub, const char * fmt, ...);
	void LOG_CDECL log_specs	(const char * sub, const char * fmt, ...);

	// ��¼��־��Ϣ
	void LOG_CDECL log_msg		(int sub,		  LOG_LEVEL lvl, int style, const char * fmt, ...);
	void LOG_CDECL log_msg		(const char* sub, LOG_LEVEL lvl, int style, const char * fmt, ...);
	void LOG_CDECL log_msg		(const char* msg);
	void LOG_CDECL log_msg		(const wchar_t* msg);

	// ������ģ����Ϣ����־����
	void LOG_CDECL write_msg	(const wchar_t * fmt, ...);
	void LOG_CDECL log_crit		(const wchar_t * fmt, ...);
	void LOG_CDECL log_error	(const wchar_t * fmt, ...);
	void LOG_CDECL log_warn		(const wchar_t * fmt, ...);
	void LOG_CDECL log_info		(const wchar_t * fmt, ...);
	void LOG_CDECL log_verbose	(const wchar_t * fmt, ...);
	void LOG_CDECL log_debug	(const wchar_t * fmt, ...);
	void LOG_CDECL log_perf		(const wchar_t * fmt, ...);
	void LOG_CDECL log_trace	(const wchar_t * fmt, ...);
	void LOG_CDECL log_spec		(const wchar_t * fmt, ...);

	// ��������Ϣ����־����
	void LOG_CDECL log_crit		(int sub, const wchar_t * fmt, ...);
	void LOG_CDECL log_error	(int sub, const wchar_t * fmt, ...);
	void LOG_CDECL log_warn		(int sub, const wchar_t * fmt, ...);
	void LOG_CDECL log_info		(int sub, const wchar_t * fmt, ...);
	void LOG_CDECL log_verbose	(int sub, const wchar_t * fmt, ...);
	void LOG_CDECL log_debug	(int sub, const wchar_t * fmt, ...);
	void LOG_CDECL log_perf		(int sub, const wchar_t * fmt, ...);
	void LOG_CDECL log_trace	(int sub, const wchar_t * fmt, ...);
	void LOG_CDECL log_spec		(int sub, const wchar_t * fmt, ...);

	// ����ļ�
	int empty(void);
	// �ӻ���ˢ����־
	void flush(void);
	// �Ƿ񱻽�ֹ
	bool enable(void) const;
	// ��õ�ǰ�ߴ�
	size_t size(void) const;

	// =======================================================
	// �ⲿ��Ҫʹ�õ�һЩ����
public:
	// ������Ϣ
	cfg_impl & config(void)
		{ return config_; }
	// ����IO�ӿ�
	Log_IO * log_io(void) const
		{ return log_io_; }

	// �����ͽ���
	virtual int lock (int time_out = -1);
	virtual int unlock (void);

	// =======================================================
	// �������������������ʵ�ֵĺ���
public:
	/// дһ����¼(��Ҫ���ڲ�����)
	virtual void log_msg_i(const char* sub, LOG_LEVEL lvl, int style, const char* msg);
	virtual void log_msg_i(const char* sub, LOG_LEVEL lvl, int style, const wchar_t* msg);
	virtual void log_msg_i(LOG_LEVEL lvl, const char* msg);
	virtual void log_msg_i(LOG_LEVEL lvl, const wchar_t* msg);

	/// �����Ʒ�ʽд��
	virtual int write(const void * data, int size);

	// дһ���ָ���
	virtual void write_bound(char sep = '-');

protected:
	/// ��һ������ID������⣨��������Ҫ����ʵ�ָú�������
	virtual const char * subject(int sub, char * buf, int len) const;
	/// �û������Զ���ȼ���Ϣ���ַ���
	virtual const char * level_str(int lvl) const;

	/// ���ܺ���
	virtual int encrypt(void * msg, int len);

	/// �Ƿ���Ҫ���ĳ������Ĺ��˺���
	virtual bool sub_output(int sub) const;
	virtual bool sub_output(const char * sub) const;

	// д�ļ�ͷβ
	virtual void write_head(bool new_file = true);
	virtual void write_tail(bool end_file = true);

	// ������־
	virtual int  back_file(void);

	// ��õ�ǰʱ�䣬ȱʡʵ���Ǿ�ȷ�����
	// �����Ҫ���Ӿ�ȷ������������ʵ��
	virtual const char * cur_time(char tmbuf[], bool no_path_char = false, bool sht_fmt = false) const;

	// ��������ļ��Ƿ���ģ�������ģ����¶�ȡ
	virtual int check_config(void);

protected:
	/// ��־��������Ϣ
	cfg_impl		config_;
	/// ��־IO�ľ���ʵ����
	Log_IO *		log_io_;
	bool			call_delete_io_;
	/// ��־���ľ���ʵ��
	Log_Mutex *		lock_;
	bool			call_delete_lock_;
	// ��־������ʱ��
	time_t			last_ltime_;
	// ��־�Ƿ����һ��console
	bool			alloc_console_;
};

// =======================================================
// ���ܼ�����Զ�������
// ���캯����ʼ��ʱ����������������ʱ�����Ҵ�ӡ������Ϣ
// =======================================================
template <class TIMER, class LOG, typename T = double>
class Log_Perf_Helper
{
public:
	Log_Perf_Helper(const char* name)
		: ext_nsec_ (0)
		, stopped_	(false)
		{
		#if _MSC_VER >= 1400
		strcpy_s(name_, sizeof name_, name);
		#else
		strcpy(name_, name);
		#endif
		timer_.start(); msg_[0] = 0;
		}
	Log_Perf_Helper(const char* name, T & nsec) 
		: ext_nsec_ (&nsec)
		, stopped_	(false)
		{
		#if _MSC_VER >= 1400
		strcpy_s(name_, sizeof name_, name);
		#else
		strcpy(name_, name);
		#endif
		timer_.start(); msg_[0] = 0;
		}
	~Log_Perf_Helper(void)
		{
		double msec = stop() * 1e-6;
		if ( LOG::instance() )
			{
			int pth = LOG::instance()->config().perf_thres();
			if ( pth > 0 && pth < msec )
				LOG::instance()->log_perf("%s %s %.03f msec.", name_, msg_, msec);
			}
		if ( ext_nsec_ )
			*ext_nsec_ = nanosec_;
		}
	const T & stop(void)
		{
		if ( stopped_ )
			return nanosec_;
		timer_.stop();
		timer_.elapsed_time(nanosec_);
		stopped_ = true;
		return nanosec_;
		}

	const char * msg(void) const
		{ return msg_; }
	const T & nanosec(void) const
		{ return nanosec_; }
	TIMER & timer(void)
		{ return timer_; }

	void log(const char* fmt, ...)
		{
		if ( LOG::instance() == 0 )
			return ;
		msg_[0] = 0;
		if ( LOG::instance()->config().output() == 0 )
			return ;
		if ( LOG::instance()->config().level() & lgl_perf )
			{
			va_list va;
			va_start(va, fmt);
			#if _MSC_VER >= 1400
			vsprintf_s(msg_, sizeof msg_, fmt, va);
			#else
			vsprintf(msg_, fmt, va);
			#endif
			va_end(va);
			}
		}

public:
	TIMER			timer_;
	char			name_[60];
	char			msg_ [10*1024];
	T 				nanosec_;
	T *				ext_nsec_;
	bool			stopped_;
};

// ===============================================
// �ṩ��һ���򵥵ļ�ʱ��
// Windowsϵͳ��ʵ���˾�ȷ�ļ�ʱ����
// ===============================================
class Log_Timer
{
public:
	void start(void)
		{
		#if defined(WIN32)
			QueryPerformanceCounter(&timestart_);
		#else
			gettimeofday(&timestart_, 0);
		#endif
		}
	double stop (bool restart = false)
		{
		elapsed_time_ = 0;
		#ifdef WIN32
			LARGE_INTEGER cur;
			if ( QueryPerformanceCounter(&cur) )
				{
				elapsed_time_ = (((cur.QuadPart - timestart_.QuadPart) * 1e9 * 1.) / frequency_.QuadPart);
				if ( restart )
					timestart_.QuadPart = cur.QuadPart;
				}
			return elapsed_time_;
		#else
			struct timeval	cur;
			gettimeofday(&cur, 0);
			elapsed_time_ = ( cur.tv_sec - timestart_.tv_sec ) * 1e9
				+ ( cur.tv_usec - timestart_.tv_usec ) * 1e3;
			return elapsed_time_;
		#endif
		}
	// ��λ����
	double elapsed_time(void)
		{ return elapsed_time_; }
	double elapsed_time(double & et)
		{ return et = elapsed_time_; }

	Log_Timer(void)
		: elapsed_time_(0.)
		{
		#ifdef WIN32
		QueryPerformanceFrequency(&frequency_);
		#endif
		start();
		}
	~Log_Timer(void)
		{ }

private:
	// �߷ֱ��ʵļ���
	#ifdef WIN32
	LARGE_INTEGER	frequency_;
	LARGE_INTEGER	timestart_;
	#else // linux
	struct timeval	timestart_;		
	#endif /* WIN32 */

	// �ڲ�����
	double elapsed_time_;
};

// ===============================================
// �ṩ��һ���򵥵ĺ������ø�����
// ���ù��캯���������������ص�
// ===============================================
template<class LOGGER>
class Log_Func_Tracer
{
public:
	Log_Func_Tracer (const char * sub)
		{
		if ( sub )
			sub_ = sub;

		if ( LOGGER::instance() )
			LOGGER::instance()->log_trace("%s | enter.", sub_.c_str());
		}
	Log_Func_Tracer(const char * sub, const char * fmt, ...)
		{
		if ( sub )
			sub_ = sub;

		if ( LOGGER::instance() )
			{
			char msg[1024];
			va_list va;
			va_start(va, fmt);
			#if _MSC_VER >= 1400
			vsprintf_s(msg, sizeof msg, fmt, va);
			#else
			vsprintf(msg, fmt, va);
			#endif
			LOGGER::instance()->log_trace("%s | enter, %s", sub_.c_str(), msg);
			va_end(va);
			}
		}
	~Log_Func_Tracer(void)
		{
		if ( LOGGER::instance() )
			LOGGER::instance()->log_trace("%s | leave.", sub_.c_str());
		}
private:
	// ����
	std::string sub_;
};

// ===============================================
// �ṩһ����ȫ�ĳ�ʼ����
// ���������������ص���������������������ʼ��
// ===============================================
template <class T>
class Log_Auto_Close
{
public:
	Log_Auto_Close(void)
		{ }
	~Log_Auto_Close(void)
		{ T::close(); }
};

/** 
 * @class	Log_Singleton
 * @brief	��־������
 * @author	jdyu
 * @date	2004��5��21��
 * @see		
 * @par ��ע���û����Դ����������������ʵ��GetSubjuct()���������ܹ�ʵ�����⹦��
 */
template <typename IO = Log_IO_FILE, typename LOCK = Log_Mutex_Null, typename CFG = Log_Cfg_T<Log_Cfg_Heap_Reg> >
class Log_Singleton
	: public iFly_Singleton_T< Log_Impl_T<IO, LOCK, CFG> >
{
public:
	// ����ı���
	typedef iFly_Singleton_T< Log_Impl_T<IO, LOCK, CFG> > the_parent;

	/** 
	 * @brief 	Log_Singleton::open
	 *  
	 *  �û���õĳ�ʼ����־����
	 *  �������ļ������ȡ������Ϣ�ĳ�ʼ������
	 *  
	 * @author	jdyu
	 * @date	2006-2-14
	 * @return	static int	- Return 0 in success, otherwise return error code.
	 * @param	const char * cfg_file	- [in] ָ���������ļ���ע���λ�� 
	 * @param	const char * key = 0	- [in] ��־ʹ�õ������ƣ�ȱʡΪlogger
	 * @param	const CFG  * dcfg = 0	- [in] ȱʡ������Ϣ
	 * @param	LOG_HANDLE hmod = -1	- [in] ��ǰģ��ľ����exeΪ0��dll��Ҫ���ݣ����ڻ�ȡ��ģ���·����Ϣ
	 * @param	Log_IO * io = 0			- [in] �����IO�ӿ�
	 * @param	void * rev = 0			- [in] ���������ڴ��ݸ�IO�ӿڵĳ�ʼ��
	 * @see		
	 */
	static int open(const CFG & cfg, const char * cfg_file = 0, const char * key = 0);
	static int open(const char * cfg_file, const char * key = 0, const CFG * dcfg = 0, LOG_HANDLE hmod = LOG_INVALID_HANDLE, Log_IO * io = 0, void * rev = 0);

	/** 
	 * @brief 	Log_Singleton::open
	 *  
	 *  �򻯵���־��ʼ������
	 *  ��ȡ������Ϣ����ʼ�����û��Լ����������Ϣ�ĳ�ʼ������
	 *  ����û�������io_impl�ӿڣ�˵���û��Լ����������е�io�ӿڣ���io�ӿڿ���Я������������Ϣ
	 *  
	 * @author	jdyu
	 * @date	2006-2-14
	 * @return	static int	- Return 0 in success, otherwise return error code.
	 * @param	const CFG & cfg			- [in] ��־����
	 * @param	Log_IO * io_impl		- [in] ʹ�õ�io�ӿ�ʵ�֣�������㣬�����ڲ�����һ��ioʵ��
	 * @param	void * rev = 0			- [in] ���������ڴ��ݸ�IO�ӿڵĳ�ʼ��
	 * @see		
	 */
	static int open(const CFG & cfg,  Log_IO * io_impl, void * rev = 0);
	static int open(Log_IO * io_impl, LOG_HANDLE mod = LOG_INVALID_HANDLE, void * rev = 0);

	/** 
	 * @brief 	open
	 *  
	 *  ��һ��IOʵ�ֿ��ʼ��һ��ȫ�ֵ���־������
	 *  ���������������û��Լ�ʵ����һ��IO�ӿڣ�������һ��dll�е����
	 *  
	 * @author	jdyu
	 * @date	2006-2-14
	 * @return	static int	- Return 0 in success, otherwise return error code.
	 * @param	const char * io_lib		- [in] �û�ָ��Log_IO�ӿ�ʵ�ֿ⣬���dll���뵼��ifly_create_logio��ifly_destroy_logio�ӿ�
	 * @param	const char * cfg_file	- [in] ��ο�����ĺ���
	 * @see		Log_IO
	 */
	static int open(const char * io_lib, const char * cfg_file, const char * key, const CFG * cfg, LOG_HANDLE hmod, void * rev);

	/** 
	 * @brief 	close
	 *  
	 *  ��ҹ��õ����ʼ�����������̽���ʱ�����˵���
	 *  
	 * @author	jdyu
	 * @date	2006-2-14
	 * @return	static int	- Return 0 in success, otherwise return error code.
	 * @see		
	 */
	static int close(void);
};

// =============================================================================
// ��������Ψһ������־����ĺ�
// ϣ���û�ʹ�������ﵽ��ʵ�õ�Ŀ��
// =============================================================================
#define LOG_DECL_SINGLETON(io, lock, cfg, name)					\
	LOG_UT_DECL(io, name##io);									\
	typedef cfg name##_Cfg;										\
	typedef Log_Singleton<name##io, lock, name##_Cfg> name;

#define LOG_DECL_SINGLETON_EX(io, lock, cfg, name)				\
	LOG_DECL_SINGLETON(io, lock, Log_Cfg_T<cfg>, name)

#define LOG_DECL_SIMPLE(io, lock, name)							\
	LOG_DECL_SINGLETON_EX(io, lock, Log_Cfg_Heap_Reg, name)

// =============================================================================
// ���ﶨ����һ��ʹ���ļ���ʹ���ٽ�����������־��
//  typedef Log_Singleton<> IFLY_Logger;
//  typedef Log_Cfg_T<Log_Cfg_Heap_Reg> IFLY_Logger_Cfg;
// ���ϵ������������Բ�������ĺ����
// ����û���Ҫʹ�ý������������ⲿ����IFLY_LOG_PROCESSԤ�����
// =============================================================================
#ifdef IFLY_LOG_PROCESS
LOG_DECL_SIMPLE(Log_IO_FILE, Log_Win32_Process_Mutex, IFLY_Logger)
# pragma message("iFly_Log::compile with process mutex.")
#else  // default to use thread mutex
LOG_DECL_SIMPLE(Log_IO_FILE, Log_Thread_Mutex, IFLY_Logger)
#endif // Log mutex

/* enable __VA_ARGS__? */
#if __STDC_VERSION__ >= 199901L || _MSC_VER >= 1400
# ifndef IFLY_LOG_NOVAGS
#  define VA_ARGS_ENABLE		1
# endif /* IFLY_LOG_NOVAGS */
#else	/* lower C99 standard */
#endif  /* __VA_ARGS__ */

/* �������������꣬��ô���е���־������ */
#ifndef IFLY_LOG_DISABLE

/**
 * @brief	����ʹ�õĺ�
 *	��ʹ�õ�ʵ��ģʽ��ʱ���⼸������Ը�����ʹ����־����
 *  ע�⣺IFLY_LOG_DISABLE����Խ������е���־����
 */
#  define IFLY_LOGINST()				IFLY_Logger::instance()
#  define IFLY_LOGFLUSH					if ( IFLY_LOGINST() ) IFLY_LOGINST()->flush
#  define IFLY_LOGCRIT					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_crit
#  define IFLY_LOGERR					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_error
#  define IFLY_LOGWARN					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_warn
#  define IFLY_LOGINFO					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_info
#  define IFLY_LOGVERBOSE				if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_verbose
#  define IFLY_LOGDBG					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_debug
#  define IFLY_LOGPERF					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_perf
#  define IFLY_LOGTRACE					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_trace
#  define IFLY_LOGWMSG					if ( IFLY_LOGINST() ) IFLY_LOGINST()->write_msg
   /* ֧�����⹦�ܵ���־���� */
#  define IFLY_LOGCRITS					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_crits
#  define IFLY_LOGERRS					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_errors
#  define IFLY_LOGWARNS					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_warns
#  define IFLY_LOGINFOS					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_infos
#  define IFLY_LOGVBSS					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_verboses
#  define IFLY_LOGDBGS					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_debugs
#  define IFLY_LOGPERFS					if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_perfs
#  define IFLY_LOGTRACES				if ( IFLY_LOGINST() ) IFLY_LOGINST()->log_traces

// �ܹ��Զ�ͳ�����ܲ���ӡ�ģ��ֱ�Ϊ����ӡ���⡢Я�����
# define IFLY_LOGPERF_N(name)			Log_Perf_Helper<Log_Timer, IFLY_Logger>		__ph__(#name);
# define IFLY_LOGPERF_S(name,fmt)		Log_Perf_Helper<Log_Timer, IFLY_Logger>		__ph__(#name); __ph__.log fmt;
# define IFLY_LOGPERF_NSEC(name, nsec)	Log_Perf_Helper<Log_Timer, IFLY_Logger>		__ph__(#name, nsec);
# define IFLY_LOGPERF_STOP()														__ph__.stop()

// ���ٺ������õķ���
# define IFLY_TRACER					Log_Func_Tracer<IFLY_Logger>				__ilt__
# ifdef VA_ARGS_ENABLE
#  define IFLY_TRACERS(sub, fmt, ...)	Log_Func_Tracer<IFLY_Logger>				__ilt__(sub, fmt, ##__VA_ARGS__);
# endif /* VA_ARGS_ENABLE */

// ��ʼ�������ʼ��
# define IFLYLOG_OPEN					IFLY_Logger::open
# define IFLYLOG_CLOSE					IFLY_Logger::close
# define IFLYLOG_OPEN_AC				Log_Auto_Close<IFLY_Logger> __lac__; IFLY_Logger::open

#else  /* IFLY_LOG_DISABLE */

# ifdef VA_ARGS_ENABLE
#  define IFLY_LOGINST()				IFLY_Logger::instance()
#  define IFLY_LOGCRIT(...)
#  define IFLY_LOGERR(...)
#  define IFLY_LOGWARN(...)
#  define IFLY_LOGINFO(...)
#  define IFLY_LOGVERBOSE(...)
#  define IFLY_LOGDBG(...)
#  define IFLY_LOGPERF(...)
#  define IFLY_LOGTRACE(...)
#  define IFLY_LOGWMSG(...)
#  define IFLY_LOGFLUSH()
#  define IFLY_LOGPERF_N(name)
#  define IFLY_LOGPERF_S(name,fmt)
#  define IFLY_LOGPERF_NSEC(name, nsec)
#  define IFLY_LOGPERF_STOP()
#  define IFLY_TRACER(...)
#  define IFLY_TRACERS(...)
#  define IFLYLOG_OPEN(...)
#  define IFLYLOG_CLOSE(...)
#  define IFLYLOG_OPEN_AC(...)
# else
#  pragma(message, "This compiler not support VA_ARGS! CANNOT disable the log macros.")
#  define IFLY_LOGINST()				IFLY_Logger::instance()
#  define IFLY_LOGCRIT					__log_fake__
#  define IFLY_LOGERR					__log_fake__
#  define IFLY_LOGWARN					__log_fake__
#  define IFLY_LOGINFO					__log_fake__
#  define IFLY_LOGVERBOSE				__log_fake__
#  define IFLY_LOGDBG					__log_fake__
#  define IFLY_LOGPERF					__log_fake__
#  define IFLY_LOGTRACE					__log_fake__
#  define IFLY_LOGWMSG					__log_fake__
#  define IFLY_LOGFLUSH					__log_fake__
#  define IFLY_LOGPERF_N				__log_fake__
#  define IFLY_LOGPERF_S				__log_fake__
#  define IFLY_LOGPERF_NSEC				__log_fake__
#  define IFLY_LOGPERF_STOP				__log_fake__
#  define IFLY_TRACER					__log_fake__
#  define IFLY_TRACERS					__log_fake__
#  define IFLYLOG_OPEN					__log_fake__
#  define IFLYLOG_CLOSE					__log_fake__
#  define IFLYLOG_OPEN_AC				__log_fake__
   inline void __log_fake__(...)		{ ; }
# endif /* C99 */

#endif /* IFLY_LOG_DISABLE */

// ����ĺ����
#define IFLYLOG_ERROR_ASSERT(exp, msg)			\
				if (!(exp)) { IFLY_LOGERR msg; SP_ASSERTS(exp, msg); }
#define IFLYLOG_ASSERT_RETURN(exp, msg, ret)	\
				if (!(exp)) { IFLY_LOGERR msg; SP_ASSERTS(exp, msg); return ret; }
#define IFLYLOG_ERROR_RETURN(exp, msg, ret)		\
				if (!(exp)) { IFLY_LOGERR msg; return ret; }
#define IFLYLOG_FUNC_INFO(ret, succ, fail)		\
				if ( ret != 0 ) { IFLY_LOGERR msg; }  else { IFLY_LOGINFO msg; }
#define IFLYLOG_FUNC_VBS(ret, succ, fail)		\
				if ( ret == 0 ) { IFLY_LOGVBS succ;	} else { IFLY_LOGERR fail; }

// �����ⲿʵ�ֵĺ���
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// ������־ʵ�ֵĺ���
const char fn_ifly_logio_create[] = "ifly_logio_create";
int ifly_logio_create(Log_IO** io, void* res);
typedef int (* proc_ifly_logio_create)(Log_IO ** io, void * res);

// ������־ʵ�ֵĺ���
const char fn_ifly_logio_destroy[] = "ifly_logio_destroy";
int ifly_logio_destroy(Log_IO** io, void* res);
typedef int (* proc_ifly_logio_destroy)(Log_IO * io);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

// һ�ѵ�ʵ�ִ������������ļ����棬�����ϲ���Ҫ��
#include "iFly_Log.i"

#endif /* __IFLYLOGGER_H__ */
