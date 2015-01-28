/**
 * @file    iFly_MRCP.h
 * @brief   iFLY MRCP SDK header file
 * 
 * This file contains the MRCP application programming interface(API) declarations 
 * of iFLYTEK. TTS developer can include this file in your project to build applications.
 * For more information, please read the developer guide.
 * Copyright (C)    2004 - 2006 by ANHUI USTC IFLYTEK. Co.,LTD.
 *                  All rights reserved.
 * 
 * @author  Speech Dept. iFLYTEK.
 * @version 1.0
 * @date    2006/08/09
 * 
 * @see        
 * 
 * Histroy:
 * index    version        date            author        notes
 * 0        1.0            2006/08/09      hjma          Create this file
 * 1		2.0			   2009/03/16	   hlli			 Add contents of Verifier and Recorder
 */

#ifndef IFLY_MRCP_H
#define IFLY_MRCP_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)
# ifdef IFLYIMS_EXPORTS
#  define IMSLIBAPI __declspec(dllexport)
# else
#  define IMSLIBAPI __declspec(dllimport)
# endif
#else /* defined(WIN32) */
# define IMSLIBAPI
#endif /* defined(WIN32) */

typedef void*			HIMSINSTANCE;
typedef const char*		IMSCSTR;	

typedef int				IMSBOOL;

const int IMSTRUE		=	1;
const int IMSFALSE		=	0;

const int IMS_IPADDR_MAX_SIZE				=	32;

const int IMS_CONTENT_TYPE_MAX_SIZE			=	128;

const int IMS_CONTENT_ID_MAX_SIZE			=	32;

const int IMS_CONTENT_BASE_MAX_SIZE			=	32;

const int IMS_CONTENT_LOCATION_MAX_SIZE		=	256;

const int IMS_CONTENT_ENCODE_MAX_SIZE		=	128;

const int IMS_PAYLOAD_MAX_SIZE				=   1024;

/* MRCP Protocol Version */
enum
{
	MRCP_V1		=		1,
	MRCP_V2		=		2,
};

/* MRCP Media Resource Types */
const char	IMSResourceRecog[]				=	"speechrecog";
const char	IMSResourceSynth[]				=	"speechsynth";
const char	IMSResourceRecorder[]			=	"recorder";
const char	IMSResourceVerify[]				=	"speakverify";

/* MRCP Event Types returned by server */
/* Audio Event */
const char	IMSEventGetAudio[]				=	"GET_AUDIO";
const char	IMSEventNoEvent[]				=	"NO_EVENT";

/* Common event shared by Recognizer, Recorder and Verifier */
const char	IMSEventStartOfInput[]			=	"START_OF_INPUT";

/* Synthesizer Event */
const char	IMSEventSpeakComplete[]			=	"SYNTH_SPEAK_COMPLETE";
const char	IMSEventSpeechMarker[]			=	"SYNTH_SPEECH_MARKER";

/* Recognizer Event */
const char	IMSEventRecogComplete[]			=	"RECOG_RECOGNITION_COMPLETE";
const char	IMSEventInterpretComplete[]		=	"RECOG_INTERPRETATION_COMPLETE";

/* Recorder Event */
const char	IMSEventRecordComplete[]		=	"RECOR_RECORD_COMPLETE";

/* Verifier Event */
const char	IMSEventVerificationComplete[]	=	"VERIF_VERIFICATION_COMPLETE";


// Generic Mrcp Header
const char	ParamChannelIdentifier[]		=	"channel-identifier";			// 1*HEXDIG "@" 1*VCHAR，Mrcp channel id
const char	ParamAccept[]					=	"accept";						// #( ("*/*" | (type "/" "*")) *(";" parameter) ), Accepted media type for the response
const char	ParamActiveRequestIdList[]		=	"active-request-id-list";		// "request-id *(,request-id)CRLF", the list of request-ids that apply to/be affected
const char	ParamProxySyncId[]				=	"proxy-sync-id";				// "1*VCHAR", for the communicate between media resources
const char	ParamAcceptCharset[]			=	"accept-charset";				// 1#( ( charset | "*" )[ ";" "q" "=" qvalue ] ),Accepted character sets for the response
const char	ParamContentId[]				=	"content-id";					// "1*VCHAR", an ID or name for the content
const char	ParamContentType[]				=	"content-type";					// media-type, media type of content
const char	ParamContentLength[]			=	"content-length";				// 1*DIGIT, length of content
const char	ParamContentBase[]				=	"content-base";					// absoluteURI, base URI for the relative URLs
const char	ParamContentLocation[]			=	"content-location";				// absoluteURI / relativeURI, the resource location for the entity, used to optimize operations
const char	ParamContentEncoding[]			=	"content-encoding";				// *WSP content-coding *(*WSP "," *WSP content-coding *WSP ), as a modifier to the media-type
const char	ParamFetchTimeout[]				=	"fetch-timeout";				// 1*DIGIT, timeout for fetching URI, milliseconds
const char	ParamCacheControl[]				=	"cache-control";				// 1#(max-age | max-stale | min-fresh "="delta-seconds), cache control info, seconds
const char	ParamLoggingTag[]				=	"logging-tag";					// 1*VCHAR, tag for log of this client's request
const char	ParamSetCookie[]				=	"set-cookie";					// cookie sets, cookie info for http request
const char	ParamSetCookie2[]				=	"set-cookie2";					// cookie sets, cookie info for http request
const char	ParamVendorSpecific[]			=	"vendor-specific-parameters";	// *(vendor-av-pair-name "=" value ";"), vendor specific params

const char	ParamCompletionCause[]			=	"completion-cause";				// 1*DIGIT SP 1*VCHAR, Cause of Request completion, diffrent between resources
const char	ParamCompletionReason[]			=	"completion-reason";			// quoted-string, Reason of Request completion, diffrent between resources
const char	ParamSpeechLanguage[]			=	"speech-language";				// 1*VCHAR, default language of the speech data 
const char	ParamFailedUri[]				=	"failed-uri";					// uri, the uri failed to access
const char	ParamFailedUriCause[]			=	"failed-uri-cause";				// 1*alphanum, the cause of accessing uri failed

const char	ParamStartInputTimers[]			=	"start-input-timers";			// boolean-value, start the no-input timer or not
const char	ParamWaveformUri[]				=	"waveform-uri";					// "<" Uri ">"";" "size" "=" 1*DIGIT";" "duration" "=" 1*DIGIT, URI for the recorded incoming audio
const char	ParamNoInputTimeout[]			=	"no-input-timeout";				// "speech" / "dtmf", specify the input that caused the barge-in is DTMF or speech
const char	ParamSaveWaveform[]				=	"save-waveform";
const char	ParamMediaType[]				=	"media-type";					// media-type-value, the MIME content type to store captured audio 
const char	ParamVerBufferUtterance[]		=	"ver-buffer-utterance";
const char	ParamNewAudioChannel[]			=	"new-audio-channel";
const char	ParamSensitivityLevel[]			=	"sensitivity-level";			// FLOAT, 0.0-1.0, variable level of sound sensitivity

