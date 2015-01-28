/**
 * @file    isr_as.h
 * @brief   iFLY Speech Recognizer Header File
 * 
 *  This file contains the application programming interface (API) declarations 
 *  of ISR. Developer can include this file in your project to build applications.
 *  For more information, please read the developer guide.
 
 *  Use of this software is subject to certain restrictions and limitations set
 *  forth in a license agreement entered into between iFLYTEK, Co,LTD.
 *  and the licensee of this software.  Please refer to the license
 *  agreement for license use rights and restrictions.
 *
 *  Copyright (C)    1999 - 2007 by ANHUI USTC iFLYTEK Co,LTD.
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

#ifndef __ISR_AS_H__
#define __ISR_AS_H__

#include "base/srbase.h"
#include "isr_errors.h"
#include "isr_types.h"

#pragma warning(disable : 4996)

_BEGIN_NAMESPACE(as)

/*----------------------------------------------------------------------------
* Audio encoding:
*---------------------------------------------------------------------------*/

typedef enum {
	AudioFirstEncoding_ = 0,                    /* Internal use only */
	AudioEncodingUnknown = AudioFirstEncoding_, /* Unknown or don't care */
	AudioEncodingUlaw,                         /* u-law (misspelled) */
	AudioEncodingAlaw,                          /* a-law */
	AudioEncodingLinear,                        /* linear */
	AudioNumEncodings_                          /* Internal use only */
} audio_encoding;

/*----------------------------------------------------------------------------
* Audio format:
*---------------------------------------------------------------------------*/

typedef struct {
	int             rate;               /* Sample rate [samples/sec] */
	int             bits;               /* Sample resolution [bits/sample] */
	audio_encoding  encoding;           /* Encoding (alaw, ulaw, linear) */
} audio_format;

/* Parameter type */
typedef enum {
	ParaZero,
	ParaSimWavPath,		// ģ��as��������Ƶ�ļ���·��
	ParaSimWavMode,		// ģ��as��������Ƶ�ļ��Ķ�ȡ��ʽ�����\˳��
	ParaSimChnlNum,		// ģ��as��channel��
	ParaSimSampleSize,	// ģ��as����Ƶ�ļ���ÿ�ζ���������KB��

} as_para;


class as_channel_i
	: public sr::if_base
{
public:
	// ��ʾ�����Ž���֪ͨ;
	virtual	int		on_playback_done(void) = 0;

	// ¼�����ݵ���������ǰ������ʵ�ʶ�ȡ�����ݳ���;
	virtual int		on_sample_recorded(const void *buf, int len) = 0;

	// DTMF�¼�����;
	virtual	int		on_dtmf_event(const char *dtmfstr) = 0;

	// Զ�˹һ�֪ͨ;
	virtual	int		on_remote_hangup(void) = 0;

};

class as_mngr_i
	: public sr::if_base
{
public:
	// �绰���룬������գ�����һ��channel���󷵻ظ�as;
	virtual	int		on_incoming_call(as_channel_i **channel) = 0;

	// �绰��������������ɹ�����������һ��channel���󷵻ظ�as;
	virtual	int		on_outgoing_call_completed(int code, as_channel_i **channel) = 0;
};

class audsrc_i
	: public sr::if_base
{
public:
	virtual	int		init(void) = 0;
	virtual	int		fini(void) = 0;

	// ��ѯ��asģ������ԣ�����֧��֧�ֵ绰����
	virtual int		feature(int type) const = 0;

	// ����AudioSourceģ���ȫ�ֲ���
	virtual int		set_param(int type, int value, const void *para1) = 0;
	virtual int		get_param(int type, int &value, void *para1) = 0;

	// ����/ֹͣAudioSource������
	virtual	int		start(as_mngr_i *mngr) = 0;
	virtual	int		stop(void) = 0;

	// ��ͨ����صĲ���
	virtual int		set_param(as_channel_i *channel, int type, int value, const void *para1) = 0;	// ����ͨ������
	virtual int		get_param(as_channel_i *channel, int type, int &value, void *para1) = 0;		// ��ȡͨ������
	virtual int		start_record(as_channel_i *channel) = 0;						// ͨ����ʼ¼��
	virtual int		stop_record(as_channel_i *channel) = 0;							// ͨ��ֹͣ¼��
	virtual int		playback(as_channel_i *channel, const void *buf, int len) = 0;	// ͨ��������ʾ��
	virtual int		kill_playback(as_channel_i *channel) = 0;						// ͨ��ֹͣ��ʾ������
	virtual int		hangup(as_channel_i *channel) = 0;								// ͨ���һ�ָ��
	// ��������绰���У��첽����on_outgoing_call_completed�������
	virtual int		initial_outgoing_call(const char *phone_no) = 0;
};


_END_NAMESPACE	// as

DEFINE_SHPTR(as::audsrc_i, "audsrc");
DEFINE_SHMOD(audsrc);

//////////////////////////////////////////////////////////////////////////


#endif /* __ISR_AP_H__ */
