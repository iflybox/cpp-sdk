//////////////////////////////////////////////////////////////////////////
//                      Copyright 1999-2003 by iFLYTEK.
//                           All Rights Reserved
//////////////////////////////////////////////////////////////////////////

#ifndef _AUCODEC_H_
#define _AUCODEC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WIN32//Modified by hwlin, 2009-2-18 15:05:46
#include <tchar.h>
#endif // WIN32


#ifndef AU_DWORD
typedef short           AU_WORD;
typedef size_t          AU_DWORD;
typedef unsigned char   BYTE;
typedef BYTE*           PBYTE;
typedef	const char*		PCSTR;
#endif

// Export symbols
#if	defined(ISF_WINDOWS) || defined(WIN32)
#	ifdef AUCODEC_EXPORTS
#		define AUCODECAPI __declspec(dllexport)
#	else
#		define AUCODECAPI
#	endif //_USRDLL
#else
#	define	AUCODECAPI
#endif

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// 缺省名称
#if	defined(ISF_WINDOWS) || defined(WIN32)
#	if defined(_DEBUG)
		#define AU_DLL_NAME			_T("AUCodecd.dll")
#	else
		#define AU_DLL_NAME			_T("AUCodec.dll")
#	endif
#else
	#define AU_DLL_NAME			_T("libaucodec.so")
#endif
#define AU_DLL_PFN_CREATE		_T("CreateAUCodec")
#define AU_DLL_PFN_DESTORY		_T("DestroyAUCodec")

// AMR和MP3的桢大小
#define AMR_FRAME_IN			320		// L_FRAME16k
#define AMR_FRAME_OUT			37		// block_size[3]
#define MP3_FRAME_IN			576
#define MP3_FRAME_OUT			108


#pragma pack(2)

////////////////////////////////////////////////////////////////////////
// 音频头定义
////////////////////////////////////////////////////////////////////////
struct TWavePCMHdr
{
	char	    szRiff[4];			// = "RIFF"
	AU_DWORD	dwSize_8;			// = FileSize - 8
	char	    szWave[4];			// = "WAVE"
	char	    szFmt[4];			// = "fmt "
	AU_DWORD	dwFmtSize;			// = 下一个结构体的大小 : 16

	AU_WORD	    wFormatTag; 		// = PCM : 1
	AU_WORD	    wChannels;			// = 通道数 : 1
	AU_DWORD	dwSamplesPerSec;	// = 采样率 : 8000 | 6000 | 11025 | 16000
	AU_DWORD	dwAvgBytesPerSec;	// = 每秒字节数 : dwSamplesPerSec * wBitsPerSample / 8
	AU_WORD	    wBlockAlign;		// = 每采样点字节数 : wBitsPerSample / 8
	AU_WORD 	wBitsPerSample; 	// = 量化比特数: 8 | 16

	char	szData[4];			// = "data";
	AU_DWORD	dwDatasize; 		// = 纯数据长度 : FileSize - 44 
} ;

const TWavePCMHdr DEF_PCMWAVHDR = 
{
	{ 'R', 'I', 'F', 'F' },
	0,							// need modify
	{'W', 'A', 'V', 'E'},
	{'f', 'm', 't', ' '},
	16,
	1,
	1,
	8000,						// need modify
	16000,						// need modify
	2,							// need modify
	16, 						// need modify
	{'d', 'a', 't', 'a'},
	0							// need modify
};

////////////////////////////////////////////////////////////////////////
// A/U率声音数据头的定义
////////////////////////////////////////////////////////////////////////
struct TWaveAULAWHdr
{
	char	    szRiff[4];			// = "RIFF"
	AU_DWORD	dwSize_8;			// = FileSize - 8
	char	    szWave[4];			// = "WAVE"
	char	    szFmt[4];			// = "fmt "
	AU_DWORD	dwFmtSize;			// = 下一个结构体的大小 : 18
	
