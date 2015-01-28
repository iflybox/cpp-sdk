/** 
 * @file	proc.hpp
 * @brief	子进程管理
 * 
 *  创建子进程，把子进程的输出截取回来
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2007-6-6
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2007-6-6	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __PROC_INCLUDED
#define __PROC_INCLUDED

#include <string>
#include "isr_errors.h"
#include "iFly_Assert.h"
#include "utils/sutils.h"
#include "log/srlog.h"

#pragma warning(push)
#pragma warning(disable : 4819)
#include "boost/thread.hpp"
#pragma warning(pop)

namespace sp {

#ifdef WIN32

/** 
 * @class	subproc_win32
 * 
 * @brief	WIN32下的子进程输入输出截取类
 * 
 *  这个类在Windows环境下创建一个子进程，获取其输出信息，
 *  并且能够设置输入信息。
 * 
 * @author	jdyu
 * @date	2007-6-6
 * 
 * @see		
 */
template <class char_t = wchar_t>
class subproc_win32
{
public:
	// wait mode
	enum end_mode
		{
		pid_wait,
		no_wait
		};
	// types
	typedef std::basic_string<char_t, std::char_traits<char_t>, std::allocator<char_t> > proc_string;

	// create child process to capture output
	int open(end_mode em, int argc, const wchar_t* argv[], bool start_thr = true, bool supp_input = false)
		{
		PROCESS_INFORMATION		pi;
		SECURITY_ATTRIBUTES		sa;
		STARTUPINFO				si;

		ZeroMemory(&pi, sizeof pi);
		ZeroMemory(&sa, sizeof sa);
		ZeroMemory(&si, sizeof si);

		sa.nLength              = sizeof SECURITY_ATTRIBUTES;
		sa.bInheritHandle       = TRUE;
		sa.lpSecurityDescriptor = NULL;

		BOOL suc = CreatePipe(&out_rd_pipe_, &out_wt_pipe_, &sa, 0);
		if ( !suc )
			{
			SP_ASSERTS(0, ("subproc_win32::open | CreatePipe for output failed, error %d.", GetLastError()));
			return -1;
			}

		end_mode_  = em;
		start_thr_ = start_thr;
		supp_input_ = supp_input;
		if ( supp_input_ )
			{
			suc = CreatePipe(&in_rd_pipe_, &in_wt_pipe_, &sa, 0);
			if ( !suc )
				{
				SP_ASSERTS(0, ("subproc_win32::open | CreatePipe for input failed, error %d.", GetLastError()));
				return -1;
				}
			}

		// Set up STARTUPINFO structure.
		si.cb           = sizeof(STARTUPINFO);
		si.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
		si.wShowWindow  = SW_HIDE;
		si.hStdOutput   = out_wt_pipe_;
		si.hStdError    = out_wt_pipe_;
		if ( supp_input_ )
			si.hStdInput = in_rd_pipe_;

		cmd_ = argv2cmd(argc, argv, L" ");;

		// start thread data
		if ( em == no_wait && start_thr )
			{
			thr_ready_ = CreateEvent(0, 0, 0, 0);
			prc_ready_ = CreateEvent(0, 0, 0, 0);
			thr_start();
			WaitForSingleObject(thr_ready_, INFINITE);
			}

		// Create child process
		suc = CreateProcessW(
			NULL,
			(LPWSTR) cmd_.c_str(),
			NULL,
			NULL,
			TRUE,
			0,
			NULL,
			cwd_.size() ? cwd_.c_str() : NULL,
			&si,
			&pi);
		if ( !suc )
			{
			SP_ASSERTS(0, (L"subproc_win32::open | CreateProcessW failed, cmd [%s], error %d.",
				cmd_.c_str(), GetLastError()));
			return -1;
			}
		process_ = pi.hProcess;
		main_thr_ = pi.hThread;
		if ( em == no_wait && start_thr )
			SetEvent(prc_ready_);

		// read data
		if ( em == no_wait )
			return 0;

		int ret = read_output();
		return ret;
		}

	// create child process to capture output
	int open(end_mode em, int argc, const wchar_t * argv[], const wchar_t * cwd)
		{
		cwd_ = cwd;
		return open(em, argc, argv);
		}

	int close(void)
		{
		return close(L"");
		}

	int close(const proc_string & endf)
		{
		if ( process_ == NULL )
			return 0;

		if ( supp_input_ && endf.length() )
			write_input(endf);
		
		if ( end_mode_ == no_wait )
			wait();

		if ( out_rd_pipe_ )
			CloseHandle(out_rd_pipe_), out_rd_pipe_ = NULL;
		if ( out_wt_pipe_ )
			CloseHandle(out_wt_pipe_), out_wt_pipe_ = NULL;

		if ( in_rd_pipe_ )
			CloseHandle(in_rd_pipe_), in_rd_pipe_ = NULL;
		if ( in_wt_pipe_ )
			CloseHandle(in_wt_pipe_), in_wt_pipe_ = NULL;
		
		if ( process_ )
			CloseHandle(process_), process_ = NULL;
		if ( main_thr_ )
			CloseHandle(main_thr_), main_thr_ = NULL;

		if ( thr_ready_ )
			CloseHandle(thr_ready_), thr_ready_ = NULL;
		if ( prc_ready_ )
			CloseHandle(prc_ready_), prc_ready_ = NULL;
		return 0;
		}

	int wait(void)
		{
		int ret = 0;
		if ( start_thr_ )
			ret = thr_stop();
		else
			read_output(str_output_, result_);
		return ret;
		}

	const wchar_t * get_output(void) const
		{
		return str_output_.c_str();
		}
	int set_input(const wchar_t * input_str)
		{
		return write_input(input_str);
		}

