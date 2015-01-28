/** 
 * @file	iFly_Dongle.h
 * @brief	iFlyTek Dongle Management module
 * 
 * detail...
 * 
 * @author	hjye
 * @version	1.0
 * @date	2005��4��20��
 * 
 * @see		
 * 
 * @par �汾��¼��
 * <table border=1>
 *  <tr> <th>�汾	<th>����				<th>����		<th>��ע </tr>
 *  <tr> <td>1.0	<td>2005��4��20��	<td>hjye	<td>���� </tr>
 * </table>
 */

#ifndef _IFLY_DONGLE_H_
#define _IFLY_DONGLE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <time.h>

// for time_t staff
#ifdef WIN32
# if _MSC_VER >= 1400
        typedef __time32_t              time32_t;
        #define time32_func             _time32
        #define mktime32_func           _mktime32
        #define localtime32_func        _localtime32
        #define difftime32_func          _difftime32
# else  // _MSC_VER
        typedef	time_t                  time32_t;
        #define time32_func             time
        #define mktime32_func           mktime
        #define localtime32_func        localtime
        #define difftime32_func         difftime
# endif
#else   // WIN32
        typedef time_t                  time32_t;
        #define time32_func             time
        #define mktime32_func           mktime
        #define localtime32_func        localtime
        #define difftime32_func         difftime
#endif

// for lib api
#if defined(WIN32)
# ifdef IFLYDGL_EXPORTS
#  define DGLLIBAPI __declspec(dllexport)
# else
#  define DGLLIBAPI __declspec(dllimport)
# endif
#else /* defined(WIN32) */
# define DGLLIBAPI
#endif /* defined(WIN32) */

/* Macro to make the version of the iFlyTek dongle module */
#define IFLY_DONGLE_VERSION(MajorVer, MinorVer)	( ( (unsigned short)MajorVer ) << 16 | ( (unsigned short)MinorVer ) )

/* Dongle producer */
typedef enum
{
    dp_unknown          = 0,                    /* Unknown */
    dp_aladdin          = 1,                    /* Aladdin */
    dp_sentinel         = 2,                    /* Sentinel */
} Dongle_Producer;

/* Dongle model */
typedef enum
{
    dm_unknown          = 0,					/* Unknown */

    dm_hasp_3           = 1,                    /* HASP-3     (  0 bytes) */
    dm_hasp_memo_1      = 2,                    /* MemoHASP-1 (112 bytes) */
    dm_hasp_memo_4      = 3,                    /* MemoHASP-4 (496 bytes) */
    dm_hasp_time        = 4,                    /* TimeHasp   ( 16 bytes) */
    dm_hasp_time_4      = 5,                    /* TimeHasp-4 (512 bytes) */
    dm_hasp_net         = 6,                    /* NetHasp    (496 bytes) */

    dm_sentinel_sp      = 7,                    /* Sentinel SuperPro (112�ֽ�) */
    dm_sentinel_up      = 8,                    /* Sentinel UltraPro (504�ֽ�) */
    dm_hasp_hl          = 9,                    /* HASP HL (3584�ֽ�) */

    dm_hasphl_time      = 10,                   /* hasphl��ʱ�乷 */
    dm_hasp_file        = 11,                   /* ������ļ��� */
} Dongle_Model;

/* Dongle electric interface model */
typedef enum
{
    det_unknown         = 0,                    /* Unknown */
    det_lpt             = 1,                    /* LPT */
    det_usb             = 2,                    /* USB */
} Dongle_Ele_Type;

/* Version of the protection information structure reside in dongle memory */
#define STRUCT_VERSION_UNKNOWN      -1          /* Unknown information version */
#define STRUCT_VERSION_INTP35       0           /* Version used by Intp35 (Start at KD2000 and stop in Intp35) */
#define STRUCT_VERSION_INTP40       1           /* Version used by Intp40 (Start at Intp40) */