// Synthesizer Header
const char	ParamJumpSize[]					=	"jump-size";					// 1*VCHAR SP "Tag" | (("+" / "-") 1*DIGIT SP "Second"|"Word"|"Sentence"|"Paragraph"), size to jump while speak
const char	ParamKillOnBargeIn[]			=	"kill-on-barge-in";				// boolean-value, enable kill on barge in or not
const char	ParamSpeakerProfile[]			=	"speaker-profile";				// uri, profile of speaker like voice parameters
const char	ParamVoiceGender[]				=	"voice-gender";					// Voice param
const char	ParamVoiceAge[]					=	"voice-age";					// Voice param
const char	ParamVoiceVariant[]				=	"voice-variant";				// Voice param
const char	ParamVoiceName[]				=	"voice-name";					// Voice param
const char	ParamProsodyPitch[]				=	"prosody-pitch";				// Prosody param
const char	ParamProsodyContour[]			=	"prosody-contour";				// Prosody param
const char	ParamProsodyRange[]				=	"prosody-range";				// Prosody param
const char	ParamProsodyRate[]				=	"prosody-rate";					// Prosody param
const char	ParamProsodyDuration[]			=	"prosody-duration";				// Prosody param
const char	ParamProsodyVolume[]			=	"prosody-volume";				// Prosody param
const char	ParamSpeechMarker[]				=	"speech-marker";				// 1*VCHAR[";" timestamp ], marker tag match the markup context
const char	ParamFetchHint[]				=	"fetch-hint";					// prefetch | safe, when to fetch document, time received or be referenced
const char	ParamAudioFetchHint[]			=	"audio-fetch-hint";				// safe | prefetch | stream, when to fetch audio document
const char	ParamSpeakRestart[]				=	"speak-restart";				// boolean-value, note if speak is restarted
const char	ParamSpeakLength[]				=	"speak-length";					// 1*VCHAR SP "Tag" | (("+" / "-") 1*DIGIT SP "Second"|"Word"|"Sentence"|"Paragraph"),length to speak
const char	ParamLoadLexicon[]				=	"load-lexicon";					// boolean-value, if load lexicon or not
const char	ParamLexiconSearchOrder[]		=	"lexicon-search-order";			// uri-list, search order of lexicon list
const char	ParamInputWaveformUri[]			=	"input-waveform-uri";			// Uri, audio content to be processed by the RECOGNIZE operation
const char	ParamSpeechCompleteTimeout[]	=	"speech-complete-timeout";		// 1*DIGIT, length of silence following speech before finalizes a complete result,milliseconds

// Recognizer Header
const char	ParamConfidenceThreshold[]		=	"confidence-threshold";			// FLOAT, 0.0-1.0, confidence level with the match
const char	ParamSpeedVsAccuracy[]			=	"speed-vs-accuracy";			// FLOAT, 0.0-1.0, tunable between Performance and Accuracy, 0.0 fastest
const char	ParamNBestListLength[]			=	"n-best-list-length";			// 1*DIGIT, the best match count above the confidence threshold
const char	ParamInputType[]				=	"input-type";					// 1*DIGIT,  no speech detected timeout from recognition start,milliseconds
const char	ParamRecognitionTimeout[]		=	"recognition-timeout";			// 1*DIGIT, no match timeout from recognition start,milliseconds
const char	ParamRecognizerContextBlock[]	=	"recognizer-context-block";		// 1*VCHAR,  recognizer context block
const char	ParamSpeechIncompleteTimeout[]	=	"speech-incomplete-timeout";	// 1*DIGIT,  length of silence following speech before finalizes a incomplete result,milliseconds
const char	ParamDtmfInterdigitTimeout[]	=	"dtmf-interdigit-timeout";
const char	ParamDtmfTermTimeout[]			=	"dtmf-term-timeout";
const char	ParamDtmfTermChar[]				=	"dtmf-term-char";
const char	ParamRecognitionMode[]			=	"recognition-mode";
const char	ParamCancelIfQueue[]			=	"cancel-if-queue";
const char	ParamHotwordMaxDuration[]		=	"hotword-max-duration";
const char	ParamHotwordMinDuration[]		=	"hotword-min-duration";
const char	ParamInterpretText[]			=	"interpret-text";
const char	ParamDtmfBufferTime[]			=	"dtmf-buffer-time";
const char	ParamClearDtmfBuffer[]			=	"clear-dtmf-buffer";
const char	ParamEarlyNoMatch[]				=	"early-no-match";

const char	ParamNumMinConsistentPronunciations[]	=	"num-min-consistent-pronunciations";
const char	ParamConsistencyThreshold[]		=	"consistency-threshold";
const char	ParamClashThreshold[]			=	"clash-threshold";
const char	ParamEnrollUtterance[]			=	"enroll-utterance";
const char	ParamConfusablePhrasesUri[]		=	"confusable-phrases-uri";
const char	ParamPersonalGrammarUri[]		=	"personal-grammar-uri";
const char	ParamPhraseId[]					=	"phrase-id";
const char	ParamPhraseNl[]					=	"phrase-nl";
const char	ParamWeight[]					=	"weight";
const char	ParamSaveBestWaveform[]			=	"save-best-waveform";
const char	ParamNewPhraseId[]				=	"new-phrase-id";
const char	ParamAbortPhraseEnrollment[]	=	"abort-phrase-enrollment";

// Recorder Header
const char	ParamRecordUri[]				=	"record-uri";
const char	ParamMaxTime[]					=	"max-time";
const char	ParamTrimLength[]				=	"trim-length";
const char	ParamFinalSilence[]				=	"final-silence";
const char	ParamCaptureOnSpeech[]			=	"capture-on-speech";

// Verifier Header
const char	ParamRepositoryUri[]			=	"repository-uri";
const char	ParamVoiceprintIdentifier[]		=	"voiceprint-identifier";
const char	ParamVerificationMode[]			=	"verification-mode";
const char	ParamAdaptModel[]				=	"adapt-model";
const char	ParamAbortModel[]				=	"abort-model";
const char	ParamMinVerificationScore[]		=	"min-verification-score";
const char	ParamNumMinVerificationPhrases[]=	"num-min-verification-phrases";
const char	ParamNumMaxVerificationPhrases[]=	"num-max-verification-phrases";
const char	ParamVoiceprintExists[]			=	"voiceprint-exists";
const char	ParamAbortVerification[]		=	"abort-verification";


// vendor specified parameter
const char g_vsp_tts_vid[]                    = "com.iflytek.tts.vid";                // 音库ID
const char g_vsp_tts_speed[]                  = "com.iflytek.tts.speed";              // 语速, [-500~500]
const char g_vsp_tts_volume[]                 = "com.iflytek.tts.volume";             // 音量, [-20~20]
const char g_vsp_tts_entertreat[]             = "com.iflytek.tts.enter-treat";        // 回车符是否作为分句标志
const char g_vsp_tts_readallmarks[]           = "com.iflytek.tts.read-all-marks";     // read all marks
const char g_vsp_tts_readnumber[]             = "com.iflytek.tts.read-number";        // how to read number
const char g_vsp_tts_readenglish[]            = "com.iflytek.tts.read-english";       // how to read English
const char g_vsp_tts_vpttreat[]               = "com.iflytek.tts.vpt-treat";          // com.iflytek.tts.vpt-treat = 1
const char g_vsp_tts_bgsound[]                = "com.iflytek.tts.bg-sound";           // com.iflytek.tts.bg-sound = 2

