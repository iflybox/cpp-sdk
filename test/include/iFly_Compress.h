/** 
 * @file	iFly_Compress.h
 * @brief	iFly使用的压缩解压函数
 * 
 * 使用zlib 开源库封装
 * 
 * @author	mwang
 * @version	1.0
 * @date	2006-9-25
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2006-9-25	<td>mwang	<td>Create this file</tr>
 * </table>
 * 
 */

#ifndef IFLY_COMPRESS_H
#define IFLY_COMPRESS_H

#include "zlib.h"

#define DLL_IMPORT __declspec( dllimport )

enum EC_METHOD
{
	EC_DEFLATE = 8 // the only method 
};

enum EC_LEVEL	
{
	EC_DEFAULT_LEV = -1,
	EC_BEST_SPEED = 1,
	EC_BEST_COMPRESSION = 9
};

enum EC_STRATEGY
{
	EC_DEFAULT_STRATEGY = 0,
	EC_HUFFMAN_ONLY = 2,
	EC_FILTERED = 1, // for small values with a somewhat random distribution   
	EC_PNG = 3,		// for PNG image data better

};

// int EC_Init (int arg1, void * arg2); 
// int EC_Fini (void);

DLL_IMPORT int EC_Compress		(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level, int strategy);

// compress data, 
DLL_IMPORT int EC_Compress2	(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen, int level, int strategy, int  method);

DLL_IMPORT int EC_Uncompress	(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);

#endif	// IFLY_COMPRESS_H