/* Voice flag */
#define DONGLE_PROT_VOICE_WenJing   0x00000001  /* WenJing */
#define DONGLE_PROT_VOICE_YanPing   0x00000002  /* YanPing */
#define DONGLE_PROT_VOICE_XiaoMei   0x00000004  /* XiaoMei */
#define DONGLE_PROT_VOICE_YuFeng    0x00000008  /* YuFeng */
#define DONGLE_PROT_VOICE_Sherri    0x00000010  /* Sherri */
#define DONGLE_PROT_VOICE_XieJing   0x00000020  /* XieJing */
#define DONGLE_PROT_VOICE_PeiYu     0x00000040  /* PeiYu */
#define DONGLE_PROT_VOICE_Wangru	0x00000080  /* voicer 8 */
#define DONGLE_PROT_VOICE_Tianchang 0x00000100  /* voicer 9 */
#define DONGLE_PROT_VOICE_Terry     0x00000200  /* voicer 10 */
#define DONGLE_PROT_VOICE_Voicer11  0x00000400  /* voicer 11*/
#define DONGLE_PROT_VOICE_Voicer12  0x00000800  /* voicer 12 */
#define DONGLE_PROT_VOICE_Voicer13  0x00001000  /* voicer 13 */
#define DONGLE_PROT_VOICE_Voicer14  0x00002000  /* voicer 14 */
#define DONGLE_PROT_VOICE_Voicer15  0x00004000  /* voicer 15 */
#define DONGLE_PROT_VOICE_Voicer16  0x00008000  /* voicer 16 */
#define DONGLE_PROT_VOICE_Voicer17  0x00010000  /* voicer 17 */
#define DONGLE_PROT_VOICE_Voicer18  0x00020000  /* voicer 18 */
#define DONGLE_PROT_VOICE_Voicer19  0x00040000  /* voicer 19 */
#define DONGLE_PROT_VOICE_Voicer20  0x00080000  /* voicer 20 */
#define DONGLE_PROT_VOICE_Voicer21  0x00100000  /* voicer 21 */
#define DONGLE_PROT_VOICE_Voicer22  0x00200000  /* voicer 22 */
#define DONGLE_PROT_VOICE_Voicer23  0x00400000  /* voicer 23 */
#define DONGLE_PROT_VOICE_Voicer24  0x00800000  /* voicer 24 */
#define DONGLE_PROT_VOICE_Voicer25  0x01000000  /* voicer 25 */
#define DONGLE_PROT_VOICE_Voicer26  0x02000000  /* voicer 26 */
#define DONGLE_PROT_VOICE_Voicer27  0x04000000  /* voicer 27 */
#define DONGLE_PROT_VOICE_Voicer28  0x08000000  /* voicer 28 */
#define DONGLE_PROT_VOICE_Voicer29  0x10000000  /* voicer 29 */
#define DONGLE_PROT_VOICE_Voicer30  0x20000000  /* voicer 30 */
#define DONGLE_PROT_VOICE_Voicer31  0x40000000  /* voicer 31 */
#define DONGLE_PROT_VOICE_Voicer32  0x80000000  /* voicer 32 */
#define DONGLE_PROT_VOICE_ALL       0xFFFFFFFF  /* All voices */


/* ��Ʒ������Ϣ�ӿ� -- ���ӿڲ�֧�ֶ��߳� */
struct IDongle_Prod_Prot_Info
{
    /* ������Ϣ�ṹΪȱʡֵ */
    virtual void reset(void) = 0;

    /* ����\���ò�Ʒ���� */
    virtual const char * prod_name(void) = 0;
    virtual void prod_name(const char * name) = 0;

    /* ����\����������� */
    virtual const char * prod_number(void) = 0;
    virtual void prod_number(const char * number) = 0;

    /* ����\���ò�Ʒ�汾 */
    virtual void prod_ver(int * major, int * minor, int * build) = 0;
    virtual void prod_ver(int major, int minor, int build) = 0;

    /* ����\������Ȩ���� */
    virtual unsigned long licence(void) = 0;
    virtual void licence(unsigned long lic) = 0;

    /* ����\���ù�������ʹ�� */
    virtual bool expired_time_enable(void) = 0;
    virtual void expired_time_enable(bool enable) = 0;

    /* ����\���ù������� */
    virtual time32_t expired_time(void) = 0;
    virtual void expired_time(time32_t timer) = 0;

    /* ����\�������д���ʹ�� */
    virtual bool run_count_enable(void) = 0;
    virtual void run_count_enable(bool enable) = 0;

    /* ����\�������д��� */
    virtual unsigned short run_count(void) = 0;
    virtual void run_count(unsigned short count) = 0;
	virtual unsigned short dec_run_count(short count) = 0;