const char g_vsp_asr_ep_timeout[]             = "com.iflytek.asr.ep.timeout";
const char g_vsp_asr_ep_incompletetimeout[]   = "com.iflytek.asr.ep.incompletetimeout";
const char g_vsp_asr_ep_maxspeechtimeout[]    = "com.iflytek.asr.ep.maxspeechtimeout";
const char g_vsp_asr_ep_bargein[]             = "com.iflytek.asr.ep.bargein";
const char g_vsp_asr_ep_tenantname[]          = "com.iflytek.asr.ep.tenant-name";
const char g_vsp_asr_ep_companyname[]         = "com.iflytek.asr.ep.company-name";
const char g_vsp_asr_ep_licenseport[]         = "com.iflytek.asr.ep.license-port";
const char g_vsp_asr_ep_licenseportoverdraftthresh[] = "com.iflytek.asr.ep.license-port-overdraft-thresh";
const char g_vsp_asr_ep_licensingfeatures[]   = "com.iflytek.asr.ep.licensing-features";
const char g_vsp_asr_ep_inpromptsensitivitypercent[] = "com.iflytek.asr.ep.in-prompt-sensitivity-percent";
const char g_vsp_asr_ep_bosbackoff[]          = "com.iflytek.asr.ep.bos-backoff";
const char g_vsp_asr_ep_eosbackoff[]          = "com.iflytek.asr.ep.eos-backoff";
const char g_vsp_asr_ep_speechsnr[]           = "com.iflytek.asr.ep.speech-snr";
const char g_vsp_asr_ep_audioenvironment[]    = "com.iflytek.asr.ep.audio-environment";
const char g_vsp_asr_ep_minbytestoprocess[]   = "com.iflytek.asr.ep.min-bytes-to-process";
const char g_vsp_asr_ep_waveformlogging[]     = "com.iflytek.asr.ep.waveform-logging";
const char g_vsp_asr_ep_useisrepread[]        = "com.iflytek.asr.ep.use-isrep-read";
const char g_vsp_asr_ep_version[]             = "com.iflytek.asr.ep.version";
const char g_vsp_asr_ep_minspeechlength[]     = "com.iflytek.asr.ep.min-speech-length";
const char g_vsp_asr_ep_gmmwinsize[]          = "com.iflytek.asr.ep.gmm-win-size";
const char g_vsp_asr_ep_suppressbargeintime[] = "com.iflytek.asr.ep.suppress-barge-in-time";
const char g_vsp_asr_ep_featuredimension[]    = "com.iflytek.asr.ep.feature-dimension";

const char g_vsp_asr_rec_timeout[]            = "com.iflytek.asr.rec.timeout";
const char g_vsp_asr_rec_incompletetimeout[]  = "com.iflytek.asr.rec.incompletetimeout";
const char g_vsp_asr_rec_maxspeechtimeout[]   = "com.iflytek.asr.rec.maxspeechtimeout";
const char g_vsp_asr_rec_bargein[]            = "com.iflytek.asr.rec.bargein";
const char g_vsp_asr_rec_tenantname[]         = "com.iflytek.asr.rec.tenant-name";
const char g_vsp_asr_rec_companyname[]        = "com.iflytek.asr.rec.company-name";
const char g_vsp_asr_rec_histthreshold[]      = "com.iflytek.asr.rec.hist-threshold";
const char g_vsp_asr_rec_histenable[]         = "com.iflytek.asr.rec.hist-enable";
const char g_vsp_asr_rec_beamthreshold[]      = "com.iflytek.asr.rec.beam-threshold";
const char g_vsp_asr_rec_beamenable[]         = "com.iflytek.asr.rec.beam-enable";
const char g_vsp_asr_rec_logenable[]          = "com.iflytek.asr.rec.log-enable";
const char g_vsp_asr_rec_gsenable[]           = "com.iflytek.asr.rec.gs-enable";
const char g_vsp_asr_rec_cmenable[]           = "com.iflytek.asr.rec.cm-enable";
const char g_vsp_asr_rec_wavefilename[]       = "com.iflytek.asr.rec.wave-filename";  // wave file name for this channel
const char g_vsp_asr_rec_insertpenalty[]      = "com.iflytek.asr.rec.insert-penalty";
const char g_vsp_asr_rec_trelliswidth[]       = "com.iflytek.asr.rec.trellis-width";
const char g_vsp_asr_rec_maxtokenframe[]      = "com.iflytek.asr.rec.max-token-frame";
const char g_vsp_asr_rec_incompleteconfidencethreshold[] = "com.iflytek.asr.rec.incomplete-confidence-threshold";
const char g_vsp_asr_rec_returnwaveform[]     = "com.iflytek.asr.rec.return-waveform";
const char g_vsp_asr_rec_maxpresemcount[]     = "com.iflytek.asr.rec.max-presem-count";
const char g_vsp_asr_rec_elapselogenable[]    = "com.iflytek.asr.rec.elapse-log-enable";
const char g_vsp_asr_rec_outputimcompleteresult[] = "com.iflytek.asr.rec.output-imcomplete-result";
const char g_vsp_asr_rec_maxspeechlength[]    = "com.iflytek.asr.rec.max-speech-length";
const char g_vsp_asr_rec_resultencoding[]     = "com.iflytek.asr.rec.result-encoding";
const char g_vsp_asr_rec_computefrm[]         = "com.iflytek.asr.rec.compute-frm";
const char g_vsp_asr_rec_dynamicbeam[]        = "com.iflytek.asr.rec.dynamic-beam";
const char g_vsp_asr_rec_resultdtw[]          = "com.iflytek.asr.rec.result-dtw";

const char g_vsp_isv_target_fa[]			  = "com.iflytek.isv.target-fa";
const char g_vsp_isv_mdoel_id_list[]          = "com.iflytek.isv.mdoel-id-list";

