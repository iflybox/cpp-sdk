/* ----------------------------------------------------------- */
/*      Module : Hcr.h-> header iFlyHcr engine             */
/*      Author : liangwang@iflytek.com                         */
/*      Version : 2.95                                         */
/* notice: 1, should work with corresponding library which has */
/*            the same version!!!                              */
/*         2, this is thread-unsafe version, should use all    */
/*            functions in the same thread!!!                  */
/* ----------------------------------------------------------- */


#ifndef __IFLYHCR_H__
#define __IFLYHCR_H__

#ifdef __cplusplus
extern  "C"
{
#endif

#ifdef	WIN32
#ifdef	AIWRITEAPI_EAPORT
#define	AIWRITEAPI __declspec(dllexport)
#else
#define	AIWRITEAPI __declspec(dllimport)
#endif	/* IFLYISE_EXPORTS */
#else /* defined(WIN32) */
# define AIWRITEAPI
#endif /* defined(WIN32) */

    typedef     int               iHCR_RET;
#define     iHCR_OK           ( 0 )
#define     iHCR_ERROR        ( -1 )
#define     iHCR_INVCAL       ( -2 )
#define     iHCR_INVARG       ( -3 )
#define     iHCR_RESERROR     ( -4 )


#define     iHCR_LANGUAGE_NONE                  ( 0x00000000 )
    /* a-z */
#define     iHCR_LANGUAGE_English_LowerCase     ( 0x00000001 )
    /* A-Z */
#define     iHCR_LANGUAGE_English_UpperCase     ( 0x00000002 )
    /* lowercase and uppercase */
#define     iHCR_LANGUAGE_English               ( iHCR_LANGUAGE_English_LowerCase | iHCR_LANGUAGE_English_UpperCase )

    /* interpunctions: 0x0021-0x002F(0xFF01-0xFF0F),0x003A-0x0040(0xFF1A-0xFF20), */
    /* 0x005B-0x0060(0xFF3B-0xFF40),0x007B-0x007E(0xFF5B-0xFF5E), 0x00A2(0xFFE0), */
    /* 0x00A3(0xFFE1),0x00A5(0xFFE5),0x00B0,0x00D7,0x00F7, 0x2014,0x2018,0x2019,  */
    /* 0x201C,0x201D,0x2022,0x2026,0x2030,0x2032,0x2033,0x20AC,0x2103,0x221A,0x2236, */
    /* 0x3001,0x3002,0x3008,0x3009,0x300A,0x300B */
#define     iHCR_LANGUAGE_Symbols               ( 0x00000004 )
    /* 0-9 */
#define     iHCR_LANGUAGE_Numerals              ( 0x00000008 )
    /* space(0x0020), enter(0x000D), backspace(0x0008), delete(0x001E) */
#define     iHCR_LANGUAGE_Gestures              ( 0x00000010 )

#define     iHCR_LANGUAGE_REGION_Mask           ( 0xF0000000 )
#define     iHCR_LANGUAGE_REGION_European       ( 0x10000000 )
#define     iHCR_LANGUAGE_REGION_African        ( 0x20000000 )
#define     iHCR_LANGUAGE_REGION_MiddleEastern  ( 0x30000000 )
#define     iHCR_LANGUAGE_REGION_American       ( 0x40000000 )
#define     iHCR_LANGUAGE_REGION_Indic          ( 0x50000000 )
#define     iHCR_LANGUAGE_REGION_Philippine     ( 0x60000000 )
#define     iHCR_LANGUAGE_REGION_SouthEastAsian ( 0x70000000 )
#define     iHCR_LANGUAGE_REGION_EastAsian      ( 0x80000000 )
#define     iHCR_LANGUAGE_REGION_CentralAsian   ( 0x90000000 )
#define     iHCR_LANGUAGE_REGION_AncientAsian   ( 0xA0000000 )
#define     iHCR_LANGUAGE_REGION_Other          ( 0xB0000000 )

#define     iHCR_LANGUAGE_German                ( iHCR_LANGUAGE_REGION_European      | 0x00000100 )
#define     iHCR_LANGUAGE_French                ( iHCR_LANGUAGE_REGION_European      | 0x00000200 )
#define     iHCR_LANGUAGE_Italy                 ( iHCR_LANGUAGE_REGION_European      | 0x00000400 )
#define     iHCR_LANGUAGE_Spanish               ( iHCR_LANGUAGE_REGION_European      | 0x00000800 )
#define     iHCR_LANGUAGE_Portuguese            ( iHCR_LANGUAGE_REGION_European      | 0x00001000 )
#define     iHCR_LANGUAGE_Polish                ( iHCR_LANGUAGE_REGION_European      | 0x00002000 )
#define     iHCR_LANGUAGE_Dutch                 ( iHCR_LANGUAGE_REGION_European      | 0x00004000 )
#define     iHCR_LANGUAGE_Czech                 ( iHCR_LANGUAGE_REGION_European      | 0x00008000 )
#define     iHCR_LANGUAGE_Hungarian             ( iHCR_LANGUAGE_REGION_European      | 0x00010000 )
#define     iHCR_LANGUAGE_Danish                ( iHCR_LANGUAGE_REGION_European      | 0x00020000 )
#define     iHCR_LANGUAGE_Finnish               ( iHCR_LANGUAGE_REGION_European      | 0x00040000 )
#define     iHCR_LANGUAGE_Icelandic             ( iHCR_LANGUAGE_REGION_European      | 0x00080000 )
#define     iHCR_LANGUAGE_Norwegian             ( iHCR_LANGUAGE_REGION_European      | 0x00100000 )
#define     iHCR_LANGUAGE_Swedish               ( iHCR_LANGUAGE_REGION_European      | 0x00200000 )
#define     iHCR_LANGUAGE_Turkish               ( iHCR_LANGUAGE_REGION_European      | 0x00400000 )
#define     iHCR_LANGUAGE_Russian               ( iHCR_LANGUAGE_REGION_European      | 0x00800000 )
#define     iHCR_LANGUAGE_Serbian               ( iHCR_LANGUAGE_REGION_European      | 0x01000000 )
#define     iHCR_LANGUAGE_Greek                 ( iHCR_LANGUAGE_REGION_European      | 0x02000000 )
#define     iHCR_LANGUAGE_Macedonian            ( iHCR_LANGUAGE_REGION_European      | 0x04000000 )

#define     iHCR_LANGUAGE_Arabic                ( iHCR_LANGUAGE_REGION_MiddleEastern | 0x00000100 )
    /* Not supported yet */
#define     iHCR_LANGUAGE_Hebrew                ( iHCR_LANGUAGE_REGION_MiddleEastern | 0x00000200 )

#define     iHCR_LANGUAGE_Thai                  ( iHCR_LANGUAGE_REGION_SouthEastAsian | 0x00000100 )
#define     iHCR_LANGUAGE_Vietnamese            ( iHCR_LANGUAGE_REGION_SouthEastAsian | 0x00000200 )

    /* GB2312, total number: 6763 */
#define     iHCR_LANGUAGE_Chinese_GB            ( iHCR_LANGUAGE_REGION_EastAsian     | 0x00000200 )
    /* GBK, total number: 21003 */
#define     iHCR_LANGUAGE_Chinese_GBK           ( iHCR_LANGUAGE_REGION_EastAsian     | 0x00000100 )
    /* Big5, total number: 13053 */
#define     iHCR_LANGUAGE_Chinese_Big5          ( iHCR_LANGUAGE_REGION_EastAsian     | 0x00000400 )
#define     iHCR_LANGUAGE_Chinese_Symbols       ( iHCR_LANGUAGE_REGION_EastAsian     | 0x00000800 )
#define     iHCR_LANGUAGE_Hangul                ( iHCR_LANGUAGE_REGION_EastAsian     | 0x00004000 )
#define     iHCR_LANGUAGE_Japanese_Hiragana     ( iHCR_LANGUAGE_REGION_EastAsian     | 0x00008000 )
#define     iHCR_LANGUAGE_Japanese_Katagana     ( iHCR_LANGUAGE_REGION_EastAsian     | 0x00010000 )
#define     iHCR_LANGUAGE_Japanese_Kanji        ( iHCR_LANGUAGE_REGION_EastAsian     | 0x00020000 )


#define     iHCR_RECOGNITION_CONFIG_FLAG         ( 0xF000 )
    /* default recognition manner, treat input points as character */
#define     iHCR_RECOGNITION_CHAR               ( 0x1000 )
    /* treat input points as sentence, *new* in iHCR3.0 */
#define     iHCR_RECOGNITION_SENT_MASK          ( 0x2000 )
    /* treat input points as sentence written in a line */
#define     iHCR_RECOGNITION_SENT_LINE          ( iHCR_RECOGNITION_SENT_MASK | 0x0001 )
    /* treat input points as sentence written in full-screen */
#define     iHCR_RECOGNITION_SENT_FS            ( iHCR_RECOGNITION_SENT_MASK | 0x0002 )
    /* treat input points as sentence written overlaply */
#define     iHCR_RECOGNITION_SENT_OVLP          ( iHCR_RECOGNITION_SENT_MASK | 0x0003 )

    /* language-model type defintions: used for lm-configuration's convenience, *new* in 3.0 */
#define     iHCR_RECOGNITION_LM_MASK            ( 0x3000 )
    /* apply to sms/note context */
#define     iHCR_RECOGNITION_LM_NOTE            ( iHCR_RECOGNITION_LM_MASK | 0x0000 )
    /* apply to persons' names context */
#define     iHCR_RECOGNITION_LM_NAME            ( iHCR_RECOGNITION_LM_MASK | 0x0001 )
    /* apply to telephone number context */
#define     iHCR_RECOGNITION_LM_TEL             ( iHCR_RECOGNITION_LM_MASK | 0x0002 )
    /* apply to date/time context */
#define     iHCR_RECOGNITION_LM_TIME            ( iHCR_RECOGNITION_LM_MASK | 0x0003 )
    /* apply to address context */
#define     iHCR_RECOGNITION_LM_ADDR            ( iHCR_RECOGNITION_LM_MASK | 0x0004 )
    /* apply to arithmetic calculation context */
#define     iHCR_RECOGNITION_LM_CALC            ( iHCR_RECOGNITION_LM_MASK | 0x0005 )
#define     iHCR_RECOGNITION_LM_DEFAULT         ( iHCR_RECOGNITION_LM_NOTE )

    /* toggle recognition support for cursive strokes */
#define     iHCR_RECOGNITION_CURSIVE            ( 0x0001 )
    /* toggle recognition support for slant handwritten word */
#define     iHCR_RECOGNITION_SLANT              ( 0x0002 )
    /* toggle recognition support for disorder written strokes */
#define     iHCR_RECOGNITION_DISORDERED         ( 0x0004 )
    /* toggle recognition support for rotated handwritten word */
#define     iHCR_RECOGNITION_ROTATION           ( 0x0008 )
#define     iHCR_RECOGNITION_DEFAULT            ( iHCR_RECOGNITION_CURSIVE )

    /* new function in iHCR2.1 */
    /* a one-stop recognition function */
#define     iHCR_ACTION_FLAG                    ( 0xF0000000 )
#define     iHCR_ACTION_MASK                    ( 0x10000000 )
#define     iHCR_ACTION_Recognition             ( iHCR_ACTION_MASK | 0x00000001 )
#define     iHCR_ACTION_Trad2Simp               ( iHCR_ACTION_MASK | 0x00000002 )
#define     iHCR_ACTION_Simp2Trad               ( iHCR_ACTION_MASK | 0x00000003 )
#define     iHCR_ACTION_Homophone               ( iHCR_ACTION_MASK | 0x00000004 )
#define     iHCR_ACTION_LearnChar               ( iHCR_ACTION_MASK | 0x00000005 )
#define     iHCR_ACTION_LearnWord               ( iHCR_ACTION_MASK | 0x00000006 )
#define     iHCR_ACTION_Prediction              ( iHCR_ACTION_MASK | 0x00000007 )
#define     iHCR_ACTION_Reset                   ( iHCR_ACTION_MASK | 0x00000008 )

    /* suggested size for user dict space */
#define     iHCR_USERDICT_SIZE                  ( 96 * 1024 )

#ifndef __defined_iHCR_PARAM__
#define __defined_iHCR_PARAM__
    typedef enum
    {
        /* parameter indicating recognition range */
        /* supported value range: iHCR_LANGUAGE_* and their '|' combination */
        /* with iHCR_LANGUAGE_ flag of english/numeral/symbols */
        iHCR_PARAM_LANGUAGE,

        /* parameter indication recognition mode */
        /* support value range: iHCR_RECONIGTION_CURSIVE, iHCR_RECOGNITION_SLANT, */
        /* iHCR_RECOGNITION_DISORDERED, iHCR_RECOGNITION_ROTATION and their '|' combination */
        iHCR_PARAM_RECOGMODE,

        /* parmeter indicating recognition engine's working manner, *new* in 3.0 */
        /* supported value range: iHCR_RECOGNITION_CHAR, iHCR_RECOGNITION_SENT_LINE, */
        /* iHCR_RECOGNITION_SENT_FS, iHCR_RECOGNITION_SENT_OVLP */
        iHCR_PARAM_RECOGMANNER,

        /* *new* in 3.0 */
        /* parameter indicating which type of language-model is to be used */
        /* supported value range: iHCR_RECOGNITION_LM_* */
        iHCR_PARAM_RECOGLM,

        /* parameter indication maximum word number of returned recognition result */
        iHCR_PARAM_MAXCANDNUM
    } iHCR_PARAM;
#endif




    typedef void* HcrInstance;
    typedef HcrInstance* PHcrInstance;

    typedef void* Hcr_ResourceInst;
    typedef Hcr_ResourceInst* PHcr_ResourceInst;

#ifndef __HcrPoint__
#define  __HcrPoint__
#pragma pack(1)
    typedef struct HcrPoint
    {
        short   x;  // x-coordinate
        short   y;  // y-coordinate
        unsigned char   s;  // pen state or flags
    } HcrPoint, *PHcrPoint;
#pragma pack()
#endif


//   typedef void* Hcr_ResourceInst;



    /**
     * @brief	Create HCR Recognize Instance
     *
     * detail...
     *
     * @author	liangwang
     * @date	2010-9-13
     * @return	long								not zero mean error
     * @param	HcrInstance*  phInstance			[out]return created HCR Instance handle
     * @param	const char* pParams					[in]parameters to create HCR Instance
     * example  std::sting params("__HWR_RES__=hwrfilename.dat;__LM_RES__=lmfilename.dat;__USER_DICT_RES__=userdictfilename;__USER_WORD_RES__=userwrodfilename;")
     __HWR_RES__		:		HWR resource index str
     __LM_RES__			:		Language Module resource index str
     __USER_DICT_RES__	:		User Dict resource index str
     __USER_WORD_RES__	:		User Word resource index str
     * @see
     * @exception
     */
    long AIWRITEAPI iFlyHcrModuleCreate( HcrInstance*  phInstance, const char* pParams);
    typedef long (*Proc_iFlyHcrModuleCreate)( HcrInstance*  phInstance, const char* pParams);


    /**
     * @brief	Destory HCR Recognize Instance
     *
     * detail...
     *
     * @author	liangwang
     * @date	2010-9-13
     * @return	long								not zero mean error
     * @param	HcrInstance hInstance				[in]recognize instance handle to destory
     * @see
     * @exception
     */
    long AIWRITEAPI iFlyHcrModuleDestroy( HcrInstance hInstance );
    typedef long (*Proc_iFlyHcrModuleDestroy)( HcrInstance hInstance );

    /**
    * @brief	Start One Section of Recognize
    *
    * detail...
    *
    * @author	liangwang
    * @date	2010-9-13
    * @return	long								not zero mean error
    * @param	 HcrInstance hInstance				[in] recognize instance handle
    * @param	long RecognitionLanguage			[in] recognize support language
    * @param	long RecognitionManner				[in] recognize manner
    * @param	long RecognitionMode				[in] recognize mode
    * @param	long RecognitionMaxCandidateNumber	[in] recognize max candidate number
    * @param	long HandwriteingAreaLeft			[in] left of handwriting area
    * @param	long HandwriteingAreaTop			[in] top of handwriting area
    * @param	long HandwriteingAreaRight			[in] right of handwriting area
    * @param	long HandwriteingAreaBottom			[in] bottom of handwriting area
    * @see
    * @exception
    */
    long AIWRITEAPI iFlyHcrModuleStart(HcrInstance	hInstance,
                                       long RecognitionLanguage, long RecognitionManner, long RecognitionMode, long RecognitionMaxCandidateNumber,
                                       int HandwriteingAreaLeft, int HandwriteingAreaTop, int HandwriteingAreaRight, int bottoHandwriteingAreaBottom);
    typedef long (*Proc_iFlyHcrModuleStart)(HcrInstance	hInstance,
                                            long RecognitionLanguage, long RecognitionManner, long RecognitionMode, long RecognitionMaxCandidateNumber,
                                            int HandwriteingAreaLeft, int HandwriteingAreaTop, int HandwriteingAreaRight, int bottoHandwriteingAreaBottom);


    /**
    * @brief	Append Writing Strokes to Recognize Section
    *
    * detail...
    *
    * @author	liangwang
    * @date	2010-9-13
    * @return	long								not zero mean error
    * @param	 HcrInstance hInstance				[in] recognize instance handle
    * @param	HcrPoint* pPoints					[in] pointer to handwriting points
    * @param	long nPointCount					[in] handwriting points count
    * @see
    * @exception
    */
    long AIWRITEAPI iFlyHcrModuleAppendStrokes( HcrInstance hInstance, HcrPoint* pPoints, long nPointCount );
    typedef long (*Proc_iFlyHcrModuleAppendStrokes)( HcrInstance hInstance, HcrPoint* pPoints, long nPointCount );


    /**
    * @brief	Get Result From Recognize Section
    *
    * detail...
    *
    * @author	liangwang
    * @date	2010-9-13
    * @return	long								not zero mean error
    * @param	 HcrInstance hInstance				[in] recognize instance handle
    * @param	char** ppResult						[out] result string pointer in JSon type
    * @see
    * @exception
    */
    long AIWRITEAPI iFlyHcrModuleGetResult( HcrInstance hInstance, char** ppResult );
    typedef long (*Proc_iFlyHcrModuleGetResult)( HcrInstance hInstance, char** ppResult );



    /**
    * @brief	Predict form HCR Instance
    *
    * detail...
    *
    * @author	liangwang
    * @date	2010-9-13
    * @return	long AIWRITEAPI						not zero mean error
    * @param	 HcrInstance hInstance				[in] recognize instance handle
    * @param	 char *pPreWords					[in] predict from this words, max length is 3036
    * @see
    * @exception
    */
    long AIWRITEAPI iFlyHcrSetPredictData( HcrInstance hInstance, char *pPreWords);
    typedef long (*Proc_iFlyHcrSetPredictData)( HcrInstance hInstance, char *pPreWords);


    /**
    * @brief	Predict form HCR Instance
    *
    * detail...
    *
    * @author	liangwang
    * @date	2010-9-13
    * @return	long AIWRITEAPI						not zero mean error
    * @param	HcrInstance hInstance				[in] recognize instance handle
    * @param	char **pNextWords					[out] predict words, user should't delete the ptr
    * @see
    * @exception
    */
    long AIWRITEAPI iFlyHcrGetPredictResult( HcrInstance hInstance, char **ppNextWords);
    typedef long (*Proc_iFlyHcrGetPredictResult)( HcrInstance hInstance, char **ppNextWords);


#ifdef __cplusplus
}
#endif

#endif