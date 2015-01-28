#ifndef __FDSTREAM_INCLUDED
#define __FDSTREAM_INCLUDED

#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <share.h>
#include <streambuf>
#include <iostream>
#include "../base/srbase.h"

_SR_BEGIN

#ifdef _WIN32
enum {
	of_rdonly =	_O_RDONLY,
	of_wronly = _O_WRONLY,
	of_rdwr = _O_RDWR,
	of_append = _O_APPEND,

	of_creat = _O_CREAT,
	of_trunc = _O_TRUNC, 
	of_excl = _O_EXCL,

	of_text = _O_TEXT,  
	of_binary = _O_BINARY,
	of_wtext = _O_WTEXT,
	of_u16text = _O_U16TEXT,
	of_u8text = _O_U8TEXT
	};

enum 
	{
	read_pipe,
	write_pipe
	};


inline int wopen(pcwstr filename, int flags, int mode)
	{
	int fd = -1;
	if (filename != NULL && *filename != 0)
		{
		errno_t err = ::_wsopen_s(&fd, 
			filename, flags|_O_BINARY, _SH_DENYRW, _S_IREAD | _S_IWRITE);
		_set_errno(err);
		}
	return fd;
	}

inline int close(int fh)
	{
	return ::_close(fh);
	}

inline size_t read(int fh, void* buff_p, size_t nbyte)
	{
	return ::_read(fh, buff_p, unsigned(nbyte));
	}

inline size_t write(int fh, const void *buf, size_t nbyte)
	{
	return ::_write(fh, buf, unsigned(nbyte));
	}

inline int flush(int fh)
	{
	return ::_commit(fh);
	}

inline int pipe(int fildes[2])
	{
	return ::_pipe(fildes, 4096*4, O_BINARY);
	}
#else
	// unix
	using ::open;
	using ::close;
	using ::read;
	using ::write;
#endif // _WIN32

using namespace std;