static const char * payload_fmt [] = 
{
	"no-define\r\n",		// 0 TTS_ADF_DEFAULT
	"102 L8/8000\r\n",		// 1 TTS_ADF_PCM8K8B1C
	"103 L8/16000\r\n",		// 2 TTS_ADF_PCM16K8B1C
	"104 L16/8000\r\n",     // 3 TTS_ADF_PCM8K16B1C
	"105 L16/16000\r\n",    // 4 TTS_ADF_PCM16K16B1C
	"106 L8/11000\r\n",		// 5 TTS_ADF_PCM11K8B1C
	"107 L16/11000\r\n",	// 6 TTS_ADF_PCM11K16B1C
	"108 L8/6000\r\n",		// 7 TTS_ADF_PCM6K8B1C
	"109 L16/6000\r\n",		// 8 TTS_ADF_PCM6K16B1C
	"110 PCMA/16000\r\n",   // 9 TTS_ADF_ALAW16K1C
	"111 PCMU/16000\r\n",	// 10 TTS_ADF_ULAW16K1C
	"8 PCMA/8000\r\n",		// 11 TTS_ADF_ALAW8K1C
	"0 PCMU/8000\r\n",      // 12 TTS_ADF_ULAW8K1C
	"112 PCMA/11000\r\n",   // 13 TTS_ADF_ALAW11K1C
	"113 PCMU/11000\r\n",	// 14 TTS_ADF_ULAW11K1C 
	"114 PCMA/6000\r\n",	// 15 TTS_ADF_ALAW6K1C  
	"115 PCMU/6000\r\n",	// 16 TTS_ADF_ULAW6K1C 
	"116 G721/8000\r\n",    // 17 TTS_ADF_ADPCMG7218K4B1C
	"117 G721/6000\r\n",    // 18 TTS_ADF_ADPCMG7216K4B1C
	"4 G723/8000\r\n",      // 19 TTS_ADF_ADPCMG7233B1C  
	"118 G732/8000\r\n",    // 20 TTS_ADF_ADPCMG7235B1C
	"119 VOX/6000\r\n",		// 21 TTS_ADF_VOX6K1C   
	"120 VOX/8000\r\n",		// 22 TTS_ADF_VOX8K1C   
	"121 AMR/6000\r\n",		// 23 TTS_ADF_AMR6K1C   
	"122 AMR/8000\r\n",		// 24 TTS_ADF_AMR8K1C   
	"123 AMR/11000\r\n",	// 25 TTS_ADF_AMR11K1C  
	"124 AMR/16000\r\n",	// 26 TTS_ADF_AMR16K1C  
	"35 MP3/36000\r\n",		// 27 TTS_ADF_MP36K1C 
	"36 MP3/38000\r\n",		// 28 TTS_ADF_MP38K1C 	
	"37 MP3/11000\r\n",		// 29 TTS_ADF_MP311K1C
	"38 MP3/16000\r\n"		// 30 TTS_ADF_MP316K1C
};

/* Mrcp Resource information */
typedef struct _IMSResourceInfo
{
	int		cbSize;								    /* [in] size of this struct */

	IMSCSTR	szResType;							    /* [in] Resource Type to use */
	IMSCSTR	szResName;						     	/* [in] Resource name, used only in mrcv1! */
	IMSCSTR	szOfferPayloads;					    /* [in] Description of Client's payload types */
	int		dwOfferTransMode;					    /* [in] Mode of Client RTP channel, 0 = sendrecv, 1 = sendonly, 2 = recvonly */
	int     RtpId;                                  /* [in] identify rtp info. */
	char	szAnswerPayloads[IMS_PAYLOAD_MAX_SIZE];	/* [out] Description of Server's payload types selection, malloced by Lib */
	int		dwAnswerTransMode;					    /* [out] Mode of Server RTP channel, 0 = sendrecv, 1 = sendonly, 2 = recvonly */
	int		dwReserved;							    /* [in, out] Reserved Section */

	char	szClientAddr[IMS_IPADDR_MAX_SIZE];	    /* [in] Local IP Address for SIP/MRCP/RTP, "ip" */
	char	szAudioAddr[IMS_IPADDR_MAX_SIZE];		/* [in] RTP Client IP Address, "port/ip:port", Lib will create RTP channel when this is filled with only "port"  */
	char	szServerAddr[IMS_IPADDR_MAX_SIZE];		/* [out] Server IP Address for SIP/MRCP/RTP, "ip" */
} IMSResourceInfo, *PIMSResourceInfo;

/* Mrcp content related information */
typedef struct _IMSContent
{
	int		cbSize;

	char	szType[IMS_CONTENT_TYPE_MAX_SIZE];			/* [in, out] Content-Type(plain/ssml etc.) */
	char	szId[IMS_CONTENT_ID_MAX_SIZE];				/* [in, out] Content-Id */
	char	szBase[IMS_CONTENT_BASE_MAX_SIZE];			/* [in, out] Content-Base */
	char	szLocation[IMS_CONTENT_LOCATION_MAX_SIZE];	/* [in, out] Content-Location */
	char	szEncoding[IMS_CONTENT_ENCODE_MAX_SIZE];	/* [in, out] Content-Encoding */
	void*	pContent;									/* [in, out] Content Buffer, malloc by in:user/out:Lib */
	int		dwContentSize;								/* [in, out] Content-Length */
	int		dwReserved;
} IMSContent, *PIMSContent;

typedef int (*IMSPROCESSCB)(HIMSINSTANCE hSession, const char* szEventType, int nRequestId, char* szProcessParams, 
							PIMSContent pContent, int dwErrorCode, const char* nParam, void* pUSerData);

typedef int (*IMSEVENTCB)(HIMSINSTANCE hSession, const char* szEventType, int nRequestId,
						  char* szEventParams, const char* nParam, void* pUserData);

/** 
* @brief   IMSInitialize
* 
* Global initialize Mrcp System, it is the first function to be called
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   int nVersion    - [in] The version of MRCP Module to be initialized
* @param   void* pReserved - [in, out] The Reserved section
* @see
*/
int IMSLIBAPI IMSInitialize(int nVersion, void* pReserved);
typedef int (*Proc_IMSInitialize)(int nVersion, void* pReserved);

/** 
* @brief   IMSUninitialize
* 
* Global uninitialize Mrcp System, it is the last function to be called
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param
* @see
*/
int IMSLIBAPI IMSUninitialize();
typedef int (*Proc_IMSUninitialize)();

/** 
* @brief   IMSCreateSession
* 
* Create a mrcp hSession for a speech call
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE* phSession        - [out] Mrcp Session's Instance handle
* @param   char* szServerAddr             - [in, out] in: the server addr to connect, out: the actual server addr
* @param   PIMSResourceInfo pResInfo      - [in, out] The pointer of Resource Information struct, set NULL for no default resource
* @param   const char* szVendorAttributes - [in] some Vendor info to set, can be used for load balance etc.
* @see
*/
int IMSLIBAPI IMSCreateSession(HIMSINSTANCE* phSession,
							   char* szServerAddr,
							   PIMSResourceInfo pResInfo,
							   IMSCSTR szVendorAttributes);
typedef int (*Proc_IMSCreateSession)(HIMSINSTANCE* phSession,
									 char* szServerAddr,
									 PIMSResourceInfo pResInfo,
									 IMSCSTR szVendorAttributes);

/** 
* @brief   IMSDeleteSession
* 
* Delete a mrcp hSession for a speech call
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession - [in] Mrcp Session Instance
* @see
*/
int IMSLIBAPI IMSDeleteSession(HIMSINSTANCE hSession);
typedef int (*Proc_IMSDeleteSession)(HIMSINSTANCE hSession);

/** 
* @brief   IMSQueryCapability
* 
* Query the Capability of Mrcp Session
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession   - [in] Mrcp Session Instance
* @param   IMSCSTR* pszPayLoads    - [out] Capability of Mrcp Session, malloced by Lib
* @param   IMSCSTR* pszResTypes    - [out] Resources supported by this Mrcp Session, malloced by Lib
* @see
*/
int IMSLIBAPI IMSQueryCapability(HIMSINSTANCE hSession, IMSCSTR* pszPayLoads, IMSCSTR* pszResTypes);
typedef int (*Proc_IMSQueryCapability)(HIMSINSTANCE hSession, IMSCSTR* pszPayLoads, IMSCSTR* pszResTypes);