	AU_WORD	    wFormatTag; 		// = aLaw : 6 | uLaw : 7
	AU_WORD 	wChannels;			// = 通道数 : 1
	AU_DWORD	dwSamplesPerSec;	// = 采样率 : 8000 | 6000 | 11025 | 16000
	AU_DWORD	dwAvgBytesPerSec;	// = 每秒字节数 : dwSamplesPerSec * wBitsPerSample / 8
	AU_WORD	    wBlockAlign;		// = 每采样点字节数 : wBitsPerSample / 8
	AU_WORD	    wBitsPerSample; 	// = 量化比特数: 8 | 16
	AU_WORD	    wCbSize;			// = 下一个结构体的大小 : 0

	char	    szFact[4];			// = "fact"
	AU_WORD	    wUnknown1;			// = 4;
	AU_WORD	    wUnknown2;			// = 0;
	AU_DWORD	dwDatasize_raw; 	// = Datasize : FileSize - 58

	char	    szData[4];			// = "data";
	AU_DWORD	dwDatasize; 		// = DataSize : FileSize - 58
};

const TWaveAULAWHdr DEF_AULAWWAVHDR = 
{
	{ 'R', 'I', 'F', 'F' },
	0,							// need modify
	{'W', 'A', 'V', 'E'},
	{'f', 'm', 't', ' '},
	18,
	6,							// need modify
	1,
	8000,						// need modify
	16000,						// need modify
	2,							// need modify
	16,				 			// need modify
	0,
	{'f', 'a', 'c', 't'},
	4,
	0,
	0,							// need modify
	{'d', 'a', 't', 'a'},
	0,							// need modify
};
#pragma pack()

typedef struct TConvFmtIO
{
    PBYTE   pbData;				// 指向转化输入\输出数据的指针
    AU_DWORD   cbSize;				// 数据的长度(以字节算)
    AU_DWORD   dwFormat;			// 数据的格式
}TConvFmtIO, *PTConvFmtIO;

enum au_fmt_tag
{
    aft_none    = 0,
    aft_pcm     = 1,
    aft_alaw    = 6,
    aft_ulaw    = 7,
    aft_mp3     = 8,        // 注：mp3 没有标准值，这里的值是人为指定的

    aft_cnt     = 5,
};

// 这里dwFormat对应的是合成系统的格式，
// 为了避免歧义，则重定义一个
struct TConvFmtIOEx
{
    PBYTE       pbData;				// 指向转化输入\输出数据的指针
    DWORD       cbSize;				// 数据的长度(以字节算)
    au_fmt_tag  aft;			    // 数据的格式(注!不是合成系统的格式)

    DWORD	    dwSR;				//	采样率
    WORD	    wChannels;			//	通道数
    WORD	    wBits;		        //  比特数
};
typedef TConvFmtIOEx*	PTConvFmtIOEx;

// 采样转化输入输出数据
typedef struct TSRCIO
{
	short*	pData;				// PCM数据
	AU_DWORD	dwCnt;				// 采样点数
	AU_DWORD	dwSR;				// 采样率
} TSRCIO, *PTSRCIO;

typedef struct TMixIO
{
	short* data;
	float ratio;
} TMixIO, *PTMixIO;

typedef enum SRC_Quatity
{
	SRC_FIR_BEST				= 0, 
	SRC_FIR_MEDIUM				= 1,
	SRC_FIR_LOW_FAST			= 2,
	SRC_ZERO_ORDER_HOLD_FAST	= 3,
	SRC_LINEAR_FAST				= 4
} SRC_Q;

typedef struct TAudioInfo
{
	AU_DWORD	dwSR;				// 采样率
	AU_WORD 	wFmtTag;			// wave格式
	AU_WORD	    wBits;				// 量化比特位数
	AU_WORD	    wChannels;			// 通道数
	AU_WORD	    wBlockAgn;			// 块对齐大小
	float	    fDuration;			// 时长
	AU_DWORD	cbFile;				// 文件的长度
	AU_DWORD	cbHead;				// 文件头的长度
	AU_DWORD	cbData;				// 数据的长度
	BYTE*	    pbData;
} TAudioInfo, *PTAudioInfo;

// 音频转换接口
struct IAUCodec
{
	/////////////////////////////////////////////////////////////////////////
	// 初始化、逆初始化
	/////////////////////////////////////////////////////////////////////////
	virtual long Initialize() = 0;
	virtual long Uninitialize() = 0;