template<class _CharT, class _Traits, class _Alloc>
class basic_fdbuf
	: public std::basic_streambuf<_CharT, _Traits>
	{
public:                         // Typedefs.
	typedef _CharT                     char_type;
	typedef typename _Traits::int_type int_type;
	typedef typename _Traits::pos_type pos_type;
	typedef typename _Traits::off_type off_type;
	typedef _Traits                    traits_type;

	typedef basic_streambuf<_CharT, _Traits>          _Base;
	typedef basic_stringbuf<_CharT, _Traits, _Alloc>  _Self;
	typedef basic_string<_CharT, _Traits, _Alloc>     _String;

public:
	basic_fdbuf(fd_type fh)
		{
		init(fh);
		}

	fd_type file_handle() const
		{
		return file_handle_;
		}

	virtual ~basic_fdbuf()
		{
		}

protected:                      // Virtual get area functions, as defined in
                                // 17.5.2.4.3 and 17.5.2.4.4 of the standard.
	// Returns a lower bound on the number of characters that we can read,
	// with underflow, before reaching end of file.  (-1 is a special value:
	// it means that underflow will fail.)  Most subclasses should probably
	// override this virtual member function.
//	virtual streamsize showmanyc();

	// Reads up to __n characters.  Return value is the number of 
	// characters read.
	//virtual streamsize xsgetn(char_type* __s, streamsize __n)
	//	{
	//	return 0;
	//	}

	// Called when there is no read position, i.e. when gptr() is null
	// or when gptr() >= egptr().  Subclasses are expected to override
	// this virtual member function.
	virtual int_type underflow()
		{
		if (file_handle_ == invalid_fh)
			return traits_type::eof();

		_CharT* __ibegin = _Base::eback();
		_CharT* __iend = _Base::egptr();
		ptrdiff_t rdc = read(__ibegin, __iend);
		setg(__ibegin, __ibegin, __ibegin+rdc);
		return rdc == 0 ?
			traits_type::eof() :
			traits_type::to_int_type(*__ibegin);
		}

	// Similar to underflow(), but used for unbuffered input.  Most 
	// subclasses should probably override this virtual member function.
	//virtual int_type uflow()
	//	{
	//	return traits_type::eof();
	//	}

	// Called when there is no putback position, i.e. when gptr() is null
	// or when gptr() == eback().  All subclasses are expected to override
	// this virtual member function.
	//virtual int_type pbackfail(int_type = traits_type::eof())
	//	{
	//	return traits_type::eof();
	//	}

protected:                      // Virtual put area functions, as defined in
                                // 27.5.2.4.5 of the standard.
	// Writes up to __n characters.  Return value is the number of characters
	// written.
	//virtual streamsize xsputn(const char_type* __s, streamsize __n)
	//	{
	//	return 0;
	//	}


	// Called when there is no write position.  All subclasses are expected to
	// override this virtual member function.
	virtual int_type overflow(int_type __c = traits_type::eof())
		{
	 // Switch to output mode, if necessary.
	 // if (!_M_in_output_mode)
		//if (!_M_switch_to_output_mode())
		//  return traits_type::eof();

		_CharT* __ibegin = file_buffer_;
		_CharT* __iend  = pptr();
		this->setp(file_buffer_, file_buffer_+fd_buff_size-1);

		// Put __c at the end of the internal buffer.
		if (!traits_type::eq_int_type(__c, traits_type::eof()))
			*__iend++ = __c;

		// For variable-width encodings, output may take more than one pass.
		while (__ibegin != __iend)
			{
			//const _CharT* __inext = __ibegin;
			//char* __enext         = _M_ext_buf;
			//typename _Codecvt::result __status
			//  = _M_codecvt->out(_M_state, __ibegin, __iend, __inext,
			//							  _M_ext_buf, _M_ext_buf_EOS, __enext);
			//if (__status == _Codecvt::noconv)
			 // return _Noconv_output<_Traits>::_M_doit(this, __ibegin, __iend)
				//? traits_type::not_eof(__c)
				//: _M_output_error();
			return (__iend - __ibegin) == write(__ibegin, __iend);

			// For a constant-width encoding we know that the external buffer
			// is large enough, so failure to consume the entire internal buffer
			// or to produce the correct number of external characters, is an error.
			// For a variable-width encoding, however, we require only that we 
			// consume at least one internal character
			//else if (__status != _Codecvt::error && 
			//		 ((__inext == __iend && (__enext - _M_ext_buf == 
			//								 _M_width * (__iend - __ibegin))) ||
			//		  (!_M_constant_width && __inext != __ibegin)))
			//	{
			//	// We successfully converted part or all of the internal buffer.
			//	ptrdiff_t __n = __enext - _M_ext_buf;
			//	if (_M_write(_M_ext_buf, __n))
			//		__ibegin += __inext - __ibegin;
			//	else
			//		return _M_output_error();
			//	}
			//else
			//  return _M_output_error();
			}

		return traits_type::not_eof(__c);
		}

public:
	virtual int sync()
		{
		return traits_type::eq_int_type(
				this->overflow(traits_type::eof()),
				traits_type::eof())
			? -1
			: flush(file_handle_);
		}

	static const int fd_buff_size = 64;

private:
	void init(fd_type fh)
		{
		file_handle_ = fh;
		setp(file_buffer_, file_buffer_+fd_buff_size-1);
		setg(file_buffer_, file_buffer_+fd_buff_size, file_buffer_+fd_buff_size);
		}

	ptrdiff_t read(_CharT* pbegin, _CharT* pend) const
		{
		const ptrdiff_t __char_size = sizeof(_CharT);
		return sr::read(file_handle_, pbegin,
			(pend-pbegin)*__char_size) / __char_size;
		}

	ptrdiff_t write(_CharT* pbegin, _CharT* pend) const
		{
		const ptrdiff_t __char_size = sizeof(_CharT);
		return sr::write(file_handle_, pbegin,
			(pend-pbegin)*__char_size) / __char_size;
		}

	sr::fd_type		file_handle_;
	_CharT			file_buffer_[fd_buff_size];
	};