/** 
* @brief   IMSAddResource
* 
* Add a Speech Resource to a Mrcp Session
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession     - [in] Mrcp Session Instance
* @param   IMSResourceInfo* pResInfo - [in, out] MRCP Resource Information
* @see
*/
int IMSLIBAPI IMSAddResource(HIMSINSTANCE hSession, IMSResourceInfo* pResInfo);
typedef int (*Proc_IMSAddResource)(HIMSINSTANCE hSession, IMSResourceInfo* pResInfo);

/** 
* @brief   IMSRemoveResource
* 
* Remove a Speech Resource from a Mrcp Session
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE resource - [in] Mrcp Resource Instance
* @param   IMSCSTR szResType     - [in] Mrcp Resource Type
* @see
*/
int IMSLIBAPI IMSRemoveResource(HIMSINSTANCE hSession, IMSCSTR szResType);
typedef int (*Proc_IMSRemoveResource)(HIMSINSTANCE hSession, IMSCSTR szResType);

/** 
* @brief   IMSWaitForEvent
* 
* Wait for Event from Mrcp Server,Announce some complete or error events
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @param   IMSCSTR szResType      - [in] Mrcp Resource Type
* @param   IMSCSTR* pEventType    - [out] Mrcp Event name
* @param   IMSCSTR* szEventParams - [out] Mrcp Event params, malloced by Lib
* @param   PIMSContent pEventData - [out] Mrcp Event Content array, malloced by Lib
* @param   int* pnDataCount       - [out] Mrcp Event Content count
* @param   int timeout            - [in] Timeout of Waiting Event, ms
* @see
*/
int IMSLIBAPI IMSWaitForEvent(HIMSINSTANCE hSession,
							  IMSCSTR szResType,
							  IMSCSTR* pEventType,
							  IMSCSTR* szEventParams,
							  PIMSContent pEventData,
							  int* pnDataCount,
							  int nTimeout);
typedef int (*Proc_IMSWaitForEvent)(HIMSINSTANCE hSession,
									IMSCSTR szResType,
									IMSCSTR* pEventType,
									IMSCSTR* szEventParams,
									PIMSContent pEventData,
									int* pnDataCount,
									int nTimeout);

/** 
* @brief   IMSGetAudio
* 
* Get received audio of Mrcp Media Resource
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession - [in] Mrcp Session Instance
* @param   IMSCSTR szResType     - [in] Mrcp Resource Type
* @param   IMSCSTR* szBuffer     - [out] buffer for audio data,malloced by Lib
* @param   int* pnLength         - [out] length of audio data
* @param   int* pnTimestamp      - [out] timestamp info of audio data
* @see
*/
int IMSLIBAPI IMSGetAudio(HIMSINSTANCE hSession,
						  IMSCSTR szResType,
						  IMSCSTR* szBuffer,
						  int* pnLength,
						  int* pnTimestamp);
typedef int (*Proc_IMSGetAudio)(HIMSINSTANCE hSession,
								IMSCSTR szResType,
								IMSCSTR* szBuffer,
								int* pnLength,
								int* pnTimestamp);

/** 
* @brief   IMSGetAudioEx
* 
* Get received audio of Mrcp Media Resource
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession - [in] Mrcp Session Instance
* @param   IMSCSTR szResType     - [in] Mrcp Resource Type
* @param   IMSCSTR* szBuffer     - [out] buffer for audio data,malloced by Lib
* @param   int* pnLength         - [out] length of audio data
* @param   int* pnTimestamp      - [out] timestamp info of audio data
* @param   int* flag             - [in/out] flags of audio
* @see
*/
int IMSLIBAPI IMSGetAudioEx(HIMSINSTANCE hSession,
							IMSCSTR szResType,
							IMSCSTR* szBuffer,
							int* pnLength,
							int* pnTimestamp,
							int* flag);
typedef int (*Proc_IMSGetAudioEx)(HIMSINSTANCE hSession,
								  IMSCSTR szResType,
								  IMSCSTR* szBuffer,
								  int* pnLength,
								  int* pnTimestamp,
								  int* flag);

/** 
* @brief   IMSPutAudio
* 
* Put audio to the Mrcp Media Resource
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession - [in] Mrcp Session Instance
* @param   IMSCSTR szResType     - [in] Mrcp Resource Type
* @param   IMSCSTR szBuffer      - [in] buffer for audio data to send
* @param   int nLength           - [in] length of audio data
* @param   int nTimestamp        - [in] timestamp info of audio data
* @see
*/
int IMSLIBAPI IMSPutAudio(HIMSINSTANCE hSession,
						  IMSCSTR szResType,
						  IMSCSTR szBuffer,
						  int nLength,
						  int nTimestamp);
typedef int (*Proc_IMSPutAudio)(HIMSINSTANCE hSession,
								IMSCSTR szResType,
								IMSCSTR szBuffer,
								int nLength,
								int nTimestamp);

/** 
* @brief   IMSUtilAddParam
* 
* Util function, Add Param to the ParamList String
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   char* szParamList     - [in, out] Storaged params
* @param   int nListSize         - [in] maxsize of this list
* @param   IMSCSTR ParamName     - [in] param name to add
* @param   IMSCSTR szParamValue  - [in] param value to add
* @see
*/
int IMSLIBAPI IMSUtilAddParam(char* szParamList,
							  int nListSize,
							  IMSCSTR ParamName,
							  IMSCSTR szParamValue);
typedef int (*Proc_IMSUtilAddParam)(char* szParamList,
									int nListSize,
									IMSCSTR ParamName,
									IMSCSTR szParamValue);

/** 
* @brief   IMSUtilRemoveParam
* 
* Util function, Remove Param from the ParamList String
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   char* szParamList - [in, out] Storaged params
* @param   IMSCSTR ParamName - [in] param name to remove
* @see
*/
int IMSLIBAPI IMSUtilRemoveParam(char* szParamList, IMSCSTR ParName);
typedef int (*Proc_IMSUtilRemoveParam)(char* szParamList, IMSCSTR ParName);

/** 
* @brief   IMSUtilParamCount
* 
* Util function, Get Params count of the ParamList String
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   IMSCSTR szParamList - [in] Storaged params
* @param   int* pnCount        - [out] params count
* @see
*/
int IMSLIBAPI IMSUtilParamCount(IMSCSTR szParamList, int* pnCount);
typedef int (*Proc_IMSUtilParamCount)(IMSCSTR szParamList, int* pnCount);

/** 
* @brief   IMSUtilGetParam
* 
* Util function, Get Param of the ParamList String
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   IMSCSTR szParamList   - [in] Storaged params
* @param   int nPos              - [in] get the nPosth param,set -1 when get by name
* @param   char* pParamName		 - [in, out] in: when nPos=-1,the name of param to get; out: get by nPos, the name of nPos param, malloced by User
* @param   size_t nNameSize		 - [in] when nPos=-1, it should be the buffer size of param name, or it should be the length of parameter name 
* @param   char* szParamValue	 - [out] the value of this parameter, malloced by User
* @param   size_t nValueSize	 - [in] buffer size of parameter value 
* @see
*/
int IMSLIBAPI IMSUtilGetParam ( IMSCSTR szParamList, int nPos, 
							    char* pParamName, size_t nNameSize, 
							    char* szParamValue, size_t nValueSize);