    /* ����\��������ʱ��ʹ�� */
    virtual bool run_time_enable(void) = 0;
    virtual void run_time_enable(bool enable) = 0;

    /* ����\��������ʱ�� */
    virtual unsigned short run_time(void) = 0;
    virtual void run_time(unsigned short hour) = 0;
	virtual unsigned short dec_run_time(short time) = 0;

    /* ����\����\���\ɾ��\������Ȩ���� */
    virtual unsigned int voices(void) = 0;
    virtual void voices(unsigned int vocs) = 0;
    virtual bool is_voice_supp(unsigned int voice) = 0;
    virtual void add_voice(unsigned int voice) = 0;
    virtual void del_voice(unsigned int voice) = 0;
    virtual const char * voice_desc(unsigned int voice) = 0;

    /* ����\����\���\ɾ��\��Ȩ��Դ�� */
    virtual const char * packages(void) = 0;
    virtual void packages(const char * packs) = 0;
	virtual bool is_package_supp(const char * pack) = 0;
	virtual void add_package(const char * pack) = 0;
    virtual void del_package(const char * pack) = 0;
};

/* Intp40������Ϣ�ӿ� -- ���ӿڲ�֧�ֶ��߳� */
struct IDongle_Intp40_Prot_Info
{
    /* ������Ϣ�ṹΪȱʡֵ */
    virtual void reset(void) = 0;

    /* ��ȡ��Ʒ������Ϣ����Ŀ */
    virtual int prod_count(void) = 0;

    /* ��ȡָ��λ�õĲ�Ʒ�ı�����Ϣ�ӿ� */
    virtual IDongle_Prod_Prot_Info * get_prod(int idx) = 0;

    /* ���һ����Ʒ�ı�����Ϣ */
    virtual IDongle_Prod_Prot_Info * add_prod(void) = 0;

    /* ɾ��һ����Ʒ�ı�����Ϣ */
    virtual bool del_prod(int idx) = 0;
    virtual bool del_prod(IDongle_Prod_Prot_Info * prod) = 0;
};


/* Intp35������Ϣ�ӿ� -- ���ӿڲ�֧�ֶ��߳� */
struct IDongle_Intp35_Prot_Info
{
    /* ������Ϣ�ṹΪȱʡֵ */
    virtual void reset(void) = 0;

    /* ����\���ò�Ʒ���� */
    virtual const char * prod_name(void) = 0;
    virtual void prod_name(const char * name) = 0;

    /* ����\����������� */
    virtual const char * prod_number(void) = 0;
    virtual void prod_number(const char * number) = 0;

    /* ����\���ò�Ʒ��� */
    virtual unsigned long prod_id(void) = 0;
    virtual void prod_id(unsigned long id) = 0;

    /* ����\���ò�Ʒ�汾 */
    virtual void prod_ver(int * major, int * minor, int * build) = 0;
    virtual void prod_ver(int major, int minor, int build) = 0;

    /* ����\������Ȩ���� */
    virtual unsigned long licence(void) = 0;
    virtual void licence(unsigned long lic) = 0;

    /* ����\���ù������� */
    virtual time32_t expired_time(void) = 0;
    virtual void expired_time(time32_t timer) = 0;

    /* ����\��������˵�� */
    virtual const char * spec_desc(void) = 0;
    virtual void spec_desc(const char * desc) = 0;

    /* ����\������Ȩ��Դ�� */
    virtual const char * packages(void) = 0;
    virtual void packages(const char packs[32]) = 0;
	virtual bool is_package_supp(const char * pack) = 0;
};

/* ���ܹ�������Ϣ�ӿ� -- ���ӿڲ�֧�ֶ��߳� */
struct IDongle_Prot_Info
{
    /* ��Ϣ�ṹ�İ汾 */
    virtual unsigned int version(void) = 0;

    /* ������Ϣ�ṹΪȱʡֵ */
    virtual void reset(unsigned int version) = 0;

    /* ռ�ô洢�ռ��С */
    virtual int info_size(void) = 0;

    /* ����Intp35������Ϣ�ӿ� */
    virtual IDongle_Intp35_Prot_Info * intp35(void) = 0;

    /* ����Intp40������Ϣ�ӿ� */
    virtual IDongle_Intp40_Prot_Info * intp40(void) = 0;

	/* ��ȡ���ܹ��������Ϣ��(����Զ�̸���) */
	virtual int get_update_pack(char *buf, int *buf_len) = 0;