	int get_result(void)
		{
		return result_;
		}

	subproc_win32(void)
		: process_		(0)
		, main_thr_		(0)
		, end_mode_		(pid_wait)
		, out_rd_pipe_	(0)
		, out_wt_pipe_	(0)
		, supp_input_	(false)
		, in_rd_pipe_	(0)
		, in_wt_pipe_	(0)
		, thr_inst_		(0)
		, thr_stop_		(false)
		, result_		(0)
		, start_thr_	(false)
		, thr_ready_	(0)
		, prc_ready_	(0)
		{
		}
	~subproc_win32(void)
		{
		close();
		}

protected:
	// read output of child process
	int read_output(void)
		{
		str_output_.clear();
		return read_output(str_output_, result_);
		}
	// read output of child process
	int read_output(proc_string & output_str, int & result, int time_out = -1)
		{
		const int BUF_SIZE = 8192;

		DWORD	bytes_read;
		DWORD	bytes_left = 0;
		DWORD	bytes_all = 0;
		char	pipe_data[BUF_SIZE];
		int		time_cost = 0;

		for ( ; ; )
			{
			bytes_read = 0;
			BOOL suc = PeekNamedPipe(out_rd_pipe_,
				pipe_data, 1, &bytes_read, &bytes_all, &bytes_left);
			if ( !suc )
				{
				SP_ASSERTS(0, ("PeekNamedPipe failed, error %d", GetLastError()));
				break ;
				}

			if ( bytes_read > 0 )
				{
				suc = ReadFile(out_rd_pipe_,
					pipe_data, BUF_SIZE - 1, &bytes_read, NULL);
				if ( !suc )
					{
					SP_ASSERTS(0, ("ReadFile from output pipe failed, error %d", GetLastError()));
					break ;
					}

				//	Zero-terminate the data.
				pipe_data[bytes_read] = 0;

				//	Append the output
				output_str += sp::char2wchar(pipe_data);
				}
			else
				{
				// Is time out
				if ( bytes_all == 0
					&& time_out != -1 && time_cost >= time_out )
					{
					srlog_warn(L"subproc_win32::read_output read time out, %d.", time_out);
					return ISR_ERROR_TIME_OUT;
					}

				// If the child process has completed, break out.
				const int WAIT_TIME = 100;
				if ( WaitForSingleObject(process_, WAIT_TIME) == WAIT_OBJECT_0 )
					{
					DWORD excode = 0;
					GetExitCodeProcess(process_, &excode);
					result = (int)excode;

					srlog_info(L"subproc_win32::read_output the process ended, result %d, output \n%s.",
						excode, output_str.c_str());
					break ;
					}
				time_cost += WAIT_TIME;
				}
			}
		return 0;
		}
	// write input to child process
	int write_input(const proc_string & input_str)
		{
		DWORD	bytes_write;
		DWORD	bytes_all  = (DWORD)input_str.size() + 1;
		DWORD	bytes_left = bytes_all;

		while ( bytes_left > 0 )
			{
			BOOL suc = WriteFile(in_wt_pipe_,
				input_str.c_str() + bytes_all - bytes_left, bytes_left, &bytes_write, NULL);
			if ( !suc )
				{
				SP_ASSERTS(0, ("WriteFile to input pipe failed, error %d", GetLastError()));
				break ;
				}

			bytes_left -= bytes_write;
			}
		return 0;
		}

	// argv to command line
	proc_string argv2cmd(int argc, const wchar_t * argv[], proc_string const & spc)
		{
		proc_string cmd_str;
		for ( int i = 0; i < argc; i++ )
			cmd_str += proc_string(argv[i]) + spc;
		return cmd_str;
		}

private:
	// environment info
	std::wstring	cmd_;
	std::wstring	cwd_;
	end_mode		end_mode_;

	// process info
	HANDLE			process_;
	HANDLE			main_thr_;
	HANDLE			out_rd_pipe_;
	HANDLE			out_wt_pipe_;
	int				result_;

	// 线程握手
	HANDLE			thr_ready_;
	HANDLE			prc_ready_;

	// 输出字符串
	std::wstring	str_output_;

	// 是否支持input
	bool			supp_input_;
	HANDLE			in_rd_pipe_;
	HANDLE			in_wt_pipe_;

private:
	// 后台线程
	template <class T>
	class read_thread
		{
		public:
		read_thread(T * cntr)
			: cntr_(cntr) { }

		void operator() (void)
			{ cntr_->thr_svc();	}

		T *	cntr_;
		};
	// boost thread object
	boost::thread *			thr_inst_;

	// 同步锁定
	boost::mutex			thr_lock_;
	bool					thr_stop_;
	bool					start_thr_;

private:
	// 线程相关函数
	int	thr_start(void)
		{
		if ( thr_inst_ )
			delete thr_inst_, thr_inst_ = 0;

		thr_inst_ = new boost::thread(read_thread<subproc_win32>(this));
		srlog_info("subproc_win32::thr_start | read output thread starting...");
		return 0;
		}
	int	thr_stop (int timeout = -1)
		{
		thr_stop_ = true;
		if ( thr_inst_ )
			{
			thr_inst_->join();
			delete thr_inst_, thr_inst_ = 0;
			}
		return 0;
		}
	int	thr_svc  (void)
		{
		SetEvent(thr_ready_);
		WaitForSingleObject(prc_ready_, INFINITE);

		int result = 0;
		proc_string strout;
		read_output(strout, result, -1);

		boost::mutex::scoped_lock lock(thr_lock_);
		str_output_ = strout;
		result_		= result;
		return 0;
		}
};

#endif // WIN32

// 暂时如此
typedef subproc_win32<> subproc;

} /* sp */

#endif //__PROC_INCLUDED