typedef int (*Proc_IMSUtilGetParam)( IMSCSTR szParamList, int nPos, 
							         char* pParamName, size_t nNameSize, 
							         char* szParamValue, size_t nValueSize);
/** 
* @brief   IMSSetParams
* 
* Set Params of Mrcp Server on this hSession
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession      - [in] Mrcp Session Instance
* @param   IMSCSTR szResType          - [in] Mrcp Resource Type
* @param   IMSCSTR szParamList        - [in] Param List to set
* @param   const PIMSContent pContent - [in] Content of param
* @see
*/
int IMSLIBAPI IMSSetParams(HIMSINSTANCE hSession,
						   IMSCSTR szResType,
						   IMSCSTR szParamList,
						   const PIMSContent pContent);
typedef int (*Proc_IMSSetParams)(HIMSINSTANCE hSession,
								 IMSCSTR szResType,
								 IMSCSTR szParamList,
								 const PIMSContent pContent);

/** 
* @brief   IMSGetParams
* 
* Get Params of Mrcp Server on this hSession
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession	- [in] Mrcp Session Instance
* @param   IMSCSTR szResType		- [in] Mrcp Resource Type
* @param   IMSCSTR szParamNameList  - [in] List of parameter name
* @param   IMSCSTR* szParamList		- [out] Param List, malloced by Lib
* @param   PIMSContent pContent		- [out] Content of param, malloced by Lib
* @see
*/
int IMSLIBAPI IMSGetParams(HIMSINSTANCE hSession,
						   IMSCSTR szResType,
						   IMSCSTR szParamNameList,
						   IMSCSTR* szParamList,
						   PIMSContent pContent);
typedef int (*Proc_IMSGetParams)(HIMSINSTANCE hSession,
								 IMSCSTR szResType,
								 IMSCSTR szParamNameList,
								 IMSCSTR* szParamList,
								 PIMSContent pContent);

/** 
* @brief   IMSSynthSpeak
* 
* Request Synthesizer Resource to Speak some content
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession       - [in] Mrcp Session Instance
* @param   IMSCSTR szSynthParams       - [in] Param to be set while Speaking
* @param   const PIMSContent pContent  - [in, out] Content to be spoken/return value for this speak
* @param   int* pdwSpeakId             - [out] Speak ID of this Speak Request,used by STOP/PAUSE/RESUME method
* @param   PIMSPROCESSCB pCallBack     - [in] Callback function for fetching audio data
* @see
*/
int IMSLIBAPI IMSSynthSpeak(HIMSINSTANCE hSession,
							IMSCSTR szSynthParams,
							const PIMSContent pContent,
							int nContentCount,
							int* pnSpeakId,
 							const IMSPROCESSCB pCallBack);
typedef int (*Proc_IMSSynthSpeak)(HIMSINSTANCE hSession,
							 IMSCSTR szSynthParams,
							 const PIMSContent pContent,
							 int nContentCount,
							 int* pnSpeakId,
							 const IMSPROCESSCB pCallBack);

/** 
* @brief   IMSSynthStop
* 
* Request Synthesizer Resource to Stop current/all speaking
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession - [in] Mrcp Session Instance
* @param   int speak_count       - [in] Total Count of speak ids to stop
* @param   int* pdwSpeakIds      - [in] all Speak ids of Speak Request to stop
* @see
*/
int IMSLIBAPI IMSSynthStop(HIMSINSTANCE hSession,
						   int pnStopCount,
						   int* pnSpeakIds);
typedef int (*Proc_IMSSynthStop)(HIMSINSTANCE hSession,
								 int pnStopCount,
								 int* pnSpeakIds);

/** 
* @brief   IMSSynthPause
* 
* Request Synthesizer Resource to Pause current speaking
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @param   int* pdwSpeakId        - [out] Speak id of Speak Request affected
* @see
*/
int IMSLIBAPI IMSSynthPause(HIMSINSTANCE hSession, int* pnSpeakId);
typedef int (*Proc_IMSSynthPause)(HIMSINSTANCE hSession, int* pnSpeakId);

/** 
* @brief   IMSSynthResume
* 
* Request Synthesizer Resource to Resume current paused speaking
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @param   int* pdwSpeakId        - [out] Speak id of Speak Request affected
* @see
*/
int IMSLIBAPI IMSSynthResume(HIMSINSTANCE hSession, int* pnSpeakId);
typedef int (*Proc_IMSSynthResume)(HIMSINSTANCE hSession, int* pnSpeakId);

/** 
* @brief   IMSSynthBargin
* 
* Request Synthesizer Resource to interrupt current speaking because of Recognizer's notification
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @param   IMSCSTR proxy_id       - [in] Proxy id used to barge in in Synthesizer
* @see
*/
int IMSLIBAPI IMSSynthBargin(HIMSINSTANCE hSession, IMSCSTR szProxyId);
typedef int (*Proc_IMSSynthBargin)(HIMSINSTANCE hSession, IMSCSTR szProxyId);

/** 
* @brief   IMSSynthControl
* 
* Request Synthesizer Resource to do some action like speed up/down etc.
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession    - [in] Mrcp Session Instance
* @param   IMSCSTR szControlParams  - [in] set Jump Size of current speak
* @param   IMSBOOL* pbSpeakRestart  - [out] if this speak is restart
* @param   int* pnSpeakId           - [out] Speak id of Speak Request affected
* @see
*/
int IMSLIBAPI IMSSynthControl(HIMSINSTANCE hSession,
							  IMSCSTR szControlParams,
							  IMSBOOL* pbSpeakRestart,
							  int* pnSpeakId);
typedef int (*Proc_IMSSynthControl)(HIMSINSTANCE hSession,
									IMSCSTR szControlParams,
									IMSBOOL* pbSpeakRestart,
									int* pnSpeakId);

/** 
* @brief   IMSSynthDefineLexicon
* 
* Request Synthesizer Resource to load lexicon needed by server
* Only Supported by Mrcp/2.0
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession    - [in] Mrcp Session Instance
* @param   const PIMSData pContent  - [in] Content array of lexicon
* @param   int nContentCount        - [in] Content count of lexicon
* @param   IMSBOOL bLoadLexicon     - [in] if Load this lexcon
* @see
*/
int IMSLIBAPI IMSSynthDefineLexicon(HIMSINSTANCE hSession,
									const PIMSContent pContent,
									int nContentCount,
									IMSBOOL bLoadLexicon);
typedef int (*Proc_IMSSynthDefineLexicon)(HIMSINSTANCE hSession,
										  const PIMSContent pContent,
										  int nContentCount,
										  IMSBOOL bLoadLexicon);

/** 
* @brief   IMSRecogDefGrammar
* 
* Request Recognizer Resource to define Recognize Grammar
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession       - [in] Mrcp Session Instance
* @param   IMSCSTR szDefParam          - [in] params to define grammar
* @param   const PIMSContent pContent  - [in] Content array of grammar
* @param   int nContentCount           - [in] Content count of grammar
* @param   int* pnGrammarId            - [out] Grammar id
* @param
* @see
*/
int IMSLIBAPI IMSRecogDefGrammar(HIMSINSTANCE hSession,
								 IMSCSTR szDefParam,
								 const PIMSContent pContent,
								 int nContentCount,
								 int* pnGrammarId);