	/* Ӧ�ü��ܹ��������Ϣ��(����Զ�̸���) */
	virtual int set_update_pack(const char *buf, int buf_len) = 0;
};

/* ���ܹ�Ӳ����Ϣ�ӿ� -- ���ӿ�֧�ֶ��߳� */
struct IDongle_Hw_Info
{
    /* ����(���ַ�������) */
    virtual Dongle_Producer producer(void) = 0;
    virtual const char * producer_desc(void) = 0;

    /* �����ӿ�(���ַ�������) */
    virtual Dongle_Ele_Type electric_type(void) = 0;
    virtual const char * eletric_type_desc(void) = 0;

    /* �ͺ�(���ַ�������) */
    virtual Dongle_Model model(void) = 0;
    virtual const char * model_desc(void) = 0;

    /* ���ܹ�ID */
    virtual unsigned int id(void) = 0;

    /* �ڴ����� */
    virtual int memory(void) = 0; 

    /* �Ƿ�������ʱ�� */
    virtual bool real_clock(void) = 0;
};

/* ���ܹ��ӿ� -- ���ӿ�֧�ֶ��߳� */
struct IDongle
{
    /* ����Ӳ����Ϣ�ӿ� */
    virtual IDongle_Hw_Info * hw_info(void) = 0;

    /* ���ر�����Ϣ�ӿ� */
    virtual IDongle_Prot_Info * prot_info(void) = 0;

    /* ȡ�������������Ӧ�� */
    virtual int get_response(const char * query, int query_len, char * response, int * response_len) = 0;

    /* ���¶�ȡ\д����ܹ�������Ϣ */
    virtual bool update_data(bool save_to_dongle) = 0;

    /* ��ȡ\���ü��ܹ�ʵʱʱ�� */
    virtual int get_real_clock(time32_t * timer) = 0;
    virtual int set_real_clock(const time32_t timer) = 0;

	/* ��ȡ\Ӧ�ü��ܹ��������Ϣ��(����Զ�̸���) */
	virtual int get_update_pack(char *buf, int *buf_len) = 0;
	virtual int set_update_pack(const char *buf, int buf_len) = 0;
};

/* ���ܹ��������ӿ� -- ���ӿ�֧�ֶ��߳� */
struct IDongle_Mngr
{
    /* ���������ӿڵİ汾�� */
    virtual unsigned int version(void) = 0;

    /* ����������ʼ�� */
    virtual int init(void * para) = 0;

    /* �����������ʼ�� */
    virtual int fini(void) = 0;

    /** 
     * @brief   find_first_dongle
     *
     * Find the first dongle(���ص�һ�����ܹ��Ľӿ�)
     *
     * @return  Dongle_SAL        - Return the reference of the first dongle be find, NULL means not found.
     * @param   int * find_handle - [out] Pointer on a search handle to be used in subsequent calls to find_next_dongle
     * @see     find_next_dongle, find_close
     */
    virtual IDongle * find_first_dongle(int * find_handle) = 0;

    /** 
     * @brief   find_next_dongle
     * 
     * Find the next dongle(������һ�����ܹ��Ľӿ�)
     *
     * @return  Dongle_SAL      - Return the reference to the dongle be find, NULL means not found.
     * @param   int find_handle - [in] Handle obtained by a call to find_first_dongle
     * @see     find_first_dongle, find_close
     */
    virtual IDongle * find_next_dongle(int find_handle) = 0;

    /** 
     * @brief    find_close
     * 
     * This function closes the search handle used to find a dongle(�رղ�ѯ���)
     * 
     * @return  int - Return 0 in success, otherwise return error code.
     * @param   int find_handle - [in] Handle obtained by a call to find_first_dongle
     * @see     find_first_dongle, find_next_dongle
     */
    virtual int find_close(int find_handle) = 0;
};

/* �������ܹ�����ӿ� */
DGLLIBAPI IDongle_Mngr* create_dongle_mngr(void);
typedef  IDongle_Mngr*(*proc_create_dongle_mngr)(void);

/* ���ټ��ܹ�����ӿ� */
DGLLIBAPI void destroy_dongle_mngr(IDongle_Mngr *dongle_mngr);
typedef void (*proc_destroy_dongle_mngr)(IDongle_Mngr *dongle_mngr);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif	// _IFLY_DONGLE_H_
