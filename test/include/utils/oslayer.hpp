#ifndef __OS_LAYER_INCLUDED
#define __OS_LAYER_INCLUDED

#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>

#include <list>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <queue>

#include <assert.h>
#define trb_assert	assert

#ifdef WIN32
typedef unsigned long		ulong;
#define WIN32_LEAN_AND_MEAN
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#include <windows.h>

#define pthread_t			DWORD
#define pthread_self		GetCurrentProcessId

#include <io.h>
#include <direct.h>

#pragma warning (disable: 4100)
#pragma warning (disable: 4996)

#else
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#endif // WIN32

class os_directory
	{
public:
	// directory methods
	int open(const char* p0)
		{
#ifdef WIN32
		cur_name_ = p0;
		cur_name_ += "/*";
		ff_handle_ = _findfirst(cur_name_.c_str(), &fileinfo_);
		if (ff_handle_ == -1)
			return errno;
		return 0;
#else
		cur_name_ = p0;
		cur_name_ += "/";
		ff_handle_ = opendir(cur_name_.c_str());
		if (NULL == ff_handle_)
			return errno;
		fileinfo_ = readdir(ff_handle_);
		return 0;
#endif 
		}

	const char* next(bool& cont)
		{
#ifdef WIN32
		if (ff_handle_ == -1 || fileinfo_.name[0] == 0)
			return NULL;

		cur_name_ = fileinfo_.name;
		int ret = _findnext(ff_handle_, &fileinfo_);
		if (ret != 0)
			{
			fileinfo_.name[0] = 0;
			cont = false;
			return cur_name_.c_str();
			}
		cont = true;
		return cur_name_.c_str();
#else
		if (NULL == ff_handle_ || NULL == fileinfo_ || NULL == fileinfo_->d_name)
			{
			cont = false;
			return NULL;
			}
		cur_name_ = fileinfo_->d_name;
		fileinfo_ = readdir(ff_handle_);
		if (NULL == fileinfo_)
			{
			cont = false;
			return cur_name_.c_str();
			}
		cont = true;
		return cur_name_.c_str();
#endif // WIN32
		}

	int close()
		{
#ifdef WIN32
		if (ff_handle_ != -1)
			{
			if (0 != _findclose(ff_handle_))
				{
				ff_handle_ = -1;
				return errno;
				}
			ff_handle_ = -1;
			}
		return 0;
#else
		if (NULL != ff_handle_)
			{
			if (-1 != closedir(ff_handle_))
				{
				ff_handle_ = NULL;
				return errno;
				}
			ff_handle_ = NULL;
			}
#endif // WIN32
		}

public:
	os_directory()
#ifdef WIN32
		: ff_handle_(-1)
#else
		: ff_handle_(NULL)
#endif
		{
		}
	~os_directory()
		{
		close();
		}
	static const char* get_current_dir()
		{
#ifdef WIN32
		static char buff[MAX_PATH+1] = "C:\\";
		_getcwd(buff, MAX_PATH);
		return buff;
#else
		return get_current_dir_name();		
#endif
		}

private:
	std::string cur_name_;
#ifdef WIN32
	intptr_t	ff_handle_;
	_finddata_t	fileinfo_;
#else
	DIR*		ff_handle_;
	dirent*		fileinfo_;
#endif // WIN32
};

#endif // __OS_LAYER_INCLUDED