typedef int (*Proc_IMSRecogDefGrammar)(HIMSINSTANCE hSession,
									   IMSCSTR szDefParam,
									   const PIMSContent pContent,
									   int nContentCount,
									   int* pnGrammarId);

/** 
* @brief   IMSRecognize
* 
* Request Recognizer Resource to Recognize some speech audio
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession          - [in] Mrcp Session Instance
* @param   IMSCSTR szRecogParam           - [in] Recognize params
* @param   const PIMSContent pContent     - [in] Recognize grammar Content array
* @param   int nContentCount              - [in] Recognize grammar Content count
* @param   int* pnRecogId                 - [out] id of this recognize
* @param   const PIMSPROCESSCB pCallBack  - [in] callback function for the recognize
* @see
*/
int IMSLIBAPI IMSRecogRecognize(HIMSINSTANCE hSeesion,
								IMSCSTR szRecogParam,
								const PIMSContent pContent,
								int nContentCount,
								int* pnRecogId,
								const IMSPROCESSCB pCallBack);
typedef int (*Proc_IMSRecogRecognize)(HIMSINSTANCE hSeesion,
									  IMSCSTR szRecogParam,
									  const PIMSContent pContent,
									  int nContentCount,
									  int* pnRecogId,
									  const IMSPROCESSCB pCallBack);

/** 
* @brief   IMSRecogStop
* 
* Request Recognizer Resource to Stop recognizing
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @param   int* pnRecogId         - [out] Recognize id affected
* @see
*/
int IMSLIBAPI IMSRecogStop(HIMSINSTANCE hSession, int* pnRecogId);
typedef int (*Proc_IMSRecogStop)(HIMSINSTANCE hSession, int* pnRecogId);

/** 
* @brief   IMSInterpret
* 
* Request Recognizer Resource to Interpret some content
* Only Supported by Mrcp/2.0
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession              - [in] Mrcp Session Instance
* @param   IMSCSTR szInterpretText            - [in] text to interpret
* @param   const PIMSContent pGrammarContent  - [in] Grammar Content array for this interpret
* @param   int nGrammarContentCount           - [in] Grammar Content count for this interpret
* @param   PIMSContent pResultContent         - [out] Interpret result content
* @see
*/
int IMSLIBAPI IMSRecogInterpret(HIMSINSTANCE hSession,
								IMSCSTR szInterpretText,
								const PIMSContent pGrammarContent,
								int nGrammarContentCount,
								PIMSContent pResultContent);
typedef int (*Proc_IMSRecogInterpret)(HIMSINSTANCE hSession,
									  IMSCSTR szInterpretText,
									  const PIMSContent pGrammarContent,
									  int nGrammarContentCount,
									  PIMSContent pResultContent);

/** 
* @brief   IMSGetResult
* 
* Request Recognizer Resource to Send the Recog/Interpret Result again,which may be another result by the params
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @param   int nRecogId           - [in] Recognize Id
* @param   IMSCSTR szRecogParam   - [in] Param for get result
* @param   PIMSContent pContent   - [out] the returned result content
* @see
*/
int IMSLIBAPI IMSRecogGetResult(HIMSINSTANCE hSession,
								int nRecogId,
								IMSCSTR szRecogParam,
								PIMSContent pContent);
typedef int (*Proc_IMSRecogGetResult)(HIMSINSTANCE hSession,
									  int nRecogId,
									  IMSCSTR szRecogParam,
									  PIMSContent pContent);

/** 
* @brief   IMSStartInputTimer
* 
* Request Recognizer Resource to Start Input Timer
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @see
*/
int IMSLIBAPI IMSRecogStartInputTimer(HIMSINSTANCE hSession);
typedef int (*Proc_IMSRecogStartInputTimer)(HIMSINSTANCE hSession);

/** 
* @brief   IMSRecogVEStartPhraseEnrollment
* 
* Request Recognizer Resource to start phrase enrollment
* Only Supported by Mrcp/2.0
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @param   IMSCSTR szEnrollParam  - [in] Params of Start enrollment
* @see
*/
int IMSLIBAPI IMSRecogVEStartPhraseEnrollment( HIMSINSTANCE hSession
											 , IMSCSTR szEnrollParam );
typedef int (*Proc_IMSRecogVEStartPhraseEnrollment)( HIMSINSTANCE hSession
												   , IMSCSTR szEnrollParam);

/** 
 * @brief   IMSRecogVEEndPhraseEnrollment
 * 
 * Request Recognizer Resource to end phrase enrollment
 * Only Supported by Mrcp/2.0
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession   - [in] Mrcp Session Instance
 * @param   IMSCSTR szEndParam	    - [in] Params of End enrollment
 * @param   IMSCSTR* szWaveformUri  - [out] URI of the best repetition of Phrase, malloced by Lib
 * @see
 */
int IMSLIBAPI IMSRecogVEEndPhraseEnrollment( HIMSINSTANCE hSession
										   , IMSCSTR szEndParam
										   , IMSCSTR* szWaveformUri );
typedef int (*Proc_IMSRecogVEEndPhraseEnrollment)( HIMSINSTANCE hSession
												 , IMSCSTR szEndParam
												 , IMSCSTR* szWaveformUri );

/** 
* @brief   IMSRecogVEEnrollmentRollback
* 
* Request Recognizer Resource to Rollback last Phrase Enrollment
* Only Supported by Mrcp/2.0
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @see
*/
int IMSLIBAPI IMSRecogVEEnrollmentRollback( HIMSINSTANCE hSession );
typedef int (*Proc_IMSRecogVEEnrollmentRollback)( HIMSINSTANCE hSession );

/** 
* @brief   IMSRecogVEModifyPhrase
* 
* Request Recognizer Resource to Modify the phrase in the user's grammar
* Only Supported by Mrcp/2.0
* 
* @author  iFLYTEK
* @return  int - Return 0 in success, otherwise return error code.
* @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
* @param   IMSCSTR szModifyParam  - [in] Params of Modify Phrase
* @see
*/
int IMSLIBAPI IMSRecogVEModifyPhrase( HIMSINSTANCE hSession
								    , IMSCSTR szModifyParam );
typedef int (*Proc_IMSRecogVEModifyPhrase)( HIMSINSTANCE hSession
										  , IMSCSTR szModifyParam );

/** 
 * @brief   IMSRecogVEDeletePhrase
 * 
 * Request Recognizer Resource to Delete the phrase in the user's grammar
 * Only Supported by Mrcp/2.0
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession   - [in] Mrcp Session Instance
 * @param   IMSCSTR szDelParam		- [in] Params of Delete Phrase
 * @see
 */
int IMSLIBAPI IMSRecogVEDeletePhrase( HIMSINSTANCE hSession
									, IMSCSTR szDelParam );
typedef int (*Proc_IMSRecogVEDeletePhrase)( HIMSINSTANCE hSession
										  , IMSCSTR szDelParam );

/** 
 * @brief   IMSRecordRecord
 * 
 * Request Recorder Resource to record
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession	 - [in] Mrcp Session Instance
 * @param	IMSCSTR szRecorParam     - [in] Param(s) of recording
 * @param   int* pnRecorId           - [out] Record id of this request
 * @see
 */
