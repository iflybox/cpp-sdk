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

// ȱʡ����
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

// AMR��MP3�����С
#define AMR_FRAME_IN			320		// L_FRAME16k
#define AMR_FRAME_OUT			37		// block_size[3]
#define MP3_FRAME_IN			576
#define MP3_FRAME_OUT			108


#pragma pack(2)

////////////////////////////////////////////////////////////////////////
// ��Ƶͷ����
////////////////////////////////////////////////////////////////////////
struct TWavePCMHdr
{
	char	    szRiff[4];			// = "RIFF"
	AU_DWORD	dwSize_8;			// = FileSize - 8
	char	    szWave[4];			// = "WAVE"
	char	    szFmt[4];			// = "fmt "
	AU_DWORD	dwFmtSize;			// = ��һ���ṹ��Ĵ�С : 16

	AU_WORD	    wFormatTag; 		// = PCM : 1
	AU_WORD	    wChannels;			// = ͨ���� : 1
	AU_DWORD	dwSamplesPerSec;	// = ������ : 8000 | 6000 | 11025 | 16000
	AU_DWORD	dwAvgBytesPerSec;	// = ÿ���ֽ��� : dwSamplesPerSec * wBitsPerSample / 8
	AU_WORD	    wBlockAlign;		// = ÿ�������ֽ��� : wBitsPerSample / 8
	AU_WORD 	wBitsPerSample; 	// = ����������: 8 | 16

	char	szData[4];			// = "data";
	AU_DWORD	dwDatasize; 		// = �����ݳ��� : FileSize - 44 
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
// A/U����������ͷ�Ķ���
////////////////////////////////////////////////////////////////////////
struct TWaveAULAWHdr
{
	char	    szRiff[4];			// = "RIFF"
	AU_DWORD	dwSize_8;			// = FileSize - 8
	char	    szWave[4];			// = "WAVE"
	char	    szFmt[4];			// = "fmt "
	AU_DWORD	dwFmtSize;			// = ��һ���ṹ��Ĵ�С : 18
	
	AU_WORD	    wFormatTag; 		// = aLaw : 6 | uLaw : 7
	AU_WORD 	wChannels;			// = ͨ���� : 1
	AU_DWORD	dwSamplesPerSec;	// = ������ : 8000 | 6000 | 11025 | 16000
	AU_DWORD	dwAvgBytesPerSec;	// = ÿ���ֽ��� : dwSamplesPerSec * wBitsPerSample / 8
	AU_WORD	    wBlockAlign;		// = ÿ�������ֽ��� : wBitsPerSample / 8
	AU_WORD	    wBitsPerSample; 	// = ����������: 8 | 16
	AU_WORD	    wCbSize;			// = ��һ���ṹ��Ĵ�С : 0

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
    PBYTE   pbData;				// ָ��ת������\������ݵ�ָ��
    AU_DWORD   cbSize;				// ���ݵĳ���(���ֽ���)
    AU_DWORD   dwFormat;			// ���ݵĸ�ʽ
}TConvFmtIO, *PTConvFmtIO;

enum au_fmt_tag
{
    aft_none    = 0,
    aft_pcm     = 1,
    aft_alaw    = 6,
    aft_ulaw    = 7,
    aft_mp3     = 8,        // ע��mp3 û�б�׼ֵ�������ֵ����Ϊָ����

    aft_cnt     = 5,
};

// ����dwFormat��Ӧ���Ǻϳ�ϵͳ�ĸ�ʽ��
// Ϊ�˱������壬���ض���һ��
struct TConvFmtIOEx
{
    PBYTE       pbData;				// ָ��ת������\������ݵ�ָ��
    DWORD       cbSize;				// ���ݵĳ���(���ֽ���)
    au_fmt_tag  aft;			    // ���ݵĸ�ʽ(ע!���Ǻϳ�ϵͳ�ĸ�ʽ)

    DWORD	    dwSR;				//	������
    WORD	    wChannels;			//	ͨ����
    WORD	    wBits;		        //  ������
};
typedef TConvFmtIOEx*	PTConvFmtIOEx;

// ����ת�������������
typedef struct TSRCIO
{
	short*	pData;				// PCM����
	AU_DWORD	dwCnt;				// ��������
	AU_DWORD	dwSR;				// ������
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
	AU_DWORD	dwSR;				// ������
	AU_WORD 	wFmtTag;			// wave��ʽ
	AU_WORD	    wBits;				// ��������λ��
	AU_WORD	    wChannels;			// ͨ����
	AU_WORD	    wBlockAgn;			// ������С
	float	    fDuration;			// ʱ��
	AU_DWORD	cbFile;				// �ļ��ĳ���
	AU_DWORD	cbHead;				// �ļ�ͷ�ĳ���
	AU_DWORD	cbData;				// ���ݵĳ���
	BYTE*	    pbData;
} TAudioInfo, *PTAudioInfo;

// ��Ƶת���ӿ�
struct IAUCodec
{
	/////////////////////////////////////////////////////////////////////////
	// ��ʼ�������ʼ��
	/////////////////////////////////////////////////////////////////////////
	virtual long Initialize() = 0;
	virtual long Uninitialize() = 0;

