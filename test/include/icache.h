/**
* @file    icache.h
* @brief   iFLY Speech Recognizer Header File
* 
*  This file contains Speech Detector API declarations 
*  of ISR. Developer can include this file in your project to build applications.
*  For more information, please read the developer guide.

*  Use of this software is subject to certain restrictions and limitations set
*  forth in a license agreement entered into between iFLYTEK, Co,LTD.
*  and the licensee of this software.  Please refer to the license
*  agreement for license use rights and restrictions.
*
*  Copyright (C)    1999 - 2007 by ANHUI USTC iFLYTEK, Co,LTD.
*                   All rights reserved.
* 
* @author  Speech Dept. iFLYTEK.
* @version 1.0
* @date    2007/04/17
* 
* @see        
* 
* History:
* index    version        date            author        notes
* 0        1.0            2007/04/17      Speech        Create this file
*/

#ifndef __ICACHE_H__
#define __ICACHE_H__

#if defined(_MSC_VER)            /* Microsoft Visual C++ */
#if !defined(ICACHEAPI)
#define ICACHEAPI __stdcall
#endif
#pragma pack(push, 8)
#elif defined(__BORLANDC__)      /* Borland C++ */
#if !defined(ICACHEAPI)
#define ICACHEAPI __stdcall
#endif
#pragma option -a8
#elif defined(__WATCOMC__)       /* Watcom C++ */
#if !defined(ICACHEAPI)
#define ICACHEAPI __stdcall
#endif
#pragma pack(push, 8)
#else                            /* Any other including Unix */
#if !defined(ICACHEAPI)
#define ICACHEAPI
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
typedef void* HCACHEINSTANCE;
typedef int CACHERETVAL;
/** 
 * @brief	
 * 
 * detail...
 * 
 * @author	IFLYTEK
 * @date	2008年5月22日
 * @return	int ICACHEAPI 
 * @param	const char * engine
 * @param	void * reserved
 * @see		
 * @exception 
 */
CACHERETVAL ICACHEAPI IFLYCacheInit(const char * szEngineName, void * pReserved);
typedef CACHERETVAL(ICACHEAPI *Proc_IFLYCacheInit)(const char * szEngineName, void * pReserved);
	

/** 
 * @brief	
 * 
 * detail...
 * 
 * @author	IFLYTEK
 * @date	2008年5月22日
 * @return	int ICACHEAPI 
 * @param	void
 * @see		
 * @exception 
 */
CACHERETVAL ICACHEAPI IFLYCacheFini(void);
typedef CACHERETVAL (ICACHEAPI * Proc_IFLYCacheFini)(void);

/** 
* @brief	异步接口，向服务器中添加一个URL。
* 
* detail...
* 
* @author	IFLYTEK
* @date	2008年5月22日
* @return	int ICACHEAPI 
* @param	const char *url
* @param	const char * server_ip
* @see		
* @exception 
*/
CACHERETVAL ICACHEAPI IFLYCacheAddUrl(const char *szUrl, const char * szServerIp);
typedef CACHERETVAL  (ICACHEAPI *Proc_IFLYCacheAddUrl)(const char *szUrl, const char * szServerIp);

/** 
 * @brief	
 * 
 * detail...
 * 
 * @author	IFLYTEK
 * @date	2008年5月22日
 * @return	int ICACHEAPI 
 * @param	const char *url
 * @param	const char * server_ip
 * @param	unsigned int len
 * @see		
 * @exception 
 */
CACHERETVAL ICACHEAPI IFLYCacheOpenUrl(HCACHEINSTANCE* hInst, const char *szUrl, const char * szServerIp);
typedef CACHERETVAL  (ICACHEAPI *Proc_IFLYCacheOpenUrl)(HCACHEINSTANCE* hInst, const char *szUrl, const char * szServerIp);
	


/** 
 * @brief	
 * 
 * detail...
 * 
 * @author	IFLYTEK
 * @date	2008年5月27日
 * @return	int ICACHEAPI 
 * @param	HCACHEINSTANCE hInst
 * @param	void* szBuff
 * @param	const unsigned int nBeginpos
 * @param	unsigned int * pBuffSize
 * @see		
 * @exception 
 */
CACHERETVAL ICACHEAPI IFLYCacheFetch(HCACHEINSTANCE hInst, void* pBuff, const unsigned int nBeginpos, unsigned int * pBuffSize);
typedef CACHERETVAL (ICACHEAPI *Proc_IFLYCacheFetch)(HCACHEINSTANCE hInst, void* pBuff, const unsigned int nBeginpos, unsigned int * pBuffSize);


/** 
 * @brief	
 * 
 * detail...
 * 
 * @author	zhli2
 * @date	2008年6月2日
 * @return	CACHERETVAL ICACHEAPI 
 * @param	HCACHEINSTANCE hInst
 * @param	const char* szQueryValue
 *			"",     return all value,formate is {name=valude;}*;
 *          "AudioFmt  return "AudioFmt=8bit/8K";
 *			"AudioSize return "AudioSize=10000";
 * @param	void* pOutBuff
 * @param	unsigned int * pBuffSize
 * @see		
 * @exception 
 */
CACHERETVAL ICACHEAPI IFLYCacheQuery(const char * url, const char* szQueryName, void* pOutBuff, unsigned int * pBuffSize, const char * szServerIp );
typedef CACHERETVAL (ICACHEAPI *Proc_IFLYCacheQuery)(const char * url, const char* szQueryName, void* pOutBuff, unsigned int * pBuffSize, const char * szServerIp);

/** 
 * @brief	
 * 
 * detail...
 * 
 * @author	IFLYTEK
 * @date	2008年5月27日
 * @return	int ICACHEAPI 
 * @param	HCACHEINSTANCE hInst
 * @see		
 * @exception 
 */
CACHERETVAL ICACHEAPI IFLYCacheCloseUrl(HCACHEINSTANCE hInst);
typedef CACHERETVAL (ICACHEAPI *Proc_IFLYCacheCloseUrl)(HCACHEINSTANCE hInst);

#ifdef __cplusplus
} /* extern "C" */
#endif

/////
// 缺省名称
#if	defined(ISF_WINDOWS) || defined(WIN32)
#	if defined(_DEBUG)
#define IFLY_CACHE_DLL_NAME			_T("icached.dll")
#	else
#define IFLY_CACHE_DLL_NAME			_T("icache.dll")
#	endif
#else
#define IFLY_CACHE_DLL_NAME			_T("icache.so")
#endif

#endif /* __ICACHE_H__ */
