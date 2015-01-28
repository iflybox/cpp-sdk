/** 
 * @file	iFly_MSP.h
 * @brief	MSP�ӿڶ���
 * 
 *  �����˹���ӿں͸����ӿ�
 * 
 * @author	jdyu
 * @version	1.0
 * @date	2009/11/26
 * 
 * @see		
 * 
 * <b>History:</b><br>
 * <table>
 *  <tr> <th>Version	<th>Date		<th>Author	<th>Notes</tr>
 *  <tr> <td>1.0		<td>2009/11/26	<td>jdyu	<td>Create this file</tr>
 * </table>
 * 
 */
#ifndef __IFLY_MSP_H__
#define __IFLY_MSP_H__

/**
 * MSP API type
 */
#if defined(_MSC_VER)            /* Microsoft Visual C++ */
  #if !defined(MSPAPI)
    #define MSPAPI __stdcall
  #endif
  #pragma pack(push, 8)
#elif defined(__BORLANDC__)      /* Borland C++ */
  #if !defined(MSPAPI)
    #define MSPAPI __stdcall
  #endif
  #pragma option -a8
#elif defined(__WATCOMC__)       /* Watcom C++ */
  #if !defined(MSPAPI)
    #define MSPAPI __stdcall
  #endif
  #pragma pack(push, 8)
#else                            /* Any other including Unix */
  #if !defined(MSPAPI)
    #define MSPAPI
  #endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /* C++ */

/** 
 * @fn		MSPInit
 * @brief	��ʼ������
 * 
 *  ���Բ�����
 * 
 * @author	jdyu
 * @date	2009/11/26
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* configs	- [in] ��ʼ����������Ϣ
 * @see		
 */
int MSPAPI MSPInit(const char* configs);

/** 
 * @fn		MSPUserRegister
 * @brief	ע���û�
 * 
 *  ��MSPƽ̨ע���û������һ��Key
 * 
 * @author	jdyu
 * @date	2009/11/27
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* userInfo	- [in] 
 * @param	const char** userKey	- [in] 
 * @param	const char** keyInfo	- [in] 
 * @see		
 */
int MSPAPI MSPUserRegister(const char* userInfo, const char** userKey, const char** keyInfo);

/** 
 * @fn		MSPKeyValidate
 * @brief	��֤һ��key
 * 
 *  ��֤һ��key����Ч��
 * 
 * @author	jdyu
 * @date	2009/11/27
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* userKey	- [in] 
 * @param	const char** keyInfo	- [in] 
 * @see		
 */
int MSPAPI MSPKeyValidate(const char* userKey, const char** keyInfo);

/** 
 * @fn		MSPServiceQuery
 * @brief	��ѯMSP�ṩ�ķ���
 * 
 *  �����ṩ�������һ��XML�ļ��ĸ�ʽ�ṩ
 * 
 * @author	jdyu
 * @date	2009/11/27
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @param	const char* optionStr	- [in] 
 * @param	const char** serviceInfo	- [in] 
 * @see		
 */
int MSPAPI MSPServiceQuery(const char* optionStr, const char** serviceInfo);

/** 
 * @fn		MSPFini
 * @brief	���ʼ��
 * 
 *  ���Բ�����
 * 
 * @author	jdyu
 * @date	2009/11/26
 * @return	int MSPAPI	- Return 0 in success, otherwise return error code.
 * @see		
 */
int MSPAPI MSPFini(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* C++ */

/* Reset the structure packing alignments for different compilers. */
#if defined(_MSC_VER)            /* Microsoft Visual C++ */
  #pragma pack(pop)
#elif defined(__BORLANDC__)      /* Borland C++ */
  #pragma option -a.
#elif defined(__WATCOMC__)       /* Watcom C++ */
  #pragma pack(pop)
#endif

#endif /* __IFLY_MSP_H__ */