	/////////////////////////////////////////////////////////////////////////
	// 功能函数
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// 编解码函数，包括Amr, MP3；
	// nPcmLen指采样点数，函数AmrEnc，Mp3Enc输出转化后的字节数
	virtual long AmrEnc(short* pnPcm, int nPcmLen, unsigned char* pbAmr) = 0;
	virtual long Mp3Enc(short* pnPcm, int nPcmLen, unsigned char* pbMp3) = 0;
	virtual long AmrDec(BYTE* pbAmr, int nAmrLen, short* pnPcm, int nBufLen) = 0;
	virtual long Mp3Dec(BYTE* pbMp3, int nMp3Len, short* pnPcm, int nBufLen) = 0;
    virtual long AlawDec16(BYTE* pbAlaw, int nAlawLen, short* pnPcm, int& nBufLen) = 0;
    virtual long UlawDec16(BYTE* pbUlaw, int nUlawLen, short* pnPcm, int& nBufLen) = 0;

	/////////////////////////////////////////////////////////////////////////
	// 转换音频格式，包括对pcm、alaw、ulaw、amr、mp3、vox等格式的音频数据的互
	// 相转化，注意只能在相同采样率的数据间进行转换
	virtual long ConvAudioFormat(PTConvFmtIO pIn, PTConvFmtIO pOut) = 0;
	// 估算音频格式转化输出的数据的大小
	virtual long ConvFmtEvalSize(PTConvFmtIO pIn, PTConvFmtIO pOut) = 0;

	/////////////////////////////////////////////////////////////////////////
	// 采样率转换函数: 在一般的语音转换中，推荐使用那几个特定的转换函数；在高
	// 采样率的音频转换中，使用通用函数。

	// 通用任意采样率、多声道采样率转换函数
	virtual long SRCSimple(PTSRCIO pIn, PTSRCIO pOut, 
						SRC_Q srct = SRC_FIR_BEST, int nchans = 1) = 0;
	
	// 几个指定采样率的单声道采样率转换函数
	// 注意：使用SRC16KTo11K函数时要注意输出缓冲区是输入缓冲区长度的4倍左右,
	// 这里传入的nSize为采样点数
	virtual long SRC16KTo08K(short* pIn, int nSize, short* pOut) = 0;
	virtual long SRC16KTo06K(short* pIn, int nSize, short* pOut) = 0;
	virtual long SRC16KTo11K(short* pIn, int nSize, short* pOut) = 0;
	virtual long SRC08KTo06K(short* pIn, int nSize, short* pOut) = 0;

	/////////////////////////////////////////////////////////////////////////
	// 其它函数

	// 获取音频文件信息
	virtual long GetAudioInfo(PCSTR szFile, TAudioInfo& tInfo) = 0;
	// 多通道音频混音函数，bits只能为8和16
	virtual void WaveMix(BYTE chans, PTMixIO io, long count, BYTE out = 0, BYTE bits = 16) = 0;
	// 线程安全的MP3解码函数
	virtual long MADDec(unsigned char* pbMp3, int& cbMp3, short* pnPcm, int& nCnt, int& nSR) = 0;

    // mp3 解码时，需要一定的缓冲区，为了方便多次解码。做了缓冲区预分配
    // 由于aucodec 与实例无关，所以目前的方式是绑定在了当前线程中。
    // 如果是基于线程池实现，可以考虑prealloc_mem=true
    // 注：预分配的内存，会绑定在当前工作线程中。直到系统退出时才释放
    virtual long MADDec(const char* psz_mp3_in_path, const char* psz_pcm_out_path, bool prealloc_mem = false) = 0;

    // 将支持的A/U-lav PCM转成指定采样率的PCM
    virtual long ConvAudioFormatEx(PTConvFmtIOEx pIn, PTConvFmtIOEx pOut) = 0;
};

typedef IAUCodec *PIAUCodec;

// 创建音频编解码器接口
long AUCODECAPI CreateAUCodec(IAUCodec** ppAUCodec);
typedef long AUCODECAPI (*PROC_AUCODEC_CREATE)(IAUCodec** ppAUCodec);

// 释放音频编解码器接口
long AUCODECAPI DestroyAUCodec(IAUCodec* pAUCodec);
typedef long AUCODECAPI (*PROC_AUCODEC_DESTROY)(IAUCodec* pAUCodec);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _AUCODEC_H_
