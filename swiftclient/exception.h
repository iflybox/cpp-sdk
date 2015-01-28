#ifndef __IFLYBOX__EXCEPTION_H__
#define __IFLYBOX__EXCEPTION_H__

#include <exception>
#include <string>
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <stdexcept>

namespace iflybox
{

	class iflyException 
		: public std::runtime_error{
	public:
		iflyException(int error, const char* what, const char* file, int line)
			: std::runtime_error(what), error_(error), file_(file), line_(line), detail_(NULL){

		}
		~iflyException() throw() {
			delete [] detail_;
		}
		iflyException(): std::runtime_error(""){}
		int error()const{
			return error_;
		}
		const char* detail()const{
			if(detail_ == NULL)
			{
				int len = strlen(what()) + strlen(file_) + 50;
				detail_ = new char[len];
#ifdef _WIN32
				sprintf_s(detail_, len, "%s(%d) throw exception:%s, error:%d", file_, line_, what(), error_);
#else
				snprintf(detail_, len, "%s(%d) throw exception:%s, error:%d", file_, line_, what(), error_);
#endif
			}
			return detail_;
		}
	protected:
		int				error_;
		mutable char*	detail_;
		const char*		file_;
		int				line_;
	};

	class iflyParamException
		: public iflyException{
	public:
		iflyParamException(int error, const char* what, const char* file, int line)
			:iflyException(error, what, file, line){

		}
	};

	class iflyCurlException
		: public iflyException{
	public:
		iflyCurlException(int error, const char* what, const char* file, int line)
			:iflyException(error, what, file, line){
			
		}
	};

}


#endif // __IFLYBOX__EXCEPTION_H__
