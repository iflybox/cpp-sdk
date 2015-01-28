#ifndef __MACRO_DEFINE_H__
#define __MACRO_DEFINE_H__

#ifdef WIN32
	#define ALIGN_N(n) __declspec(align(n))
#else // LINU
	typedef unsigned long       DWORD;
	typedef unsigned short		WORD;
	typedef void*				HINSTANCE;
	typedef unsigned char       BYTE;
	typedef BYTE*				PBYTE;
	typedef unsigned char		byte;
	typedef DWORD				DWORD_PTR;
	typedef const char			PCSTR;
	typedef DWORD				ULONG;
	typedef unsigned long		ULONG_PTR, *PULONG_PTR;
	#define PtrToUlong( p ) ((ULONG)(ULONG_PTR) (p) )
	#define ALIGN_N(n) 
	#define PtrToUlong( p ) ((ULONG)(ULONG_PTR) (p) )

#ifndef min
#define min(x, y) std::min(x, y)
#endif

#ifndef max
#define max(x, y) std::max(x, y)
#endif
	
#endif

#endif // __MACRO_DEFINE_H__