template <class _CharT, class _Traits, class _Alloc=allocator<_CharT> >
class basic_ifdstream
	: public basic_istream<_CharT, _Traits>
{
public:                         // Typedefs
	typedef typename _Traits::char_type   char_type;
	typedef typename _Traits::int_type    int_type;
	typedef typename _Traits::pos_type    pos_type;
	typedef typename _Traits::off_type    off_type;
	typedef _Traits traits_type;

	typedef basic_fdbuf<_CharT, _Traits, _Alloc>	_Buf;

public:
	basic_ifdstream(fd_type fd = invalid_fh)
		: basic_istream(&fdbuf_)
		, fdbuf_(fd)
		{
		}

template <typename data_type>
	size_t read_data(data_type& da)
		{
		read((char*)&da, sizeof(da));
		return sizeof(da);
		};

template <>
	size_t read_data(std::wstring& ws)
		{
		short int meta = 0;
		while (meta != traits_type::eof())
			{
			bool _eof = eof();
			read((char*)&meta, sizeof(meta));
			if (eof() && !_eof)
				{
				unget();
				break;
				}
			if (meta == 0)
				break;
			ws.push_back(wchar_t(meta));
			}
		return (ws.length()+1) * sizeof(meta);
		}

private:
	_Buf	fdbuf_;
};

//----------------------------------------------------------------------
// Class basic_ostringstream, an output stream that uses a stringbuf.

template <class _CharT, class _Traits, class _Alloc=allocator<_CharT> >
class basic_ofdstream
	: public basic_ostream<_CharT, _Traits>
{
public:                         // Typedefs
	typedef typename _Traits::char_type   char_type;
	typedef typename _Traits::int_type    int_type;
	typedef typename _Traits::pos_type    pos_type;
	typedef typename _Traits::off_type    off_type;
	typedef _Traits traits_type;

	//typedef basic_ios<_CharT, _Traits>                _Basic_ios;
	//typedef basic_ostream<_CharT, _Traits>            _Base;
	//typedef basic_string<_CharT, _Traits, _Alloc>     _String;
	typedef basic_fdbuf<_CharT, _Traits, _Alloc>		_Buf;

public:
	basic_ofdstream(fd_type fd = invalid_fh)
		: basic_ostream(&fdbuf_)
		, fdbuf_(fd)
		{
		}

	virtual ~basic_ofdstream()
		{
		fdbuf_.sync();
		}

	fd_type handle() const
		{
		return fdbuf_.file_handle();
		}

template <typename data_type>
	size_t write_data(const data_type& da)
		{
		write((char*)&da, sizeof(da));
		return sizeof(da);
		};

template <>
	size_t write_data(const std::wstring& str)
		{
		short int meta = 0;
		size_t i = 0;
		for ( ; i < str.length(); i ++)
			{
			meta = (short int)(str[i]);
			write((char*)&meta, sizeof(meta));
			if (fail())
				{
				break;
				}
			}
		meta = 0;
		write((char*)&meta, sizeof(meta));
		return i * sizeof(meta);
		}

private:
	_Buf	fdbuf_;
};


template <class _CharT, class _Traits, class _Alloc=allocator<_CharT> >
class basic_fdstream
	: public basic_iostream<_CharT, _Traits>
{
public:                         // Typedefs
	typedef typename _Traits::char_type char_type;
	typedef typename _Traits::int_type  int_type;
	typedef typename _Traits::pos_type  pos_type;
	typedef typename _Traits::off_type  off_type;
	typedef _Traits  traits_type;
};

typedef basic_ifdstream<char, char_traits<char> > ifdstream;
typedef basic_ofdstream<char, char_traits<char> > ofdstream;
typedef basic_fdstream<char, char_traits<char> > fdstream;

typedef basic_ifdstream<wchar_t, char_traits<wchar_t> > wifdstream;
typedef basic_ofdstream<wchar_t, char_traits<wchar_t> > wofdstream;
typedef basic_fdstream<wchar_t, char_traits<wchar_t> > wfdstream;

_SR_END

#endif // __FDSTREAM_INCLUDED