int IMSLIBAPI IMSRecordRecord( HIMSINSTANCE hSession
							 , IMSCSTR szRecorParam
							 , int* pnRecorId );
typedef int (*Proc_IMSRecordRecord)( HIMSINSTANCE hSession
								   , IMSCSTR szRecorParam
								   , int* pnRecorId );

/** 
 * @brief   IMSRecordStop
 * 
 * Request Recorder Resource to Stop recording
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @param	IMSCSTR szStopParam    - [in] Param(s) of stopping
 * @param   IMSCSTR* szRecordUri   - [out] Record uri, malloced by Lib
 * @param   int* pnRecorId         - [out] Record id affected
 * @see
 */
int IMSLIBAPI IMSRecordStop( HIMSINSTANCE hSession
						   , IMSCSTR szStopParam
						   , IMSCSTR* szRecordUri
						   , int* pnRecorId );
typedef int (*Proc_IMSRecordStop)( HIMSINSTANCE hSession
								 , IMSCSTR szStopParam
								 , IMSCSTR* szRecordUri
								 , int* pnRecorId );

/** 
 * @brief   IMSRecordStartInputTimers
 * 
 * Request Recorder Resource to start input timers
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @see
 */
int IMSLIBAPI IMSRecordStartInputTimers( HIMSINSTANCE hSession );
typedef int (*Proc_IMSRecordStartInputTimers)( HIMSINSTANCE hSession );

/** 
 * @brief   IMSVerifierStartSession
 * 
 * Request Verifier Resource to start session
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @param   IMSCSTR szStartParam   - [in] Params of starting session
 * @see
 */
int IMSLIBAPI IMSVerifierStartSession( HIMSINSTANCE hSession
									 , IMSCSTR szStartParam );
typedef int (*Proc_IMSVerifierStartSession)( HIMSINSTANCE hSession
										   , IMSCSTR szStartParam );

/** 
 * @brief   IMSVerifierEndSession
 * 
 * Request Verifier Resource to end session
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @param   IMSCSTR szEndParam     - [in] Params of ending session
 * @see
 */
int IMSLIBAPI IMSVerifierEndSession( HIMSINSTANCE hSession
								   , IMSCSTR szEndParam );
typedef int (*Proc_IMSVerifierEndSession)( HIMSINSTANCE hSession
										 , IMSCSTR szEndParam );

/** 
 * @brief   IMSVerifierQueryVoiceprint
 * 
 * Request Verifier Resource to query voiceprint
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @param   IMSCSTR szQueryParam   - [in] Params of querying voiceprint
 * @param   IMSBOOL* exist         - [out] Whether the voiceprint exists or not
 * @see
 */
int IMSLIBAPI IMSVerifierQueryVoiceprint( HIMSINSTANCE hSession 
										, IMSCSTR szQueryParam 
										, IMSBOOL* exist );
typedef int (*Proc_IMSVerifierQueryVoiceprint)( HIMSINSTANCE hSession 
											  , IMSCSTR szQueryParam 
										      , IMSBOOL* exist );

/** 
 * @brief   IMSVerifierDeleteVoiceprint
 * 
 * Request Verifier Resource to delete voiceprint
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @param   IMSCSTR szDelParam     - [in] Params of deleting voiceprint
 * @see
 */
int IMSLIBAPI IMSVerifierDeleteVoiceprint( HIMSINSTANCE hSession
										 , IMSCSTR szDelParam );
typedef int (*Proc_IMSVerifierDeleteVoiceprint)( HIMSINSTANCE hSession
											   , IMSCSTR szDelParam );

/** 
 * @brief   IMSVerifierVerify
 * 
 * Request Verifier Resource to verify
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @param   IMSCSTR szVerParam     - [in] Params of verifying
 * @param   int* pnVerId           - [out] Id of this verification
 * @see
 */
int IMSLIBAPI IMSVerifierVerify( HIMSINSTANCE hSession 
							   , IMSCSTR szVerParam
							   , int* pnVerId );
typedef int (*Proc_IMSVerifierVerify)( HIMSINSTANCE hSession 
								     , IMSCSTR szVerParam
									 , int* pnVerId );

/** 
 * @brief   IMSVerifierVerifyFromBuffer
 * 
 * Request Verifier Resource to verify from buffer
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @param   IMSCSTR szVerParam     - [in] Params of verifying from buffer
 * @param   int* pnVerId           - [out] Id of this verification
 * @see
 */
int IMSLIBAPI IMSVerifierVerifyFromBuffer( HIMSINSTANCE hSession
										 , IMSCSTR szVerParam
										 , int* pnVerId );
typedef int (*Proc_IMSVerifierVerifyFromBuffer)( HIMSINSTANCE hSession
											   , IMSCSTR szVerParam
											   , int* pnVerId );

/** 
 * @brief   IMSVerifierVerifyRollback
 * 
 * Request Verifier Resource to do verify_rollback
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @see
 */
int IMSLIBAPI IMSVerifierVerifyRollback( HIMSINSTANCE hSession );
typedef int (*Proc_IMSVerifierVerifyRollback)( HIMSINSTANCE hSession );

/** 
 * @brief   IMSVerifierStop
 * 
 * Request Verifier Resource to stop verification
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @param   IMSCSTR szStopParam    - [in] Param of stopping
 * @param   PIMSContent pContent   - [out] Content returned by server
 * @param   int* pnVerId           - [out] Verification id affected
 * @see
 */
int IMSLIBAPI IMSVerifierStop( HIMSINSTANCE hSession
							 , IMSCSTR szStopParam
							 , PIMSContent pContent
							 , int* pnVerId );
typedef int (*Proc_IMSVerifierStop)( HIMSINSTANCE hSession
								   , IMSCSTR szStopParam
								   , PIMSContent pContent
								   , int* pnVerId );

/** 
 * @brief   IMSVerifierClearBuffer
 * 
 * Request Verifier Resource to clear buffer
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @see
 */
int IMSLIBAPI IMSVerifierClearBuffer( HIMSINSTANCE hSession );
typedef int (*Proc_IMSVerifierClearBuffer)( HIMSINSTANCE hSession );

/** 
 * @brief   IMSVerifierStartInputTimers
 * 
 * Request Verifier Resource to start input timers
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @see
 */
int IMSLIBAPI IMSVerifierStartInputTimers( HIMSINSTANCE hSession );
typedef int (*Proc_IMSVerifierStartInputTimers)( HIMSINSTANCE hSession );

/** 
 * @brief   IMSVerifierGetIntermediateResult
 * 
 * Request Verifier Resource to get intermediate result
 * 
 * @author  iFLYTEK
 * @return  int - Return 0 in success, otherwise return error code.
 * @param   HIMSINSTANCE hSession  - [in] Mrcp Session Instance
 * @param   PIMSContent pContent   - [out] Result content returned by server
 * @see
 */
int IMSLIBAPI IMSVerifierGetIntermediateResult( HIMSINSTANCE hSession 
											  , PIMSContent pResultContent );
typedef int (*Proc_IMSVerifierGetIntermediateResult)( HIMSINSTANCE hSession 
													, PIMSContent pResultContent );

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif	/* IFLY_MRCP_H */