	/////////////////////////////////////////////////////////////////////////
	// ���ܺ���
	/////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////
	// ����뺯��������Amr, MP3��
	// nPcmLenָ��������������AmrEnc��Mp3Enc���ת������ֽ���
	virtual long AmrEnc(short* pnPcm, int nPcmLen, unsigned char* pbAmr) = 0;
	virtual long Mp3Enc(short* pnPcm, int nPcmLen, unsigned char* pbMp3) = 0;
	virtual long AmrDec(BYTE* pbAmr, int nAmrLen, short* pnPcm, int nBufLen) = 0;
	virtual long Mp3Dec(BYTE* pbMp3, int nMp3Len, short* pnPcm, int nBufLen) = 0;
    virtual long AlawDec16(BYTE* pbAlaw, int nAlawLen, short* pnPcm, int& nBufLen) = 0;
    virtual long UlawDec16(BYTE* pbUlaw, int nUlawLen, short* pnPcm, int& nBufLen) = 0;

	/////////////////////////////////////////////////////////////////////////
	// ת����Ƶ��ʽ��������pcm��alaw��ulaw��amr��mp3��vox�ȸ�ʽ����Ƶ���ݵĻ�
	// ��ת����ע��ֻ������ͬ�����ʵ����ݼ����ת��
	virtual long ConvAudioFormat(PTConvFmtIO pIn, PTConvFmtIO pOut) = 0;
	// ������Ƶ��ʽת����������ݵĴ�С
	virtual long ConvFmtEvalSize(PTConvFmtIO pIn, PTConvFmtIO pOut) = 0;

	/////////////////////////////////////////////////////////////////////////
	// ������ת������: ��һ�������ת���У��Ƽ�ʹ���Ǽ����ض���ת���������ڸ�
	// �����ʵ���Ƶת���У�ʹ��ͨ�ú�����

	// ͨ����������ʡ�������������ת������
	virtual long SRCSimple(PTSRCIO pIn, PTSRCIO pOut, 
						SRC_Q srct = SRC_FIR_BEST, int nchans = 1) = 0;
	
	// ����ָ�������ʵĵ�����������ת������
	// ע�⣺ʹ��SRC16KTo11K����ʱҪע����������������뻺�������ȵ�4������,
	// ���ﴫ���nSizeΪ��������
	virtual long SRC16KTo08K(short* pIn, int nSize, short* pOut) = 0;
	virtual long SRC16KTo06K(short* pIn, int nSize, short* pOut) = 0;
	virtual long SRC16KTo11K(short* pIn, int nSize, short* pOut) = 0;
	virtual long SRC08KTo06K(short* pIn, int nSize, short* pOut) = 0;

	/////////////////////////////////////////////////////////////////////////
	// ��������

	// ��ȡ��Ƶ�ļ���Ϣ
	virtual long GetAudioInfo(PCSTR szFile, TAudioInfo& tInfo) = 0;
	// ��ͨ����Ƶ����������bitsֻ��Ϊ8��16
	virtual void WaveMix(BYTE chans, PTMixIO io, long count, BYTE out = 0, BYTE bits = 16) = 0;
	// �̰߳�ȫ��MP3���뺯��
	virtual long MADDec(unsigned char* pbMp3, int& cbMp3, short* pnPcm, int& nCnt, int& nSR) = 0;

    // mp3 ����ʱ����Ҫһ���Ļ�������Ϊ�˷����ν��롣���˻�����Ԥ����
    // ����aucodec ��ʵ���޹أ�����Ŀǰ�ķ�ʽ�ǰ����˵�ǰ�߳��С�
    // ����ǻ����̳߳�ʵ�֣����Կ���prealloc_mem=true
    // ע��Ԥ������ڴ棬����ڵ�ǰ�����߳��С�ֱ��ϵͳ�˳�ʱ���ͷ�
    virtual long MADDec(const char* psz_mp3_in_path, const char* psz_pcm_out_path, bool prealloc_mem = false) = 0;

    // ��֧�ֵ�A/U-lav PCMת��ָ�������ʵ�PCM
    virtual long ConvAudioFormatEx(PTConvFmtIOEx pIn, PTConvFmtIOEx pOut) = 0;
};

typedef IAUCodec *PIAUCodec;

// ������Ƶ��������ӿ�
long AUCODECAPI CreateAUCodec(IAUCodec** ppAUCodec);
typedef long AUCODECAPI (*PROC_AUCODEC_CREATE)(IAUCodec** ppAUCodec);

// �ͷ���Ƶ��������ӿ�
long AUCODECAPI DestroyAUCodec(IAUCodec* pAUCodec);
typedef long AUCODECAPI (*PROC_AUCODEC_DESTROY)(IAUCodec* pAUCodec);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _AUCODEC_H_
